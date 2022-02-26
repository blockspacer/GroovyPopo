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
* @brief キュー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

namespace nn {
namespace gfx {

/**
* @brief キューを初期化するための情報を表す構造体です。
*/
struct QueueInfoData
{
    Bit16 capability; //!< nn::gfx::QueueCapability の組み合わせによって表される、キューの機能性です

    char reserved[ 30 ]; //!< 予約領域です
};

}
}
