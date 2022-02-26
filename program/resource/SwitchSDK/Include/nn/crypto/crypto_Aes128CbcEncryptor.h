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

/**
    @file
    @brief 鍵サイズが 128 bit の AES の CBC モードによる暗号化を行うための API の宣言
 */

#include <nn/nn_SdkAssert.h>
#include <nn/crypto/crypto_AesEncryptor.h>
#include <nn/crypto/crypto_CbcEncryptor.h>

namespace nn { namespace crypto {

/**
    @brief      鍵サイズが 128bit の AES の CBC モードによる暗号化を利用するためのクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態

 */
class Aes128CbcEncryptor
{
    NN_DISALLOW_COPY(Aes128CbcEncryptor);

public:
    static const size_t BlockSize = CbcEncryptor<AesEncryptor128>::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t IvSize    = CbcEncryptor<AesEncryptor128>::BlockSize; //!< IV のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Aes128CbcEncryptor() NN_NOEXCEPT {}

    /**
        @brief      鍵サイズが 128 bit の AES-CBC モードによる暗号化の処理を初期化します。

        @param[in]  pKey     鍵データへのポインタ。
        @param[in]  keySize  鍵データのバイトサイズ。
        @param[in]  pIv      IV データへのポインタ。
        @param[in]  ivSize   IV データのバイトサイズ。

        @pre
        - keySize == 16
        - ivSize == IvSize

        @post
        - インスタンスは処理可能状態になる。

        @details
        鍵データと IV を入力し、暗号化処理を初期化します。
        鍵データおよび IV データは内部にコピーされます。
     */
    void Initialize(const void* pKey, size_t keySize, const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(keySize == 16);
        NN_SDK_REQUIRES(ivSize == IvSize);

        m_Aes.Initialize(pKey, keySize);
        m_CbcEncryptor.Initialize(&m_Aes, pIv, ivSize);
    }

    /**
        @brief      鍵サイズが 128 bit の AES-CBC モードによる暗号化を行います。

        @copydetails CbcEncryptor::Update
     */
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_CbcEncryptor.Update(pDst, dstSize, pSrc, srcSize);
    }

    /**
        @copydoc    CbcEncryptor::GetBufferedDataSize
     */
    size_t GetBufferedDataSize() const NN_NOEXCEPT
    {
        return m_CbcEncryptor.GetBufferedDataSize();
    }

private:
    AesEncryptor128               m_Aes;
    CbcEncryptor<AesEncryptor128> m_CbcEncryptor;
};

//! @name AES 暗号化・復号化
//! @{

/**
    @brief    鍵サイズが 128 bit の AES-CBC モードによる暗号化を行うユーティリティ関数です。

    @param[out] pDst     暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  dstSize  pDst が指すバッファのバイトサイズ。
    @param[in]  pKey     鍵データへのポインタ。
    @param[in]  keySize  鍵データのバイトサイズ。
    @param[in]  pIv      IV データへのポインタ。
    @param[in]  ivSize   IV データのバイトサイズ。
    @param[in]  pSrc     暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  srcSize  暗号化するデータのバイトサイズ。

    @return     実際に暗号化されたバイト数

    @pre
    - keySize == 16
    - ivSize == 16
    - srcSize で指定されるサイズがブロックサイズの倍数である。
    - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがないか、完全に一致している。

    @post
    - pDst に暗号化された結果が書き込まれる。

    @details
    与えられたデータ列を AES-CBC モードで暗号化します。
 */
size_t EncryptAes128Cbc(void* pDst, size_t dstSize,
                        const void* pKey, size_t keySize,
                        const void* pIv, size_t ivSize,
                        const void* pSrc, size_t srcSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::crypto

