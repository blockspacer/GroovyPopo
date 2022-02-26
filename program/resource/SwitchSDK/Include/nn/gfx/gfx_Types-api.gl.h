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
* @brief 型に関する定義（GL 特殊版）
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_Device.h>

#include <nn/gfx/detail/gfx_Declare.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/gfx_Types-api.gl.4.h>
#endif

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief バッファーを表す型定義です（GL 特殊版）
*/
typedef TBuffer< ApiTypeGl >
    NN_MACRO_CONCATENATE( Buffer, NN_GFX_GL_SUFFIX );

/**
* @brief バッファーテクスチャービューを表す型定義です（GL 特殊版）
*/
typedef TBufferTextureView< ApiTypeGl >
    NN_MACRO_CONCATENATE( BufferTextureView, NN_GFX_GL_SUFFIX );

/**
* @brief コマンドバッファーを表す型定義です（GL 特殊版）
*/
typedef TCommandBuffer< ApiTypeGl >
    NN_MACRO_CONCATENATE( CommandBuffer, NN_GFX_GL_SUFFIX );

/**
* @brief デスクリプタープールを表す型定義です（GL 特殊版）
*/
typedef TDescriptorPool< ApiTypeGl >
    NN_MACRO_CONCATENATE( DescriptorPool, NN_GFX_GL_SUFFIX );

/**
* @brief デバイスを表す型定義です（GL 特殊版）
*/
typedef TDevice< ApiTypeGl >
    NN_MACRO_CONCATENATE( Device, NN_GFX_GL_SUFFIX );

/**
* @brief メモリープールを表す型定義です（GL 特殊版）
*/
typedef TMemoryPool< ApiTypeGl >
    NN_MACRO_CONCATENATE( MemoryPool, NN_GFX_GL_SUFFIX );

/**
* @brief パイプラインを表す型定義です（GL 特殊版）
*/
typedef TPipeline< ApiTypeGl >
    NN_MACRO_CONCATENATE( Pipeline, NN_GFX_GL_SUFFIX );

/**
* @brief キューを表す型定義です（GL 特殊版）
*/
typedef TQueue< ApiTypeGl >
    NN_MACRO_CONCATENATE( Queue, NN_GFX_GL_SUFFIX );

/**
* @brief ルートシグネチャーを表す型定義です（GL 特殊版）
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
NN_DEPRECATED typedef TRootSignature< ApiTypeGl >
    NN_MACRO_CONCATENATE( RootSignature, NN_GFX_GL_SUFFIX );

/**
* @brief サンプラーを表す型定義です（GL 特殊版）。
*/
typedef TSampler< ApiTypeGl >
    NN_MACRO_CONCATENATE( Sampler, NN_GFX_GL_SUFFIX );

/**
* @brief シェーダーを表す型定義です（GL 特殊版）
*/
typedef TShader< ApiTypeGl >
    NN_MACRO_CONCATENATE( Shader, NN_GFX_GL_SUFFIX );

/**
* @brief ラスタライザーステートを表す型定義です（GL 特殊版）
*/
typedef TRasterizerState< ApiTypeGl >
    NN_MACRO_CONCATENATE( RasterizerState, NN_GFX_GL_SUFFIX );

/**
* @brief ブレンドステートを表す型定義です（GL 特殊版）
*/
typedef TBlendState< ApiTypeGl >
    NN_MACRO_CONCATENATE( BlendState, NN_GFX_GL_SUFFIX );

/**
* @brief 深度ステンシルステートを表す型定義です（GL 特殊版）
*/
typedef TDepthStencilState< ApiTypeGl >
    NN_MACRO_CONCATENATE( DepthStencilState, NN_GFX_GL_SUFFIX );

/**
* @brief 頂点ステートを表す型定義です（GL 特殊版）
*/
typedef TVertexState< ApiTypeGl >
    NN_MACRO_CONCATENATE( VertexState, NN_GFX_GL_SUFFIX );

/**
* @brief テッセレーションステートを表す型定義です（GL 特殊版）
*/
typedef TTessellationState< ApiTypeGl >
    NN_MACRO_CONCATENATE( TessellationState, NN_GFX_GL_SUFFIX );

/**
* @brief ビューポートシザーステートを表す型定義です（GL 特殊版）
*/
typedef TViewportScissorState< ApiTypeGl >
    NN_MACRO_CONCATENATE( ViewportScissorState, NN_GFX_GL_SUFFIX );

