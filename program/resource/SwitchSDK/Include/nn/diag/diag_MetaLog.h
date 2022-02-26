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

#include <nn/nn_Macro.h>

#if defined(NN_ENABLE_SDK_META_LOG)
#include <nn/diag/detail/diag_DetailMetaLog.h>

#define NN_DIAG_META_MARK()                     NN_DETAIL_DIAG_META_MARK()
#define NN_DIAG_META_LOG(...)                   NN_DETAIL_DIAG_META_LOG(__VA_ARGS__)
#define NN_DIAG_META_LOG_CONTEXT(category, ...) NN_DETAIL_DIAG_META_LOG_CONTEXT(category, __VA_ARGS__)

#else

namespace nn { namespace diag {
struct MetaLogContext
{
    uint64_t AsUint64() const NN_NOEXCEPT
    {
        return 0x1234567890ABCDEFULL;
    }
};
}}

#define NN_DIAG_META_MARK()                     static_cast<void>(0)
#define NN_DIAG_META_LOG(...)                   static_cast<void>(0)
#define NN_DIAG_META_LOG_CONTEXT(category, ...) (::nn::diag::MetaLogContext{})

#endif
