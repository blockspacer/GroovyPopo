/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief 初期化・終了に関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_TargetConfig.h>

#include <nn/gfx/detail/gfx_Declare.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Core-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Core-api.nvn.8.h>
#endif
#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Core-api.gx.2.h>
#endif
#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Core-api.vk.1.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Core-api.d3d.11.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief API のメジャーバージョンです。
*/
static const int ApiMajorVersion = 0;

/**
* @brief API のマイナーバージョンです。
*/
static const int ApiMinorVersion = 0;

/**
* @brief gfx ライブラリーを初期化します。
*
* @pre
* - gfx ライブラリーが初期化されていない
*
* @post
* - gfx ライブラリーが初期化されている
*/
inline void Initialize() NN_NOEXCEPT
{
    return detail::Initialize< TargetConfig::Variation >();
}

/**
* @brief gfx ライブラリーを終了します。
*
* @pre
* - gfx ライブラリーが初期化されている
*
* @post
* - gfx ライブラリーが初期化されていない
*/
inline void Finalize() NN_NOEXCEPT
{
    return detail::Finalize< TargetConfig::Variation >();
}

}
}
