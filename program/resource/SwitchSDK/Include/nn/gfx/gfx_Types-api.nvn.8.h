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
* @brief 型に関する定義（NVN8 特殊版）
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>
#include <nn/gfx/gfx_Device.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Core-api.nvn.8.h>
#include <nn/gfx/detail/gfx_Texture-api.nvn.8.h>
#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#include <nn/gfx/detail/gfx_BufferLayout-api.nvn.8.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief バッファーを表す型定義です（NVN8 特殊版）
*/
typedef TBuffer< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Buffer, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief バッファーテクスチャービューを表す型定義です（NVN8 特殊版）
*/
typedef TBufferTextureView< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( BufferTextureView, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief コマンドバッファーを表す型定義です（NVN8 特殊版）
*/
typedef TCommandBuffer< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( CommandBuffer, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief デバイスを表す型定義です（NVN8 特殊版）
*/
typedef TDevice< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Device, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief パイプラインを表す型定義です（NVN8 特殊版）
*/
typedef TPipeline< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Pipeline, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief キューを表す型定義です（NVN8 特殊版）
*/
typedef TQueue< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Queue, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief サンプラーを表す型定義です（NVN8 特殊版）。
*/
typedef TSampler< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Sampler, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief シェーダーを表す型定義です（NVN8 特殊版）
*/
typedef TShader< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Shader, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief ラスタライザーステートを表す型定義です（NVN8 特殊版）
*/
typedef TRasterizerState< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( RasterizerState, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief ブレンドステートを表す型定義です（NVN8 特殊版）
*/
typedef TBlendState< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( BlendState, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief 深度ステンシルステートを表す型定義です（NVN8 特殊版）
*/
typedef TDepthStencilState< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( DepthStencilState, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief 頂点ステートを表す型定義です（NVN8 特殊版）
*/
typedef TVertexState< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( VertexState, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief テッセレーションステートを表す型定義です（NVN8 特殊版）
*/
typedef TTessellationState< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( TessellationState, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief ビューポートシザーステートを表す型定義です（NVN8 特殊版）
*/
typedef TViewportScissorState< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( ViewportScissorState, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief スワップチェーンを表す型定義です（NVN8 特殊版）
*/
typedef TSwapChain< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( SwapChain, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief フェンスを表す型定義です（NVN8 特殊版）
*/
typedef TFence< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Fence, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief セマフォを表す型定義です（NVN8 特殊版）
*/
typedef TSemaphore< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Semaphore, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief テクスチャーを表す型定義です（NVN8 特殊版）
*/
typedef TTexture< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( Texture, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief テクスチャービューを表す型定義です（NVN8 特殊版）
*/
typedef TTextureView< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( TextureView, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief カラーターゲットビューを表す型定義です（NVN8 特殊版）
*/
typedef TColorTargetView< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( ColorTargetView, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief 深度ステンシルビューを表す型定義です（NVN8 特殊版）
*/
typedef TDepthStencilView< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( DepthStencilView, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief 相互運用を行うためのクラスの型定義です（NVN 特殊版）
*/
typedef TInteroperation< ApiVariation< ApiTypeNvn > >
    NN_MACRO_CONCATENATE( Interoperation, NN_GFX_NVN_SUFFIX );

/**
* @brief タイムスタンプに使われるバッファーを表す型定義です（NVN8 特殊版）
*/
typedef TTimestampBuffer< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( TimestampBuffer, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief クエリーに使われるバッファーを表す型定義です（NVN8 特殊版）
*/
typedef TQueryBuffer< ApiVariationNvn8 >
    NN_MACRO_CONCATENATE( QueryBuffer, NN_MACRO_CONCATENATE( NN_GFX_NVN_SUFFIX, 8 ) );

/**
* @brief gfx ライブラリーを初期化します（NVN8 特殊版）
*
* @post
* gfx ライブラリーが初期化されている。
*/
inline void NN_MACRO_CONCATENATE( Initialize, NN_MACRO_CONCATENATE(
    NN_GFX_NVN_SUFFIX, 8 ) )() NN_NOEXCEPT
{
    return detail::Initialize< ApiVariationNvn8 >();
}

/**
* @brief gfx ライブラリーを終了します（NVN8 特殊版）
*
* @post
* gfx ライブラリーが終了されている。
*/
inline void NN_MACRO_CONCATENATE( Finalize, NN_MACRO_CONCATENATE(
    NN_GFX_NVN_SUFFIX, 8 ) )() NN_NOEXCEPT
{
    return detail::Finalize< ApiVariationNvn8 >();
}

/**
* @brief イメージフォーマットのプロパティーを取得します（NVN8 特殊版）
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
inline void GetImageFormatPropertyNvn8( ImageFormatProperty* pOutImageFormatProperty,
    TDevice< ApiVariationNvn8 >* pDevice, ImageFormat imageFormat ) NN_NOEXCEPT
{
    return detail::GetImageFormatProperty< ApiVariationNvn8 >(
        pOutImageFormatProperty, pDevice, imageFormat );
}

/**
* @brief 2 つのタイムスタンプ値の間の時間を取得します（NVN8 特殊版）
*
* @param[in] startTimestampValue 始点のタイムスタンプの値
* @param[in] endTimestampValue 終点のタイムスタンプの値
*
* @return 指定された始点と終点の間の時間を返します。
*
* @details
* タイムスタンプの値には nn::gfx::TimestampBuffer::GetValue で得られる値を使うことができます。
*/
inline nn::TimeSpan GetDurationNvn8( int64_t startTimestampValue, int64_t endTimestampValue ) NN_NOEXCEPT
{
    return detail::GetDuration< ApiVariationNvn8 >( startTimestampValue, endTimestampValue );
}

}
}

NN_PRAGMA_POP_WARNINGS
