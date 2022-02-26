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
    @brief RSA-PSS-SHA-256 署名を検証するための API の宣言
 */

#include <nn/crypto/crypto_RsaPssVerifier.h>
#include <nn/crypto/crypto_Sha256Generator.h>

namespace nn { namespace crypto {

/**
    @brief  鍵サイズが 2048bit の RSA-PSS-SHA-256 署名を検証するクラスです。

    @details
    2048bit の RSA-PSS-SHA-256 署名を検証します。
    詳しくは RsaPssVerifier を参照してください。
 */
typedef RsaPssVerifier<256, nn::crypto::Sha256Generator> Rsa2048PssSha256Verifier;

/**
    @brief  鍵サイズが 4096bit の RSA-PSS-SHA-256 署名を検証するクラスです。

    @details
    4096bit の RSA-PSS-SHA-256 署名を検証します。
    詳しくは RsaPssVerifier を参照してください。
 */
typedef RsaPssVerifier<512, nn::crypto::Sha256Generator> Rsa4096PssSha256Verifier;


//! @name RSA 署名
//! @{

/**
    @brief      鍵サイズが 2048bit の RSA-PSS-SHA-256 署名を検証するユーティリティ関数です。作業領域としてスタックを利用します。

    @param[in]  pSignature            署名データへのポインタ。
    @param[in]  signatureSize         署名データのバイトサイズ。
    @param[in]  pModulus              RSA の法データへのポインタ。
    @param[in]  modulusSize           RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent       RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize    RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage           検証対象のデータへのポインタ。
    @param[in]  rawMessageSize        検証対象のデータのバイトサイズ。

    @return     署名の検証に成功したかを返します。@n
                失敗した場合は正しい署名データではないか不正なパラメータが与えられています。

    @pre
    - signatureSize == 256
    - modulusSize <= 256
    - publicExponentSize <= 3

    @details
    与えられた署名データが与えられたデータ列の正しい RSA-PSS-SHA-256 署名であることを検証します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    検証対象のデータのハッシュ値は内部で計算されます。

    ワークバッファとしてスタックから 4096 バイトが利用されます。
 */
inline bool VerifyRsa2048PssSha256( const void* pSignature, size_t signatureSize,
                                    const void* pModulus, size_t modulusSize,
                                    const void* pPublicExponent, size_t publicExponentSize,
                                    const void* pRawMessage, size_t rawMessageSize ) NN_NOEXCEPT
{
    return Rsa2048PssSha256Verifier::Verify( pSignature, signatureSize,
                                             pModulus, modulusSize,
                                             pPublicExponent, publicExponentSize,
                                             pRawMessage, rawMessageSize );
}

/**
    @brief      鍵サイズが 2048bit の RSA-PSS-SHA-256 署名を検証するユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[in]  pSignature            署名データへのポインタ。
    @param[in]  signatureSize         署名データのバイトサイズ。
    @param[in]  pModulus              RSA の法データへのポインタ。
    @param[in]  modulusSize           RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent       RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize    RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage           検証対象のデータへのポインタ。
    @param[in]  rawMessageSize        検証対象のデータのバイトサイズ。
    @param[in]  pWorkBuffer           ワークバッファへのポインタ。
    @param[in]  workBufferSize        ワークバッファのバイトサイズ。

    @return     署名の検証に成功したかを返します。@n
                失敗した場合は正しい署名データではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

    @pre
    - signatureSize == 256
    - modulusSize <= 256
    - publicExponentSize <= 3
    - workBufferSize >= 4096

    @details
    与えられた署名データが与えられたデータ列の正しい RSA-PSS-SHA-256 署名であることを検証します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    検証対象のデータのハッシュ値は内部で計算されます。

    ワークバッファは 4096 バイト以上が必要です。
 */
inline bool VerifyRsa2048PssSha256( const void* pSignature, size_t signatureSize,
                                    const void* pModulus, size_t modulusSize,
                                    const void* pPublicExponent, size_t publicExponentSize,
                                    const void* pRawMessage, size_t rawMessageSize,
                                    void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
{
    return Rsa2048PssSha256Verifier::Verify( pSignature, signatureSize,
                                             pModulus, modulusSize,
                                             pPublicExponent, publicExponentSize,
                                             pRawMessage, rawMessageSize,
                                             pWorkBuffer, workBufferSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-PSS-SHA-256 署名を検証するユーティリティ関数です。作業領域としてスタックを利用します。

    @param[in]  pSignature            署名データへのポインタ。
    @param[in]  signatureSize         署名データのバイトサイズ。
    @param[in]  pModulus              RSA の法データへのポインタ。
    @param[in]  modulusSize           RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent       RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize    RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage           検証対象のデータへのポインタ。
    @param[in]  rawMessageSize        検証対象のデータのバイトサイズ。

    @return     署名の検証に成功したかを返します。@n
                失敗した場合は正しい署名データではないか不正なパラメータが与えられています。

    @pre
    - signatureSize == 512
    - modulusSize <= 512
    - publicExponentSize <= 3

    @details
    与えられた署名データが与えられたデータ列の正しい RSA-PSS-SHA-256 署名であることを検証します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    検証対象のデータのハッシュ値は内部で計算されます。

    ワークバッファとしてスタックから 8192 バイトが利用されます。
 */
inline bool VerifyRsa4096PssSha256( const void* pSignature, size_t signatureSize,
                                    const void* pModulus, size_t modulusSize,
                                    const void* pPublicExponent, size_t publicExponentSize,
                                    const void* pRawMessage, size_t rawMessageSize ) NN_NOEXCEPT
{
    return Rsa4096PssSha256Verifier::Verify( pSignature, signatureSize,
                                             pModulus, modulusSize,
                                             pPublicExponent, publicExponentSize,
                                             pRawMessage, rawMessageSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-PSS-SHA-256 署名を検証するユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[in]  pSignature            署名データへのポインタ。
    @param[in]  signatureSize         署名データのバイトサイズ。
    @param[in]  pModulus              RSA の法データへのポインタ。
    @param[in]  modulusSize           RSA の法データのバイトサイズ。
    @param[in]  pPublicExponent       RSA の公開指数データへのポインタ
    @param[in]  publicExponentSize    RSA の公開指数データのバイトサイズ
    @param[in]  pRawMessage           検証対象のデータへのポインタ。
    @param[in]  rawMessageSize        検証対象のデータのバイトサイズ。
    @param[in]  pWorkBuffer           ワークバッファへのポインタ。
    @param[in]  workBufferSize        ワークバッファのバイトサイズ。

    @return     署名の検証に成功したかを返します。@n
                失敗した場合は正しい署名データではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

    @pre
    - signatureSize == 512
    - modulusSize <= 512
    - publicExponentSize <= 3
    - workBufferSize >= 8192

    @details
    与えられた署名データが与えられたデータ列の正しい RSA-PSS-SHA-256 署名であることを検証します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    検証対象のデータのハッシュ値は内部で計算されます。

    ワークバッファは 8192 バイト以上が必要です。
 */
inline bool VerifyRsa4096PssSha256( const void* pSignature, size_t signatureSize,
                                    const void* pModulus, size_t modulusSize,
                                    const void* pPublicExponent, size_t publicExponentSize,
                                    const void* pRawMessage, size_t rawMessageSize,
                                    void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
{
    return Rsa4096PssSha256Verifier::Verify( pSignature, signatureSize,
                                             pModulus, modulusSize,
                                             pPublicExponent, publicExponentSize,
                                             pRawMessage, rawMessageSize,
                                             pWorkBuffer, workBufferSize );
}

//! @}

}} // namespace nn::crypto
