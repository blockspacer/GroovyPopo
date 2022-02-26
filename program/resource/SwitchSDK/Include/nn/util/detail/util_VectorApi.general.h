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
#include <nn/util/detail/util_MathTypes.general.h>
#include <nn/util/util_MathTypes.h>
#include <cmath>
#include <algorithm>

//
// SIMD ユニットを用いないベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::general 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

#define NN_UTIL_GENERAL_VECTOR_2F_INITIALIZER(x, y) \
    { \
        { x, y } \
    }

#define NN_UTIL_GENERAL_VECTOR_3F_INITIALIZER(x, y, z) \
    { \
        { x, y, z } \
    }

#define NN_UTIL_GENERAL_VECTOR_4F_INITIALIZER(x, y, z, w) \
    { \
        { x, y, z, w } \
    }

namespace nn { namespace util { namespace general {

// Vector2fType
NN_FORCEINLINE Vector2fType MakeVector2fType(float x, float y) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const float* pSource) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const Float2& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector2fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(Float2* pOutValue, const Vector2fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSet(Vector2fType* pOutValue, float x, float y) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetX(Vector2fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetY(Vector2fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetX(const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetY(const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorZero(Vector2fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector, float error) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorAdd(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSubtract(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDot(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorCross(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorLength(const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorLengthSquared(const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDistance(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDistanceSquared(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorNormalize(Vector2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMaximize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMinimize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLerp(Vector2fType* pOutValue, const Vector2fType& from, const Vector2fType& to, float t) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

// Vector3fType
NN_FORCEINLINE Vector3fType MakeVector3fType(float x, float y, float z) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const float* pSource) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const Float3& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(Float3* pOutValue, const Vector3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSet(Vector3fType* pOutValue, float x, float y, float z) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetX(Vector3fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetY(Vector3fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetZ(Vector3fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetX(const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetY(const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetZ(const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorZero(Vector3fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector, float error) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorAdd(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSubtract(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDot(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorCross(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorLength(const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorLengthSquared(const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDistance(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDistanceSquared(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorNormalize(Vector3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMaximize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMinimize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLerp(Vector3fType* pOutValue, const Vector3fType& from, const Vector3fType& to, float t) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorRotate(Vector3fType* pOutValue, const Vector3fType& vector, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

// Vector4fType
NN_FORCEINLINE Vector4fType MakeVector4fType(float x, float y, float z, float w) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Uint8x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Unorm8x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const float* pSource) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Float4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(Uint8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(Unorm8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorStore(Float4* pOutValue, const Vector4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSet(Vector4fType* pOutValue, float x, float y, float z, float w) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetX(Vector4fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetY(Vector4fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetZ(Vector4fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSetW(Vector4fType* pOutValue, float value) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetX(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetY(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetZ(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorGetW(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorZero(Vector4fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE bool       VectorIsZero(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE bool       VectorIsZeroWOne(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorAdd(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorSubtract(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDot(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorLength(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorLengthSquared(const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDistance(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorDistanceSquared(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE float      VectorNormalize(Vector4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMaximize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorMinimize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;


//
// 初期化
//
NN_FORCEINLINE Vector2fType MakeVector2fType(float x, float y) NN_NOEXCEPT
{
    Vector2fType vec = NN_UTIL_GENERAL_VECTOR_2F_INITIALIZER(x, y);
    return vec;
}

NN_FORCEINLINE Vector3fType MakeVector3fType(float x, float y, float z) NN_NOEXCEPT
{
    Vector3fType vec = NN_UTIL_GENERAL_VECTOR_3F_INITIALIZER(x, y, z);
    return vec;
}

NN_FORCEINLINE Vector4fType MakeVector4fType(float x, float y, float z, float w) NN_NOEXCEPT
{
    Vector4fType vec = NN_UTIL_GENERAL_VECTOR_4F_INITIALIZER(x, y, z, w);
    return vec;
}

//
// 読み込み
//
NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const float* pSource) NN_NOEXCEPT
{
    pOutValue->_v[0] = pSource[0];
    pOutValue->_v[1] = pSource[1];
}

NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const float* pSource) NN_NOEXCEPT
{
    pOutValue->_v[0] = pSource[0];
    pOutValue->_v[1] = pSource[1];
    pOutValue->_v[2] = pSource[2];
}

NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const Float2& source) NN_NOEXCEPT
{
    pOutValue->_v[0] = source.v[0];
    pOutValue->_v[1] = source.v[1];
}

NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const Float3& source) NN_NOEXCEPT
{
    pOutValue->_v[0] = source.v[0];
    pOutValue->_v[1] = source.v[1];
    pOutValue->_v[2] = source.v[2];
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Uint8x4& source) NN_NOEXCEPT
{
    pOutValue->_v[0] = static_cast<float>(source.v[0]);
    pOutValue->_v[1] = static_cast<float>(source.v[1]);
    pOutValue->_v[2] = static_cast<float>(source.v[2]);
    pOutValue->_v[3] = static_cast<float>(source.v[3]);
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Unorm8x4& source) NN_NOEXCEPT
{
    const float invMax = 1.0f / 255.0f;
    pOutValue->_v[0] = source.v[0] * invMax;
    pOutValue->_v[1] = source.v[1] * invMax;
    pOutValue->_v[2] = source.v[2] * invMax;
    pOutValue->_v[3] = source.v[3] * invMax;
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const float* pSource) NN_NOEXCEPT
{
    pOutValue->_v[0] = pSource[0];
    pOutValue->_v[1] = pSource[1];
    pOutValue->_v[2] = pSource[2];
    pOutValue->_v[3] = pSource[3];
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Float4& source) NN_NOEXCEPT
{
    pOutValue->_v[0] = source.v[0];
    pOutValue->_v[1] = source.v[1];
    pOutValue->_v[2] = source.v[2];
    pOutValue->_v[3] = source.v[3];
}

//
// 書き出し
//
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector2fType& source) NN_NOEXCEPT
{
    pOutValue[0] = source._v[0];
    pOutValue[1] = source._v[1];
}

NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector3fType& source) NN_NOEXCEPT
{
    pOutValue[0] = source._v[0];
    pOutValue[1] = source._v[1];
    pOutValue[2] = source._v[2];
}

NN_FORCEINLINE void       VectorStore(Float2* pOutValue, const Vector2fType& source) NN_NOEXCEPT
{
    pOutValue->v[0] = source._v[0];
    pOutValue->v[1] = source._v[1];
}

NN_FORCEINLINE void       VectorStore(Float3* pOutValue, const Vector3fType& source) NN_NOEXCEPT
{
    pOutValue->v[0] = source._v[0];
    pOutValue->v[1] = source._v[1];
    pOutValue->v[2] = source._v[2];
}

NN_FORCEINLINE void       VectorStore(Uint8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    pOutValue->v[0] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[0], 255.f), 0.f) + 0.5f));
    pOutValue->v[1] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[1], 255.f), 0.f) + 0.5f));
    pOutValue->v[2] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[2], 255.f), 0.f) + 0.5f));
    pOutValue->v[3] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[3], 255.f), 0.f) + 0.5f));
}

NN_FORCEINLINE void       VectorStore(Unorm8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    pOutValue->v[0] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[0], 1.f), 0.f) * 255.0f + 0.5f));
    pOutValue->v[1] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[1], 1.f), 0.f) * 255.0f + 0.5f));
    pOutValue->v[2] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[2], 1.f), 0.f) * 255.0f + 0.5f));
    pOutValue->v[3] = static_cast<uint8_t>(::std::floor(::std::max(::std::min(source._v[3], 1.f), 0.f) * 255.0f + 0.5f));
}

NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    pOutValue[0] = source._v[0];
    pOutValue[1] = source._v[1];
    pOutValue[2] = source._v[2];
    pOutValue[3] = source._v[3];
}

NN_FORCEINLINE void       VectorStore(Float4* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    pOutValue->v[0] = source._v[0];
    pOutValue->v[1] = source._v[1];
    pOutValue->v[2] = source._v[2];
    pOutValue->v[3] = source._v[3];
}

//
// アクセサ
//

// 2次元ベクトル
NN_FORCEINLINE void       VectorSet(Vector2fType* pOutValue, float x, float y) NN_NOEXCEPT
{
    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
}

NN_FORCEINLINE void       VectorSetX(Vector2fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[0] = value;
}

NN_FORCEINLINE void       VectorSetY(Vector2fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[1] = value;
}

NN_FORCEINLINE float      VectorGetX(const Vector2fType& vector) NN_NOEXCEPT
{
    return vector._v[0];
}

NN_FORCEINLINE float      VectorGetY(const Vector2fType& vector) NN_NOEXCEPT
{
    return vector._v[1];
}

// 3次元ベクトル
NN_FORCEINLINE void       VectorSet(Vector3fType* pOutValue, float x, float y, float z) NN_NOEXCEPT
{
    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
}

NN_FORCEINLINE void       VectorSetX(Vector3fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[0] = value;
}

NN_FORCEINLINE void       VectorSetY(Vector3fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[1] = value;
}

NN_FORCEINLINE void       VectorSetZ(Vector3fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[2] = value;
}

NN_FORCEINLINE float      VectorGetX(const Vector3fType& vector) NN_NOEXCEPT
{
    return vector._v[0];
}

NN_FORCEINLINE float      VectorGetY(const Vector3fType& vector) NN_NOEXCEPT
{
    return vector._v[1];
}

NN_FORCEINLINE float      VectorGetZ(const Vector3fType& vector) NN_NOEXCEPT
{
    return vector._v[2];
}

// 4次元ベクトル
NN_FORCEINLINE void       VectorSet(Vector4fType* pOutValue, float x, float y, float z, float w) NN_NOEXCEPT
{
    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE void       VectorSetX(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[0] = value;
}

NN_FORCEINLINE void       VectorSetY(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[1] = value;
}

NN_FORCEINLINE void       VectorSetZ(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[2] = value;
}

NN_FORCEINLINE void       VectorSetW(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v[3] = value;
}

NN_FORCEINLINE float      VectorGetX(const Vector4fType& vector) NN_NOEXCEPT
{
    return vector._v[0];
}

NN_FORCEINLINE float      VectorGetY(const Vector4fType& vector) NN_NOEXCEPT
{
    return vector._v[1];
}

NN_FORCEINLINE float      VectorGetZ(const Vector4fType& vector) NN_NOEXCEPT
{
    return vector._v[2];
}

NN_FORCEINLINE float      VectorGetW(const Vector4fType& vector) NN_NOEXCEPT
{
    return vector._v[3];
}

//
// ベクトル
//

// 2次元ベクトル
NN_FORCEINLINE void       VectorZero(Vector2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v[0] = 0.f;
    pOutValue->_v[1] = 0.f;
}

NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector) NN_NOEXCEPT
{
    return vector._v[0] == 0.f && vector._v[1] == 0.f;
}

NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector, float error) NN_NOEXCEPT
{
    return (-error <= vector._v[0] && vector._v[0] <= error)
        && (-error <= vector._v[1] && vector._v[1] <= error);
}

NN_FORCEINLINE void       VectorAdd(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] + vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] + vector2._v[1];
}

NN_FORCEINLINE void       VectorSubtract(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] - vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] - vector2._v[1];
}

NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector._v[0] * factor;
    pOutValue->_v[1] = vector._v[1] * factor;
}

NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] * vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] * vector2._v[1];
}

NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT
{
    float recprocalFactor = 1.f / factor;

    pOutValue->_v[0] = vector._v[0] * recprocalFactor;
    pOutValue->_v[1] = vector._v[1] * recprocalFactor;
}

NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] / vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] / vector2._v[1];
}

NN_FORCEINLINE float      VectorDot(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return (vector1._v[0] * vector2._v[0]) + (vector1._v[1] * vector2._v[1]);
}

NN_FORCEINLINE float      VectorCross(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return (vector1._v[0] * vector2._v[1]) - (vector1._v[1] * vector2._v[0]);
}

NN_FORCEINLINE float      VectorLength(const Vector2fType& vector) NN_NOEXCEPT
{
    return ::std::sqrt(VectorLengthSquared(vector));
}

NN_FORCEINLINE float      VectorLengthSquared(const Vector2fType& vector) NN_NOEXCEPT
{
    return (vector._v[0] * vector._v[0]) + (vector._v[1] * vector._v[1]);
}

NN_FORCEINLINE float      VectorDistance(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return ::std::sqrt(VectorDistanceSquared(vector1, vector2));
}

NN_FORCEINLINE float      VectorDistanceSquared(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    float x = vector1._v[0] - vector2._v[0];
    float y = vector1._v[1] - vector2._v[1];

    return (x * x) + (y * y);
}

NN_FORCEINLINE float      VectorNormalize(Vector2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    float lengthSquared = VectorLengthSquared(vector);

    if (lengthSquared > 0.0f)
    {
        float reciprocalMagnitude = 1.f / ::std::sqrt(lengthSquared);

        pOutValue->_v[0] = vector._v[0] * reciprocalMagnitude;
        pOutValue->_v[1] = vector._v[1] * reciprocalMagnitude;
    }
    else
    {
        VectorZero(pOutValue);
    }

    return lengthSquared;
}

NN_FORCEINLINE void       VectorMaximize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = ::std::max(vector1._v[0], vector2._v[0]);
    pOutValue->_v[1] = ::std::max(vector1._v[1], vector2._v[1]);
}

NN_FORCEINLINE void       VectorMinimize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = ::std::min(vector1._v[0], vector2._v[0]);
    pOutValue->_v[1] = ::std::min(vector1._v[1], vector2._v[1]);
}

NN_FORCEINLINE void       VectorLerp(Vector2fType* pOutValue, const Vector2fType& from, const Vector2fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v[0] = from._v[0] + t * (to._v[0] - from._v[0]);
    pOutValue->_v[1] = from._v[1] + t * (to._v[1] - from._v[1]);
}

// 3次元ベクトル
NN_FORCEINLINE void       VectorZero(Vector3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v[0] = 0.f;
    pOutValue->_v[1] = 0.f;
    pOutValue->_v[2] = 0.f;
}

NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector) NN_NOEXCEPT
{
    return vector._v[0] == 0.f && vector._v[1] == 0.f && vector._v[2] == 0.f;
}

NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector, float error) NN_NOEXCEPT
{
    return (-error <= vector._v[0] && vector._v[0] <= error)
        && (-error <= vector._v[1] && vector._v[1] <= error)
        && (-error <= vector._v[2] && vector._v[2] <= error);
}

NN_FORCEINLINE void       VectorAdd(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] + vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] + vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] + vector2._v[2];
}

