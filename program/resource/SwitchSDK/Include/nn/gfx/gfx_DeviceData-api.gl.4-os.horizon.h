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
* @brief デバイスのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#if !defined( NN_BUILD_CONFIG_OS_SUPPORTS_HORIZON )
    #error
#endif

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_DeviceInfoData.h>

#include <nn/gfx/detail/gfx_Common-api.gl.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief デバイスを表す構造体です（GL4 特殊版）
*/
template<>
struct DeviceImplData< ApiVariationGl4 >
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
    Bit8 debugMode; //!< デバッグモードです
    nn::util::BitPack8 flags; //!< フラグです
    char reserved; //!< 予約領域です。
    uint32_t maxColorAttachments; //!< 最大のカラーアタッチメント数です
    uint32_t alignmentConstantBuffer; //!< 定数バッファーのアライメントです
    uint32_t alignmentUnorderedAccessBuffer; //!< アンオーダードアクセスバッファーのアライメントです
    detail::Ptr< void > hDisplay; //!< ディスプレイハンドルです
    detail::Ptr< void > hSurface; //!< サーフェイスハンドルです
    detail::GlRenderingContext renderingContext; //!< レンダリングコンテキストです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
