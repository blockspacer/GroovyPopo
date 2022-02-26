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

//
// 4x4, T4x4 共通の処理をまとめた関数
//
NN_FORCEINLINE float32x4x4_t Matrix4x4fInverse(float32x4_t *pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t Matrix4x4fTranspose(float32x4x4_t matrix) NN_NOEXCEPT;

//
// 4x4 行列
//
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fConvertFromRowMajor4x3f(float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fLoad(const FloatRowMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fLoad(const FloatColumnMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fStore(FloatRowMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fStore(FloatColumnMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSet(
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fSetRow(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetRow(const float32x4x4_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x4_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fSetColumn(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2, const float32x4_t& column3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetColumn(const float32x4x4_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, float32x4_t* pOutColumn3, const float32x4x4_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisX(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisY(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisZ(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisW(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisX(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisY(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisZ(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisW(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x4fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fZero() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fAdd(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSubtract(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fMultiply(float32x4x4_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fMultiplyRowMajor4x3f(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fMultiply(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fDivide(float32x4x4_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fInverse(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fTranspose(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fOrthographicOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

//
// T4x4 行列
//
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fConvertFromColumnMajor4x3f(float32x4x3_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fLoad(const FloatRowMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fLoad(const FloatColumnMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fStore(FloatRowMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fStore(FloatColumnMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSet(
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fSetRow(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetRow(const float32x4x4_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x4_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fSetColumn(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2, const float32x4_t& column3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetColumn(const float32x4x4_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, float32x4_t* pOutColumn3, const float32x4x4_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisX(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisY(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisZ(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisW(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisX(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisY(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisZ(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisW(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x4fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fZero() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fAdd(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSubtract(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fMultiply(float32x4x4_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fMultiplyColumnMajor4x3f(float32x4x4_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fMultiply(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fDivide(float32x4x4_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fInverse(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fTranspose(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fOrthographicOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

//
// 4x3 行列
//
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fFromQuaternion(float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fConvertFromRowMajor4x4f(float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLoad(const FloatRowMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLoad(const FloatColumnMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fStore(FloatRowMajor4x3* pOutValue, float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fStore(FloatColumnMajor4x3* pOutValue, float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSet(
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fSetRow(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetRow(const float32x4x4_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x4_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fSetColumn(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetColumn(const float32x4x4_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, const float32x4x4_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisX(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisY(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisZ(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisW(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisX(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisY(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisZ(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisW(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor4x3fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fZero() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fAdd(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSubtract(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiply(float32x4x4_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiply(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiplyRowMajor4x4f(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiplyAdd(float t, float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fDivide(float32x4x4_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fInverse(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTranspose(float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fInverseTranspose(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float32x4_t up) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float twistRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fCameraRotateZxy(float32x4_t cameraPosition, float32x4_t rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetTranslate(float32x4x4_t matrix, float32x4_t translate) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetScale(float32x4x4_t matrix, float32x4_t scale) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetRotate(float32x4x4_t matrix, float32x4_t rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetRotateXyz(float32x4x4_t matrix, float32x4_t rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetScaleRotate(float32x4x4_t matrix, float32x4_t scale, float32x4_t rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetScaleRotateXyz(float32x4x4_t matrix, float32x4_t scale, float32x4_t rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTextureProjectionPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTextureProjectionPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTextureProjectionOrthographicOffCenterRightHanded(float left, float right, float bottom, float top) NN_NOEXCEPT;

//
// T4x3 行列
//
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fFromQuaternion(float32x4_t quaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fConvertFromColumnMajor4x4f(float32x4x4_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLoad(const FloatRowMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLoad(const FloatColumnMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fStore(FloatRowMajor4x3* pOutValue, float32x4x3_t source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fStore(FloatColumnMajor4x3* pOutValue, float32x4x3_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSet(
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fSetRow(float32x4x3_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetRow(const float32x4x3_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x3_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fSetColumn(float32x4x3_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetColumn(const float32x4x3_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, const float32x4x3_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisX(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisY(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisZ(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisW(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisX(float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisY(float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisZ(float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisW(float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor4x3fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fZero() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fAdd(float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSubtract(float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fMultiply(float32x4x3_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fMultiply(float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x3fMultiplyColumnMajor4x4f(float32x4x3_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fMultiplyAdd(float t, float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fDivide(float32x4x3_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fInverse(float32x4_t* pDeterminant, float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTranspose(float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fInverseTranspose(float32x4_t* pDeterminant, float32x4x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float32x4_t up) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float twistRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fCameraRotateZxy(float32x4_t cameraPosition, float32x4_t rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetTranslate(float32x4x3_t matrix, float32x4_t translate) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetScale(float32x4x3_t matrix, float32x4_t scale) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetRotate(float32x4x3_t matrix, float32x4_t rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetRotateXyz(float32x4x3_t matrix, float32x4_t rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetScaleRotate(float32x4x3_t matrix, float32x4_t scale, float32x4_t rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetScaleRotateXyz(float32x4x3_t matrix, float32x4_t scale, float32x4_t rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTextureProjectionPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTextureProjectionPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTextureProjectionOrthographicOffCenterRightHanded(float left, float right, float bottom, float top) NN_NOEXCEPT;

//
// 3x2 行列
//
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fLoad(const FloatRowMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fLoad(const FloatColumnMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fStore(FloatRowMajor3x2* pOutValue, float32x2x3_t source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fStore(FloatColumnMajor3x2* pOutValue, float32x2x3_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSet(
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fSetRow(float32x2x3_t* pOutValue, int index, const float32x2_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetRows(const float32x2_t& row0, const float32x2_t& row1, const float32x2_t& row2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetRow(const float32x2x3_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fGetRows(float32x2_t* pOutRow0, float32x2_t* pOutRow1, float32x2_t* pOutRow2, const float32x2x3_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fSetColumn(float32x2x3_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetColumns(const float32x4_t& column0, const float32x4_t& column1) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixRowMajor3x2fGetColumn(const float32x2x3_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, const float32x2x3_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fSetAxisX(float32x2x3_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fSetAxisY(float32x2x3_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fSetAxisZ(float32x2x3_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetAxes(float32x2_t axisX, float32x2_t axisY, float32x2_t axisZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetAxisX(float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetAxisY(float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetAxisZ(float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixRowMajor3x2fGetAxes(float32x2_t* pOutAxisX, float32x2_t* pOutAxisY, float32x2_t* pOutAxisZ, float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fZero() NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fAdd(float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSubtract(float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fMultiply(float32x2x3_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fMultiply(float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fMultiplyAdd(float t, float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fDivide(float32x2x3_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fInverse(float32x4_t* pDeterminant, float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fTranspose(float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fInverseTranspose(float32x4_t* pDeterminant, float32x2x3_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetTranslate(float32x2x3_t matrix, float32x2_t translate) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetScale(float32x2x3_t matrix, float32x2_t scale) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetRotate(float32x2x3_t matrix, float rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetScaleRotate(float32x2x3_t matrix, float32x2_t scale, float rotationRadian) NN_NOEXCEPT;

//
// T3x2 行列
//
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fLoad(const FloatRowMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fLoad(const FloatColumnMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fStore(FloatRowMajor3x2* pOutValue, float32x4x2_t source) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fStore(FloatColumnMajor3x2* pOutValue, float32x4x2_t source) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSet(
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fSetRow(float32x4x2_t* pOutValue, int index, const float32x2_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetRows(const float32x2_t& row0, const float32x2_t& row1, const float32x2_t& row2) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetRow(const float32x4x2_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fGetRows(float32x2_t* pOutRow0, float32x2_t* pOutRow1, float32x2_t* pOutRow2, const float32x4x2_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fSetColumn(float32x4x2_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetColumns(const float32x4_t& column0, const float32x4_t& column1) NN_NOEXCEPT;
NN_FORCEINLINE float32x4_t MatrixColumnMajor3x2fGetColumn(const float32x4x2_t& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, const float32x4x2_t& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fSetAxisX(float32x4x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fSetAxisY(float32x4x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fSetAxisZ(float32x4x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetAxes(float32x2_t axisX, float32x2_t axisY, float32x2_t axisZ) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetAxisX(float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetAxisY(float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetAxisZ(float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE void MatrixColumnMajor3x2fGetAxes(float32x2_t* pOutAxisX, float32x2_t* pOutAxisY, float32x2_t* pOutAxisZ, float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fZero() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fIdentity() NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fAdd(float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSubtract(float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fMultiply(float32x4x2_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fMultiply(float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fMultiplyAdd(float t, float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fDivide(float32x4x2_t matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fInverse(float32x4_t* pDeterminant, float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fTranspose(float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fInverseTranspose(float32x4_t* pDeterminant, float32x4x2_t matrix) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetTranslate(float32x4x2_t matrix, float32x2_t translate) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetScale(float32x4x2_t matrix, float32x2_t scale) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetRotate(float32x4x2_t matrix, float rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetScaleRotate(float32x4x2_t matrix, float32x2_t scale, float rotationRadian) NN_NOEXCEPT;

}}}} // namespace nn::util::neon::detail

#include <nn/util/detail/util_MatrixApiDetailImpl.neon.h>
