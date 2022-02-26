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
    @brief RSA-OAEP-SHA-256 による暗号化を行うための API の宣言
 */

#include <nn/crypto/crypto_RsaOaepEncryptor.h>
#include <nn/crypto/crypto_Sha256Generator.h>

namespace nn { namespace crypto {

/**
    @brief  鍵サイズが 2048bit の RSA-OAEP-SHA-256 暗号化を行うクラスです。

    @details
    2048bit の RSA-OAEP-SHA-256 暗号化を行います。
    詳しくは RsaOaepEncryptor を参照してください。
*/
typedef RsaOaepEncryptor<256, nn::crypto::Sha256Generator> Rsa2048OaepSha256Encryptor;

/**
    @brief  鍵サイズが 4096bit の RSA-OAEP-SHA-256 暗号化を行うクラスです。

    @details
    4096bit の RSA-OAEP-SHA-256 暗号化を行います。
    詳しくは RsaOaepEncryptor を参照してください。
*/
typedef RsaOaepEncryptor<512, nn::crypto::Sha256Generator> Rsa4096OaepSha256Encryptor;


//! @name RSA 暗号
//! @{

/**
    @brief      鍵サイズが 2048bit の RSA-OAEP-SHA-256 による暗号化を行うユーティリティ関数です。作業領域としてスタックを利用します。

    @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
    @param[in]  pSeed                Seed データへのポインタ。
    @param[in]  seedSize             Seed データのサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。

    @return     暗号化に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられています。

    @pre
    - modulusSize <= 256
    - publicExponentSize <= 3
    - cipherSize == 256
    - rawMessageSize <= 190

    @post
    - pOutCipher に暗号化された結果が書き込まれる。

    @details
    与えられたデータ列を暗号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。

    Seed には暗号化を行うごとに異なる 32 バイトの乱数を指定してください。

    ワークバッファとしてスタックから 4096 バイトが利用されます。
 */
inline bool EncryptRsa2048OaepSha256(void* pOutCipher, size_t cipherSize,
                                     const void* pModulus, size_t modulusSize,
                                     const void* pPublicExponent, size_t publicExponentSize,
                                     const void* pRawMessage, size_t rawMessageSize,
                                     const void* pSeed, size_t seedSize,
                                     const void* pLabel, size_t labelSize ) NN_NOEXCEPT
{
    return Rsa2048OaepSha256Encryptor::Encrypt( pOutCipher, cipherSize,
                                                pModulus, modulusSize,
                                                pPublicExponent, publicExponentSize,
                                                pRawMessage, rawMessageSize,
                                                pSeed, seedSize,
                                                pLabel, labelSize );
}

/**
    @brief      鍵サイズが 2048bit の RSA-OAEP-SHA-256 による暗号化を行うユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
    @param[in]  pSeed                Seed データへのポインタ。
    @param[in]  seedSize             Seed データのサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。
    @param[in]  pWorkBuffer          ワークバッファへのポインタ。
    @param[in]  workBufferSize       ワークバッファのバイトサイズ。

    @return     暗号化に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

    @pre
    - modulusSize <= 256
    - publicExponentSize <= 3
    - cipherSize == 256
    - rawMessageSize <= 190
    - workBufferSize >= 4096

    @post
    - pOutCipher に暗号化された結果が書き込まれる。

    @details
    与えられたデータ列を暗号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。

    Seed には暗号化を行うごとに異なる 32 バイトの乱数を指定してください。

    ワークバッファは 4096 バイト以上が必要です。
 */
inline bool EncryptRsa2048OaepSha256(void* pOutCipher, size_t cipherSize,
                                     const void* pModulus, size_t modulusSize,
                                     const void* pPublicExponent, size_t publicExponentSize,
                                     const void* pRawMessage, size_t rawMessageSize,
                                     const void* pSeed, size_t seedSize,
                                     const void* pLabel, size_t labelSize,
                                     void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
{
    return Rsa2048OaepSha256Encryptor::Encrypt( pOutCipher, cipherSize,
                                                pModulus, modulusSize,
                                                pPublicExponent, publicExponentSize,
                                                pRawMessage, rawMessageSize,
                                                pSeed, seedSize,
                                                pLabel, labelSize,
                                                pWorkBuffer, workBufferSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-OAEP-SHA-256 による暗号化を行うユーティリティ関数です。作業領域としてスタックを利用します。

    @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
    @param[in]  pSeed                Seed データへのポインタ。
    @param[in]  seedSize             Seed データのサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。

    @return     暗号化に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられています。

    @pre
    - modulusSize <= 512
    - publicExponentSize <= 3
    - cipherSize == 512
    - rawMessageSize <= 446

    @post
    - pOutCipher に暗号化された結果が書き込まれる。

    @details
    与えられたデータ列を暗号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。

    Seed には暗号化を行うごとに異なる 32 バイトの乱数を指定してください。

    ワークバッファとしてスタックから 8192 バイトが利用されます。
 */
inline bool EncryptRsa4096OaepSha256(void* pOutCipher, size_t cipherSize,
                                     const void* pModulus, size_t modulusSize,
                                     const void* pPublicExponent, size_t publicExponentSize,
                                     const void* pRawMessage, size_t rawMessageSize,
                                     const void* pSeed, size_t seedSize,
                                     const void* pLabel, size_t labelSize ) NN_NOEXCEPT
{
    return Rsa4096OaepSha256Encryptor::Encrypt( pOutCipher, cipherSize,
                                                pModulus, modulusSize,
                                                pPublicExponent, publicExponentSize,
                                                pRawMessage, rawMessageSize,
                                                pSeed, seedSize,
                                                pLabel, labelSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-OAEP-SHA-256 による暗号化を行うユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
    @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
    @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
    @param[in]  pSeed                Seed データへのポインタ。
    @param[in]  seedSize             Seed データのサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。
    @param[in]  pWorkBuffer          ワークバッファへのポインタ。
    @param[in]  workBufferSize       ワークバッファのバイトサイズ。

    @return     暗号化に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

    @pre
    - modulusSize <= 512
    - publicExponentSize <= 3
    - cipherSize == 512
    - rawMessageSize <= 446
    - workBufferSize >= 8192

    @post
    - pOutCipher に暗号化された結果が書き込まれる。

    @details
    与えられたデータ列を暗号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。

    Seed には暗号化を行うごとに異なる 32 バイトの乱数を指定してください。

    ワークバッファは 8192 バイト以上が必要です。
 */
inline bool EncryptRsa4096OaepSha256(void* pOutCipher, size_t cipherSize,
                                     const void* pModulus, size_t modulusSize,
                                     const void* pPublicExponent, size_t publicExponentSize,
                                     const void* pRawMessage, size_t rawMessageSize,
                                     const void* pSeed, size_t seedSize,
                                     const void* pLabel, size_t labelSize,
                                     void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
{
    return Rsa4096OaepSha256Encryptor::Encrypt( pOutCipher, cipherSize,
                                                pModulus, modulusSize,
                                                pPublicExponent, publicExponentSize,
                                                pRawMessage, rawMessageSize,
                                                pSeed, seedSize,
                                                pLabel, labelSize,
                                                pWorkBuffer, workBufferSize );
}

//! @}

}} // namespace nn::crypto
