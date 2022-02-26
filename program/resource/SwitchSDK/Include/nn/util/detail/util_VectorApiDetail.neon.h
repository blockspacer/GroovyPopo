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
#include <arm_neon.h>

namespace nn { namespace util { namespace neon { namespace detail {

const float32x4_t Vector4fValueOne = { 1.f, 1.f, 1.f, 1.f };
const float32x4_t Vector4fValueOneWZero = { 1.f, 1.f, 1.f, 0.f };
const float32x4_t Vector4fValueNegativeOne = { -1.f, -1.f, -1.f, -1.f };
const float32x4_t Vector4fValueOneWNegativeOne = { 1.f, 1.f, 1.f, -1.f };
const float32x4_t Vector4fValueNegativeOneWOne = { -1.f, -1.f, -1.f, 1.f };
const float32x4_t Vector4fValueIdentityAxisX = { 1.f, 0.f, 0.f, 0.f };
const float32x4_t Vector4fValueIdentityAxisY = { 0.f, 1.f, 0.f, 0.f };
const float32x4_t Vector4fValueIdentityAxisZ = { 0.f, 0.f, 1.f, 0.f };
const float32x4_t Vector4fValueIdentityAxisW = { 0.f, 0.f, 0.f, 1.f };
const float32x4_t Vector4fValueNegativeIdentityAxisX = { -1.f, 0.f, 0.f, 0.f };
const float32x4_t Vector4fValueNegativeIdentityAxisY = { 0.f, -1.f, 0.f, 0.f };
const float32x4_t Vector4fValueNegativeIdentityAxisZ = { 0.f, 0.f, -1.f, 0.f };
const float32x4_t Vector4fValueNegativeIdentityAxisW = { 0.f, 0.f, 0.f, -1.f };
const float32x4_t Vector4fValueR8G8B8A8 = { 255.f, 255.f, 255.f, 255.f };
const float32x4_t Vector4fValue0Point5 = { 0.5f, 0.5f, 0.5f, 0.5f };

const float32x2_t Vector2fValueOne = { 1.f, 1.f };
const float32x2_t Vector2fValueNegativeOne = { -1.f, -1.f };
const float32x2_t Vector2fValueIdentityAxisX = { 1.f, 0.f };
const float32x2_t Vector2fValueIdentityAxisY = { 0.f, 1.f };
const float32x2_t Vector2fValueNegateX = { -1.f, 1.f };
const float32x2_t Vector2fValueNegateY = { 1.f, -1.f };

const uint32x4_t Vector4uValueSelect1000 = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
const uint32x4_t Vector4uValueSelect0100 = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
const uint32x4_t Vector4uValueSelect0010 = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };
const uint32x4_t Vector4uValueSelect0001 = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
const uint32x4_t Vector4uValueSelect0101 = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF };
const uint32x4_t Vector4uValueSelect1010 = { 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000 };
const uint32x4_t Vector4uValueSelect0110 = { 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
const uint32x4_t Vector4uValueSelect0111 = { 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
const uint32x4_t Vector4uValueSelect1011 = { 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF };
const uint32x4_t Vector4uValueSelect1101 = { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF };
const uint32x4_t Vector4uValueSelect1110 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
const uint32x4_t Vector4uValueX1Y1Z1W0 = { 1, 1, 1, 0 };
const uint32x4_t Vector4uValueMaskX = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
const uint32x4_t Vector4uValueMaskY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
const uint32x4_t Vector4uValueMaskZ = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };
const uint32x4_t Vector4uValueMaskW = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
const uint32x4_t Vector4uValueMaskXYZ = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };

const uint32x4_t Vector2uValueMaskX = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
const uint32x4_t Vector2uValueMaskY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };

const uint32_t SwizzleX = 0;
const uint32_t SwizzleY = 1;
const uint32_t SwizzleZ = 2;
const uint32_t SwizzleW = 3;

const uint32_t Permute0X = 0;
const uint32_t Permute0Y = 1;
const uint32_t Permute0Z = 2;
const uint32_t Permute0W = 3;
const uint32_t Permute1X = 4;
const uint32_t Permute1Y = 5;
const uint32_t Permute1Z = 6;
const uint32_t Permute1W = 7;

NN_FORCEINLINE float32x4_t Vector4fLoad(const Uint8x4& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fLoad(const Unorm8x4& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fLoad(const float* pSource) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fLoad(const Float4& source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector4fStore(Uint8x4* pOutValue, float32x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector4fStore(Unorm8x4* pOutValue, float32x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector4fStore(float32_t* pOutValue, float32x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector4fStore(Float4* pOutValue, float32x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSet(float32_t x, float32_t y, float32_t z, float32_t w) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSetElement(float32x4_t vector, int index, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSetX(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSetY(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSetZ(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSetW(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector4fGetElement(float32x4_t vector, int index) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector4fGetX(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector4fGetY(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector4fGetZ(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector4fGetW(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fZero() NN_NOEXCEPT;
NN_FORCEINLINE bool Vector4fEqual(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector4fIsZero(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector4fIsZero(float32x4_t vector, float32_t error) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector4fIsZeroWOne(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAdd(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSubtract(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiply(float32x4_t vector, float32_t factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiply(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplyAdd(float32x4_t vector1, float32x4_t vector2, float32x4_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplyAdd(float32x4_t vector1, float32x4_t vector2, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplyAddLane0(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplyAddLane1(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplySubtract(float32x4_t vector1, float32x4_t vector2, float32x4_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplySubtract(float32x4_t vector1, float32x4_t vector2, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplySubtractLane0(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMultiplySubtractLane1(float32x4_t vector1, float32x4_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fDivide(float32x4_t vector1, float32_t factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fDivide(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fDot(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fLength(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fLengthSquared(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fDistance(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fDistanceSquared(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fNormalize(float32x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMaximize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMinimize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fLerp(float32x4_t from, float32x4_t to, float32_t t) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fNegate(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fMask(float32x4_t vector, uint32x4_t mask) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSqrt(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fRecp(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fRsqrt(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAbs(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSign(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSelect(float32x4_t vector1, float32x4_t vector2, uint32x4_t select) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSwizzle(float32x4_t vector, uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fPermute(float32x4_t vector1, float32x4_t vector2, uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fRound(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fModPi(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fSin(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fCos(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void Vector4fSinCos(float32x4_t* pOutSin, float32x4_t* pOutCos, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fTan(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAsin(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAcos(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAtanImpl(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAtan(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fAtan2(float32x4_t vectorY, float32x4_t vectorX) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fTransformRowMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector4fTransformColumnMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;

NN_FORCEINLINE float32x4_t Vector3fLoad(const float* pSource) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fLoad(const Float3& source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector3fStore(float32_t* pOutValue, float32x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector3fStore(Float3* pOutValue, float32x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fSet(float32_t x, float32_t y, float32_t z) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fSetElement(float32x4_t vector, int index, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fSetX(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fSetY(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fSetZ(float32x4_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector3fGetElement(float32x4_t vector, int index) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector3fGetX(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector3fGetY(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector3fGetZ(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fZero() NN_NOEXCEPT;
NN_FORCEINLINE bool Vector3fEqual(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector3fIsZero(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector3fIsZero(float32x4_t vector, float32_t error) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fAdd(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fSubtract(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fMultiply(float32x4_t vector, float32_t factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fMultiply(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fDivide(float32x4_t vector, float32_t factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fDivide(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fDot(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fCross(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fLength(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fLengthSquared(float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fDistance(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fDistanceSquared(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fNormalize(float32x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fMaximize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fMinimize(float32x4_t vector1, float32x4_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fLerp(float32x4_t from, float32x4_t to, float32_t t) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fRotate(float32x4_t vector, float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformRowMajor4x3(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformRowMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformColumnMajor4x3(float32x4_t vector, float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformColumnMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformNormalRowMajor4x3(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformNormalColumnMajor4x3(float32x4_t vector, float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformCoordRowMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t Vector3fTransformCoordColumnMajor4x4(float32x4_t vector, float32x4x4_t matrix) NN_NOEXCEPT;

NN_FORCEINLINE float32x2_t Vector2fLoad(const float* pSource) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fLoad(const Float2& source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector2fStore(float32_t* pOutValue, float32x2_t source) NN_NOEXCEPT;
NN_FORCEINLINE void Vector2fStore(Float2* pOutValue, float32x2_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSet(float32_t x, float32_t y) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSetElement(float32x2_t vector, int index, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSetX(float32x2_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSetY(float32x2_t vector, float32_t value) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector2fGetElement(float32x2_t vector, int index) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector2fGetX(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32_t Vector2fGetY(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fZero() NN_NOEXCEPT;
NN_FORCEINLINE bool Vector2fEqual(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector2fIsZero(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE bool Vector2fIsZero(float32x2_t vector, float32_t error) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fAdd(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSubtract(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiply(float32x2_t vector, float32_t factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiply(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplyAdd(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplyAdd(float32x2_t vector1, float32x2_t vector2, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplyAddLane0(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplyAddLane1(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplySubtract(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplySubtract(float32x2_t vector1, float32x2_t vector2, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplySubtractLane0(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMultiplySubtractLane1(float32x2_t vector1, float32x2_t vector2, float32x2_t vector3) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fDivide(float32x2_t vector, float32_t factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fDivide(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fDot(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fCross(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fLength(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fLengthSquared(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fDistance(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fDistanceSquared(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fNormalize(float32x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMaximize(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMinimize(float32x2_t vector1, float32x2_t vector2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fLerp(float32x2_t from, float32x2_t to, float32_t t) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fNegate(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fMask(float32x2_t vector, uint32x2_t mask) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSqrt(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fRecp(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fRsqrt(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fRound(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fModPi(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fSin(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fCos(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void Vector2fSinCos(float32x2_t* pOutSin, float32x2_t* pOutCos, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fTan(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fAsin(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fAcos(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fAtanImpl(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fAtan(float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fAtan2(float32x2_t vectorY, float32x2_t vectorX) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fTransformRowMajor3x2(float32x2_t vector, float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fTransformColumnMajor3x2(float32x2_t vector, float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fTransformNormalRowMajor3x2(float32x2_t vector, float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t Vector2fTransformNormalColumnMajor3x2(float32x2_t vector, float32x4x2_t matrix) NN_NOEXCEPT;

}}}} // namespace nn::util::neon::detail

#include <nn/util/detail/util_VectorApiDetailImpl.neon.h>