/**
* @brief スワップチェーンを表す型定義です（GL 特殊版）
*/
typedef TSwapChain< ApiTypeGl >
    NN_MACRO_CONCATENATE( SwapChain, NN_GFX_GL_SUFFIX );

/**
* @brief フェンスを表す型定義です（GL 特殊版）
*/
typedef TFence< ApiTypeGl >
    NN_MACRO_CONCATENATE( Fence, NN_GFX_GL_SUFFIX );

/**
* @brief セマフォを表す型定義です（GL 特殊版）
*/
typedef TSemaphore< ApiTypeGl >
    NN_MACRO_CONCATENATE( Semaphore, NN_GFX_GL_SUFFIX );

/**
* @brief テクスチャーを表す型定義です（GL 特殊版）
*/
typedef TTexture< ApiTypeGl >
    NN_MACRO_CONCATENATE( Texture, NN_GFX_GL_SUFFIX );

/**
* @brief テクスチャービューを表す型定義です（GL 特殊版）
*/
typedef TTextureView< ApiTypeGl >
    NN_MACRO_CONCATENATE( TextureView, NN_GFX_GL_SUFFIX );

/**
* @brief カラーターゲットビューを表す型定義です（GL 特殊版）
*/
typedef TColorTargetView< ApiTypeGl >
    NN_MACRO_CONCATENATE( ColorTargetView, NN_GFX_GL_SUFFIX );

/**
* @brief 深度ステンシルビューを表す型定義です（GL 特殊版）
*/
typedef TDepthStencilView< ApiTypeGl >
    NN_MACRO_CONCATENATE( DepthStencilView, NN_GFX_GL_SUFFIX );

/**
* @brief タイムスタンプに使われるバッファーを表す型定義です（GL 特殊版）
*/
typedef TTimestampBuffer< ApiTypeGl >
    NN_MACRO_CONCATENATE( TimestampBuffer, NN_GFX_GL_SUFFIX );

/**
* @brief クエリーに使われるバッファーを表す型定義です（GL 特殊版）
*/
typedef TQueryBuffer< ApiTypeGl >
    NN_MACRO_CONCATENATE( QueryBuffer, NN_GFX_GL_SUFFIX );

/**
* @brief gfx ライブラリーを初期化します（GL 特殊版）
*
* @post
* gfx ライブラリーが初期化されている。
*/
inline void NN_MACRO_CONCATENATE( Initialize, NN_GFX_GL_SUFFIX )() NN_NOEXCEPT
{
    return detail::Initialize< ApiTypeGl >();
}

/**
* @brief gfx ライブラリーを終了します（GL 特殊版）
*
* @post
* gfx ライブラリーが終了されている。
*/
inline void NN_MACRO_CONCATENATE( Finalize, NN_GFX_GL_SUFFIX )() NN_NOEXCEPT
{
    return detail::Finalize< ApiTypeGl >();
}

/**
* @brief イメージフォーマットのプロパティーを取得します（GL 特殊版）
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
inline void GetImageFormatPropertyGl(
    ImageFormatProperty* pOutImageFormatProperty, TDevice< typename
    detail::TargetVariation< ApiTypeGl >::Type >* pDevice, ImageFormat imageFormat ) NN_NOEXCEPT
{
    return detail::GetImageFormatProperty< typename detail::TargetVariation<
        ApiTypeGl >::Type >( pOutImageFormatProperty, pDevice, imageFormat );
}

/**
* @brief 2 つのタイムスタンプ値の間の時間を取得します（GL 特殊版）
*
* @param[in] startTimestampValue 始点のタイムスタンプの値
* @param[in] endTimestampValue 終点のタイムスタンプの値
*
* @return 指定された始点と終点の間の時間を返します。
*
* @details
* タイムスタンプの値には nn::gfx::TimestampBuffer::GetValue で得られる値を使うことができます。
*/
inline nn::TimeSpan GetDurationGl(
    int64_t startTimestampValue, int64_t endTimestampValue ) NN_NOEXCEPT
{
    return detail::GetDuration< typename detail::TargetVariation<
        ApiTypeGl >::Type >( startTimestampValue, endTimestampValue );
}

}
}

NN_PRAGMA_POP_WARNINGS
