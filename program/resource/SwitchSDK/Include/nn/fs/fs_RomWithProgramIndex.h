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
*   @brief  マルチプログラムアプリケーション向けのリソースデータ（ROM）関連の API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

//! @name マルチプログラムアプリケーション向けのリソースデータ（ROM）関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      指定したプログラムに対する nn::fs::MountRom(const char*, int, void*, size_t) が実行可能かを取得します。
*
*   @pre
*       - マウントされている数が @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限未満である。
*
*   @details    この API はデバッグ用途に限り使用可能です。
*
*   @param[in]  programIndex 対象のプログラムのインデックス
*
*   @return     処理の結果が返ります。
*   @retval     true                            MountRom(const char*, int, void*, size_t) が実行可能です。
*   @retval     false                           MountRom(const char*, int, void*, size_t) が実行できません。
*/
bool CanMountRomForDebug(int programIndex) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      指定したプログラムに対する nn::fs::MountRom(const char*, int, void*, size_t) のファイルシステムキャッシュに必要なサイズを取得します。
*
*   @pre
*       - 対象のリソースデータ（ROM）が存在する。
*       - pOutValue が有効なメモリを指している。
*       - マウントされている数が @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限未満である。
*
*   @details    対象のプログラムを指定する必要があること以外は通常の nn::fs::QueryMountRomCacheSize(size_t*) と同様です。詳細については nn::fs::QueryMountRomCacheSize(size_t*) も併せて参照してください。
*
*   @param[out] pOutValue    ファイルシステムキャッシュに必要なサイズ
*   @param[in]  programIndex 対象のプログラムのインデックス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess        成功しました。
*/
Result QueryMountRomCacheSize(size_t* pOutValue, int programIndex) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      指定したプログラムのリソースデータ（ROM）をマウントします。
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - 対象のリソースデータ（ROM）が存在する。
*       - pFileSystemCacheBuffer が有効なメモリを指している。
*       - fileSystemCacheBufferSize が QueryMountRomCacheSize(size_t* pOutValue, int) で取得できるサイズ以上である。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*
*   @details    対象のプログラムを指定する以外は nn::fs::MountRom(const char*, void*, size_t) と同様です。詳細については nn::fs::MountRom(const char*, void*, size_t) も併せて参照してください。
*
*   @param[in]  name                      マウント名
*   @param[in]  programIndex              対象のプログラムのインデックス
*   @param[in]  pFileSystemCacheBuffer    ファイルシステムキャッシュのバッファ
*   @param[in]  fileSystemCacheBufferSize バッファのサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*/
Result MountRom(const char* name, int programIndex, void* pFileSystemCacheBuffer, size_t fileSystemCacheBufferSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
