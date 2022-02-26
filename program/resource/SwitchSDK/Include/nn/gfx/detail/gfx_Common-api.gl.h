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

#include <nn/nn_Common.h>

#include <nn/os/os_MutexTypes.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {
namespace detail {

typedef uint32_t GlHandle;
typedef uint32_t GlEnum;
typedef uint8_t GlBool;
typedef void* GlRcHandle;

enum
{
    GlInvalidHandle = 0,
    GlInvalidLocation = -1
};

struct GlRenderingContext
{
    Ptr< void > hGlRc;
    nn::os::MutexType mutex;
};

struct GlCommandContext
{
    GlHandle hFbo;
    GlHandle hTmpFbo[ 2 ];
    GlHandle hVao;
    GlHandle hPipeline;
    GlHandle hQuery[ QueryTarget_End ];
};

inline bool IsValid( GlHandle handle )
{
    return handle != GlInvalidHandle;
}

}
}
}
