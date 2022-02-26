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
* @brief    構造化ログの実装です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>
#include <nn/diag/detail/diag_DetailLog.h>

#if defined(NN_DETAIL_ENABLE_LOG) || defined(NN_DETAIL_ENABLE_SDK_LOG)

#if defined(NN_LOG_USE_DEFAULT_LOCALE_CHARSET) && defined(NN_BUILD_CONFIG_OS_WIN)
#define NN_DETAIL_LOG_USE_DEFAULT_LOCALE_CHARSET_VALUE true
#else
#define NN_DETAIL_LOG_USE_DEFAULT_LOCALE_CHARSET_VALUE false
#endif

#define NN_DETAIL_SETUP_LOG_META_DATA(                                  \
    logMetaData, inModule, inSeverity, inVerbosity)                     \
    do {                                                                \
        logMetaData.sourceInfo.lineNumber = __LINE__;                   \
        logMetaData.sourceInfo.fileName = __FILE__;                     \
        logMetaData.sourceInfo.functionName = NN_CURRENT_FUNCTION_NAME; \
        logMetaData.moduleName = inModule;                              \
        logMetaData.severity = inSeverity;                              \
        logMetaData.verbosity = inVerbosity;                            \
        logMetaData.useDefaultLocaleCharset = NN_DETAIL_LOG_USE_DEFAULT_LOCALE_CHARSET_VALUE; \
        logMetaData.pAdditionalData = static_cast<void*>(0);            \
        logMetaData.additionalDataBytes = 0;                            \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_STRUCTURED_LOG_IMPL(                                  \
    module, severity, verbosity, ...)                                   \
    do {                                                                \
        ::nn::diag::LogMetaData logMetaData;                            \
        NN_DETAIL_SETUP_LOG_META_DATA(                                  \
            logMetaData, module, severity, verbosity);                  \
        ::nn::diag::detail::LogImpl(logMetaData, __VA_ARGS__);          \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_STRUCTURED_VLOG_IMPL(                                 \
    module, severity, verbosity, format, args)                          \
    do {                                                                \
        ::nn::diag::LogMetaData logMetaData;                            \
        NN_DETAIL_SETUP_LOG_META_DATA(                                  \
            logMetaData, module, severity, verbosity);                  \
        ::nn::diag::detail::VLogImpl(logMetaData, format, args);        \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_STRUCTURED_PUT_IMPL(                                  \
    module, severity, verbosity, text, length)                          \
    do {                                                                \
        ::nn::diag::LogMetaData logMetaData;                            \
        NN_DETAIL_SETUP_LOG_META_DATA(                                  \
            logMetaData, module, severity, verbosity);                  \
        ::nn::diag::detail::PutImpl(logMetaData, text, length);         \
    } while (NN_STATIC_CONDITION(0))

#endif // #if defined(NN_DETAIL_ENABLE_LOG) || defined(NN_DETAIL_ENABLE_SDK_LOG)
