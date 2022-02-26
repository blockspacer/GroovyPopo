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
* @brief バッファーのデータに関する API の宣言（GL4 特殊版）
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
* @brief Buffer を表す構造体です（GL4 特殊版）
*/
template<>
struct BufferImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_Shared,
        Flag_FlushExplicit
    };

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    detail::GlHandle hBuffer; //!< バッファーハンドルです
    detail::Ptr< detail::DeviceImpl< ApiVariationGl4 > > pGfxDevice; //!< デバイスへのポインターです
    detail::Ptr< void > pMapped; //!< マップポインターです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

/**
* @brief BufferTextureView を表す構造体です（GL4 特殊版）
*/
template<>
struct BufferTextureViewImplData< ApiVariationGl4 >
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

    Bit8 state; //!< オブジェクトの状態です。
    nn::util::BitPack8 flags; //!< フラグです。
    char reserved[ 2 ]; //!< 予約領域です。

    detail::GlHandle hBufferTexture; //!< バッファーテクスチャーのハンドルです。
};

}
}
