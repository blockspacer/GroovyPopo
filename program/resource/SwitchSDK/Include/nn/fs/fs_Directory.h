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
*   @brief  ディレクトリ関連のファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

/**
* @brief ディレクトリを扱うためのハンドルです。
*/
    struct DirectoryHandle
    {
        void* handle;
    };

/**
* @brief ファイルシステム内での最大エントリ名長です
*/
    const int EntryNameLengthMax = 768;

/**
* @brief ディレクトリエントリを表す構造体です
*/
    struct DirectoryEntry
    {
        char name[EntryNameLengthMax + 1];  //!< エントリの名前
        char reserved1[3];
        int8_t directoryEntryType;          //!< エントリのエントリタイプ
        char reserved2[3];
        int64_t fileSize;                   //!< エントリがファイルの場合はファイルサイズ、ディレクトリの場合は 0
    };

//! @name ディレクトリ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリの子エントリを列挙します。
*
*   @param[out] outValue         格納された子エントリの個数
*   @param[in]  entryBuffer      子エントリの格納先バッファ
*   @param[in]  handle           ディレクトリのハンドル
*   @param[in]  entryBufferCount entryBuffer の要素数
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                       成功しました。
*/
Result ReadDirectory(int64_t* outValue, DirectoryEntry* entryBuffer, DirectoryHandle handle, int64_t entryBufferCount) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリの子エントリの個数を取得します。
*
*   @param[out] outValue        子エントリの個数
*   @param[in]  handle          ディレクトリのハンドル
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                       成功しました。
*/
Result GetDirectoryEntryCount(int64_t* outValue, DirectoryHandle handle) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリをクローズします。
*
*   @param[in]  handle     クローズするディレクトリのハンドル
*
*   @return     なし
*/
void CloseDirectory(DirectoryHandle handle) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs

