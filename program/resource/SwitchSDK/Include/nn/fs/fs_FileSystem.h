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
*   @brief  ファイルシステムに関連するファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

    struct FileHandle;
    struct DirectoryHandle;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルオープン時のモードを指定するフラグです
*
*   @details
*       ファイルを開く際のモードを指定します。
*       必要に応じてビットごとの OR で複数指定してください。
*       読み込みモードのみでの共有は可能ですが、書き込み操作ありでの共有はできません。
*/
    enum OpenMode
    {
        OpenMode_Read = 1 << 0,          //!< 読み込みモードを指定します
        OpenMode_Write = 1 << 1,         //!< 書き込みモードを指定します
        OpenMode_AllowAppend = 1 << 2,   //!< 追記書き込み操作を許可します
    };

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリオープン時のモードを指定するフラグです
*
*   @details
*       ディレクトリ開く際のモードを指定します。
*       指定された種類の子エントリのみが、 ReadDirectory() などで列挙の対象となります。
*       ビットごとの OR で複数指定することも可能です。
*/
    enum OpenDirectoryMode
    {
        OpenDirectoryMode_Directory = 1 << 0,  //!< 子ディレクトリのみ列挙します
        OpenDirectoryMode_File = 1 << 1,       //!< 子ファイルのみ列挙します
        OpenDirectoryMode_All = OpenDirectoryMode_Directory | OpenDirectoryMode_File, //!< 全ての子エントリを列挙します
    };

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリエントリの種類を表す列挙型です
*
*   @details
*       ディレクトリエントリが、ファイルなのかディレクトリなのかを識別します。
*/
    enum DirectoryEntryType
    {
        DirectoryEntryType_Directory,    //!< ディレクトリです
        DirectoryEntryType_File,         //!< ファイルです
    };

//! @name ファイルシステム関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      ファイルを作成します。
*
*   @param[in]  path       新規に作成するファイルのパス
*   @param[in]  size       新規に作成するファイルのサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              パスに含まれるディレクトリが存在しません。
*   @retval     ResultPathAlreadyExists         対象ファイルはすでに存在しています。
*   @retval     ResultUsableSpaceNotEnough      空き容量が不足しています。
*/
Result CreateFile(const char* path, int64_t size) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルを削除します。
*
*   @param[in]  path       削除するファイルのパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象ファイルが存在しません。
*   @retval     ResultTargetLocked              対象ファイルが開かれているため削除できません。
*/
Result DeleteFile(const char* path) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリを作成します。
*
*   @param[in]  path       新規に作成するディレクトリのパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              パスに含まれるディレクトリが存在しません。
*   @retval     ResultPathAlreadyExists         対象ディレクトリはすでに存在しています。
*   @retval     ResultUsableSpaceNotEnough      空き容量が不足しています。
*/
Result CreateDirectory(const char* path) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリを削除します。
*
*   @param[in]  path       削除するディレクトリのパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象ディレクトリが存在しません。
*   @retval     ResultDirectoryNotEmpty         ディレクトリに子エントリが存在するので削除できません。
*   @retval     ResultTargetLocked              対象ディレクトリが開かれているため削除できません。
*                                               子エントリが開かれているため削除できません。
*/
Result DeleteDirectory(const char* path) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      子エントリも含め、ディレクトリを再帰的に削除します。
*
*   @param[in]  path       削除するディレクトリのパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象ディレクトリが存在しません。
*   @retval     ResultTargetLocked              対象ディレクトリが開かれているため削除できません。
*                                               子エントリが開かれているため削除できません。
*/
Result DeleteDirectoryRecursively(const char* path) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリに含まれる全ての子エントリを再帰的に削除します。
*
*   @param[in]  path       子エントリを削除するディレクトリのパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象ディレクトリが存在しません。
*   @retval     ResultTargetLocked              子エントリが開かれているため削除できません。
*/
Result CleanDirectoryRecursively(const char* path) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイル名を変更します。ファイルの移動も可能です。
*
*   @param[in]  currentPath 変更元のファイルパス
*   @param[in]  newPath     変更先のファイルパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              リネーム元ファイルが存在しません。
*                                               リネーム先のパスに含まれるディレクトリが存在しません。
*   @retval     ResultPathAlreadyExists         リネーム先ファイルがすでに存在しています。
*   @retval     ResultTargetLocked              リネーム元ファイルが開かれているのでリネームできません。
*/
Result RenameFile(const char* currentPath, const char* newPath) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリ名を変更します。ディレクトリの移動も可能です。
*
*   @param[in]  currentPath 変更元のディレクトリパス
*   @param[in]  newPath     変更先のディレクトリパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              リネーム元ディレクトリが存在しません。
*                                               リネーム先のパスに含まれるディレクトリが存在しません。
*   @retval     ResultPathAlreadyExists         リネーム先ディレクトリがすでに存在しています。
*   @retval     ResultTargetLocked              リネーム元のディレクトリが開かれているのでリネームできません。
*/
Result RenameDirectory(const char* currentPath, const char* newPath) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      エントリの種類を取得します。
*
*   @param[out] outValue    ディレクトリエントリの種類
*   @param[in]  path        対象エントリのパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象エントリが存在しません。
*/
Result GetEntryType(DirectoryEntryType* outValue, const char* path) NN_NOEXCEPT;

//! @}

//! @name ファイル関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      ファイルをオープンします。
*
*   @param[out] outValue    ファイルのハンドル
*   @param[in]  path        オープンするファイルのパス
*   @param[in]  mode        ファイルへのアクセスモード（OpenMode 型の要素、またはそのビット和）
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象ファイルが存在しません。
*   @retval     ResultTargetLocked              対象ファイルがすでに OpenMode_Write を指定して開かれているので、開けません。
*                                               対象ファイルが開かれているので、OpenMode_Write を指定して開けません。
*/
Result OpenFile(FileHandle* outValue, const char* path, int mode) NN_NOEXCEPT;

//! @}

//! @name ディレクトリ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      ディレクトリをオープンします。
*
*   @param[out] outValue    ディレクトリのハンドル
*   @param[in]  path        オープンするディレクトリのパス
*   @param[in]  mode        列挙する対象（OpenDirectoryMode 型の要素、またはそのビット和）
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultPathNotFound              対象ディレクトリが存在しません。
*/
Result OpenDirectory(DirectoryHandle* outValue, const char* path, int mode) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs

