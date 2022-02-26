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
    @brief      mem モジュール用の、ログを出力する API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailStructuredSdkLog.h>

#define NN_DETAIL_MEM_TRACE(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(mem, Trace, 0, ##__VA_ARGS__)
#define NN_DETAIL_MEM_TRACE_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Trace, 1, ##__VA_ARGS__)
#define NN_DETAIL_MEM_TRACE_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Trace, 2, ##__VA_ARGS__)
#define NN_DETAIL_MEM_TRACE_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Trace, 3, ##__VA_ARGS__)

#define NN_DETAIL_MEM_INFO(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(mem, Info, 0, ##__VA_ARGS__)
#define NN_DETAIL_MEM_INFO_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(mem, Info, 1, ##__VA_ARGS__)
#define NN_DETAIL_MEM_INFO_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(mem, Info, 2, ##__VA_ARGS__)
#define NN_DETAIL_MEM_INFO_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(mem, Info, 3, ##__VA_ARGS__)

#define NN_DETAIL_MEM_WARN(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(mem, Warn, 0, ##__VA_ARGS__)
#define NN_DETAIL_MEM_WARN_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(mem, Warn, 1, ##__VA_ARGS__)
#define NN_DETAIL_MEM_WARN_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(mem, Warn, 2, ##__VA_ARGS__)
#define NN_DETAIL_MEM_WARN_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(mem, Warn, 3, ##__VA_ARGS__)

#define NN_DETAIL_MEM_ERROR(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(mem, Error, 0, ##__VA_ARGS__)
#define NN_DETAIL_MEM_ERROR_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Error, 1, ##__VA_ARGS__)
#define NN_DETAIL_MEM_ERROR_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Error, 2, ##__VA_ARGS__)
#define NN_DETAIL_MEM_ERROR_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Error, 3, ##__VA_ARGS__)

#define NN_DETAIL_MEM_FATAL(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(mem, Fatal, 0, ##__VA_ARGS__)
#define NN_DETAIL_MEM_FATAL_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Fatal, 1, ##__VA_ARGS__)
#define NN_DETAIL_MEM_FATAL_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Fatal, 2, ##__VA_ARGS__)
#define NN_DETAIL_MEM_FATAL_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(mem, Fatal, 3, ##__VA_ARGS__)

#define NN_DETAIL_MEM_VTRACE(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Trace, 0, format, args)
#define NN_DETAIL_MEM_VTRACE_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Trace, 1, format, args)
#define NN_DETAIL_MEM_VTRACE_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Trace, 2, format, args)
#define NN_DETAIL_MEM_VTRACE_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Trace, 3, format, args)

#define NN_DETAIL_MEM_VINFO(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Info, 0, format, args)
#define NN_DETAIL_MEM_VINFO_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Info, 1, format, args)
#define NN_DETAIL_MEM_VINFO_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Info, 2, format, args)
#define NN_DETAIL_MEM_VINFO_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Info, 3, format, args)

#define NN_DETAIL_MEM_VWARN(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Warn, 0, format, args)
#define NN_DETAIL_MEM_VWARN_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Warn, 1, format, args)
#define NN_DETAIL_MEM_VWARN_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Warn, 2, format, args)
#define NN_DETAIL_MEM_VWARN_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Warn, 3, format, args)

#define NN_DETAIL_MEM_VERROR(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Error, 0, format, args)
#define NN_DETAIL_MEM_VERROR_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Error, 1, format, args)
#define NN_DETAIL_MEM_VERROR_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Error, 2, format, args)
#define NN_DETAIL_MEM_VERROR_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Error, 3, format, args)

#define NN_DETAIL_MEM_VFATAL(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Fatal, 0, format, args)
#define NN_DETAIL_MEM_VFATAL_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Fatal, 1, format, args)
#define NN_DETAIL_MEM_VFATAL_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Fatal, 2, format, args)
#define NN_DETAIL_MEM_VFATAL_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(mem, Fatal, 3, format, args)

#define NN_DETAIL_MEM_PUT_TRACE(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(mem, Trace, 0, text, length)
#define NN_DETAIL_MEM_PUT_TRACE_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Trace, 1, text, length)
#define NN_DETAIL_MEM_PUT_TRACE_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Trace, 2, text, length)
#define NN_DETAIL_MEM_PUT_TRACE_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Trace, 3, text, length)

#define NN_DETAIL_MEM_PUT_INFO(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(mem, Info, 0, text, length)
#define NN_DETAIL_MEM_PUT_INFO_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(mem, Info, 1, text, length)
#define NN_DETAIL_MEM_PUT_INFO_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(mem, Info, 2, text, length)
#define NN_DETAIL_MEM_PUT_INFO_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(mem, Info, 3, text, length)

#define NN_DETAIL_MEM_PUT_WARN(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(mem, Warn, 0, text, length)
#define NN_DETAIL_MEM_PUT_WARN_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(mem, Warn, 1, text, length)
#define NN_DETAIL_MEM_PUT_WARN_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(mem, Warn, 2, text, length)
#define NN_DETAIL_MEM_PUT_WARN_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(mem, Warn, 3, text, length)

#define NN_DETAIL_MEM_PUT_ERROR(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(mem, Error, 0, text, length)
#define NN_DETAIL_MEM_PUT_ERROR_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Error, 1, text, length)
#define NN_DETAIL_MEM_PUT_ERROR_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Error, 2, text, length)
#define NN_DETAIL_MEM_PUT_ERROR_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Error, 3, text, length)

#define NN_DETAIL_MEM_PUT_FATAL(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(mem, Fatal, 0, text, length)
#define NN_DETAIL_MEM_PUT_FATAL_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Fatal, 1, text, length)
#define NN_DETAIL_MEM_PUT_FATAL_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Fatal, 2, text, length)
#define NN_DETAIL_MEM_PUT_FATAL_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(mem, Fatal, 3, text, length)
