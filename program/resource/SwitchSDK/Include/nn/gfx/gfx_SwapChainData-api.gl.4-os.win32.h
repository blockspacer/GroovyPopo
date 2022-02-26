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
* @brief スワップチェーンのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#if !defined( NN_BUILD_CONFIG_OS_SUPPORTS_WIN32 )
    #error "OS 種別として Win32 が指定されていません。"
#endif

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_TextureData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Common-api.gl.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief スワップチェーンを表す構造体です（GL4 特殊版）
*/
template<>
struct SwapChainImplData< ApiVariationGl4 >
{
    typedef void* WndHandle;
    typedef void* DcHandle;

    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_Acquired,
        Flag_Copied
    };

    static const int MaxScanBuffer = 6;

    Bit8 state; //!< オブジェクトの状態です
    nn::util::BitPack8 flag; //!< フラグです
    int8_t scanBufferCount; //!< スキャンバッファーの数です
    int8_t currentScanBufferIndex; //!< 現在のスキャンバッファーインデックスです
    char reserved[ 4 ]; //!< 予約領域です。
    detail::Ptr< void > hWnd; //!< ウインドウハンドルです
    detail::Ptr< void > hDc; //!< デバイスコンテキストです
    detail::Ptr< detail::DeviceImpl< ApiVariationGl4 > > pGfxDevice; //!< デバイスへのポインターです

    TextureImplData< ApiVariationGl4 > scanBuffers[ MaxScanBuffer ]; // スキャンバッファーです
    ColorTargetViewImplData< ApiVariationGl4 > scanBufferViews[ MaxScanBuffer ]; //!< スキャンバッファービューです

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