NN_FORCEINLINE void       VectorSubtract(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] - vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] - vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] - vector2._v[2];
}

NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector._v[0] * factor;
    pOutValue->_v[1] = vector._v[1] * factor;
    pOutValue->_v[2] = vector._v[2] * factor;
}

NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] * vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] * vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] * vector2._v[2];
}

NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT
{
    float recprocalFactor = 1.f / factor;

    pOutValue->_v[0] = vector._v[0] * recprocalFactor;
    pOutValue->_v[1] = vector._v[1] * recprocalFactor;
    pOutValue->_v[2] = vector._v[2] * recprocalFactor;
}

NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] / vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] / vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] / vector2._v[2];
}

NN_FORCEINLINE float      VectorDot(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    return (vector1._v[0] * vector2._v[0]) + (vector1._v[1] * vector2._v[1]) + (vector1._v[2] * vector2._v[2]);
}

NN_FORCEINLINE void       VectorCross(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    float x = (vector1._v[1] * vector2._v[2]) - (vector1._v[2] * vector2._v[1]);
    float y = (vector1._v[2] * vector2._v[0]) - (vector1._v[0] * vector2._v[2]);
    float z = (vector1._v[0] * vector2._v[1]) - (vector1._v[1] * vector2._v[0]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
}

NN_FORCEINLINE float      VectorLength(const Vector3fType& vector) NN_NOEXCEPT
{
    return ::std::sqrt(VectorLengthSquared(vector));
}

NN_FORCEINLINE float      VectorLengthSquared(const Vector3fType& vector) NN_NOEXCEPT
{
    return (vector._v[0] * vector._v[0]) + (vector._v[1] * vector._v[1]) + (vector._v[2] * vector._v[2]);
}

NN_FORCEINLINE float      VectorDistance(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    return ::std::sqrt(VectorDistanceSquared(vector1, vector2));
}

NN_FORCEINLINE float      VectorDistanceSquared(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    float x = vector1._v[0] - vector2._v[0];
    float y = vector1._v[1] - vector2._v[1];
    float z = vector1._v[2] - vector2._v[2];

    return (x * x) + (y * y) + (z * z);
}

NN_FORCEINLINE float      VectorNormalize(Vector3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    float lengthSquared = VectorLengthSquared(vector);

    if ( lengthSquared > 0.0f )
    {
        float reciprocalMagnitude = 1.f / ::std::sqrt(lengthSquared);

        pOutValue->_v[0] = vector._v[0] * reciprocalMagnitude;
        pOutValue->_v[1] = vector._v[1] * reciprocalMagnitude;
        pOutValue->_v[2] = vector._v[2] * reciprocalMagnitude;
    }
    else
    {
        VectorZero(pOutValue);
    }

    return lengthSquared;
}

NN_FORCEINLINE void       VectorMaximize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = ::std::max(vector1._v[0], vector2._v[0]);
    pOutValue->_v[1] = ::std::max(vector1._v[1], vector2._v[1]);
    pOutValue->_v[2] = ::std::max(vector1._v[2], vector2._v[2]);
}

