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
    @brief RSA-OAEP-SHA-256 による復号化を行うための API の宣言
 */

#include <nn/crypto/crypto_RsaOaepDecryptor.h>
#include <nn/crypto/crypto_Sha256Generator.h>

namespace nn { namespace crypto {

/**
    @brief  鍵サイズが 2048bit の RSA-OAEP-SHA-256 復号化を行うクラスです。

    @details
    2048bit の RSA-OAEP-SHA-256 復号化を行います。
    詳しくは RsaOaepDecryptor を参照してください。
*/
typedef RsaOaepDecryptor<256, nn::crypto::Sha256Generator> Rsa2048OaepSha256Decryptor;

/**
    @brief  鍵サイズが 4096bit の RSA-OAEP-SHA-256 復号化を行うクラスです。

    @details
    4096bit の RSA-OAEP-SHA-256 復号化を行います。
    詳しくは RsaOaepDecryptor を参照してください。
*/
typedef RsaOaepDecryptor<512, nn::crypto::Sha256Generator> Rsa4096OaepSha256Decryptor;


//! @name RSA 暗号
//! @{

/**
    @brief      鍵サイズが 2048bit の RSA-OAEP-SHA-256 による復号化を行うユーティリティ関数です。作業領域としてスタックを利用します。

    @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
    @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ
    @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
    @param[in]  cipherSize           復号化するデータのバイトサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。

    @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられています。

    @pre
    - modulusSize <= 256
    - privateExponentSize <= 256
    - cipherSize == 256

    @post
    - pOutPlain に復号化された結果が書き込まれる。

    @details
    与えられたデータ列を復号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    ワークバッファとしてスタックから 4096 バイトが利用されます。
 */
inline size_t DecryptRsa2048OaepSha256(void* pOutPlain, size_t plainSize,
                                       const void* pModulus, size_t modulusSize,
                                       const void* pPrivateExponent, size_t privateExponentSize,
                                       const void* pCipher, size_t cipherSize,
                                       const void* pLabel, size_t labelSize) NN_NOEXCEPT
{
    return Rsa2048OaepSha256Decryptor::Decrypt( pOutPlain, plainSize,
                                                pModulus, modulusSize,
                                                pPrivateExponent, privateExponentSize,
                                                pCipher, cipherSize,
                                                pLabel, labelSize );
}

/**
    @brief      鍵サイズが 2048bit の RSA-OAEP-SHA-256 による復号化を行うユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
    @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ
    @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
    @param[in]  cipherSize           復号化するデータのバイトサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。
    @param[in]  pWorkBuffer          ワークバッファへのポインタ。
    @param[in]  workBufferSize       ワークバッファのバイトサイズ。

    @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

    @pre
    - modulusSize <= 256
    - privateExponentSize <= 256
    - cipherSize == 256
    - workBufferSize >= 4096

    @post
    - pOutPlain に復号化された結果が書き込まれる。

    @details
    与えられたデータ列を復号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    ワークバッファは 4096 バイト以上が必要です。
 */
inline size_t DecryptRsa2048OaepSha256(void* pOutPlain, size_t plainSize,
                                       const void* pModulus, size_t modulusSize,
                                       const void* pPrivateExponent, size_t privateExponentSize,
                                       const void* pCipher, size_t cipherSize,
                                       const void* pLabel, size_t labelSize,
                                       void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
{
    return Rsa2048OaepSha256Decryptor::Decrypt( pOutPlain, plainSize,
                                                pModulus, modulusSize,
                                                pPrivateExponent, privateExponentSize,
                                                pCipher, cipherSize,
                                                pLabel, labelSize,
                                                pWorkBuffer, workBufferSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-OAEP-SHA-256 による復号化を行うユーティリティ関数です。作業領域としてスタックを利用します。

    @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
    @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ
    @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
    @param[in]  cipherSize           復号化するデータのバイトサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。

    @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられています。

    @pre
    - modulusSize <= 512
    - privateExponentSize <= 512
    - cipherSize == 512

    @post
    - pOutPlain に復号化された結果が書き込まれる。

    @details
    与えられたデータ列を復号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    ワークバッファとしてスタックから 8192 バイトが利用されます。
 */
inline size_t DecryptRsa4096OaepSha256(void* pOutPlain, size_t plainSize,
                                       const void* pModulus, size_t modulusSize,
                                       const void* pPrivateExponent, size_t privateExponentSize,
                                       const void* pCipher, size_t cipherSize,
                                       const void* pLabel, size_t labelSize) NN_NOEXCEPT
{
    return Rsa4096OaepSha256Decryptor::Decrypt( pOutPlain, plainSize,
                                                pModulus, modulusSize,
                                                pPrivateExponent, privateExponentSize,
                                                pCipher, cipherSize,
                                                pLabel, labelSize );
}

/**
    @brief      鍵サイズが 4096bit の RSA-OAEP-SHA-256 による復号化を行うユーティリティ関数です。作業領域として与えられたワークバッファを利用します。

    @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
    @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
    @param[in]  pModulus             RSA の法データへのポインタ。
    @param[in]  modulusSize          RSA の法データのバイトサイズ。
    @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
    @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ
    @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
    @param[in]  cipherSize           復号化するデータのバイトサイズ。
    @param[in]  pLabel               ラベルのデータへのポインタ。
    @param[in]  labelSize            ラベルのバイトサイズ。
    @param[in]  pWorkBuffer          ワークバッファへのポインタ。
    @param[in]  workBufferSize       ワークバッファのバイトサイズ。

    @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

    @pre
    - modulusSize <= 512
    - privateExponentSize <= 512
    - cipherSize == 512
    - workBufferSize >= 8192

    @post
    - pOutPlain に復号化された結果が書き込まれる。

    @details
    与えられたデータ列を復号化します。

    法と指数データは常に符号なし、ビッグエンディアンとして扱います。

    ワークバッファは 8192 バイト以上が必要です。
 */
inline size_t DecryptRsa4096OaepSha256(void* pOutPlain, size_t plainSize,
                                       const void* pModulus, size_t modulusSize,
                                       const void* pPrivateExponent, size_t privateExponentSize,
                                       const void* pCipher, size_t cipherSize,
                                       const void* pLabel, size_t labelSize,
                                       void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
{
    return Rsa4096OaepSha256Decryptor::Decrypt( pOutPlain, plainSize,
                                                pModulus, modulusSize,
                                                pPrivateExponent, privateExponentSize,
                                                pCipher, cipherSize,
                                                pLabel, labelSize,
                                                pWorkBuffer, workBufferSize );
}

//! @}

}} // namespace nn::crypto
