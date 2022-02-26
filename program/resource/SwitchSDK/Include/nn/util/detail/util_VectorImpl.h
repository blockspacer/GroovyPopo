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
#include <nn/util/util_Matrix.h>
#include <nn/util/util_Quaternion.h>
#include <limits>

namespace nn
{
namespace util
{

//-------------------------------------------------------------------------------------------------
//      2次元ベクトルクラス.
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
Vector2f::Vector2f() NN_NOEXCEPT
{}

NN_FORCEINLINE
Vector2f::Vector2f(float x, float y) NN_NOEXCEPT
{
    VectorSet(this, x, y);
}

NN_FORCEINLINE
Vector2f::Vector2f(const Float2& value) NN_NOEXCEPT
{
    VectorLoad(this, value);
}

NN_FORCEINLINE
Vector2f::Vector2f(const Vector2fType& value) NN_NOEXCEPT
: Vector2fType(value)
{}

NN_FORCEINLINE
float Vector2f::GetX() const NN_NOEXCEPT
{
    return VectorGetX(*this);
}

NN_FORCEINLINE
float Vector2f::GetY() const NN_NOEXCEPT
{
    return VectorGetY(*this);
}

NN_FORCEINLINE
void Vector2f::Get(float*  pOutX, float* pOutY) const NN_NOEXCEPT
{
    // 各成分ごとに取り出すよりも，一気に取り出したほうがシャッフルなどが無く速いことを期待して実装。
    // TODO : 要検証（命令数比較，速度比較）
    NN_SDK_REQUIRES_NOT_NULL(pOutX);
    NN_SDK_REQUIRES_NOT_NULL(pOutY);
    Float2 element;
    VectorStore(&element, *this);
    (*pOutX) = element.v[0];
    (*pOutY) = element.v[1];
}

NN_FORCEINLINE
void Vector2f::SetX(float x) NN_NOEXCEPT
{
    VectorSetX(this, x);
}

NN_FORCEINLINE
void Vector2f::SetY(float y) NN_NOEXCEPT
{
    VectorSetY(this, y);
}

NN_FORCEINLINE
void Vector2f::Set(float x, float y) NN_NOEXCEPT
{
    VectorSet(this, x, y);
}

NN_FORCEINLINE
Vector2f Vector2f::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
Vector2f Vector2f::operator - () const NN_NOEXCEPT
{
    Vector2fType zero = NN_UTIL_VECTOR_2F_INITIALIZER(0.0f, 0.0f);
    Vector2f result;
    VectorSubtract(&result, zero, (*this));
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::operator + (const Vector2f& value) const NN_NOEXCEPT
{
    Vector2f result;
    VectorAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::operator - (const Vector2f& value) const NN_NOEXCEPT
{
    Vector2f result;
    VectorSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::operator * (float value) const NN_NOEXCEPT
{
    Vector2f result;
    VectorMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector2f operator * (float scalar, const Vector2f& vector) NN_NOEXCEPT
{
    Vector2f result;
    VectorMultiply(&result, vector, scalar);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::operator / (float value) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    Vector2f result;
    VectorDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector2f& Vector2f::operator += (const Vector2f& value) NN_NOEXCEPT
{
    VectorAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector2f& Vector2f::operator -= (const Vector2f& value) NN_NOEXCEPT
{
    VectorSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector2f& Vector2f::operator *= (float value) NN_NOEXCEPT
{
    VectorMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector2f& Vector2f::operator /= (float value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    VectorDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
bool Vector2f::Normalize() NN_NOEXCEPT
{
    return VectorNormalize(this, *this) != 0.f;
}

NN_FORCEINLINE
float Vector2f::Dot(const Vector2f& value) const NN_NOEXCEPT
{
    return VectorDot(*this, value);
}

NN_FORCEINLINE
float Vector2f::Cross(const Vector2f& value) const NN_NOEXCEPT
{
    return VectorCross(*this, value);
}

NN_FORCEINLINE
float Vector2f::Length() const NN_NOEXCEPT
{
    return VectorLength(*this);
}

NN_FORCEINLINE
float Vector2f::LengthSquared() const NN_NOEXCEPT
{
    return VectorLengthSquared(*this);
}

NN_FORCEINLINE
float Vector2f::Dot(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT
{
    return VectorDot(a, b);
}

NN_FORCEINLINE
float Vector2f::Distance(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT
{
    return VectorDistance(a, b);
}

NN_FORCEINLINE
float Vector2f::DistanceSquared(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT
{
    return VectorDistanceSquared(a, b);
}

NN_FORCEINLINE
Vector2f Vector2f::Normalize(const Vector2f& value) NN_NOEXCEPT
{
    Vector2f result;
    VectorNormalize(&result, value);
    return result;
}

NN_FORCEINLINE
float Vector2f::Cross(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT
{
    return VectorCross(a, b);
}

NN_FORCEINLINE
Vector2f Vector2f::Maximize(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT
{
    Vector2f result;
    VectorMaximize(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::Minimize(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT
{
    Vector2f result;
    VectorMinimize(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::Lerp(const Vector2f& a, const Vector2f& b, float t) NN_NOEXCEPT
{
    Vector2f result;
    VectorLerp(&result, a, b, t);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::Transform(const Vector2f& value, const MatrixRowMajor3x2f& transform) NN_NOEXCEPT
{
    Vector2f result;
    VectorTransform(&result, value, transform);
    return result;
}

NN_FORCEINLINE
Vector2f Vector2f::TransformNormal(const Vector2f& value, const MatrixRowMajor3x2f& transform) NN_NOEXCEPT
{
    Vector2f result;
    VectorTransformNormal(&result, value, transform);
    return result;
}

NN_FORCEINLINE
const Vector2f& Vector2f::Zero() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector2f&>(ConstantZero);
}

NN_FORCEINLINE
const Vector2f& Vector2f::One() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector2f&>(ConstantOne);
}

NN_FORCEINLINE
const Vector2f& Vector2f::UnitX() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector2f&>(ConstantUnitX);
}

NN_FORCEINLINE
const Vector2f& Vector2f::UnitY() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector2f&>(ConstantUnitY);
}

//-------------------------------------------------------------------------------------------------
//      3次元ベクトルクラス.
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
Vector3f::Vector3f() NN_NOEXCEPT
{}

NN_FORCEINLINE
Vector3f::Vector3f(float x, float y, float z) NN_NOEXCEPT
{
    VectorSet(this, x, y, z);
}

NN_FORCEINLINE
Vector3f::Vector3f(const Float3& value) NN_NOEXCEPT
{
    VectorLoad(this, value);
}

NN_FORCEINLINE
Vector3f::Vector3f(const Vector3fType& value) NN_NOEXCEPT
: Vector3fType(value)
{}

NN_FORCEINLINE
float Vector3f::GetX() const NN_NOEXCEPT
{
    return VectorGetX(*this);
}

NN_FORCEINLINE
float Vector3f::GetY() const NN_NOEXCEPT
{
    return VectorGetY(*this);
}

NN_FORCEINLINE
float Vector3f::GetZ() const NN_NOEXCEPT
{
    return VectorGetZ(*this);
}

NN_FORCEINLINE
void Vector3f::Get(float*  pOutX, float* pOutY, float* pOutZ) const NN_NOEXCEPT
{
    // 各成分ごとに取り出すよりも，一気に取り出したほうがシャッフルなどが無く速いことを期待して実装。
    // TODO : 要検証（命令数比較，速度比較）
    NN_SDK_REQUIRES_NOT_NULL(pOutX);
    NN_SDK_REQUIRES_NOT_NULL(pOutY);
    NN_SDK_REQUIRES_NOT_NULL(pOutZ);
    Float3 element;
    VectorStore(&element, *this);
    (*pOutX) = element.v[0];
    (*pOutY) = element.v[1];
    (*pOutZ) = element.v[2];
}

NN_FORCEINLINE
void Vector3f::SetX(float x) NN_NOEXCEPT
{
    VectorSetX(this, x);
}

NN_FORCEINLINE
void Vector3f::SetY(float y) NN_NOEXCEPT
{
    VectorSetY(this, y);
}

NN_FORCEINLINE
void Vector3f::SetZ(float z) NN_NOEXCEPT
{
    VectorSetZ(this, z);
}

NN_FORCEINLINE
void Vector3f::Set(float x, float y, float z) NN_NOEXCEPT
{
    VectorSet(this, x, y, z);
}

NN_FORCEINLINE
Vector3f Vector3f::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
Vector3f Vector3f::operator - () const NN_NOEXCEPT
{
    Vector3fType zero = NN_UTIL_VECTOR_3F_INITIALIZER(0.0f, 0.0f, 0.0f);
    Vector3f result;
    VectorSubtract(&result, zero, (*this));
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::operator + (const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::operator - (const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::operator * (float value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector3f operator * (float scalar, const Vector3f& vector) NN_NOEXCEPT
{
    Vector3f result;
    VectorMultiply(&result, vector, scalar);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::operator / (float value) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    Vector3f result;
    VectorDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector3f& Vector3f::operator += (const Vector3f& value) NN_NOEXCEPT
{
    VectorAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector3f& Vector3f::operator -= (const Vector3f& value) NN_NOEXCEPT
{
    VectorSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector3f& Vector3f::operator *= (float value) NN_NOEXCEPT
{
    VectorMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector3f& Vector3f::operator /= (float value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    VectorDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
bool Vector3f::Normalize() NN_NOEXCEPT
{
    return VectorNormalize(this, *this) != 0.f;
}

NN_FORCEINLINE
float Vector3f::Dot(const Vector3f& value) const NN_NOEXCEPT
{
    return VectorDot(*this, value);
}

NN_FORCEINLINE
Vector3f Vector3f::Cross(const Vector3f& value) const NN_NOEXCEPT
{
    Vector3f result;
    VectorCross(&result, *this, value);
    return result;
}

NN_FORCEINLINE
float Vector3f::Length() const NN_NOEXCEPT
{
    return VectorLength(*this);
}

NN_FORCEINLINE
float Vector3f::LengthSquared() const NN_NOEXCEPT
{
    return VectorLengthSquared(*this);
}

NN_FORCEINLINE
float Vector3f::Dot(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT
{
    return VectorDot(a, b);
}

NN_FORCEINLINE
float Vector3f::Distance(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT
{
    return VectorDistance(a, b);
}

NN_FORCEINLINE
float Vector3f::DistanceSquared(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT
{
    return VectorDistanceSquared(a, b);
}

NN_FORCEINLINE
Vector3f Vector3f::Normalize(const Vector3f& value) NN_NOEXCEPT
{
    Vector3f result;
    VectorNormalize(&result, value);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::Cross(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT
{
    Vector3f result;
    VectorCross(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::Maximize(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT
{
    Vector3f result;
    VectorMaximize(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::Minimize(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT
{
    Vector3f result;
    VectorMinimize(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::Lerp(const Vector3f& a, const Vector3f& b, float t) NN_NOEXCEPT
{
    Vector3f result;
    VectorLerp(&result, a, b, t);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::Rotate(const Vector3f& value, const Quaternion& rotation) NN_NOEXCEPT
{
    Vector3f result;
    VectorRotate(&result, value, rotation);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::Transform(const Vector3f& value, const MatrixRowMajor4x3f& transform) NN_NOEXCEPT
{
    Vector3f result;
    VectorTransform(&result, value, transform);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::TransformNormal(const Vector3f& value, const MatrixRowMajor4x3f& transform) NN_NOEXCEPT
{
    Vector3f result;
    VectorTransformNormal(&result, value, transform);
    return result;
}

NN_FORCEINLINE
Vector3f Vector3f::TransformCoord(const Vector3f& value, const MatrixRowMajor4x4f& transform) NN_NOEXCEPT
{
    Vector3f result;
    VectorTransformCoord(&result, value, transform);
    return result;
}

NN_FORCEINLINE
const Vector3f& Vector3f::Zero() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector3f&>(ConstantZero);
}

NN_FORCEINLINE
const Vector3f& Vector3f::One() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector3f&>(ConstantOne);
}

NN_FORCEINLINE
const Vector3f& Vector3f::UnitX() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector3f&>(ConstantUnitX);
}

NN_FORCEINLINE
const Vector3f& Vector3f::UnitY() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector3f&>(ConstantUnitY);
}

NN_FORCEINLINE
const Vector3f& Vector3f::UnitZ() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector3f&>(ConstantUnitZ);
}


//-------------------------------------------------------------------------------------------------
//      4次元ベクトルクラス.
//-------------------------------------------------------------------------------------------------
NN_FORCEINLINE
Vector4f::Vector4f() NN_NOEXCEPT
{}

NN_FORCEINLINE
Vector4f::Vector4f(float x, float y, float z, float w) NN_NOEXCEPT
{
    VectorSet(this, x, y, z, w);
}

NN_FORCEINLINE
Vector4f::Vector4f(const Vector3f& v, float w) NN_NOEXCEPT
{
    Float3 element;
    VectorStore(&element, v);
    VectorSet(this, element.v[0], element.v[1], element.v[2], w);
}

NN_FORCEINLINE
Vector4f::Vector4f(const Float4& value) NN_NOEXCEPT
{
    VectorLoad(this, value);
}

NN_FORCEINLINE
Vector4f::Vector4f(const Vector4fType& value) NN_NOEXCEPT
: Vector4fType(value)
{}

NN_FORCEINLINE
float Vector4f::GetX() const NN_NOEXCEPT
{
    return VectorGetX(*this);
}

NN_FORCEINLINE
float Vector4f::GetY() const NN_NOEXCEPT
{
    return VectorGetY(*this);
}

NN_FORCEINLINE
float Vector4f::GetZ() const NN_NOEXCEPT
{
    return VectorGetZ(*this);
}

NN_FORCEINLINE
float Vector4f::GetW() const NN_NOEXCEPT
{
    return VectorGetW(*this);
}

NN_FORCEINLINE
void Vector4f::Get(float*  pOutX, float* pOutY, float* pOutZ, float* pOutW) const NN_NOEXCEPT
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
void Vector4f::SetX(float x) NN_NOEXCEPT
{
    VectorSetX(this, x);
}

NN_FORCEINLINE
void Vector4f::SetY(float y) NN_NOEXCEPT
{
    VectorSetY(this, y);
}

NN_FORCEINLINE
void Vector4f::SetZ(float z) NN_NOEXCEPT
{
    VectorSetZ(this, z);
}

NN_FORCEINLINE
void Vector4f::SetW(float w) NN_NOEXCEPT
{
    VectorSetW(this, w);
}

NN_FORCEINLINE
void Vector4f::Set(float x, float y, float z, float w) NN_NOEXCEPT
{
    VectorSet(this, x, y, z, w);
}

NN_FORCEINLINE
Vector4f Vector4f::operator + () const NN_NOEXCEPT
{
    return *this;
}

NN_FORCEINLINE
Vector4f Vector4f::operator - () const NN_NOEXCEPT
{
    Vector4fType zero = NN_UTIL_VECTOR_4F_INITIALIZER(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4f result;
    VectorSubtract(&result, zero, (*this));
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::operator + (const Vector4f& value) const NN_NOEXCEPT
{
    Vector4f result;
    VectorAdd(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::operator - (const Vector4f& value) const NN_NOEXCEPT
{
    Vector4f result;
    VectorSubtract(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::operator * (float value) const NN_NOEXCEPT
{
    Vector4f result;
    VectorMultiply(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector4f operator * (float scalar, const Vector4f& vector) NN_NOEXCEPT
{
    Vector4f result;
    VectorMultiply(&result, vector, scalar);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::operator / (float value) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    Vector4f result;
    VectorDivide(&result, *this, value);
    return result;
}

NN_FORCEINLINE
Vector4f& Vector4f::operator += (const Vector4f& value) NN_NOEXCEPT
{
    VectorAdd(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector4f& Vector4f::operator -= (const Vector4f& value) NN_NOEXCEPT
{
    VectorSubtract(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector4f& Vector4f::operator *= (float value) NN_NOEXCEPT
{
    VectorMultiply(this, *this, value);
    return *this;
}

NN_FORCEINLINE
Vector4f& Vector4f::operator /= (float value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(std::abs(value) > std::numeric_limits<float>::epsilon());
    VectorDivide(this, *this, value);
    return *this;
}

NN_FORCEINLINE
bool Vector4f::Normalize() NN_NOEXCEPT
{
    return VectorNormalize(this, *this) != 0.f;
}

NN_FORCEINLINE
float Vector4f::Dot(const Vector4f& value) const NN_NOEXCEPT
{
    return VectorDot(*this, value);
}

NN_FORCEINLINE
float Vector4f::Length() const NN_NOEXCEPT
{
    return VectorLength(*this);
}

NN_FORCEINLINE
float Vector4f::LengthSquared() const NN_NOEXCEPT
{
    return VectorLengthSquared(*this);
}

NN_FORCEINLINE
float Vector4f::Dot(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT
{
    return VectorDot(a, b);
}

NN_FORCEINLINE
float Vector4f::Distance(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT
{
    return VectorDistance(a, b);
}

NN_FORCEINLINE
float Vector4f::DistanceSquared(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT
{
    return VectorDistanceSquared(a, b);
}

NN_FORCEINLINE
Vector4f Vector4f::Normalize(const Vector4f& value) NN_NOEXCEPT
{
    Vector4f result;
    VectorNormalize(&result, value);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::Maximize(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT
{
    Vector4f result;
    VectorMaximize(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::Minimize(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT
{
    Vector4f result;
    VectorMinimize(&result, a, b);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::Lerp(const Vector4f& a, const Vector4f& b, float t) NN_NOEXCEPT
{
    Vector4f result;
    VectorLerp(&result, a, b, t);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::Transform(const Vector4f& value, const MatrixRowMajor4x4f& transform) NN_NOEXCEPT
{
    Vector4f result;
    VectorTransform(&result, value, transform);
    return result;
}

NN_FORCEINLINE
Vector4f Vector4f::Transform(const Vector3f& value, const MatrixRowMajor4x4f& transform) NN_NOEXCEPT
{
    Vector4f result;
    VectorTransform(&result, value, transform);
    return result;
}


NN_FORCEINLINE
const Vector4f& Vector4f::Zero() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector4f&>(ConstantZero);
}

NN_FORCEINLINE
const Vector4f& Vector4f::One() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector4f&>(ConstantOne);
}

NN_FORCEINLINE
const Vector4f& Vector4f::UnitX() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector4f&>(ConstantUnitX);
}

NN_FORCEINLINE
const Vector4f& Vector4f::UnitY() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector4f&>(ConstantUnitY);
}

NN_FORCEINLINE
const Vector4f& Vector4f::UnitZ() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector4f&>(ConstantUnitZ);
}

NN_FORCEINLINE
const Vector4f& Vector4f::UnitW() NN_NOEXCEPT
{
    // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
    return static_cast<const Vector4f&>(ConstantUnitW);
}

}
}
