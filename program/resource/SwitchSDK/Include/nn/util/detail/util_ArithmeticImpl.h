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

#include <nn/nn_Common.h>
#include <nn/util/util_MathTypes.h>
#include <nn/util/detail/util_ConstantImpl.h>

namespace nn { namespace util { namespace detail {

//
// 多項式近似による三角関数の内部実装に用います。
//

extern const float SinCoefficients[5];
extern const float CosCoefficients[5];
extern const float AtanCoefficients[8];

// 対象を [-pi, pi] の範囲に mod します。
NN_FORCEINLINE float ModPi(float x) NN_NOEXCEPT
{
    float factor = (x >= 0.f) ? 0.5f : -0.5f;
    float roundedQuotient = (float)((int)(x * Float1Divided2Pi + factor));

    return x - Float2Pi * roundedQuotient;
}

NN_FORCEINLINE float AtanEstImpl(float x) NN_NOEXCEPT
{
    float xx = x * x;
    return ((((((((AtanCoefficients[0] * xx - AtanCoefficients[1]) * xx + AtanCoefficients[2]) * xx - AtanCoefficients[3]) * xx + AtanCoefficients[4]) * xx - AtanCoefficients[5]) * xx + AtanCoefficients[6]) * xx - AtanCoefficients[7]) * xx + 1.f) * x;
}

//
// テーブルによる三角関数の内部実装に用います。
//

struct SinCosSample
{
    float cosValue;
    float sinValue;
    float cosDelta;
    float sinDelta;
};

struct AtanSample
{
    AngleIndex atanValue;
    float atanDelta;
};

extern const SinCosSample SinCosSampleTable[256];
extern const AtanSample AtanSampleTable[128 + 1];

NN_FORCEINLINE AngleIndex AtanTableImpl(float x) NN_NOEXCEPT
{
    float temporary = x * 128.f;
    int sampleTableIndex = static_cast<int>(temporary);
    float rest = temporary - sampleTableIndex;

    return AtanSampleTable[sampleTableIndex].atanValue + static_cast<AngleIndex>(AtanSampleTable[sampleTableIndex].atanDelta * rest);
}

}}}
