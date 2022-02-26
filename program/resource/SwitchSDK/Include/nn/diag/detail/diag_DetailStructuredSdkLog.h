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
* @brief    SDK 用の、構造化ログを出力する API の実装です。
* @detail   これらは、モジュールごとにラップした API から使用します。
*           モジュールごとにラップした API は Tools/CommandLineTools/MakeSdkLogHeader.exe で生成します。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>
#include <nn/diag/detail/diag_DetailStructuredLog.h>

#if defined(NN_DETAIL_ENABLE_SDK_LOG)

/**
* @brief    引数に指定された文字列を、ログとして出力します。
*
* @param[in] module     ログを出力するモジュールの名前
* @param[in] severity   ログの重大性
* @param[in] verbosity  ログの冗長性
* @param[in] ...        printf 形式で指定する、ログとして出力する文字列の書式指定文字列と、それに与える引数
*/
#define NN_DETAIL_STRUCTURED_SDK_LOG(                                       \
    module, severity, verbosity, ...)                                       \
    NN_DETAIL_STRUCTURED_LOG_IMPL(                                          \
        "$" NN_MACRO_STRINGIZE(module), ::nn::diag::LogSeverity_##severity, verbosity, __VA_ARGS__)

/**
* @brief    引数に指定された文字列を、ログとして出力します。引数を可変長引数リストで与えることができます。
*
* @param[in] module     ログを出力するモジュールの名前
* @param[in] severity   ログの重大性
* @param[in] verbosity  ログの冗長性
* @param[in] format     printf 形式で指定する書式指定文字列
* @param[in] args       書式指定文字列に与える引数
*/
#define NN_DETAIL_STRUCTURED_SDK_VLOG(                                      \
    module, severity, verbosity, format, args)                              \
    NN_DETAIL_STRUCTURED_VLOG_IMPL(                                         \
        "$" NN_MACRO_STRINGIZE(module), ::nn::diag::LogSeverity_##severity, verbosity, format, args)

/**
* @brief    引数に指定された文字列を、ログとして出力します。出力する文字列のバイトサイズを引数で指定します。
*
* @param[in] module     ログを出力するモジュールの名前
* @param[in] severity   ログの重大性
* @param[in] verbosity  ログの冗長性
* @param[in] text       出力する文字列
* @param[in] length     出力する文字列のバイトサイズ
*/
#define NN_DETAIL_STRUCTURED_SDK_PUT(                                       \
    module, severity, verbosity, text, length)                              \
    NN_DETAIL_STRUCTURED_PUT_IMPL(                                          \
        "$" NN_MACRO_STRINGIZE(module), ::nn::diag::LogSeverity_##severity, verbosity, text, length)

#else // #if defined(NN_DETAIL_ENABLE_SDK_LOG)

#define NN_DETAIL_STRUCTURED_SDK_LOG(module, severity, verbosity, ...)              static_cast<void>(0)
#define NN_DETAIL_STRUCTURED_SDK_VLOG(module, severity, verbosity, format, args)    static_cast<void>(0)
#define NN_DETAIL_STRUCTURED_SDK_PUT(module, severity, verbosity, text, length)     static_cast<void>(0)

#endif // #if defined(NN_DETAIL_ENABLE_SDK_LOG)
