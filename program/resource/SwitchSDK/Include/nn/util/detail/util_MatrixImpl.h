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
#include <nn/util/util_Quaternion.h>
#include <limits>

namespace nn
{
namespace util
{

//-------------------------------------------------------------------------------------------------
//      3行2列　列優先行列クラス
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
MatrixRowMajor3x2f::MatrixRowMajor3x2f() NN_NOEXCEPT
{}

NN_FORCEINLINE
MatrixRowMajor3x2f::MatrixRowMajor3x2f
(
    float m00, float m01,
    float m10, float m11,
    float m20, float m21
) NN_NOEXCEPT
{
    MatrixSet(
        this,
        m00, m01,
        m10, m11,
        m20, m21);
}

NN_FORCEINLINE
MatrixRowMajor3x2f::MatrixRowMajor3x2f
(
    const Vector2f& axisX,
    const Vector2f& axisY,
    const Vector2f& axisZ
) NN_NOEXCEPT
{
    MatrixSetAxes(this, axisX, axisY, axisZ);
}

NN_FORCEINLINE
MatrixRowMajor3x2f::MatrixRowMajor3x2f(const FloatRowMajor3x2& value) NN_NOEXCEPT
{
    MatrixLoad(this, value);
}

NN_FORCEINLINE
MatrixRowMajor3x2f::MatrixRowMajor3x2f(const MatrixRowMajor3x2fType& value) NN_NOEXCEPT
: MatrixRowMajor3x2fType(value)
{}

NN_FORCEINLINE
Vector2f MatrixRowMajor3x2f::GetAxisX() const NN_NOEXCEPT
{
    Vector2f result;
    MatrixGetAxisX(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector2f MatrixRowMajor3x2f::GetAxisY() const NN_NOEXCEPT
{
    Vector2f result;
    MatrixGetAxisY(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector2f MatrixRowMajor3x2f::GetAxisZ() const NN_NOEXCEPT
{
    Vector2f result;
    MatrixGetAxisZ(&result, *this);
    return result;
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::Get(Vector2f* pOutX, Vector2f* pOutY, Vector2f* pOutZ) const NN_NOEXCEPT
{
    MatrixGetAxes(pOutX, pOutY, pOutZ, *this);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::SetAxisX(const Vector2f& value) NN_NOEXCEPT
{
    MatrixSetAxisX(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::SetAxisY(const Vector2f& value) NN_NOEXCEPT
{
    MatrixSetAxisY(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::SetAxisZ(const Vector2f& value) NN_NOEXCEPT
{
    MatrixSetAxisZ(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::Set
(
    const Vector2f& axisX,
    const Vector2f& axisY,
    const Vector2f& axisZ
) NN_NOEXCEPT
{
    MatrixSetAxes(this, axisX, axisY, axisZ);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::SetScale(const Vector2f& value) NN_NOEXCEPT
{
    MatrixSetScale(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::SetTranslate(const Vector2f& value) NN_NOEXCEPT
{
    MatrixSetTranslate(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::SetRotate(float value) NN_NOEXCEPT
{
    MatrixSetRotate(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor3x2f::Transpose() NN_NOEXCEPT
{
    MatrixTranspose(this, *this);
}

NN_FORCEINLINE
bool MatrixRowMajor3x2f::Inverse() NN_NOEXCEPT
{
    return MatrixInverse(this, *this);
}

NN_FORCEINLINE
bool MatrixRowMajor3x2f::InverseTranspose() NN_NOEXCEPT
{
    return MatrixInverseTranspose(this, *this);
}

NN_FORCEINLINE
Vector2f MatrixRowMajor3x2f::Transform(const Vector2f& value) const NN_NOEXCEPT
{
    Vector2f result;
    VectorTransform(&result, value, *this);
    return result;
}

NN_FORCEINLINE
Vector2f MatrixRowMajor3x2f::TransformNormal(const Vector2f& value) const NN_NOEXCEPT
{
    Vector2f result;
    VectorTransformNormal(&result, value, *this);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator - () const NN_NOEXCEPT
{
    MatrixRowMajor3x2fType zero;
    MatrixZero(&zero);
    MatrixRowMajor3x2f result;
    MatrixSubtract(&result, zero, *this);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator + (const MatrixRowMajor3x2f& value) const NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator - (const MatrixRowMajor3x2f& value) const NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator * (const MatrixRowMajor3x2f& value) const NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator * (float value) const NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::operator / (float value) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    MatrixRowMajor3x2f result;
    MatrixDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f operator * (float scalar, const MatrixRowMajor3x2f& matrix) NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixMultiply(&result, matrix, scalar);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f& MatrixRowMajor3x2f::operator += (const MatrixRowMajor3x2f& value) NN_NOEXCEPT
{
    MatrixAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor3x2f& MatrixRowMajor3x2f::operator -= (const MatrixRowMajor3x2f& value) NN_NOEXCEPT
{
    MatrixSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor3x2f& MatrixRowMajor3x2f::operator *= (const MatrixRowMajor3x2f& value) NN_NOEXCEPT
{
    MatrixMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor3x2f& MatrixRowMajor3x2f::operator *= (float value) NN_NOEXCEPT
{
    MatrixMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor3x2f& MatrixRowMajor3x2f::operator /= (float value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    MatrixDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::MakeTranslation(const Vector2f& value) NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixIdentity(&result);
    MatrixSetTranslate(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::MakeScale(const Vector2f& value) NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixIdentity(&result);
    MatrixSetScale(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::MakeRotation(float radian) NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixIdentity(&result);
    MatrixSetRotate(&result, radian);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor3x2f MatrixRowMajor3x2f::MakeTranspose(const MatrixRowMajor3x2f& value) NN_NOEXCEPT
{
    MatrixRowMajor3x2f result;
    MatrixTranspose(&result, value);
    return result;
}

NN_FORCEINLINE
const MatrixRowMajor3x2f& MatrixRowMajor3x2f::Zero() NN_NOEXCEPT
{
    return static_cast<const MatrixRowMajor3x2f&>(ConstantZero);
}

NN_FORCEINLINE
const MatrixRowMajor3x2f& MatrixRowMajor3x2f::Identity() NN_NOEXCEPT
{
    return static_cast<const MatrixRowMajor3x2f&>(ConstantIdentity);
}


//-------------------------------------------------------------------------------------------------
//      4行3列　列優先行列クラス
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
MatrixRowMajor4x3f::MatrixRowMajor4x3f() NN_NOEXCEPT
{}

NN_FORCEINLINE
MatrixRowMajor4x3f::MatrixRowMajor4x3f
(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22,
    float m30, float m31, float m32
) NN_NOEXCEPT
{
    MatrixSet(
        this,
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22,
        m30, m31, m32);
}

NN_FORCEINLINE
MatrixRowMajor4x3f::MatrixRowMajor4x3f
(
    const Vector3f& axisX,
    const Vector3f& axisY,
    const Vector3f& axisZ,
    const Vector3f& axisW
) NN_NOEXCEPT
{
    MatrixSetAxes(this, axisX, axisY, axisZ, axisW);
}

NN_FORCEINLINE
MatrixRowMajor4x3f::MatrixRowMajor4x3f(const FloatRowMajor4x3& value) NN_NOEXCEPT
{
    MatrixLoad(this, value);
}

NN_FORCEINLINE
MatrixRowMajor4x3f::MatrixRowMajor4x3f(const MatrixRowMajor4x3fType& value) NN_NOEXCEPT
: MatrixRowMajor4x3fType(value)
{}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x3f::GetAxisX() const NN_NOEXCEPT
{
    Vector3f result;
    MatrixGetAxisX(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x3f::GetAxisY() const NN_NOEXCEPT
{
    Vector3f result;
    MatrixGetAxisY(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x3f::GetAxisZ() const NN_NOEXCEPT
{
    Vector3f result;
    MatrixGetAxisZ(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x3f::GetAxisW() const NN_NOEXCEPT
{
    Vector3f result;
    MatrixGetAxisW(&result, *this);
    return result;
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::Get(Vector3f* pOutX, Vector3f* pOutY, Vector3f* pOutZ, Vector3f* pOutW) const NN_NOEXCEPT
{
    MatrixGetAxes(pOutX, pOutY, pOutZ, pOutW, *this);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetAxisX(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetAxisX(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetAxisY(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetAxisY(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetAxisZ(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetAxisZ(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetAxisW(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetAxisW(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::Set
(
    const Vector3f& axisX,
    const Vector3f& axisY,
    const Vector3f& axisZ,
    const Vector3f& axisW
) NN_NOEXCEPT
{
    MatrixSetAxes(this, axisX, axisY, axisZ, axisW);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetScale(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetScale(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetTranslate(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetTranslate(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetRotate(const Quaternion& value) NN_NOEXCEPT
{
    MatrixSetRotate(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::SetRotate(const Vector3f& value) NN_NOEXCEPT
{
    MatrixSetRotateXyz(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x3f::Transpose() NN_NOEXCEPT
{
    MatrixTranspose(this, *this);
}

NN_FORCEINLINE
bool MatrixRowMajor4x3f::Inverse() NN_NOEXCEPT
{
    return MatrixInverse(this, *this);
}

NN_FORCEINLINE
bool MatrixRowMajor4x3f::InverseTranspose() NN_NOEXCEPT
{
    return MatrixInverseTranspose(this, *this);
}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x3f::Transform(const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorTransform(&result, value, *this);
    return result;
}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x3f::TransformNormal(const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorTransformNormal(&result, value, *this);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator - () const NN_NOEXCEPT
{
    MatrixRowMajor4x3fType zero;
    MatrixZero(&zero);
    MatrixRowMajor4x3f result;
    MatrixSubtract(&result, zero, *this);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator + (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator - (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator * (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x3f::operator * (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator * (float value) const NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::operator / (float value) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    MatrixRowMajor4x3f result;
    MatrixDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f operator * (float scalar, const MatrixRowMajor4x3f& matrix) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixMultiply(&result, matrix, scalar);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f& MatrixRowMajor4x3f::operator += (const MatrixRowMajor4x3f& value) NN_NOEXCEPT
{
    MatrixAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x3f& MatrixRowMajor4x3f::operator -= (const MatrixRowMajor4x3f& value) NN_NOEXCEPT
{
    MatrixSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x3f& MatrixRowMajor4x3f::operator *= (const MatrixRowMajor4x3f& value) NN_NOEXCEPT
{
    MatrixMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x3f& MatrixRowMajor4x3f::operator *= (float value) NN_NOEXCEPT
{
    MatrixMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x3f& MatrixRowMajor4x3f::operator /= (float value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    MatrixDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::MakeTranslation(const Vector3f& value) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixIdentity(&result);
    MatrixSetTranslate(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::MakeScale(const Vector3f& value) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixIdentity(&result);
    MatrixSetScale(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::MakeRotation(const Vector3f& radian) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixIdentity(&result);
    MatrixSetRotateXyz(&result, radian);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::MakeRotation(const Quaternion& value) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixIdentity(&result);
    MatrixSetRotate(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::MakeTranspose(const MatrixRowMajor4x3f& value) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixTranspose(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::LookAtRightHanded
(
    const Vector3f& position,
    const Vector3f& target,
    const Vector3f& upward
) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixLookAtRightHanded(&result, position, target, upward);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x3f::LookToRightHanded
(
    const Vector3f& position,
    const Vector3f& direction,
    const Vector3f& upward
) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixLookAtRightHanded(&result, position, - direction + position, upward);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x3f::ToMatrix4x4f(const MatrixRowMajor4x3f& value) NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixConvert(&result, value);
    return result;
}

NN_FORCEINLINE
const MatrixRowMajor4x3f& MatrixRowMajor4x3f::Zero() NN_NOEXCEPT
{
    return static_cast<const MatrixRowMajor4x3f&>(ConstantZero);
}

NN_FORCEINLINE
const MatrixRowMajor4x3f& MatrixRowMajor4x3f::Identity() NN_NOEXCEPT
{
    return static_cast<const MatrixRowMajor4x3f&>(ConstantIdentity);
}


//-------------------------------------------------------------------------------------------------
//      4行4列　列優先行列クラス
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
MatrixRowMajor4x4f::MatrixRowMajor4x4f() NN_NOEXCEPT
{}

NN_FORCEINLINE
MatrixRowMajor4x4f::MatrixRowMajor4x4f
(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
) NN_NOEXCEPT
{
    MatrixSet(
        this,
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33);
}

NN_FORCEINLINE
MatrixRowMajor4x4f::MatrixRowMajor4x4f
(
    const Vector4f& axisX,
    const Vector4f& axisY,
    const Vector4f& axisZ,
    const Vector4f& axisW
) NN_NOEXCEPT
{
    MatrixSetRows(this, axisX, axisY, axisZ, axisW);
}

NN_FORCEINLINE
MatrixRowMajor4x4f::MatrixRowMajor4x4f(const FloatRowMajor4x4& value) NN_NOEXCEPT
{
    MatrixLoad(this, value);
}

NN_FORCEINLINE
MatrixRowMajor4x4f::MatrixRowMajor4x4f(const MatrixRowMajor4x4fType& value) NN_NOEXCEPT
: MatrixRowMajor4x4fType(value)
{}

NN_FORCEINLINE
Vector4f MatrixRowMajor4x4f::GetAxisX() const NN_NOEXCEPT
{
    Vector4f result;
    MatrixGetAxisX(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector4f MatrixRowMajor4x4f::GetAxisY() const NN_NOEXCEPT
{
    Vector4f result;
    MatrixGetAxisY(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector4f MatrixRowMajor4x4f::GetAxisZ() const NN_NOEXCEPT
{
    Vector4f result;
    MatrixGetAxisZ(&result, *this);
    return result;
}

NN_FORCEINLINE
Vector4f MatrixRowMajor4x4f::GetAxisW() const NN_NOEXCEPT
{
    Vector4f result;
    MatrixGetAxisW(&result, *this);
    return result;
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::Get(Vector4f* pOutX, Vector4f* pOutY, Vector4f* pOutZ, Vector4f* pOutW) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(pOutX);
    NN_SDK_REQUIRES_NOT_NULL(pOutY);
    NN_SDK_REQUIRES_NOT_NULL(pOutZ);
    NN_SDK_REQUIRES_NOT_NULL(pOutW);

    MatrixGetAxes(pOutX, pOutY, pOutZ, pOutW, *this);
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::SetAxisX(const Vector4f& value) NN_NOEXCEPT
{
    MatrixSetAxisX(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::SetAxisY(const Vector4f& value) NN_NOEXCEPT
{
    MatrixSetAxisY(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::SetAxisZ(const Vector4f& value) NN_NOEXCEPT
{
    MatrixSetAxisZ(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::SetAxisW(const Vector4f& value) NN_NOEXCEPT
{
    MatrixSetAxisW(this, value);
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::Set
(
    const Vector4f& axisX,
    const Vector4f& axisY,
    const Vector4f& axisZ,
    const Vector4f& axisW
) NN_NOEXCEPT
{
    MatrixSetAxes(this, axisX, axisY, axisZ, axisW);
}

NN_FORCEINLINE
void MatrixRowMajor4x4f::Transpose() NN_NOEXCEPT
{
    MatrixTranspose(this, *this);
}

NN_FORCEINLINE
bool MatrixRowMajor4x4f::Inverse() NN_NOEXCEPT
{
    return MatrixInverse(this, *this);
}

NN_FORCEINLINE
bool MatrixRowMajor4x4f::InverseTranspose() NN_NOEXCEPT
{
    if (!MatrixInverse(this, *this))
    {
        return false;
    }

    MatrixTranspose(this, *this);
    return true;
}

NN_FORCEINLINE
Vector4f MatrixRowMajor4x4f::Transform(const Vector4f& value) const NN_NOEXCEPT
{
    Vector4f result;
    VectorTransform(&result, value, *this);
    return result;
}

NN_FORCEINLINE
Vector3f MatrixRowMajor4x4f::TransformCoord(const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorTransformCoord(&result, value, *this);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator - () const NN_NOEXCEPT
{
    MatrixRowMajor4x4fType zero;
    MatrixZero(&zero);
    MatrixRowMajor4x4f result;
    MatrixSubtract(&result, zero, *this);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator + (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator - (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator * (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator * (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator * (float value) const NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::operator / (float value) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    MatrixRowMajor4x4f result;
    MatrixDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f operator * (float scalar, const MatrixRowMajor4x4f& matrix) NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixMultiply(&result, matrix, scalar);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f& MatrixRowMajor4x4f::operator += (const MatrixRowMajor4x4f& value) NN_NOEXCEPT
{
    MatrixAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x4f& MatrixRowMajor4x4f::operator -= (const MatrixRowMajor4x4f& value) NN_NOEXCEPT
{
    MatrixSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x4f& MatrixRowMajor4x4f::operator *= (const MatrixRowMajor4x4f& value) NN_NOEXCEPT
{
    MatrixMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x4f& MatrixRowMajor4x4f::operator *= (float value) NN_NOEXCEPT
{
    MatrixMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x4f& MatrixRowMajor4x4f::operator /= (float value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    MatrixDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::MakeTranspose(const MatrixRowMajor4x4f& value) NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixTranspose(&result, value);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::PerspectiveRightHanded(float width, float height, float nearClip, float farClip) NN_NOEXCEPT
{
    float depth = farClip - nearClip;
    NN_SDK_REQUIRES(std::abs(width)  > std::numeric_limits<float>::epsilon());
    NN_SDK_REQUIRES(std::abs(height) > std::numeric_limits<float>::epsilon());
    NN_SDK_REQUIRES(std::abs(depth)  > std::numeric_limits<float>::epsilon());

    return MatrixRowMajor4x4f(
        2.0f * nearClip / width,
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        2.0f * nearClip / height,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        -farClip / depth,
        -1.0f,

        0.0f,
        0.0f,
        -(nearClip * farClip) / depth,
        0.0f);
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::PerspectiveFieldOfViewRightHanded(float fieldOfView, float aspectRatio, float nearClip, float farClip) NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixPerspectiveFieldOfViewRightHanded(&result, fieldOfView, aspectRatio, nearClip, farClip);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::PerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearClip, float farClip) NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixPerspectiveOffCenterRightHanded(&result, left, right, bottom, top, nearClip, farClip);
    return result;
}


NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::OrthographicRightHanded(float width, float height, float nearClip, float farClip) NN_NOEXCEPT
{
    float depth = (farClip - nearClip);
    NN_SDK_REQUIRES(std::abs(width)  > std::numeric_limits<float>::epsilon());
    NN_SDK_REQUIRES(std::abs(height) > std::numeric_limits<float>::epsilon());
    NN_SDK_REQUIRES(std::abs(depth)  > std::numeric_limits<float>::epsilon());

    return MatrixRowMajor4x4f(
        2.0f / width,
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        2.0f / height,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        -1.0f / depth,
        0.0f,

        0.0f,
        0.0f,
        -nearClip / depth,
        1.0f);
}

NN_FORCEINLINE
MatrixRowMajor4x4f MatrixRowMajor4x4f::OrthographicOffCenterRightHanded
(
    float left,
    float right,
    float bottom,
    float top,
    float nearClip,
    float farClip
) NN_NOEXCEPT
{
    MatrixRowMajor4x4f result;
    MatrixOrthographicOffCenterRightHanded(&result, left, right, bottom, top, nearClip, farClip);
    return result;
}

NN_FORCEINLINE
MatrixRowMajor4x3f MatrixRowMajor4x4f::ToMatrix4x3f(const MatrixRowMajor4x4f& value) NN_NOEXCEPT
{
    MatrixRowMajor4x3f result;
    MatrixConvert(&result, value);
    return result;
}

NN_FORCEINLINE
const MatrixRowMajor4x4f& MatrixRowMajor4x4f::Zero() NN_NOEXCEPT
{
    return static_cast<const MatrixRowMajor4x4f&>(ConstantZero);
}

NN_FORCEINLINE
const MatrixRowMajor4x4f& MatrixRowMajor4x4f::Identity() NN_NOEXCEPT
{
    return static_cast<const MatrixRowMajor4x4f&>(ConstantIdentity);
}

}
}
