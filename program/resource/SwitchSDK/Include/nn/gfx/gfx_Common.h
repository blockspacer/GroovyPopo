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
*   @file
*   @brief  gfx API の各機能で共通に使われる定義
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Config.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL )
    #include <nn/gfx/gfx_Variation-api.gl.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN )
    #include <nn/gfx/gfx_Variation-api.nvn.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX )
    #include <nn/gfx/gfx_Variation-api.gx.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D )
    #include <nn/gfx/gfx_Variation-api.d3d.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK )
    #include <nn/gfx/gfx_Variation-api.vk.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief 範囲を表す構造体です。
*/
struct Range
{
    ptrdiff_t offset; //!< バイトでの先頭からのオフセットです
    size_t size; //!< バイトでのサイズです
};

/**
* @brief メモリー不足のイベントに渡される引数です。
*/
struct OutOfMemoryEventArg
{
    size_t minRequiredSize; //!< バイトでの最小の要求メモリーサイズです
};

/**
* @brief クリアカラーの値を表す共用体です。
*/
union ClearColorValue
{
    float valueFloat[ 4 ]; //!< 赤成分、緑成分、青成分、アルファ成分の順での浮動小数点数でのクリアカラーです
    int32_t valueInt[ 4 ]; //!< 赤成分、緑成分、青成分、アルファ成分の順での符号付き整数でのクリアカラーです
    uint32_t valueUint[ 4 ]; //!< 赤成分、緑成分、青成分、アルファ成分の順での符号なし整数でのクリアカラーです
};

/**
* @brief オブジェクトが初期化されているかどうかを取得します。
*
* @tparam TTarget 対象の低レベルグラフィックス API
* @tparam TObject 対象のオブジェクト型
*
* @param[in] obj 対象のオブジェクト
*
* @return 対象のオブジェクトが初期化されているかどうかの真偽値を返します。
*/
template< typename TTarget, template< typename > class TObject >
bool IsInitialized( const TObject< TTarget >& obj ) NN_NOEXCEPT
{
    return obj.ToData()->state != TObject< TTarget >::DataType::State_NotInitialized;
}

}
}
