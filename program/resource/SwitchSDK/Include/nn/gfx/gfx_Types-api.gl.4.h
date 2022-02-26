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
* @brief 型に関する定義（GL4 特殊版）
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_Device.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Core-api.gl.4.h>
#include <nn/gfx/detail/gfx_Texture-api.gl.4.h>
#include <nn/gfx/detail/gfx_Device-api.gl.4.h>
#include <nn/gfx/detail/gfx_BufferLayout-api.gl.4.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief バッファーを表す型定義です（GL4 特殊版）
*/
typedef TBuffer< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Buffer, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief バッファーテクスチャービューを表す型定義です（GL4 特殊版）
*/
typedef TBufferTextureView< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( BufferTextureView, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief コマンドバッファーを表す型定義です（GL4 特殊版）
*/
typedef TCommandBuffer< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( CommandBuffer, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief デスクリプタープールを表す型定義です（GL4 特殊版）
*/
typedef TDescriptorPool< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( DescriptorPool, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief デバイスを表す型定義です（GL4 特殊版）
*/
typedef TDevice< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Device, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief メモリープールを表す型定義です（GL4 特殊版）
*/
typedef TMemoryPool< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( MemoryPool, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief パイプラインを表す型定義です（GL4 特殊版）
*/
typedef TPipeline< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Pipeline, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief キューを表す型定義です（GL4 特殊版）
*/
typedef TQueue< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Queue, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief ルートシグネチャーを表す型定義です（GL4 特殊版）
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
NN_DEPRECATED typedef TRootSignature< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( RootSignature, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief サンプラーを表す型定義です（GL4 特殊版）。
*/
typedef TSampler< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Sampler, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief シェーダーを表す型定義です（GL4 特殊版）
*/
typedef TShader< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Shader, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief ラスタライザーステートを表す型定義です（GL4 特殊版）
*/
typedef TRasterizerState< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( RasterizerState, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief ブレンドステートを表す型定義です（GL4 特殊版）
*/
typedef TBlendState< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( BlendState, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief 深度ステンシルステートを表す型定義です（GL4 特殊版）
*/
typedef TDepthStencilState< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( DepthStencilState, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief 頂点ステートを表す型定義です（GL4 特殊版）
*/
typedef TVertexState< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( VertexState, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief テッセレーションステートを表す型定義です（GL4 特殊版）
*/
typedef TTessellationState< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( TessellationState, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief ビューポートシザーステートを表す型定義です（GL4 特殊版）
*/
typedef TViewportScissorState< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( ViewportScissorState, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief スワップチェーンを表す型定義です（GL4 特殊版）
*/
typedef TSwapChain< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( SwapChain, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief フェンスを表す型定義です（GL4 特殊版）
*/
typedef TFence< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Fence, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief セマフォを表す型定義です（GL4 特殊版）
*/
typedef TSemaphore< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Semaphore, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief テクスチャーを表す型定義です（GL4 特殊版）
*/
typedef TTexture< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Texture, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief テクスチャービューを表す型定義です（GL4 特殊版）
*/
typedef TTextureView< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( TextureView, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief カラーターゲットビューを表す型定義です（GL4 特殊版）
*/
typedef TColorTargetView< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( ColorTargetView, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief 深度ステンシルビューを表す型定義です（GL4 特殊版）
*/
typedef TDepthStencilView< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( DepthStencilView, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief 相互運用を行うためのクラスの型定義です（GL4 特殊版）
*/
typedef TInteroperation< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( Interoperation, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief タイムスタンプに使われるバッファーを表す型定義です（GL4 特殊版）
*/
typedef TTimestampBuffer< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( TimestampBuffer, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief クエリーに使われるバッファーを表す型定義です（GL4 特殊版）
*/
typedef TQueryBuffer< ApiVariationGl4 >
    NN_MACRO_CONCATENATE( QueryBuffer, NN_MACRO_CONCATENATE( NN_GFX_GL_SUFFIX, 4 ) );

/**
* @brief gfx ライブラリーを初期化します（GL4 特殊版）
*
* @post
* gfx ライブラリーが初期化されている。
*/
inline void NN_MACRO_CONCATENATE( Initialize, NN_MACRO_CONCATENATE(
    NN_GFX_GL_SUFFIX, 4 ) )() NN_NOEXCEPT
{
    return detail::Initialize< ApiVariationGl4 >();
}

/**
* @brief gfx ライブラリーを終了します（GL4 特殊版）
*
* @post
* gfx ライブラリーが終了されている。
*/
inline void NN_MACRO_CONCATENATE( Finalize, NN_MACRO_CONCATENATE(
    NN_GFX_GL_SUFFIX, 4 ) )() NN_NOEXCEPT
{
    return detail::Finalize< ApiVariationGl4 >();
}

/**
* @brief イメージフォーマットのプロパティーを取得します（GL4 特殊版）
*
* @param[out] pOutImageFormatProperty 取得したイメージフォーマットのプロパティーを格納する領域へのポインター
* @param[in] pDevice デバイスへのポインター
* @param[in] imageFormat 取得する対象のイメージフォーマット
*
* @pre
* - pOutImageFormatProperty != NULL
* - pDevice != NULL
* - pDevice が初期化されている
*
* @post
* - pOutImageFormatProperty の指す領域に取得されたイメージフォーマットのプロパティーが格納されている
*/
inline void GetImageFormatPropertyGl4( ImageFormatProperty* pOutImageFormatProperty,
    TDevice< ApiVariationGl4 >* pDevice, ImageFormat imageFormat ) NN_NOEXCEPT
{
    return detail::GetImageFormatProperty< ApiVariationGl4 >(
        pOutImageFormatProperty, pDevice, imageFormat );
}

/**
* @brief 2 つのタイムスタンプ値の間の時間を取得します（GL4 特殊版）
*
* @param[in] startTimestampValue 始点のタイムスタンプの値
* @param[in] endTimestampValue 終点のタイムスタンプの値
*
* @return 指定された始点と終点の間の時間を返します。
*
* @details
* タイムスタンプの値には nn::gfx::TimestampBuffer::GetValue で得られる値を使うことができます。
*/
inline nn::TimeSpan GetDurationGl4(
    int64_t startTimestampValue, int64_t endTimestampValue ) NN_NOEXCEPT
{
    return detail::GetDuration< ApiVariationGl4 >( startTimestampValue, endTimestampValue );
}

}
}

NN_PRAGMA_POP_WARNINGS
