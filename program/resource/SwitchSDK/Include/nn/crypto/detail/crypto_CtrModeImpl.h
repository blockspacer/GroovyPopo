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

#include "../crypto_AesEncryptor.h"
#include <cstring>
#include <algorithm>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/crypto/detail/crypto_Clear.h>

namespace nn { namespace crypto { namespace detail {

template <typename BlockCipher>
class CtrModeImpl
{
public:
    static const size_t BlockSize = BlockCipher::BlockSize;
    static const size_t IvSize = BlockSize;

public:
    CtrModeImpl() NN_NOEXCEPT : m_State(State_None) {}
    ~CtrModeImpl() NN_NOEXCEPT
    {
        ClearMemory(this, sizeof(*this));
    }

    void   Initialize(const BlockCipher* pBlockCipher, const void* pIv, size_t ivSize) NN_NOEXCEPT;
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT;
    void   IncrementCounter() NN_NOEXCEPT;

private:
    void   ProcessBlocks(uint8_t* pDst8, const uint8_t* pSrc8, int numBlocks) NN_NOEXCEPT;
    void   ProcessBlock(uint8_t* pDst8, const uint8_t* pSrc8, size_t size) NN_NOEXCEPT;

private:
    enum State
    {
        State_None,
        State_Initialized,
    };

private:
    const BlockCipher* m_pBlockCipher;
    uint8_t            m_Counter[IvSize];
    uint8_t            m_EncryptedCounter[BlockSize];
    size_t             m_ProcessedByte;
    State              m_State;
};

template <typename BlockCipher>
inline void CtrModeImpl<BlockCipher>::Initialize(const BlockCipher* pBlockCipher,
                                                 const void* pIv, size_t ivSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(ivSize == IvSize);

    m_pBlockCipher = pBlockCipher;
    std::memcpy(m_Counter, pIv, ivSize);
    m_ProcessedByte = 0;

    m_State = State_Initialized;
}

template <typename BlockCipher>
size_t CtrModeImpl<BlockCipher>::Update(void* pDst, size_t dstSize,
                                               const void* pSrc, size_t srcSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(m_State == State_Initialized, "Invalid state. Please restart from Initialize().");
    NN_SDK_REQUIRES(dstSize >= srcSize);
    NN_UNUSED(dstSize);

    const uint8_t* pSrc8 = static_cast<const uint8_t*>(pSrc);
    uint8_t* pDst8 = static_cast<uint8_t*>(pDst);
    size_t remaining = srcSize;

    /* 前の処理の残りがあったら1ブロックに到達するかデータが無くなるまで暗号化済カウンタと XOR を取る */
    if (m_ProcessedByte > 0)
    {
        size_t fillSize = std::min(BlockSize - m_ProcessedByte, remaining);

        uint8_t* pEncryptedCounter = m_EncryptedCounter + m_ProcessedByte;
        for (int i = 0; i < static_cast<int>(fillSize); ++i)
        {
            pDst8[i] = pSrc8[i] ^ pEncryptedCounter[i];
        }
        pSrc8 += fillSize;
        pDst8 += fillSize;
        remaining -= fillSize;
        m_ProcessedByte += fillSize;

        if (m_ProcessedByte == BlockSize)
        {
            m_ProcessedByte = 0;
        }
    }

    if (remaining >= BlockSize)
    {
        int numBlocks = static_cast<int>(remaining / BlockSize);

        ProcessBlocks(pDst8, pSrc8, numBlocks);
        pDst8     += numBlocks * BlockSize;
        pSrc8     += numBlocks * BlockSize;
        remaining -= numBlocks * BlockSize;
    }

    /* ブロックサイズ以下の端数が出たらカウンタを更新して途中まで XOR を取る */
    if (remaining > 0)
    {
        ProcessBlock(pDst8, pSrc8, remaining);

        m_ProcessedByte = remaining;
    }

    /* この関数では入力データサイズ分は必ず処理される */
    return srcSize;
}

template <typename BlockCipher>
inline void CtrModeImpl<BlockCipher>::ProcessBlocks(uint8_t* pDst8, const uint8_t* pSrc8, int numBlocks) NN_NOEXCEPT
{
    while (numBlocks--)
    {
        ProcessBlock(pDst8, pSrc8, BlockSize);
        pSrc8 += BlockSize;
        pDst8 += BlockSize;
    }
}

/* CTR モード暗号化/復号化の本体実装(1ブロック分の処理) */
template <typename BlockCipher>
inline void CtrModeImpl<BlockCipher>::ProcessBlock(uint8_t* pDst8, const uint8_t* pSrc8, size_t srcSize) NN_NOEXCEPT
{
    /* カウンタを暗号化して更新する */
    m_pBlockCipher->EncryptBlock(m_EncryptedCounter, BlockSize, m_Counter, IvSize);
    IncrementCounter();

    /* 指定サイズ分だけ入力と XOR を取る */
    for (int i = 0; i < static_cast<int>(srcSize); ++i)
    {
        pDst8[i] = pSrc8[i] ^ m_EncryptedCounter[i];
    }
}

/* 指定されたバイト列を符号なし整数としてインクリメントする */
template <typename BlockCipher>
inline void CtrModeImpl<BlockCipher>::IncrementCounter() NN_NOEXCEPT
{
    for (int i = IvSize - 1; i >= 0; --i)
    {
        if (++m_Counter[i] != 0)
        {
            break;
        }
    }
}

// template specialization
template <>
void CtrModeImpl<AesEncryptor128>::ProcessBlocks(uint8_t* pDst8, const uint8_t* pSrc8, int numBlocks) NN_NOEXCEPT;

}}} // namespace nn::crypto::detail
