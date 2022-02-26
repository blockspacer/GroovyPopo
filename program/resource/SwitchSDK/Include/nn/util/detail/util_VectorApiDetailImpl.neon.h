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
#include <nn/util/detail/util_ConstantImpl.h>
#include <nn/util/detail/util_QuaternionApiDetail.neon.h>
#include <nn/util/detail/util_VectorApiDetail.neon.h>
#include <cmath>
#include <arm_neon.h>

namespace nn { namespace util { namespace neon { namespace detail {

NN_FORCEINLINE float32x4_t Vector4fLoad(const Uint8x4& source) NN_NOEXCEPT
{
    uint32x2_t vLow = vcreate_u32(
        static_cast<uint64_t>(source.v[0]) |
        static_cast<uint64_t>(source.v[1]) << 32
        );
    uint32x2_t vHigh = vcreate_u32(
        static_cast<uint64_t>(source.v[2]) |
        static_cast<uint64_t>(source.v[3]) << 32
        );

    return vcombine_f32(vcvt_f32_u32(vLow), vcvt_f32_u32(vHigh));
}

NN_FORCEINLINE float32x4_t Vector4fLoad(const Unorm8x4& source) NN_NOEXCEPT
{
    uint32x2_t vLow = vcreate_u32(
        static_cast<uint64_t>(source.v[0]) |
        static_cast<uint64_t>(source.v[1]) << 32
        );
    uint32x2_t vHigh = vcreate_u32(
        static_cast<uint64_t>(source.v[2]) |
        static_cast<uint64_t>(source.v[3]) << 32
        );

    float32x4_t v = vcombine_f32(vcvt_f32_u32(vLow), vcvt_f32_u32(vHigh));
    float32x4_t vInvMax = vdupq_n_f32(1.0f / 255.0f);

    return vmulq_f32(v, vInvMax);
}

NN_FORCEINLINE float32x4_t Vector4fLoad(const float* pSource) NN_NOEXCEPT
{
    return vld1q_f32(pSource);
}

NN_FORCEINLINE float32x4_t Vector4fLoad(const Float4& source) NN_NOEXCEPT
{
    return Vector4fLoad(source.v);
}

NN_FORCEINLINE void Vector4fStore(Uint8x4* pOutValue, float32x4_t source) NN_NOEXCEPT
{
    float32x4_t v = vminq_f32(source, Vector4fValueR8G8B8A8);
    v = vmaxq_f32(v, Vector4fZero());
    v = vaddq_f32(v, Vector4fValue0Point5);

    pOutValue->v[0] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 0)));
    pOutValue->v[1] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 1)));
    pOutValue->v[2] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 2)));
    pOutValue->v[3] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 3)));
}

NN_FORCEINLINE void Vector4fStore(Unorm8x4* pOutValue, float32x4_t source) NN_NOEXCEPT
{
    float32x4_t v = vminq_f32(source, Vector4fValueOne);
    v = vmaxq_f32(v, Vector4fZero());
    v = Vector4fMultiplyAdd(Vector4fValue0Point5, v, Vector4fValueR8G8B8A8);

    pOutValue->v[0] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 0)));
    pOutValue->v[1] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 1)));
    pOutValue->v[2] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 2)));
    pOutValue->v[3] = static_cast<uint8_t>(::std::floor(vgetq_lane_f32(v, 3)));
}

NN_FORCEINLINE void Vector4fStore(float32_t* pOutValue, float32x4_t source) NN_NOEXCEPT
{
    vst1q_f32(pOutValue, source);
}

NN_FORCEINLINE void Vector4fStore(Float4* pOutValue, float32x4_t source) NN_NOEXCEPT
{
    Vector4fStore(pOutValue->v, source);
}

NN_FORCEINLINE float32x4_t Vector4fSet(float32_t x, float32_t y, float32_t z, float32_t w) NN_NOEXCEPT
{
    float32x2_t vLow  = vcreate_f32(
            static_cast<uint64_t>(*reinterpret_cast<uint32_t*>(&x)) |
            static_cast<uint64_t>(*reinterpret_cast<uint32_t*>(&y)) << 32
            );
    float32x2_t vHigh  = vcreate_f32(
            static_cast<uint64_t>(*reinterpret_cast<uint32_t*>(&z)) |
            static_cast<uint64_t>(*reinterpret_cast<uint32_t*>(&w)) << 32
            );

    return vcombine_f32(vLow, vHigh);
}

NN_FORCEINLINE float32x4_t Vector4fSetElement(float32x4_t vector, int index, float32_t value) NN_NOEXCEPT
{
    vector[index] = value;
    return vector;
}

NN_FORCEINLINE float32x4_t Vector4fSetX(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    return vsetq_lane_f32(value, vector, 0);
}

NN_FORCEINLINE float32x4_t Vector4fSetY(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    return vsetq_lane_f32(value, vector, 1);
}

NN_FORCEINLINE float32x4_t Vector4fSetZ(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    return vsetq_lane_f32(value, vector, 2);
}

NN_FORCEINLINE float32x4_t Vector4fSetW(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    return vsetq_lane_f32(value, vector, 3);
}

NN_FORCEINLINE float32_t Vector4fGetElement(float32x4_t vector, int index) NN_NOEXCEPT
{
    return vector[index];
}

NN_FORCEINLINE float32_t Vector4fGetX(float32x4_t vector) NN_NOEXCEPT
{
    return vgetq_lane_f32(vector, 0);
}

NN_FORCEINLINE float32_t Vector4fGetY(float32x4_t vector) NN_NOEXCEPT
{
    return vgetq_lane_f32(vector, 1);
}

NN_FORCEINLINE float32_t Vector4fGetZ(float32x4_t vector) NN_NOEXCEPT
{
    return vgetq_lane_f32(vector, 2);
}

NN_FORCEINLINE float32_t Vector4fGetW(float32x4_t vector) NN_NOEXCEPT
{
    return vgetq_lane_f32(vector, 3);
}

NN_FORCEINLINE float32x4_t Vector4fZero() NN_NOEXCEPT
{
    return vdupq_n_f32(0);
}

NN_FORCEINLINE bool Vector4fEqual(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    uint32x4_t vResult = vceqq_f32(vector1, vector2);
    uint32x2_t vHighlow = vand_u32(vget_high_u32(vResult), vget_low_u32(vResult));

    return (vget_lane_u32(vHighlow, 0) & vget_lane_u32(vHighlow, 1));
}

NN_FORCEINLINE bool Vector4fIsZero(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fEqual(vector, Vector4fZero());
}

NN_FORCEINLINE bool Vector4fIsZero(float32x4_t vector, float32_t error) NN_NOEXCEPT
{
    float32x4_t vError = vdupq_n_f32(error);
    uint32x4_t vResult = vcaleq_f32(vector, vError);
    uint32x2_t vHighlow = vand_u32(vget_high_u32(vResult), vget_low_u32(vResult));

    return (vget_lane_u32(vHighlow, 0) & vget_lane_u32(vHighlow, 1));
}

NN_FORCEINLINE bool Vector4fIsZeroWOne(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fEqual(vector, Vector4fValueIdentityAxisW);
}

