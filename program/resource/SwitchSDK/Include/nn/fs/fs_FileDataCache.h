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
*   @brief  ファイルデータキャッシュ関連の API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

//! @name ファイルデータキャッシュ関連 API
//! @{

/**
*   @brief      グローバルファイルデータキャッシュを有効化します。
*
*   @param[in]  pBuffer       キャッシュに使用するバッファ
*   @param[in]  bufferSize    バッファのサイズ
*
*   @return     なし
*/
void EnableGlobalFileDataCache(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

/**
*   @brief      グローバルファイルデータキャッシュを無効化します。
*
*   @return     なし
*/
void DisableGlobalFileDataCache() NN_NOEXCEPT;

/**
*   @brief      個別ファイルデータキャッシュを有効化します。
*
*   @param[in]  path          対象ファイルのパス
*   @param[in]  pBuffer       キャッシュに使用するバッファ
*   @param[in]  bufferSize    バッファのサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                               成功しました。
*   @retval     ResultPathNotFound                          対象ファイルが存在しません。
*   @retval     ResultIndividualFileDataCacheAlreadyEnabled 対象ファイルがすでにキャッシュされています。
*   @retval     ResultTargetLocked                          対象ファイルがすでに OpenMode_Write を指定して開かれているので、開けません。
*/
Result EnableIndividualFileDataCache(const char* path, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

/**
*   @brief      個別ファイルデータキャッシュを無効化します。
*
*   @param[in]  path          対象ファイルのパス
*
*   @return     なし
*/
void DisableIndividualFileDataCache(const char* path) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
