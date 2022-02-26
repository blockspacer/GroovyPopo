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
    @brief      ige モジュール用の、ログを出力する API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailStructuredSdkLog.h>

#define NN_DETAIL_IGE_TRACE(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(ige, Trace, 0, ##__VA_ARGS__)
#define NN_DETAIL_IGE_TRACE_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Trace, 1, ##__VA_ARGS__)
#define NN_DETAIL_IGE_TRACE_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Trace, 2, ##__VA_ARGS__)
#define NN_DETAIL_IGE_TRACE_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Trace, 3, ##__VA_ARGS__)

#define NN_DETAIL_IGE_INFO(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(ige, Info, 0, ##__VA_ARGS__)
#define NN_DETAIL_IGE_INFO_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(ige, Info, 1, ##__VA_ARGS__)
#define NN_DETAIL_IGE_INFO_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(ige, Info, 2, ##__VA_ARGS__)
#define NN_DETAIL_IGE_INFO_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(ige, Info, 3, ##__VA_ARGS__)

#define NN_DETAIL_IGE_WARN(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(ige, Warn, 0, ##__VA_ARGS__)
#define NN_DETAIL_IGE_WARN_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(ige, Warn, 1, ##__VA_ARGS__)
#define NN_DETAIL_IGE_WARN_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(ige, Warn, 2, ##__VA_ARGS__)
#define NN_DETAIL_IGE_WARN_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(ige, Warn, 3, ##__VA_ARGS__)

#define NN_DETAIL_IGE_ERROR(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(ige, Error, 0, ##__VA_ARGS__)
#define NN_DETAIL_IGE_ERROR_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Error, 1, ##__VA_ARGS__)
#define NN_DETAIL_IGE_ERROR_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Error, 2, ##__VA_ARGS__)
#define NN_DETAIL_IGE_ERROR_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Error, 3, ##__VA_ARGS__)

#define NN_DETAIL_IGE_FATAL(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(ige, Fatal, 0, ##__VA_ARGS__)
#define NN_DETAIL_IGE_FATAL_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Fatal, 1, ##__VA_ARGS__)
#define NN_DETAIL_IGE_FATAL_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Fatal, 2, ##__VA_ARGS__)
#define NN_DETAIL_IGE_FATAL_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(ige, Fatal, 3, ##__VA_ARGS__)

#define NN_DETAIL_IGE_VTRACE(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Trace, 0, format, args)
#define NN_DETAIL_IGE_VTRACE_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Trace, 1, format, args)
#define NN_DETAIL_IGE_VTRACE_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Trace, 2, format, args)
#define NN_DETAIL_IGE_VTRACE_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Trace, 3, format, args)

#define NN_DETAIL_IGE_VINFO(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Info, 0, format, args)
#define NN_DETAIL_IGE_VINFO_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Info, 1, format, args)
#define NN_DETAIL_IGE_VINFO_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Info, 2, format, args)
#define NN_DETAIL_IGE_VINFO_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Info, 3, format, args)

#define NN_DETAIL_IGE_VWARN(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Warn, 0, format, args)
#define NN_DETAIL_IGE_VWARN_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Warn, 1, format, args)
#define NN_DETAIL_IGE_VWARN_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Warn, 2, format, args)
#define NN_DETAIL_IGE_VWARN_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Warn, 3, format, args)

#define NN_DETAIL_IGE_VERROR(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Error, 0, format, args)
#define NN_DETAIL_IGE_VERROR_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Error, 1, format, args)
#define NN_DETAIL_IGE_VERROR_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Error, 2, format, args)
#define NN_DETAIL_IGE_VERROR_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Error, 3, format, args)

#define NN_DETAIL_IGE_VFATAL(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Fatal, 0, format, args)
#define NN_DETAIL_IGE_VFATAL_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Fatal, 1, format, args)
#define NN_DETAIL_IGE_VFATAL_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Fatal, 2, format, args)
#define NN_DETAIL_IGE_VFATAL_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(ige, Fatal, 3, format, args)

#define NN_DETAIL_IGE_PUT_TRACE(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(ige, Trace, 0, text, length)
#define NN_DETAIL_IGE_PUT_TRACE_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Trace, 1, text, length)
#define NN_DETAIL_IGE_PUT_TRACE_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Trace, 2, text, length)
#define NN_DETAIL_IGE_PUT_TRACE_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Trace, 3, text, length)

#define NN_DETAIL_IGE_PUT_INFO(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(ige, Info, 0, text, length)
#define NN_DETAIL_IGE_PUT_INFO_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(ige, Info, 1, text, length)
#define NN_DETAIL_IGE_PUT_INFO_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(ige, Info, 2, text, length)
#define NN_DETAIL_IGE_PUT_INFO_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(ige, Info, 3, text, length)

#define NN_DETAIL_IGE_PUT_WARN(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(ige, Warn, 0, text, length)
#define NN_DETAIL_IGE_PUT_WARN_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(ige, Warn, 1, text, length)
#define NN_DETAIL_IGE_PUT_WARN_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(ige, Warn, 2, text, length)
#define NN_DETAIL_IGE_PUT_WARN_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(ige, Warn, 3, text, length)

#define NN_DETAIL_IGE_PUT_ERROR(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(ige, Error, 0, text, length)
#define NN_DETAIL_IGE_PUT_ERROR_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Error, 1, text, length)
#define NN_DETAIL_IGE_PUT_ERROR_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Error, 2, text, length)
#define NN_DETAIL_IGE_PUT_ERROR_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Error, 3, text, length)

#define NN_DETAIL_IGE_PUT_FATAL(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(ige, Fatal, 0, text, length)
#define NN_DETAIL_IGE_PUT_FATAL_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Fatal, 1, text, length)
#define NN_DETAIL_IGE_PUT_FATAL_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Fatal, 2, text, length)
#define NN_DETAIL_IGE_PUT_FATAL_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(ige, Fatal, 3, text, length)
