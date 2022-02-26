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
* @brief コマンドバッファーのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief コマンドバッファーを表す構造体です（GL4 特殊版）
*/
template<>
struct CommandBufferImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized,
        State_Begun,
        State_Callback
    };

    Bit8 state; //!< オブジェクトの状態です。
    Bit8 commandBufferType; //!< コマンドバッファーの種類です
    Bit16 queueCapability; //!< 提出先のキューの機能性です
    uint32_t commandMemorySize; //!< コマンドメモリーのサイズです
    uint32_t controlMemorySize; //!< コントロールメモリーのサイズです
    char reserved[ 4 ]; //!< 予約領域です。
    detail::Ptr< detail::DeviceImpl< ApiVariationGl4 > > pGfxDevice; //!< デバイスへのポインターです
    detail::Ptr< const void > pCommandList; //!< コマンドリストへのポインターです
    detail::Ptr< const void > pHeadCommandMemory; //!< コマンドメモリーの先頭へのポインターです
    detail::Ptr< void > pCommandMemory; //!< コマンドメモリーへのポインターです
    detail::Ptr< const void > pHeadControlMemory; //!< コントロールメモリーの先頭へのポインターです
    detail::Ptr< void > pControlMemory; //!< コントロールメモリーへのポインターです
    detail::Ptr< void () > pOutOfCommandMemoryCallback; //!< コマンドメモリーが不足したときに呼ばれるコールバックへのポインターです
    detail::Ptr< void () > pOutOfControlMemoryCallback; //!< コントロールメモリーが不足したときに呼ばれるコールバックへのポインターです

    detail::Ptr< const detail::RootSignatureImpl< ApiVariationGl4 > > pGfxRootSignature; //!< ルートシグネチャーへのポインターです

    detail::Ptr< void > userPtr; //!< ユーザーデータへのポインターです

    char defaultControlMemory[ 40 ]; //!< デフォルトのコントロールメモリーです
};

}
}
