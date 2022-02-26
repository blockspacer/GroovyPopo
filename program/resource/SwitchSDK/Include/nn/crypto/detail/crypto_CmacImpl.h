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

#include <cstring>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/crypto/detail/crypto_CbcMacImpl.h>
#include <nn/crypto/detail/crypto_Clear.h>

namespace nn { namespace crypto { namespace detail {

template <typename BlockCipher>
class CmacImpl
{
public:
    static const size_t BlockSize = 16;

public:
    CmacImpl() NN_NOEXCEPT : m_State(State_None) {}
    ~CmacImpl() NN_NOEXCEPT
    {
        ClearMemory(this, sizeof(*this));
    }

    void   Initialize(const BlockCipher* pBlockCipher) NN_NOEXCEPT;
    void   Update(const void* pData, size_t dataSize) NN_NOEXCEPT;
    void   GetMac(void* pMac, size_t macSize) NN_NOEXCEPT;

private:
    void   MultiplyOneOverGF128(uint8_t* pData) NN_NOEXCEPT
    {
        // 先にシフトしたときに桁あふれするか調べておく
        uint8_t carry = pData[0] & 0x80;

        // 全体を1ビット左シフト
        for (int i = 0; i < static_cast<int>(BlockSize) - 1; i++)
        {
            pData[i] = pData[i] << 1 | pData[i + 1] >> 7;
        }
        pData[BlockSize - 1] <<= 1;

        if (carry)
        {
            // 最上位に桁上りがあったら最下位バイトに 0x87 を XOR する
            // a^128 = a^7 + a^2 + a^1 + 1 より
            pData[BlockSize - 1] ^= 0x87;
        }
    }

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done
    };

private:
    State      m_State;
    uint8_t    m_SubKey[BlockSize];
    CbcMacImpl m_CbcMacImpl;

};

template <typename BlockCipher>
inline void CmacImpl<BlockCipher>::Initialize(const BlockCipher* pBlockCipher) NN_NOEXCEPT
{
    /* SubKey(K1) を計算する */
    std::memset(m_SubKey, 0x00, BlockSize);

    pBlockCipher->EncryptBlock(m_SubKey, BlockSize, m_SubKey, BlockSize);

    MultiplyOneOverGF128(m_SubKey);

    /* CBC-MAC 計算の初期化 */
    m_CbcMacImpl.Initialize(pBlockCipher);

    m_State = State_Initialized;
}

template <typename BlockCipher>
inline void CmacImpl<BlockCipher>::Update(const void* pData, size_t dataSize) NN_NOEXCEPT
{
    m_CbcMacImpl.template Update<BlockCipher>(pData, dataSize);
}

template <typename BlockCipher>
inline void CmacImpl<BlockCipher>::GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
{
    if (m_State == State_Initialized)
    {
        if (m_CbcMacImpl.GetBufferedDataSize() != BlockSize)
        {
            /* ブロックサイズちょうどではない場合はパディングを付加し、K2 を XOR する */
            size_t  remaining = BlockSize - m_CbcMacImpl.GetBufferedDataSize();

            /* パディングは 0x80, 0x00, ... のバイト列 */
            static const uint8_t padding[BlockSize] = {0x80 /*, 0x00, 0x00, ... */ };
            m_CbcMacImpl.template Update<BlockCipher>(padding, remaining);

            /* K2 を計算して SubKey を更新 */
            MultiplyOneOverGF128(m_SubKey);
        }

        /* SubKey の値を最後のブロックに XOR */
        m_CbcMacImpl.MaskBufferedData(m_SubKey, BlockSize);

        m_State = State_Done;
    }

    m_CbcMacImpl.GetMac(pMac, macSize);
}

}}} // namespace nn::crypto::detail
