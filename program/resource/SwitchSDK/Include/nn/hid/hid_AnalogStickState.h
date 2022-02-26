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
 * @brief       アナログスティックに関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace hid {

const int AnalogStickMax = 0x7fff;  //!< アナログスティックの最大値

/**
 * @brief       アナログスティックの入力状態を表す構造体です。
 */
struct AnalogStickState
{
     int32_t x;  //!< アナログスティックの x 軸座標
     int32_t y;  //!< アナログスティックの y 軸座標
};

}} // namespace nn::hid
