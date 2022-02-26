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

#include <nn/nn_SdkAssert.h>
#include <nn/util/util_Vector.h>
#include <nn/util/util_Matrix.h>
#include <nn/util/util_Quaternion.h>
#include <nn/util/util_Constant.h>


namespace nn
{
namespace util
{

//-------------------------------------------------------------------------------------------------
//      四元数クラス
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
Quaternion::Quaternion() NN_NOEXCEPT
{}

NN_FORCEINLINE
Quaternion::Quaternion(float x, float y, float z, float w) NN_NOEXCEPT
{
    VectorSet(this, x, y, z, w);
}

NN_FORCEINLINE
Quaternion::Quaternion(const Float4& value) NN_NOEXCEPT
{
    VectorLoad(this, value);
}

NN_FORCEINLINE
Quaternion::Quaternion(const Vector4fType& value) NN_NOEXCEPT
: Vector4fType(value)
{}

NN_FORCEINLINE
float Quaternion::GetX() const NN_NOEXCEPT
{
    return VectorGetX(*this);
}

NN_FORCEINLINE
float Quaternion::GetY() const NN_NOEXCEPT
{
    return VectorGetY(*this);
}

NN_FORCEINLINE
float Quaternion::GetZ() const NN_NOEXCEPT
{
    return VectorGetZ(*this);
}

NN_FORCEINLINE
float Quaternion::GetW() const NN_NOEXCEPT
{
    return VectorGetW(*this);
}

NN_FORCEINLINE
void Quaternion::Get(float* pOutX, float* pOutY, float* pOutZ, float* pOutW) const NN_NOEXCEPT
{
    // 各成分ごとに取り出すよりも，一気に取り出したほうがシャッフルなどが無く速いことを期待して実装。
    // TODO : 要検証（命令数比較，速度比較）
    NN_SDK_REQUIRES_NOT_NULL(pOutX);
    NN_SDK_REQUIRES_NOT_NULL(pOutY);
    NN_SDK_REQUIRES_NOT_NULL(pOutZ);
    NN_SDK_REQUIRES_NOT_NULL(pOutW);
    Float4 element;
    VectorStore(&element, *this);
    (*pOutX) = element.v[0];
    (*pOutY) = element.v[1];
    (*pOutZ) = element.v[2];
    (*pOutW) = element.v[3];
}

NN_FORCEINLINE
void Quaternion::SetX(float value) NN_NOEXCEPT
{
    VectorSetX(this, value);
}

NN_FORCEINLINE
void Quaternion::SetY(float value) NN_NOEXCEPT
{
    VectorSetY(this, value);
}

NN_FORCEINLINE
void Quaternion::SetZ(float value) NN_NOEXCEPT
{
    VectorSetZ(this, value);
}

NN_FORCEINLINE
void Quaternion::SetW(float value) NN_NOEXCEPT
{
    VectorSetW(this, value);
}

NN_FORCEINLINE
void Quaternion::Set(float x, float y, float z, float w) NN_NOEXCEPT
{
    VectorSet(this, x, y, z, w);
}

NN_FORCEINLINE
Quaternion Quaternion::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
Quaternion Quaternion::operator - () const NN_NOEXCEPT
{
    Vector4fType zero;
    VectorZero(&zero);
    Quaternion result;
    QuaternionSubtract(&result, zero, *this);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::operator + (const Quaternion& value) const NN_NOEXCEPT
{
    Quaternion result;
    QuaternionAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::operator - (const Quaternion& value) const NN_NOEXCEPT
{
    Quaternion result;
    QuaternionSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::operator * (const Quaternion& value) const NN_NOEXCEPT
{
    Quaternion result;
    QuaternionMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::operator / (const Quaternion& value) const NN_NOEXCEPT
{
    Quaternion result;
    QuaternionDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Quaternion& Quaternion::operator += (const Quaternion& value) NN_NOEXCEPT
{
    QuaternionAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Quaternion& Quaternion::operator -= (const Quaternion& value) NN_NOEXCEPT
{
    QuaternionSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Quaternion& Quaternion::operator *= (const Quaternion& value) NN_NOEXCEPT
{
    QuaternionMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Quaternion& Quaternion::operator /= (const Quaternion& value) NN_NOEXCEPT
{
    QuaternionDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
void Quaternion::Conjugate() NN_NOEXCEPT
{
    Float4 element;
    VectorStore(&element, *this);
    element.v[0] = -element.v[0];
    element.v[1] = -element.v[1];
    element.v[2] = -element.v[2];
    VectorLoad(this, element);
}

NN_FORCEINLINE
void Quaternion::Inverse() NN_NOEXCEPT
{
    QuaternionInverse(this, *this);
}

NN_FORCEINLINE
Vector3f Quaternion::Rotate(const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorRotate(&result, value, *this);
    return result;
}

NN_FORCEINLINE
bool Quaternion::Normalize() NN_NOEXCEPT
{
    return QuaternionNormalize(this, *this) != 0.f;
}

NN_FORCEINLINE
float Quaternion::Dot(const Quaternion& value) const NN_NOEXCEPT
{
    return QuaternionDot(*this, value);
}

NN_FORCEINLINE
float Quaternion::Length() const NN_NOEXCEPT
{
    return VectorLength(*this);
}

NN_FORCEINLINE
float Quaternion::LengthSquared() const NN_NOEXCEPT
{
    return VectorLengthSquared(*this);
}

NN_FORCEINLINE
Vector3f Quaternion::ToAxisAngle() const NN_NOEXCEPT
{
    Float4 q;
    VectorStore(&q, *this);
    float x2 = q.x * q.x;
    float y2 = q.y * q.y;
    float z2 = q.z * q.z;
    float w2 = q.w * q.w;
    float mag = x2 + y2 + z2 + w2;
    float test = q.x * q.y + q.z * q.w;
    float rad = FloatPi / 2.0f;
    if (test > rad * mag)
    {
        return Vector3f(
            0.0f,
            2.0f * Atan2Est(q.x, q.w),
            FloatPi / 2.0f);
    }
    if (test < -rad * mag)
    {
        return Vector3f(
            0.0f,
            -2.0f * Atan2Est(q.x, q.w),
            -FloatPi / 2.0f);
    }

    return Vector3f(
        Atan2Est(2.0f * q.x * q.w - 2.0f * q.y * q.z, -x2 + y2 - z2 + w2),
        Atan2Est(2.0f * q.y * q.w - 2.0f * q.x * q.z,  x2 - y2 - z2 + w2),
        AsinEst(2.0f * test / mag));
}


NN_FORCEINLINE
float Quaternion::Dot(const Quaternion& a, const Quaternion& b) NN_NOEXCEPT
{
    return QuaternionDot(a, b);
}

NN_FORCEINLINE
Quaternion Quaternion::Normalize(const Quaternion& value) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionNormalize(&result, value);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::Inverse(const Quaternion& value) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionInverse(&result, value);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionLerp(&result, a, b, t);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionSlerp(&result, a, b, t);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::Squad
(
    const Quaternion& a,
    const Quaternion& b,
    const Quaternion& c,
    const Quaternion& d,
    float t
) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionSquad(&result, a, b, c, d, t);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::FromYawPitchRoll(float yaw, float pitch, float roll) NN_NOEXCEPT
{
    float sr, cr;
    float sp, cp;
    float sy, cy;
    SinCosEst(&sr, &cr, roll  * 0.5f);
    SinCosEst(&sp, &cp, pitch * 0.5f);
    SinCosEst(&sy, &cy, yaw   * 0.5f);

    return Quaternion(
        cy * sp * cr + sy * cp * sr,
        sy * cp * cr - cy * sp * sr,
        cy * cp * sr - sy * sp * cr,
        cy * cp * cr + sy * sp * sr);
}

NN_FORCEINLINE
Quaternion Quaternion::FromRotationMatrix(const MatrixRowMajor4x3f& value) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionFromMatrix(&result, value);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::FromAxisAngle(const Vector3f& axis, float radian) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionRotateAxis(&result, axis, radian);
    return result;
}

NN_FORCEINLINE
Quaternion Quaternion::MakeVectorRotation(const Vector3f& from, const Vector3f& to) NN_NOEXCEPT
{
    Quaternion result;
    QuaternionMakeVectorRotation(&result, from, to);
    return result;
}

NN_FORCEINLINE
const Quaternion& Quaternion::Identity() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Quaternion&>(ConstantIdentity);
}

}
}
