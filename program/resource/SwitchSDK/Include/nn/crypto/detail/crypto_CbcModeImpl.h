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
#include <algorithm>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_Abort.h>
#include <nn/crypto/detail/crypto_Clear.h>
#include "../crypto_AesDecryptor.h"
#include "../crypto_AesEncryptor.h"

namespace nn { namespace crypto { namespace detail {

template <typename BlockCipher>
class CbcModeImpl
{
public:
    static const int BlockSize = BlockCipher::BlockSize;
    static const int IvSize = BlockSize;

public:
    CbcModeImpl() NN_NOEXCEPT : m_State(State_None) {}
    ~CbcModeImpl() NN_NOEXCEPT
    {
        ClearMemory(this, sizeof(*this));
    }

    void   Initialize(const BlockCipher* pBlockCipher, const void* pIv, size_t ivSize) NN_NOEXCEPT;
    size_t UpdateEncryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT;
    size_t UpdateDecryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT;
    size_t GetBufferedDataSize() const NN_NOEXCEPT
    {
        return m_BufferedByte;
    }

private:
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize,
                  void (*pProcessBlocks)(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const BlockCipher* pBlockCipher)) NN_NOEXCEPT;
    static void EncryptBlocks(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const BlockCipher* pBlockCipher) NN_NOEXCEPT;
    static void DecryptBlocks(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const BlockCipher* pBlockCipher) NN_NOEXCEPT;

private:
    enum State
    {
        State_None,
        State_Initialized,
    };

private:
    const BlockCipher* m_pBlockCipher;
    uint8_t            m_Iv[IvSize];
    uint8_t            m_TemporalBlockBuffer[BlockSize];
    size_t             m_BufferedByte;
    State              m_State;
};

template <typename BlockCipher>
inline void CbcModeImpl<BlockCipher>::Initialize(const BlockCipher* pBlockCipher, const void* pIv, size_t ivSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(ivSize == IvSize);

    m_pBlockCipher = pBlockCipher;
    std::memcpy(m_Iv, pIv, ivSize);
    m_BufferedByte = 0;

    m_State = State_Initialized;
}

template <typename BlockCipher>
inline size_t CbcModeImpl<BlockCipher>::UpdateEncryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
{
    NN_ABORT_UNLESS(dstSize >= ((srcSize + GetBufferedDataSize()) / BlockSize) * BlockSize, "Precondition does not met.");
    NN_SDK_REQUIRES(m_State == State_Initialized, "Invalid state. Please restart from Initialize().");

    return Update(pDst, dstSize, pSrc, srcSize, &EncryptBlocks);
}

template <typename BlockCipher>
inline size_t CbcModeImpl<BlockCipher>::UpdateDecryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
{
    NN_ABORT_UNLESS(dstSize >= ((srcSize + GetBufferedDataSize()) / BlockSize) * BlockSize, "Precondition does not met.");
    NN_SDK_REQUIRES(m_State == State_Initialized, "Invalid state. Please restart from Initialize().");

    return Update(pDst, dstSize, pSrc, srcSize, &DecryptBlocks);
}

template <typename BlockCipher>
inline size_t CbcModeImpl<BlockCipher>::Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize,
                                               void (*pProcessBlocks)(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const BlockCipher* pBlockCipher)) NN_NOEXCEPT
{
    NN_UNUSED(dstSize);

    const uint8_t* pSrc8 = static_cast<const uint8_t*>(pSrc);
    uint8_t* pDst8 = static_cast<uint8_t*>(pDst);
    size_t remaining = srcSize;
    size_t processed = 0;

    /* 前の処理の残りがあったら1ブロックに到達するかデータが無くなるまで埋める */
    if (m_BufferedByte > 0)
    {
        size_t fillSize = std::min(BlockSize - m_BufferedByte, remaining);

        std::memcpy(m_TemporalBlockBuffer + m_BufferedByte, pSrc8, fillSize);
        pSrc8 += fillSize;
        remaining -= fillSize;
        m_BufferedByte += fillSize;

        if (m_BufferedByte == BlockSize)
        {
            pProcessBlocks(pDst8, m_Iv, m_TemporalBlockBuffer, 1, m_pBlockCipher);
            processed += BlockSize;
            pDst8 += BlockSize;
            m_BufferedByte = 0;
        }
    }

    /* ブロックサイズ以上の残りがある場合はブロックごとに処理 */
    if (remaining >= BlockSize)
    {
        int numBlocks = static_cast<int>(remaining / BlockSize);

        pProcessBlocks(pDst8, m_Iv, pSrc8, numBlocks, m_pBlockCipher);
        pSrc8 += numBlocks * BlockSize;
        remaining -= numBlocks * BlockSize;
        processed += numBlocks * BlockSize;
    }

    /* ブロックサイズ以下の端数が出たら次の処理のために保存しておく */
    if (remaining > 0)
    {
        m_BufferedByte = remaining;
        std::memcpy(m_TemporalBlockBuffer, pSrc8, remaining);
    }

    return processed;
}

/*
 * CBC モード暗号化の本体実装
 */
template <typename BlockCipher>
inline void CbcModeImpl<BlockCipher>::EncryptBlocks(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const BlockCipher* pEncryptor) NN_NOEXCEPT
{
    const uint8_t* pCurrentIv8 = pIv8;
    uint8_t  tmp[BlockSize];

    while (numBlocks--)
    {
        /* IV と入力の XOR を取り */
        for (int i = 0; i < static_cast<int>(BlockSize); i++)
        {
            tmp[i] = pSrc8[i] ^ pCurrentIv8[i];
        }

        /* 暗号化する */
        pEncryptor->EncryptBlock(pDst8, BlockSize, tmp, BlockSize);

        /* 暗号結果が次の IV となる */
        pCurrentIv8 = pDst8;
        pSrc8 += BlockSize;
        pDst8 += BlockSize;
    }

    if (pIv8 != pCurrentIv8)
    {
        /* 次の IV を保存する */
        std::memcpy(pIv8, pCurrentIv8, BlockSize);
    }
}

/*
 * CBC モード復号化の本体実装
 */
template <typename BlockCipher>
inline void CbcModeImpl<BlockCipher>::DecryptBlocks(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const BlockCipher* pDecryptor) NN_NOEXCEPT
{
    uint8_t nextIv[BlockSize];

    /* 最後の暗号文ブロックが次の IV になるので、処理前に退避しておく */
    std::memcpy(nextIv, pSrc8 + ((numBlocks - 1) * BlockSize), BlockSize);

    if (pDst8 == pSrc8) // In-Place 復号
    {
        /* 復号化によって IV となるべき前の暗号文ブロックが破壊されるので後ろから復号化する */
        pSrc8 = pSrc8 + (BlockSize * (numBlocks - 1));
        pDst8 = pDst8 + (BlockSize * (numBlocks - 1));
        const uint8_t* pCurrentIv8 = (numBlocks == 1)? pIv8: pSrc8 - BlockSize;

        /* 最後のブロック以外を処理する */
        while (numBlocks-- > 1)
        {
            /* 復号化して */
            pDecryptor->DecryptBlock(pDst8, BlockSize, pSrc8, BlockSize);

            /* IV と入力の XOR を取る */
            for (int i = 0; i < static_cast<int>(BlockSize); i++)
            {
                pDst8[i] = pDst8[i] ^ pCurrentIv8[i];
            }

            /* 1つ前の暗号文が次の IV となる */
            pCurrentIv8  -= BlockSize;
            pSrc8 -= BlockSize;
            pDst8 -= BlockSize;
        }

        /* 最後のブロックを復号化して */
        pDecryptor->DecryptBlock(pDst8, BlockSize, pSrc8, BlockSize);

        /* IV と入力の XOR を取る */
        for (int i = 0; i < static_cast<int>(BlockSize); i++)
        {
            pDst8[i] = pDst8[i] ^ pIv8[i];
        }
    }
    else // Out-Place 復号
    {
        const uint8_t* pCurrentIv8 = pIv8;

        while (numBlocks--)
        {
            /* 復号化して */
            pDecryptor->DecryptBlock(pDst8, BlockSize, pSrc8, BlockSize);

            /* IV と入力の XOR を取る */
            for (int i = 0; i < static_cast<int>(BlockSize); i++)
            {
                pDst8[i] = pDst8[i] ^ pCurrentIv8[i];
            }

            /* 暗号文が次の IV となる */
            pCurrentIv8 = pSrc8;
            pSrc8 += BlockSize;
            pDst8 += BlockSize;
        }
    }

    /* 退避しておいた IV を保存する */
    std::memcpy(pIv8, nextIv, BlockSize);
}

/* template specialization */
template <>
void CbcModeImpl<AesEncryptor128>::EncryptBlocks(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const AesEncryptor128* pEncryptor) NN_NOEXCEPT;
template <>
void CbcModeImpl<AesDecryptor128>::DecryptBlocks(uint8_t* pDst8, uint8_t* pIv8, const uint8_t* pSrc8, int numBlocks, const AesDecryptor128* pDecryptor) NN_NOEXCEPT;

}}} // namespace nn::crypto::detail
