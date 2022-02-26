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
    @brief 鍵サイズが 128 bit の AES の CCM モードによる認証付き暗号化を行うための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/crypto_AesEncryptor.h>
#include <nn/crypto/crypto_GcmEncryptor.h>
#include <cstring>

namespace nn { namespace crypto {

/**
    @brief      鍵サイズが 128bit の AES の GCM モードによる認証付き暗号化を利用するためのクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 追加認証データ処理可能状態
    - 暗号化処理可能状態
    - 完了状態

 */
class Aes128GcmEncryptor
{
    NN_DISALLOW_COPY(Aes128GcmEncryptor);

public:
    static const size_t BlockSize = AesEncryptor128::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t MacSize = 16;                           //!< MAC のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Aes128GcmEncryptor() NN_NOEXCEPT {}

    /**
        @brief      鍵サイズが 128bit の AES-GCM モードでの認証付き暗号化の処理を初期化します。

        @param[in]  pKey      鍵データへのポインタ。
        @param[in]  keySize   鍵データのバイトサイズ。
        @param[in]  pIv       カウンタの初期値データへのポインタ。
        @param[in]  ivSize    カウンタの初期値データのバイトサイズ。

        @pre
        - keySize == 16

        @post
        - インスタンスは追加認証データ処理可能状態になる。

        @details
        カウンタの初期値は暗号化ごとに異なるものを使用する必要があります。
        推奨サイズは 12 バイトです。

        鍵データおよびカウンタの初期値データは内部にコピーされます。
      */
    void Initialize(const void* pKey, size_t keySize, const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        m_Aes.Initialize(pKey,keySize);
        m_Gcm.Initialize(&m_Aes,pIv,ivSize);
    }

    /**
        @brief      鍵サイズが 128bit の AES-GCM モードの処理をリセットします。

        @copydetails GcmEncryptor::Reset
     */
    void Reset( const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        m_Gcm.Reset( pIv, ivSize );
    }

    /**
        @brief      鍵サイズが 128bit の AES-GCM モードによる追加認証データを入力します。

        @copydetails GcmEncryptor::UpdateAad
     */
    void UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT
    {
        m_Gcm.UpdateAad( pAad, aadSize );
    }

    /**
        @brief      鍵サイズが 128bit の AES-GCM モードによる認証付き暗号化を行います。

        @copydetails GcmEncryptor::Update
     */
    size_t Update(void* pDst, size_t dstSize,
                  const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_Gcm.Update( pDst, dstSize, pSrc, srcSize );
    }

    /**
        @brief      鍵サイズが 128bit の AES-GCM モードによる最終的な MAC を取得します。

        @copydetails GcmEncryptor::GetMac
     */
    void   GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
    {
        m_Gcm.GetMac(pMac, macSize);
    }

private:
    nn::crypto::AesEncryptor128 m_Aes;
    GcmEncryptor<nn::crypto::AesEncryptor128> m_Gcm;
};

//! @name AES 暗号化・復号化
//! @{

/**
    @brief    鍵サイズが 128bit の AES-GCM モードによる暗号化を行うユーティリティ関数です。

    @param[out] pDst          暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  dstSize       pDst が指すバッファのバイトサイズ。
    @param[out] pMac          MAC を受け取るバッファへのポインタ。
    @param[in]  macSize       pMac が指すバッファのバイトサイズ。
    @param[in]  pKey          鍵データへのポインタ。
    @param[in]  keySize       鍵データのバイトサイズ。
    @param[in]  pIv           カウンタの初期値データへのポインタ。
    @param[in]  ivSize        カウンタの初期値データのバイトサイズ。
    @param[in]  pSrc          暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  srcSize       暗号化するデータのバイトサイズ。
    @param[in]  pAad          追加認証データが格納されているバッファへのポインタ。
    @param[in]  aadSize       追加認証データのバイトサイズ。

    @return     実際に暗号化されたバイト数。

    @pre
    - keySize == 16
    - dstSize >= srcSize
    - macSize >= 16
    - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがないか、完全に一致している。

    @post
    - pDst に暗号化された結果が書き込まれる。
    - pMac に MAC が書き込まれる。

    @details
    与えられたデータ列を AES-GCM モードで暗号化し、MAC を取得します。
    追加認証データがない場合は aadSize に 0 を指定してください。
 */
inline size_t EncryptAes128Gcm(void* pDst, size_t dstSize,
                               void* pMac, size_t macSize,
                               const void* pKey, size_t keySize,
                               const void* pIv, size_t ivSize,
                               const void* pSrc, size_t srcSize,
                               const void* pAad, size_t aadSize ) NN_NOEXCEPT
{
    Aes128GcmEncryptor aes;
    aes.Initialize(pKey, keySize, pIv, ivSize);
    aes.UpdateAad(pAad, aadSize);
    size_t processed = aes.Update(pDst, dstSize, pSrc, srcSize);
    aes.GetMac(pMac, macSize);
    return processed;
}

//! @}

}} // namespace nn::crypto
