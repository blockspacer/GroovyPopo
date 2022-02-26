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
* @brief デスクリプタープールのデータに関する API の宣言（NVN8 特殊版）
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
* @brief デスクリプタープールを表す構造体です（NVN8 特殊版）。
*/
template<>
struct DescriptorPoolImplData< ApiVariationNvn8 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized,
        State_Begun
    };

    enum Flag
    {
        Flag_Shared
    };

    Bit8 state; //!< オブジェクトの状態です
    Bit8 descriptorPoolType; //!< nn::gfx::DescriptorPoolType によって表される、デスクリプタープールの種類です
    nn::util::BitPack8 flags; //!< フラグです
    char reserved; //!< 予約領域です。
    int32_t slotCount; //!< デスクリプタープールのスロット数です
    int32_t reservedSlots;
    char reserved2[ 4 ]; //!< 予約領域です。

    detail::Ptr< void > pDescriptorPool;

    NN_ALIGNAS( 8 ) char nvnDescriptorPool[ 32 ];

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
