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
#include <nn/util/detail/util_ConstantImpl.h>
#include <nn/util/detail/util_VectorApiDetail.neon.h>
#include <nn/util/detail/util_MatrixApiDetail.neon.h>
#include <cmath>
#include <arm_neon.h>

namespace nn { namespace util { namespace neon { namespace detail {

NN_FORCEINLINE float32x4_t QuaternionFromMatrixRowMajor4x3f(float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4_t Sign0 = { 1.f, -1.f, -1.f, 1.f };
    float32x4_t Sign1 = { -1.f, 1.f, -1.f, 1.f };
    float32x4_t Sign2 = { -1.f, -1.f, 1.f, 1.f };

    float32x4_t xAxis = MatrixRowMajor4x3fGetAxisX(matrix);
    float32x4_t yAxis = MatrixRowMajor4x3fGetAxisY(matrix);
    float32x4_t zAxis = MatrixRowMajor4x3fGetAxisZ(matrix);

    float32x4_t vM00 = vdupq_n_f32(Vector4fGetX(xAxis));
    float32x4_t vM11 = vdupq_n_f32(Vector4fGetY(yAxis));
    float32x4_t vM22 = vdupq_n_f32(Vector4fGetZ(zAxis));

    float32x4_t v4xx4yy4zz4ww = vmulq_f32(Sign0, vM00);
    v4xx4yy4zz4ww = Vector4fMultiplyAdd(v4xx4yy4zz4ww, Sign1, vM11);
    v4xx4yy4zz4ww = Vector4fMultiplyAdd(v4xx4yy4zz4ww, Sign2, vM22);
    v4xx4yy4zz4ww = vaddq_f32(v4xx4yy4zz4ww, Vector4fValueOne);

    float32x4_t m01m20m12 = Vector4fSet(
        Vector4fGetY(xAxis),
        Vector4fGetX(zAxis),
        Vector4fGetZ(yAxis),
        0.f
        );
    float32x4_t m10m02m21 = Vector4fSet(
        Vector4fGetX(yAxis),
        Vector4fGetZ(xAxis),
        Vector4fGetY(zAxis),
        0.f
        );

    float32x4_t v4xy4xz4yz = vaddq_f32(m01m20m12, m10m02m21);
    float32x4_t v4wz4wy4wx = vsubq_f32(m01m20m12, m10m02m21);

    float32x4_t v4xx4xy4xz4xw = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 0, 0, 1, 6),
        Vector4uValueSelect1000
        );

    float32x4_t v4xy4yy4yz4yw = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 0, 0, 2, 5),
        Vector4uValueSelect0100
        );

    float32x4_t v4xz4yz4zz4zw = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 1, 2, 0, 4),
        Vector4uValueSelect0010
        );

    float32x4_t v4xw4yw4zw4ww = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 6, 5, 4, 0),
        Vector4uValueSelect0001
        );

    float32x4_t v4xx = vdupq_n_f32(Vector4fGetX(v4xx4yy4zz4ww));
    float32x4_t v4yy = vdupq_n_f32(Vector4fGetY(v4xx4yy4zz4ww));
    float32x4_t v4zz = vdupq_n_f32(Vector4fGetZ(v4xx4yy4zz4ww));
    float32x4_t v4ww = vdupq_n_f32(Vector4fGetW(v4xx4yy4zz4ww));

    float32x4_t vBiggest = v4xx;
    float32x4_t vResult = v4xx4xy4xz4xw;

    uint32x4_t vYGreaterEqualBiggest = vcgeq_f32(v4yy, vBiggest);
    vBiggest = vbslq_f32(vYGreaterEqualBiggest, v4yy, vBiggest);
    vResult = vbslq_f32(vYGreaterEqualBiggest, v4xy4yy4yz4yw, vResult);

    uint32x4_t vZGreaterEqualBiggest = vcgeq_f32(v4zz, vBiggest);
    vBiggest = vbslq_f32(vZGreaterEqualBiggest, v4zz, vBiggest);
    vResult = vbslq_f32(vZGreaterEqualBiggest, v4xz4yz4zz4zw, vResult);

    uint32x4_t vWGreaterEqualBiggest = vcgeq_f32(v4ww, vBiggest);
    vBiggest = vbslq_f32(vWGreaterEqualBiggest, v4ww, vBiggest);
    vResult = vbslq_f32(vWGreaterEqualBiggest, v4xw4yw4zw4ww, vResult);

    Vector4fNormalize(&vResult, vResult);

    return vResult;
}

