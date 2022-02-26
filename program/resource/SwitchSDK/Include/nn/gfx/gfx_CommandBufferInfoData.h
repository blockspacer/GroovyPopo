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
* @brief コマンドバッファー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief コマンドバッファーを初期化するための情報を表す構造体です。
*/
struct CommandBufferInfoData
{
    Bit8 commandBufferType; //!< nn::gfx::CommandBufferType によって表される、コマンドバッファーの種類です
    char reserved2; //!< 予約領域です。
    Bit16 queueCapability; //!< nn::gfx::QueueCapabirity の組み合わせによって表される、提出先のキューの機能性です

    char reserved[ 28 ]; //!< 予約領域です
};

}
}
