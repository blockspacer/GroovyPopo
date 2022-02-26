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
*   @brief  デバッグ用途のタイムスタンプ関連 API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/nn_StaticAssert.h>
#include <nn/time/time_PosixTime.h>

namespace nn { namespace fs {

//--------------------------------------------------------------------------
/**
*   @brief      ファイルのタイムスタンプ情報を定義した構造体です
*
*   @details
*       ファイルの作成日時、最終更新日時、最終アクセス日時がそれぞれ UNIX 時間で定義されています。
*/
struct FileTimeStamp
{
    nn::time::PosixTime create; //!< ファイルの作成日時です。
    nn::time::PosixTime modify; //!< ファイルの最終更新日時 です。
    nn::time::PosixTime access; //!< ファイルへの最終アクセス日時です。
    bool isLocalTime;           //!< true の時は、時差を含んだ現地時間を取得しています。false の時は、時差を含まない協定世界時(UTC)を取得しています。
    char reserved[7];
};
NN_STATIC_ASSERT(std::is_pod<FileTimeStamp>::value);

//! @name デバッグ用途 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      対象のパスのファイルのタイムスタンプを取得します。
*
*   @param[out] outTimeStamp    ファイルのタイムスタンプ
*   @param[in]  path            タイムスタンプを取得したいパス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultPathNotFound      対象のエントリが存在しません。
*
*   @pre
*       - outTimeStamp が有効なメモリを指している。
*       - path が @link entry_path 有効なパス @endlink である。
*
*   @details    この API はデバッグ用途に限り使用可能です。@n
*
*   @platformbegin{NX}
*       NX Addon 3.1.0 以降で、ホストファイルシステムで使用できます。@n
*       NX Addon 4.1.0 以降で、SDカードファイルシステムで使用できます。@n
*       各ファイルシステムの詳細は NintendoSDK ドキュメントの FS ライブラリ @confluencelink{107320200, 機能説明} を参照してください。
*   @platformend
*/
Result GetFileTimeStampForDebug(FileTimeStamp* outTimeStamp, const char* path) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