NN_FORCEINLINE float32x4_t Vector4fAdd(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return vaddq_f32(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector4fSubtract(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return vsubq_f32(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector4fMultiply(float32x4_t vector, float32_t factor) NN_NOEXCEPT
{
    return vmulq_n_f32(vector, factor);
}

NN_FORCEINLINE float32x4_t Vector4fMultiply(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return vmulq_f32(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector4fMultiplyAdd(float32x4_t vector1, float32x4_t vector2, float32x4_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmaq_f32(vector1, vector2, vector3);
#else
    return vmlaq_f32(vector1, vector2, vector3);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplyAdd(float32x4_t vector1, float32x4_t vector2, float factor) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmaq_n_f32(vector1, vector2, factor);
#else
    return vmlaq_n_f32(vector1, vector2, factor);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplyAddLane0(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmaq_lane_f32(vector1, vector2, vector3, 0);
#else
    return vmlaq_lane_f32(vector1, vector2, vector3, 0);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplyAddLane1(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmaq_lane_f32(vector1, vector2, vector3, 1);
#else
    return vmlaq_lane_f32(vector1, vector2, vector3, 1);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplySubtract(float32x4_t vector1, float32x4_t vector2, float32x4_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmsq_f32(vector1, vector2, vector3);
#else
    return vmlsq_f32(vector1, vector2, vector3);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplySubtract(float32x4_t vector1, float32x4_t vector2, float factor) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmsq_n_f32(vector1, vector2, factor);
#else
    return vmlsq_n_f32(vector1, vector2, factor);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplySubtractLane0(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmsq_lane_f32(vector1, vector2, vector3, 0);
#else
    return vmlsq_lane_f32(vector1, vector2, vector3, 0);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fMultiplySubtractLane1(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfmsq_lane_f32(vector1, vector2, vector3, 1);
#else
    return vmlsq_lane_f32(vector1, vector2, vector3, 1);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fDivide(float32x4_t vector1, float32_t factor) NN_NOEXCEPT
{
    return vmulq_n_f32(vector1, 1.0f / factor);
}

NN_FORCEINLINE float32x4_t Vector4fDivide(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vdivq_f32(vector1, vector2);
#else
    return vmulq_f32(vector1, Vector4fRecp(vector2));
#endif
}

NN_FORCEINLINE float32x4_t Vector4fDot(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    float32x4_t vMultipled = vmulq_f32(vector1, vector2);
    float32x2_t vHighLowAdded = vadd_f32(vget_high_f32(vMultipled), vget_low_f32(vMultipled));
    float32x2_t vDot = vpadd_f32(vHighLowAdded, vHighLowAdded);

    return vcombine_f32(vDot, vDot);
}

NN_FORCEINLINE float32x4_t Vector4fLength(float32x4_t vector) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    float32x4_t vLengthSquared = Vector4fLengthSquared(vector);
    return Vector4fSqrt(vLengthSquared);
#else
    float32x4_t vLengthSquared = Vector4fLengthSquared(vector);
    float32x4_t vReciprocalLength = Vector4fRsqrt(vLengthSquared);
    float32x4_t vLength = vmulq_f32(vLengthSquared, vReciprocalLength);
    float32x4_t vZero = Vector4fZero();
    uint32x4_t vComparedToZero = vceqq_f32(vLengthSquared, vZero);

    return vbslq_f32(vComparedToZero, vZero, vLength);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fLengthSquared(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fDot(vector, vector);
}

NN_FORCEINLINE float32x4_t Vector4fDistance(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    float32x4_t vDistanceSquared = Vector4fDistanceSquared(vector1, vector2);
    return Vector4fSqrt(vDistanceSquared);
#else
    float32x4_t vDistanceSquared = Vector4fDistanceSquared(vector1, vector2);
    float32x4_t vReciprocalLength = Vector4fRsqrt(vDistanceSquared);
    float32x4_t vDistance = vmulq_f32(vDistanceSquared, vReciprocalLength);
    float32x4_t vZero = Vector4fZero();
    uint32x4_t vComparedToZero = vceqq_f32(vDistanceSquared, vZero);

    return vbslq_f32(vComparedToZero, vZero, vDistance);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fDistanceSquared(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    float32x4_t vSubtracted = vsubq_f32(vector1, vector2);
    float32x4_t vMultipled = vmulq_f32(vSubtracted, vSubtracted);
    float32x2_t vHighLowAdded = vadd_f32(vget_high_f32(vMultipled), vget_low_f32(vMultipled));
    float32x2_t vDistanceSquared = vpadd_f32(vHighLowAdded, vHighLowAdded);

    return vcombine_f32(vDistanceSquared, vDistanceSquared);
}

NN_FORCEINLINE float32x4_t Vector4fNormalize(float32x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vLengthSquared = Vector4fLengthSquared(vector);
    float32x4_t vReciprocalLength = Vector4fRsqrt(vLengthSquared);
    float32x4_t vNormalized = vmulq_f32(vector, vReciprocalLength);
    float32x4_t vZero = Vector4fZero();
    uint32x4_t vComparedToZero = vceqq_f32(vLengthSquared, vZero);

    *pOutValue = vbslq_f32(vComparedToZero, vZero, vNormalized);

    return vLengthSquared;
}

NN_FORCEINLINE float32x4_t Vector4fMaximize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return vmaxq_f32(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector4fMinimize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return vminq_f32(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector4fLerp(float32x4_t from, float32x4_t to, float32_t t) NN_NOEXCEPT
{
    float32x4_t vT = vdupq_n_f32(t);
    float32x4_t vDirection = vsubq_f32(to, from);

    return Vector4fMultiplyAdd(from, vT, vDirection);
}

NN_FORCEINLINE float32x4_t Vector4fNegate(float32x4_t vector) NN_NOEXCEPT
{
    return vnegq_f32(vector);
}

NN_FORCEINLINE float32x4_t Vector4fMask(float32x4_t vector, uint32x4_t mask) NN_NOEXCEPT
{
    return vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(vector), mask));
}

NN_FORCEINLINE float32x4_t Vector4fSqrt(float32x4_t vector) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vsqrtq_f32(vector);
#else
    float32x4_t vRsqrt = Vector4fRsqrt(vector);
    float32x4_t vZero = Vector4fZero();
    uint32x4_t vEqualZero = vceqq_f32(vector, vZero);

    float32x4_t vSqrt = Vector4fRecp(vRsqrt);
    vSqrt = vbslq_f32(vEqualZero, vZero, vSqrt);

    return vSqrt;
#endif
}

NN_FORCEINLINE float32x4_t Vector4fRecp(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vRecp = vrecpeq_f32(vector);
    vRecp = vmulq_f32(vRecp, vrecpsq_f32(vRecp, vector));
    vRecp = vmulq_f32(vRecp, vrecpsq_f32(vRecp, vector));

    return vRecp;
}

NN_FORCEINLINE float32x4_t Vector4fRsqrt(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vRsqrt = vrsqrteq_f32(vector);
    vRsqrt = vmulq_f32(vRsqrt, vrsqrtsq_f32(vRsqrt, vmulq_f32(vector, vRsqrt)));
    vRsqrt = vmulq_f32(vRsqrt, vrsqrtsq_f32(vRsqrt, vmulq_f32(vector, vRsqrt)));

    return vRsqrt;
}

NN_FORCEINLINE float32x4_t Vector4fAbs(float32x4_t vector) NN_NOEXCEPT
{
    return vabsq_f32(vector);
}

NN_FORCEINLINE float32x4_t Vector4fSign(float32x4_t vector) NN_NOEXCEPT
{
    uint32x4_t vLessThanZero = vcltq_f32(vector, Vector4fZero());

    return vbslq_f32(vLessThanZero, Vector4fValueNegativeOne, Vector4fValueOne);
}

NN_FORCEINLINE float32x4_t Vector4fSelect(float32x4_t vector1, float32x4_t vector2, uint32x4_t select) NN_NOEXCEPT
{
    return vbslq_f32(select, vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector4fSwizzle(float32x4_t vector, uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW) NN_NOEXCEPT
{
    uint8x8x2_t vTable;
    vTable.val[0] = vreinterpret_u8_f32(vget_low_f32(vector));
    vTable.val[1] = vreinterpret_u8_f32(vget_high_f32(vector));

    const uint32_t SwizzleValueSelector0X = (0 | 1 << 8 | 2 << 16 | 3 << 24);
    const uint32_t SwizzleValueSelector0Y = (4 | 5 << 8 | 6 << 16 | 7 << 24);
    const uint32_t SwizzleValueSelector0Z = (8 | 9 << 8 | 10 << 16 | 11 << 24);
    const uint32_t SwizzleValueSelector0W = (12 | 13 << 8 | 14 << 16 | 15 << 24);

    const uint32_t map[4] =
    {
        SwizzleValueSelector0X,
        SwizzleValueSelector0Y,
        SwizzleValueSelector0Z,
        SwizzleValueSelector0W,
    };

    uint8x8_t vLowIndex = vcreate_u8(
            static_cast<uint64_t>(map[indexX]) |
            static_cast<uint64_t>(map[indexY]) << 32
            );
    uint8x8_t vLow = vtbl2_u8(vTable, vLowIndex);

    uint8x8_t vHighIndex = vcreate_u8(
            static_cast<uint64_t>(map[indexZ]) |
            static_cast<uint64_t>(map[indexW]) << 32
            );
    uint8x8_t vHigh = vtbl2_u8(vTable, vHighIndex);

    return vcombine_f32(
            vreinterpret_f32_u8(vLow),
            vreinterpret_f32_u8(vHigh)
            );
}

NN_FORCEINLINE float32x4_t Vector4fPermute(float32x4_t vector1, float32x4_t vector2, uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW) NN_NOEXCEPT
{
    uint8x8x4_t vTable;
    vTable.val[0] = vreinterpret_u8_f32(vget_low_f32(vector1));
    vTable.val[1] = vreinterpret_u8_f32(vget_high_f32(vector1));
    vTable.val[2] = vreinterpret_u8_f32(vget_low_f32(vector2));
    vTable.val[3] = vreinterpret_u8_f32(vget_high_f32(vector2));

    const uint32_t SwizzleValueSelector0X = (0 | 1 << 8 | 2 << 16 | 3 << 24);
    const uint32_t SwizzleValueSelector0Y = (4 | 5 << 8 | 6 << 16 | 7 << 24);
    const uint32_t SwizzleValueSelector0Z = (8 | 9 << 8 | 10 << 16 | 11 << 24);
    const uint32_t SwizzleValueSelector0W = (12 | 13 << 8 | 14 << 16 | 15 << 24);

    const uint32_t SwizzleValueSelector1X = (16 | 17 << 8 | 18 << 16 | 19 << 24);
    const uint32_t SwizzleValueSelector1Y = (20 | 21 << 8 | 22 << 16 | 23 << 24);
    const uint32_t SwizzleValueSelector1Z = (24 | 25 << 8 | 26 << 16 | 27 << 24);
    const uint32_t SwizzleValueSelector1W = (28 | 29 << 8 | 30 << 16 | 31 << 24);

    const uint32_t map[8] =
    {
        SwizzleValueSelector0X,
        SwizzleValueSelector0Y,
        SwizzleValueSelector0Z,
        SwizzleValueSelector0W,
        SwizzleValueSelector1X,
        SwizzleValueSelector1Y,
        SwizzleValueSelector1Z,
        SwizzleValueSelector1W,
    };

    uint8x8_t vLowIndex = vcreate_u8(
            static_cast<uint64_t>(map[indexX]) |
            static_cast<uint64_t>(map[indexY]) << 32
            );
    uint8x8_t vLow = vtbl4_u8(vTable, vLowIndex);

    uint8x8_t vHighIndex = vcreate_u8(
            static_cast<uint64_t>(map[indexZ]) |
            static_cast<uint64_t>(map[indexW]) << 32
            );
    uint8x8_t vHigh = vtbl4_u8(vTable, vHighIndex);

    return vcombine_f32(
            vreinterpret_f32_u8(vLow),
            vreinterpret_f32_u8(vHigh)
            );
}

NN_FORCEINLINE float32x4_t Vector4fRound(float32x4_t vector) NN_NOEXCEPT
{
#if false
    return vrndnq_f32(vector);
#else
    uint32x4_t vGreaterEqualZero = vcgeq_f32(vector, Vector4fZero());
    float32x4_t vFactor = vbslq_f32(vGreaterEqualZero, vdupq_n_f32(0.5f), vdupq_n_f32(-0.5f));
    float32x4_t vTmp = vaddq_f32(vector, vFactor);
    int32x4_t vTmp2 = vcvtq_s32_f32(vTmp);

    return vcvtq_f32_s32(vTmp2);
#endif
}

NN_FORCEINLINE float32x4_t Vector4fModPi(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vVectorDivided2Pi = vmulq_f32(vector, vdupq_n_f32(nn::util::detail::Float1Divided2Pi));
    float32x4_t vRoundedQuotient = Vector4fRound(vVectorDivided2Pi);
    float32x4_t vFloat2Pi = vdupq_n_f32(nn::util::detail::Float2Pi);

    return Vector4fMultiplySubtract(vector, vRoundedQuotient, vFloat2Pi);
}

NN_FORCEINLINE float32x4_t Vector4fSin(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t v = Vector4fModPi(vector);

    float32x4_t vPositiveFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);
    uint32x4_t vGreaterThanPositiveFloatPiDivided2 = vcgtq_f32(v, vPositiveFloatPiDivided2);
    float32x4_t vTmp1 = vsubq_f32(vdupq_n_f32(nn::util::detail::FloatPi), v);

    v = vbslq_f32(vGreaterThanPositiveFloatPiDivided2, vTmp1, v);

    float32x4_t vNegativeFloatPiDivided2 = vdupq_n_f32(-nn::util::detail::FloatPiDivided2);
    uint32x4_t vLessThanNegativeFloatPiDivided2 = vcltq_f32(v, vNegativeFloatPiDivided2);
    float32x4_t vTmp2 = vsubq_f32(vdupq_n_f32(-nn::util::detail::FloatPi), v);

    v = vbslq_f32(vLessThanNegativeFloatPiDivided2, vTmp2, v);

    float32x4_t vv = vmulq_f32(v, v);

    float32x4_t vResult = vdupq_n_f32(-nn::util::detail::SinCoefficients[0]);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::SinCoefficients[1]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::SinCoefficients[2]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::SinCoefficients[3]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::SinCoefficients[4]), vResult, vv);
    vResult = Vector4fMultiplyAdd(Vector4fValueOne, vResult, vv);
    vResult = vmulq_f32(vResult, v);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fCos(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t v = Vector4fModPi(vector);
    float32x4_t vSign = Vector4fValueOne;
    float32x4_t vNegativeSign = Vector4fValueNegativeOne;

    float32x4_t vPositiveFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);
    uint32x4_t vGreaterThanPositiveFloatPiDivided2 = vcgtq_f32(v, vPositiveFloatPiDivided2);
    float32x4_t vTmp1 = vsubq_f32(vdupq_n_f32(nn::util::detail::FloatPi), v);

    v = vbslq_f32(vGreaterThanPositiveFloatPiDivided2, vTmp1, v);
    vSign = vbslq_f32(vGreaterThanPositiveFloatPiDivided2, vNegativeSign, vSign);

    float32x4_t vNegativeFloatPiDivided2 = vdupq_n_f32(-nn::util::detail::FloatPiDivided2);
    uint32x4_t vLessThanNegativeFloatPiDivided2 = vcltq_f32(v, vNegativeFloatPiDivided2);
    float32x4_t vTmp2 = vsubq_f32(vdupq_n_f32(-nn::util::detail::FloatPi), v);

    v = vbslq_f32(vLessThanNegativeFloatPiDivided2, vTmp2, v);
    vSign = vbslq_f32(vLessThanNegativeFloatPiDivided2, vNegativeSign, vSign);

    float32x4_t vv = vmulq_f32(v, v);

    float32x4_t vResult = vdupq_n_f32(-nn::util::detail::CosCoefficients[0]);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::CosCoefficients[1]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::CosCoefficients[2]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::CosCoefficients[3]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::CosCoefficients[4]), vResult, vv);
    vResult = Vector4fMultiplyAdd(Vector4fValueOne, vResult, vv);
    vResult = vmulq_f32(vSign, vResult);

    return vResult;
}

NN_FORCEINLINE void Vector4fSinCos(float32x4_t* pOutSin, float32x4_t* pOutCos, float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t v = Vector4fModPi(vector);
    float32x4_t vSign = Vector4fValueOne;
    float32x4_t vNegativeSign = Vector4fValueNegativeOne;

    float32x4_t vPositiveFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);
    uint32x4_t vGreaterThanPositiveFloatPiDivided2 = vcgtq_f32(v, vPositiveFloatPiDivided2);
    float32x4_t vTmp1 = vsubq_f32(vdupq_n_f32(nn::util::detail::FloatPi), v);

    v = vbslq_f32(vGreaterThanPositiveFloatPiDivided2, vTmp1, v);
    vSign = vbslq_f32(vGreaterThanPositiveFloatPiDivided2, vNegativeSign, vSign);

    float32x4_t vNegativeFloatPiDivided2 = vdupq_n_f32(-nn::util::detail::FloatPiDivided2);
    uint32x4_t vLessThanNegativeFloatPiDivided2 = vcltq_f32(v, vNegativeFloatPiDivided2);
    float32x4_t vTmp2 = vsubq_f32(vdupq_n_f32(-nn::util::detail::FloatPi), v);

    v = vbslq_f32(vLessThanNegativeFloatPiDivided2, vTmp2, v);
    vSign = vbslq_f32(vLessThanNegativeFloatPiDivided2, vNegativeSign, vSign);

    float32x4_t vv = vmulq_f32(v, v);

    float32x4_t vSin = vdupq_n_f32(-nn::util::detail::SinCoefficients[0]);
    vSin = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::SinCoefficients[1]), vSin, vv);
    vSin = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::SinCoefficients[2]), vSin, vv);
    vSin = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::SinCoefficients[3]), vSin, vv);
    vSin = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::SinCoefficients[4]), vSin, vv);
    vSin = Vector4fMultiplyAdd(Vector4fValueOne, vSin, vv);
    vSin = vmulq_f32(vSin, v);

    *pOutSin = vSin;

    float32x4_t vCos = vdupq_n_f32(-nn::util::detail::CosCoefficients[0]);
    vCos = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::CosCoefficients[1]), vCos, vv);
    vCos = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::CosCoefficients[2]), vCos, vv);
    vCos = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::CosCoefficients[3]), vCos, vv);
    vCos = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::CosCoefficients[4]), vCos, vv);
    vCos = Vector4fMultiplyAdd(Vector4fValueOne, vCos, vv);
    vCos = vmulq_f32(vSign, vCos);

    *pOutCos = vCos;
}

