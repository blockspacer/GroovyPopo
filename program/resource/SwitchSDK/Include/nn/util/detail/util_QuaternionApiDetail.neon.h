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
#include <arm_neon.h>

namespace nn { namespace util { namespace neon { namespace detail {

NN_FORCEINLINE float32x4_t QuaternionIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionAdd(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionSubtract(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionDivide(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionMultiply(float32x4_t quaternion, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionMultiply(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionDot(float32x4_t quaternion1, float32x4_t quaternion2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionConjugate(float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionInverse(float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionNormalize(float32x4_t* pOutValue, float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionExp(float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionLogN(float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionLerp(float32x4_t from, float32x4_t to, float t) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionSlerp(float32x4_t from, float32x4_t to, float t) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionSquad(float32x4_t p, float32x4_t a, float32x4_t b, float32x4_t q, float t) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionMakeClosest(float32x4_t quaternion, float32x4_t quaternionTo) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionRotateAxis(float32x4_t axis, float radian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t QuaternionMakeVectorRotation(float32x4_t from, float32x4_t to) NN_NOEXCEPT;

}}}} // namespace nn::util::neon::detail

#include <nn/util/detail/util_QuaternionApiDetailImpl.neon.h>
