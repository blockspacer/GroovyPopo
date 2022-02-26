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
* @brief 同期オブジェクト初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

namespace nn {
namespace gfx {

/**
* @brief フェンスを初期化するための情報を表す構造体です。
*/
struct FenceInfoData
{
    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief セマフォを初期化するための情報を表す構造体です。
*/
struct SemaphoreInfoData
{
    char reserved[ 16 ]; //!< 予約領域です
};

}
}