NN_FORCEINLINE float32x4_t Vector4fTan(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vSin, vCos;

    Vector4fSinCos(&vSin, &vCos, vector);

    return Vector4fDivide(vSin, vCos);
}

NN_FORCEINLINE float32x4_t Vector4fAsin(float32x4_t vector) NN_NOEXCEPT
{
    const float FloatSqrt0Point5 = 0.707106781186548f;  // sqrt(0.5)

    float32x4_t vAbs = Vector4fAbs(vector);

    float32x4_t vSqrtOneMinusVv = Vector4fSqrt(Vector4fMultiplySubtract(Vector4fValueOne, vector, vector));

    float32x4_t vSqrtOneMinusVvDividedAbsV = Vector4fDivide(vSqrtOneMinusVv, vAbs);
    float32x4_t vAtanImplSqrtOneMinusVvDividedAbsV = Vector4fAtanImpl(vSqrtOneMinusVvDividedAbsV);
    float32x4_t vFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);
    float32x4_t vTmp1 = vsubq_f32(vFloatPiDivided2, vAtanImplSqrtOneMinusVvDividedAbsV);

    float32x4_t vAbsVDividedSqrtOneMinusVv = Vector4fDivide(vAbs, vSqrtOneMinusVv);
    float32x4_t vTmp2 = Vector4fAtanImpl(vAbsVDividedSqrtOneMinusVv);

    float32x4_t vSign = Vector4fSign(vector);
    float32x4_t vFloatSqrt0Point5 = vdupq_n_f32(FloatSqrt0Point5);
    uint32x4_t vAbsGreaterThanFloatSqrt0Point5 = vcgtq_f32(vAbs, vFloatSqrt0Point5);

    float32x4_t vResult = vbslq_f32(vAbsGreaterThanFloatSqrt0Point5, vTmp1, vTmp2);
    vResult = vmulq_f32(vSign, vResult);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fAcos(float32x4_t vector) NN_NOEXCEPT
{
    const float FloatSqrt0Point5 = 0.707106781186548f;  // sqrt(0.5)

    float32x4_t vAbs = Vector4fAbs(vector);

    float32x4_t vSqrtOneMinusVv = Vector4fSqrt(Vector4fMultiplySubtract(Vector4fValueOne, vector, vector));
    uint32x4_t vGreaterEqualZero = vcgeq_f32(vector, Vector4fZero());

    float32x4_t vSqrtOneMinusVvDividedAbsV = Vector4fDivide(vSqrtOneMinusVv, vAbs);
    float32x4_t vAtanImplSqrtOneMinusVvDividedAbsV = Vector4fAtanImpl(vSqrtOneMinusVvDividedAbsV);
    float32x4_t vFloatPi = vdupq_n_f32(nn::util::detail::FloatPi);
    float32x4_t vTmp1_1 = vAtanImplSqrtOneMinusVvDividedAbsV;
    float32x4_t vTmp1_2 = vsubq_f32(vFloatPi, vAtanImplSqrtOneMinusVvDividedAbsV);
    float32x4_t vTmp1 = vbslq_f32(vGreaterEqualZero, vTmp1_1, vTmp1_2);

    float32x4_t vAbsVDividedSqrtOneMinusVv = Vector4fDivide(vAbs, vSqrtOneMinusVv);
    float32x4_t vAtanImplAbsVDividedSqrtOneMinusVv = Vector4fAtanImpl(vAbsVDividedSqrtOneMinusVv);
    float32x4_t vFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);
    float32x4_t vTmp2_1 = vsubq_f32(vFloatPiDivided2, vAtanImplAbsVDividedSqrtOneMinusVv);
    float32x4_t vTmp2_2 = vaddq_f32(vFloatPiDivided2, vAtanImplAbsVDividedSqrtOneMinusVv);
    float32x4_t vTmp2 = vbslq_f32(vGreaterEqualZero, vTmp2_1, vTmp2_2);

    float32x4_t vFloatSqrt0Point5 = vdupq_n_f32(FloatSqrt0Point5);
    uint32x4_t vAbsGreaterThanFloatSqrt0Point5 = vcgtq_f32(vAbs, vFloatSqrt0Point5);
    float32x4_t vResult = vbslq_f32(vAbsGreaterThanFloatSqrt0Point5, vTmp1, vTmp2);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fAtanImpl(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vv = vmulq_f32(vector, vector);

    float32x4_t vResult = vdupq_n_f32(nn::util::detail::AtanCoefficients[0]);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::AtanCoefficients[1]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::AtanCoefficients[2]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::AtanCoefficients[3]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::AtanCoefficients[4]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::AtanCoefficients[5]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(nn::util::detail::AtanCoefficients[6]), vResult, vv);
    vResult = Vector4fMultiplyAdd(vdupq_n_f32(-nn::util::detail::AtanCoefficients[7]), vResult, vv);
    vResult = Vector4fMultiplyAdd(Vector4fValueOne, vResult, vv);
    vResult = vmulq_f32(vResult, vector);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fAtan(float32x4_t vector) NN_NOEXCEPT
{
    float32x4_t vAbs = Vector4fAbs(vector);
    uint32x4_t vAbsGreaterThanOne = vcgtq_f32(vAbs, Vector4fValueOne);
    float32x4_t vSign = Vector4fSign(vector);
    float32x4_t vFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);

    float32x4_t vTmp1 = Vector4fAtanImpl(vAbs);
    float32x4_t vTmp2 = vsubq_f32(vFloatPiDivided2, Vector4fAtanImpl(Vector4fRecp(vAbs)));

    float32x4_t vResult = vbslq_f32(vAbsGreaterThanOne, vTmp2, vTmp1);
    vResult = vmulq_f32(vSign, vResult);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fAtan2(float32x4_t vectorY, float32x4_t vectorX) NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4_t vYAbs = Vector4fAbs(vectorY);
    float32x4_t vXAbs = Vector4fAbs(vectorX);
    float32x4_t vSign = Vector4fSign(vectorY);
    uint32x4_t vXGreaterEqualZero = vcgeq_f32(vectorX, vZero);

    float32x4_t vAtanImplYDividedX = Vector4fAtanImpl(Vector4fDivide(vYAbs, vXAbs));
    float32x4_t vFloatPi = vdupq_n_f32(nn::util::detail::FloatPi);
    float32x4_t vTmp1_1 = vAtanImplYDividedX;
    float32x4_t vTmp1_2 = vsubq_f32(vFloatPi, vAtanImplYDividedX);
    float32x4_t vTmp1 = vbslq_f32(vXGreaterEqualZero, vTmp1_1, vTmp1_2);

    float32x4_t vAtanImplXDividedY = Vector4fAtanImpl(Vector4fDivide(vXAbs, vYAbs));
    float32x4_t vFloatPiDivided2 = vdupq_n_f32(nn::util::detail::FloatPiDivided2);
    float32x4_t vTmp2_1 = vsubq_f32(vFloatPiDivided2, vAtanImplXDividedY);
    float32x4_t vTmp2_2 = vaddq_f32(vFloatPiDivided2, vAtanImplXDividedY);
    float32x4_t vTmp2 = vbslq_f32(vXGreaterEqualZero, vTmp2_1, vTmp2_2);

    uint32x4_t vXAbsGreaterEqualYAbs = vcgeq_f32(vXAbs, vYAbs);
    uint32x4_t vXEqualZero = vceqq_f32(vectorX, vZero);
    uint32x4_t vYEqualZero = vceqq_f32(vectorY, vZero);
    uint32x4_t vXYEqualZero = vandq_u32(vXEqualZero, vYEqualZero);
    float32x4_t vResult = vbslq_f32(vXAbsGreaterEqualYAbs, vTmp1, vTmp2);
    vResult = vmulq_f32(vSign, vResult);
    vResult = vbslq_f32(vXYEqualZero, vZero, vResult);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fTransformRowMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x2_t vLow = vget_low_f32(vector);
    float32x4_t vResult = vmulq_lane_f32(matrix.val[0], vLow, 0);
    vResult = Vector4fMultiplyAddLane1(vResult, matrix.val[1], vLow);

    float32x2_t vHigh = vget_high_f32(vector);
    vResult = Vector4fMultiplyAddLane0(vResult, matrix.val[2], vHigh);
    vResult = Vector4fMultiplyAddLane1(vResult, matrix.val[3], vHigh);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector4fTransformColumnMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4_t vRow0 = vmulq_f32(vector, matrix.val[0]);
    float32x4_t vRow1 = vmulq_f32(vector, matrix.val[1]);
    float32x2_t vTmp0 = vpadd_f32(vget_low_f32(vRow0), vget_high_f32(vRow0));
    float32x2_t vTmp1 = vpadd_f32(vget_low_f32(vRow1), vget_high_f32(vRow1));
    float32x2_t vLow = vpadd_f32(vTmp0, vTmp1);

    float32x4_t vRow2 = vmulq_f32(vector, matrix.val[2]);
    float32x4_t vRow3 = vmulq_f32(vector, matrix.val[3]);
    float32x2_t vTmp2 = vpadd_f32(vget_low_f32(vRow2), vget_high_f32(vRow2));
    float32x2_t vTmp3 = vpadd_f32(vget_low_f32(vRow3), vget_high_f32(vRow3));
    float32x2_t vHigh = vpadd_f32(vTmp2, vTmp3);

    return vcombine_f32(vLow, vHigh);
}

