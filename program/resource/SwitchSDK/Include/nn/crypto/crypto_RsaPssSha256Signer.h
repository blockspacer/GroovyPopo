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
    @brief RSA-PSS-SHA-256 署名を生成するための API の宣言
 */

#include <nn/crypto/crypto_RsaPssSigner.h>
#include <nn/crypto/crypto_Sha256Generator.h>

namespace nn { namespace crypto {

/**
    @brief  鍵サイズが 2048bit の RSA-PSS-SHA-256 署名を生成するクラスです。

    @details
    2048bit の RSA-PSS-SHA-256 署名を生成します。
    詳しくは RsaPssSigner を参照してください。
*/
typedef RsaPssSigner<256, nn::crypto::Sha256Generator> Rsa2048PssSha256Signer;

/**
    @brief  鍵サイズが 4096bit の RSA-PSS-SHA-256 署名を生成するクラスです。

    @details
    4096bit の RSA-PSS-SHA-256 署名を生成します。
    詳しくは RsaPssSigner を参照してください。
*/
typedef RsaPssSigner<512, nn::crypto::Sha256Generator> Rsa4096PssSha256Signer;


//! @name RSA 署名
//! @{

/**
    @brief      鍵サイズが 2048bit の RSA-PSS-SHA-256 署名を生成するユーティリティ関数です。作業領域としてスタックを利用します。

    @param[out] pOutSignature          署名データが格納されるバッファへのポインタ。
    @param[in]  signatureSize          pOutSignature が指すバッファのバイトサイズ。
    @param[in]  pModulus               RSA の法データへのポインタ。
    @param[in]  modulusSize            RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent       RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize    RSA の秘密指数データのバイトサイズ
    @param[in]  pRawMessage            署名対象のデータへのポインタ。
    @param[in]  rawMessageSize         署名対象のデータのバイトサイズ。
    @param[in]  pSalt                  Salt データへのポインタ。
    @param[in]  saltSize               Salt データのバイトサイズ。

    @return     署名の生成に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられています。

    @pre
    - signatureSize == 256
    - 0 < saltSize && saltSize <= 222

    @post
    - pOutSignature に署名が格納される。

    @details
    与えられたデータ列の RSA-PSS-SHA-256 署名を生成します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    Salt データには署名の生成ごとに異なる乱数を指定してください。
    この関数での推奨サイズは 32 バイトです。

    ワークバッファとしてスタックから 4096 バイトが利用されます。
 */
inline bool SignRsa2048PssSha256( void* pOutSignature, size_t signatureSize,
                                  const void* pModulus, size_t modulusSize,
                                  const void* pPrivateExponent, size_t privateExponentSize,
                                  const void* pRawMessage, size_t rawMessageSize,
                                  const void* pSalt, size_t saltSize ) NN_NOEXCEPT
{
    return Rsa2048PssSha256Signer::Sign( pOutSignature, signatureSize,
                                         pModulus, modulusSize,
                                         pPrivateExponent, privateExponentSize,
                                         pRawMessage, rawMessageSize,
                                         pSalt, saltSize );
}

/**
    @brief      鍵サイズが 2048bit の RSA-PSS-SHA-256 署名を生成するユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[out] pOutSignature          署名データが格納されるバッファへのポインタ。
    @param[in]  signatureSize          pOutSignature が指すバッファのバイトサイズ。
    @param[in]  pModulus               RSA の法データへのポインタ。
    @param[in]  modulusSize            RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent       RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize    RSA の秘密指数データのバイトサイズ
    @param[in]  pRawMessage            署名対象のデータへのポインタ。
    @param[in]  rawMessageSize         署名対象のデータのバイトサイズ。
    @param[in]  pSalt                  Salt データへのポインタ。
    @param[in]  saltSize               Salt データのバイトサイズ。
    @param[in]  pWorkBuffer            ワークバッファへのポインタ。
    @param[in]  workBufferSize         ワークバッファのバイトサイズ。

    @return     署名の生成に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

    @pre
    - signatureSize == 256
    - 0 < saltSize && saltSize <= 222
    - workBufferSize >= 4096

    @post
    - pOutSignature に署名が格納される。

    @details
    与えられたデータ列の RSA-PSS-SHA-256 署名を生成します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    Salt データには署名の生成ごとに異なる乱数を指定してください。
    この関数での推奨サイズは 32 バイトです。

    ワークバッファは 4096 バイト以上が必要です。
 */
inline bool SignRsa2048PssSha256( void* pOutSignature, size_t signatureSize,
                                  const void* pModulus, size_t modulusSize,
                                  const void* pPrivateExponent, size_t privateExponentSize,
                                  const void* pRawMessage, size_t rawMessageSize,
                                  const void* pSalt, size_t saltSize,
                                  void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
{
    return Rsa2048PssSha256Signer::Sign( pOutSignature, signatureSize,
                                         pModulus, modulusSize,
                                         pPrivateExponent, privateExponentSize,
                                         pRawMessage, rawMessageSize,
                                         pSalt, saltSize,
                                         pWorkBuffer, workBufferSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-PSS-SHA-256 署名を生成するユーティリティ関数です。作業領域としてスタックを利用します。

    @param[out] pOutSignature          署名データが格納されるバッファへのポインタ。
    @param[in]  signatureSize          pOutSignature が指すバッファのバイトサイズ。
    @param[in]  pModulus               RSA の法データへのポインタ。
    @param[in]  modulusSize            RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent       RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize    RSA の秘密指数データのバイトサイズ
    @param[in]  pRawMessage            署名対象のデータへのポインタ。
    @param[in]  rawMessageSize         署名対象のデータのバイトサイズ。
    @param[in]  pSalt                  Salt データへのポインタ。
    @param[in]  saltSize               Salt データのバイトサイズ。

    @return     署名の生成に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられています。

    @pre
    - signatureSize == 512
    - 0 < saltSize && saltSize <= 478

    @post
    - pOutSignature に署名が格納される。

    @details
    与えられたデータ列の RSA-PSS-SHA-256 署名を生成します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    Salt データには署名の生成ごとに異なる乱数を指定してください。
    この関数での推奨サイズは 32 バイトです。

    ワークバッファとしてスタックから 8192 バイトが利用されます。
 */
inline bool SignRsa4096PssSha256( void* pOutSignature, size_t signatureSize,
                                  const void* pModulus, size_t modulusSize,
                                  const void* pPrivateExponent, size_t privateExponentSize,
                                  const void* pRawMessage, size_t rawMessageSize,
                                  const void* pSalt, size_t saltSize ) NN_NOEXCEPT
{
    return Rsa4096PssSha256Signer::Sign( pOutSignature, signatureSize,
                                         pModulus, modulusSize,
                                         pPrivateExponent, privateExponentSize,
                                         pRawMessage, rawMessageSize,
                                         pSalt, saltSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-PSS-SHA-256 署名を生成するユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[out] pOutSignature          署名データが格納されるバッファへのポインタ。
    @param[in]  signatureSize          pOutSignature が指すバッファのバイトサイズ。
    @param[in]  pModulus               RSA の法データへのポインタ。
    @param[in]  modulusSize            RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent       RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize    RSA の秘密指数データのバイトサイズ
    @param[in]  pRawMessage            署名対象のデータへのポインタ。
    @param[in]  rawMessageSize         署名対象のデータのバイトサイズ。
    @param[in]  pSalt                  Salt データへのポインタ。
    @param[in]  saltSize               Salt データのバイトサイズ。
    @param[in]  pWorkBuffer            ワークバッファへのポインタ。
    @param[in]  workBufferSize         ワークバッファのバイトサイズ。

    @return     署名の生成に成功したかを返します。@n
                失敗した場合は不正なパラメータが与えられています。

    @pre
    - signatureSize == 512
    - 0 < saltSize && saltSize <= 478
    - workBufferSize >= 8192

    @post
    - pOutSignature に署名が格納される。

    @details
    与えられたデータ列の RSA-PSS-SHA-256 署名を生成します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    Salt データには署名の生成ごとに異なる乱数を指定してください。
    この関数での推奨サイズは 32 バイトです。

    ワークバッファは 8192 バイト以上が必要です。
 */
inline bool SignRsa4096PssSha256( void* pOutSignature, size_t signatureSize,
                                  const void* pModulus, size_t modulusSize,
                                  const void* pPrivateExponent, size_t privateExponentSize,
                                  const void* pRawMessage, size_t rawMessageSize,
                                  const void* pSalt, size_t saltSize,
                                  void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
{
    return Rsa4096PssSha256Signer::Sign( pOutSignature, signatureSize,
                                         pModulus, modulusSize,
                                         pPrivateExponent, privateExponentSize,
                                         pRawMessage, rawMessageSize,
                                         pSalt, saltSize,
                                         pWorkBuffer, workBufferSize );
}

//! @}

}} // namespace nn::crypto
