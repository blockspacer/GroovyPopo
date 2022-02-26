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
    @brief      gfx モジュール用の、ログを出力する API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailStructuredSdkLog.h>

#define NN_DETAIL_GFX_TRACE(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Trace, 0, ##__VA_ARGS__)
#define NN_DETAIL_GFX_TRACE_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Trace, 1, ##__VA_ARGS__)
#define NN_DETAIL_GFX_TRACE_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Trace, 2, ##__VA_ARGS__)
#define NN_DETAIL_GFX_TRACE_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Trace, 3, ##__VA_ARGS__)

#define NN_DETAIL_GFX_INFO(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Info, 0, ##__VA_ARGS__)
#define NN_DETAIL_GFX_INFO_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Info, 1, ##__VA_ARGS__)
#define NN_DETAIL_GFX_INFO_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Info, 2, ##__VA_ARGS__)
#define NN_DETAIL_GFX_INFO_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Info, 3, ##__VA_ARGS__)

#define NN_DETAIL_GFX_WARN(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Warn, 0, ##__VA_ARGS__)
#define NN_DETAIL_GFX_WARN_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Warn, 1, ##__VA_ARGS__)
#define NN_DETAIL_GFX_WARN_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Warn, 2, ##__VA_ARGS__)
#define NN_DETAIL_GFX_WARN_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Warn, 3, ##__VA_ARGS__)

#define NN_DETAIL_GFX_ERROR(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Error, 0, ##__VA_ARGS__)
#define NN_DETAIL_GFX_ERROR_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Error, 1, ##__VA_ARGS__)
#define NN_DETAIL_GFX_ERROR_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Error, 2, ##__VA_ARGS__)
#define NN_DETAIL_GFX_ERROR_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Error, 3, ##__VA_ARGS__)

#define NN_DETAIL_GFX_FATAL(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Fatal, 0, ##__VA_ARGS__)
#define NN_DETAIL_GFX_FATAL_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Fatal, 1, ##__VA_ARGS__)
#define NN_DETAIL_GFX_FATAL_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Fatal, 2, ##__VA_ARGS__)
#define NN_DETAIL_GFX_FATAL_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(gfx, Fatal, 3, ##__VA_ARGS__)

#define NN_DETAIL_GFX_VTRACE(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Trace, 0, format, args)
#define NN_DETAIL_GFX_VTRACE_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Trace, 1, format, args)
#define NN_DETAIL_GFX_VTRACE_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Trace, 2, format, args)
#define NN_DETAIL_GFX_VTRACE_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Trace, 3, format, args)

#define NN_DETAIL_GFX_VINFO(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Info, 0, format, args)
#define NN_DETAIL_GFX_VINFO_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Info, 1, format, args)
#define NN_DETAIL_GFX_VINFO_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Info, 2, format, args)
#define NN_DETAIL_GFX_VINFO_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Info, 3, format, args)

#define NN_DETAIL_GFX_VWARN(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Warn, 0, format, args)
#define NN_DETAIL_GFX_VWARN_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Warn, 1, format, args)
#define NN_DETAIL_GFX_VWARN_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Warn, 2, format, args)
#define NN_DETAIL_GFX_VWARN_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Warn, 3, format, args)

#define NN_DETAIL_GFX_VERROR(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Error, 0, format, args)
#define NN_DETAIL_GFX_VERROR_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Error, 1, format, args)
#define NN_DETAIL_GFX_VERROR_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Error, 2, format, args)
#define NN_DETAIL_GFX_VERROR_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Error, 3, format, args)

#define NN_DETAIL_GFX_VFATAL(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Fatal, 0, format, args)
#define NN_DETAIL_GFX_VFATAL_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Fatal, 1, format, args)
#define NN_DETAIL_GFX_VFATAL_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Fatal, 2, format, args)
#define NN_DETAIL_GFX_VFATAL_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(gfx, Fatal, 3, format, args)

#define NN_DETAIL_GFX_PUT_TRACE(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Trace, 0, text, length)
#define NN_DETAIL_GFX_PUT_TRACE_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Trace, 1, text, length)
#define NN_DETAIL_GFX_PUT_TRACE_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Trace, 2, text, length)
#define NN_DETAIL_GFX_PUT_TRACE_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Trace, 3, text, length)

#define NN_DETAIL_GFX_PUT_INFO(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Info, 0, text, length)
#define NN_DETAIL_GFX_PUT_INFO_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Info, 1, text, length)
#define NN_DETAIL_GFX_PUT_INFO_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Info, 2, text, length)
#define NN_DETAIL_GFX_PUT_INFO_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Info, 3, text, length)

#define NN_DETAIL_GFX_PUT_WARN(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Warn, 0, text, length)
#define NN_DETAIL_GFX_PUT_WARN_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Warn, 1, text, length)
#define NN_DETAIL_GFX_PUT_WARN_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Warn, 2, text, length)
#define NN_DETAIL_GFX_PUT_WARN_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Warn, 3, text, length)

#define NN_DETAIL_GFX_PUT_ERROR(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Error, 0, text, length)
#define NN_DETAIL_GFX_PUT_ERROR_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Error, 1, text, length)
#define NN_DETAIL_GFX_PUT_ERROR_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Error, 2, text, length)
#define NN_DETAIL_GFX_PUT_ERROR_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Error, 3, text, length)

#define NN_DETAIL_GFX_PUT_FATAL(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Fatal, 0, text, length)
#define NN_DETAIL_GFX_PUT_FATAL_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Fatal, 1, text, length)
#define NN_DETAIL_GFX_PUT_FATAL_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Fatal, 2, text, length)
#define NN_DETAIL_GFX_PUT_FATAL_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(gfx, Fatal, 3, text, length)
