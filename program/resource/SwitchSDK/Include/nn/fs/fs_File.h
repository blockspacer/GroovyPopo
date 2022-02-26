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
*   @brief  ファイル関連のファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

/**
* @brief ファイル読み込み時のオプション指定子です。
*/
    struct ReadOption
    {
        int reserved;

        /**
        * @brief ReadOption を生成するユーティリティ関数です。
        *
        * @param[in] reserved 予約された引数
        *
        * @return ReadOption が返ります。
        *
        * @details reserved は将来的にファイル読み込み時のオプション指定フラグが定義された際に使用される引数です。@n
        *          現在は 0 を指定してください。
        */
        static ReadOption MakeValue(int reserved)
        {
            NN_UNUSED(reserved);
            ReadOption option = {0};
            return option;
        }

    };

/**
* @brief ファイル書き込み時のオプション指定フラグです。
*/
    enum WriteOptionFlag
    {
        WriteOptionFlag_Flush = 1 << 0, //!< 書き込みと同時にフラッシュする場合に指定します
    };

/**
* @brief ファイル書き込み時のオプション指定子です。
*/
    struct WriteOption
    {
        int flags;                 //!< WriteOptionFlag を必要に応じて組み合わせ、指定してください。

        /**
        * @brief 指定したオプション指定フラグを持った WriteOption を生成するユーティリティ関数です。
        *
        * @param[in] flags    WriteOptionFlag の要素、またはそのビット和
        *
        * @return 指定したオプション指定フラグを持った WriteOption が返ります。
        */
        static WriteOption MakeValue(int flags)
        {
            WriteOption option = {0};
            option.flags = flags;
            return option;
        }

    };

/**
* @brief ファイルを扱うためのハンドルです。
* @details
*        あるファイルハンドルに対して、内部の状態異常による操作の失敗が発生した場合、それ以降そのファイルハンドルに対しては異常状態フラグが立ちます。@n
*        異常状態にあるファイルハンドルに対して Close 以外の操作が行われた場合、適切なハンドリングを促すために開発者に通知が行われます。
*/
    struct FileHandle
    {
        void* handle;
    };

//! @name ファイル関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      サイズが既知であるファイルの内容を、指定されたメモリへ読み込みます。
*
*   @param[in]  handle          ファイルのハンドル
*   @param[in]  offset          読み込みを開始するファイル先頭からのオフセット
*   @param[in]  buffer          読み込みデータの格納先バッファ
*   @param[in]  size            読み込むバイトサイズ
*   @param[in]  option          読み込みオプション
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*
*   @overloadlist{nn_fs_ReadFile}
*/
Result ReadFile(FileHandle handle, int64_t offset, void* buffer, size_t size, const ReadOption& option) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      サイズが既知であるファイルの内容を、指定されたメモリへオプションなしで読み込みます。
*
*   @param[in]  handle          ファイルのハンドル
*   @param[in]  offset          読み込みを開始するファイル先頭からのオフセット
*   @param[in]  buffer          読み込みデータの格納先バッファ
*   @param[in]  size            読み込むバイトサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*
*   @overloadlist{nn_fs_ReadFile}
*/
Result ReadFile(FileHandle handle, int64_t offset, void* buffer, size_t size) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      サイズが不明なファイルの内容を、指定されたメモリへ読み込みます。
*
*   @param[out] outValue        読み込んだデータのバイトサイズ
*   @param[in]  handle          ファイルのハンドル
*   @param[in]  offset          読み込みを開始するファイル先頭からのオフセット
*   @param[in]  buffer          読み込みデータの格納先バッファ
*   @param[in]  size            読み込むバイトサイズ
*   @param[in]  option          読み込みオプション
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*
*   @overloadlist{nn_fs_ReadFile}
*/
Result ReadFile(size_t* outValue, FileHandle handle, int64_t offset, void* buffer, size_t size, const ReadOption& option) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      サイズが不明なファイルの内容を、指定されたメモリへオプションなしで読み込みます。
*
*   @param[out] outValue        読み込んだデータのバイトサイズ
*   @param[in]  handle          ファイルのハンドル
*   @param[in]  offset          読み込みを開始するファイル先頭からのオフセット
*   @param[in]  buffer          読み込みデータの格納先バッファ
*   @param[in]  size            読み込むバイトサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*
*   @overloadlist{nn_fs_ReadFile}
*/
Result ReadFile(size_t* outValue, FileHandle handle, int64_t offset, void* buffer, size_t size) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルへ指定されたメモリの内容を書き込みます。
*
*   @param[in]  handle          ファイルのハンドル
*   @param[in]  offset          書き込みを開始するファイル先頭からのオフセット
*   @param[in]  buffer          書き込むデータ
*   @param[in]  size            書き込むバイトサイズ
*   @param[in]  option          書き込みオプション
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultUsableSpaceNotEnough      空き容量が不足しています。
*/
Result WriteFile(FileHandle handle, int64_t offset, const void* buffer, size_t size, const WriteOption& option) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      WriteFile() で指定した内容を、確実に書き込まれた状態にします。
*
*   @param[in]  handle      ファイルのハンドル
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*/
Result FlushFile(FileHandle handle) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルのサイズを変更します。
*
*   @param[in]  handle      ファイルのハンドル
*   @param[in]  size        変更後のファイルサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*   @retval     ResultUsableSpaceNotEnough      空き容量が不足しています。
*/
Result SetFileSize(FileHandle handle, int64_t size) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルのサイズを取得します。
*
*   @param[out] outValue    ファイルサイズの格納先
*   @param[in]  handle      ファイルのハンドル
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*/
Result GetFileSize(int64_t* outValue, FileHandle handle) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルオープン時に指定したモードを取得します。
*
*   @param[in]  handle      ファイルのハンドル
*
*   @return     ファイルオープン時に指定したモード（OpenMode 型の要素、またはそのビット和）が返ります。
*/
int GetFileOpenMode(FileHandle handle) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ファイルをクローズします。
*
*   @param[in]  handle     クローズするファイルのハンドル
*
*   @return     なし
*/
void CloseFile(FileHandle handle) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs

