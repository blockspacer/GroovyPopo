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
* @brief 同期のデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief フェンスを表す構造体です（GL4 特殊版）
*/
template<>
struct FenceImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    enum Flag
    {
        Flag_Signaled
    };
    Bit8 state; //!< オブジェクトの状態です。
    nn::util::BitPack8 flags; //!< フラグです。
    char reserved[ 6 ]; //!< 予約領域です。
    detail::Ptr< void > glSync; //!< 同期オブジェクトです
    detail::Ptr< detail::QueueImpl< ApiVariationGl4 > > pQueue; //!< キューへのポインターです
};

/**
* @brief セマフォを表す構造体です（GL4 特殊版）
*/
template<>
struct SemaphoreImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    enum Flag
    {
        Flag_Signaled
    };
    Bit8 state; //!< オブジェクトの状態です。
    nn::util::BitPack8 flags; //!< フラグです。
    char reserved[ 6 ]; //!< 予約領域です。
    detail::Ptr< void > glSync; //!< 同期オブジェクトです
    detail::Ptr< detail::QueueImpl< ApiVariationGl4 > > pQueue; //!< キューへのポインターです
};

}
}