NN_FORCEINLINE void       VectorMinimize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = ::std::min(vector1._v[0], vector2._v[0]);
    pOutValue->_v[1] = ::std::min(vector1._v[1], vector2._v[1]);
    pOutValue->_v[2] = ::std::min(vector1._v[2], vector2._v[2]);
}

NN_FORCEINLINE void       VectorLerp(Vector3fType* pOutValue, const Vector3fType& from, const Vector3fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v[0] = from._v[0] + t * (to._v[0] - from._v[0]);
    pOutValue->_v[1] = from._v[1] + t * (to._v[1] - from._v[1]);
    pOutValue->_v[2] = from._v[2] + t * (to._v[2] - from._v[2]);
}

// 4次元ベクトル
NN_FORCEINLINE void       VectorZero(Vector4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v[0] = 0.f;
    pOutValue->_v[1] = 0.f;
    pOutValue->_v[2] = 0.f;
    pOutValue->_v[3] = 0.f;
}

NN_FORCEINLINE bool       VectorIsZero(const Vector4fType& vector) NN_NOEXCEPT
{
    return vector._v[0] == 0.f && vector._v[1] == 0.f && vector._v[2] == 0.f && vector._v[3] == 0.f;
}

NN_FORCEINLINE bool       VectorIsZeroWOne(const Vector4fType& vector) NN_NOEXCEPT
{
    return vector._v[0] == 0.f && vector._v[1] == 0.f && vector._v[2] == 0.f && vector._v[3] == 1.f;
}