NN_FORCEINLINE float32x4_t QuaternionFromMatrixColumnMajor4x3f(float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t Sign0 = { 1.f, -1.f, -1.f, 1.f };
    float32x4_t Sign1 = { -1.f, 1.f, -1.f, 1.f };
    float32x4_t Sign2 = { -1.f, -1.f, 1.f, 1.f };

    float32x4_t xAxis = MatrixColumnMajor4x3fGetAxisX(matrix);
    float32x4_t yAxis = MatrixColumnMajor4x3fGetAxisY(matrix);
    float32x4_t zAxis = MatrixColumnMajor4x3fGetAxisZ(matrix);

    float32x4_t vM00 = vdupq_n_f32(Vector4fGetX(xAxis));
    float32x4_t vM11 = vdupq_n_f32(Vector4fGetY(yAxis));
    float32x4_t vM22 = vdupq_n_f32(Vector4fGetZ(zAxis));

    float32x4_t v4xx4yy4zz4ww = vmulq_f32(Sign0, vM00);
    v4xx4yy4zz4ww = Vector4fMultiplyAdd(v4xx4yy4zz4ww, Sign1, vM11);
    v4xx4yy4zz4ww = Vector4fMultiplyAdd(v4xx4yy4zz4ww, Sign2, vM22);
    v4xx4yy4zz4ww = vaddq_f32(v4xx4yy4zz4ww, Vector4fValueOne);

    float32x4_t m01m20m12 = Vector4fSet(
        Vector4fGetY(xAxis),
        Vector4fGetX(zAxis),
        Vector4fGetZ(yAxis),
        0.f
        );
    float32x4_t m10m02m21 = Vector4fSet(
        Vector4fGetX(yAxis),
        Vector4fGetZ(xAxis),
        Vector4fGetY(zAxis),
        0.f
        );

    float32x4_t v4xy4xz4yz = vaddq_f32(m01m20m12, m10m02m21);
    float32x4_t v4wz4wy4wx = vsubq_f32(m01m20m12, m10m02m21);

    float32x4_t v4xx4xy4xz4xw = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 0, 0, 1, 6),
        Vector4uValueSelect1000
        );

    float32x4_t v4xy4yy4yz4yw = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 0, 0, 2, 5),
        Vector4uValueSelect0100
        );

    float32x4_t v4xz4yz4zz4zw = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 1, 2, 0, 4),
        Vector4uValueSelect0010
        );

    float32x4_t v4xw4yw4zw4ww = Vector4fSelect(
        v4xx4yy4zz4ww,
        Vector4fPermute(v4xy4xz4yz, v4wz4wy4wx, 6, 5, 4, 0),
        Vector4uValueSelect0001
        );

    float32x4_t v4xx = vdupq_n_f32(Vector4fGetX(v4xx4yy4zz4ww));
    float32x4_t v4yy = vdupq_n_f32(Vector4fGetY(v4xx4yy4zz4ww));
    float32x4_t v4zz = vdupq_n_f32(Vector4fGetZ(v4xx4yy4zz4ww));
    float32x4_t v4ww = vdupq_n_f32(Vector4fGetW(v4xx4yy4zz4ww));

    float32x4_t vBiggest = v4xx;
    float32x4_t vResult = v4xx4xy4xz4xw;

    uint32x4_t vYGreaterEqualBiggest = vcgeq_f32(v4yy, vBiggest);
    vBiggest = vbslq_f32(vYGreaterEqualBiggest, v4yy, vBiggest);
    vResult = vbslq_f32(vYGreaterEqualBiggest, v4xy4yy4yz4yw, vResult);

    uint32x4_t vZGreaterEqualBiggest = vcgeq_f32(v4zz, vBiggest);
    vBiggest = vbslq_f32(vZGreaterEqualBiggest, v4zz, vBiggest);
    vResult = vbslq_f32(vZGreaterEqualBiggest, v4xz4yz4zz4zw, vResult);

    uint32x4_t vWGreaterEqualBiggest = vcgeq_f32(v4ww, vBiggest);
    vBiggest = vbslq_f32(vWGreaterEqualBiggest, v4ww, vBiggest);
    vResult = vbslq_f32(vWGreaterEqualBiggest, v4xw4yw4zw4ww, vResult);

    Vector4fNormalize(&vResult, vResult);

    return vResult;
}

NN_FORCEINLINE float32x4_t QuaternionIdentity() NN_NOEXCEPT
{
    return Vector4fValueIdentityAxisW;
}

