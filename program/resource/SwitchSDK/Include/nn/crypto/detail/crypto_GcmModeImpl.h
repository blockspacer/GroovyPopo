/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_CtrModeImpl.h>
#include <nn/crypto/detail/crypto_GhashImpl.h>
#include <nn/crypto/detail/crypto_Clear.h>

namespace nn { namespace crypto {

namespace detail {

template <typename BlockCipher>
class GcmModeImpl
{
public:
    static const size_t BlockSize = 16;

public:
    GcmModeImpl() NN_NOEXCEPT : m_State(State_None) {}

    ~GcmModeImpl() NN_NOEXCEPT
    {
        ClearMemory(this, sizeof(*this));
    }

    void Initialize(const BlockCipher& pBlockCipher) NN_NOEXCEPT
    {
        m_pBlockCipher = &pBlockCipher;
        m_GhashImpl.Initialize(m_pBlockCipher);
        m_State = State_Initialized;
    }

    void Reset(const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State >= State_Initialized, "Invalid state. Please call Initialize().");

        m_GhashImpl.Reset();

        std::memset(m_Mac, 0x00, sizeof(m_Mac));
        if (ivSize == 12)
        {
            std::memcpy(m_Mac, pIv, 12);
            m_Mac[15] = 1;
        }
        else
        {
            m_GhashImpl.Update(pIv, ivSize);
            m_GhashImpl.GetGhash(m_Mac, sizeof(m_Mac));
            m_GhashImpl.Reset();
        }

        m_CtrModeImpl.Initialize(m_pBlockCipher, m_Mac, sizeof(m_Mac));
        m_pBlockCipher->EncryptBlock(m_Mac, BlockSize, m_Mac, BlockSize);

        // 初期カウンタ値は暗号化では利用しない
        m_CtrModeImpl.IncrementCounter();

        m_State = State_ProcessingAad;
    }

    void UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_ProcessingAad, "Invalid state. Please restart from Reset().");
        NN_SDK_REQUIRES(m_GhashImpl.GetMessageSize() + aadSize <= (Bit64(1) << 61));
        NN_SDK_REQUIRES(m_GhashImpl.GetMessageSize() + aadSize >= aadSize); // Check for overflowing AAD length

        m_GhashImpl.Update(pAad, aadSize);
    }

    size_t UpdateEncryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_ProcessingAad || m_State == State_ProcessingData,
                        "Invalid state. Please restart from Reset().");

        if (m_State == State_ProcessingAad)
        {
            m_GhashImpl.ProcessPadding();
            m_GhashImpl.SwitchMessageSizeDigit();

            m_State = State_ProcessingData;
        }

        NN_SDK_REQUIRES(m_GhashImpl.GetMessageSize() + srcSize <= (Bit64(1) << 36) - 32);
        NN_SDK_REQUIRES(m_GhashImpl.GetMessageSize() + srcSize >= srcSize);

        size_t processed = m_CtrModeImpl.Update(pDst, dstSize, pSrc, srcSize);
        m_GhashImpl.Update(pDst, srcSize);
        return processed;
    }

    size_t UpdateDecryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_ProcessingAad || m_State == State_ProcessingData,
                        "Invalid state. Please restart from Reset().");

        if (m_State == State_ProcessingAad)
        {
            m_GhashImpl.ProcessPadding();
            m_GhashImpl.SwitchMessageSizeDigit();

            m_State = State_ProcessingData;
        }

        m_GhashImpl.Update(pSrc, srcSize);
        return m_CtrModeImpl.Update(pDst, dstSize, pSrc, srcSize);
    }

    void GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES((m_State >= State_ProcessingAad) && (m_State <= State_Done),
                        "Invalid state. Please restart from Reset().");
        NN_SDK_REQUIRES(macSize >= BlockSize);
        NN_UNUSED(macSize);

        if (m_State != State_Done)
        {
            if (m_State == State_ProcessingAad)
            {
                m_GhashImpl.ProcessPadding();
                m_GhashImpl.SwitchMessageSizeDigit();
            }

            uint8_t tmp[BlockSize];
            m_GhashImpl.GetGhash(tmp, sizeof(tmp));

            for (int i = 0; i < 16; i++)
            {
                m_Mac[i] ^= tmp[i];
            }

            // swipe out stack
            ClearMemory(tmp, sizeof(tmp));

            m_State = State_Done;
        }

        std::memcpy(pMac, m_Mac, BlockSize);
    }

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_ProcessingAad,
        State_ProcessingData,
        State_Done,
    };

private:
    uint8_t                  m_Mac[BlockSize];
    const BlockCipher*       m_pBlockCipher;
    GhashImpl                m_GhashImpl;
    CtrModeImpl<BlockCipher> m_CtrModeImpl;
    State                    m_State;
};

}}} // namespace nn::crypto::detail
