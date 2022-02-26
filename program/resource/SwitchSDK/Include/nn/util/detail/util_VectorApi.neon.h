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
#include <nn/util/detail/util_MathTypes.neon.h>
#include <nn/util/detail/util_ConstantImpl.h>
#include <nn/util/detail/util_VectorApiDetail.neon.h>
#include <arm_neon.h>

//
// NEON を用いたベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::neon 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

//
// 初期化マクロ
//

#define NN_UTIL_NEON_VECTOR_2F_INITIALIZER(x, y) \
    { \
        { x, y } \
    }

#define NN_UTIL_NEON_VECTOR_3F_INITIALIZER(x, y, z) \
    { \
        { x, y, z, 0 } \
    }

#define NN_UTIL_NEON_VECTOR_4F_INITIALIZER(x, y, z, w) \
    { \
        { x, y, z, w } \
    }


namespace nn { namespace util { namespace neon {

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
NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

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
    Vector2fType vec = NN_UTIL_NEON_VECTOR_2F_INITIALIZER(x, y);
    return vec;
}

NN_FORCEINLINE Vector3fType MakeVector3fType(float x, float y, float z) NN_NOEXCEPT
{
    Vector3fType vec = NN_UTIL_NEON_VECTOR_3F_INITIALIZER(x, y, z);
    return vec;
}

NN_FORCEINLINE Vector4fType MakeVector4fType(float x, float y, float z, float w) NN_NOEXCEPT
{
    Vector4fType vec = NN_UTIL_NEON_VECTOR_4F_INITIALIZER(x, y, z, w);
    return vec;
}

//
// 読み込み
//
NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const float* pSource) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fLoad(pSource);
}

NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const Float2& source) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fLoad(source);
}

NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const float* pSource) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fLoad(pSource);
}

NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const Float3& source) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fLoad(source);
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Uint8x4& source) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fLoad(source);
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Unorm8x4& source) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fLoad(source);
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const float* pSource) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fLoad(pSource);
}

NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Float4& source) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fLoad(source);
}

//
// 書き出し
//
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector2fType& source) NN_NOEXCEPT
{
    detail::Vector2fStore(pOutValue, source._v);
}
NN_FORCEINLINE void       VectorStore(Float2* pOutValue, const Vector2fType& source) NN_NOEXCEPT
{
    detail::Vector2fStore(pOutValue->v, source._v);
}

NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector3fType& source) NN_NOEXCEPT
{
    detail::Vector3fStore(pOutValue, source._v);
}

NN_FORCEINLINE void       VectorStore(Float3* pOutValue, const Vector3fType& source) NN_NOEXCEPT
{
    detail::Vector3fStore(pOutValue->v, source._v);
}

NN_FORCEINLINE void       VectorStore(Uint8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    detail::Vector4fStore(pOutValue, source._v);
}

NN_FORCEINLINE void       VectorStore(Unorm8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    detail::Vector4fStore(pOutValue, source._v);
}

NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    detail::Vector4fStore(pOutValue, source._v);
}

NN_FORCEINLINE void       VectorStore(Float4* pOutValue, const Vector4fType& source) NN_NOEXCEPT
{
    detail::Vector4fStore(pOutValue->v, source._v);
}

//
// アクセサ
//
// 2次元ベクトル
NN_FORCEINLINE void       VectorSet(Vector2fType* pOutValue, float x, float y) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fSet(x, y);
}

NN_FORCEINLINE void       VectorSetX(Vector2fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fSetX(pOutValue->_v, value);
}

NN_FORCEINLINE void       VectorSetY(Vector2fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fSetY(pOutValue->_v, value);
}

NN_FORCEINLINE float      VectorGetX(const Vector2fType& vector) NN_NOEXCEPT
{
    return detail::Vector2fGetX(vector._v);
}

NN_FORCEINLINE float      VectorGetY(const Vector2fType& vector) NN_NOEXCEPT
{
    return detail::Vector2fGetY(vector._v);
}

// 3次元ベクトル
NN_FORCEINLINE void       VectorSet(Vector3fType* pOutValue, float x, float y, float z) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fSet(x, y, z);
}

NN_FORCEINLINE void       VectorSetX(Vector3fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fSetX(pOutValue->_v, value);
}

NN_FORCEINLINE void       VectorSetY(Vector3fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fSetY(pOutValue->_v, value);
}

NN_FORCEINLINE void       VectorSetZ(Vector3fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fSetZ(pOutValue->_v, value);
}

NN_FORCEINLINE float      VectorGetX(const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fGetX(vector._v);
}

NN_FORCEINLINE float      VectorGetY(const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fGetY(vector._v);
}

NN_FORCEINLINE float      VectorGetZ(const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fGetZ(vector._v);
}

// 4次元ベクトル
NN_FORCEINLINE void       VectorSet(Vector4fType* pOutValue, float x, float y, float z, float w) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE void       VectorSetX(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fSetX(pOutValue->_v, value);
}

NN_FORCEINLINE void       VectorSetY(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fSetY(pOutValue->_v, value);
}

NN_FORCEINLINE void       VectorSetZ(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fSetZ(pOutValue->_v, value);
}

NN_FORCEINLINE void       VectorSetW(Vector4fType* pOutValue, float value) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fSetW(pOutValue->_v, value);
}

NN_FORCEINLINE float      VectorGetX(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetX(vector._v);
}

NN_FORCEINLINE float      VectorGetY(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetY(vector._v);
}

NN_FORCEINLINE float      VectorGetZ(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetZ(vector._v);
}