NN_FORCEINLINE void       VectorAdd(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] + vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] + vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] + vector2._v[2];
    pOutValue->_v[3] = vector1._v[3] + vector2._v[3];
}

NN_FORCEINLINE void       VectorSubtract(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] - vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] - vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] - vector2._v[2];
    pOutValue->_v[3] = vector1._v[3] - vector2._v[3];
}

NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector._v[0] * factor;
    pOutValue->_v[1] = vector._v[1] * factor;
    pOutValue->_v[2] = vector._v[2] * factor;
    pOutValue->_v[3] = vector._v[3] * factor;
}

NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] * vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] * vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] * vector2._v[2];
    pOutValue->_v[3] = vector1._v[3] * vector2._v[3];
}

NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT
{
    float reciprocalFactor = 1.f / factor;

    pOutValue->_v[0] = vector._v[0] * reciprocalFactor;
    pOutValue->_v[1] = vector._v[1] * reciprocalFactor;
    pOutValue->_v[2] = vector._v[2] * reciprocalFactor;
    pOutValue->_v[3] = vector._v[3] * reciprocalFactor;
}

NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = vector1._v[0] / vector2._v[0];
    pOutValue->_v[1] = vector1._v[1] / vector2._v[1];
    pOutValue->_v[2] = vector1._v[2] / vector2._v[2];
    pOutValue->_v[3] = vector1._v[3] / vector2._v[3];
}

