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
    @brief 鍵サイズが 128 bit の AES の CTR モードによる復号化を行うための API の宣言
 */

#include <nn/nn_SdkAssert.h>
#include <nn/crypto/crypto_AesEncryptor.h>
#include <nn/crypto/crypto_CtrDecryptor.h>

namespace nn { namespace crypto {

/**
    @brief      鍵サイズが 128bit の AES の CBC モードによる復号化を利用するためのクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態

 */
class Aes128CtrDecryptor
{
    NN_DISALLOW_COPY(Aes128CtrDecryptor);

public:
    static const size_t BlockSize = CtrDecryptor<AesEncryptor128>::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t IvSize    = CtrDecryptor<AesEncryptor128>::BlockSize; //!< IV のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Aes128CtrDecryptor() NN_NOEXCEPT {}

    /**
        @brief      鍵サイズが 128 bit の AES-CTR モードによる復号化の処理を初期化します。

        @param[in]  pKey     鍵データへのポインタ。
        @param[in]  keySize  鍵データのバイトサイズ。
        @param[in]  pIv      カウンタの初期値データへのポインタ。
        @param[in]  ivSize   カウンタの初期値データのバイトサイズ。

        @pre
        - keySize == 16
        - ivSize == IvSize

        @post
        - インスタンスは処理可能状態になる。

        @details
        鍵データとカウンタの初期値を入力し、復号化処理を初期化します。
        鍵データおよびカウンタの初期値は内部にコピーされます。
     */
    void Initialize(const void* pKey, size_t keySize, const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(keySize == 16);
        NN_SDK_REQUIRES(ivSize == IvSize);

        m_Aes.Initialize(pKey, keySize);
        m_CtrDecryptor.Initialize(&m_Aes, pIv, ivSize);
    }

    /**
        @brief      鍵サイズが 128 bit の AES-CTR モードによる復号化を行います。

        @copydetails CtrDecryptor::Update
     */
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_CtrDecryptor.Update(pDst, dstSize, pSrc, srcSize);
    }

private:
    AesEncryptor128               m_Aes;
    CtrDecryptor<AesEncryptor128> m_CtrDecryptor;
};

//! @name AES 暗号化・復号化
//! @{

/**
    @brief    鍵サイズが 128 bit の AES-CTR モードによる復号化を行うユーティリティ関数です。

    @param[out] pDst     復号化されたデータを受け取るバッファへのポインタ。
    @param[in]  dstSize  pDst が指すバッファのバイトサイズ。
    @param[in]  pKey     鍵データへのポインタ。
    @param[in]  keySize  鍵データのバイトサイズ。
    @param[in]  pIv      カウンタの初期値データへのポインタ。
    @param[in]  ivSize   カウンタの初期値データのバイトサイズ。
    @param[in]  pSrc     復号化するデータが格納されているバッファへのポインタ。
    @param[in]  srcSize  復号化するデータのバイトサイズ。

    @return     実際に復号化されたバイト数

    @pre
    - keySize == 16
    - ivSize == 16
    - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがないか、完全に一致している。

    @post
    - pDst に復号化された結果が書き込まれる。

    @details
    与えられたデータ列を AES-CTR モードで復号化します。
 */
size_t DecryptAes128Ctr(void* pDst, size_t dstSize,
                        const void* pKey, size_t keySize,
                        const void* pIv, size_t ivSize,
                        const void* pSrc, size_t srcSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::crypto

