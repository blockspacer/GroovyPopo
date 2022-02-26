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
* @brief シェーダーのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_ShaderInfoData.h>

#include <nn/gfx/detail/gfx_Common-api.gl.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief シェーダーを表す構造体です（GL4 特殊版）
*/
template<>
struct ShaderImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_SeparationEnable = ShaderInfoData::Flag_SeparationEnable,
        Flag_Shared
    };

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    union
    {
        detail::GlHandle hCombinedProgram; //!< 組み合わされたプログラムハンドルです
        detail::GlHandle hVertexProgram; //!< 頂点プログラムハンドルです
    };
    detail::GlHandle hHullProgram; //!< ハルプログラムハンドルです
    detail::GlHandle hDomainProgram; //!< ドメインプログラムハンドルです
    detail::GlHandle hGeometryProgram; //!< ジオメトリプログラムハンドルです
    detail::GlHandle hPixelProgram; //!< ピクセルプログラムハンドルです
    detail::GlHandle hComputeProgram; //!< 演算プログラムハンドルです
    char reserved2[ 4 ]; //!< 予約領域です。
    detail::Ptr< detail::DeviceImpl< ApiVariationGl4 > > pGfxDevice; //!< デバイスへのポインターです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
