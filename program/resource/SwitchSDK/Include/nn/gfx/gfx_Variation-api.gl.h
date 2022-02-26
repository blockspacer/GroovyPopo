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

#include <nn/gfx/gfx_VariationBase.h>

namespace nn {
namespace gfx {

enum /* LowLevelApi */
{
    LowLevelApi_Gl = NN_GFX_CONFIG_API_TYPE_GL
};

/**
* @brief GL を表す型定義です。
*/
typedef ApiType< LowLevelApi_Gl > ApiTypeGl;

/**
* @brief GL4 API を表す型定義です。
*/
typedef ApiVariation< ApiTypeGl, ApiVersion< 4 > > ApiVariationGl4;

template<>
struct DefaultVersion< ApiTypeGl >
{
    typedef ApiVersion< NN_GFX_CONFIG_GL_DEFAULT_VERSION > Version;
};

template<>
struct TargetVersion< ApiTypeGl >
{
    typedef ApiVersion< NN_GFX_CONFIG_GL_TARGET_VERSION > Version;
};

}
}
