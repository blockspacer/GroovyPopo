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
    @brief      atk モジュール用の、ログを出力する API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailStructuredSdkLog.h>

#define NN_DETAIL_ATK_TRACE(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(atk, Trace, 0, ##__VA_ARGS__)
#define NN_DETAIL_ATK_TRACE_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Trace, 1, ##__VA_ARGS__)
#define NN_DETAIL_ATK_TRACE_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Trace, 2, ##__VA_ARGS__)
#define NN_DETAIL_ATK_TRACE_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Trace, 3, ##__VA_ARGS__)

#define NN_DETAIL_ATK_INFO(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(atk, Info, 0, ##__VA_ARGS__)
#define NN_DETAIL_ATK_INFO_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(atk, Info, 1, ##__VA_ARGS__)
#define NN_DETAIL_ATK_INFO_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(atk, Info, 2, ##__VA_ARGS__)
#define NN_DETAIL_ATK_INFO_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(atk, Info, 3, ##__VA_ARGS__)

#define NN_DETAIL_ATK_WARN(...)                     NN_DETAIL_STRUCTURED_SDK_LOG(atk, Warn, 0, ##__VA_ARGS__)
#define NN_DETAIL_ATK_WARN_V1(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(atk, Warn, 1, ##__VA_ARGS__)
#define NN_DETAIL_ATK_WARN_V2(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(atk, Warn, 2, ##__VA_ARGS__)
#define NN_DETAIL_ATK_WARN_V3(...)                  NN_DETAIL_STRUCTURED_SDK_LOG(atk, Warn, 3, ##__VA_ARGS__)

#define NN_DETAIL_ATK_ERROR(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(atk, Error, 0, ##__VA_ARGS__)
#define NN_DETAIL_ATK_ERROR_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Error, 1, ##__VA_ARGS__)
#define NN_DETAIL_ATK_ERROR_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Error, 2, ##__VA_ARGS__)
#define NN_DETAIL_ATK_ERROR_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Error, 3, ##__VA_ARGS__)

#define NN_DETAIL_ATK_FATAL(...)                    NN_DETAIL_STRUCTURED_SDK_LOG(atk, Fatal, 0, ##__VA_ARGS__)
#define NN_DETAIL_ATK_FATAL_V1(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Fatal, 1, ##__VA_ARGS__)
#define NN_DETAIL_ATK_FATAL_V2(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Fatal, 2, ##__VA_ARGS__)
#define NN_DETAIL_ATK_FATAL_V3(...)                 NN_DETAIL_STRUCTURED_SDK_LOG(atk, Fatal, 3, ##__VA_ARGS__)

#define NN_DETAIL_ATK_VTRACE(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Trace, 0, format, args)
#define NN_DETAIL_ATK_VTRACE_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Trace, 1, format, args)
#define NN_DETAIL_ATK_VTRACE_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Trace, 2, format, args)
#define NN_DETAIL_ATK_VTRACE_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Trace, 3, format, args)

#define NN_DETAIL_ATK_VINFO(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Info, 0, format, args)
#define NN_DETAIL_ATK_VINFO_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Info, 1, format, args)
#define NN_DETAIL_ATK_VINFO_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Info, 2, format, args)
#define NN_DETAIL_ATK_VINFO_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Info, 3, format, args)

#define NN_DETAIL_ATK_VWARN(format, args)           NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Warn, 0, format, args)
#define NN_DETAIL_ATK_VWARN_V1(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Warn, 1, format, args)
#define NN_DETAIL_ATK_VWARN_V2(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Warn, 2, format, args)
#define NN_DETAIL_ATK_VWARN_V3(format, args)        NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Warn, 3, format, args)

#define NN_DETAIL_ATK_VERROR(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Error, 0, format, args)
#define NN_DETAIL_ATK_VERROR_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Error, 1, format, args)
#define NN_DETAIL_ATK_VERROR_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Error, 2, format, args)
#define NN_DETAIL_ATK_VERROR_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Error, 3, format, args)

#define NN_DETAIL_ATK_VFATAL(format, args)          NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Fatal, 0, format, args)
#define NN_DETAIL_ATK_VFATAL_V1(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Fatal, 1, format, args)
#define NN_DETAIL_ATK_VFATAL_V2(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Fatal, 2, format, args)
#define NN_DETAIL_ATK_VFATAL_V3(format, args)       NN_DETAIL_STRUCTURED_SDK_VLOG(atk, Fatal, 3, format, args)

#define NN_DETAIL_ATK_PUT_TRACE(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(atk, Trace, 0, text, length)
#define NN_DETAIL_ATK_PUT_TRACE_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Trace, 1, text, length)
#define NN_DETAIL_ATK_PUT_TRACE_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Trace, 2, text, length)
#define NN_DETAIL_ATK_PUT_TRACE_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Trace, 3, text, length)

#define NN_DETAIL_ATK_PUT_INFO(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(atk, Info, 0, text, length)
#define NN_DETAIL_ATK_PUT_INFO_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(atk, Info, 1, text, length)
#define NN_DETAIL_ATK_PUT_INFO_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(atk, Info, 2, text, length)
#define NN_DETAIL_ATK_PUT_INFO_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(atk, Info, 3, text, length)

#define NN_DETAIL_ATK_PUT_WARN(text, length)        NN_DETAIL_STRUCTURED_SDK_PUT(atk, Warn, 0, text, length)
#define NN_DETAIL_ATK_PUT_WARN_V1(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(atk, Warn, 1, text, length)
#define NN_DETAIL_ATK_PUT_WARN_V2(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(atk, Warn, 2, text, length)
#define NN_DETAIL_ATK_PUT_WARN_V3(text, length)     NN_DETAIL_STRUCTURED_SDK_PUT(atk, Warn, 3, text, length)

#define NN_DETAIL_ATK_PUT_ERROR(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(atk, Error, 0, text, length)
#define NN_DETAIL_ATK_PUT_ERROR_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Error, 1, text, length)
#define NN_DETAIL_ATK_PUT_ERROR_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Error, 2, text, length)
#define NN_DETAIL_ATK_PUT_ERROR_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Error, 3, text, length)

#define NN_DETAIL_ATK_PUT_FATAL(text, length)       NN_DETAIL_STRUCTURED_SDK_PUT(atk, Fatal, 0, text, length)
#define NN_DETAIL_ATK_PUT_FATAL_V1(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Fatal, 1, text, length)
#define NN_DETAIL_ATK_PUT_FATAL_V2(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Fatal, 2, text, length)
#define NN_DETAIL_ATK_PUT_FATAL_V3(text, length)    NN_DETAIL_STRUCTURED_SDK_PUT(atk, Fatal, 3, text, length)
