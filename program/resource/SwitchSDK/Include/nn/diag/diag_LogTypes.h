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

/**
* @file
* @brief   ログ出力API関係の型定義
*/

#include <nn/nn_Common.h>
#include <cstdarg>

namespace nn { namespace diag {

/**
*   @brief      ログの文字列を生成するための情報です。
*/
struct LogMessage
{
    const char* format; //!< 書式文字列（暫定的なメンバ）
    std::va_list* args; //!< 書式文字列に与えられる引数（暫定的なメンバ）
};

/**
*   @brief      ログの重大性です。
*/
enum LogSeverity {
    LogSeverity_Trace,  //!< デバッグ時のみ関心がある情報を知らせる
    LogSeverity_Info,   //!< 想定通りのことが起きた
    LogSeverity_Warn,   //!< 想定外のことが起きた、もしくは起きそう
    LogSeverity_Error,  //!< そのモジュールの一部の機能が実行できない
    LogSeverity_Fatal   //!< モジュールの実行を継続できない
};

/**
*   @brief      ソースコードから得られる情報です。
*/
struct SourceInfo
{
    int lineNumber;             //!< ソースコード内での行番号
    const char* fileName;       //!< ソースコードのファイル名
    const char* functionName;   //!< 関数名
};

/**
*   @brief      ログのメタデータです。
*/
struct LogMetaData
{
    SourceInfo sourceInfo;          //!< ソースコードから得られる情報
    const char* moduleName;         //!< モジュール名
    LogSeverity severity;           //!< 重大性
    int verbosity;                  //!< 冗長性
    bool useDefaultLocaleCharset;   //!< ログのエンコーディングに UTF8 ではなく、ロケール既定の設定を使用している
    void* pAdditionalData;          //!< 追加のメタデータ（未使用）
    size_t additionalDataBytes;     //!< 追加のメタデータのバイト数（未使用）
};

}}
