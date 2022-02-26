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

/**
* @brief 対象の低レベルグラフィックス API の設定を表す構造体です。
*/
struct TargetConfig
{
    typedef ApiType< NN_GFX_TARGET_API_TYPE > Type;
    typedef ApiVersion< NN_GFX_TARGET_VERSION > Version;
    typedef ApiVariation< Type, Version > Variation;
};

}
}
