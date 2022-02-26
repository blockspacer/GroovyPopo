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

#include <nn/nn_Macro.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_Enum.h>

#include <nn/gfx/detail/gfx_Declare.h>

struct HGLRC__;

namespace nn {
namespace gfx {

/**
* @brief GL4 API との相互運用を行うためのクラスです。
*
* @details
* 元のオブジェクトの作成者がオブジェクトの所有権をもちます。@n
* Initialize が呼ばれていない gfx オブジェクトに対して Finalize を呼ぶことはできません。@n
*
* 元のオブジェクトを作成したコンテキストとそれを使用するコンテキストは同じまたは共有関係にある必要があります。
*/
template<>
class TInteroperation< ApiVariationGl4 >
{
    NN_DISALLOW_COPY( TInteroperation );
    TInteroperation() NN_NOEXCEPT;

public:
    /**
    * @brief 低レベルグラフィックス API のターゲットです。
    */
    typedef ApiVariationGl4 Target;

    /**
    * @brief GL のハンドルの型です。
    */
    typedef unsigned int GlHandleType;

    /**
    * @brief GL の列挙の型です。
    */
    typedef unsigned int GlEnumType;

    /**
    * @brief 指定された GL4 のバッファーを gfx のバッファーに変換します。
    *
    * @param[out] pOutGfxBuffer 変換された gfx のバッファーを受け取るためのポインター
    * @param[in] pDevice デバイスへのポインター
    * @param[in] hGl4Buffer 変換する GL4 のバッファー
    *
    * @pre
    * - pOutGfxBuffer != NULL
    * - pOutGfxBuffer が初期化されていない
    * - pDevice != NULL
    * - hGl4Buffer が有効な GL4 バッファーオブジェクトである
    */
    static void ConvertToGfxBuffer( TBuffer< Target >* pOutGfxBuffer,
        TDevice< Target >* pDevice, GlHandleType hGl4Buffer ) NN_NOEXCEPT;

    /**
    * @brief 指定された Windows の GL レンダリングコンテキストを gfx のデバイスに変換します。
    *
    * @param[out] pOutGfxDevice 変換された gfx のデバイスを受け取るためのポインター
    * @param[in] hGlRc 変換する GL レンダリングコンテキスト
    *
    * @pre
    * - pOutGfxDevice != NULL
    * - hGlRc != NULL
    */
    static void ConvertToGfxDevice( TDevice< Target >* pOutGfxDevice, HGLRC__* hGlRc ) NN_NOEXCEPT;

    /**
    * @brief 指定された GL4 のサンプラーを gfx のサンプラーに変換します。
    *
    * @param[out] pOutGfxSampler 変換された gfx のサンプラーを受け取るためのポインター
    * @param[in] hGl4Sampler 変換する GL4 のサンプラー
    *
    * @pre
    * - pOutGfxSampler != NULL
    * - pOutGfxSampler が初期化されていない
    * - hGl4Sampler が有効な GL4 サンプラーオブジェクトである
    */
    static void ConvertToGfxSampler( TSampler< Target >* pOutGfxSampler,
        GlHandleType hGl4Sampler ) NN_NOEXCEPT;

    /**
    * @brief 指定された GL4 のプログラムを gfx のシェーダーに変換します。
    *
    * @param[out] pOutGfxShader 変換された gfx のシェーダーを受け取るためのポインター
    * @param[in] pDevice デバイスへのポインター
    * @param[in] hGl4Program 変換する GL4 のプログラム
    *
    * @pre
    * - pOutGfxShader != NULL
    * - pOutGfxShader が初期化されていない
    * - pDevice != NULL
    * - hGl4Program が有効な GL4 のプログラムオブジェクトである
    *
    * @details
    * hGl4Program は GL_PROGRAM_SEPARABLE が GL_FALSE である必要があります。
    */
    static void ConvertToGfxShader( TShader< Target >* pOutGfxShader,
        TDevice< Target >* pDevice, GlHandleType hGl4Program ) NN_NOEXCEPT;