NN_FORCEINLINE float32x4_t Vector3fLoad(const float* pSource) NN_NOEXCEPT
{
    float32x2_t vLow = vld1_f32(pSource);
    float32x2_t vHigh = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&pSource[2])));

    return vcombine_f32(vLow, vHigh);
}

NN_FORCEINLINE float32x4_t Vector3fLoad(const Float3& source) NN_NOEXCEPT
{
    return Vector3fLoad(source.v);
}

NN_FORCEINLINE void Vector3fStore(float32_t* pOutValue, float32x4_t source) NN_NOEXCEPT
{
    vst1_f32(pOutValue, vget_low_f32(source));
    pOutValue[2] = vgetq_lane_f32(source, 2);
}

NN_FORCEINLINE void Vector3fStore(Float3* pOutValue, float32x4_t source) NN_NOEXCEPT
{
    Vector3fStore(pOutValue->v, source);
}

NN_FORCEINLINE float32x4_t Vector3fSet(float32_t x, float32_t y, float32_t z) NN_NOEXCEPT
{
    return Vector4fSet(x, y, z, 0.f);
}

NN_FORCEINLINE float32x4_t Vector3fSetElement(float32x4_t vector, int index, float32_t value) NN_NOEXCEPT
{
    return Vector4fSetElement(vector, index, value);
}

