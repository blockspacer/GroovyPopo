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
#include <nn/util/detail/util_VectorApi.neon.h>
#include <nn/util/detail/util_ConstantImpl.h>
#include <nn/util/detail/util_QuaternionApiDetail.neon.h>
#include <cmath>
#include <arm_neon.h>

//
// NEON を用いたベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::neon 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

namespace nn { namespace util { namespace neon {

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
    pOutValue->_v = detail::QuaternionFromMatrixRowMajor4x3f(matrix._m);
}

NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionFromMatrixColumnMajor4x3f(matrix._m);
}

//
// クオータニオン
//

NN_FORCEINLINE void       QuaternionIdentity(Vector4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionIdentity();
}

NN_FORCEINLINE void       QuaternionAdd(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionAdd(quaternion1._v, quaternion2._v);
}

NN_FORCEINLINE void       QuaternionSubtract(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionSubtract(quaternion1._v, quaternion2._v);
}

NN_FORCEINLINE void       QuaternionDivide(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionDivide(quaternion1._v, quaternion2._v);
}

NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion, float factor) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionMultiply(quaternion._v, factor);
}

NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionMultiply(quaternion1._v, quaternion2._v);
}

NN_FORCEINLINE float      QuaternionDot(const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::QuaternionDot(quaternion1._v, quaternion2._v));
}

NN_FORCEINLINE void       QuaternionInverse(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionInverse(quaternion._v);
}

NN_FORCEINLINE float      QuaternionNormalize(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    return detail::Vector4fGetX(detail::QuaternionNormalize(&pOutValue->_v, quaternion._v));
}

NN_FORCEINLINE void       QuaternionExp(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionExp(quaternion._v);
}

NN_FORCEINLINE void       QuaternionLogN(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionLogN(quaternion._v);
}

NN_FORCEINLINE void       QuaternionLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionLerp(from._v, to._v, t);
}

NN_FORCEINLINE void       QuaternionSlerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionSlerp(from._v, to._v, t);
}

NN_FORCEINLINE void       QuaternionSquad(Vector4fType* pOutValue, const Vector4fType& p, const Vector4fType& a, const Vector4fType& b, const Vector4fType& q, float t) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionSquad(p._v, a._v, b._v, q._v, t);
}

NN_FORCEINLINE void       QuaternionMakeClosest(Vector4fType*  pOutValue, const Vector4fType& quaternion, const Vector4fType& quaternionTo) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionMakeClosest(quaternion._v, quaternionTo._v);
}

NN_FORCEINLINE void       QuaternionRotateAxis(Vector4fType* pOutValue, const Vector3fType& axis, float radian) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionRotateAxis(axis._v, radian);
}

NN_FORCEINLINE void       QuaternionMakeVectorRotation(Vector4fType* pOutValue, const Vector3fType& from, const Vector3fType& to) NN_NOEXCEPT
{
    pOutValue->_v = detail::QuaternionMakeVectorRotation(from._v, to._v);
}

}}} // namespace nn::util::neon
