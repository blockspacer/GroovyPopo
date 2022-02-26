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
* @brief   SDK 用の、ログを出力する API の宣言
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>
#include <nn/diag/detail/diag_DetailStructuredLog.h>

#if defined(NN_DETAIL_ENABLE_SDK_LOG) || defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

#ifdef NN_SDK_LOG_DEFAULT_MODULE_NAME
#define NN_DETAIL_SDK_LOG_MODULE_NAME    "$" NN_MACRO_STRINGIZE(NN_SDK_LOG_DEFAULT_MODULE_NAME)
#else
#define NN_DETAIL_SDK_LOG_MODULE_NAME    "$"
#endif

/**
* @brief 引数に指定された文字列を、ログとして出力します。
*
* @param[in] ...    printf 形式で指定する、ログとして出力する文字列の書式指定文字列と、それに与える引数
*/
#define NN_SDK_LOG(...)             \
    NN_DETAIL_STRUCTURED_LOG_IMPL(NN_DETAIL_SDK_LOG_MODULE_NAME, ::nn::diag::LogSeverity_Info, 0, __VA_ARGS__)

/**
* @brief 引数に指定された文字列を、ログとして出力します。引数を可変長引数リストで与えることができます。
*
* @param[in] format printf 形式で指定する書式指定文字列
* @param[in] args   書式指定文字列に与える引数
*/
#define NN_SDK_VLOG(format, args)   \
    NN_DETAIL_STRUCTURED_VLOG_IMPL(NN_DETAIL_SDK_LOG_MODULE_NAME, ::nn::diag::LogSeverity_Info, 0, format, args)

/**
* @brief 引数に指定された文字列を、ログとして出力します。出力する文字列のバイトサイズを引数で指定します。
*
* @param[in] text   出力する文字列
* @param[in] length 出力する文字列のバイトサイズ
*/
#define NN_SDK_PUT(text, length)    \
    NN_DETAIL_STRUCTURED_PUT_IMPL(NN_DETAIL_SDK_LOG_MODULE_NAME, ::nn::diag::LogSeverity_Info, 0, text, length)

#else // #if defined(NN_DETAIL_ENABLE_SDK_LOG)

#define NN_SDK_LOG(...)             static_cast<void>(0)
#define NN_SDK_VLOG(format, args)   static_cast<void>(0)
#define NN_SDK_PUT(text, length)    static_cast<void>(0)

#endif // #if defined(NN_DETAIL_ENABLE_SDK_LOG)
