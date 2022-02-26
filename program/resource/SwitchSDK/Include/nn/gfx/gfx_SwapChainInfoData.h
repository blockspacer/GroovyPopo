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
* @brief スワップチェーン初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace vi {

class Layer;

}
}

namespace nn {
namespace gfx {

/**
* @brief スワップチェーンを初期化するための情報を表す構造体です。
*/
struct SwapChainInfoData
{
    uint8_t bufferCount; //!< スキャンバッファーの数です
    char reserved2[ 3 ]; //!< 予約領域です。
    Bit32 format; //!< nn::gfx::ImageFormat で表される、スキャンバッファーのフォーマットです
    uint32_t width; //!< スキャンバッファーの幅です
    uint32_t height; //!< スキャンバッファーの高さです

    char reserved3[ 8 ]; //!< 予約領域です。
    detail::Ptr< nn::vi::Layer > pLayer; //!< レイヤーへのポインターです

    char reserved[ 30 ]; //!< 予約領域です
};

}
}