NN_FORCEINLINE float32x4_t Vector3fSetX(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    return Vector4fSetX(vector, value);
}

NN_FORCEINLINE float32x4_t Vector3fSetY(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    return Vector4fSetY(vector, value);
}

NN_FORCEINLINE float32x4_t Vector3fSetZ(float32x4_t vector, float32_t value) NN_NOEXCEPT
{
    float32x4_t vResult = Vector4fSetZ(vector, value);
    vResult = Vector4fSetW(vResult, 0);

    return vResult;
}

NN_FORCEINLINE float32_t Vector3fGetElement(float32x4_t vector, int index) NN_NOEXCEPT
{
    return Vector4fGetElement(vector, index);
}

NN_FORCEINLINE float32_t Vector3fGetX(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fGetX(vector);
}

NN_FORCEINLINE float32_t Vector3fGetY(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fGetY(vector);
}

NN_FORCEINLINE float32_t Vector3fGetZ(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fGetZ(vector);
}

NN_FORCEINLINE float32x4_t Vector3fZero() NN_NOEXCEPT
{
    return Vector4fZero();
}

NN_FORCEINLINE bool Vector3fEqual(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fEqual(vector1, vector2);
}

NN_FORCEINLINE bool Vector3fIsZero(float32x4_t vector) NN_NOEXCEPT
{
    return Vector3fEqual(vector, Vector3fZero());
}

NN_FORCEINLINE bool Vector3fIsZero(float32x4_t vector, float32_t error) NN_NOEXCEPT
{
    return Vector4fIsZero(vector, error);
}

