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
* @brief パイプライン初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

struct RasterizerStateInfoData;
struct BlendStateInfoData;
struct DepthStencilStateInfoData;
struct RenderTargetStateInfoData;
struct VertexStateInfoData;
struct TessellationStateInfoData;

/**
* @brief グラフィックスパイプラインを初期化するための情報を表す構造体です。
*/
struct GraphicsPipelineInfoData
{
    detail::Ptr< const RasterizerStateInfoData > pRasterizerState; //!< ラスタライザーステートへのポインターです
    detail::Ptr< const BlendStateInfoData > pBlendState; //!< ブレンドステートへのポインターです
    detail::Ptr< const DepthStencilStateInfoData > pDepthStencilState; //!< 深度ステンシルステートへのポインターです
    detail::Ptr< const RenderTargetStateInfoData > pRenderTargetState; //!< レンダーターゲットステートへのポインターです
    detail::Ptr< const VertexStateInfoData > pVertexState; //!< 頂点ステートへのポインターです
    detail::Ptr< const void > pShader; //!< シェーダーへのポインターです
    detail::Ptr< const TessellationStateInfoData > pTessellationState; //!< テッセレーションステートへのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief 演算パイプラインを初期化するための情報を表す構造体です。
*/
struct ComputePipelineInfoData
{
    detail::Ptr< const void > pShader; //!< シェーダーへのポインターです

    char reserved[ 24 ]; //!< 予約領域です
};

}
}
