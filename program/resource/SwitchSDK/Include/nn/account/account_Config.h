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

#include <nn/nn_Common.h>

/** @file
    @brief  アカウントシステムのスペックを記述します。
 */

namespace nn { namespace account {

//! @name アカウントシステムの数値的な性能
//! @{

/**
    @brief アカウントシステムが同時に管理可能なユーザーの最大数
    @details
        アカウントシステムは、この値を上限としてユーザーを同時に管理できます。
        アカウントシステムがこの数のユーザーを既に管理している場合、新たにユーザーを追加することはできません。
*/
const int UserCountMax = 8;

/**
    @brief ユーザーに設定可能なニックネームの最大バイト数
    @details
        ユーザーに設定可能なニックネームは、終端文字を含まずこのバイト数を上限とします。
*/
const size_t NicknameBytesMax = 32;

/**
    @brief ユーザーに設定可能なプロフィール画像の最大バイト数
    @details
        ユーザーに設定可能なプロフィール画像はこのバイト数を上限とします。
*/
const size_t ProfileImageBytesMax = 128 * 1024;

/**
    @brief ネットワークサービスアカウントのIDトークンの最大長
    @details
        ネットワークサービスアカウントのIDトークン(ASCIIコード列)は、終端文字を含まずこの長さを上限とします。
*/
const size_t NetworkServiceAccountIdTokenLengthMax = 3072;

/**
    @brief ニンテンドーアカウントの認可コードの最大長
    @details
        ニンテンドーアカウントの認可コード(ASCIIコード列)の最大長は、終端文字を含まずこの長さを上限とします。
*/
const size_t NintendoAccountAuthorizationCodeLengthMax = 512;

/**
    @brief ニンテンドーアカウントのIDトークンの最大長
    @details
        ニンテンドーアカウントのIDトークン(ASCIIコード列)の最大長は、終端文字を含まずこの長さを上限とします。
*/
const size_t NintendoAccountIdTokenLengthMax = 1536;

/**
    @brief 有効な nn::account::NintendoAccountAuthorizationRequestContext オブジェクトの作成に必要なバッファの大きさ(バイト数)
*/
const size_t RequiredBufferSizeForNintendoAccountAuthorizationRequestContext = 4096;

//! @}

}} // ~namespace nn::account