    /**
    * @brief 指定された GL4 のテクスチャーを gfx のテクスチャーに変換します。
    *
    * @param[out] pOutGfxTexture 変換された gfx のテクスチャーを受け取るためのポインター
    * @param[in] hGl4Texture 変換する GL4 のテクスチャー
    * @param[in] gl4BindTarget テクスチャーの GL_TEXTURE_TARGET
    * @param[in] imageFormat テクスチャーのイメージフォーマット
    *
    * @pre
    * - pOutGfxTexture != NULL
    * - pOutGfxTexture が初期化されていない
    * - hGl4Texture が有効な GL4 のテクスチャーオブジェクトである
    *
    * @details
    * hGl4Texture の GL_TEXTURE_TARGET が GL_NONE でない場合、gl4BindTarget は hGl4Texture の GL_TEXTURE_TARGET と等しい必要があります。
    */
    static void ConvertToGfxTexture( TTexture< Target >* pOutGfxTexture,
        GlHandleType hGl4Texture, GlEnumType gl4BindTarget, ImageFormat imageFormat ) NN_NOEXCEPT;

    /**
    *@brief 指定された GL4 のテクスチャーを gfx のテクスチャービューに変換します。
    *
    * @param[out] pOutGfxTextureView 変換された gfx のテクスチャービューを受け取るためのポインター
    * @param[in] hGl4Texture 変換する GL4 のテクスチャー
    * @param[in] gl4BindTarget テクスチャーの GL_TEXTURE_TARGET
    *
    * @pre
    * - pOutGfxTextureView != NULL
    * - pOutGfxTextureView が初期化されていない
    * - hGl4Texture が有効な GL のテクスチャーオブジェクトである
    *
    * @details
    * hGl4Texture の GL_TEXTURE_TARGET が GL_NONE でない場合、gl4BindTarget は hGl4Texture の GL_TEXTURE_TARGET と等しい必要があります。
    */
    static void ConvertToGfxTextureView( TTextureView< Target >* pOutGfxTextureView,
        GlHandleType hGl4Texture, GlEnumType gl4BindTarget ) NN_NOEXCEPT;

    /**
    * @brief 指定された GL4 のテクスチャーを gfx のカラーターゲットビューに変換します。
    *
    * @param[out] pOutGfxColorTargetView 変換された gfx のカラーターゲットビューを受け取るためのポインター
    * @param[in] hGl4Texture 変換する GL4 のテクスチャー
    * @param[in] gl4BindTarget テクスチャーの GL_TEXTURE_TARGET
    *
    * @pre
    * - pOutGfxColorTargetView != NULL
    * - pOutGfxColorTargetView が初期化されていない
    * - hGl4Texture が有効な GL のテクスチャーオブジェクトである
    *
    * @details
    * hGl4Texture の GL_TEXTURE_TARGET が GL_NONE でない場合、gl4BindTarget は hGl4Texture の GL_TEXTURE_TARGET と等しい必要があります。
    */
    static void ConvertToGfxColorTargetView( TColorTargetView< Target >* pOutGfxColorTargetView,
        GlHandleType hGl4Texture, GlEnumType gl4BindTarget ) NN_NOEXCEPT;

    /**
    * @brief 指定された GL4 のテクスチャーを gfx の深度ステンシルビューに変換します。
    *
    * @param[out] pOutGfxDepthStencilView 変換された gfx の深度ステンシルビューを受け取るためのポインター
    * @param[in] hGl4Texture 変換する GL4 のテクスチャー
    * @param[in] gl4BindTarget テクスチャーの GL_TEXTURE_TARGET
    * @param[in] gl4InternalFormat テクスチャーの GL_TEXTURE_INTERNAL_FORMAT
    *
    * @pre
    * - pOutGfxDepthStencilView != NULL
    * - pOutGfxDepthStencilView が初期化されていない
    * - hGl4Texture が有効な GL のテクスチャーオブジェクトである
    *
    * @details
    * hGl4Texture の GL_TEXTURE_TARGET が GL_NONE でない場合、gl4BindTarget は hGl4Texture の GL_TEXTURE_TARGET と等しい必要があります。
    */
    static void ConvertToGfxDepthStencilView( TDepthStencilView< Target >* pOutGfxDepthStencilView,
        GlHandleType hGl4Texture, GlEnumType gl4BindTarget, GlEnumType gl4InternalFormat ) NN_NOEXCEPT;
};

}
}
