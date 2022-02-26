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
#include <nn/util/util_Arithmetic.h>
#include <nn/util/detail/util_MathTypes.general.h>
#include <nn/util/detail/util_VectorApi.general.h>
#include <nn/util/detail/util_ConstantImpl.h>
#include <cmath>

//
// SIMD ユニットを用いないベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::general 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

namespace nn { namespace util { namespace general {

NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionIdentity(Vector4fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionAdd(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionSubtract(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionDivide(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE float      QuaternionDot(const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionInverse(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float      QuaternionNormalize(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionExp(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionLogN(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionSlerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionSquad(Vector4fType* pOutValue, const Vector4fType& p, const Vector4fType& a, const Vector4fType& b, const Vector4fType& q, float t) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionMakeClosest(Vector4fType*  pOutValue, const Vector4fType& quaternion, const Vector4fType& quaternionTo) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionRotateAxis(Vector4fType* pOutValue, const Vector3fType& axis, float rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       QuaternionMakeVectorRotation(Vector4fType* pOutValue, const Vector3fType& from, const Vector3fType& to) NN_NOEXCEPT;

//
// ユーティリティ
//
// クオータニオン
NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    const float (*const m)[3] = matrix._m;

    float tr = m[0][0] + m[1][1] + m[2][2];

    if ( tr > 0.0f )
    {
        float s = ::std::sqrt(tr + 1.0f);

        pOutValue->_v[3] = s * 0.5f;

        s = 0.5f / s;

        pOutValue->_v[0] = (m[1][2] - m[2][1]) * s;
        pOutValue->_v[1] = (m[2][0] - m[0][2]) * s;
        pOutValue->_v[2] = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        int i = 0;

        if (m[1][1] > m[0][0])
        {
            i = 1;
        }

        if (m[2][2] > m[i][i])
        {
            i = 2;
        }

        int nxt[3] = {1, 2, 0};
        int j = nxt[i];
        int k = nxt[j];

        float q[3];
        float s = ::std::sqrt( (m[i][i] - (m[j][j] + m[k][k])) + 1.0f );

        q[i] = s * 0.5f;

        if (s != 0.0f)
        {
            s = 0.5f / s;
        }

        pOutValue->_v[3] = (m[j][k] - m[k][j]) * s;

        q[j] = (m[j][i] + m[i][j]) * s;
        q[k] = (m[k][i] + m[i][k]) * s;

        pOutValue->_v[0] = q[0];
        pOutValue->_v[1] = q[1];
        pOutValue->_v[2] = q[2];
    }
}

NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    const float (*const m)[4] = matrix._m;

    float tr = m[0][0] + m[1][1] + m[2][2];

    if ( tr > 0.0f )
    {
        float s = ::std::sqrt(tr + 1.0f);

        pOutValue->_v[3] = s * 0.5f;

        s = 0.5f / s;

        pOutValue->_v[0] = (m[2][1] - m[1][2]) * s;
        pOutValue->_v[1] = (m[0][2] - m[2][0]) * s;
        pOutValue->_v[2] = (m[1][0] - m[0][1]) * s;
    }
    else
    {
        int i = 0;

        if (m[1][1] > m[0][0])
        {
            i = 1;
        }

        if (m[2][2] > m[i][i])
        {
            i = 2;
        }

        int nxt[3] = {1, 2, 0};
        int j = nxt[i];
        int k = nxt[j];

        float q[3];
        float s = ::std::sqrt( (m[i][i] - (m[j][j] + m[k][k])) + 1.0f );

        q[i] = s * 0.5f;

        if (s != 0.0f)
        {
            s = 0.5f / s;
        }

        pOutValue->_v[3] = (m[k][j] - m[j][k]) * s;

        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        pOutValue->_v[0] = q[0];
        pOutValue->_v[1] = q[1];
        pOutValue->_v[2] = q[2];
    }
}

//
// クオータニオン
//

NN_FORCEINLINE void       QuaternionIdentity(Vector4fType* pOutValue) NN_NOEXCEPT
{
    VectorSet(pOutValue, 0.f, 0.f, 0.f, 1.f);
}

NN_FORCEINLINE void       QuaternionAdd(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    VectorAdd(pOutValue, quaternion1, quaternion2);
}

NN_FORCEINLINE void       QuaternionSubtract(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    VectorSubtract(pOutValue, quaternion1, quaternion2);
}

NN_FORCEINLINE void       QuaternionDivide(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    Vector4fType quaternion2Inversed;

    QuaternionInverse(&quaternion2Inversed, quaternion2);
    QuaternionMultiply(pOutValue, quaternion2Inversed, quaternion1);
}

NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion, float factor) NN_NOEXCEPT
{
    VectorMultiply(pOutValue, quaternion, factor);
}

NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    float x = quaternion1._v[3] * quaternion2._v[0] + quaternion1._v[0] * quaternion2._v[3] + quaternion1._v[1] * quaternion2._v[2] - quaternion1._v[2] * quaternion2._v[1];
    float y = quaternion1._v[3] * quaternion2._v[1] + quaternion1._v[1] * quaternion2._v[3] + quaternion1._v[2] * quaternion2._v[0] - quaternion1._v[0] * quaternion2._v[2];
    float z = quaternion1._v[3] * quaternion2._v[2] + quaternion1._v[2] * quaternion2._v[3] + quaternion1._v[0] * quaternion2._v[1] - quaternion1._v[1] * quaternion2._v[0];
    float w = quaternion1._v[3] * quaternion2._v[3] - quaternion1._v[0] * quaternion2._v[0] - quaternion1._v[1] * quaternion2._v[1] - quaternion1._v[2] * quaternion2._v[2];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE float      QuaternionDot(const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    return VectorDot(quaternion1, quaternion2);
}

NN_FORCEINLINE void       QuaternionInverse(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float magnitude = VectorLengthSquared(quaternion);
    float reciprocalMagnitude = (magnitude != 0.f) ? 1.f / magnitude : 1.f;

    pOutValue->_v[0] = -quaternion._v[0] * reciprocalMagnitude;
    pOutValue->_v[1] = -quaternion._v[1] * reciprocalMagnitude;
    pOutValue->_v[2] = -quaternion._v[2] * reciprocalMagnitude;
    pOutValue->_v[3] =  quaternion._v[3] * reciprocalMagnitude;
}

NN_FORCEINLINE float      QuaternionNormalize(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float magnitude = VectorLengthSquared(quaternion);

    if(magnitude >= nn::util::detail::FloatQuaternionEpsilon)
    {
        float reciprocalMagnitude = 1.f / ::std::sqrt(magnitude);

        pOutValue->_v[0] = quaternion._v[0] * reciprocalMagnitude;
        pOutValue->_v[1] = quaternion._v[1] * reciprocalMagnitude;
        pOutValue->_v[2] = quaternion._v[2] * reciprocalMagnitude;
        pOutValue->_v[3] = quaternion._v[3] * reciprocalMagnitude;
    }
    else
    {
        magnitude = 0.f;
        VectorZero(pOutValue);
    }

    return magnitude;
}

NN_FORCEINLINE void       QuaternionExp(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float theta = ::std::sqrt(quaternion._v[0] * quaternion._v[0] + quaternion._v[1] * quaternion._v[1] + quaternion._v[2] * quaternion._v[2]);
    float scale = (theta > nn::util::detail::FloatQuaternionEpsilon) ? nn::util::SinEst(theta) / theta : 1.f;

    pOutValue->_v[0] = quaternion._v[0] * scale;
    pOutValue->_v[1] = quaternion._v[1] * scale;
    pOutValue->_v[2] = quaternion._v[2] * scale;
    pOutValue->_v[3] = nn::util::CosEst(theta);
}

NN_FORCEINLINE void       QuaternionLogN(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float theta = ::std::sqrt(quaternion._v[0] * quaternion._v[0] + quaternion._v[1] * quaternion._v[1] + quaternion._v[2] * quaternion._v[2]);
    float scale = (theta > 0.f) ? nn::util::Atan2Est(theta, quaternion._v[3]) / theta : theta;

    pOutValue->_v[0] = quaternion._v[0] * scale;
    pOutValue->_v[1] = quaternion._v[1] * scale;
    pOutValue->_v[2] = quaternion._v[2] * scale;
    pOutValue->_v[3] = 0.f;
}

NN_FORCEINLINE void       QuaternionLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT
{
    VectorLerp(pOutValue, from, to, t);
}

NN_FORCEINLINE void       QuaternionSlerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT
{
    float theta, sin_th, cos_th, tp, tq;

    cos_th = QuaternionDot(from, to);
    tq     = 1.f;

    if ( cos_th < 0.f )
    {
        cos_th = -cos_th;
        tq     = -tq;
    }

    if ( cos_th <= 1.f - nn::util::detail::FloatQuaternionEpsilon )
    {
        theta  = nn::util::AcosEst(cos_th);
        sin_th = nn::util::SinEst(theta);
        tp     = nn::util::SinEst((1.f - t) * theta) / sin_th;
        tq    *= nn::util::SinEst( t * theta ) / sin_th;
    }
    else
    {
        tp = 1.f - t;
        tq = tq * t;
    }

    pOutValue->_v[0] = tp * from._v[0] + tq * to._v[0];
    pOutValue->_v[1] = tp * from._v[1] + tq * to._v[1];
    pOutValue->_v[2] = tp * from._v[2] + tq * to._v[2];
    pOutValue->_v[3] = tp * from._v[3] + tq * to._v[3];
}

NN_FORCEINLINE void       QuaternionSquad(Vector4fType* pOutValue, const Vector4fType& p, const Vector4fType& a, const Vector4fType& b, const Vector4fType& q, float t) NN_NOEXCEPT
{
    float t2 = 2 * t * (1.f - t);

    Vector4fType pq, ab;

    QuaternionSlerp(&pq, p, q, t);
    QuaternionSlerp(&ab, a, b, t);
    QuaternionSlerp(pOutValue, pq, ab, t2);
}

NN_FORCEINLINE void       QuaternionMakeClosest(Vector4fType*  pOutValue, const Vector4fType& quaternion, const Vector4fType& quaternionTo) NN_NOEXCEPT
{
    float dot = QuaternionDot(quaternion, quaternionTo);

    if(dot < 0.f)
    {
        pOutValue->_v[0] = -quaternion._v[0];
        pOutValue->_v[1] = -quaternion._v[1];
        pOutValue->_v[2] = -quaternion._v[2];
        pOutValue->_v[3] = -quaternion._v[3];
    }
    else
    {
        *pOutValue = quaternion;
    }
}

NN_FORCEINLINE void       QuaternionRotateAxis(Vector4fType* pOutValue, const Vector3fType& axis, float radian) NN_NOEXCEPT
{
    Vector3fType axisNormalized;
    VectorNormalize(&axisNormalized, axis);

    float halfRadian = radian * 0.5f;
    float halfSin, halfCos;
    nn::util::SinCosEst(&halfSin, &halfCos, halfRadian);

    pOutValue->_v[0] = halfSin * axisNormalized._v[0];
    pOutValue->_v[1] = halfSin * axisNormalized._v[1];
    pOutValue->_v[2] = halfSin * axisNormalized._v[2];
    pOutValue->_v[3] = halfCos;
}

NN_FORCEINLINE void       QuaternionMakeVectorRotation(Vector4fType* pOutValue, const Vector3fType& from, const Vector3fType& to) NN_NOEXCEPT
{
    Vector3fType cross;
    VectorCross(&cross, from, to);

    float dotPlus1 = VectorDot(from, to) + 1.f;

    if(dotPlus1 <= nn::util::detail::FloatUlp)
    {
        pOutValue->_v[0] = 1.f;
        pOutValue->_v[1] = 0.f;
        pOutValue->_v[2] = 0.f;
        pOutValue->_v[3] = 0.f;
    }
    else
    {
        float s = ::std::sqrt(dotPlus1 * 2.f);
        float oos = 1.f / s;

        pOutValue->_v[0] = cross._v[0] * oos;
        pOutValue->_v[1] = cross._v[1] * oos;
        pOutValue->_v[2] = cross._v[2] * oos;
        pOutValue->_v[3] = s * 0.5f;
    }
}

}}} // namespace nn::util::general
