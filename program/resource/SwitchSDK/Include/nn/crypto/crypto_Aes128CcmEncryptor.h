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

#include <nn/crypto/crypto_AesEncryptor.h>
#include <nn/crypto/crypto_CcmEncryptor.h>

namespace nn { namespace crypto {

/**
    @brief      鍵サイズが 128bit の AES の CCM モードによる認証付き暗号化を利用するためのクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 追加認証データ処理可能状態
    - 暗号化処理可能状態
    - データ入力完了状態
    - 完了状態

 */
class Aes128CcmEncryptor
{
    NN_DISALLOW_COPY(Aes128CcmEncryptor);

public:
    static const size_t BlockSize    = CcmEncryptor<AesEncryptor128>::BlockSize;    //!< ブロックサイズを表す定数です。
    static const size_t MaxNonceSize = CcmEncryptor<AesEncryptor128>::MaxNonceSize; //!< 最大の Nonce のサイズを表す定数です。
    static const size_t MaxMacSize   = CcmEncryptor<AesEncryptor128>::MaxMacSize;   //!< 最大の MAC のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Aes128CcmEncryptor() NN_NOEXCEPT {}

    /**
        @brief      鍵サイズが 128bit の AES-CCM モードでの認証付き暗号化の処理を初期化します。

        @param[in]  pKey      鍵データへのポインタ。
        @param[in]  keySize   鍵データのバイトサイズ。
        @param[in]  pNonce    Nonce データへのポインタ。
        @param[in]  nonceSize Nonce データのバイトサイズ。
        @param[in]  aadSize   追加認証データのバイトサイズ。
        @param[in]  dataSize  暗号化の対象となるデータのバイトサイズ。
        @param[in]  macSize   メッセージ認証コードのバイトサイズ。

        @pre
        - keySize == 16
        - nonceSize >= 7 && nonceSize <= 13
        - macSize >= 4 && macSize <= 16 && macSize % 2 == 0
        - aadSize >= 0
        - dataSize >= 0
        - (nonceSize == 7) ? dataSize <= INT64_MAX : dataSize < (1LL << ((15 - nonceSize) * 8))

        @post
        - aadSize が 0 の場合、インスタンスは暗号化処理可能状態になる。
        - aadSize が 0 でない場合、インスタンスは追加認証データ処理可能状態になる。

        @details
        各パラメータのサイズなど詳しくは
        CcmEncryptor::Initialize を参照してください。
      */
    void   Initialize(const void* pKey, size_t keySize,
                      const void* pNonce, size_t nonceSize,
                      int64_t aadSize, int64_t dataSize, size_t macSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(keySize == 16);
        // その他の事前条件は CcmEncryptor の中で確認されます

        m_Aes.Initialize(pKey, keySize);
        m_CcmEncryptor.Initialize(&m_Aes, pNonce, nonceSize, aadSize, dataSize, macSize);
    }

    /**
        @brief      鍵サイズが 128bit の AES-CCM モードによる追加認証データを入力します。

        @copydetails CcmEncryptor::UpdateAad
     */
    void   UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT
    {
        m_CcmEncryptor.UpdateAad(pAad, aadSize);
    }

    /**
        @brief      鍵サイズが 128bit の AES-CCM モードによる認証付き暗号化を行います。

        @copydetails CcmEncryptor::Update
     */
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_CcmEncryptor.Update(pDst, dstSize, pSrc, srcSize);
    }

    /**
        @brief      鍵サイズが 128bit の AES-CCM モードによる最終的な MAC を取得します。

        @copydetails CcmEncryptor::GetMac
     */
    void   GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
    {
        m_CcmEncryptor.GetMac(pMac, macSize);
    }

private:
    AesEncryptor128               m_Aes;
    CcmEncryptor<AesEncryptor128> m_CcmEncryptor;
};

//! @name AES 暗号化・復号化
//! @{

/**
    @brief    鍵サイズが 128bit の AES-CCM モードによる暗号化を行うユーティリティ関数です。

    @param[out] pDst          暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  dstSize       pDst が指すバッファのバイトサイズ。
    @param[out] pMac          MAC を受け取るバッファへのポインタ。
    @param[in]  macBufferSize pMac が指すバッファのバイトサイズ。
    @param[in]  pKey          鍵データへのポインタ。
    @param[in]  keySize       鍵データのバイトサイズ。
    @param[in]  pNonce        Nonce データへのポインタ。
    @param[in]  nonceSize     Nonce データのバイトサイズ。
    @param[in]  pSrc          暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  srcSize       暗号化するデータのバイトサイズ。
    @param[in]  pAad          追加認証データが格納されているバッファへのポインタ。
    @param[in]  aadSize       追加認証データのバイトサイズ。
    @param[in]  macSize       MAC データのバイトサイズ。

    @return     実際に暗号化されたバイト数。

    @pre
    - keySize == 16
    - nonceSize >= 7 && nonceSize <= 13
    - macSize >= 4 && macSize <= 16 && macSize % 2 == 0
    - (nonceSize == 7) ? srcSize <= INT64_MAX : srcSize < (1LL << ((15 - nonceSize) * 8))
    - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがないか、完全に一致している。

    @post
    - pDst に暗号化された結果が書き込まれる。
    - pMac に MAC が書き込まれる。

    @details
    与えられたデータ列を AES-CCM モードで暗号化し、MAC を取得します。
    追加認証データがない場合は aadSize に 0 を指定してください。
 */
size_t EncryptAes128Ccm(void* pDst, size_t dstSize,
                        void* pMac, size_t macBufferSize,
                        const void* pKey, size_t keySize,
                        const void* pNonce, size_t nonceSize,
                        const void* pSrc, size_t srcSize,
                        const void* pAad, size_t aadSize,
                        size_t macSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::crypto

