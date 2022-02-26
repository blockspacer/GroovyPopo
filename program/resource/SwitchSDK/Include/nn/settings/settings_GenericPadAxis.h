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
 * @brief       汎用ゲームパッドの座標軸定義
 */

#pragma once

#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace settings {

/**
 * @brief       汎用ゲームパッドの座標軸定義です。
 */
enum GenericPadAxis
{
    GenericPadAxis_X = 1,   //!< 汎用ゲームパッドの座標軸 x
    GenericPadAxis_Y,       //!< 汎用ゲームパッドの座標軸 y
    GenericPadAxis_Z,       //!< 汎用ゲームパッドの座標軸 z
    GenericPadAxis_R,       //!< 汎用ゲームパッドの座標軸 r
    GenericPadAxis_U,       //!< 汎用ゲームパッドの座標軸 u
    GenericPadAxis_V,       //!< 汎用ゲームパッドの座標軸 v
};

/**
 * @brief       汎用ゲームパッドの座標軸の属性定義です。
 */
struct GenericPadAxisAttribute
{
    typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute>::Flag<0> IsXAxisInverted;    //!< x 軸の向きを反転するか否か
    typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute>::Flag<1> IsYAxisInverted;    //!< y 軸の向きを反転するか否か
    typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute>::Flag<2> IsZAxisInverted;    //!< z 軸の向きを反転するか否か
    typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute>::Flag<3> IsRAxisInverted;    //!< r 軸の向きを反転するか否か
    typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute>::Flag<4> IsUAxisInverted;    //!< u 軸の向きを反転するか否か
    typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute>::Flag<5> IsVAxisInverted;    //!< v 軸の向きを反転するか否か
};

/**
 * @brief       汎用ゲームパッドの座標軸の属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, GenericPadAxisAttribute> GenericPadAxisAttributeSet;

}} // namespace nn::settings
