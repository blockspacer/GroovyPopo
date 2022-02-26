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
* @brief パイプラインのデータに関する API の宣言（NVN8 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Common-api.nvn.h>
#include <nn/gfx/detail/gfx_State-api.nvn.8.h>

namespace nn {
namespace gfx {

/**
* @brief パイプラインを表す構造体です（NVN8 特殊版）
*/
template<>
struct PipelineImplData< ApiVariationNvn8 >
{
    enum PipelineType
    {
        PipelineType_Graphics,
        PipelineType_Compute
    };

    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_HasTessellationState
    };

    Bit8 state; //!< オブジェクトの状態です
    Bit8 nnPipelineType; //!< nn::gfx::PipelineType によって表される、パイプラインの種類です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved; //!< 予約領域です。
    Bit32 memorySize;

    detail::VertexStateImpl< ApiVariationNvn8 >::DataType nnVertexState;
    detail::BlendStateImpl< ApiVariationNvn8 >::DataType nnBlendState;
    detail::RasterizerStateImpl< ApiVariationNvn8 >::DataType nnRasterizerState;
    detail::DepthStencilStateImpl< ApiVariationNvn8 >::DataType nnDepthStencilState;
    detail::TessellationStateImpl< ApiVariationNvn8 >::DataType nnTessellationState;

    detail::Ptr< const detail::ShaderImpl< ApiVariationNvn8 > > pShader; //!< シェーダーです
    detail::Ptr< void > pWorkMemory;

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