NN_FORCEINLINE float32x4_t Vector3fAdd(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fAdd(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fSubtract(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fSubtract(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fMultiply(float32x4_t vector, float32_t factor) NN_NOEXCEPT
{
    return Vector4fMultiply(vector, factor);
}

NN_FORCEINLINE float32x4_t Vector3fMultiply(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fMultiply(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fDivide(float32x4_t vector, float32_t factor) NN_NOEXCEPT
{
    float32x4_t vResult = Vector4fDivide(vector, factor);

    return Vector4fMask(vResult, Vector4uValueMaskXYZ);
}

NN_FORCEINLINE float32x4_t Vector3fDivide(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    float32x4_t vResult = Vector4fDivide(vector1, vector2);

    return Vector4fMask(vResult, Vector4uValueMaskXYZ);
}

NN_FORCEINLINE float32x4_t Vector3fDot(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fDot(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fCross(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    float32x4_t v1yzx = Vector4fSwizzle(vector1, 1, 2, 0, 3);
    float32x4_t v2zxy = Vector4fSwizzle(vector2, 2, 0, 1, 3);

    float32x4_t result = vmulq_f32(v1yzx, v2zxy);

    float32x4_t v1zxy = Vector4fSwizzle(vector1, 2, 0, 1, 3);
    float32x4_t v2yzx = Vector4fSwizzle(vector2, 1, 2, 0, 3);

    return Vector4fMultiplySubtract(result, v1zxy, v2yzx);
}

NN_FORCEINLINE float32x4_t Vector3fLength(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fLength(vector);
}

NN_FORCEINLINE float32x4_t Vector3fLengthSquared(float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fDot(vector, vector);
}

NN_FORCEINLINE float32x4_t Vector3fDistance(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fDistance(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fDistanceSquared(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fDistanceSquared(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fNormalize(float32x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    return Vector4fNormalize(pOutValue, vector);
}

NN_FORCEINLINE float32x4_t Vector3fMaximize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fMaximize(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fMinimize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT
{
    return Vector4fMinimize(vector1, vector2);
}

NN_FORCEINLINE float32x4_t Vector3fLerp(float32x4_t from, float32x4_t to, float32_t t) NN_NOEXCEPT
{
    return Vector4fLerp(from, to, t);
}

NN_FORCEINLINE float32x4_t Vector3fRotate(float32x4_t vector, float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t vQuaternionConjugated = QuaternionConjugate(quaternion);
    float32x4_t vResult = QuaternionMultiply(vector, vQuaternionConjugated);
    vResult = QuaternionMultiply(quaternion, vResult);
    vResult = Vector4fMask(vResult, Vector4uValueMaskXYZ);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector3fTransformRowMajor4x3(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x2_t vLow = vget_low_f32(vector);
    float32x4_t vResult = vmulq_lane_f32(matrix.val[0], vLow, 0);
    vResult = Vector4fMultiplyAddLane1(vResult, matrix.val[1], vLow);

    float32x2_t vHigh = vget_high_f32(vector);
    vResult = Vector4fMultiplyAddLane0(vResult, matrix.val[2], vHigh);
    vResult = vaddq_f32(vResult, matrix.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector3fTransformRowMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x2_t vLow = vget_low_f32(vector);
    float32x4_t vResult = vmulq_lane_f32(matrix.val[0], vLow, 0);
    vResult = Vector4fMultiplyAddLane1(vResult, matrix.val[1], vLow);

    float32x2_t vHigh = vget_high_f32(vector);
    vResult = Vector4fMultiplyAddLane0(vResult, matrix.val[2], vHigh);
    vResult = vaddq_f32(vResult, matrix.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector3fTransformColumnMajor4x3(float32x4_t vector, float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t v = vsetq_lane_f32(1.f, vector, 3);

    float32x4_t vRow0 = vmulq_f32(v, matrix.val[0]);
    float32x4_t vRow1 = vmulq_f32(v, matrix.val[1]);
    float32x2_t vTmp0 = vpadd_f32(vget_low_f32(vRow0), vget_high_f32(vRow0));
    float32x2_t vTmp1 = vpadd_f32(vget_low_f32(vRow1), vget_high_f32(vRow1));
    float32x2_t vLow = vpadd_f32(vTmp0, vTmp1);

    float32x4_t vRow2 = vmulq_f32(v, matrix.val[2]);
    float32x2_t vTmp2 = vpadd_f32(vget_low_f32(vRow2), vget_high_f32(vRow2));
    float32x2_t vHigh = vpadd_f32(vTmp2, vdup_n_f32(0));

    return vcombine_f32(vLow, vHigh);
}

NN_FORCEINLINE float32x4_t Vector3fTransformColumnMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4_t v = vsetq_lane_f32(1.f, vector, 3);

    float32x4_t vRow0 = vmulq_f32(v, matrix.val[0]);
    float32x4_t vRow1 = vmulq_f32(v, matrix.val[1]);
    float32x2_t vTmp0 = vpadd_f32(vget_low_f32(vRow0), vget_high_f32(vRow0));
    float32x2_t vTmp1 = vpadd_f32(vget_low_f32(vRow1), vget_high_f32(vRow1));
    float32x2_t vLow = vpadd_f32(vTmp0, vTmp1);

    float32x4_t vRow2 = vmulq_f32(v, matrix.val[2]);
    float32x4_t vRow3 = vmulq_f32(v, matrix.val[3]);
    float32x2_t vTmp2 = vpadd_f32(vget_low_f32(vRow2), vget_high_f32(vRow2));
    float32x2_t vTmp3 = vpadd_f32(vget_low_f32(vRow3), vget_high_f32(vRow3));
    float32x2_t vHigh = vpadd_f32(vTmp2, vTmp3);

    return vcombine_f32(vLow, vHigh);
}

NN_FORCEINLINE float32x4_t Vector3fTransformNormalRowMajor4x3(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x2_t vLow = vget_low_f32(vector);
    float32x4_t vResult = vmulq_lane_f32(matrix.val[0], vLow, 0);
    vResult = Vector4fMultiplyAddLane1(vResult, matrix.val[1], vLow);

    float32x2_t vHigh = vget_high_f32(vector);
    vResult = Vector4fMultiplyAddLane0(vResult, matrix.val[2], vHigh);

    return vResult;
}

NN_FORCEINLINE float32x4_t Vector3fTransformNormalColumnMajor4x3(float32x4_t vector, float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t v = vector;

    float32x4_t vRow0 = vmulq_f32(v, matrix.val[0]);
    float32x4_t vRow1 = vmulq_f32(v, matrix.val[1]);
    float32x2_t vTmp0 = vpadd_f32(vget_low_f32(vRow0), vget_high_f32(vRow0));
    float32x2_t vTmp1 = vpadd_f32(vget_low_f32(vRow1), vget_high_f32(vRow1));
    float32x2_t vLow = vpadd_f32(vTmp0, vTmp1);

    float32x4_t vRow2 = vmulq_f32(v, matrix.val[2]);
    float32x2_t vTmp2 = vpadd_f32(vget_low_f32(vRow2), vget_high_f32(vRow2));
    float32x2_t vHigh = vpadd_f32(vTmp2, vdup_n_f32(0));

    return vcombine_f32(vLow, vHigh);
}

NN_FORCEINLINE float32x4_t Vector3fTransformCoordRowMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x2_t vLow = vget_low_f32(vector);
    float32x4_t vResult = vmulq_lane_f32(matrix.val[0], vLow, 0);
    vResult = Vector4fMultiplyAddLane1(vResult, matrix.val[1], vLow);

    float32x2_t vHigh = vget_high_f32(vector);
    vResult = Vector4fMultiplyAddLane0(vResult, matrix.val[2], vHigh);
    vResult = vaddq_f32(vResult, matrix.val[3]);

    float32x4_t vW = vdupq_n_f32(vgetq_lane_f32(vResult, 3));
    float32x4_t vReciprocalW = Vector4fRecp(vW);
    vResult = vmulq_f32(vResult, vReciprocalW);

    return Vector4fMask(vResult, Vector4uValueMaskXYZ);
}

NN_FORCEINLINE float32x4_t Vector3fTransformCoordColumnMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4_t v = vsetq_lane_f32(1.f, vector, 3);

    float32x4_t vRow0 = vmulq_f32(v, matrix.val[0]);
    float32x4_t vRow1 = vmulq_f32(v, matrix.val[1]);
    float32x2_t vTmp0 = vpadd_f32(vget_low_f32(vRow0), vget_high_f32(vRow0));
    float32x2_t vTmp1 = vpadd_f32(vget_low_f32(vRow1), vget_high_f32(vRow1));
    float32x2_t vLow = vpadd_f32(vTmp0, vTmp1);

    float32x4_t vRow2 = vmulq_f32(v, matrix.val[2]);
    float32x4_t vRow3 = vmulq_f32(v, matrix.val[3]);
    float32x2_t vTmp2 = vpadd_f32(vget_low_f32(vRow2), vget_high_f32(vRow2));
    float32x2_t vTmp3 = vpadd_f32(vget_low_f32(vRow3), vget_high_f32(vRow3));
    float32x2_t vHigh = vpadd_f32(vTmp2, vTmp3);

    float32x4_t vResult = vcombine_f32(vLow, vHigh);

    float32x4_t vW = vdupq_n_f32(vgetq_lane_f32(vResult, 3));
    float32x4_t vReciprocalW = Vector4fRecp(vW);
    vResult = vmulq_f32(vResult, vReciprocalW);

    return Vector4fMask(vResult, Vector4uValueMaskXYZ);
}

NN_FORCEINLINE float32x2_t Vector2fLoad(const float* pSource) NN_NOEXCEPT
{
    return vld1_f32(pSource);
}

NN_FORCEINLINE float32x2_t Vector2fLoad(const Float2& source) NN_NOEXCEPT
{
    return Vector2fLoad(source.v);
}

NN_FORCEINLINE void Vector2fStore(float32_t* pOutValue, float32x2_t source) NN_NOEXCEPT
{
    vst1_f32(pOutValue, source);
}

NN_FORCEINLINE void Vector2fStore(Float2* pOutValue, float32x2_t source) NN_NOEXCEPT
{
    Vector2fStore(pOutValue->v, source);
}

NN_FORCEINLINE float32x2_t Vector2fSet(float32_t x, float32_t y) NN_NOEXCEPT
{
    return vcreate_f32(
        static_cast<uint64_t>(*reinterpret_cast<uint32_t*>(&x)) |
        static_cast<uint64_t>(*reinterpret_cast<uint32_t*>(&y)) << 32
    );
}

NN_FORCEINLINE float32x2_t Vector2fSetElement(float32x2_t vector, int index, float32_t value) NN_NOEXCEPT
{
    vector[index] = value;
    return vector;
}

NN_FORCEINLINE float32x2_t Vector2fSetX(float32x2_t vector, float32_t value) NN_NOEXCEPT
{
    return vset_lane_f32(value, vector, 0);
}

NN_FORCEINLINE float32x2_t Vector2fSetY(float32x2_t vector, float32_t value) NN_NOEXCEPT
{
    return vset_lane_f32(value, vector, 1);
}

NN_FORCEINLINE float32_t Vector2fGetElement(float32x2_t vector, int index) NN_NOEXCEPT
{
    return vector[index];
}

NN_FORCEINLINE float32_t Vector2fGetX(float32x2_t vector) NN_NOEXCEPT
{
    return vget_lane_f32(vector, 0);
}

NN_FORCEINLINE float32_t Vector2fGetY(float32x2_t vector) NN_NOEXCEPT
{
    return vget_lane_f32(vector, 1);
}

NN_FORCEINLINE float32x2_t Vector2fZero() NN_NOEXCEPT
{
    return vdup_n_f32(0);
}

NN_FORCEINLINE bool Vector2fEqual(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    uint32x2_t vResult = vceq_f32(vector1, vector2);

    return (vget_lane_u32(vResult, 0) & vget_lane_u32(vResult, 1));
}

NN_FORCEINLINE bool Vector2fIsZero(float32x2_t vector) NN_NOEXCEPT
{
    return Vector2fEqual(vector, Vector2fZero());
}

NN_FORCEINLINE bool Vector2fIsZero(float32x2_t vector, float32_t error) NN_NOEXCEPT
{
    float32x2_t vError = vdup_n_f32(error);
    uint32x2_t vResult = vcale_f32(vector, vError);

    return (vget_lane_u32(vResult, 0) & vget_lane_u32(vResult, 1));
}

NN_FORCEINLINE float32x2_t Vector2fAdd(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    return vadd_f32(vector1, vector2);
}

NN_FORCEINLINE float32x2_t Vector2fSubtract(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    return vsub_f32(vector1, vector2);
}

NN_FORCEINLINE float32x2_t Vector2fMultiply(float32x2_t vector, float32_t factor) NN_NOEXCEPT
{
    return vmul_n_f32(vector, factor);
}

NN_FORCEINLINE float32x2_t Vector2fMultiply(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    return vmul_f32(vector1, vector2);
}

NN_FORCEINLINE float32x2_t Vector2fMultiplyAdd(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfma_f32(vector1, vector2, vector3);
#else
    return vmla_f32(vector1, vector2, vector3);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplyAdd(float32x2_t vector1, float32x2_t vector2, float factor) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfma_n_f32(vector1, vector2, factor);
#else
    return vmla_n_f32(vector1, vector2, factor);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplyAddLane0(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfma_lane_f32(vector1, vector2, vector3, 0);
#else
    return vmla_lane_f32(vector1, vector2, vector3, 0);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplyAddLane1(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfma_lane_f32(vector1, vector2, vector3, 1);
#else
    return vmla_lane_f32(vector1, vector2, vector3, 1);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplySubtract(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfms_f32(vector1, vector2, vector3);
#else
    return vmls_f32(vector1, vector2, vector3);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplySubtract(float32x2_t vector1, float32x2_t vector2, float factor) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfms_n_f32(vector1, vector2, factor);
#else
    return vmls_n_f32(vector1, vector2, factor);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplySubtractLane0(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfms_lane_f32(vector1, vector2, vector3, 0);
#else
    return vmls_lane_f32(vector1, vector2, vector3, 0);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fMultiplySubtractLane1(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vfms_lane_f32(vector1, vector2, vector3, 1);
#else
    return vmls_lane_f32(vector1, vector2, vector3, 1);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fDivide(float32x2_t vector, float32_t factor) NN_NOEXCEPT
{
    return vmul_n_f32(vector, 1.0f / factor);
}

NN_FORCEINLINE float32x2_t Vector2fDivide(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vdiv_f32(vector1, vector2);
#else
    return vmul_f32(vector1, Vector2fRecp(vector2));
#endif
}

NN_FORCEINLINE float32x2_t Vector2fDot(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    float32x2_t vMultipled = vmul_f32(vector1, vector2);

    return vpadd_f32(vMultipled, vMultipled);
}

NN_FORCEINLINE float32x2_t Vector2fCross(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    float32x2_t v1xy = vector1;
    float32x2_t v2yx = vrev64_f32(vector2);

    const float32x2_t vNegateY = { 1.f, -1.f };

    float32x2_t vTmp0 = vmul_f32(v1xy, v2yx);
    float32x2_t vTmp1 = vmul_f32(vTmp0, vNegateY);

    return vpadd_f32(vTmp1, vTmp1);
}

NN_FORCEINLINE float32x2_t Vector2fLength(float32x2_t vector) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    float32x2_t vLengthSquared = Vector2fLengthSquared(vector);
    return Vector2fSqrt(vLengthSquared);
#else
    float32x2_t vLengthSquared = Vector2fLengthSquared(vector);
    float32x2_t vReciprocalLength = Vector2fRsqrt(vLengthSquared);
    float32x2_t vLength = vmul_f32(vLengthSquared, vReciprocalLength);
    float32x2_t vZero = Vector2fZero();
    uint32x2_t vComparedToZero = vceq_f32(vLengthSquared, vZero);

    return vbsl_f32(vComparedToZero, vZero, vLength);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fLengthSquared(float32x2_t vector) NN_NOEXCEPT
{
    return Vector2fDot(vector, vector);
}

NN_FORCEINLINE float32x2_t Vector2fDistance(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    float32x2_t vDistanceSquared = Vector2fDistanceSquared(vector1, vector2);
    return Vector2fSqrt(vDistanceSquared);
#else
    float32x2_t vDistanceSquared = Vector2fDistanceSquared(vector1, vector2);
    float32x2_t vReciprocalLength = Vector2fRsqrt(vDistanceSquared);
    float32x2_t vDistance = vmul_f32(vDistanceSquared, vReciprocalLength);
    float32x2_t vZero = Vector2fZero();
    uint32x2_t vComparedToZero = vceq_f32(vDistanceSquared, vZero);

    return vbsl_f32(vComparedToZero, vZero, vDistance);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fDistanceSquared(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    float32x2_t vSubtracted = vsub_f32(vector1, vector2);
    float32x2_t vMultipled = vmul_f32(vSubtracted, vSubtracted);

    return vpadd_f32(vMultipled, vMultipled);
}

NN_FORCEINLINE float32x2_t Vector2fNormalize(float32x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t vLengthSquared = Vector2fLengthSquared(vector);
    float32x2_t vReciprocalLength = Vector2fRsqrt(vLengthSquared);
    float32x2_t vNormalized = vmul_f32(vector, vReciprocalLength);
    float32x2_t vZero = Vector2fZero();
    uint32x2_t vComparedToZero = vceq_f32(vLengthSquared, vZero);

    *pOutValue = vbsl_f32(vComparedToZero, vZero, vNormalized);

    return vLengthSquared;
}

NN_FORCEINLINE float32x2_t Vector2fMaximize(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    return vmax_f32(vector1, vector2);
}

NN_FORCEINLINE float32x2_t Vector2fMinimize(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT
{
    return vmin_f32(vector1, vector2);
}

NN_FORCEINLINE float32x2_t Vector2fLerp(float32x2_t from, float32x2_t to, float32_t t) NN_NOEXCEPT
{
    float32x2_t vT = vdup_n_f32(t);
    float32x2_t vDirection = vsub_f32(to, from);

    return Vector2fMultiplyAdd(from, vT, vDirection);
}

NN_FORCEINLINE float32x2_t Vector2fNegate(float32x2_t vector) NN_NOEXCEPT
{
    return vneg_f32(vector);
}

NN_FORCEINLINE float32x2_t Vector2fMask(float32x2_t vector, uint32x2_t mask) NN_NOEXCEPT
{
    return vreinterpret_f32_u32(vand_u32(vreinterpret_u32_f32(vector), mask));
}

NN_FORCEINLINE float32x2_t Vector2fSqrt(float32x2_t vector) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64) && defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    return vsqrt_f32(vector);
#else
    float32x2_t vRsqrt = Vector2fRsqrt(vector);
    float32x2_t vZero = Vector2fZero();
    uint32x2_t vEqualZero = vceq_f32(vector, vZero);

    float32x2_t vSqrt = Vector2fRecp(vRsqrt);
    vSqrt = vbsl_f32(vEqualZero, vZero, vSqrt);

    return vSqrt;
#endif
}

NN_FORCEINLINE float32x2_t Vector2fRecp(float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t vRecp = vrecpe_f32(vector);
    vRecp = vmul_f32(vRecp, vrecps_f32(vRecp, vector));
    vRecp = vmul_f32(vRecp, vrecps_f32(vRecp, vector));

    return vRecp;
}

NN_FORCEINLINE float32x2_t Vector2fRsqrt(float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t vRsqrt = vrsqrte_f32(vector);
    vRsqrt = vmul_f32(vRsqrt, vrsqrts_f32(vRsqrt, vmul_f32(vector, vRsqrt)));
    vRsqrt = vmul_f32(vRsqrt, vrsqrts_f32(vRsqrt, vmul_f32(vector, vRsqrt)));

    return vRsqrt;
}

NN_FORCEINLINE float32x2_t Vector2fRound(float32x2_t vector) NN_NOEXCEPT
{
#if false
    return vrndn_f32(vector);
#else
    uint32x2_t vGreaterEqualZero = vcge_f32(vector, Vector2fZero());
    float32x2_t vFactor = vbsl_f32(vGreaterEqualZero, vdup_n_f32(0.5f), vdup_n_f32(-0.5f));
    float32x2_t vTmp = vadd_f32(vector, vFactor);
    int32x2_t vTmp2 = vcvt_s32_f32(vTmp);

    return vcvt_f32_s32(vTmp2);
#endif
}

NN_FORCEINLINE float32x2_t Vector2fModPi(float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t vVectorDivided2Pi = vmul_f32(vector, vdup_n_f32(nn::util::detail::Float1Divided2Pi));
    float32x2_t vRoundedQuotient = Vector2fRound(vVectorDivided2Pi);
    float32x2_t vFloat2Pi = vdup_n_f32(nn::util::detail::Float2Pi);

    return Vector2fMultiplySubtract(vector, vRoundedQuotient, vFloat2Pi);
}

NN_FORCEINLINE float32x2_t Vector2fSin(float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t v = Vector2fModPi(vector);

    float32x2_t vPositiveFloatPiDivided2 = vdup_n_f32(nn::util::detail::FloatPiDivided2);
    uint32x2_t vGreaterThanPositiveFloatPiDivided2 = vcgt_f32(v, vPositiveFloatPiDivided2);
    float32x2_t vTmp1 = vsub_f32(vdup_n_f32(nn::util::detail::FloatPi), v);

    v = vbsl_f32(vGreaterThanPositiveFloatPiDivided2, vTmp1, v);

    float32x2_t vNegativeFloatPiDivided2 = vdup_n_f32(-nn::util::detail::FloatPiDivided2);
    uint32x2_t vLessThanNegativeFloatPiDivided2 = vclt_f32(v, vNegativeFloatPiDivided2);
    float32x2_t vTmp2 = vsub_f32(vdup_n_f32(-nn::util::detail::FloatPi), v);

    v = vbsl_f32(vLessThanNegativeFloatPiDivided2, vTmp2, v);

    float32x2_t vv = vmul_f32(v, v);

    float32x2_t vSin = vdup_n_f32(-nn::util::detail::SinCoefficients[0]);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::SinCoefficients[1]), vSin, vv);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::SinCoefficients[2]), vSin, vv);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::SinCoefficients[3]), vSin, vv);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::SinCoefficients[4]), vSin, vv);
    vSin = Vector2fMultiplyAdd(Vector2fValueOne, vSin, vv);
    vSin = vmul_f32(vSin, v);

    return vSin;
}

NN_FORCEINLINE float32x2_t Vector2fCos(float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t v = Vector2fModPi(vector);
    float32x2_t vSign = Vector2fValueOne;
    float32x2_t vNegativeSign = Vector2fValueNegativeOne;

    float32x2_t vPositiveFloatPiDivided2 = vdup_n_f32(nn::util::detail::FloatPiDivided2);
    uint32x2_t vGreaterThanPositiveFloatPiDivided2 = vcgt_f32(v, vPositiveFloatPiDivided2);
    float32x2_t vTmp1 = vsub_f32(vdup_n_f32(nn::util::detail::FloatPi), v);

    v = vbsl_f32(vGreaterThanPositiveFloatPiDivided2, vTmp1, v);
    vSign = vbsl_f32(vGreaterThanPositiveFloatPiDivided2, vNegativeSign, vSign);

    float32x2_t vNegativeFloatPiDivided2 = vdup_n_f32(-nn::util::detail::FloatPiDivided2);
    uint32x2_t vLessThanNegativeFloatPiDivided2 = vclt_f32(v, vNegativeFloatPiDivided2);
    float32x2_t vTmp2 = vsub_f32(vdup_n_f32(-nn::util::detail::FloatPi), v);

    v = vbsl_f32(vLessThanNegativeFloatPiDivided2, vTmp2, v);
    vSign = vbsl_f32(vLessThanNegativeFloatPiDivided2, vNegativeSign, vSign);

    float32x2_t vv = vmul_f32(v, v);

    float32x2_t vCos = vdup_n_f32(-nn::util::detail::CosCoefficients[0]);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::CosCoefficients[1]), vCos, vv);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::CosCoefficients[2]), vCos, vv);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::CosCoefficients[3]), vCos, vv);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::CosCoefficients[4]), vCos, vv);
    vCos = Vector2fMultiplyAdd(Vector2fValueOne, vCos, vv);
    vCos = vmul_f32(vSign, vCos);

    return vCos;
}

NN_FORCEINLINE void Vector2fSinCos(float32x2_t* pOutSin, float32x2_t* pOutCos, float32x2_t vector) NN_NOEXCEPT
{
    float32x2_t v = Vector2fModPi(vector);
    float32x2_t vSign = Vector2fValueOne;
    float32x2_t vNegativeSign = Vector2fValueNegativeOne;

    float32x2_t vPositiveFloatPiDivided2 = vdup_n_f32(nn::util::detail::FloatPiDivided2);
    uint32x2_t vGreaterThanPositiveFloatPiDivided2 = vcgt_f32(v, vPositiveFloatPiDivided2);
    float32x2_t vTmp1 = vsub_f32(vdup_n_f32(nn::util::detail::FloatPi), v);

    v = vbsl_f32(vGreaterThanPositiveFloatPiDivided2, vTmp1, v);
    vSign = vbsl_f32(vGreaterThanPositiveFloatPiDivided2, vNegativeSign, vSign);

    float32x2_t vNegativeFloatPiDivided2 = vdup_n_f32(-nn::util::detail::FloatPiDivided2);
    uint32x2_t vLessThanNegativeFloatPiDivided2 = vclt_f32(v, vNegativeFloatPiDivided2);
    float32x2_t vTmp2 = vsub_f32(vdup_n_f32(-nn::util::detail::FloatPi), v);

    v = vbsl_f32(vLessThanNegativeFloatPiDivided2, vTmp2, v);
    vSign = vbsl_f32(vLessThanNegativeFloatPiDivided2, vNegativeSign, vSign);

    float32x2_t vv = vmul_f32(v, v);

    float32x2_t vSin = vdup_n_f32(-nn::util::detail::SinCoefficients[0]);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::SinCoefficients[1]), vSin, vv);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::SinCoefficients[2]), vSin, vv);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::SinCoefficients[3]), vSin, vv);
    vSin = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::SinCoefficients[4]), vSin, vv);
    vSin = Vector2fMultiplyAdd(Vector2fValueOne, vSin, vv);
    vSin = vmul_f32(vSin, v);

    *pOutSin = vSin;

    float32x2_t vCos = vdup_n_f32(-nn::util::detail::CosCoefficients[0]);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::CosCoefficients[1]), vCos, vv);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::CosCoefficients[2]), vCos, vv);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(nn::util::detail::CosCoefficients[3]), vCos, vv);
    vCos = Vector2fMultiplyAdd(vdup_n_f32(-nn::util::detail::CosCoefficients[4]), vCos, vv);
    vCos = Vector2fMultiplyAdd(Vector2fValueOne, vCos, vv);
    vCos = vmul_f32(vSign, vCos);

    *pOutCos = vCos;
}

