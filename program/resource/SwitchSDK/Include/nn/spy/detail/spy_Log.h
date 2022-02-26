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
    @file
    @brief      spy モジュール用の、ログを出力する API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailStructuredSdkLog.h>

#define NN_DETAIL_SPY_TRACE(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(spy, Trace, 0, ##__VA_ARGS__)
#define NN_DETAIL_SPY_TRACE_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Trace, 1, ##__VA_ARGS__)
#define NN_DETAIL_SPY_TRACE_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Trace, 2, ##__VA_ARGS__)
#define NN_DETAIL_SPY_TRACE_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Trace, 3, ##__VA_ARGS__)

#define NN_DETAIL_SPY_INFO(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(spy, Info, 0, ##__VA_ARGS__)
#define NN_DETAIL_SPY_INFO_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(spy, Info, 1, ##__VA_ARGS__)
#define NN_DETAIL_SPY_INFO_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(spy, Info, 2, ##__VA_ARGS__)
#define NN_DETAIL_SPY_INFO_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(spy, Info, 3, ##__VA_ARGS__)

#define NN_DETAIL_SPY_WARN(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(spy, Warn, 0, ##__VA_ARGS__)
#define NN_DETAIL_SPY_WARN_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(spy, Warn, 1, ##__VA_ARGS__)
#define NN_DETAIL_SPY_WARN_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(spy, Warn, 2, ##__VA_ARGS__)
#define NN_DETAIL_SPY_WARN_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(spy, Warn, 3, ##__VA_ARGS__)

#define NN_DETAIL_SPY_ERROR(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(spy, Error, 0, ##__VA_ARGS__)
#define NN_DETAIL_SPY_ERROR_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Error, 1, ##__VA_ARGS__)
#define NN_DETAIL_SPY_ERROR_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Error, 2, ##__VA_ARGS__)
#define NN_DETAIL_SPY_ERROR_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Error, 3, ##__VA_ARGS__)

#define NN_DETAIL_SPY_FATAL(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(spy, Fatal, 0, ##__VA_ARGS__)
#define NN_DETAIL_SPY_FATAL_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Fatal, 1, ##__VA_ARGS__)
#define NN_DETAIL_SPY_FATAL_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Fatal, 2, ##__VA_ARGS__)
#define NN_DETAIL_SPY_FATAL_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(spy, Fatal, 3, ##__VA_ARGS__)

#define NN_DETAIL_SPY_VTRACE(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Trace, 0, format, args)
#define NN_DETAIL_SPY_VTRACE_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Trace, 1, format, args)
#define NN_DETAIL_SPY_VTRACE_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Trace, 2, format, args)
#define NN_DETAIL_SPY_VTRACE_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Trace, 3, format, args)

#define NN_DETAIL_SPY_VINFO(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Info, 0, format, args)
#define NN_DETAIL_SPY_VINFO_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Info, 1, format, args)
#define NN_DETAIL_SPY_VINFO_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Info, 2, format, args)
#define NN_DETAIL_SPY_VINFO_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Info, 3, format, args)

#define NN_DETAIL_SPY_VWARN(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Warn, 0, format, args)
#define NN_DETAIL_SPY_VWARN_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Warn, 1, format, args)
#define NN_DETAIL_SPY_VWARN_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Warn, 2, format, args)
#define NN_DETAIL_SPY_VWARN_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Warn, 3, format, args)

#define NN_DETAIL_SPY_VERROR(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Error, 0, format, args)
#define NN_DETAIL_SPY_VERROR_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Error, 1, format, args)
#define NN_DETAIL_SPY_VERROR_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Error, 2, format, args)
#define NN_DETAIL_SPY_VERROR_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Error, 3, format, args)

#define NN_DETAIL_SPY_VFATAL(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Fatal, 0, format, args)
#define NN_DETAIL_SPY_VFATAL_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Fatal, 1, format, args)
#define NN_DETAIL_SPY_VFATAL_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Fatal, 2, format, args)
#define NN_DETAIL_SPY_VFATAL_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(spy, Fatal, 3, format, args)

#define NN_DETAIL_SPY_PUT_TRACE(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(spy, Trace, 0, text, length)
#define NN_DETAIL_SPY_PUT_TRACE_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Trace, 1, text, length)
#define NN_DETAIL_SPY_PUT_TRACE_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Trace, 2, text, length)
#define NN_DETAIL_SPY_PUT_TRACE_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Trace, 3, text, length)

#define NN_DETAIL_SPY_PUT_INFO(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(spy, Info, 0, text, length)
#define NN_DETAIL_SPY_PUT_INFO_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(spy, Info, 1, text, length)
#define NN_DETAIL_SPY_PUT_INFO_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(spy, Info, 2, text, length)
#define NN_DETAIL_SPY_PUT_INFO_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(spy, Info, 3, text, length)

#define NN_DETAIL_SPY_PUT_WARN(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(spy, Warn, 0, text, length)
#define NN_DETAIL_SPY_PUT_WARN_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(spy, Warn, 1, text, length)
#define NN_DETAIL_SPY_PUT_WARN_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(spy, Warn, 2, text, length)
#define NN_DETAIL_SPY_PUT_WARN_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(spy, Warn, 3, text, length)

#define NN_DETAIL_SPY_PUT_ERROR(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(spy, Error, 0, text, length)
#define NN_DETAIL_SPY_PUT_ERROR_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Error, 1, text, length)
#define NN_DETAIL_SPY_PUT_ERROR_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Error, 2, text, length)
#define NN_DETAIL_SPY_PUT_ERROR_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Error, 3, text, length)

#define NN_DETAIL_SPY_PUT_FATAL(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(spy, Fatal, 0, text, length)
#define NN_DETAIL_SPY_PUT_FATAL_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Fatal, 1, text, length)
#define NN_DETAIL_SPY_PUT_FATAL_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Fatal, 2, text, length)
#define NN_DETAIL_SPY_PUT_FATAL_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(spy, Fatal, 3, text, length)
