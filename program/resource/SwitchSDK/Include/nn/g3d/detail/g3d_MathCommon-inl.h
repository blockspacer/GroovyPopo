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

#include <cmath>
#include <algorithm>
#include <limits>

#if defined( NN_BUILD_CONFIG_OS_COS )
#include <ppc_ghs.h>
#endif

#include <nn/g3d/detail/g3d_Inlines.h>

namespace nn { namespace g3d { namespace detail {

//--------------------------------------------------------------------------------------------------

inline
float Math::Abs(float x) NN_NOEXCEPT
{
    return std::fabs(x);
}

inline
float Math::NAbs(float x) NN_NOEXCEPT
{
    return -std::fabs(x);
}

inline
float Math::Mod(float x, float y) NN_NOEXCEPT
{
    return std::fmod(x, y);
}

inline
float Math::Select(float cond, float pos, float neg) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return __FSEL(cond, pos, neg);
#else
    return cond >= 0.0f ? pos : neg;
#endif
}

inline
float Math::Min(float x, float y) NN_NOEXCEPT
{
    return Select(x - y, y, x);
}

inline
float Math::Max(float x, float y) NN_NOEXCEPT
{
    return Select(x - y, x, y);
}

inline
float Math::Clamp(float x, float minVal, float maxVal) NN_NOEXCEPT
{
    return Max(minVal, Min(maxVal, x));
}

//--------------------------------------------------------------------------------------------------

template <typename T>
inline
T Math::Ceil(float x) NN_NOEXCEPT
{
    return StaticCast<T>(std::ceil(x));
}

template <typename T>
inline
T Math::Floor(float x) NN_NOEXCEPT
{
    if (NN_STATIC_CONDITION(std::numeric_limits<T>::is_signed))
    {
        return StaticCast<T>(std::floor(x));
    }
    else
    {
        return Trunc<T>(x);
    }
}

template <typename T>
inline
T Math::Trunc(float x) NN_NOEXCEPT
{
    return StaticCast<T>(x);
}

template <>
inline
float Math::Trunc<float>(float x) NN_NOEXCEPT
{
    float out;
    std::modf(x, &out);
    return out;
}

template <typename T>
inline
T Math::Round(float x) NN_NOEXCEPT
{
    if(NN_STATIC_CONDITION(std::numeric_limits<T>::is_signed))
    {
        return StaticCast<T>(x + Select(x, 0.5f, -0.5f));
    }
    else
    {
        return StaticCast<T>(x);
    }
}

template <>
inline
float Math::Round<float>(float x) NN_NOEXCEPT
{
    float out;
    std::modf(x + Select(x, 0.5f, -0.5f), &out);
    return out;
}


inline
float Math::Ceil(float x) NN_NOEXCEPT
{
    return Ceil<float>(x);
}

inline
float Math::Floor(float x) NN_NOEXCEPT
{
    return Floor<float>(x);
}

inline
float Math::Trunc(float x) NN_NOEXCEPT
{
    return Trunc<float>(x);
}

inline
float Math::Round(float x) NN_NOEXCEPT
{
    return Round<float>(x);
}

//--------------------------------------------------------------------------------------------------

inline
float Math::Exp(float x) NN_NOEXCEPT
{
    return std::exp(x);
}

inline
float Math::Log(float x) NN_NOEXCEPT
{
    return std::log(x);
}

inline
float Math::Log10(float x) NN_NOEXCEPT
{
    return std::log10(x);
}

//--------------------------------------------------------------------------------------------------

inline
bool Math::IsNaN(float x) NN_NOEXCEPT
{
    return x != x;
}

}}} // namespace nn::g3d::detail