NN_FORCEINLINE float32x2_t Vector2fTransformRowMajor3x2(float32x2_t vector, float32x2x3_t matrix) NN_NOEXCEPT
{
    float32x2_t vResult;
    vResult = vmul_lane_f32(matrix.val[0], vector, 0);
    vResult = Vector2fMultiplyAddLane1(vResult, matrix.val[1], vector);
    vResult = Vector2fAdd(vResult, matrix.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x2_t Vector2fTransformColumnMajor3x2(float32x2_t vector, float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x2_t vTmp0 = vmul_f32(vector, vget_low_f32(matrix.val[0]));
    vTmp0 = vpadd_f32(vTmp0, vget_high_f32(matrix.val[0]));

    float32x2_t vTmp1 = vmul_f32(vector, vget_low_f32(matrix.val[1]));
    vTmp1 = vpadd_f32(vTmp1, vget_high_f32(matrix.val[1]));

    return vpadd_f32(vTmp0, vTmp1);
}

NN_FORCEINLINE float32x2_t Vector2fTransformNormalRowMajor3x2(float32x2_t vector, float32x2x3_t matrix) NN_NOEXCEPT
{
    float32x2_t vResult;
    vResult = vmul_lane_f32(matrix.val[0], vector, 0);
    vResult = Vector2fMultiplyAddLane1(vResult, matrix.val[1], vector);

    return vResult;
}

NN_FORCEINLINE float32x2_t Vector2fTransformNormalColumnMajor3x2(float32x2_t vector, float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x2_t vTmp0 = vmul_f32(vector, vget_low_f32(matrix.val[0]));
    float32x2_t vTmp1 = vmul_f32(vector, vget_low_f32(matrix.val[1]));

    return vpadd_f32(vTmp0, vTmp1);
}

}}}} // namespace nn::util::neon::detail
