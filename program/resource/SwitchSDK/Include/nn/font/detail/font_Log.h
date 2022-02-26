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
    @brief      font モジュール用の、ログを出力する API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailStructuredSdkLog.h>

#define NN_DETAIL_FONT_TRACE(...)                   NN_DETAIL_STRUCTURED_SDK_LOG(font, Trace, 0, ##__VA_ARGS__)
#define NN_DETAIL_FONT_TRACE_V1(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Trace, 1, ##__VA_ARGS__)
#define NN_DETAIL_FONT_TRACE_V2(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Trace, 2, ##__VA_ARGS__)
#define NN_DETAIL_FONT_TRACE_V3(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Trace, 3, ##__VA_ARGS__)

#define NN_DETAIL_FONT_INFO(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(font, Info, 0, ##__VA_ARGS__)
#define NN_DETAIL_FONT_INFO_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(font, Info, 1, ##__VA_ARGS__)
#define NN_DETAIL_FONT_INFO_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(font, Info, 2, ##__VA_ARGS__)
#define NN_DETAIL_FONT_INFO_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(font, Info, 3, ##__VA_ARGS__)

#define NN_DETAIL_FONT_WARN(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(font, Warn, 0, ##__VA_ARGS__)
#define NN_DETAIL_FONT_WARN_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(font, Warn, 1, ##__VA_ARGS__)
#define NN_DETAIL_FONT_WARN_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(font, Warn, 2, ##__VA_ARGS__)
#define NN_DETAIL_FONT_WARN_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(font, Warn, 3, ##__VA_ARGS__)

#define NN_DETAIL_FONT_ERROR(...)                   NN_DETAIL_STRUCTURED_SDK_LOG(font, Error, 0, ##__VA_ARGS__)
#define NN_DETAIL_FONT_ERROR_V1(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Error, 1, ##__VA_ARGS__)
#define NN_DETAIL_FONT_ERROR_V2(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Error, 2, ##__VA_ARGS__)
#define NN_DETAIL_FONT_ERROR_V3(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Error, 3, ##__VA_ARGS__)

#define NN_DETAIL_FONT_FATAL(...)                   NN_DETAIL_STRUCTURED_SDK_LOG(font, Fatal, 0, ##__VA_ARGS__)
#define NN_DETAIL_FONT_FATAL_V1(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Fatal, 1, ##__VA_ARGS__)
#define NN_DETAIL_FONT_FATAL_V2(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Fatal, 2, ##__VA_ARGS__)
#define NN_DETAIL_FONT_FATAL_V3(...)                NN_DETAIL_STRUCTURED_SDK_LOG(font, Fatal, 3, ##__VA_ARGS__)

#define NN_DETAIL_FONT_VTRACE(format, args)         NN_DETAIL_STRUCTURED_SDK_VLOG(font, Trace, 0, format, args)
#define NN_DETAIL_FONT_VTRACE_V1(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Trace, 1, format, args)
#define NN_DETAIL_FONT_VTRACE_V2(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Trace, 2, format, args)
#define NN_DETAIL_FONT_VTRACE_V3(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Trace, 3, format, args)

#define NN_DETAIL_FONT_VINFO(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(font, Info, 0, format, args)
#define NN_DETAIL_FONT_VINFO_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(font, Info, 1, format, args)
#define NN_DETAIL_FONT_VINFO_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(font, Info, 2, format, args)
#define NN_DETAIL_FONT_VINFO_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(font, Info, 3, format, args)

#define NN_DETAIL_FONT_VWARN(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(font, Warn, 0, format, args)
#define NN_DETAIL_FONT_VWARN_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(font, Warn, 1, format, args)
#define NN_DETAIL_FONT_VWARN_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(font, Warn, 2, format, args)
#define NN_DETAIL_FONT_VWARN_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(font, Warn, 3, format, args)

#define NN_DETAIL_FONT_VERROR(format, args)         NN_DETAIL_STRUCTURED_SDK_VLOG(font, Error, 0, format, args)
#define NN_DETAIL_FONT_VERROR_V1(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Error, 1, format, args)
#define NN_DETAIL_FONT_VERROR_V2(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Error, 2, format, args)
#define NN_DETAIL_FONT_VERROR_V3(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Error, 3, format, args)

#define NN_DETAIL_FONT_VFATAL(format, args)         NN_DETAIL_STRUCTURED_SDK_VLOG(font, Fatal, 0, format, args)
#define NN_DETAIL_FONT_VFATAL_V1(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Fatal, 1, format, args)
#define NN_DETAIL_FONT_VFATAL_V2(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Fatal, 2, format, args)
#define NN_DETAIL_FONT_VFATAL_V3(format, args)      NN_DETAIL_STRUCTURED_SDK_VLOG(font, Fatal, 3, format, args)

#define NN_DETAIL_FONT_PUT_TRACE(text, length)      NN_DETAIL_STRUCTURED_SDK_PUT(font, Trace, 0, text, length)
#define NN_DETAIL_FONT_PUT_TRACE_V1(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Trace, 1, text, length)
#define NN_DETAIL_FONT_PUT_TRACE_V2(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Trace, 2, text, length)
#define NN_DETAIL_FONT_PUT_TRACE_V3(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Trace, 3, text, length)

#define NN_DETAIL_FONT_PUT_INFO(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(font, Info, 0, text, length)
#define NN_DETAIL_FONT_PUT_INFO_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(font, Info, 1, text, length)
#define NN_DETAIL_FONT_PUT_INFO_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(font, Info, 2, text, length)
#define NN_DETAIL_FONT_PUT_INFO_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(font, Info, 3, text, length)

#define NN_DETAIL_FONT_PUT_WARN(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(font, Warn, 0, text, length)
#define NN_DETAIL_FONT_PUT_WARN_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(font, Warn, 1, text, length)
#define NN_DETAIL_FONT_PUT_WARN_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(font, Warn, 2, text, length)
#define NN_DETAIL_FONT_PUT_WARN_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(font, Warn, 3, text, length)

#define NN_DETAIL_FONT_PUT_ERROR(text, length)      NN_DETAIL_STRUCTURED_SDK_PUT(font, Error, 0, text, length)
#define NN_DETAIL_FONT_PUT_ERROR_V1(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Error, 1, text, length)
#define NN_DETAIL_FONT_PUT_ERROR_V2(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Error, 2, text, length)
#define NN_DETAIL_FONT_PUT_ERROR_V3(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Error, 3, text, length)

#define NN_DETAIL_FONT_PUT_FATAL(text, length)      NN_DETAIL_STRUCTURED_SDK_PUT(font, Fatal, 0, text, length)
#define NN_DETAIL_FONT_PUT_FATAL_V1(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Fatal, 1, text, length)
#define NN_DETAIL_FONT_PUT_FATAL_V2(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Fatal, 2, text, length)
#define NN_DETAIL_FONT_PUT_FATAL_V3(text, length)   NN_DETAIL_STRUCTURED_SDK_PUT(font, Fatal, 3, text, length)
