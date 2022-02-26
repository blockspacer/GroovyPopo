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
#include <nn/crypto/detail/crypto_CtrModeImpl.h>
#include <nn/crypto/detail/crypto_CbcMacImpl.h>
#include <nn/crypto/detail/crypto_Clear.h>

namespace nn { namespace crypto { namespace detail {

template <typename BlockCipher>
class CcmModeImpl
{
public:
    static const size_t BlockSize = BlockCipher::BlockSize;

public:
    CcmModeImpl() NN_NOEXCEPT : m_State(State_None) {}
    ~CcmModeImpl() NN_NOEXCEPT
    {
        ClearMemory(this, sizeof(*this));
    }

    void   Initialize(const BlockCipher* pBlockCipher,
                      const void* pNonce, size_t nonceSize,
                      int64_t aadSize, int64_t dataSize, size_t macSize) NN_NOEXCEPT;
    void   UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT;
    size_t UpdateEncryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT;
    size_t UpdateDecryption(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT;
    void   GetMac(void* pMac, size_t macSize) NN_NOEXCEPT;

private:
    void   MakeInitialCounter(void* pCounter, const void* pNonce, size_t nonceSize) const NN_NOEXCEPT;
    void   MakeHeaderBlock(void* pBlock, const void* pNonce, size_t nonceSize,
                           int64_t aadSize, int64_t dataSize, size_t macSize) const NN_NOEXCEPT;
    void   ProcessEncodedAadSize() NN_NOEXCEPT;
    void   ProcessPadding() NN_NOEXCEPT;
    void   GenerateMac() NN_NOEXCEPT;

private:
    enum State
    {
        State_None,
        State_ProcessingAad,
        State_ProcessingData,
        State_DataInputDone,
        State_Done
    };

private:
    uint8_t                  m_Mac[BlockSize];
    int64_t                  m_GivenDataSize;
    int64_t                  m_GivenAadSize;
    size_t                   m_GivenMacSize;
    int64_t                  m_ProcessedDataSize;
    int64_t                  m_ProcessedAadSize;
    State                    m_State;
    CtrModeImpl<BlockCipher> m_CtrModeImpl; // 暗号化は CTR モードで行う
    CbcMacImpl               m_CbcMacImpl;  // 認証は CBC-MAC で行う

};

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::Initialize(const BlockCipher* pBlockCipher,
                                                 const void* pNonce, size_t nonceSize,
                                                 int64_t aadSize, int64_t dataSize, size_t macSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(nonceSize >= 7 && nonceSize <= 13);
    NN_SDK_REQUIRES(macSize >= 4 && macSize <= 16 && macSize % 2 == 0);
    NN_SDK_REQUIRES(aadSize >= 0);
    NN_SDK_REQUIRES(dataSize >= 0);
    NN_SDK_REQUIRES((nonceSize == 7) ? dataSize <= INT64_MAX : dataSize < (1LL << ((15 - nonceSize) * 8)));

    uint8_t tmp[BlockSize];

    /* メンバ変数の初期化 */
    m_GivenAadSize = aadSize;
    m_GivenDataSize = dataSize;
    m_GivenMacSize = macSize;
    m_ProcessedAadSize = 0;
    m_ProcessedDataSize = 0;

    /* Nonce から初期カウンタ値を生成する */
    MakeInitialCounter(tmp, pNonce, nonceSize);

    /* 暗号化した初期カウンタ値を MAC 計算のために保存しておく */
    pBlockCipher->EncryptBlock(m_Mac, BlockSize, tmp, BlockSize);

    /* 暗号化は初期カウンタ +1 から開始する */
    m_CtrModeImpl.Initialize(pBlockCipher, tmp, BlockSize);
    m_CtrModeImpl.IncrementCounter();

    /* 各パラメータから認証に入力する先頭のブロックを生成する */
    MakeHeaderBlock(tmp, pNonce, nonceSize, aadSize, dataSize, macSize);

    /* 作成した先頭のブロックを入力して MAC 計算を開始 */
    m_CbcMacImpl.Initialize(pBlockCipher);
    m_CbcMacImpl.template Update<BlockCipher>(tmp, BlockSize);

    if (aadSize > 0)
    {
        /* AAD があったらエンコードされた AAD のサイズを AAD の前に MAC 計算 */
        ProcessEncodedAadSize();
        m_State = State_ProcessingAad;
    }
    else
    {
        m_State = State_ProcessingData;
    }
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(m_State == State_ProcessingAad, "Invalid state. Please restart from Initialize().");
    NN_SDK_REQUIRES(m_ProcessedAadSize + static_cast<int64_t>(aadSize) <= m_GivenAadSize);

    m_CbcMacImpl.template Update<BlockCipher>(pAad, aadSize);
    m_ProcessedAadSize += aadSize;

    if (m_ProcessedAadSize == m_GivenAadSize)
    {
        /* 最後のブロックにはブロックサイズの倍数になるように 0 を付加する */
        ProcessPadding();

        if (m_GivenDataSize > 0)
        {
            m_State = State_ProcessingData;
        }
        else
        {
            m_State = State_DataInputDone;
        }
    }
}

template <typename BlockCipher>
inline size_t CcmModeImpl<BlockCipher>::UpdateEncryption(void* pDst, size_t dstSize,
                                                         const void* pSrc, size_t srcSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(m_State == State_ProcessingData, "Invalid state. Please restart from Initialize().");
    NN_SDK_REQUIRES(m_ProcessedDataSize + static_cast<int64_t>(srcSize) <= m_GivenDataSize);
    NN_SDK_REQUIRES(dstSize >= srcSize);

    size_t processed;

    /* 平文の MAC 計算 */
    m_CbcMacImpl.template Update<BlockCipher>(pSrc, srcSize);

    /* 暗号化は CTR モードで行う */
    processed = m_CtrModeImpl.Update(pDst, dstSize, pSrc, srcSize);
    m_ProcessedDataSize += srcSize;

    if (m_ProcessedDataSize == m_GivenDataSize)
    {
        /* 最後のブロックにはブロックサイズの倍数になるように 0 を付加する */
        ProcessPadding();

        m_State = State_DataInputDone;
    }

    return processed;
}

template <typename BlockCipher>
inline size_t CcmModeImpl<BlockCipher>::UpdateDecryption(void* pDst, size_t dstSize,
                                                         const void* pSrc, size_t srcSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(m_State == State_ProcessingData, "Invalid state. Please restart from Initialize().");
    NN_SDK_REQUIRES(m_ProcessedDataSize + static_cast<int64_t>(srcSize) <= m_GivenDataSize);
    NN_SDK_REQUIRES(dstSize >= srcSize);

    size_t processed;

    /* 復号化は CTR モードで行う */
    processed = m_CtrModeImpl.Update(pDst, dstSize, pSrc, srcSize);
    m_ProcessedDataSize += srcSize;

    /* 復号化された平文の MAC を計算 */
    m_CbcMacImpl.template Update<BlockCipher>(pDst, processed);

    if (m_ProcessedDataSize == m_GivenDataSize)
    {
        /* 最後のブロックにはブロックサイズの倍数になるように 0 を付加する */
        ProcessPadding();

        m_State = State_DataInputDone;
    }

    return processed;
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES((m_State == State_DataInputDone) || (m_State == State_Done), "Invalid state. Please restart from Initialize().");
    NN_SDK_REQUIRES(macSize >= m_GivenMacSize);
    NN_UNUSED(macSize);

    if (m_State == State_DataInputDone)
    {
        /* 最終的な MAC を生成する */
        GenerateMac();
        m_State = State_Done;
    }

    std::memcpy(pMac, m_Mac, m_GivenMacSize);
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::MakeInitialCounter(void* pCounter,
                                                         const void* pNonce, size_t nonceSize) const NN_NOEXCEPT
{
    uint8_t* pCounter8 = static_cast<uint8_t*>(pCounter);

    /* 最初にカウンタ全体を 0 で初期化 */
    std::memset(pCounter8, 0x00, BlockSize);

    /* 先頭の1バイトに Flag を設定する */
    uint8_t q = (BlockSize - 1 - nonceSize) & 0xFF;
    pCounter8[0] = ((q - 1) & 0x07);

    /* 2バイト目から Nonce を設定する */
    std::memcpy(pCounter8 + 1, pNonce, nonceSize);
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::MakeHeaderBlock(void* pBlock, const void* pNonce, size_t nonceSize,
                                                      int64_t aadSize, int64_t dataSize, size_t macSize) const NN_NOEXCEPT
{
    uint8_t* pBlock8 = static_cast<uint8_t*>(pBlock);

    /* 最初にブロック全体を 0 で初期化 */
    std::memset(pBlock8, 0x00, BlockSize);

    /* 先頭の1バイトに Flag を設定する */
    uint8_t q = (BlockSize - 1 - nonceSize) & 0xFF;
    pBlock8[0] = (((aadSize > 0) ? 0x40: 0x00)          // bit 7-6: Reserved | Adata
                  | ((((macSize - 2) / 2) & 0x07) << 3) // bit 5-3: (macSize - 2) / 2
                  | ((q - 1) & 0x07));                  // bit 2-0: q - 1

    /* 後ろから8バイト分の領域に dataSize を格納する */
    for (int i = 0; i < static_cast<int>(sizeof(int64_t)); ++i)
    {
        pBlock8[BlockSize - 1 - i] = ((dataSize & 0xFFFFFFFFFFFFFFFFull) >> (i * 8));
    }

    /* 2バイト目から Nonce を設定する(このとき dataSize の先頭の不要な桁が上書きされる) */
    std::memcpy(pBlock8 + 1, pNonce, nonceSize);
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::ProcessEncodedAadSize() NN_NOEXCEPT
{
    uint8_t encodedAad[10];
    size_t  encodedAadSize;

    /* AAD のサイズからエンコードされるサイズが決まる */
    if (m_GivenAadSize < 0xFF00u)
    {
        /* サイズ(2バイト) */
        encodedAad[0] = (m_GivenAadSize >> 8) & 0xFF;
        encodedAad[1] = (m_GivenAadSize >> 0) & 0xFF;
        encodedAadSize = 2;
    }
    else if (m_GivenAadSize < 0xFFFFFFFFu)
    {
        /* 0xFFFE + サイズ(4バイト) = 6バイト */
        encodedAad[0] = 0xFF;
        encodedAad[1] = 0xFE;
        encodedAad[2] = (m_GivenAadSize >> 24) & 0xFF;
        encodedAad[3] = (m_GivenAadSize >> 16) & 0xFF;
        encodedAad[4] = (m_GivenAadSize >>  8) & 0xFF;
        encodedAad[5] = (m_GivenAadSize >>  0) & 0xFF;
        encodedAadSize = 6;
    }
    else
    {
        /* 0xFFFF + サイズ(8バイト) = 10 バイト */
        encodedAad[0] = 0xFF;
        encodedAad[1] = 0xFF;
        encodedAad[2] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >> 56) & 0xFF;
        encodedAad[3] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >> 48) & 0xFF;
        encodedAad[4] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >> 40) & 0xFF;
        encodedAad[5] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >> 32) & 0xFF;
        encodedAad[6] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >> 24) & 0xFF;
        encodedAad[7] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >> 16) & 0xFF;
        encodedAad[8] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >>  8) & 0xFF;
        encodedAad[9] = ((m_GivenAadSize & 0xFFFFFFFFFFFFFFFFull) >>  0) & 0xFF;
        encodedAadSize = 10;
    }

    m_CbcMacImpl.template Update<BlockCipher>(encodedAad, encodedAadSize);
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::ProcessPadding() NN_NOEXCEPT
{
    /* 入力データがブロックサイズの倍数になるように 0 を入力する */
    if (m_CbcMacImpl.GetBufferedDataSize() > 0)
    {
        uint8_t zeroBlock[BlockSize] = {0};

        m_CbcMacImpl.template Update<BlockCipher>(zeroBlock, BlockSize - m_CbcMacImpl.GetBufferedDataSize());
    }
}

template <typename BlockCipher>
inline void CcmModeImpl<BlockCipher>::GenerateMac() NN_NOEXCEPT
{
    uint8_t tmp[BlockSize];

    /* 計算された CBC-MAC と暗号化された初期カウンタ値との XOR を取ったものが最終的な MAC になる */
    m_CbcMacImpl.GetMac(tmp, BlockSize);

    for (int i = 0; i < static_cast<int>(BlockSize); ++i)
    {
        m_Mac[i] ^= tmp[i];
    }
}

}}} // namespace nn::crypto::detail
