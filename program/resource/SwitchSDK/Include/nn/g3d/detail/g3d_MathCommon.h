/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/g3d/g3d_Configuration.h>

namespace nn { namespace g3d { namespace detail {

class Math
{
public:

    static float Abs(float x) NN_NOEXCEPT;
    static float NAbs(float x) NN_NOEXCEPT;
    static float Mod(float x, float y) NN_NOEXCEPT;
    static float Select(float cond, float pos, float neg) NN_NOEXCEPT;
    static float Min(float x, float y) NN_NOEXCEPT;
    static float Max(float x, float y) NN_NOEXCEPT;
    static float Clamp(float x, float minVal, float maxVal) NN_NOEXCEPT;

    static float Ceil(float x) NN_NOEXCEPT;
    static float Floor(float x) NN_NOEXCEPT;
    static float Trunc(float x) NN_NOEXCEPT;
    static float Round(float x) NN_NOEXCEPT;

    template <typename T> static T Ceil(float x) NN_NOEXCEPT;
    template <typename T> static T Floor(float x) NN_NOEXCEPT;
    template <typename T> static T Trunc(float x) NN_NOEXCEPT;
    template <typename T> static T Round(float x) NN_NOEXCEPT;

    static float Exp(float x) NN_NOEXCEPT;
    static float Log(float x) NN_NOEXCEPT;
    static float Log10(float x) NN_NOEXCEPT;

#if defined ( NN_BUILD_CONFIG_OS_COS )
    static f32x2 Rcp(f32x2 x) NN_NOEXCEPT;
    static f32x2 RSqrt(f32x2 x) NN_NOEXCEPT;
#endif

    static bool IsNaN(float x) NN_NOEXCEPT;

    // インライン化のために関数化した定数
    inline static float Napier() NN_NOEXCEPT
    {
        return 2.718281828459045f;
    } // 自然対数底
};

}}} // namespace nn::g3d::detail

#include "g3d_MathCommon-inl.h"

