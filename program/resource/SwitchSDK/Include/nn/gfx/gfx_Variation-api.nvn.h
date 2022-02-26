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
    LowLevelApi_Nvn = NN_GFX_CONFIG_API_TYPE_NVN
};

/**
* @brief NVN を表す型定義です。
*/
typedef ApiType< LowLevelApi_Nvn > ApiTypeNvn;

/**
* @brief NVN API を表す型定義です。
*/
typedef ApiVariation< ApiTypeNvn, ApiVersion< 8 > > ApiVariationNvn8;

template<>
struct DefaultVersion< ApiTypeNvn >
{
    typedef ApiVersion< NN_GFX_CONFIG_NVN_DEFAULT_VERSION > Version;
};

template<>
struct TargetVersion< ApiTypeNvn >
{
    typedef ApiVersion< NN_GFX_CONFIG_NVN_TARGET_VERSION > Version;
};

}
}
