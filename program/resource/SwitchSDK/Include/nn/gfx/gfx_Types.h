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
* @brief 型に関する定義
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_TargetConfig.h>

#include <nn/gfx/detail/gfx_Declare.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL )
    #include <nn/gfx/gfx_Types-api.gl.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX )
    #include <nn/gfx/gfx_Types-api.gx.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN )
    #include <nn/gfx/gfx_Types-api.nvn.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D )
    #include <nn/gfx/gfx_Types-api.d3d.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK )
    #include <nn/gfx/gfx_Types-api.vk.h>
#endif

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief バッファーを表す型定義です。
*
* @see
* nn::gfx::TBuffer
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TBuffer< TargetConfig::Variation > Buffer;

/**
* @brief バッファーテクスチャービューを表す型定義です。
*
* @see
* nn::gfx::TBufferTextureView
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TBufferTextureView< TargetConfig::Variation > BufferTextureView;

/**
* @brief コマンドバッファーを表す型定義です。
*
* @see
* nn::gfx::TCommandBuffer
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TCommandBuffer< TargetConfig::Variation > CommandBuffer;

/**
* @brief デスクリプタープールを表す型定義です。
*
* @see
* nn::gfx::TDescriptorPool
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TDescriptorPool< TargetConfig::Variation > DescriptorPool;

/**
* @brief デバイスを表す型定義です。
*
* @see
* nn::gfx::TDevice
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TDevice< TargetConfig::Variation > Device;

/**
* @brief メモリープールを表す型定義です。
*
* @see
* nn::gfx::TMemoryPool
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TMemoryPool< TargetConfig::Variation > MemoryPool;

/**
* @brief パイプラインを表す型定義です。
*
* @see
* nn::gfx::TPipeline
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TPipeline< TargetConfig::Variation > Pipeline;

/**
* @brief キューを表す型定義です。
*
* @see
* nn::gfx::TQueue
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TQueue< TargetConfig::Variation > Queue;

/**
* @brief ルートシグネチャーを表す型定義です。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*
* @see
* nn::gfx::TRootSignature
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
NN_DEPRECATED typedef TRootSignature< TargetConfig::Variation > RootSignature;

/**
* @brief サンプラーを表す型定義です。
*
* @see
* nn::gfx::TSampler
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TSampler< TargetConfig::Variation > Sampler;

/**
* @brief シェーダーを表す型定義です。
*
* @see
* nn::gfx::TShader
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TShader< TargetConfig::Variation > Shader;

/**
* @brief ラスタライザーステートを表す型定義です。
*
* @see
* nn::gfx::TRasterizerState
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TRasterizerState< TargetConfig::Variation > RasterizerState;

/**
* @brief ブレンドステートを表す型定義です。
*
* @see
* nn::gfx::TBlendState
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TBlendState< TargetConfig::Variation > BlendState;

/**
* @brief 深度ステンシルステートを表す型定義です。
*
* @see
* nn::gfx::TDepthStencilState
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TDepthStencilState< TargetConfig::Variation > DepthStencilState;

/**
* @brief 頂点ステートを表す型定義です。
*
* @see
* nn::gfx::TVertexState
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TVertexState< TargetConfig::Variation > VertexState;

/**
* @brief テッセレーションステートを表す型定義です。
*
* @see
* nn::gfx::TTessellationState
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TTessellationState< TargetConfig::Variation > TessellationState;

/**
* @brief ビューポートシザーステートを表す型定義です。
*
* @see
* nn::gfx::TViewportScissorState
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TViewportScissorState< TargetConfig::Variation > ViewportScissorState;

/**
* @brief スワップチェーンを表す型定義です。
*
* @see
* nn::gfx::TSwapChain
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TSwapChain< TargetConfig::Variation > SwapChain;

/**
* @brief フェンスを表す型定義です。
*
* @see
* nn::gfx::TFence
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TFence< TargetConfig::Variation > Fence;

/**
* @brief セマフォを表す型定義です。
*
* @see
* nn::gfx::TSemaphore
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TSemaphore< TargetConfig::Variation > Semaphore;

/**
* @brief テクスチャーを表す型定義です。
*
* @see
* nn::gfx::TTexture
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TTexture< TargetConfig::Variation > Texture;

/**
* @brief テクスチャービューを表す型定義です。
*
* @see
* nn::gfx::TTextureView
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TTextureView< TargetConfig::Variation > TextureView;

/**
* @brief カラーターゲットビューを表す型定義です。
*
* @see
* nn::gfx::TColorTargetView
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TColorTargetView< TargetConfig::Variation > ColorTargetView;

/**
* @brief 深度ステンシルビューを表す型定義です。
*
* @see
* nn::gfx::TDepthStencilView
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TDepthStencilView< TargetConfig::Variation > DepthStencilView;

/**
* @brief タイムスタンプに使われるバッファーを表す型定義です。
*
* @see
* nn::gfx::TTimestampBuffer
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TTimestampBuffer< TargetConfig::Variation > TimestampBuffer;

/**
* @brief クエリーに使われるバッファーを表す型定義です。
*
* @see
* nn::gfx::TQueryBuffer
*
* @details
* 対象の低レベルグラフィックス API を明示的に指定したい場合はサフィックス付きの特殊版を利用してください。
*/
typedef TQueryBuffer< TargetConfig::Variation > QueryBuffer;

}
}

NN_PRAGMA_POP_WARNINGS
