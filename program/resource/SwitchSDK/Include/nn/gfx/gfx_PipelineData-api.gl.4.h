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
* @brief パイプラインのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>

#include <nn/gfx/detail/gfx_Common-api.gl.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Shader-api.gl.4.h>
#include <nn/gfx/detail/gfx_State-api.gl.4.h>

namespace nn {
namespace gfx {

/**
* @brief パイプラインを表す構造体です（GL4 特殊版）
*/
template<>
struct PipelineImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_HasTessellationState
    };

    Bit8 state; //!< オブジェクトの状態です。
    Bit8 pipelineType; //!< パイプラインの種類です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved; //!< 予約領域です。
    uint32_t memorySize; //!< 作業メモリーのサイズです
    detail::RasterizerStateImpl< ApiVariationGl4 >::DataType rasterizerState; //!< ラスタライザーステートです
    detail::BlendStateImpl< ApiVariationGl4 >::DataType blendState; //!< ブレンドステートです
    detail::DepthStencilStateImpl< ApiVariationGl4 >::DataType depthStencilState; //!< 深度ステンシルステートです
    detail::VertexStateImpl< ApiVariationGl4 >::DataType vertexState; //!< 頂点ステートです
    detail::TessellationStateImpl< ApiVariationGl4 >::DataType tessellationState; //!< テッセレーションステートです
    detail::Ptr< const detail::ShaderImpl< ApiVariationGl4 > > pShader; //!< シェーダーです
    detail::Ptr< void > pWorkMemory; //!< 作業メモリーへのポインターです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
