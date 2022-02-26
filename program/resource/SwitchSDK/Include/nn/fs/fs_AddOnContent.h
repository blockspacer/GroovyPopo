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
*   @brief  追加コンテンツ関連 API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/aoc/aoc_Types.h>

namespace nn { namespace fs {

//! @name 追加コンテンツ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      MountAddOnContent のファイルシステムキャッシュに必要なサイズを取得します。
*
*   @param[out] pOutValue                       ファイルシステムキャッシュに必要なサイズ
*   @param[in]  targetIndex                     対象追加コンテンツの追加コンテンツインデックス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*
*   @pre
*       - pOutValue が有効なメモリを指している。
*       - 対象の追加コンテンツがインストール済みである。
*       - マウントされている数が @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限未満である。
*/
Result QueryMountAddOnContentCacheSize( size_t* pOutValue,
                                        nn::aoc::AddOnContentIndex  targetIndex) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
*   @brief      追加コンテンツを扱う読み込み専用のファイルシステムをマウントします。
*
*   @param[in]  name                            マウント名
*   @param[in]  targetIndex                     対象追加コンテンツの追加コンテンツインデックス
*   @param[in]  pFileSystemCacheBuffer          ファイルシステムキャッシュのバッファ
*   @param[in]  fileSystemCacheBufferSize       バッファのサイズ（必要なバッファサイズは QueryMountAddOnContentCacheSize(size_t* pOutValue, nn::aoc::AddOnContentIndex  targetIndex) で取得できます）
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - pFileSystemCacheBuffer != nullptr
*       - fileSystemCacheBufferSize が QueryMountAddOnContentCacheSize(size_t* pOutValue, nn::aoc::AddOnContentIndex  targetIndex) で取得できるサイズ以上
*       - 対象の追加コンテンツがインストール済みである。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*/
Result MountAddOnContent(   const char*                 name,
                            nn::aoc::AddOnContentIndex  targetIndex,
                            void*                       pFileSystemCacheBuffer,
                            size_t                      fileSystemCacheBufferSize) NN_NOEXCEPT;


//! @}

}} // namespace nn::fs
