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
*   @brief  リソースデータ（ROM）関連の API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

//! @name リソースデータ（ROM）関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      nn::fs::MountRom のファイルシステムキャッシュに必要なサイズを取得します。
*
*   @param[out] pOutValue   ファイルシステムキャッシュに必要なサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess        成功しました。
*/
Result QueryMountRomCacheSize(size_t* pOutValue) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      リソースデータ（ROM）をマウントします。
*
*   @param[in]  name                      マウント名
*   @param[in]  pFileSystemCacheBuffer    ファイルシステムキャッシュのバッファ
*   @param[in]  fileSystemCacheBufferSize バッファのサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*/
Result MountRom(const char* name, void* pFileSystemCacheBuffer, size_t fileSystemCacheBufferSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
