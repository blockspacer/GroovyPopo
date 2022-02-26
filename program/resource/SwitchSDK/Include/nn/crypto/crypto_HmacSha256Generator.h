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
    @brief HMAC-SHA-256 の計算を行う API の宣言
 */

#include <nn/crypto/crypto_Sha256Generator.h>
#include <nn/crypto/crypto_HmacGenerator.h>

namespace nn { namespace crypto {

/**
    @brief      HMAC-SHA-256 を計算するクラスです。

    @details
    SHA-256 を利用して MAC を計算します。
    詳細は HmacGenerator を参照してください。
 */
typedef HmacGenerator<Sha256Generator> HmacSha256Generator;

//! @name MAC
//! @{

/**
    @brief    HMAC-SHA-256 を計算するユーティリティ関数です。

    @param[out] pMac     MAC を格納するバッファへのポインタ。
    @param[in]  macSize  pMac が指すバッファのバイトサイズ。
    @param[in]  pData    入力データへのポインタ。
    @param[in]  dataSize 入力データのバイトサイズ。
    @param[in]  pKey     鍵データへのポインタ。
    @param[in]  keySize  鍵データのバイトサイズ。

    @post
    - pMac に MAC が書き込まれる

    @details
    与えられたデータ列の MAC を計算します。
    複数に分割されたデータ列の MAC を計算したい場合は @ref HmacSha256Generator クラスを利用してください。
 */
void   GenerateHmacSha256Mac(void* pMac, size_t macSize,
                             const void* pData, size_t dataSize,
                             const void* pKey, size_t keySize) NN_NOEXCEPT;

//! @}

}} // namespace nn::crypto

