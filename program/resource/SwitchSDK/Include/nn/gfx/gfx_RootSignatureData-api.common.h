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
* @brief ルートシグネチャーのデータに関する API の宣言（共通版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief ルートシグネチャーを表す構造体です（共通版）
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
template<>
struct NN_DEPRECATED RootSignatureImplData< ApiType< LowLevelApi_Common > >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です
    char reserved[ 3 ]; //!< 予約領域です。
    uint32_t memorySize; //!< バイトでのメモリーのサイズです
    detail::Ptr< void > pWorkMemory; //!< メモリーへのポインターです
};

}
}