NN_FORCEINLINE float      VectorDot(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    return (vector1._v[0] * vector2._v[0]) + (vector1._v[1] * vector2._v[1]) + (vector1._v[2] * vector2._v[2]) + (vector1._v[3] * vector2._v[3]);
}

NN_FORCEINLINE float      VectorLength(const Vector4fType& vector) NN_NOEXCEPT
{
    return ::std::sqrt(VectorLengthSquared(vector));
}

NN_FORCEINLINE float      VectorLengthSquared(const Vector4fType& vector) NN_NOEXCEPT
{
    return (vector._v[0] * vector._v[0]) + (vector._v[1] * vector._v[1]) + (vector._v[2] * vector._v[2]) + (vector._v[3] * vector._v[3]);
}

NN_FORCEINLINE float      VectorDistance(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    return ::std::sqrt(VectorDistanceSquared(vector1, vector2));
}

NN_FORCEINLINE float      VectorDistanceSquared(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    float x = vector1._v[0] - vector2._v[0];
    float y = vector1._v[1] - vector2._v[1];
    float z = vector1._v[2] - vector2._v[2];
    float w = vector1._v[3] - vector2._v[3];

    return (x * x) + (y * y) + (z * z) + (w * w);
}

NN_FORCEINLINE float      VectorNormalize(Vector4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    float lengthSquared = VectorLengthSquared(vector);

    if ( lengthSquared > 0.0f )
    {
        float reciprocalMagnitude = 1.f / ::std::sqrt(lengthSquared);

        pOutValue->_v[0] = vector._v[0] * reciprocalMagnitude;
        pOutValue->_v[1] = vector._v[1] * reciprocalMagnitude;
        pOutValue->_v[2] = vector._v[2] * reciprocalMagnitude;
        pOutValue->_v[3] = vector._v[3] * reciprocalMagnitude;
    }
    else
    {
        VectorZero(pOutValue);
    }

    return lengthSquared;
}