NN_FORCEINLINE float      VectorGetW(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetW(vector._v);
}

//
// ベクトル
//
// 2次元ベクトル
NN_FORCEINLINE void       VectorZero(Vector2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fZero();
}

NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector) NN_NOEXCEPT
{
    return detail::Vector2fIsZero(vector._v);
}

NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector, float error) NN_NOEXCEPT
{
    return detail::Vector2fIsZero(vector._v, error);
}

NN_FORCEINLINE void       VectorAdd(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fAdd(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorSubtract(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fSubtract(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fMultiply(vector._v, factor);
}

NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fMultiply(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fDivide(vector._v, factor);
}

NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fDivide(vector1._v, vector2._v);
}

NN_FORCEINLINE float      VectorDot(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fDot(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorCross(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fCross(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorLength(const Vector2fType& vector) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fLength(vector._v));
}

NN_FORCEINLINE float      VectorLengthSquared(const Vector2fType& vector) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fLengthSquared(vector._v));
}

NN_FORCEINLINE float      VectorDistance(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fDistance(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorDistanceSquared(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fDistanceSquared(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorNormalize(Vector2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    return detail::Vector2fGetX(detail::Vector2fNormalize(&pOutValue->_v, vector._v));
}

NN_FORCEINLINE void       VectorMaximize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fMaximize(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorMinimize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fMinimize(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorLerp(Vector2fType* pOutValue, const Vector2fType& from, const Vector2fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fLerp(from._v, to._v, t);
}

// 3次元ベクトル
NN_FORCEINLINE void       VectorZero(Vector3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fZero();
}

NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fIsZero(vector._v);
}

NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector, float error) NN_NOEXCEPT
{
    return detail::Vector3fIsZero(vector._v, error);
}

NN_FORCEINLINE void       VectorAdd(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fAdd(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorSubtract(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fSubtract(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fMultiply(vector._v, factor);
}

NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fMultiply(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fDivide(vector._v, factor);
}

NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fDivide(vector1._v, vector2._v);
}

NN_FORCEINLINE float      VectorDot(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    return detail::Vector3fGetX(detail::Vector3fDot(vector1._v, vector2._v));
}

NN_FORCEINLINE void       VectorCross(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fCross(vector1._v, vector2._v);
}

NN_FORCEINLINE float      VectorLength(const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fGetX(detail::Vector3fLength(vector._v));
}

NN_FORCEINLINE float      VectorLengthSquared(const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fGetX(detail::Vector3fLengthSquared(vector._v));
}

NN_FORCEINLINE float      VectorDistance(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    return detail::Vector3fGetX(detail::Vector3fDistance(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorDistanceSquared(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    return detail::Vector3fGetX(detail::Vector3fDistanceSquared(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorNormalize(Vector3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    return detail::Vector3fGetX(detail::Vector3fNormalize(&pOutValue->_v, vector._v));
}

NN_FORCEINLINE void       VectorMaximize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fMaximize(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorMinimize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fMinimize(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorLerp(Vector3fType* pOutValue, const Vector3fType& from, const Vector3fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fLerp(from._v, to._v, t);
}

// 4次元ベクトル
NN_FORCEINLINE void       VectorZero(Vector4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fZero();
}

NN_FORCEINLINE bool       VectorIsZero(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fIsZero(vector._v);
}

NN_FORCEINLINE bool       VectorIsZeroWOne(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fIsZeroWOne(vector._v);
}

NN_FORCEINLINE void       VectorAdd(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fAdd(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorSubtract(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fSubtract(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fMultiply(vector._v, factor);
}

NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fMultiply(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fDivide(vector._v, factor);
}

NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fDivide(vector1._v, vector2._v);
}

NN_FORCEINLINE float      VectorDot(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::Vector4fDot(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorLength(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::Vector4fLength(vector._v));
}

NN_FORCEINLINE float      VectorLengthSquared(const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::Vector4fLengthSquared(vector._v));
}

NN_FORCEINLINE float      VectorDistance(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::Vector4fDistance(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorDistanceSquared(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::Vector4fDistanceSquared(vector1._v, vector2._v));
}

NN_FORCEINLINE float      VectorNormalize(Vector4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::Vector4fNormalize(&pOutValue->_v, vector._v));
}

NN_FORCEINLINE void       VectorMaximize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fMaximize(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorMinimize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fMinimize(vector1._v, vector2._v);
}

NN_FORCEINLINE void       VectorLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fLerp(from._v, to._v, t);
}

//
// ベクトル・行列ユーティリティ
//
// 2次元ベクトル
NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fTransformRowMajor3x2(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fTransformColumnMajor3x2(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fTransformNormalRowMajor3x2(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector2fTransformNormalColumnMajor3x2(vector._v, matrix._m);
}

// 3次元ベクトル

NN_FORCEINLINE void       VectorRotate(Vector3fType* pOutValue, const Vector3fType& vector, const Vector4fType& quaternion) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fRotate(vector._v, quaternion._v);
}

NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformRowMajor4x3(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformRowMajor4x4(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformColumnMajor4x3(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformColumnMajor4x4(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformNormalRowMajor4x3(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformNormalColumnMajor4x3(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformCoordRowMajor4x4(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector3fTransformCoordColumnMajor4x4(vector._v, matrix._m);
}

// 4次元ベクトル
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fTransformRowMajor4x4(vector._v, matrix._m);
}

NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::Vector4fTransformColumnMajor4x4(vector._v, matrix._m);
}

}}} // namespace nn::util::neon
