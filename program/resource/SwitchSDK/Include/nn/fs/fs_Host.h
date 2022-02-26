/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
*   @file
*   @brief  ホスト PC 関連の API の宣言
*/

#pragma once

#include<nn/nn_Common.h>
#include<nn/nn_Result.h>

namespace nn { namespace fs {

//! @name ホスト PC 関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      ホスト PC 上ファイルシステムの指定されたディレクトリをファイルシステムとしてマウントし、操作できるようにします。
*
*   @param[in]  name       このファイルシステムを表すマウント名
*   @param[in]  rootPath   name に紐付けられるホスト PC 上のパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultPathNotFound           rootPath で表されるディレクトリが存在しません。
*   @retval     ResultTargetNotFound         ホスト PC に接続できませんでした。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*/
Result MountHost(const char* name, const char* rootPath) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ホスト PC 上のファイルシステムをマウントし、Windows 上のパスと同じパスで操作できるようにします。
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultTargetNotFound         ホスト PC に接続できませんでした。
*   @retval     ResultMountNameAlreadyExists ホスト PC 上のファイルシステムは既に存在しています。
*/
Result MountHostRoot() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      MountHostRoot() でマウントしたファイルシステムをアンマウントし、リソースを解放します。
*
*   @return     なし
*/
void UnmountHostRoot() NN_NOEXCEPT;

//! @}

}} // namespace nn::fs