NN_FORCEINLINE void       VectorMaximize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = ::std::max(vector1._v[0], vector2._v[0]);
    pOutValue->_v[1] = ::std::max(vector1._v[1], vector2._v[1]);
    pOutValue->_v[2] = ::std::max(vector1._v[2], vector2._v[2]);
    pOutValue->_v[3] = ::std::max(vector1._v[3], vector2._v[3]);
}

NN_FORCEINLINE void       VectorMinimize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v[0] = ::std::min(vector1._v[0], vector2._v[0]);
    pOutValue->_v[1] = ::std::min(vector1._v[1], vector2._v[1]);
    pOutValue->_v[2] = ::std::min(vector1._v[2], vector2._v[2]);
    pOutValue->_v[3] = ::std::min(vector1._v[3], vector2._v[3]);
}

NN_FORCEINLINE void       VectorLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v[0] = from._v[0] + t * (to._v[0] - from._v[0]);
    pOutValue->_v[1] = from._v[1] + t * (to._v[1] - from._v[1]);
    pOutValue->_v[2] = from._v[2] + t * (to._v[2] - from._v[2]);
    pOutValue->_v[3] = from._v[3] + t * (to._v[3] - from._v[3]);
}

//
// ベクトル・行列ユーティリティ
//
// 2次元ベクトル
NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + matrix._m[2][0];
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + matrix._m[2][1];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + matrix._m[3][0];
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + matrix._m[3][1];
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + matrix._m[3][2];
    float w = (vector._v[0] * matrix._m[0][3]) + (vector._v[1] * matrix._m[1][3]) + matrix._m[3][3];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + matrix._m[0][2];
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + matrix._m[1][2];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + matrix._m[0][3];
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + matrix._m[1][3];
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + matrix._m[2][3];
    float w = (vector._v[0] * matrix._m[3][0]) + (vector._v[1] * matrix._m[3][1]) + matrix._m[3][3];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]);
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
}

NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]);
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
}

NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + matrix._m[3][0];
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + matrix._m[3][1];
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + matrix._m[3][2];
    float w = (vector._v[0] * matrix._m[0][3]) + (vector._v[1] * matrix._m[1][3]) + matrix._m[3][3];
    float reciprocalW = 1.f / w;

    pOutValue->_v[0] = x * reciprocalW;
    pOutValue->_v[1] = y * reciprocalW;
    pOutValue->_v[2] = z * reciprocalW;
}

NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + matrix._m[0][3];
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + matrix._m[1][3];
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + matrix._m[2][3];
    float w = (vector._v[0] * matrix._m[3][0]) + (vector._v[1] * matrix._m[3][1]) + matrix._m[3][3];
    float reciprocalW = 1.f / w;

    pOutValue->_v[0] = x * reciprocalW;
    pOutValue->_v[1] = y * reciprocalW;
    pOutValue->_v[2] = z * reciprocalW;
}

// 3次元ベクトル
NN_FORCEINLINE void       VectorRotate(Vector3fType* pOutValue, const Vector3fType& vector, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float quatX = quaternion._v[0];
    float quatY = quaternion._v[1];
    float quatZ = quaternion._v[2];
    float quatW = quaternion._v[3];
    float vecX = vector._v[0];
    float vecY = vector._v[1];
    float vecZ = vector._v[2];

    float tmpX = +vecX * quatW - vecY * quatZ + vecZ * quatY;
    float tmpY = +vecX * quatZ + vecY * quatW - vecZ * quatX;
    float tmpZ = -vecX * quatY + vecY * quatX + vecZ * quatW;
    float tmpW = +vecX * quatX + vecY * quatY + vecZ * quatZ;

    pOutValue->_v[0] = quatW * tmpX + quatX * tmpW + quatY * tmpZ - quatZ * tmpY;
    pOutValue->_v[1] = quatW * tmpY - quatX * tmpZ + quatY * tmpW + quatZ * tmpX;
    pOutValue->_v[2] = quatW * tmpZ + quatX * tmpY - quatY * tmpX + quatZ * tmpW;
}

NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + (vector._v[2] * matrix._m[2][0]) + matrix._m[3][0];
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[2][1]) + matrix._m[3][1];
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + (vector._v[2] * matrix._m[2][2]) + matrix._m[3][2];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + (vector._v[2] * matrix._m[2][0]) + matrix._m[3][0];
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[2][1]) + matrix._m[3][1];
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + (vector._v[2] * matrix._m[2][2]) + matrix._m[3][2];
    float w = (vector._v[0] * matrix._m[0][3]) + (vector._v[1] * matrix._m[1][3]) + (vector._v[2] * matrix._m[2][3]) + matrix._m[3][3];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + (vector._v[2] * matrix._m[0][2]) + matrix._m[0][3];
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[1][2]) + matrix._m[1][3];
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + (vector._v[2] * matrix._m[2][2]) + matrix._m[2][3];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + (vector._v[2] * matrix._m[0][2]) + matrix._m[0][3];
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[1][2]) + matrix._m[1][3];
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + (vector._v[2] * matrix._m[2][2]) + matrix._m[2][3];
    float w = (vector._v[0] * matrix._m[3][0]) + (vector._v[1] * matrix._m[3][1]) + (vector._v[2] * matrix._m[3][2]) + matrix._m[3][3];

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + (vector._v[2] * matrix._m[2][0]);
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[2][1]);
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + (vector._v[2] * matrix._m[2][2]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
}

NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + (vector._v[2] * matrix._m[0][2]);
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[1][2]);
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + (vector._v[2] * matrix._m[2][2]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
}

NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + (vector._v[2] * matrix._m[2][0]) + matrix._m[3][0];
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[2][1]) + matrix._m[3][1];
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + (vector._v[2] * matrix._m[2][2]) + matrix._m[3][2];
    float w = (vector._v[0] * matrix._m[0][3]) + (vector._v[1] * matrix._m[1][3]) + (vector._v[2] * matrix._m[2][3]) + matrix._m[3][3];
    float reciprocalW = 1.f / w;

    pOutValue->_v[0] = x * reciprocalW;
    pOutValue->_v[1] = y * reciprocalW;
    pOutValue->_v[2] = z * reciprocalW;
}

NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + (vector._v[2] * matrix._m[0][2]) + matrix._m[0][3];
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[1][2]) + matrix._m[1][3];
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + (vector._v[2] * matrix._m[2][2]) + matrix._m[2][3];
    float w = (vector._v[0] * matrix._m[3][0]) + (vector._v[1] * matrix._m[3][1]) + (vector._v[2] * matrix._m[3][2]) + matrix._m[3][3];
    float reciprocalW = 1.f / w;

    pOutValue->_v[0] = x * reciprocalW;
    pOutValue->_v[1] = y * reciprocalW;
    pOutValue->_v[2] = z * reciprocalW;
}

// 4次元ベクトル
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[1][0]) + (vector._v[2] * matrix._m[2][0]) + (vector._v[3] * matrix._m[3][0]);
    float y = (vector._v[0] * matrix._m[0][1]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[2][1]) + (vector._v[3] * matrix._m[3][1]);
    float z = (vector._v[0] * matrix._m[0][2]) + (vector._v[1] * matrix._m[1][2]) + (vector._v[2] * matrix._m[2][2]) + (vector._v[3] * matrix._m[3][2]);
    float w = (vector._v[0] * matrix._m[0][3]) + (vector._v[1] * matrix._m[1][3]) + (vector._v[2] * matrix._m[2][3]) + (vector._v[3] * matrix._m[3][3]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    float x = (vector._v[0] * matrix._m[0][0]) + (vector._v[1] * matrix._m[0][1]) + (vector._v[2] * matrix._m[0][2]) + (vector._v[3] * matrix._m[0][3]);
    float y = (vector._v[0] * matrix._m[1][0]) + (vector._v[1] * matrix._m[1][1]) + (vector._v[2] * matrix._m[1][2]) + (vector._v[3] * matrix._m[1][3]);
    float z = (vector._v[0] * matrix._m[2][0]) + (vector._v[1] * matrix._m[2][1]) + (vector._v[2] * matrix._m[2][2]) + (vector._v[3] * matrix._m[2][3]);
    float w = (vector._v[0] * matrix._m[3][0]) + (vector._v[1] * matrix._m[3][1]) + (vector._v[2] * matrix._m[3][2]) + (vector._v[3] * matrix._m[3][3]);

    pOutValue->_v[0] = x;
    pOutValue->_v[1] = y;
    pOutValue->_v[2] = z;
    pOutValue->_v[3] = w;
}

}}} // namespace nn::util::general