NN_FORCEINLINE float32x4_t QuaternionAdd(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT
{
    return Vector4fAdd(quaternion1, quaternion2);
}

NN_FORCEINLINE float32x4_t QuaternionSubtract(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT
{
    return Vector4fSubtract(quaternion1, quaternion2);
}

NN_FORCEINLINE float32x4_t QuaternionDivide(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT
{
    float32x4_t vInverse = QuaternionInverse(quaternion2);

    return QuaternionMultiply(vInverse, quaternion1);
}

NN_FORCEINLINE float32x4_t QuaternionMultiply(float32x4_t quaternion, float factor) NN_NOEXCEPT
{
    return Vector4fMultiply(quaternion, factor);
}

NN_FORCEINLINE float32x4_t QuaternionMultiply(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT
{
    float32x4_t q1 = quaternion1;
    float32x4_t q2 = quaternion2;

    float32x4_t result = vmulq_f32(Vector4fSwizzle(q1, 3, 3, 3, 3), q2);
    result = Vector4fMultiplyAdd(result, Vector4fValueOneWNegativeOne, vmulq_f32(Vector4fSwizzle(q1, 0, 1, 2, 0), Vector4fSwizzle(q2, 3, 3, 3, 0)));
    result = Vector4fMultiplyAdd(result, Vector4fValueOneWNegativeOne, vmulq_f32(Vector4fSwizzle(q1, 1, 2, 0, 1), Vector4fSwizzle(q2, 2, 0, 1, 1)));
    result = Vector4fMultiplySubtract(result, Vector4fSwizzle(q1, 2, 0, 1, 2), Vector4fSwizzle(q2, 1, 2, 0, 2));

    return result;
}

NN_FORCEINLINE float32x4_t QuaternionDot(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT
{
    return Vector4fDot(quaternion1, quaternion2);
}

NN_FORCEINLINE float32x4_t QuaternionConjugate(float32x4_t quaternion) NN_NOEXCEPT
{
    return vmulq_f32(Vector4fValueNegativeOneWOne, quaternion);
}

NN_FORCEINLINE float32x4_t QuaternionInverse(float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t vConjugate = QuaternionConjugate(quaternion);
    float32x4_t vMagnitude = Vector4fLengthSquared(quaternion);
    float32x4_t vReciprocalMagnitude = Vector4fRecp(vMagnitude);
    float32x4_t vZero = Vector4fZero();
    uint32x4_t vComparedToZero = vceqq_f32(vMagnitude, vZero);

    float32x4_t vInverse = vmulq_f32(vConjugate, vReciprocalMagnitude);

    return vbslq_f32(vComparedToZero, vConjugate, vInverse);
}

NN_FORCEINLINE float32x4_t QuaternionNormalize(float32x4_t* pOutValue, float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t vEpsilon = vdupq_n_f32(nn::util::detail::FloatQuaternionEpsilon);
    float32x4_t vLengthSquared = Vector4fLengthSquared(quaternion);
    float32x4_t vReciprocalLength = Vector4fRsqrt(vLengthSquared);
    float32x4_t vNormalized = vmulq_f32(quaternion, vReciprocalLength);
    float32x4_t vZero = Vector4fZero();
    uint32x4_t vLessThanEpsilon = vcltq_f32(vLengthSquared, vEpsilon);

    *pOutValue = vbslq_f32(vLessThanEpsilon, vZero, vNormalized);

    return vbslq_f32(vLessThanEpsilon, vZero, vLengthSquared);
}

NN_FORCEINLINE float32x4_t QuaternionExp(float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t v3 = Vector4fMask(quaternion, Vector4uValueMaskXYZ);
    float theta = Vector4fGetX(Vector4fLength(v3));

    float sinTheta, cosTheta;
    nn::util::SinCosEst(&sinTheta, &cosTheta, theta);

    float scale = (theta > nn::util::detail::FloatQuaternionEpsilon) ? sinTheta / theta : 1.f;

    float32x4_t vResult = Vector4fMultiply(v3, scale);
    vResult = Vector4fSetW(vResult, cosTheta);

    return vResult;
}

NN_FORCEINLINE float32x4_t QuaternionLogN(float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t v3 = Vector4fMask(quaternion, Vector4uValueMaskXYZ);
    float theta = Vector4fGetX(Vector4fLength(v3));

    float atan2Theta = nn::util::Atan2Est(theta, Vector4fGetW(quaternion));

    float scale = (theta > 0.f) ? atan2Theta / theta : theta;

    float32x4_t vResult = Vector4fMultiply(v3, scale);

    return vResult;
}

NN_FORCEINLINE float32x4_t QuaternionLerp(float32x4_t from, float32x4_t to, float t) NN_NOEXCEPT
{
    return Vector4fLerp(from, to, t);
}

NN_FORCEINLINE float32x4_t QuaternionSlerp(float32x4_t from, float32x4_t to, float t) NN_NOEXCEPT
{
    float cosTheta = Vector4fGetX(QuaternionDot(from, to));
    float cosThetaAbs = cosTheta >= 0.f ? cosTheta : -cosTheta;
    float cosThetaSign = cosTheta >= 0.f ? 1.f : -1.f;

    float theta = nn::util::AcosEst(cosThetaAbs);
    float sinTheta = nn::util::SinEst(theta);
    float oneMinusT = 1.f - t;

    float32x2_t vTmp = Vector2fSet(oneMinusT, t);
    vTmp = vmul_n_f32(vTmp, theta);
    vTmp = Vector2fSin(vTmp);
    float sinOneMinusTMultipliedByTheta = vTmp[0];
    float sinTMultipliedByTheta = vTmp[1];

    float reciprocalSinTheta = 1 / sinTheta;
    float tp = sinOneMinusTMultipliedByTheta * reciprocalSinTheta;
    float tq = cosThetaSign * sinTMultipliedByTheta * reciprocalSinTheta;

    float oneMinusEpsilon = 1.f - nn::util::detail::FloatQuaternionEpsilon;
    bool greaterThanOneMinusEpsilon = cosThetaAbs > oneMinusEpsilon;

    tp = greaterThanOneMinusEpsilon ? oneMinusT : tp;
    tq = greaterThanOneMinusEpsilon ? cosThetaSign * t : tq;

    float32x4_t vResult = Vector4fMultiply(from, tp);
    vResult = Vector4fMultiplyAdd(vResult, to, tq);

    return vResult;
}

NN_FORCEINLINE float32x4_t QuaternionSquad(float32x4_t p, float32x4_t a, float32x4_t b, float32x4_t q, float t) NN_NOEXCEPT
{
    float t2 = 2 * t * (1.f - t);

    float32x4_t vPq = QuaternionSlerp(p, q, t);
    float32x4_t vAb = QuaternionSlerp(a, b, t);

    return QuaternionSlerp(vPq, vAb, t2);
}

NN_FORCEINLINE float32x4_t QuaternionMakeClosest(float32x4_t quaternion, float32x4_t quaternionTo) NN_NOEXCEPT
{
    float32x4_t vDot = QuaternionDot(quaternion, quaternionTo);
    float32x4_t vDotSign = Vector4fSign(vDot);

    return vmulq_f32(vDotSign, quaternion);
}

NN_FORCEINLINE float32x4_t QuaternionRotateAxis(float32x4_t axis, float radian) NN_NOEXCEPT
{
    float32x4_t vAxisNormalized;
    Vector4fNormalize(&vAxisNormalized, axis);

    float halfRadian = radian * 0.5f;
    float halfSin, halfCos;
    nn::util::SinCosEst(&halfSin, &halfCos, halfRadian);

    float32x4_t vResult = vmulq_n_f32(vAxisNormalized, halfSin);
    vResult = Vector4fSetW(vResult, halfCos);

    return vResult;
}

NN_FORCEINLINE float32x4_t QuaternionMakeVectorRotation(float32x4_t from, float32x4_t to) NN_NOEXCEPT
{
    float32x4_t vCross = Vector3fCross(from, to);
    float32x4_t vDot = Vector4fDot(from, to);
    float32x4_t vDotPlusOne = vaddq_f32(vDot, vdupq_n_f32(1.f));

    float32x4_t vS = Vector4fSqrt(vaddq_f32(vDotPlusOne, vDotPlusOne));

    float32x4_t vRotation = Vector4fDivide(vCross, vS);
    vRotation = Vector4fSelect(vRotation, vmulq_n_f32(vS, 0.5f), Vector4uValueSelect1110);

    float32x4_t vFloatUlp = vdupq_n_f32(nn::util::detail::FloatUlp);
    uint32x4_t vLessEqualFloatUlp = vcleq_f32(vDotPlusOne, vFloatUlp);

    return vbslq_f32(vLessEqualFloatUlp, Vector4fValueIdentityAxisX, vRotation);
}

}}}} // namespace nn::util::neon::detail
