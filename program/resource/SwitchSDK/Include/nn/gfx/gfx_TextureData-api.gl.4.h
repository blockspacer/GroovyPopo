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
* @brief テクスチャーのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>

#include <nn/gfx/detail/gfx_Common-api.gl.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief テクスチャーを表す構造体です（GL4 特殊版）
*/
template<>
struct TextureImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_Shared
    };

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    detail::GlHandle hTexture; //!< テクスチャーハンドルです
    uint32_t target; //!< バインドターゲットです
    Bit32 imageFormat; //!< イメージフォーマットです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

/**
* @brief テクスチャービューを表す構造体です（GL4 特殊版）
*/
template<>
struct TextureViewImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_Shared,
        Flag_HandleShared
    };

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    detail::GlHandle hTexture; //!< テクスチャーハンドルです
    uint32_t target; //!< バインドターゲットです
    char reserved2[ 4 ]; //!< 予約領域です。

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

/**
* @brief カラーターゲットビューを表す構造体です（GL4 特殊版）
*/
template<>
struct ColorTargetViewImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_Shared,
        Flag_HandleShared
    };

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    detail::GlHandle hTexture; //!< テクスチャーハンドルです
    uint32_t target; //!< バインドターゲットです
    int32_t layer; //!< 3D カラーターゲットのレイヤです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

/**
* @brief 深度ステンシルビューを表す構造体です（GL4 特殊版）
*/
template<>
struct DepthStencilViewImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_Shared,
        Flag_HandleShared
    };

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    detail::GlHandle hTexture; //!< テクスチャーハンドルです
    uint32_t target; //!< バインドターゲットです
    uint32_t internalFormat;

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
