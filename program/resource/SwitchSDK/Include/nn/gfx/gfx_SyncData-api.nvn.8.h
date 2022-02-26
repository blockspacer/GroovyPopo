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
* @brief 同期のデータに関する API の宣言（NVN8 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief フェンスを表す構造体です（NVN8 特殊版）
*/
template<>
struct FenceImplData< ApiVariationNvn8 >
{
    detail::Ptr< const detail::DeviceImpl< ApiVariationNvn8 > > pNnDevice; //!< デバイスへのポインターです
    detail::Ptr< void > pNvnSync;

    NN_ALIGNAS( 8 ) char nvnSync[ 64 ]; // Storage of the NVNsync object

    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    char reserved[ 3 ]; //!< 予約領域です。
};

/**
* @brief セマフォを表す構造体です（NVN8 特殊版）
*/
template<>
struct SemaphoreImplData< ApiVariationNvn8 >
{
    detail::Ptr< const detail::DeviceImpl< ApiVariationNvn8 > > pNnDevice; //!< デバイスへのポインターです
    detail::Ptr< void > pNvnSync;

    NN_ALIGNAS( 8 ) char nvnSync[ 64 ]; // Storage of the NVNsync object

    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    char reserved[ 3 ]; //!< 予約領域です。
};

}
}
