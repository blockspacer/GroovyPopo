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
#include <nn/util/detail/util_ArithmeticImpl.h>
#include <cmath>
#include <algorithm>

/**
* @file
* @brief    三角関数、算術ユーティリティを定義します。
*/

namespace nn { namespace util {

//! @name   三角関数
//! @{

/**
* @brief    角度をラジアン表現からインデックス表現に変換します。
*
* @param[in]    radian  角度（ラジアン表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE AngleIndex RadianToAngleIndex(float radian) NN_NOEXCEPT
{
    return static_cast<AngleIndex>(static_cast<int64_t>(radian * (nn::util::detail::AngleIndexHalfRound / nn::util::detail::FloatPi)));
}

/**
* @brief    角度を度数表現からインデックス表現に変換します。
*
* @param[in]    degree  角度（度数表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE AngleIndex DegreeToAngleIndex(float degree) NN_NOEXCEPT
{
    return static_cast<AngleIndex>(static_cast<int64_t>(degree * (nn::util::detail::AngleIndexHalfRound / nn::util::detail::FloatDegree180)));
}

/**
* @brief    角度を度数表現からラジアン表現に変換します。
*
* @param[in]    degree  角度（度数表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float DegreeToRadian(float degree) NN_NOEXCEPT
{
    return degree * (nn::util::detail::FloatPi / nn::util::detail::FloatDegree180);
}

/**
* @brief    角度をラジアン表現から度数表現に変換します。
*
* @param[in]    radian  角度（ラジアン表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float RadianToDegree(float radian) NN_NOEXCEPT
{
    return radian * (nn::util::detail::FloatDegree180 / nn::util::detail::FloatPi);
}

/**
* @brief    角度をインデックス表現からラジアン表現に変換します。
*
* @param[in]    angleIndex  角度（インデックス表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float AngleIndexToRadian(AngleIndex angleIndex) NN_NOEXCEPT
{
    return angleIndex * (nn::util::detail::FloatPi / nn::util::detail::AngleIndexHalfRound);
}

/**
* @brief    角度をインデックス表現から度数表現に変換します。
*
* @param[in]    angleIndex  角度（インデックス表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float AngleIndexToDegree(AngleIndex angleIndex) NN_NOEXCEPT
{
    return angleIndex * (nn::util::detail::FloatDegree180 / nn::util::detail::AngleIndexHalfRound);
}

/**
* @brief    多項式近似によって、正弦を求めます。
*
* @param[in]    radian  角度（ラジアン表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float SinEst(float radian) NN_NOEXCEPT
{
    float x = nn::util::detail::ModPi(radian);

    if(x > nn::util::detail::FloatPiDivided2)
    {
        x = nn::util::detail::FloatPi - x;
    }
    else if(x < -nn::util::detail::FloatPiDivided2)
    {
        x = -nn::util::detail::FloatPi - x;
    }

    float xx = x * x;

    return ( ( ( ( (-nn::util::detail::SinCoefficients[0] * xx + nn::util::detail::SinCoefficients[1]) * xx - nn::util::detail::SinCoefficients[2] ) * xx + nn::util::detail::SinCoefficients[3] ) * xx - nn::util::detail::SinCoefficients[4] ) * xx + 1.0f ) * x;
}

/**
* @brief    テーブル引きによって、正弦を求めます。
*
* @param[in]    angleIndex  角度（インデックス表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float SinTable(AngleIndex angleIndex) NN_NOEXCEPT
{
    uint32_t sampleTableIndex = (angleIndex >> 24) & 0xFF;
    float rest = static_cast<float>(angleIndex & 0xFFFFFF) / 0x1000000;
    const nn::util::detail::SinCosSample* table = &nn::util::detail::SinCosSampleTable[sampleTableIndex];

    return table->sinValue + table->sinDelta * rest;
}

/**
* @brief    多項式近似によって、余弦を求めます。
*
* @param[in]    radian  角度（ラジアン表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float CosEst(float radian) NN_NOEXCEPT
{
    float x = nn::util::detail::ModPi(radian);

    float sign;
    if(x > nn::util::detail::FloatPiDivided2)
    {
        x = nn::util::detail::FloatPi - x;
        sign = -1.f;
    }
    else if(x < -nn::util::detail::FloatPiDivided2)
    {
        x = -nn::util::detail::FloatPi - x;
        sign = -1.f;
    }
    else
    {
        sign = 1.f;
    }

    float xx = x * x;

    float cos = ( ( ( ( -nn::util::detail::CosCoefficients[0] * xx + nn::util::detail::CosCoefficients[1] ) * xx - nn::util::detail::CosCoefficients[2] ) * xx + nn::util::detail::CosCoefficients[3] ) * xx - nn::util::detail::CosCoefficients[4] ) * xx + 1.0f;

    return sign * cos;
}

/**
* @brief    テーブル引きによって、余弦を求めます。
*
* @param[in]    angleIndex  角度（インデックス表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float CosTable(AngleIndex angleIndex) NN_NOEXCEPT
{
    uint32_t sampleTableIndex = (angleIndex >> 24) & 0xFF;
    float rest = static_cast<float>(angleIndex & 0xFFFFFF) / 0x1000000;
    const nn::util::detail::SinCosSample* table = &nn::util::detail::SinCosSampleTable[sampleTableIndex];

    return table->cosValue + table->cosDelta * rest;
}

/**
* @brief    多項式近似によって、正弦と余弦を求めます。
*
* @param[out]   pOutSin 正弦の格納先
* @param[out]   pOutCos 余弦の格納先
* @param[in]    radian  角度（ラジアン表現）
*/
NN_FORCEINLINE void SinCosEst(float* pOutSin, float* pOutCos, float radian) NN_NOEXCEPT
{
    float x = nn::util::detail::ModPi(radian);

    float sign;
    if(x > nn::util::detail::FloatPiDivided2)
    {
        x = nn::util::detail::FloatPi - x;
        sign = -1.f;
    }
    else if(x < -nn::util::detail::FloatPiDivided2)
    {
        x = -nn::util::detail::FloatPi - x;
        sign = -1.f;
    }
    else
    {
        sign = 1.f;
    }

    float xx = x * x;

    float sin = ( ( ( ( (-nn::util::detail::SinCoefficients[0] * xx + nn::util::detail::SinCoefficients[1]) * xx - nn::util::detail::SinCoefficients[2] ) * xx + nn::util::detail::SinCoefficients[3] ) * xx - nn::util::detail::SinCoefficients[4] ) * xx + 1.0f ) * x;
    float cos = ( ( ( ( -nn::util::detail::CosCoefficients[0] * xx + nn::util::detail::CosCoefficients[1] ) * xx - nn::util::detail::CosCoefficients[2] ) * xx + nn::util::detail::CosCoefficients[3] ) * xx - nn::util::detail::CosCoefficients[4] ) * xx + 1.0f;

    cos *= sign;

    *pOutSin = sin;
    *pOutCos = cos;
}

/**
* @brief    テーブル引きによって、正弦と余弦を求めます。
*
* @param[out]   pOutSin     正弦の格納先
* @param[out]   pOutCos     余弦の格納先
* @param[in]    angleIndex  角度（インデックス表現）
*/
NN_FORCEINLINE void SinCosTable(float* pOutSin, float* pOutCos, AngleIndex angleIndex) NN_NOEXCEPT
{
    uint32_t sampleTableIndex = (angleIndex >> 24) & 0xFF;
    float rest = static_cast<float>(angleIndex & 0xFFFFFF) / 0x1000000;
    const nn::util::detail::SinCosSample* table = &nn::util::detail::SinCosSampleTable[sampleTableIndex];

    *pOutSin = table->sinValue + table->sinDelta * rest;
    *pOutCos = table->cosValue + table->cosDelta * rest;
}

/**
* @brief    多項式近似によって、正接を求めます。
*
* @param[in]    radian  角度（ラジアン表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float TanEst(float radian) NN_NOEXCEPT
{
    float sin, cos;

    SinCosEst(&sin, &cos, radian);

    return sin / cos;
}

/**
* @brief    テーブル引きによって、正接を求めます。
*
* @param[in]    angleIndex  角度（インデックス表現）
*
* @return   結果を返します。
*/
NN_FORCEINLINE float TanTable(AngleIndex angleIndex) NN_NOEXCEPT
{
    uint32_t sampleTableIndex = (angleIndex >> 24) & 0xFF;
    float rest = static_cast<float>(angleIndex & 0xFFFFFF) / 0x1000000;
    const nn::util::detail::SinCosSample* table = &nn::util::detail::SinCosSampleTable[sampleTableIndex];

    return (table->sinValue + table->sinDelta * rest) / (table->cosValue + table->cosDelta * rest);
}

/**
* @brief    多項式近似によって、逆正接を求めます。
*
* @param[in]    x   正弦値
*
* @return   結果を返します。
*/
NN_FORCEINLINE float AtanEst(float x) NN_NOEXCEPT
{
    if (x >= 0.f)
    {
        if (x > 1.f)
        {
            return nn::util::detail::FloatPiDivided2 - nn::util::detail::AtanEstImpl(1.f / x);
        }
        else
        {
            return nn::util::detail::AtanEstImpl(x);
        }
    }
    else
    {
        if (x < -1.f)
        {
            return -nn::util::detail::FloatPiDivided2 + nn::util::detail::AtanEstImpl(-1.f / x);
        }
        else
        {
            return -nn::util::detail::AtanEstImpl(-x);
        }
    }
}

/**
* @brief    テーブル引きによって、逆正接を求めます。
*
* @param[in]    x   正弦値
*
* @return   結果を返します。
*/
NN_FORCEINLINE AngleIndex AtanTable(float x) NN_NOEXCEPT
{
    if (x >= 0.f)
    {
        if (x > 1.f)
        {
            return nn::util::detail::AngleIndexQuarterRound - nn::util::detail::AtanTableImpl(1.f / x);
        }
        else
        {
            return nn::util::detail::AtanTableImpl(x);
        }
    }
    else
    {
        if (x < -1.f)
        {
            return nn::util::detail::AngleIndexThreeQuarterRound + nn::util::detail::AtanTableImpl(-1.f / x);
        }
        else
        {
            return - static_cast<int>(nn::util::detail::AtanTableImpl(-x));
        }
    }
}


/**
* @brief    多項式近似によって、逆正接を求めます。
*
* @param[in]    y   値 Y
* @param[in]    x   値 X
*
* @return   結果を返します。
*/
NN_FORCEINLINE float Atan2Est(float y, float x) NN_NOEXCEPT
{
    if (x == 0.f && y == 0.f)
    {
        return 0;
    }

    if (x >= 0.f)
    {
        if (y >= 0.f)
        {
            if (x >= y)
            {
                return nn::util::detail::AtanEstImpl(y / x);
            }
            else
            {
                return nn::util::detail::FloatPiDivided2 - nn::util::detail::AtanEstImpl(x / y);
            }
        }
        else
        {
            if (x >= -y)
            {
                return -nn::util::detail::AtanEstImpl(-y / x);
            }
            else
            {
                return -nn::util::detail::FloatPiDivided2 + nn::util::detail::AtanEstImpl(x / -y);
            }
        }
    }
    else
    {
        if (y >= 0.f)
        {
            if (-x >= y)
            {
                return nn::util::detail::FloatPi - nn::util::detail::AtanEstImpl(y / -x);
            }
            else
            {
                return nn::util::detail::FloatPiDivided2 + nn::util::detail::AtanEstImpl(-x / y);
            }
        }
        else
        {
            if (x <= y)
            {
                return -nn::util::detail::FloatPi + nn::util::detail::AtanEstImpl(y / x);
            }
            else
            {
                return -nn::util::detail::FloatPiDivided2 - nn::util::detail::AtanEstImpl(x / y);
            }
        }
    }
}

/**
* @brief    テーブル引きによって、逆正接を求めます。
*
* @param[in]    y   値 Y
* @param[in]    x   値 X
*
* @return   結果を返します。
*/
NN_FORCEINLINE AngleIndex Atan2Table(float y, float x) NN_NOEXCEPT
{
    if (x == 0.f && y == 0.f)
    {
        return 0;
    }

    if (x >= 0.f)
    {
        if (y >= 0.f)
        {
            if (x >= y)
            {
                return nn::util::detail::AtanTableImpl(y / x);
            }
            else
            {
                return nn::util::detail::AngleIndexQuarterRound - nn::util::detail::AtanTableImpl(x / y);
            }
        }
        else
        {
            if (x >= -y)
            {
                return - static_cast<int>(nn::util::detail::AtanTableImpl(-y / x));
            }
            else
            {
                return nn::util::detail::AngleIndexThreeQuarterRound + nn::util::detail::AtanTableImpl(x / -y);
            }
        }
    }
    else
    {
        if (y >= 0.f)
        {
            if (-x >= y)
            {
                return nn::util::detail::AngleIndexHalfRound - nn::util::detail::AtanTableImpl(y / -x);
            }
            else
            {
                return nn::util::detail::AngleIndexQuarterRound + nn::util::detail::AtanTableImpl(-x / y);
            }
        }
        else
        {
            if (x <= y)
            {
                return nn::util::detail::AngleIndexHalfRound + nn::util::detail::AtanTableImpl(y / x);
            }
            else
            {
                return nn::util::detail::AngleIndexThreeQuarterRound - nn::util::detail::AtanTableImpl(x / y);
            }
        }
    }
}

/**
* @brief    多項式近似によって、逆正弦を求めます。
*
* @param[in]    x   正弦値
*
* @return   結果を返します。
*/
NN_FORCEINLINE float AsinEst(float x) NN_NOEXCEPT
{
    // nn::util::detail::FloatSqrt0Point5 を使用すると、Release ビルドにて "warning C4723" 0 除算に関する警告が発生するため、ここで別途定義します。
    const float FloatSqrt0Point5 = 0.707106781186548f;  // sqrt(0.5)

    if (x >= 0.f)
    {
        if (x > FloatSqrt0Point5)
        {
            return nn::util::detail::FloatPiDivided2 - nn::util::detail::AtanEstImpl(::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return nn::util::detail::AtanEstImpl(x / ::std::sqrt(1.f - x*x));
        }
    }
    else
    {
        if (x < -FloatSqrt0Point5)
        {
            return -nn::util::detail::FloatPiDivided2 + nn::util::detail::AtanEstImpl(-::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return -nn::util::detail::AtanEstImpl(-x / ::std::sqrt(1.f - x*x));
        }
    }
}

/**
* @brief    テーブル引きによって、逆正弦を求めます。
*
* @param[in]    x   正弦値
*
* @return   結果を返します。
*/
NN_FORCEINLINE AngleIndex AsinTable(float x) NN_NOEXCEPT
{
    const float FloatSqrt0Point5 = 0.707106781186548f;  // sqrt(0.5)

    if (x >= 0.f)
    {
        if (x > FloatSqrt0Point5)
        {
            return nn::util::detail::AngleIndexQuarterRound - nn::util::detail::AtanTableImpl(::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return nn::util::detail::AtanTableImpl(x / ::std::sqrt(1.f - x*x));
        }
    }
    else
    {
        if (x < -FloatSqrt0Point5)
        {
            return nn::util::detail::AngleIndexThreeQuarterRound + nn::util::detail::AtanTableImpl(-::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return - static_cast<int>(nn::util::detail::AtanTableImpl(-x / ::std::sqrt(1.f - x*x)));
        }
    }
}

/**
* @brief    多項式近似によって、逆余弦を求めます。
*
* @param[in]    x   余弦値
*
* @return   結果を返します。
*/
NN_FORCEINLINE float AcosEst(float x) NN_NOEXCEPT
{
    const float FloatSqrt0Point5 = 0.707106781186548f;  // sqrt(0.5)

    if (x >= 0.f)
    {
        if (x > FloatSqrt0Point5)
        {
            return nn::util::detail::AtanEstImpl(::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return nn::util::detail::FloatPiDivided2 - nn::util::detail::AtanEstImpl(x / ::std::sqrt(1.f - x*x));
        }
    }
    else
    {
        if (x < -FloatSqrt0Point5)
        {
            return nn::util::detail::FloatPi - nn::util::detail::AtanEstImpl(-::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return nn::util::detail::FloatPiDivided2 + nn::util::detail::AtanEstImpl(-x / ::std::sqrt(1.f - x*x));
        }
    }
}

/**
* @brief    テーブル引きによって、逆余弦を求めます。
*
* @param[in]    x   余弦値
*
* @return   結果を返します。
*/
NN_FORCEINLINE AngleIndex AcosTable(float x) NN_NOEXCEPT
{
    const float FloatSqrt0Point5 = 0.707106781186548f;  // sqrt(0.5)

    if (x >= 0.f)
    {
        if (x > FloatSqrt0Point5)
        {
            return nn::util::detail::AtanTableImpl(::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return nn::util::detail::AngleIndexQuarterRound - nn::util::detail::AtanTableImpl(x / ::std::sqrt(1.f - x*x));
        }
    }
    else
    {
        if (x < -FloatSqrt0Point5)
        {
            return nn::util::detail::AngleIndexHalfRound - nn::util::detail::AtanTableImpl(-::std::sqrt(1.f - x*x) / x);
        }
        else
        {
            return nn::util::detail::AngleIndexQuarterRound + nn::util::detail::AtanTableImpl(-x / ::std::sqrt(1.f - x*x));
        }
    }
}

//! @}

//! @name 算術ユーティリティ
//! @{

/**
* @brief    逆数を求めます。
*
* @param[in]    x   値
*
* @return   結果を返します。
*/
NN_FORCEINLINE float Rcp(float x) NN_NOEXCEPT
{
    return 1.f / x;
}

/**
* @brief    逆数を求めます。指定した回数だけ補正を行います。Rcp() よりも精度で劣ります。
*
* @tparam       N   補正回数
*
* @param[in]    x   値
*
* @details  プラットフォームによっては、精度で劣るかわりに高速な実装を利用できる場合があります。
*           本関数は、そのような最適化を期待する箇所で利用することを想定しています。
*/
template<int N>
NN_FORCEINLINE float RcpLowPrecision(float x) NN_NOEXCEPT
{
    return 1.f / x;
}

/**
* @brief    平方根の逆数を求めます。
*
* @param[in]    x   値
*
* @return   結果を返します。
*/
NN_FORCEINLINE float Rsqrt(float x) NN_NOEXCEPT
{
    return 1.f / ::std::sqrt(x);
}

/**
* @brief    平方根の逆数を求めます。 指定した回数だけ補正を行います。Rsqrt() よりも精度で劣ります。
*
* @tparam       N   補正回数
*
* @param[in]    x   値
*
* @return   結果を返します。
*
* @details  プラットフォームによっては、精度で劣るかわりに高速な実装を利用できる場合があります。
*           本関数は、そのような最適化を期待する箇所で利用することを想定しています。
*/
template<int N>
NN_FORCEINLINE float RsqrtLowPrecision(float x) NN_NOEXCEPT
{
    float estimate = Rsqrt(x);

    for(int i = 0; i < N; i++)
    {
        estimate = 0.5f * estimate * (3 - (x * estimate) * estimate);
    }

    return estimate;
}

/**
* @brief    絶対誤差を用いて等値判定を行います。
*
* @param[in]    x           値1
* @param[in]    y           値2
* @param[in]    absError    絶対誤差による誤差許容範囲
*
* @return   結果を返します。
* @retval   true    等値です。
* @retval   false   等値ではありません。
*/
NN_FORCEINLINE bool AreEqualAbs(float x, float y, float absError) NN_NOEXCEPT
{
    float diff = ::std::abs(x - y);
    return diff <= absError;
}

/**
* @brief    相対誤差を用いて等値判定を行います。
*
* @param[in]    x               値1
* @param[in]    y               値2
* @param[in]    relativeError   相対誤差による誤差許容範囲
*
* @return   結果を返します。
* @retval   true    等値です。
* @retval   false   等値ではありません。
*/
NN_FORCEINLINE bool AreEqualRelative(float x, float y, float relativeError) NN_NOEXCEPT
{
    float diff = ::std::abs(x - y);
    return diff <= relativeError * ::std::max(::std::abs(x), ::std::abs(y));
}

/**
* @brief    絶対誤差、もしくは、相対誤差を用いて等値判定を行います。
*
* @param[in]    x       値1
* @param[in]    y       値2
* @param[in]    error   絶対誤差、もしくは、相対誤差による誤差許容範囲
*
* @return   結果を返します。
* @retval   true    等値です。
* @retval   false   等値ではありません。
*
* @details  値1,2 の両方が 1 より小さい場合には絶対誤差として、いずれかの値が 1 より大きい場合には相対誤差として、指定された誤差許容範囲を用います。
*/
NN_FORCEINLINE bool AreEqual(float x, float y, float error) NN_NOEXCEPT
{
    float diff = ::std::abs(x - y);
    return diff <= error || diff <= error * ::std::max(::std::abs(x), ::std::abs(y));
}

//! @}

}} // namespace nn::util
