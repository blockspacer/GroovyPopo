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
#include <nn/util/detail/util_MathTypes.general.h>
#include <nn/util/detail/util_VectorApi.general.h>
#include <nn/util/detail/util_ConstantImpl.h>
#include <cmath>
#include <algorithm>

//
// SIMD ユニットを用いないベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::general 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

//
// 初期化マクロ
//

#define NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_3X2F_INITIALIZER(m00, m01, m10, m11, m20, m21) \
    { \
        { \
            { m00, m01 }, \
            { m10, m11 }, \
            { m20, m21 }, \
        } \
    }

#define NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER(m00, m01, m10, m11, m20, m21) \
    { \
        { \
            { m00, m10, m20 }, \
            { m01, m11, m21 }, \
        } \
    }

#define NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) \
    { \
        { \
            { m00, m01, m02 }, \
            { m10, m11, m12 }, \
            { m20, m21, m22 }, \
            { m30, m31, m32 }  \
        } \
    }

#define NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) \
    { \
        { \
            { m00, m10, m20, m30 }, \
            { m01, m11, m21, m31 }, \
            { m02, m12, m22, m32 }  \
        } \
    }

#define NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
    { \
        { \
            { m00, m01, m02, m03 }, \
            { m10, m11, m12, m13 }, \
            { m20, m21, m22, m23 }, \
            { m30, m31, m32, m33 }  \
        } \
    }

#define NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
    { \
        { \
            { m00, m10, m20, m30 }, \
            { m01, m11, m21, m31 }, \
            { m02, m12, m22, m32 }, \
            { m03, m13, m23, m33 }, \
        } \
    }

#define NN_UTIL_GENERAL_MATRIX_3X2F_INITIALIZER     NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_3X2F_INITIALIZER

#define NN_UTIL_GENERAL_MATRIX_T3X2F_INITIALIZER     NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER

#define NN_UTIL_GENERAL_MATRIX_4X3F_INITIALIZER     NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER

#define NN_UTIL_GENERAL_MATRIX_T4X3F_INITIALIZER    NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER

#define NN_UTIL_GENERAL_MATRIX_4X4F_INITIALIZER     NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER

#define NN_UTIL_GENERAL_MATRIX_T4X4F_INITIALIZER    NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER


namespace nn { namespace util { namespace general {

// Matrix3x2fType
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor3x2fType* pOutValue,
                            float m00, float m01,
                            float m10, float m11,
                            float m20, float m21) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor3x2fType* pOutValue,
                            float m00, float m01,
                            float m10, float m11,
                            float m20, float m21) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRow(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRow(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumn(Vector3fType *pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumn(Vector3fType *pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor3x2fType* pOutValue, float t, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor3x2fType* pOutValue, float t, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT;

// Matrix4x3fType
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x3fType* pOutValue,
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x3fType* pOutValue,
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRow(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRow(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor4x3fType* pOutValue, float t, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor4x3fType* pOutValue, float t, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT;

// Matrix4x4fType
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x4fType* pOutValue,
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x4fType* pOutValue,
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRow(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRow(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;
NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

//
// 読み込み
//

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[0][1];

    pOutValue->_m[1][0] = source.m[1][0];
    pOutValue->_m[1][1] = source.m[1][1];

    pOutValue->_m[2][0] = source.m[2][0];
    pOutValue->_m[2][1] = source.m[2][1];
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[1][0];

    pOutValue->_m[1][0] = source.m[0][1];
    pOutValue->_m[1][1] = source.m[1][1];

    pOutValue->_m[2][0] = source.m[0][2];
    pOutValue->_m[2][1] = source.m[1][2];
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[0][1];
    pOutValue->_m[0][2] = source.m[0][2];

    pOutValue->_m[1][0] = source.m[1][0];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[1][2];

    pOutValue->_m[2][0] = source.m[2][0];
    pOutValue->_m[2][1] = source.m[2][1];
    pOutValue->_m[2][2] = source.m[2][2];

    pOutValue->_m[3][0] = source.m[3][0];
    pOutValue->_m[3][1] = source.m[3][1];
    pOutValue->_m[3][2] = source.m[3][2];
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[1][0];
    pOutValue->_m[0][2] = source.m[2][0];

    pOutValue->_m[1][0] = source.m[0][1];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[2][1];

    pOutValue->_m[2][0] = source.m[0][2];
    pOutValue->_m[2][1] = source.m[1][2];
    pOutValue->_m[2][2] = source.m[2][2];

    pOutValue->_m[3][0] = source.m[0][3];
    pOutValue->_m[3][1] = source.m[1][3];
    pOutValue->_m[3][2] = source.m[2][3];
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[0][1];
    pOutValue->_m[0][2] = source.m[0][2];
    pOutValue->_m[0][3] = source.m[0][3];

    pOutValue->_m[1][0] = source.m[1][0];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[1][2];
    pOutValue->_m[1][3] = source.m[1][3];

    pOutValue->_m[2][0] = source.m[2][0];
    pOutValue->_m[2][1] = source.m[2][1];
    pOutValue->_m[2][2] = source.m[2][2];
    pOutValue->_m[2][3] = source.m[2][3];

    pOutValue->_m[3][0] = source.m[3][0];
    pOutValue->_m[3][1] = source.m[3][1];
    pOutValue->_m[3][2] = source.m[3][2];
    pOutValue->_m[3][3] = source.m[3][3];
}
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[1][0];
    pOutValue->_m[0][2] = source.m[2][0];
    pOutValue->_m[0][3] = source.m[3][0];

    pOutValue->_m[1][0] = source.m[0][1];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[2][1];
    pOutValue->_m[1][3] = source.m[3][1];

    pOutValue->_m[2][0] = source.m[0][2];
    pOutValue->_m[2][1] = source.m[1][2];
    pOutValue->_m[2][2] = source.m[2][2];
    pOutValue->_m[2][3] = source.m[3][2];

    pOutValue->_m[3][0] = source.m[0][3];
    pOutValue->_m[3][1] = source.m[1][3];
    pOutValue->_m[3][2] = source.m[2][3];
    pOutValue->_m[3][3] = source.m[3][3];
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[1][0];
    pOutValue->_m[0][2] = source.m[2][0];

    pOutValue->_m[1][0] = source.m[0][1];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[2][1];
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[0][1];
    pOutValue->_m[0][2] = source.m[0][2];

    pOutValue->_m[1][0] = source.m[1][0];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[1][2];
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[1][0];
    pOutValue->_m[0][2] = source.m[2][0];
    pOutValue->_m[0][3] = source.m[3][0];

    pOutValue->_m[1][0] = source.m[0][1];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[2][1];
    pOutValue->_m[1][3] = source.m[3][1];

    pOutValue->_m[2][0] = source.m[0][2];
    pOutValue->_m[2][1] = source.m[1][2];
    pOutValue->_m[2][2] = source.m[2][2];
    pOutValue->_m[2][3] = source.m[3][2];
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[0][1];
    pOutValue->_m[0][2] = source.m[0][2];
    pOutValue->_m[0][3] = source.m[0][3];

    pOutValue->_m[1][0] = source.m[1][0];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[1][2];
    pOutValue->_m[1][3] = source.m[1][3];

    pOutValue->_m[2][0] = source.m[2][0];
    pOutValue->_m[2][1] = source.m[2][1];
    pOutValue->_m[2][2] = source.m[2][2];
    pOutValue->_m[2][3] = source.m[2][3];
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[1][0];
    pOutValue->_m[0][2] = source.m[2][0];
    pOutValue->_m[0][3] = source.m[3][0];

    pOutValue->_m[1][0] = source.m[0][1];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[2][1];
    pOutValue->_m[1][3] = source.m[3][1];

    pOutValue->_m[2][0] = source.m[0][2];
    pOutValue->_m[2][1] = source.m[1][2];
    pOutValue->_m[2][2] = source.m[2][2];
    pOutValue->_m[2][3] = source.m[3][2];

    pOutValue->_m[3][0] = source.m[0][3];
    pOutValue->_m[3][1] = source.m[1][3];
    pOutValue->_m[3][2] = source.m[2][3];
    pOutValue->_m[3][3] = source.m[3][3];
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source.m[0][0];
    pOutValue->_m[0][1] = source.m[0][1];
    pOutValue->_m[0][2] = source.m[0][2];
    pOutValue->_m[0][3] = source.m[0][3];

    pOutValue->_m[1][0] = source.m[1][0];
    pOutValue->_m[1][1] = source.m[1][1];
    pOutValue->_m[1][2] = source.m[1][2];
    pOutValue->_m[1][3] = source.m[1][3];

    pOutValue->_m[2][0] = source.m[2][0];
    pOutValue->_m[2][1] = source.m[2][1];
    pOutValue->_m[2][2] = source.m[2][2];
    pOutValue->_m[2][3] = source.m[2][3];

    pOutValue->_m[3][0] = source.m[3][0];
    pOutValue->_m[3][1] = source.m[3][1];
    pOutValue->_m[3][2] = source.m[3][2];
    pOutValue->_m[3][3] = source.m[3][3];
}

//
// 書き出し
//
NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[0][1];

    pOutValue->m[1][0] = source._m[1][0];
    pOutValue->m[1][1] = source._m[1][1];

    pOutValue->m[2][0] = source._m[2][0];
    pOutValue->m[2][1] = source._m[2][1];
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[1][0];
    pOutValue->m[0][2] = source._m[2][0];

    pOutValue->m[1][0] = source._m[0][1];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[2][1];
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[0][1];
    pOutValue->m[0][2] = source._m[0][2];

    pOutValue->m[1][0] = source._m[1][0];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[1][2];

    pOutValue->m[2][0] = source._m[2][0];
    pOutValue->m[2][1] = source._m[2][1];
    pOutValue->m[2][2] = source._m[2][2];

    pOutValue->m[3][0] = source._m[3][0];
    pOutValue->m[3][1] = source._m[3][1];
    pOutValue->m[3][2] = source._m[3][2];
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[1][0];
    pOutValue->m[0][2] = source._m[2][0];
    pOutValue->m[0][3] = source._m[3][0];

    pOutValue->m[1][0] = source._m[0][1];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[2][1];
    pOutValue->m[1][3] = source._m[3][1];

    pOutValue->m[2][0] = source._m[0][2];
    pOutValue->m[2][1] = source._m[1][2];
    pOutValue->m[2][2] = source._m[2][2];
    pOutValue->m[2][3] = source._m[3][2];
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[0][1];
    pOutValue->m[0][2] = source._m[0][2];
    pOutValue->m[0][3] = source._m[0][3];

    pOutValue->m[1][0] = source._m[1][0];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[1][2];
    pOutValue->m[1][3] = source._m[1][3];

    pOutValue->m[2][0] = source._m[2][0];
    pOutValue->m[2][1] = source._m[2][1];
    pOutValue->m[2][2] = source._m[2][2];
    pOutValue->m[2][3] = source._m[2][3];

    pOutValue->m[3][0] = source._m[3][0];
    pOutValue->m[3][1] = source._m[3][1];
    pOutValue->m[3][2] = source._m[3][2];
    pOutValue->m[3][3] = source._m[3][3];
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[1][0];
    pOutValue->m[0][2] = source._m[2][0];
    pOutValue->m[0][3] = source._m[3][0];

    pOutValue->m[1][0] = source._m[0][1];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[2][1];
    pOutValue->m[1][3] = source._m[3][1];

    pOutValue->m[2][0] = source._m[0][2];
    pOutValue->m[2][1] = source._m[1][2];
    pOutValue->m[2][2] = source._m[2][2];
    pOutValue->m[2][3] = source._m[3][2];

    pOutValue->m[3][0] = source._m[0][3];
    pOutValue->m[3][1] = source._m[1][3];
    pOutValue->m[3][2] = source._m[2][3];
    pOutValue->m[3][3] = source._m[3][3];
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[1][0];

    pOutValue->m[1][0] = source._m[0][1];
    pOutValue->m[1][1] = source._m[1][1];

    pOutValue->m[2][0] = source._m[0][2];
    pOutValue->m[2][1] = source._m[1][2];
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[0][1];
    pOutValue->m[0][2] = source._m[0][2];

    pOutValue->m[1][0] = source._m[1][0];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[1][2];
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[1][0];
    pOutValue->m[0][2] = source._m[2][0];

    pOutValue->m[1][0] = source._m[0][1];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[2][1];

    pOutValue->m[2][0] = source._m[0][2];
    pOutValue->m[2][1] = source._m[1][2];
    pOutValue->m[2][2] = source._m[2][2];

    pOutValue->m[3][0] = source._m[0][3];
    pOutValue->m[3][1] = source._m[1][3];
    pOutValue->m[3][2] = source._m[2][3];
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[0][1];
    pOutValue->m[0][2] = source._m[0][2];
    pOutValue->m[0][3] = source._m[0][3];

    pOutValue->m[1][0] = source._m[1][0];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[1][2];
    pOutValue->m[1][3] = source._m[1][3];

    pOutValue->m[2][0] = source._m[2][0];
    pOutValue->m[2][1] = source._m[2][1];
    pOutValue->m[2][2] = source._m[2][2];
    pOutValue->m[2][3] = source._m[2][3];
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[1][0];
    pOutValue->m[0][2] = source._m[2][0];
    pOutValue->m[0][3] = source._m[3][0];

    pOutValue->m[1][0] = source._m[0][1];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[2][1];
    pOutValue->m[1][3] = source._m[3][1];

    pOutValue->m[2][0] = source._m[0][2];
    pOutValue->m[2][1] = source._m[1][2];
    pOutValue->m[2][2] = source._m[2][2];
    pOutValue->m[2][3] = source._m[3][2];

    pOutValue->m[3][0] = source._m[0][3];
    pOutValue->m[3][1] = source._m[1][3];
    pOutValue->m[3][2] = source._m[2][3];
    pOutValue->m[3][3] = source._m[3][3];
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->m[0][0] = source._m[0][0];
    pOutValue->m[0][1] = source._m[0][1];
    pOutValue->m[0][2] = source._m[0][2];
    pOutValue->m[0][3] = source._m[0][3];

    pOutValue->m[1][0] = source._m[1][0];
    pOutValue->m[1][1] = source._m[1][1];
    pOutValue->m[1][2] = source._m[1][2];
    pOutValue->m[1][3] = source._m[1][3];

    pOutValue->m[2][0] = source._m[2][0];
    pOutValue->m[2][1] = source._m[2][1];
    pOutValue->m[2][2] = source._m[2][2];
    pOutValue->m[2][3] = source._m[2][3];

    pOutValue->m[3][0] = source._m[3][0];
    pOutValue->m[3][1] = source._m[3][1];
    pOutValue->m[3][2] = source._m[3][2];
    pOutValue->m[3][3] = source._m[3][3];
}

//
// 変換
//

NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source._m[0][0];
    pOutValue->_m[0][1] = source._m[0][1];
    pOutValue->_m[0][2] = source._m[0][2];

    pOutValue->_m[1][0] = source._m[1][0];
    pOutValue->_m[1][1] = source._m[1][1];
    pOutValue->_m[1][2] = source._m[1][2];

    pOutValue->_m[2][0] = source._m[2][0];
    pOutValue->_m[2][1] = source._m[2][1];
    pOutValue->_m[2][2] = source._m[2][2];

    pOutValue->_m[3][0] = source._m[3][0];
    pOutValue->_m[3][1] = source._m[3][1];
    pOutValue->_m[3][2] = source._m[3][2];
}

NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source._m[0][0];
    pOutValue->_m[0][1] = source._m[0][1];
    pOutValue->_m[0][2] = source._m[0][2];
    pOutValue->_m[0][3] = 0;

    pOutValue->_m[1][0] = source._m[1][0];
    pOutValue->_m[1][1] = source._m[1][1];
    pOutValue->_m[1][2] = source._m[1][2];
    pOutValue->_m[1][3] = 0;

    pOutValue->_m[2][0] = source._m[2][0];
    pOutValue->_m[2][1] = source._m[2][1];
    pOutValue->_m[2][2] = source._m[2][2];
    pOutValue->_m[2][3] = 0;

    pOutValue->_m[3][0] = source._m[3][0];
    pOutValue->_m[3][1] = source._m[3][1];
    pOutValue->_m[3][2] = source._m[3][2];
    pOutValue->_m[3][3] = 1;
}

NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source._m[0][0];
    pOutValue->_m[0][1] = source._m[0][1];
    pOutValue->_m[0][2] = source._m[0][2];
    pOutValue->_m[0][3] = source._m[0][3];

    pOutValue->_m[1][0] = source._m[1][0];
    pOutValue->_m[1][1] = source._m[1][1];
    pOutValue->_m[1][2] = source._m[1][2];
    pOutValue->_m[1][3] = source._m[1][3];

    pOutValue->_m[2][0] = source._m[2][0];
    pOutValue->_m[2][1] = source._m[2][1];
    pOutValue->_m[2][2] = source._m[2][2];
    pOutValue->_m[2][3] = source._m[2][3];
}

NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = source._m[0][0];
    pOutValue->_m[0][1] = source._m[0][1];
    pOutValue->_m[0][2] = source._m[0][2];
    pOutValue->_m[0][3] = source._m[0][3];

    pOutValue->_m[1][0] = source._m[1][0];
    pOutValue->_m[1][1] = source._m[1][1];
    pOutValue->_m[1][2] = source._m[1][2];
    pOutValue->_m[1][3] = source._m[1][3];

    pOutValue->_m[2][0] = source._m[2][0];
    pOutValue->_m[2][1] = source._m[2][1];
    pOutValue->_m[2][2] = source._m[2][2];
    pOutValue->_m[2][3] = source._m[2][3];

    pOutValue->_m[3][0] = 0;
    pOutValue->_m[3][1] = 0;
    pOutValue->_m[3][2] = 0;
    pOutValue->_m[3][3] = 1;
}


//
// アクセサ
//
// 3x2 行列
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor3x2fType* pOutValue,
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = m00;
    pOutValue->_m[0][1] = m01;

    pOutValue->_m[1][0] = m10;
    pOutValue->_m[1][1] = m11;

    pOutValue->_m[2][0] = m20;
    pOutValue->_m[2][1] = m21;
}

NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[index][0] = vector._v[0];
    pOutValue->_m[index][1] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = row0._v[0];
    pOutValue->_m[0][1] = row0._v[1];

    pOutValue->_m[1][0] = row1._v[0];
    pOutValue->_m[1][1] = row1._v[1];

    pOutValue->_m[2][0] = row2._v[0];
    pOutValue->_m[2][1] = row2._v[1];
}

NN_FORCEINLINE void       MatrixGetRow(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[index][0];
    pOutValue->_v[1] = matrix._m[index][1];
}

NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutRow0->_v[0] = matrix._m[0][0];
    pOutRow0->_v[1] = matrix._m[0][1];

    pOutRow1->_v[0] = matrix._m[1][0];
    pOutRow1->_v[1] = matrix._m[1][1];

    pOutRow2->_v[0] = matrix._m[2][0];
    pOutRow2->_v[1] = matrix._m[2][1];
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][index] = vector._v[0];
    pOutValue->_m[1][index] = vector._v[1];
    pOutValue->_m[2][index] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = column0._v[0];
    pOutValue->_m[1][0] = column0._v[1];
    pOutValue->_m[2][0] = column0._v[2];

    pOutValue->_m[0][1] = column1._v[0];
    pOutValue->_m[1][1] = column1._v[1];
    pOutValue->_m[2][1] = column1._v[2];
}

NN_FORCEINLINE void       MatrixGetColumn(Vector3fType *pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][index];
    pOutValue->_v[1] = matrix._m[1][index];
    pOutValue->_v[2] = matrix._m[2][index];
}

NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutColumn0->_v[0] = matrix._m[0][0];
    pOutColumn0->_v[1] = matrix._m[1][0];
    pOutColumn0->_v[2] = matrix._m[2][0];

    pOutColumn1->_v[0] = matrix._m[0][1];
    pOutColumn1->_v[1] = matrix._m[1][1];
    pOutColumn1->_v[2] = matrix._m[2][1];
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = vector._v[0];
    pOutValue->_m[0][1] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[1][0] = vector._v[0];
    pOutValue->_m[1][1] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[2][0] = vector._v[0];
    pOutValue->_m[2][1] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = axisX._v[0];
    pOutValue->_m[0][1] = axisX._v[1];

    pOutValue->_m[1][0] = axisY._v[0];
    pOutValue->_m[1][1] = axisY._v[1];

    pOutValue->_m[2][0] = axisZ._v[0];
    pOutValue->_m[2][1] = axisZ._v[1];
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][0];
    pOutValue->_v[1] = matrix._m[0][1];
}
NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[1][0];
    pOutValue->_v[1] = matrix._m[1][1];
}
NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[2][0];
    pOutValue->_v[1] = matrix._m[2][1];
}

NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutAxisX->_v[0] = matrix._m[0][0];
    pOutAxisX->_v[1] = matrix._m[0][1];

    pOutAxisY->_v[0] = matrix._m[1][0];
    pOutAxisY->_v[1] = matrix._m[1][1];

    pOutAxisZ->_v[0] = matrix._m[2][0];
    pOutAxisZ->_v[1] = matrix._m[2][1];
}

// 4x3 行列
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x3fType* pOutValue,
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = m00;
    pOutValue->_m[0][1] = m01;
    pOutValue->_m[0][2] = m02;

    pOutValue->_m[1][0] = m10;
    pOutValue->_m[1][1] = m11;
    pOutValue->_m[1][2] = m12;

    pOutValue->_m[2][0] = m20;
    pOutValue->_m[2][1] = m21;
    pOutValue->_m[2][2] = m22;

    pOutValue->_m[3][0] = m30;
    pOutValue->_m[3][1] = m31;
    pOutValue->_m[3][2] = m32;
}

NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[index][0] = vector._v[0];
    pOutValue->_m[index][1] = vector._v[1];
    pOutValue->_m[index][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = row0._v[0];
    pOutValue->_m[0][1] = row0._v[1];
    pOutValue->_m[0][2] = row0._v[2];

    pOutValue->_m[1][0] = row1._v[0];
    pOutValue->_m[1][1] = row1._v[1];
    pOutValue->_m[1][2] = row1._v[2];

    pOutValue->_m[2][0] = row2._v[0];
    pOutValue->_m[2][1] = row2._v[1];
    pOutValue->_m[2][2] = row2._v[2];

    pOutValue->_m[3][0] = row3._v[0];
    pOutValue->_m[3][1] = row3._v[1];
    pOutValue->_m[3][2] = row3._v[2];
}

NN_FORCEINLINE void       MatrixGetRow(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[index][0];
    pOutValue->_v[1] = matrix._m[index][1];
    pOutValue->_v[2] = matrix._m[index][2];
}

NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutRow0->_v[0] = matrix._m[0][0];
    pOutRow0->_v[1] = matrix._m[0][1];
    pOutRow0->_v[2] = matrix._m[0][2];

    pOutRow1->_v[0] = matrix._m[1][0];
    pOutRow1->_v[1] = matrix._m[1][1];
    pOutRow1->_v[2] = matrix._m[1][2];

    pOutRow2->_v[0] = matrix._m[2][0];
    pOutRow2->_v[1] = matrix._m[2][1];
    pOutRow2->_v[2] = matrix._m[2][2];

    pOutRow3->_v[0] = matrix._m[3][0];
    pOutRow3->_v[1] = matrix._m[3][1];
    pOutRow3->_v[2] = matrix._m[3][2];
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][index] = vector._v[0];
    pOutValue->_m[1][index] = vector._v[1];
    pOutValue->_m[2][index] = vector._v[2];
    pOutValue->_m[3][index] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = column0._v[0];
    pOutValue->_m[1][0] = column0._v[1];
    pOutValue->_m[2][0] = column0._v[2];
    pOutValue->_m[3][0] = column0._v[3];

    pOutValue->_m[0][1] = column1._v[0];
    pOutValue->_m[1][1] = column1._v[1];
    pOutValue->_m[2][1] = column1._v[2];
    pOutValue->_m[3][1] = column1._v[3];

    pOutValue->_m[0][2] = column2._v[0];
    pOutValue->_m[1][2] = column2._v[1];
    pOutValue->_m[2][2] = column2._v[2];
    pOutValue->_m[3][2] = column2._v[3];
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][index];
    pOutValue->_v[1] = matrix._m[1][index];
    pOutValue->_v[2] = matrix._m[2][index];
    pOutValue->_v[3] = matrix._m[3][index];
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutColumn0->_v[0] = matrix._m[0][0];
    pOutColumn0->_v[1] = matrix._m[1][0];
    pOutColumn0->_v[2] = matrix._m[2][0];
    pOutColumn0->_v[3] = matrix._m[3][0];

    pOutColumn1->_v[0] = matrix._m[0][1];
    pOutColumn1->_v[1] = matrix._m[1][1];
    pOutColumn1->_v[2] = matrix._m[2][1];
    pOutColumn1->_v[3] = matrix._m[3][1];

    pOutColumn2->_v[0] = matrix._m[0][2];
    pOutColumn2->_v[1] = matrix._m[1][2];
    pOutColumn2->_v[2] = matrix._m[2][2];
    pOutColumn2->_v[3] = matrix._m[3][2];
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = vector._v[0];
    pOutValue->_m[0][1] = vector._v[1];
    pOutValue->_m[0][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[1][0] = vector._v[0];
    pOutValue->_m[1][1] = vector._v[1];
    pOutValue->_m[1][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[2][0] = vector._v[0];
    pOutValue->_m[2][1] = vector._v[1];
    pOutValue->_m[2][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[3][0] = vector._v[0];
    pOutValue->_m[3][1] = vector._v[1];
    pOutValue->_m[3][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = axisX._v[0];
    pOutValue->_m[0][1] = axisX._v[1];
    pOutValue->_m[0][2] = axisX._v[2];

    pOutValue->_m[1][0] = axisY._v[0];
    pOutValue->_m[1][1] = axisY._v[1];
    pOutValue->_m[1][2] = axisY._v[2];

    pOutValue->_m[2][0] = axisZ._v[0];
    pOutValue->_m[2][1] = axisZ._v[1];
    pOutValue->_m[2][2] = axisZ._v[2];

    pOutValue->_m[3][0] = axisW._v[0];
    pOutValue->_m[3][1] = axisW._v[1];
    pOutValue->_m[3][2] = axisW._v[2];
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][0];
    pOutValue->_v[1] = matrix._m[0][1];
    pOutValue->_v[2] = matrix._m[0][2];
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[1][0];
    pOutValue->_v[1] = matrix._m[1][1];
    pOutValue->_v[2] = matrix._m[1][2];
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[2][0];
    pOutValue->_v[1] = matrix._m[2][1];
    pOutValue->_v[2] = matrix._m[2][2];
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[3][0];
    pOutValue->_v[1] = matrix._m[3][1];
    pOutValue->_v[2] = matrix._m[3][2];
}

NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutAxisX->_v[0] = matrix._m[0][0];
    pOutAxisX->_v[1] = matrix._m[0][1];
    pOutAxisX->_v[2] = matrix._m[0][2];

    pOutAxisY->_v[0] = matrix._m[1][0];
    pOutAxisY->_v[1] = matrix._m[1][1];
    pOutAxisY->_v[2] = matrix._m[1][2];

    pOutAxisZ->_v[0] = matrix._m[2][0];
    pOutAxisZ->_v[1] = matrix._m[2][1];
    pOutAxisZ->_v[2] = matrix._m[2][2];

    pOutAxisW->_v[0] = matrix._m[3][0];
    pOutAxisW->_v[1] = matrix._m[3][1];
    pOutAxisW->_v[2] = matrix._m[3][2];
}

// 4x4 行列
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x4fType* pOutValue,
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = m00;
    pOutValue->_m[0][1] = m01;
    pOutValue->_m[0][2] = m02;
    pOutValue->_m[0][3] = m03;

    pOutValue->_m[1][0] = m10;
    pOutValue->_m[1][1] = m11;
    pOutValue->_m[1][2] = m12;
    pOutValue->_m[1][3] = m13;

    pOutValue->_m[2][0] = m20;
    pOutValue->_m[2][1] = m21;
    pOutValue->_m[2][2] = m22;
    pOutValue->_m[2][3] = m23;

    pOutValue->_m[3][0] = m30;
    pOutValue->_m[3][1] = m31;
    pOutValue->_m[3][2] = m32;
    pOutValue->_m[3][3] = m33;
}

NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[index][0] = vector._v[0];
    pOutValue->_m[index][1] = vector._v[1];
    pOutValue->_m[index][2] = vector._v[2];
    pOutValue->_m[index][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = row0._v[0];
    pOutValue->_m[0][1] = row0._v[1];
    pOutValue->_m[0][2] = row0._v[2];
    pOutValue->_m[0][3] = row0._v[3];

    pOutValue->_m[1][0] = row1._v[0];
    pOutValue->_m[1][1] = row1._v[1];
    pOutValue->_m[1][2] = row1._v[2];
    pOutValue->_m[1][3] = row1._v[3];

    pOutValue->_m[2][0] = row2._v[0];
    pOutValue->_m[2][1] = row2._v[1];
    pOutValue->_m[2][2] = row2._v[2];
    pOutValue->_m[2][3] = row2._v[3];

    pOutValue->_m[3][0] = row3._v[0];
    pOutValue->_m[3][1] = row3._v[1];
    pOutValue->_m[3][2] = row3._v[2];
    pOutValue->_m[3][3] = row3._v[3];
}

NN_FORCEINLINE void       MatrixGetRow(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[index][0];
    pOutValue->_v[1] = matrix._m[index][1];
    pOutValue->_v[2] = matrix._m[index][2];
    pOutValue->_v[3] = matrix._m[index][3];
}

NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutRow0->_v[0] = matrix._m[0][0];
    pOutRow0->_v[1] = matrix._m[0][1];
    pOutRow0->_v[2] = matrix._m[0][2];
    pOutRow0->_v[3] = matrix._m[0][3];

    pOutRow1->_v[0] = matrix._m[1][0];
    pOutRow1->_v[1] = matrix._m[1][1];
    pOutRow1->_v[2] = matrix._m[1][2];
    pOutRow1->_v[3] = matrix._m[1][3];

    pOutRow2->_v[0] = matrix._m[2][0];
    pOutRow2->_v[1] = matrix._m[2][1];
    pOutRow2->_v[2] = matrix._m[2][2];
    pOutRow2->_v[3] = matrix._m[2][3];

    pOutRow3->_v[0] = matrix._m[3][0];
    pOutRow3->_v[1] = matrix._m[3][1];
    pOutRow3->_v[2] = matrix._m[3][2];
    pOutRow3->_v[3] = matrix._m[3][3];
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][index] = vector._v[0];
    pOutValue->_m[1][index] = vector._v[1];
    pOutValue->_m[2][index] = vector._v[2];
    pOutValue->_m[3][index] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = column0._v[0];
    pOutValue->_m[1][0] = column0._v[1];
    pOutValue->_m[2][0] = column0._v[2];
    pOutValue->_m[3][0] = column0._v[3];

    pOutValue->_m[0][1] = column1._v[0];
    pOutValue->_m[1][1] = column1._v[1];
    pOutValue->_m[2][1] = column1._v[2];
    pOutValue->_m[3][1] = column1._v[3];

    pOutValue->_m[0][2] = column2._v[0];
    pOutValue->_m[1][2] = column2._v[1];
    pOutValue->_m[2][2] = column2._v[2];
    pOutValue->_m[3][2] = column2._v[3];

    pOutValue->_m[0][3] = column3._v[0];
    pOutValue->_m[1][3] = column3._v[1];
    pOutValue->_m[2][3] = column3._v[2];
    pOutValue->_m[3][3] = column3._v[3];
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][index];
    pOutValue->_v[1] = matrix._m[1][index];
    pOutValue->_v[2] = matrix._m[2][index];
    pOutValue->_v[3] = matrix._m[3][index];
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutColumn0->_v[0] = matrix._m[0][0];
    pOutColumn0->_v[1] = matrix._m[1][0];
    pOutColumn0->_v[2] = matrix._m[2][0];
    pOutColumn0->_v[3] = matrix._m[3][0];

    pOutColumn1->_v[0] = matrix._m[0][1];
    pOutColumn1->_v[1] = matrix._m[1][1];
    pOutColumn1->_v[2] = matrix._m[2][1];
    pOutColumn1->_v[3] = matrix._m[3][1];

    pOutColumn2->_v[0] = matrix._m[0][2];
    pOutColumn2->_v[1] = matrix._m[1][2];
    pOutColumn2->_v[2] = matrix._m[2][2];
    pOutColumn2->_v[3] = matrix._m[3][2];

    pOutColumn3->_v[0] = matrix._m[0][3];
    pOutColumn3->_v[1] = matrix._m[1][3];
    pOutColumn3->_v[2] = matrix._m[2][3];
    pOutColumn3->_v[3] = matrix._m[3][3];
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = vector._v[0];
    pOutValue->_m[0][1] = vector._v[1];
    pOutValue->_m[0][2] = vector._v[2];
    pOutValue->_m[0][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[1][0] = vector._v[0];
    pOutValue->_m[1][1] = vector._v[1];
    pOutValue->_m[1][2] = vector._v[2];
    pOutValue->_m[1][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[2][0] = vector._v[0];
    pOutValue->_m[2][1] = vector._v[1];
    pOutValue->_m[2][2] = vector._v[2];
    pOutValue->_m[2][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[3][0] = vector._v[0];
    pOutValue->_m[3][1] = vector._v[1];
    pOutValue->_m[3][2] = vector._v[2];
    pOutValue->_m[3][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = axisX._v[0];
    pOutValue->_m[0][1] = axisX._v[1];
    pOutValue->_m[0][2] = axisX._v[2];
    pOutValue->_m[0][3] = axisX._v[3];

    pOutValue->_m[1][0] = axisY._v[0];
    pOutValue->_m[1][1] = axisY._v[1];
    pOutValue->_m[1][2] = axisY._v[2];
    pOutValue->_m[1][3] = axisY._v[3];

    pOutValue->_m[2][0] = axisZ._v[0];
    pOutValue->_m[2][1] = axisZ._v[1];
    pOutValue->_m[2][2] = axisZ._v[2];
    pOutValue->_m[2][3] = axisZ._v[3];

    pOutValue->_m[3][0] = axisW._v[0];
    pOutValue->_m[3][1] = axisW._v[1];
    pOutValue->_m[3][2] = axisW._v[2];
    pOutValue->_m[3][3] = axisW._v[3];
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][0];
    pOutValue->_v[1] = matrix._m[0][1];
    pOutValue->_v[2] = matrix._m[0][2];
    pOutValue->_v[3] = matrix._m[0][3];
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[1][0];
    pOutValue->_v[1] = matrix._m[1][1];
    pOutValue->_v[2] = matrix._m[1][2];
    pOutValue->_v[3] = matrix._m[1][3];
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[2][0];
    pOutValue->_v[1] = matrix._m[2][1];
    pOutValue->_v[2] = matrix._m[2][2];
    pOutValue->_v[3] = matrix._m[2][3];
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[3][0];
    pOutValue->_v[1] = matrix._m[3][1];
    pOutValue->_v[2] = matrix._m[3][2];
    pOutValue->_v[3] = matrix._m[3][3];
}

NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutAxisX->_v[0] = matrix._m[0][0];
    pOutAxisX->_v[1] = matrix._m[0][1];
    pOutAxisX->_v[2] = matrix._m[0][2];
    pOutAxisX->_v[3] = matrix._m[0][3];

    pOutAxisY->_v[0] = matrix._m[1][0];
    pOutAxisY->_v[1] = matrix._m[1][1];
    pOutAxisY->_v[2] = matrix._m[1][2];
    pOutAxisY->_v[3] = matrix._m[1][3];

    pOutAxisZ->_v[0] = matrix._m[2][0];
    pOutAxisZ->_v[1] = matrix._m[2][1];
    pOutAxisZ->_v[2] = matrix._m[2][2];
    pOutAxisZ->_v[3] = matrix._m[2][3];

    pOutAxisW->_v[0] = matrix._m[3][0];
    pOutAxisW->_v[1] = matrix._m[3][1];
    pOutAxisW->_v[2] = matrix._m[3][2];
    pOutAxisW->_v[3] = matrix._m[3][3];
}

// T3x2 行列
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor3x2fType* pOutValue,
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = m00;
    pOutValue->_m[0][1] = m10;
    pOutValue->_m[0][2] = m20;

    pOutValue->_m[1][0] = m01;
    pOutValue->_m[1][1] = m11;
    pOutValue->_m[1][2] = m21;
}

NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][index] = vector._v[0];
    pOutValue->_m[1][index] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = row0._v[0];
    pOutValue->_m[1][0] = row0._v[1];

    pOutValue->_m[0][1] = row1._v[0];
    pOutValue->_m[1][1] = row1._v[1];

    pOutValue->_m[0][2] = row2._v[0];
    pOutValue->_m[1][2] = row2._v[1];
}

NN_FORCEINLINE void       MatrixGetRow(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][index];
    pOutValue->_v[1] = matrix._m[1][index];
}

NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutRow0->_v[0] = matrix._m[0][0];
    pOutRow0->_v[1] = matrix._m[1][0];
    pOutRow1->_v[0] = matrix._m[0][1];
    pOutRow1->_v[1] = matrix._m[1][1];
    pOutRow2->_v[0] = matrix._m[0][2];
    pOutRow2->_v[1] = matrix._m[1][2];
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[index][0] = vector._v[0];
    pOutValue->_m[index][1] = vector._v[1];
    pOutValue->_m[index][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = column0._v[0];
    pOutValue->_m[0][1] = column0._v[1];
    pOutValue->_m[0][2] = column0._v[2];

    pOutValue->_m[1][0] = column1._v[0];
    pOutValue->_m[1][1] = column1._v[1];
    pOutValue->_m[1][2] = column1._v[2];
}

NN_FORCEINLINE void       MatrixGetColumn(Vector3fType *pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[index][0];
    pOutValue->_v[1] = matrix._m[index][1];
    pOutValue->_v[2] = matrix._m[index][2];
}

NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutColumn0->_v[0] = matrix._m[0][0];
    pOutColumn0->_v[1] = matrix._m[0][1];
    pOutColumn0->_v[2] = matrix._m[0][2];
    pOutColumn1->_v[0] = matrix._m[1][0];
    pOutColumn1->_v[1] = matrix._m[1][1];
    pOutColumn1->_v[2] = matrix._m[1][2];
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = vector._v[0];
    pOutValue->_m[1][0] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][1] = vector._v[0];
    pOutValue->_m[1][1] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][2] = vector._v[0];
    pOutValue->_m[1][2] = vector._v[1];
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = axisX._v[0];
    pOutValue->_m[0][1] = axisY._v[0];
    pOutValue->_m[0][2] = axisZ._v[0];

    pOutValue->_m[1][0] = axisX._v[1];
    pOutValue->_m[1][1] = axisY._v[1];
    pOutValue->_m[1][2] = axisZ._v[1];
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][0];
    pOutValue->_v[1] = matrix._m[1][0];
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][1];
    pOutValue->_v[1] = matrix._m[1][1];
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][2];
    pOutValue->_v[1] = matrix._m[1][2];
}

NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutAxisX->_v[0] = matrix._m[0][0];
    pOutAxisX->_v[1] = matrix._m[1][0];

    pOutAxisY->_v[0] = matrix._m[0][1];
    pOutAxisY->_v[1] = matrix._m[1][1];

    pOutAxisZ->_v[0] = matrix._m[0][2];
    pOutAxisZ->_v[1] = matrix._m[1][2];
}

// T4x3 行列
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x3fType* pOutValue,
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = m00;
    pOutValue->_m[0][1] = m10;
    pOutValue->_m[0][2] = m20;
    pOutValue->_m[0][3] = m30;

    pOutValue->_m[1][0] = m01;
    pOutValue->_m[1][1] = m11;
    pOutValue->_m[1][2] = m21;
    pOutValue->_m[1][3] = m31;

    pOutValue->_m[2][0] = m02;
    pOutValue->_m[2][1] = m12;
    pOutValue->_m[2][2] = m22;
    pOutValue->_m[2][3] = m32;
}

NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][index] = vector._v[0];
    pOutValue->_m[1][index] = vector._v[1];
    pOutValue->_m[2][index] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = row0._v[0];
    pOutValue->_m[1][0] = row0._v[1];
    pOutValue->_m[2][0] = row0._v[2];

    pOutValue->_m[0][1] = row1._v[0];
    pOutValue->_m[1][1] = row1._v[1];
    pOutValue->_m[2][1] = row1._v[2];

    pOutValue->_m[0][2] = row2._v[0];
    pOutValue->_m[1][2] = row2._v[1];
    pOutValue->_m[2][2] = row2._v[2];

    pOutValue->_m[0][3] = row3._v[0];
    pOutValue->_m[1][3] = row3._v[1];
    pOutValue->_m[2][3] = row3._v[2];
}

NN_FORCEINLINE void       MatrixGetRow(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][index];
    pOutValue->_v[1] = matrix._m[1][index];
    pOutValue->_v[2] = matrix._m[2][index];
}
NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutRow0->_v[0] = matrix._m[0][0];
    pOutRow0->_v[1] = matrix._m[1][0];
    pOutRow0->_v[2] = matrix._m[2][0];

    pOutRow1->_v[0] = matrix._m[0][1];
    pOutRow1->_v[1] = matrix._m[1][1];
    pOutRow1->_v[2] = matrix._m[2][1];

    pOutRow2->_v[0] = matrix._m[0][2];
    pOutRow2->_v[1] = matrix._m[1][2];
    pOutRow2->_v[2] = matrix._m[2][2];

    pOutRow3->_v[0] = matrix._m[0][3];
    pOutRow3->_v[1] = matrix._m[1][3];
    pOutRow3->_v[2] = matrix._m[2][3];
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[index][0] = vector._v[0];
    pOutValue->_m[index][1] = vector._v[1];
    pOutValue->_m[index][2] = vector._v[2];
    pOutValue->_m[index][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = column0._v[0];
    pOutValue->_m[0][1] = column0._v[1];
    pOutValue->_m[0][2] = column0._v[2];
    pOutValue->_m[0][3] = column0._v[3];

    pOutValue->_m[1][0] = column1._v[0];
    pOutValue->_m[1][1] = column1._v[1];
    pOutValue->_m[1][2] = column1._v[2];
    pOutValue->_m[1][3] = column1._v[3];

    pOutValue->_m[2][0] = column2._v[0];
    pOutValue->_m[2][1] = column2._v[1];
    pOutValue->_m[2][2] = column2._v[2];
    pOutValue->_m[2][3] = column2._v[3];
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[index][0];
    pOutValue->_v[1] = matrix._m[index][1];
    pOutValue->_v[2] = matrix._m[index][2];
    pOutValue->_v[3] = matrix._m[index][3];
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutColumn0->_v[0] = matrix._m[0][0];
    pOutColumn0->_v[1] = matrix._m[0][1];
    pOutColumn0->_v[2] = matrix._m[0][2];
    pOutColumn0->_v[3] = matrix._m[0][3];

    pOutColumn1->_v[0] = matrix._m[1][0];
    pOutColumn1->_v[1] = matrix._m[1][1];
    pOutColumn1->_v[2] = matrix._m[1][2];
    pOutColumn1->_v[3] = matrix._m[1][3];

    pOutColumn2->_v[0] = matrix._m[2][0];
    pOutColumn2->_v[1] = matrix._m[2][1];
    pOutColumn2->_v[2] = matrix._m[2][2];
    pOutColumn2->_v[3] = matrix._m[2][3];
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = vector._v[0];
    pOutValue->_m[1][0] = vector._v[1];
    pOutValue->_m[2][0] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][1] = vector._v[0];
    pOutValue->_m[1][1] = vector._v[1];
    pOutValue->_m[2][1] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][2] = vector._v[0];
    pOutValue->_m[1][2] = vector._v[1];
    pOutValue->_m[2][2] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][3] = vector._v[0];
    pOutValue->_m[1][3] = vector._v[1];
    pOutValue->_m[2][3] = vector._v[2];
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = axisX._v[0];
    pOutValue->_m[0][1] = axisY._v[0];
    pOutValue->_m[0][2] = axisZ._v[0];
    pOutValue->_m[0][3] = axisW._v[0];

    pOutValue->_m[1][0] = axisX._v[1];
    pOutValue->_m[1][1] = axisY._v[1];
    pOutValue->_m[1][2] = axisZ._v[1];
    pOutValue->_m[1][3] = axisW._v[1];

    pOutValue->_m[2][0] = axisX._v[2];
    pOutValue->_m[2][1] = axisY._v[2];
    pOutValue->_m[2][2] = axisZ._v[2];
    pOutValue->_m[2][3] = axisW._v[2];
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][0];
    pOutValue->_v[1] = matrix._m[1][0];
    pOutValue->_v[2] = matrix._m[2][0];
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][1];
    pOutValue->_v[1] = matrix._m[1][1];
    pOutValue->_v[2] = matrix._m[2][1];
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][2];
    pOutValue->_v[1] = matrix._m[1][2];
    pOutValue->_v[2] = matrix._m[2][2];
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][3];
    pOutValue->_v[1] = matrix._m[1][3];
    pOutValue->_v[2] = matrix._m[2][3];
}

NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutAxisX->_v[0] = matrix._m[0][0];
    pOutAxisX->_v[1] = matrix._m[1][0];
    pOutAxisX->_v[2] = matrix._m[2][0];

    pOutAxisY->_v[0] = matrix._m[0][1];
    pOutAxisY->_v[1] = matrix._m[1][1];
    pOutAxisY->_v[2] = matrix._m[2][1];

    pOutAxisZ->_v[0] = matrix._m[0][2];
    pOutAxisZ->_v[1] = matrix._m[1][2];
    pOutAxisZ->_v[2] = matrix._m[2][2];

    pOutAxisW->_v[0] = matrix._m[0][3];
    pOutAxisW->_v[1] = matrix._m[1][3];
    pOutAxisW->_v[2] = matrix._m[2][3];
}

// T4x4 行列
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x4fType* pOutValue,
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = m00;
    pOutValue->_m[0][1] = m10;
    pOutValue->_m[0][2] = m20;
    pOutValue->_m[0][3] = m30;

    pOutValue->_m[1][0] = m01;
    pOutValue->_m[1][1] = m11;
    pOutValue->_m[1][2] = m21;
    pOutValue->_m[1][3] = m31;

    pOutValue->_m[2][0] = m02;
    pOutValue->_m[2][1] = m12;
    pOutValue->_m[2][2] = m22;
    pOutValue->_m[2][3] = m32;

    pOutValue->_m[3][0] = m03;
    pOutValue->_m[3][1] = m13;
    pOutValue->_m[3][2] = m23;
    pOutValue->_m[3][3] = m33;
}

NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][index] = vector._v[0];
    pOutValue->_m[1][index] = vector._v[1];
    pOutValue->_m[2][index] = vector._v[2];
    pOutValue->_m[3][index] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = row0._v[0];
    pOutValue->_m[1][0] = row0._v[1];
    pOutValue->_m[2][0] = row0._v[2];
    pOutValue->_m[3][0] = row0._v[3];

    pOutValue->_m[0][1] = row1._v[0];
    pOutValue->_m[1][1] = row1._v[1];
    pOutValue->_m[2][1] = row1._v[2];
    pOutValue->_m[3][1] = row1._v[3];

    pOutValue->_m[0][2] = row2._v[0];
    pOutValue->_m[1][2] = row2._v[1];
    pOutValue->_m[2][2] = row2._v[2];
    pOutValue->_m[3][2] = row2._v[3];

    pOutValue->_m[0][3] = row3._v[0];
    pOutValue->_m[1][3] = row3._v[1];
    pOutValue->_m[2][3] = row3._v[2];
    pOutValue->_m[3][3] = row3._v[3];
}

NN_FORCEINLINE void       MatrixGetRow(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][index];
    pOutValue->_v[1] = matrix._m[1][index];
    pOutValue->_v[2] = matrix._m[2][index];
    pOutValue->_v[3] = matrix._m[3][index];
}

NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutRow0->_v[0] = matrix._m[0][0];
    pOutRow0->_v[1] = matrix._m[1][0];
    pOutRow0->_v[2] = matrix._m[2][0];
    pOutRow0->_v[3] = matrix._m[3][0];

    pOutRow1->_v[0] = matrix._m[0][1];
    pOutRow1->_v[1] = matrix._m[1][1];
    pOutRow1->_v[2] = matrix._m[2][1];
    pOutRow1->_v[3] = matrix._m[3][1];

    pOutRow2->_v[0] = matrix._m[0][2];
    pOutRow2->_v[1] = matrix._m[1][2];
    pOutRow2->_v[2] = matrix._m[2][2];
    pOutRow2->_v[3] = matrix._m[3][2];

    pOutRow3->_v[0] = matrix._m[0][3];
    pOutRow3->_v[1] = matrix._m[1][3];
    pOutRow3->_v[2] = matrix._m[2][3];
    pOutRow3->_v[3] = matrix._m[3][3];
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[index][0] = vector._v[0];
    pOutValue->_m[index][1] = vector._v[1];
    pOutValue->_m[index][2] = vector._v[2];
    pOutValue->_m[index][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = column0._v[0];
    pOutValue->_m[0][1] = column0._v[1];
    pOutValue->_m[0][2] = column0._v[2];
    pOutValue->_m[0][3] = column0._v[3];

    pOutValue->_m[1][0] = column1._v[0];
    pOutValue->_m[1][1] = column1._v[1];
    pOutValue->_m[1][2] = column1._v[2];
    pOutValue->_m[1][3] = column1._v[3];

    pOutValue->_m[2][0] = column2._v[0];
    pOutValue->_m[2][1] = column2._v[1];
    pOutValue->_m[2][2] = column2._v[2];
    pOutValue->_m[2][3] = column2._v[3];

    pOutValue->_m[3][0] = column3._v[0];
    pOutValue->_m[3][1] = column3._v[1];
    pOutValue->_m[3][2] = column3._v[2];
    pOutValue->_m[3][3] = column3._v[3];
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[index][0];
    pOutValue->_v[1] = matrix._m[index][1];
    pOutValue->_v[2] = matrix._m[index][2];
    pOutValue->_v[3] = matrix._m[index][3];
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutColumn0->_v[0] = matrix._m[0][0];
    pOutColumn0->_v[1] = matrix._m[0][1];
    pOutColumn0->_v[2] = matrix._m[0][2];
    pOutColumn0->_v[3] = matrix._m[0][3];

    pOutColumn1->_v[0] = matrix._m[1][0];
    pOutColumn1->_v[1] = matrix._m[1][1];
    pOutColumn1->_v[2] = matrix._m[1][2];
    pOutColumn1->_v[3] = matrix._m[1][3];

    pOutColumn2->_v[0] = matrix._m[2][0];
    pOutColumn2->_v[1] = matrix._m[2][1];
    pOutColumn2->_v[2] = matrix._m[2][2];
    pOutColumn2->_v[3] = matrix._m[2][3];

    pOutColumn3->_v[0] = matrix._m[3][0];
    pOutColumn3->_v[1] = matrix._m[3][1];
    pOutColumn3->_v[2] = matrix._m[3][2];
    pOutColumn3->_v[3] = matrix._m[3][3];
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = vector._v[0];
    pOutValue->_m[1][0] = vector._v[1];
    pOutValue->_m[2][0] = vector._v[2];
    pOutValue->_m[3][0] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][1] = vector._v[0];
    pOutValue->_m[1][1] = vector._v[1];
    pOutValue->_m[2][1] = vector._v[2];
    pOutValue->_m[3][1] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][2] = vector._v[0];
    pOutValue->_m[1][2] = vector._v[1];
    pOutValue->_m[2][2] = vector._v[2];
    pOutValue->_m[3][2] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    pOutValue->_m[0][3] = vector._v[0];
    pOutValue->_m[1][3] = vector._v[1];
    pOutValue->_m[2][3] = vector._v[2];
    pOutValue->_m[3][3] = vector._v[3];
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = axisX._v[0];
    pOutValue->_m[1][0] = axisX._v[1];
    pOutValue->_m[2][0] = axisX._v[2];
    pOutValue->_m[3][0] = axisX._v[3];

    pOutValue->_m[0][1] = axisY._v[0];
    pOutValue->_m[1][1] = axisY._v[1];
    pOutValue->_m[2][1] = axisY._v[2];
    pOutValue->_m[3][1] = axisY._v[3];

    pOutValue->_m[0][2] = axisZ._v[0];
    pOutValue->_m[1][2] = axisZ._v[1];
    pOutValue->_m[2][2] = axisZ._v[2];
    pOutValue->_m[3][2] = axisZ._v[3];

    pOutValue->_m[0][3] = axisW._v[0];
    pOutValue->_m[1][3] = axisW._v[1];
    pOutValue->_m[2][3] = axisW._v[2];
    pOutValue->_m[3][3] = axisW._v[3];
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][0];
    pOutValue->_v[1] = matrix._m[1][0];
    pOutValue->_v[2] = matrix._m[2][0];
    pOutValue->_v[3] = matrix._m[3][0];
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][1];
    pOutValue->_v[1] = matrix._m[1][1];
    pOutValue->_v[2] = matrix._m[2][1];
    pOutValue->_v[3] = matrix._m[3][1];
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][2];
    pOutValue->_v[1] = matrix._m[1][2];
    pOutValue->_v[2] = matrix._m[2][2];
    pOutValue->_v[3] = matrix._m[3][2];
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v[0] = matrix._m[0][3];
    pOutValue->_v[1] = matrix._m[1][3];
    pOutValue->_v[2] = matrix._m[2][3];
    pOutValue->_v[3] = matrix._m[3][3];
}

NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutAxisX->_v[0] = matrix._m[0][0];
    pOutAxisX->_v[1] = matrix._m[1][0];
    pOutAxisX->_v[2] = matrix._m[2][0];
    pOutAxisX->_v[3] = matrix._m[3][0];

    pOutAxisY->_v[0] = matrix._m[0][1];
    pOutAxisY->_v[1] = matrix._m[1][1];
    pOutAxisY->_v[2] = matrix._m[2][1];
    pOutAxisY->_v[3] = matrix._m[3][1];

    pOutAxisZ->_v[0] = matrix._m[0][2];
    pOutAxisZ->_v[1] = matrix._m[1][2];
    pOutAxisZ->_v[2] = matrix._m[2][2];
    pOutAxisZ->_v[3] = matrix._m[3][2];

    pOutAxisW->_v[0] = matrix._m[0][3];
    pOutAxisW->_v[1] = matrix._m[1][3];
    pOutAxisW->_v[2] = matrix._m[2][3];
    pOutAxisW->_v[3] = matrix._m[3][3];
}

//
// 行列
//
// 3x2 行列
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = pOutValue->_m[0][1] =
    pOutValue->_m[1][0] = pOutValue->_m[1][1] =
    pOutValue->_m[2][0] = pOutValue->_m[2][1] = 0.f;
}

NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = 1.f;
    pOutValue->_m[0][1] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 1.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
}

NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] + matrix2._m[0][1];

    pOutValue->_m[1][0] = matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] + matrix2._m[1][1];

    pOutValue->_m[2][0] = matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] + matrix2._m[2][1];
}

NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] - matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] - matrix2._m[0][1];

    pOutValue->_m[1][0] = matrix1._m[1][0] - matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] - matrix2._m[1][1];

    pOutValue->_m[2][0] = matrix1._m[2][0] - matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] - matrix2._m[2][1];
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] * factor;
    pOutValue->_m[0][1] = matrix._m[0][1] * factor;

    pOutValue->_m[1][0] = matrix._m[1][0] * factor;
    pOutValue->_m[1][1] = matrix._m[1][1] * factor;

    pOutValue->_m[2][0] = matrix._m[2][0] * factor;
    pOutValue->_m[2][1] = matrix._m[2][1] * factor;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    MatrixRowMajor3x2fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[0][1] * matrix2._m[1][0];
    result._m[0][1] = matrix1._m[0][0] * matrix2._m[0][1] + matrix1._m[0][1] * matrix2._m[1][1];

    result._m[1][0] = matrix1._m[1][0] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[1][0];
    result._m[1][1] = matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[1][1] * matrix2._m[1][1];

    result._m[2][0] = matrix1._m[2][0] * matrix2._m[0][0] + matrix1._m[2][1] * matrix2._m[1][0] + matrix2._m[2][0];
    result._m[2][1] = matrix1._m[2][0] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[1][1] + matrix2._m[2][1];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor3x2fType* pOutValue, float t, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = t * matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = t * matrix1._m[0][1] + matrix2._m[0][1];

    pOutValue->_m[1][0] = t * matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = t * matrix1._m[1][1] + matrix2._m[1][1];

    pOutValue->_m[2][0] = t * matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = t * matrix1._m[2][1] + matrix2._m[2][1];
}

NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] / factor;
    pOutValue->_m[0][1] = matrix._m[0][1] / factor;

    pOutValue->_m[1][0] = matrix._m[1][0] / factor;
    pOutValue->_m[1][1] = matrix._m[1][1] / factor;

    pOutValue->_m[2][0] = matrix._m[2][0] / factor;
    pOutValue->_m[2][1] = matrix._m[2][1] / factor;
}

NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    float determinant =
        matrix._m[0][0] * matrix._m[1][1]
        - matrix._m[0][1] * matrix._m[1][0];

    if (determinant == 0.f)
    {
        MatrixZero(pOutValue);
        return false;
    }

    float reciprocalDeterminant = 1.f / determinant;

    MatrixRowMajor3x2fType result;

    result._m[0][0] =  matrix._m[1][1] * reciprocalDeterminant;
    result._m[0][1] = -matrix._m[0][1] * reciprocalDeterminant;

    result._m[1][0] = -matrix._m[1][0] * reciprocalDeterminant;
    result._m[1][1] =  matrix._m[0][0] * reciprocalDeterminant;

    result._m[2][0] = -result._m[0][0] * matrix._m[2][0] - result._m[1][0] * matrix._m[2][1];
    result._m[2][1] = -result._m[0][1] * matrix._m[2][0] - result._m[1][1] * matrix._m[2][1];

    *pOutValue = result;

    return true;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    MatrixRowMajor3x2fType result;

    result._m[0][0] = matrix._m[0][0];
    result._m[0][1] = matrix._m[1][0];

    result._m[1][0] = matrix._m[0][1];
    result._m[1][1] = matrix._m[1][1];

    result._m[2][0] = 0.f;
    result._m[2][1] = 0.f;

    *pOutValue = result;
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    MatrixRowMajor3x2fType result;

    if (!MatrixInverse(&result, matrix))
    {
        MatrixZero(pOutValue);
        return false;
    }

    MatrixTranspose(pOutValue, result);

    return true;
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT
{
    pOutValue->_m[2][0] = translate._v[0];
    pOutValue->_m[2][1] = translate._v[1];
}

NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = scale._v[0];
    pOutValue->_m[0][1] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = scale._v[1];
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT
{
    float sin, cos;
    nn::util::SinCosEst(&sin, &cos, rotationRadian);

    pOutValue->_m[0][0] = cos;
    pOutValue->_m[0][1] = sin;

    pOutValue->_m[1][0] = -sin;
    pOutValue->_m[1][1] = cos;
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT
{
    float sin, cos;
    nn::util::SinCosEst(&sin, &cos, rotationRadian);

    pOutValue->_m[0][0] = scale._v[0] * cos;
    pOutValue->_m[0][1] = scale._v[0] * sin;

    pOutValue->_m[1][0] = scale._v[1] * -sin;
    pOutValue->_m[1][1] = scale._v[1] * cos;
}

// 4x3 行列
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = pOutValue->_m[0][1] = pOutValue->_m[0][2] =
    pOutValue->_m[1][0] = pOutValue->_m[1][1] = pOutValue->_m[1][2] =
    pOutValue->_m[2][0] = pOutValue->_m[2][1] = pOutValue->_m[2][2] =
    pOutValue->_m[3][0] = pOutValue->_m[3][1] = pOutValue->_m[3][2] = 0.f;
}

NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = 1.f;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 1.f;
    pOutValue->_m[1][2] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = 1.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
}

NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] + matrix2._m[0][2];

    pOutValue->_m[1][0] = matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] + matrix2._m[1][2];

    pOutValue->_m[2][0] = matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] + matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] + matrix2._m[2][2];

    pOutValue->_m[3][0] = matrix1._m[3][0] + matrix2._m[3][0];
    pOutValue->_m[3][1] = matrix1._m[3][1] + matrix2._m[3][1];
    pOutValue->_m[3][2] = matrix1._m[3][2] + matrix2._m[3][2];
}

NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] - matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] - matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] - matrix2._m[0][2];

    pOutValue->_m[1][0] = matrix1._m[1][0] - matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] - matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] - matrix2._m[1][2];

    pOutValue->_m[2][0] = matrix1._m[2][0] - matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] - matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] - matrix2._m[2][2];

    pOutValue->_m[3][0] = matrix1._m[3][0] - matrix2._m[3][0];
    pOutValue->_m[3][1] = matrix1._m[3][1] - matrix2._m[3][1];
    pOutValue->_m[3][2] = matrix1._m[3][2] - matrix2._m[3][2];
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] * factor;
    pOutValue->_m[0][1] = matrix._m[0][1] * factor;
    pOutValue->_m[0][2] = matrix._m[0][2] * factor;

    pOutValue->_m[1][0] = matrix._m[1][0] * factor;
    pOutValue->_m[1][1] = matrix._m[1][1] * factor;
    pOutValue->_m[1][2] = matrix._m[1][2] * factor;

    pOutValue->_m[2][0] = matrix._m[2][0] * factor;
    pOutValue->_m[2][1] = matrix._m[2][1] * factor;
    pOutValue->_m[2][2] = matrix._m[2][2] * factor;

    pOutValue->_m[3][0] = matrix._m[3][0] * factor;
    pOutValue->_m[3][1] = matrix._m[3][1] * factor;
    pOutValue->_m[3][2] = matrix._m[3][2] * factor;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    MatrixRowMajor4x3fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[0][2] * matrix2._m[2][0];
    result._m[0][1] = matrix1._m[0][0] * matrix2._m[0][1] + matrix1._m[0][1] * matrix2._m[1][1] + matrix1._m[0][2] * matrix2._m[2][1];
    result._m[0][2] = matrix1._m[0][0] * matrix2._m[0][2] + matrix1._m[0][1] * matrix2._m[1][2] + matrix1._m[0][2] * matrix2._m[2][2];

    result._m[1][0] = matrix1._m[1][0] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[2][0];
    result._m[1][1] = matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[1][2] * matrix2._m[2][1];
    result._m[1][2] = matrix1._m[1][0] * matrix2._m[0][2] + matrix1._m[1][1] * matrix2._m[1][2] + matrix1._m[1][2] * matrix2._m[2][2];

    result._m[2][0] = matrix1._m[2][0] * matrix2._m[0][0] + matrix1._m[2][1] * matrix2._m[1][0] + matrix1._m[2][2] * matrix2._m[2][0];
    result._m[2][1] = matrix1._m[2][0] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[2][1];
    result._m[2][2] = matrix1._m[2][0] * matrix2._m[0][2] + matrix1._m[2][1] * matrix2._m[1][2] + matrix1._m[2][2] * matrix2._m[2][2];

    result._m[3][0] = matrix1._m[3][0] * matrix2._m[0][0] + matrix1._m[3][1] * matrix2._m[1][0] + matrix1._m[3][2] * matrix2._m[2][0] + matrix2._m[3][0];
    result._m[3][1] = matrix1._m[3][0] * matrix2._m[0][1] + matrix1._m[3][1] * matrix2._m[1][1] + matrix1._m[3][2] * matrix2._m[2][1] + matrix2._m[3][1];
    result._m[3][2] = matrix1._m[3][0] * matrix2._m[0][2] + matrix1._m[3][1] * matrix2._m[1][2] + matrix1._m[3][2] * matrix2._m[2][2] + matrix2._m[3][2];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    MatrixRowMajor4x4fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[0][2] * matrix2._m[2][0];
    result._m[0][1] = matrix1._m[0][0] * matrix2._m[0][1] + matrix1._m[0][1] * matrix2._m[1][1] + matrix1._m[0][2] * matrix2._m[2][1];
    result._m[0][2] = matrix1._m[0][0] * matrix2._m[0][2] + matrix1._m[0][1] * matrix2._m[1][2] + matrix1._m[0][2] * matrix2._m[2][2];
    result._m[0][3] = matrix1._m[0][0] * matrix2._m[0][3] + matrix1._m[0][1] * matrix2._m[1][3] + matrix1._m[0][2] * matrix2._m[2][3];

    result._m[1][0] = matrix1._m[1][0] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[2][0];
    result._m[1][1] = matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[1][2] * matrix2._m[2][1];
    result._m[1][2] = matrix1._m[1][0] * matrix2._m[0][2] + matrix1._m[1][1] * matrix2._m[1][2] + matrix1._m[1][2] * matrix2._m[2][2];
    result._m[1][3] = matrix1._m[1][0] * matrix2._m[0][3] + matrix1._m[1][1] * matrix2._m[1][3] + matrix1._m[1][2] * matrix2._m[2][3];

    result._m[2][0] = matrix1._m[2][0] * matrix2._m[0][0] + matrix1._m[2][1] * matrix2._m[1][0] + matrix1._m[2][2] * matrix2._m[2][0];
    result._m[2][1] = matrix1._m[2][0] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[2][1];
    result._m[2][2] = matrix1._m[2][0] * matrix2._m[0][2] + matrix1._m[2][1] * matrix2._m[1][2] + matrix1._m[2][2] * matrix2._m[2][2];
    result._m[2][3] = matrix1._m[2][0] * matrix2._m[0][3] + matrix1._m[2][1] * matrix2._m[1][3] + matrix1._m[2][2] * matrix2._m[2][3];

    result._m[3][0] = matrix1._m[3][0] * matrix2._m[0][0] + matrix1._m[3][1] * matrix2._m[1][0] + matrix1._m[3][2] * matrix2._m[2][0] + matrix2._m[3][0];
    result._m[3][1] = matrix1._m[3][0] * matrix2._m[0][1] + matrix1._m[3][1] * matrix2._m[1][1] + matrix1._m[3][2] * matrix2._m[2][1] + matrix2._m[3][1];
    result._m[3][2] = matrix1._m[3][0] * matrix2._m[0][2] + matrix1._m[3][1] * matrix2._m[1][2] + matrix1._m[3][2] * matrix2._m[2][2] + matrix2._m[3][2];
    result._m[3][3] = matrix1._m[3][0] * matrix2._m[0][3] + matrix1._m[3][1] * matrix2._m[1][3] + matrix1._m[3][2] * matrix2._m[2][3] + matrix2._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor4x3fType* pOutValue, float t, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = t * matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = t * matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = t * matrix1._m[0][2] + matrix2._m[0][2];

    pOutValue->_m[1][0] = t * matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = t * matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = t * matrix1._m[1][2] + matrix2._m[1][2];

    pOutValue->_m[2][0] = t * matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = t * matrix1._m[2][1] + matrix2._m[2][1];
    pOutValue->_m[2][2] = t * matrix1._m[2][2] + matrix2._m[2][2];

    pOutValue->_m[3][0] = t * matrix1._m[3][0] + matrix2._m[3][0];
    pOutValue->_m[3][1] = t * matrix1._m[3][1] + matrix2._m[3][1];
    pOutValue->_m[3][2] = t * matrix1._m[3][2] + matrix2._m[3][2];
}

NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] / factor;
    pOutValue->_m[0][1] = matrix._m[0][1] / factor;
    pOutValue->_m[0][2] = matrix._m[0][2] / factor;

    pOutValue->_m[1][0] = matrix._m[1][0] / factor;
    pOutValue->_m[1][1] = matrix._m[1][1] / factor;
    pOutValue->_m[1][2] = matrix._m[1][2] / factor;

    pOutValue->_m[2][0] = matrix._m[2][0] / factor;
    pOutValue->_m[2][1] = matrix._m[2][1] / factor;
    pOutValue->_m[2][2] = matrix._m[2][2] / factor;

    pOutValue->_m[3][0] = matrix._m[3][0] / factor;
    pOutValue->_m[3][1] = matrix._m[3][1] / factor;
    pOutValue->_m[3][2] = matrix._m[3][2] / factor;
}

NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    float determinant =
        matrix._m[0][0] * matrix._m[1][1] * matrix._m[2][2]
      + matrix._m[1][0] * matrix._m[2][1] * matrix._m[0][2]
      + matrix._m[2][0] * matrix._m[0][1] * matrix._m[1][2]
      - matrix._m[0][2] * matrix._m[1][1] * matrix._m[2][0]
      - matrix._m[0][1] * matrix._m[1][0] * matrix._m[2][2]
      - matrix._m[0][0] * matrix._m[1][2] * matrix._m[2][1];

    if(determinant == 0.f)
    {
        MatrixZero(pOutValue);
        return false;
    }

    float reciprocalDeterminant = 1.f / determinant;

    MatrixRowMajor4x3fType result;

    result._m[0][0] = (matrix._m[1][1] * matrix._m[2][2] - matrix._m[1][2] * matrix._m[2][1]) * reciprocalDeterminant;
    result._m[0][1] = (matrix._m[0][2] * matrix._m[2][1] - matrix._m[0][1] * matrix._m[2][2]) * reciprocalDeterminant;
    result._m[0][2] = (matrix._m[0][1] * matrix._m[1][2] - matrix._m[0][2] * matrix._m[1][1]) * reciprocalDeterminant;

    result._m[1][0] = (matrix._m[1][2] * matrix._m[2][0] - matrix._m[1][0] * matrix._m[2][2]) * reciprocalDeterminant;
    result._m[1][1] = (matrix._m[0][0] * matrix._m[2][2] - matrix._m[0][2] * matrix._m[2][0]) * reciprocalDeterminant;
    result._m[1][2] = (matrix._m[0][2] * matrix._m[1][0] - matrix._m[0][0] * matrix._m[1][2]) * reciprocalDeterminant;

    result._m[2][0] = (matrix._m[1][0] * matrix._m[2][1] - matrix._m[1][1] * matrix._m[2][0]) * reciprocalDeterminant;
    result._m[2][1] = (matrix._m[0][1] * matrix._m[2][0] - matrix._m[0][0] * matrix._m[2][1]) * reciprocalDeterminant;
    result._m[2][2] = (matrix._m[0][0] * matrix._m[1][1] - matrix._m[0][1] * matrix._m[1][0]) * reciprocalDeterminant;

    result._m[3][0] = -result._m[0][0] * matrix._m[3][0] - result._m[1][0] * matrix._m[3][1] - result._m[2][0] * matrix._m[3][2];
    result._m[3][1] = -result._m[0][1] * matrix._m[3][0] - result._m[1][1] * matrix._m[3][1] - result._m[2][1] * matrix._m[3][2];
    result._m[3][2] = -result._m[0][2] * matrix._m[3][0] - result._m[1][2] * matrix._m[3][1] - result._m[2][2] * matrix._m[3][2];

    *pOutValue = result;

    return true;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    MatrixRowMajor4x3fType result;

    result._m[0][0] = matrix._m[0][0];
    result._m[0][1] = matrix._m[1][0];
    result._m[0][2] = matrix._m[2][0];

    result._m[1][0] = matrix._m[0][1];
    result._m[1][1] = matrix._m[1][1];
    result._m[1][2] = matrix._m[2][1];

    result._m[2][0] = matrix._m[0][2];
    result._m[2][1] = matrix._m[1][2];
    result._m[2][2] = matrix._m[2][2];

    result._m[3][0] = 0.f;
    result._m[3][1] = 0.f;
    result._m[3][2] = 0.f;

    *pOutValue = result;
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    MatrixRowMajor4x3fType result;

    if(!MatrixInverse(&result, matrix))
    {
        MatrixZero(pOutValue);
        return false;
    }

    MatrixTranspose(pOutValue, result);

    return true;
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT
{
    Vector3fType lookVector;
    VectorSubtract(&lookVector, cameraPosition, target);
    VectorNormalize(&lookVector, lookVector);

    Vector3fType rightVector;
    VectorCross(&rightVector, up, lookVector);
    VectorNormalize(&rightVector, rightVector);

    Vector3fType upVector;
    VectorCross(&upVector, lookVector, rightVector);

    pOutValue->_m[0][0] = rightVector._v[0];
    pOutValue->_m[0][1] = upVector._v[0];
    pOutValue->_m[0][2] = lookVector._v[0];

    pOutValue->_m[1][0] = rightVector._v[1];
    pOutValue->_m[1][1] = upVector._v[1];
    pOutValue->_m[1][2] = lookVector._v[1];

    pOutValue->_m[2][0] = rightVector._v[2];
    pOutValue->_m[2][1] = upVector._v[2];
    pOutValue->_m[2][2] = lookVector._v[2];

    pOutValue->_m[3][0] = -VectorDot(cameraPosition, rightVector);
    pOutValue->_m[3][1] = -VectorDot(cameraPosition, upVector);
    pOutValue->_m[3][2] = -VectorDot(cameraPosition, lookVector);
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT
{
    Vector3fType lookReverse;
    VectorSubtract(&lookReverse, cameraPosition, target);

    if(lookReverse._v[0] == 0.f && lookReverse._v[2] == 0.f)
    {
        pOutValue->_m[0][0] = 1.f;
        pOutValue->_m[0][1] = 0.f;
        pOutValue->_m[0][2] = 0.f;

        pOutValue->_m[1][0] = 0.f;
        pOutValue->_m[1][1] = 0.f;

        pOutValue->_m[2][0] = 0.f;
        pOutValue->_m[2][2] = 0.f;

        pOutValue->_m[3][0] = -cameraPosition._v[0];

        if(lookReverse._v[1] <= 0.f)
        {
            pOutValue->_m[1][2] = -1.f;
            pOutValue->_m[2][1] = 1.f;

            pOutValue->_m[3][1] = -cameraPosition._v[2];
            pOutValue->_m[3][2] = cameraPosition._v[1];
        }
        else
        {
            pOutValue->_m[1][2] = 1.f;
            pOutValue->_m[2][1] = -1.f;

            pOutValue->_m[3][1] = cameraPosition._v[2];
            pOutValue->_m[3][2] = -cameraPosition._v[1];
        }
    }
    else
    {
        Vector3fType r;
        VectorSet(&r, lookReverse._v[2], 0.f, -lookReverse._v[0]);

        VectorNormalize(&lookReverse, lookReverse);
        VectorNormalize(&r, r);

        Vector3fType u;
        VectorCross(&u, lookReverse, r);

        float twistSin, twistCos;
        nn::util::SinCosEst(&twistSin, &twistCos, twistRadian);

        Vector3fType right;
        right._v[0] = twistSin * u._v[0] + twistCos * r._v[0];
        right._v[1] = twistSin * u._v[1];
        right._v[2] = twistSin * u._v[2] + twistCos * r._v[2];

        Vector3fType up;
        up._v[0] = twistCos * u._v[0] - twistSin * r._v[0];
        up._v[1] = twistCos * u._v[1];
        up._v[2] = twistCos * u._v[2] - twistSin * r._v[2];

        pOutValue->_m[0][0] = right._v[0];
        pOutValue->_m[0][1] = up._v[0];
        pOutValue->_m[0][2] = lookReverse._v[0];

        pOutValue->_m[1][0] = right._v[1];
        pOutValue->_m[1][1] = up._v[1];
        pOutValue->_m[1][2] = lookReverse._v[1];

        pOutValue->_m[2][0] = right._v[2];
        pOutValue->_m[2][1] = up._v[2];
        pOutValue->_m[2][2] = lookReverse._v[2];

        pOutValue->_m[3][0] = -VectorDot(cameraPosition, right);
        pOutValue->_m[3][1] = -VectorDot(cameraPosition, up);
        pOutValue->_m[3][2] = -VectorDot(cameraPosition, lookReverse);
    }
}

NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    float sinX, cosX, sinY, cosY, sinZ, cosZ;
    nn::util::SinCosEst(&sinX, &cosX, rotationRadian._v[0]);
    nn::util::SinCosEst(&sinY, &cosY, rotationRadian._v[1]);
    nn::util::SinCosEst(&sinZ, &cosZ, rotationRadian._v[2]);

    Vector3fType right, up, back;

    right._v[0] = sinX * sinY * sinZ + cosY * cosZ;
    right._v[1] = cosX * sinZ;
    right._v[2] = sinX * cosY * sinZ - sinY * cosZ;

    up._v[0] = sinX * sinY * cosZ - cosY * sinZ;
    up._v[1] = cosX * cosZ;
    up._v[2] = sinX * cosY * cosZ + sinY * sinZ;

    back._v[0] = cosX * sinY;
    back._v[1] = -sinX;
    back._v[2] = cosX * cosY;

    pOutValue->_m[0][0] = right._v[0];
    pOutValue->_m[0][1] = up._v[0];
    pOutValue->_m[0][2] = back._v[0];

    pOutValue->_m[1][0] = right._v[1];
    pOutValue->_m[1][1] = up._v[1];
    pOutValue->_m[1][2] = back._v[1];

    pOutValue->_m[2][0] = right._v[2];
    pOutValue->_m[2][1] = up._v[2];
    pOutValue->_m[2][2] = back._v[2];

    pOutValue->_m[3][0] = -VectorDot(cameraPosition, right);
    pOutValue->_m[3][1] = -VectorDot(cameraPosition, up);
    pOutValue->_m[3][2] = -VectorDot(cameraPosition, back);
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT
{
    pOutValue->_m[3][0] = translate._v[0];
    pOutValue->_m[3][1] = translate._v[1];
    pOutValue->_m[3][2] = translate._v[2];
}

NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = scale._v[0];
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = scale._v[1];
    pOutValue->_m[1][2] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = scale._v[2];
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    MatrixRowMajor4x3fType rotationMatrix;

    MatrixFromQuaternion(&rotationMatrix, rotationQuaternion);

    pOutValue->_m[0][0] = rotationMatrix._m[0][0];
    pOutValue->_m[0][1] = rotationMatrix._m[0][1];
    pOutValue->_m[0][2] = rotationMatrix._m[0][2];

    pOutValue->_m[1][0] = rotationMatrix._m[1][0];
    pOutValue->_m[1][1] = rotationMatrix._m[1][1];
    pOutValue->_m[1][2] = rotationMatrix._m[1][2];

    pOutValue->_m[2][0] = rotationMatrix._m[2][0];
    pOutValue->_m[2][1] = rotationMatrix._m[2][1];
    pOutValue->_m[2][2] = rotationMatrix._m[2][2];
}

NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    float sinX, cosX, sinY, cosY, sinZ, cosZ;
    nn::util::SinCosEst(&sinX, &cosX, rotationRadian._v[0]);
    nn::util::SinCosEst(&sinY, &cosY, rotationRadian._v[1]);
    nn::util::SinCosEst(&sinZ, &cosZ, rotationRadian._v[2]);

    float opt1 = cosX * cosZ;
    float opt2 = sinX * sinY;
    float opt3 = cosX * sinZ;

    pOutValue->_m[0][0] = cosY * cosZ;
    pOutValue->_m[0][1] = cosY * sinZ;
    pOutValue->_m[0][2] = -sinY;

    pOutValue->_m[1][0] = (opt2 * cosZ) - opt3;
    pOutValue->_m[1][1] = (opt2 * sinZ) + opt1;
    pOutValue->_m[1][2] = sinX * cosY;

    pOutValue->_m[2][0] = (opt1 * sinY) + (sinX * sinZ);
    pOutValue->_m[2][1] = (opt3 * sinY) - (sinX * cosZ);
    pOutValue->_m[2][2] = cosX * cosY;
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    MatrixRowMajor4x3fType rotationMatrix;

    MatrixFromQuaternion(&rotationMatrix, rotationQuaternion);

    pOutValue->_m[0][0] = scale._v[0] * rotationMatrix._m[0][0];
    pOutValue->_m[0][1] = scale._v[0] * rotationMatrix._m[0][1];
    pOutValue->_m[0][2] = scale._v[0] * rotationMatrix._m[0][2];

    pOutValue->_m[1][0] = scale._v[1] * rotationMatrix._m[1][0];
    pOutValue->_m[1][1] = scale._v[1] * rotationMatrix._m[1][1];
    pOutValue->_m[1][2] = scale._v[1] * rotationMatrix._m[1][2];

    pOutValue->_m[2][0] = scale._v[2] * rotationMatrix._m[2][0];
    pOutValue->_m[2][1] = scale._v[2] * rotationMatrix._m[2][1];
    pOutValue->_m[2][2] = scale._v[2] * rotationMatrix._m[2][2];
}

NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    float sinX, cosX, sinY, cosY, sinZ, cosZ;
    nn::util::SinCosEst(&sinX, &cosX, rotationRadian._v[0]);
    nn::util::SinCosEst(&sinY, &cosY, rotationRadian._v[1]);
    nn::util::SinCosEst(&sinZ, &cosZ, rotationRadian._v[2]);

    float opt1 = cosX * cosZ;
    float opt2 = sinX * sinY;
    float opt3 = cosX * sinZ;

    pOutValue->_m[0][0] = scale._v[0] * (cosY * cosZ);
    pOutValue->_m[0][1] = scale._v[0] * (cosY * sinZ);
    pOutValue->_m[0][2] = scale._v[0] * (-sinY);

    pOutValue->_m[1][0] = scale._v[1] * ((opt2 * cosZ) - opt3);
    pOutValue->_m[1][1] = scale._v[1] * ((opt2 * sinZ) + opt1);
    pOutValue->_m[1][2] = scale._v[1] * (sinX * cosY);

    pOutValue->_m[2][0] = scale._v[2] * ((opt1 * sinY) + (sinX * sinZ));
    pOutValue->_m[2][1] = scale._v[2] * ((opt3 * sinY) - (sinX * cosZ));
    pOutValue->_m[2][2] = scale._v[2] * (cosX * cosY);
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    pOutValue->_m[0][0] = (2.f * nearZ) * reciprocalWidth * scaleS;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = (2.f * nearZ) * reciprocalHeight * scaleT;
    pOutValue->_m[1][2] = 0.f;

    pOutValue->_m[2][0] = (right + left) * reciprocalWidth * scaleS - translateS;
    pOutValue->_m[2][1] = (top + bottom) * reciprocalHeight * scaleT - translateT;
    pOutValue->_m[2][2] = -1.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float angle = fovyRadian * 0.5f;
    float cotangent = 1.f / nn::util::TanEst(angle);

    pOutValue->_m[0][0] = cotangent / aspect * scaleS;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = cotangent * scaleT;
    pOutValue->_m[1][2] = 0.f;

    pOutValue->_m[2][0] = -translateS;
    pOutValue->_m[2][1] = -translateT;
    pOutValue->_m[2][2] = -1.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
}

NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    pOutValue->_m[0][0] = 2.f * reciprocalWidth * scaleS;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 2.f * reciprocalHeight * scaleT;
    pOutValue->_m[1][2] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = 0.f;

    pOutValue->_m[3][0] = -(right + left) * reciprocalWidth * scaleS + translateS;
    pOutValue->_m[3][1] = -(top + bottom) * reciprocalHeight * scaleT + translateT;
    pOutValue->_m[3][2] = 1.f;
}

// 4x4 行列
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = pOutValue->_m[0][1] = pOutValue->_m[0][2] = pOutValue->_m[0][3] =
    pOutValue->_m[1][0] = pOutValue->_m[1][1] = pOutValue->_m[1][2] = pOutValue->_m[1][3] =
    pOutValue->_m[2][0] = pOutValue->_m[2][1] = pOutValue->_m[2][2] = pOutValue->_m[2][3] =
    pOutValue->_m[3][0] = pOutValue->_m[3][1] = pOutValue->_m[3][2] = pOutValue->_m[3][3] = 0.f;
}

NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = 1.f;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 1.f;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = 1.f;
    pOutValue->_m[2][3] = 0.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
    pOutValue->_m[3][3] = 1.f;
}

NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] + matrix2._m[0][2];
    pOutValue->_m[0][3] = matrix1._m[0][3] + matrix2._m[0][3];

    pOutValue->_m[1][0] = matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] + matrix2._m[1][2];
    pOutValue->_m[1][3] = matrix1._m[1][3] + matrix2._m[1][3];

    pOutValue->_m[2][0] = matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] + matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] + matrix2._m[2][2];
    pOutValue->_m[2][3] = matrix1._m[2][3] + matrix2._m[2][3];

    pOutValue->_m[3][0] = matrix1._m[3][0] + matrix2._m[3][0];
    pOutValue->_m[3][1] = matrix1._m[3][1] + matrix2._m[3][1];
    pOutValue->_m[3][2] = matrix1._m[3][2] + matrix2._m[3][2];
    pOutValue->_m[3][3] = matrix1._m[3][3] + matrix2._m[3][3];
}

NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] - matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] - matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] - matrix2._m[0][2];
    pOutValue->_m[0][3] = matrix1._m[0][3] - matrix2._m[0][3];

    pOutValue->_m[1][0] = matrix1._m[1][0] - matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] - matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] - matrix2._m[1][2];
    pOutValue->_m[1][3] = matrix1._m[1][3] - matrix2._m[1][3];

    pOutValue->_m[2][0] = matrix1._m[2][0] - matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] - matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] - matrix2._m[2][2];
    pOutValue->_m[2][3] = matrix1._m[2][3] - matrix2._m[2][3];

    pOutValue->_m[3][0] = matrix1._m[3][0] - matrix2._m[3][0];
    pOutValue->_m[3][1] = matrix1._m[3][1] - matrix2._m[3][1];
    pOutValue->_m[3][2] = matrix1._m[3][2] - matrix2._m[3][2];
    pOutValue->_m[3][3] = matrix1._m[3][3] - matrix2._m[3][3];
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] * factor;
    pOutValue->_m[0][1] = matrix._m[0][1] * factor;
    pOutValue->_m[0][2] = matrix._m[0][2] * factor;
    pOutValue->_m[0][3] = matrix._m[0][3] * factor;

    pOutValue->_m[1][0] = matrix._m[1][0] * factor;
    pOutValue->_m[1][1] = matrix._m[1][1] * factor;
    pOutValue->_m[1][2] = matrix._m[1][2] * factor;
    pOutValue->_m[1][3] = matrix._m[1][3] * factor;

    pOutValue->_m[2][0] = matrix._m[2][0] * factor;
    pOutValue->_m[2][1] = matrix._m[2][1] * factor;
    pOutValue->_m[2][2] = matrix._m[2][2] * factor;
    pOutValue->_m[2][3] = matrix._m[2][3] * factor;

    pOutValue->_m[3][0] = matrix._m[3][0] * factor;
    pOutValue->_m[3][1] = matrix._m[3][1] * factor;
    pOutValue->_m[3][2] = matrix._m[3][2] * factor;
    pOutValue->_m[3][3] = matrix._m[3][3] * factor;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    MatrixRowMajor4x4fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[0][2] * matrix2._m[2][0] + matrix1._m[0][3] * matrix2._m[3][0];
    result._m[0][1] = matrix1._m[0][0] * matrix2._m[0][1] + matrix1._m[0][1] * matrix2._m[1][1] + matrix1._m[0][2] * matrix2._m[2][1] + matrix1._m[0][3] * matrix2._m[3][1];
    result._m[0][2] = matrix1._m[0][0] * matrix2._m[0][2] + matrix1._m[0][1] * matrix2._m[1][2] + matrix1._m[0][2] * matrix2._m[2][2] + matrix1._m[0][3] * matrix2._m[3][2];
    result._m[0][3] = matrix1._m[0][3];

    result._m[1][0] = matrix1._m[1][0] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[2][0] + matrix1._m[1][3] * matrix2._m[3][0];
    result._m[1][1] = matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[1][2] * matrix2._m[2][1] + matrix1._m[1][3] * matrix2._m[3][1];
    result._m[1][2] = matrix1._m[1][0] * matrix2._m[0][2] + matrix1._m[1][1] * matrix2._m[1][2] + matrix1._m[1][2] * matrix2._m[2][2] + matrix1._m[1][3] * matrix2._m[3][2];
    result._m[1][3] = matrix1._m[1][3];

    result._m[2][0] = matrix1._m[2][0] * matrix2._m[0][0] + matrix1._m[2][1] * matrix2._m[1][0] + matrix1._m[2][2] * matrix2._m[2][0] + matrix1._m[2][3] * matrix2._m[3][0];
    result._m[2][1] = matrix1._m[2][0] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[2][1] + matrix1._m[2][3] * matrix2._m[3][1];
    result._m[2][2] = matrix1._m[2][0] * matrix2._m[0][2] + matrix1._m[2][1] * matrix2._m[1][2] + matrix1._m[2][2] * matrix2._m[2][2] + matrix1._m[2][3] * matrix2._m[3][2];
    result._m[2][3] = matrix1._m[2][3];

    result._m[3][0] = matrix1._m[3][0] * matrix2._m[0][0] + matrix1._m[3][1] * matrix2._m[1][0] + matrix1._m[3][2] * matrix2._m[2][0] + matrix1._m[3][3] * matrix2._m[3][0];
    result._m[3][1] = matrix1._m[3][0] * matrix2._m[0][1] + matrix1._m[3][1] * matrix2._m[1][1] + matrix1._m[3][2] * matrix2._m[2][1] + matrix1._m[3][3] * matrix2._m[3][1];
    result._m[3][2] = matrix1._m[3][0] * matrix2._m[0][2] + matrix1._m[3][1] * matrix2._m[1][2] + matrix1._m[3][2] * matrix2._m[2][2] + matrix1._m[3][3] * matrix2._m[3][2];
    result._m[3][3] = matrix1._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    MatrixRowMajor4x4fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[0][2] * matrix2._m[2][0] + matrix1._m[0][3] * matrix2._m[3][0];
    result._m[0][1] = matrix1._m[0][0] * matrix2._m[0][1] + matrix1._m[0][1] * matrix2._m[1][1] + matrix1._m[0][2] * matrix2._m[2][1] + matrix1._m[0][3] * matrix2._m[3][1];
    result._m[0][2] = matrix1._m[0][0] * matrix2._m[0][2] + matrix1._m[0][1] * matrix2._m[1][2] + matrix1._m[0][2] * matrix2._m[2][2] + matrix1._m[0][3] * matrix2._m[3][2];
    result._m[0][3] = matrix1._m[0][0] * matrix2._m[0][3] + matrix1._m[0][1] * matrix2._m[1][3] + matrix1._m[0][2] * matrix2._m[2][3] + matrix1._m[0][3] * matrix2._m[3][3];

    result._m[1][0] = matrix1._m[1][0] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[2][0] + matrix1._m[1][3] * matrix2._m[3][0];
    result._m[1][1] = matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[1][2] * matrix2._m[2][1] + matrix1._m[1][3] * matrix2._m[3][1];
    result._m[1][2] = matrix1._m[1][0] * matrix2._m[0][2] + matrix1._m[1][1] * matrix2._m[1][2] + matrix1._m[1][2] * matrix2._m[2][2] + matrix1._m[1][3] * matrix2._m[3][2];
    result._m[1][3] = matrix1._m[1][0] * matrix2._m[0][3] + matrix1._m[1][1] * matrix2._m[1][3] + matrix1._m[1][2] * matrix2._m[2][3] + matrix1._m[1][3] * matrix2._m[3][3];

    result._m[2][0] = matrix1._m[2][0] * matrix2._m[0][0] + matrix1._m[2][1] * matrix2._m[1][0] + matrix1._m[2][2] * matrix2._m[2][0] + matrix1._m[2][3] * matrix2._m[3][0];
    result._m[2][1] = matrix1._m[2][0] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[2][1] + matrix1._m[2][3] * matrix2._m[3][1];
    result._m[2][2] = matrix1._m[2][0] * matrix2._m[0][2] + matrix1._m[2][1] * matrix2._m[1][2] + matrix1._m[2][2] * matrix2._m[2][2] + matrix1._m[2][3] * matrix2._m[3][2];
    result._m[2][3] = matrix1._m[2][0] * matrix2._m[0][3] + matrix1._m[2][1] * matrix2._m[1][3] + matrix1._m[2][2] * matrix2._m[2][3] + matrix1._m[2][3] * matrix2._m[3][3];

    result._m[3][0] = matrix1._m[3][0] * matrix2._m[0][0] + matrix1._m[3][1] * matrix2._m[1][0] + matrix1._m[3][2] * matrix2._m[2][0] + matrix1._m[3][3] * matrix2._m[3][0];
    result._m[3][1] = matrix1._m[3][0] * matrix2._m[0][1] + matrix1._m[3][1] * matrix2._m[1][1] + matrix1._m[3][2] * matrix2._m[2][1] + matrix1._m[3][3] * matrix2._m[3][1];
    result._m[3][2] = matrix1._m[3][0] * matrix2._m[0][2] + matrix1._m[3][1] * matrix2._m[1][2] + matrix1._m[3][2] * matrix2._m[2][2] + matrix1._m[3][3] * matrix2._m[3][2];
    result._m[3][3] = matrix1._m[3][0] * matrix2._m[0][3] + matrix1._m[3][1] * matrix2._m[1][3] + matrix1._m[3][2] * matrix2._m[2][3] + matrix1._m[3][3] * matrix2._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] / factor;
    pOutValue->_m[0][1] = matrix._m[0][1] / factor;
    pOutValue->_m[0][2] = matrix._m[0][2] / factor;
    pOutValue->_m[0][3] = matrix._m[0][3] / factor;

    pOutValue->_m[1][0] = matrix._m[1][0] / factor;
    pOutValue->_m[1][1] = matrix._m[1][1] / factor;
    pOutValue->_m[1][2] = matrix._m[1][2] / factor;
    pOutValue->_m[1][3] = matrix._m[1][3] / factor;

    pOutValue->_m[2][0] = matrix._m[2][0] / factor;
    pOutValue->_m[2][1] = matrix._m[2][1] / factor;
    pOutValue->_m[2][2] = matrix._m[2][2] / factor;
    pOutValue->_m[2][3] = matrix._m[2][3] / factor;

    pOutValue->_m[3][0] = matrix._m[3][0] / factor;
    pOutValue->_m[3][1] = matrix._m[3][1] / factor;
    pOutValue->_m[3][2] = matrix._m[3][2] / factor;
    pOutValue->_m[3][3] = matrix._m[3][3] / factor;
}

NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    MatrixRowMajor4x4fType sourceMatrix(matrix);

    MatrixIdentity(pOutValue);

    float (*source)[4] = sourceMatrix._m;
    float (*result)[4] = pOutValue->_m;

    float w;

    for (int i = 0; i < 4; ++i)
    {
        float max = 0.0f;
        int swap = i;

        for(int k = i ; k < 4 ; k++)
        {
            float ftmp = ::std::abs(source[k][i]);

            if (ftmp > max)
            {
                max = ftmp;
                swap = k;
            }
        }

        if ( max == 0.0f )
        {
            MatrixZero(pOutValue);
            return false;
        }

        if ( swap != i )
        {
            for (int k = 0; k < 4; k++)
            {
                ::std::swap(source[i][k], source[swap][k]);
                ::std::swap(result[i][k], result[swap][k]);
            }
        }

        w = 1.0f / source[i][i];

        for (int j = 0; j < 4; ++j)
        {
            source[i][j] *= w;
            result[i][j] *= w;
        }

        for (int k = 0; k < 4; ++k )
        {
            if ( k == i )
            {
                continue;
            }

            w = source[k][i];

            for (int j = 0; j < 4; ++j)
            {
                source[k][j] -= source[i][j] * w;
                result[k][j] -= result[i][j] * w;
            }
        }
    }

    return true;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    MatrixRowMajor4x4fType result;

    result._m[0][0] = matrix._m[0][0];
    result._m[0][1] = matrix._m[1][0];
    result._m[0][2] = matrix._m[2][0];
    result._m[0][3] = matrix._m[3][0];

    result._m[1][0] = matrix._m[0][1];
    result._m[1][1] = matrix._m[1][1];
    result._m[1][2] = matrix._m[2][1];
    result._m[1][3] = matrix._m[3][1];

    result._m[2][0] = matrix._m[0][2];
    result._m[2][1] = matrix._m[1][2];
    result._m[2][2] = matrix._m[2][2];
    result._m[2][3] = matrix._m[3][2];

    result._m[3][0] = matrix._m[0][3];
    result._m[3][1] = matrix._m[1][3];
    result._m[3][2] = matrix._m[2][3];
    result._m[3][3] = matrix._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    pOutValue->_m[0][0] = (2 * nearZ) * reciprocalWidth;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = (2 * nearZ) * reciprocalHeight;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = (right + left) * reciprocalWidth;
    pOutValue->_m[2][1] = (top + bottom) * reciprocalHeight;
    pOutValue->_m[2][2] = farZ * reciprocalDepth;
    pOutValue->_m[2][3] = -1.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = (farZ * nearZ) * reciprocalDepth;
    pOutValue->_m[3][3] = 0.f;
}

NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT
{
    float angle = fovyRadian * 0.5f;
    float cotangent = 1.f / nn::util::TanEst(angle);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    pOutValue->_m[0][0] = cotangent / aspect;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = cotangent;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = farZ * reciprocalDepth;
    pOutValue->_m[2][3] = -1.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = (farZ * nearZ) * reciprocalDepth;
    pOutValue->_m[3][3] = 0.f;
}

NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    pOutValue->_m[0][0] = 2.f * reciprocalWidth;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 2.f * reciprocalHeight;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = reciprocalDepth;
    pOutValue->_m[2][3] = 0.f;

    pOutValue->_m[3][0] = -(right + left) * reciprocalWidth;
    pOutValue->_m[3][1] = -(top + bottom) * reciprocalHeight;
    pOutValue->_m[3][2] = nearZ * reciprocalDepth;
    pOutValue->_m[3][3] = 1.f;
}

// T3x2 行列
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = pOutValue->_m[0][1] = pOutValue->_m[0][2] =
    pOutValue->_m[1][0] = pOutValue->_m[1][1] = pOutValue->_m[1][2] = 0.f;
}

NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = 1.f;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 1.f;
    pOutValue->_m[1][2] = 0.f;
}

NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] + matrix2._m[0][2];

    pOutValue->_m[1][0] = matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] + matrix2._m[1][2];
}

NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] - matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] - matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] - matrix2._m[0][2];

    pOutValue->_m[1][0] = matrix1._m[1][0] - matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] - matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] - matrix2._m[1][2];
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] * factor;
    pOutValue->_m[0][1] = matrix._m[0][1] * factor;
    pOutValue->_m[0][2] = matrix._m[0][2] * factor;

    pOutValue->_m[1][0] = matrix._m[1][0] * factor;
    pOutValue->_m[1][1] = matrix._m[1][1] * factor;
    pOutValue->_m[1][2] = matrix._m[1][2] * factor;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    MatrixColumnMajor3x2fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[1][0] * matrix2._m[0][1];
    result._m[0][1] = matrix1._m[0][1] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[0][1];
    result._m[0][2] = matrix1._m[0][2] * matrix2._m[0][0] + matrix1._m[1][2] * matrix2._m[0][1] + matrix2._m[0][2];

    result._m[1][0] = matrix1._m[0][0] * matrix2._m[1][0] + matrix1._m[1][0] * matrix2._m[1][1];
    result._m[1][1] = matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[1][1] * matrix2._m[1][1];
    result._m[1][2] = matrix1._m[0][2] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[1][1] + matrix2._m[1][2];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor3x2fType* pOutValue, float t, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = t * matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = t * matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = t * matrix1._m[0][2] + matrix2._m[0][2];

    pOutValue->_m[1][0] = t * matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = t * matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = t * matrix1._m[1][2] + matrix2._m[1][2];
}

NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] / factor;
    pOutValue->_m[0][1] = matrix._m[0][1] / factor;
    pOutValue->_m[0][2] = matrix._m[0][2] / factor;

    pOutValue->_m[1][0] = matrix._m[1][0] / factor;
    pOutValue->_m[1][1] = matrix._m[1][1] / factor;
    pOutValue->_m[1][2] = matrix._m[1][2] / factor;
}

NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    float determinant =
        matrix._m[0][0] * matrix._m[1][1]
        - matrix._m[0][1] * matrix._m[1][0];

    if (determinant == 0.f)
    {
        MatrixZero(pOutValue);
        return false;
    }

    float reciprocalDeterminant = 1.f / determinant;

    MatrixColumnMajor3x2fType result;

    result._m[0][0] = matrix._m[1][1] * reciprocalDeterminant;
    result._m[0][1] = -matrix._m[0][1] * reciprocalDeterminant;

    result._m[1][0] = -matrix._m[1][0] * reciprocalDeterminant;
    result._m[1][1] = matrix._m[0][0] * reciprocalDeterminant;

    result._m[0][2] = -result._m[0][0] * matrix._m[0][2] - result._m[0][1] * matrix._m[1][2];
    result._m[1][2] = -result._m[1][0] * matrix._m[0][2] - result._m[1][1] * matrix._m[1][2];

    *pOutValue = result;

    return true;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    MatrixColumnMajor3x2fType result;

    result._m[0][0] = matrix._m[0][0];
    result._m[0][1] = matrix._m[1][0];
    result._m[0][2] = 0.f;

    result._m[1][0] = matrix._m[0][1];
    result._m[1][1] = matrix._m[1][1];
    result._m[1][2] = 0.f;

    *pOutValue = result;
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    MatrixColumnMajor3x2fType result;

    if (!MatrixInverse(&result, matrix))
    {
        MatrixZero(pOutValue);
        return false;
    }

    MatrixTranspose(pOutValue, result);

    return true;
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT
{
    pOutValue->_m[0][2] = translate._v[0];
    pOutValue->_m[1][2] = translate._v[1];
}

NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = scale._v[0];
    pOutValue->_m[0][1] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = scale._v[1];
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT
{
    float sin, cos;
    nn::util::SinCosEst(&sin, &cos, rotationRadian);

    pOutValue->_m[0][0] = cos;
    pOutValue->_m[0][1] = -sin;

    pOutValue->_m[1][0] = sin;
    pOutValue->_m[1][1] = cos;
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT
{
    float sin, cos;
    nn::util::SinCosEst(&sin, &cos, rotationRadian);

    pOutValue->_m[0][0] = scale._v[0] * cos;
    pOutValue->_m[0][1] = scale._v[1] * -sin;

    pOutValue->_m[1][0] = scale._v[0] * sin;
    pOutValue->_m[1][1] = scale._v[1] * cos;
}

// T4x3 行列
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = pOutValue->_m[0][1] = pOutValue->_m[0][2] = pOutValue->_m[0][3] =
    pOutValue->_m[1][0] = pOutValue->_m[1][1] = pOutValue->_m[1][2] = pOutValue->_m[1][3] =
    pOutValue->_m[2][0] = pOutValue->_m[2][1] = pOutValue->_m[2][2] = pOutValue->_m[2][3] = 0.f;
}

NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = 1.f;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 1.f;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = 1.f;
    pOutValue->_m[2][3] = 0.f;
}

NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] + matrix2._m[0][2];
    pOutValue->_m[0][3] = matrix1._m[0][3] + matrix2._m[0][3];

    pOutValue->_m[1][0] = matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] + matrix2._m[1][2];
    pOutValue->_m[1][3] = matrix1._m[1][3] + matrix2._m[1][3];

    pOutValue->_m[2][0] = matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] + matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] + matrix2._m[2][2];
    pOutValue->_m[2][3] = matrix1._m[2][3] + matrix2._m[2][3];
}

NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] - matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] - matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] - matrix2._m[0][2];
    pOutValue->_m[0][3] = matrix1._m[0][3] - matrix2._m[0][3];

    pOutValue->_m[1][0] = matrix1._m[1][0] - matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] - matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] - matrix2._m[1][2];
    pOutValue->_m[1][3] = matrix1._m[1][3] - matrix2._m[1][3];

    pOutValue->_m[2][0] = matrix1._m[2][0] - matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] - matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] - matrix2._m[2][2];
    pOutValue->_m[2][3] = matrix1._m[2][3] - matrix2._m[2][3];
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] * factor;
    pOutValue->_m[0][1] = matrix._m[0][1] * factor;
    pOutValue->_m[0][2] = matrix._m[0][2] * factor;
    pOutValue->_m[0][3] = matrix._m[0][3] * factor;

    pOutValue->_m[1][0] = matrix._m[1][0] * factor;
    pOutValue->_m[1][1] = matrix._m[1][1] * factor;
    pOutValue->_m[1][2] = matrix._m[1][2] * factor;
    pOutValue->_m[1][3] = matrix._m[1][3] * factor;

    pOutValue->_m[2][0] = matrix._m[2][0] * factor;
    pOutValue->_m[2][1] = matrix._m[2][1] * factor;
    pOutValue->_m[2][2] = matrix._m[2][2] * factor;
    pOutValue->_m[2][3] = matrix._m[2][3] * factor;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    MatrixColumnMajor4x3fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[2][0] * matrix2._m[0][2];
    result._m[0][1] = matrix1._m[0][1] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[0][2];
    result._m[0][2] = matrix1._m[0][2] * matrix2._m[0][0] + matrix1._m[1][2] * matrix2._m[0][1] + matrix1._m[2][2] * matrix2._m[0][2];
    result._m[0][3] = matrix1._m[0][3] * matrix2._m[0][0] + matrix1._m[1][3] * matrix2._m[0][1] + matrix1._m[2][3] * matrix2._m[0][2] + matrix2._m[0][3];

    result._m[1][0] = matrix1._m[0][0] * matrix2._m[1][0] + matrix1._m[1][0] * matrix2._m[1][1] + matrix1._m[2][0] * matrix2._m[1][2];
    result._m[1][1] = matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[2][1] * matrix2._m[1][2];
    result._m[1][2] = matrix1._m[0][2] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[1][2];
    result._m[1][3] = matrix1._m[0][3] * matrix2._m[1][0] + matrix1._m[1][3] * matrix2._m[1][1] + matrix1._m[2][3] * matrix2._m[1][2] + matrix2._m[1][3];

    result._m[2][0] = matrix1._m[0][0] * matrix2._m[2][0] + matrix1._m[1][0] * matrix2._m[2][1] + matrix1._m[2][0] * matrix2._m[2][2];
    result._m[2][1] = matrix1._m[0][1] * matrix2._m[2][0] + matrix1._m[1][1] * matrix2._m[2][1] + matrix1._m[2][1] * matrix2._m[2][2];
    result._m[2][2] = matrix1._m[0][2] * matrix2._m[2][0] + matrix1._m[1][2] * matrix2._m[2][1] + matrix1._m[2][2] * matrix2._m[2][2];
    result._m[2][3] = matrix1._m[0][3] * matrix2._m[2][0] + matrix1._m[1][3] * matrix2._m[2][1] + matrix1._m[2][3] * matrix2._m[2][2] + matrix2._m[2][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    MatrixColumnMajor4x4fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[2][0] * matrix2._m[0][2];
    result._m[0][1] = matrix1._m[0][1] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[0][2];
    result._m[0][2] = matrix1._m[0][2] * matrix2._m[0][0] + matrix1._m[1][2] * matrix2._m[0][1] + matrix1._m[2][2] * matrix2._m[0][2];
    result._m[0][3] = matrix1._m[0][3] * matrix2._m[0][0] + matrix1._m[1][3] * matrix2._m[0][1] + matrix1._m[2][3] * matrix2._m[0][2] + matrix2._m[0][3];

    result._m[1][0] = matrix1._m[0][0] * matrix2._m[1][0] + matrix1._m[1][0] * matrix2._m[1][1] + matrix1._m[2][0] * matrix2._m[1][2];
    result._m[1][1] = matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[2][1] * matrix2._m[1][2];
    result._m[1][2] = matrix1._m[0][2] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[1][2];
    result._m[1][3] = matrix1._m[0][3] * matrix2._m[1][0] + matrix1._m[1][3] * matrix2._m[1][1] + matrix1._m[2][3] * matrix2._m[1][2] + matrix2._m[1][3];

    result._m[2][0] = matrix1._m[0][0] * matrix2._m[2][0] + matrix1._m[1][0] * matrix2._m[2][1] + matrix1._m[2][0] * matrix2._m[2][2];
    result._m[2][1] = matrix1._m[0][1] * matrix2._m[2][0] + matrix1._m[1][1] * matrix2._m[2][1] + matrix1._m[2][1] * matrix2._m[2][2];
    result._m[2][2] = matrix1._m[0][2] * matrix2._m[2][0] + matrix1._m[1][2] * matrix2._m[2][1] + matrix1._m[2][2] * matrix2._m[2][2];
    result._m[2][3] = matrix1._m[0][3] * matrix2._m[2][0] + matrix1._m[1][3] * matrix2._m[2][1] + matrix1._m[2][3] * matrix2._m[2][2] + matrix2._m[2][3];

    result._m[3][0] = matrix1._m[0][0] * matrix2._m[3][0] + matrix1._m[1][0] * matrix2._m[3][1] + matrix1._m[2][0] * matrix2._m[3][2];
    result._m[3][1] = matrix1._m[0][1] * matrix2._m[3][0] + matrix1._m[1][1] * matrix2._m[3][1] + matrix1._m[2][1] * matrix2._m[3][2];
    result._m[3][2] = matrix1._m[0][2] * matrix2._m[3][0] + matrix1._m[1][2] * matrix2._m[3][1] + matrix1._m[2][2] * matrix2._m[3][2];
    result._m[3][3] = matrix1._m[0][3] * matrix2._m[3][0] + matrix1._m[1][3] * matrix2._m[3][1] + matrix1._m[2][3] * matrix2._m[3][2] + matrix2._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor4x3fType* pOutValue, float t, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = t * matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = t * matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = t * matrix1._m[0][2] + matrix2._m[0][2];
    pOutValue->_m[0][3] = t * matrix1._m[0][3] + matrix2._m[0][3];

    pOutValue->_m[1][0] = t * matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = t * matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = t * matrix1._m[1][2] + matrix2._m[1][2];
    pOutValue->_m[1][3] = t * matrix1._m[1][3] + matrix2._m[1][3];

    pOutValue->_m[2][0] = t * matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = t * matrix1._m[2][1] + matrix2._m[2][1];
    pOutValue->_m[2][2] = t * matrix1._m[2][2] + matrix2._m[2][2];
    pOutValue->_m[2][3] = t * matrix1._m[2][3] + matrix2._m[2][3];
}

NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] / factor;
    pOutValue->_m[0][1] = matrix._m[0][1] / factor;
    pOutValue->_m[0][2] = matrix._m[0][2] / factor;
    pOutValue->_m[0][3] = matrix._m[0][3] / factor;

    pOutValue->_m[1][0] = matrix._m[1][0] / factor;
    pOutValue->_m[1][1] = matrix._m[1][1] / factor;
    pOutValue->_m[1][2] = matrix._m[1][2] / factor;
    pOutValue->_m[1][3] = matrix._m[1][3] / factor;

    pOutValue->_m[2][0] = matrix._m[2][0] / factor;
    pOutValue->_m[2][1] = matrix._m[2][1] / factor;
    pOutValue->_m[2][2] = matrix._m[2][2] / factor;
    pOutValue->_m[2][3] = matrix._m[2][3] / factor;
}

NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    float determinant =
        matrix._m[0][0] * matrix._m[1][1] * matrix._m[2][2]
      + matrix._m[0][1] * matrix._m[1][2] * matrix._m[2][0]
      + matrix._m[0][2] * matrix._m[1][0] * matrix._m[2][1]
      - matrix._m[2][0] * matrix._m[1][1] * matrix._m[0][2]
      - matrix._m[1][0] * matrix._m[0][1] * matrix._m[2][2]
      - matrix._m[0][0] * matrix._m[2][1] * matrix._m[1][2];

    if(determinant == 0.f)
    {
        MatrixZero(pOutValue);
        return false;
    }

    float reciprocalDeterminant = 1.f / determinant;

    MatrixColumnMajor4x3fType result;

    result._m[0][0] = (matrix._m[1][1]*matrix._m[2][2] - matrix._m[2][1]*matrix._m[1][2]) * reciprocalDeterminant;
    result._m[0][1] = (matrix._m[2][1]*matrix._m[0][2] - matrix._m[0][1]*matrix._m[2][2]) * reciprocalDeterminant;
    result._m[0][2] = (matrix._m[0][1]*matrix._m[1][2] - matrix._m[1][1]*matrix._m[0][2]) * reciprocalDeterminant;

    result._m[1][0] = (matrix._m[2][0]*matrix._m[1][2] - matrix._m[1][0]*matrix._m[2][2]) * reciprocalDeterminant;
    result._m[1][1] = (matrix._m[0][0]*matrix._m[2][2] - matrix._m[2][0]*matrix._m[0][2]) * reciprocalDeterminant;
    result._m[1][2] = (matrix._m[1][0]*matrix._m[0][2] - matrix._m[0][0]*matrix._m[1][2]) * reciprocalDeterminant;

    result._m[2][0] = (matrix._m[1][0]*matrix._m[2][1] - matrix._m[2][0]*matrix._m[1][1]) * reciprocalDeterminant;
    result._m[2][1] = (matrix._m[2][0]*matrix._m[0][1] - matrix._m[0][0]*matrix._m[2][1]) * reciprocalDeterminant;
    result._m[2][2] = (matrix._m[0][0]*matrix._m[1][1] - matrix._m[1][0]*matrix._m[0][1]) * reciprocalDeterminant;

    result._m[0][3] = -result._m[0][0]*matrix._m[0][3] - result._m[0][1]*matrix._m[1][3] - result._m[0][2]*matrix._m[2][3];
    result._m[1][3] = -result._m[1][0]*matrix._m[0][3] - result._m[1][1]*matrix._m[1][3] - result._m[1][2]*matrix._m[2][3];
    result._m[2][3] = -result._m[2][0]*matrix._m[0][3] - result._m[2][1]*matrix._m[1][3] - result._m[2][2]*matrix._m[2][3];

    *pOutValue = result;

    return true;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    MatrixColumnMajor4x3fType result;

    result._m[0][0] = matrix._m[0][0];
    result._m[0][1] = matrix._m[1][0];
    result._m[0][2] = matrix._m[2][0];
    result._m[0][3] = 0.f;

    result._m[1][0] = matrix._m[0][1];
    result._m[1][1] = matrix._m[1][1];
    result._m[1][2] = matrix._m[2][1];
    result._m[1][3] = 0.f;

    result._m[2][0] = matrix._m[0][2];
    result._m[2][1] = matrix._m[1][2];
    result._m[2][2] = matrix._m[2][2];
    result._m[2][3] = 0.f;

    *pOutValue = result;
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    MatrixColumnMajor4x3fType result;

    if(!MatrixInverse(&result, matrix))
    {
        MatrixZero(pOutValue);
        return false;
    }

    MatrixTranspose(pOutValue, result);

    return true;
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT
{
    Vector3fType lookVector;
    VectorSubtract(&lookVector, cameraPosition, target);
    VectorNormalize(&lookVector, lookVector);

    Vector3fType rightVector;
    VectorCross(&rightVector, up, lookVector);
    VectorNormalize(&rightVector, rightVector);

    Vector3fType upVector;
    VectorCross(&upVector, lookVector, rightVector);

    pOutValue->_m[0][0] = rightVector._v[0];
    pOutValue->_m[0][1] = rightVector._v[1];
    pOutValue->_m[0][2] = rightVector._v[2];
    pOutValue->_m[0][3] = -VectorDot(cameraPosition, rightVector);

    pOutValue->_m[1][0] = upVector._v[0];
    pOutValue->_m[1][1] = upVector._v[1];
    pOutValue->_m[1][2] = upVector._v[2];
    pOutValue->_m[1][3] = -VectorDot(cameraPosition, upVector);

    pOutValue->_m[2][0] = lookVector._v[0];
    pOutValue->_m[2][1] = lookVector._v[1];
    pOutValue->_m[2][2] = lookVector._v[2];
    pOutValue->_m[2][3] = -VectorDot(cameraPosition, lookVector);
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT
{
    Vector3fType lookReverse;
    VectorSubtract(&lookReverse, cameraPosition, target);

    if(lookReverse._v[0] == 0.f && lookReverse._v[2] == 0.f)
    {
        pOutValue->_m[0][0] = 1.f;
        pOutValue->_m[0][1] = 0.f;
        pOutValue->_m[0][2] = 0.f;
        pOutValue->_m[0][3] = -cameraPosition._v[0];

        pOutValue->_m[1][0] = 0.f;
        pOutValue->_m[1][1] = 0.f;

        pOutValue->_m[2][0] = 0.f;
        pOutValue->_m[2][2] = 0.f;

        if(lookReverse._v[1] <= 0.f)
        {
            pOutValue->_m[1][2] = 1.f;
            pOutValue->_m[1][3] = -cameraPosition._v[2];

            pOutValue->_m[2][1] = -1.f;
            pOutValue->_m[2][3] = cameraPosition._v[1];
        }
        else
        {
            pOutValue->_m[1][2] = -1.f;
            pOutValue->_m[1][3] = cameraPosition._v[2];

            pOutValue->_m[2][1] = 1.f;
            pOutValue->_m[2][3] = -cameraPosition._v[1];
        }
    }
    else
    {
        Vector3fType r;
        VectorSet(&r, lookReverse._v[2], 0.f, -lookReverse._v[0]);

        VectorNormalize(&lookReverse, lookReverse);
        VectorNormalize(&r, r);

        Vector3fType u;
        VectorCross(&u, lookReverse, r);

        float twistSin, twistCos;
        nn::util::SinCosEst(&twistSin, &twistCos, twistRadian);

        Vector3fType right;
        right._v[0] = twistSin * u._v[0] + twistCos * r._v[0];
        right._v[1] = twistSin * u._v[1];
        right._v[2] = twistSin * u._v[2] + twistCos * r._v[2];

        Vector3fType up;
        up._v[0] = twistCos * u._v[0] - twistSin * r._v[0];
        up._v[1] = twistCos * u._v[1];
        up._v[2] = twistCos * u._v[2] - twistSin * r._v[2];

        pOutValue->_m[0][0] = right._v[0];
        pOutValue->_m[0][1] = right._v[1];
        pOutValue->_m[0][2] = right._v[2];
        pOutValue->_m[0][3] = -VectorDot(cameraPosition, right);

        pOutValue->_m[1][0] = up._v[0];
        pOutValue->_m[1][1] = up._v[1];
        pOutValue->_m[1][2] = up._v[2];
        pOutValue->_m[1][3] = -VectorDot(cameraPosition, up);

        pOutValue->_m[2][0] = lookReverse._v[0];
        pOutValue->_m[2][1] = lookReverse._v[1];
        pOutValue->_m[2][2] = lookReverse._v[2];
        pOutValue->_m[2][3] = -VectorDot(cameraPosition, lookReverse);
    }
}

NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    float sinX, cosX, sinY, cosY, sinZ, cosZ;
    nn::util::SinCosEst(&sinX, &cosX, rotationRadian._v[0]);
    nn::util::SinCosEst(&sinY, &cosY, rotationRadian._v[1]);
    nn::util::SinCosEst(&sinZ, &cosZ, rotationRadian._v[2]);

    Vector3fType right, up, back;

    right._v[0] = sinX * sinY * sinZ + cosY * cosZ;
    right._v[1] = cosX * sinZ;
    right._v[2] = sinX * cosY * sinZ - sinY * cosZ;

    up._v[0] = sinX * sinY * cosZ - cosY * sinZ;
    up._v[1] = cosX * cosZ;
    up._v[2] = sinX * cosY * cosZ + sinY * sinZ;

    back._v[0] = cosX * sinY;
    back._v[1] = -sinX;
    back._v[2] = cosX * cosY;

    pOutValue->_m[0][0] = right._v[0];
    pOutValue->_m[0][1] = right._v[1];
    pOutValue->_m[0][2] = right._v[2];
    pOutValue->_m[0][3] = -VectorDot(cameraPosition, right);

    pOutValue->_m[1][0] = up._v[0];
    pOutValue->_m[1][1] = up._v[1];
    pOutValue->_m[1][2] = up._v[2];
    pOutValue->_m[1][3] = -VectorDot(cameraPosition, up);

    pOutValue->_m[2][0] = back._v[0];
    pOutValue->_m[2][1] = back._v[1];
    pOutValue->_m[2][2] = back._v[2];
    pOutValue->_m[2][3] = -VectorDot(cameraPosition, back);
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT
{
    pOutValue->_m[0][3] = translate._v[0];
    pOutValue->_m[1][3] = translate._v[1];
    pOutValue->_m[2][3] = translate._v[2];
}

NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = scale._v[0];
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = scale._v[1];
    pOutValue->_m[1][2] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = scale._v[2];
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    MatrixColumnMajor4x3fType rotationMatrix;

    MatrixFromQuaternion(&rotationMatrix, rotationQuaternion);

    pOutValue->_m[0][0] = rotationMatrix._m[0][0];
    pOutValue->_m[0][1] = rotationMatrix._m[0][1];
    pOutValue->_m[0][2] = rotationMatrix._m[0][2];

    pOutValue->_m[1][0] = rotationMatrix._m[1][0];
    pOutValue->_m[1][1] = rotationMatrix._m[1][1];
    pOutValue->_m[1][2] = rotationMatrix._m[1][2];

    pOutValue->_m[2][0] = rotationMatrix._m[2][0];
    pOutValue->_m[2][1] = rotationMatrix._m[2][1];
    pOutValue->_m[2][2] = rotationMatrix._m[2][2];
}

NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    float sinX, cosX, sinY, cosY, sinZ, cosZ;
    nn::util::SinCosEst(&sinX, &cosX, rotationRadian._v[0]);
    nn::util::SinCosEst(&sinY, &cosY, rotationRadian._v[1]);
    nn::util::SinCosEst(&sinZ, &cosZ, rotationRadian._v[2]);

    float opt1 = cosX * cosZ;
    float opt2 = sinX * sinY;
    float opt3 = cosX * sinZ;

    pOutValue->_m[0][0] = cosY * cosZ;
    pOutValue->_m[0][1] = (opt2 * cosZ) - opt3;
    pOutValue->_m[0][2] = (opt1 * sinY) + (sinX * sinZ);

    pOutValue->_m[1][0] = cosY * sinZ;
    pOutValue->_m[1][1] = (opt2 * sinZ) + opt1;
    pOutValue->_m[1][2] = (opt3 * sinY) - (sinX * cosZ);

    pOutValue->_m[2][0] = -sinY;
    pOutValue->_m[2][1] = sinX * cosY;
    pOutValue->_m[2][2] = cosX * cosY;
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    MatrixColumnMajor4x3fType rotationMatrix;

    MatrixFromQuaternion(&rotationMatrix, rotationQuaternion);

    pOutValue->_m[0][0] = scale._v[0] * rotationMatrix._m[0][0];
    pOutValue->_m[0][1] = scale._v[1] * rotationMatrix._m[0][1];
    pOutValue->_m[0][2] = scale._v[2] * rotationMatrix._m[0][2];

    pOutValue->_m[1][0] = scale._v[0] * rotationMatrix._m[1][0];
    pOutValue->_m[1][1] = scale._v[1] * rotationMatrix._m[1][1];
    pOutValue->_m[1][2] = scale._v[2] * rotationMatrix._m[1][2];

    pOutValue->_m[2][0] = scale._v[0] * rotationMatrix._m[2][0];
    pOutValue->_m[2][1] = scale._v[1] * rotationMatrix._m[2][1];
    pOutValue->_m[2][2] = scale._v[2] * rotationMatrix._m[2][2];
}

NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    float sinX, cosX, sinY, cosY, sinZ, cosZ;
    nn::util::SinCosEst(&sinX, &cosX, rotationRadian._v[0]);
    nn::util::SinCosEst(&sinY, &cosY, rotationRadian._v[1]);
    nn::util::SinCosEst(&sinZ, &cosZ, rotationRadian._v[2]);

    float opt1 = cosX * cosZ;
    float opt2 = sinX * sinY;
    float opt3 = cosX * sinZ;

    pOutValue->_m[0][0] = scale._v[0] * (cosY * cosZ);
    pOutValue->_m[0][1] = scale._v[1] * ((opt2 * cosZ) - opt3);
    pOutValue->_m[0][2] = scale._v[2] * ((opt1 * sinY) + (sinX * sinZ));

    pOutValue->_m[1][0] = scale._v[0] * (cosY * sinZ);
    pOutValue->_m[1][1] = scale._v[1] * ((opt2 * sinZ) + opt1);
    pOutValue->_m[1][2] = scale._v[2] * ((opt3 * sinY) - (sinX * cosZ));

    pOutValue->_m[2][0] = scale._v[0] * (-sinY);
    pOutValue->_m[2][1] = scale._v[1] * (sinX * cosY);
    pOutValue->_m[2][2] = scale._v[2] * (cosX * cosY);
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    pOutValue->_m[0][0] = (2.f * nearZ) * reciprocalWidth * scaleS;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = (right + left) * reciprocalWidth * scaleS - translateS;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = (2.f * nearZ) * reciprocalHeight * scaleT;
    pOutValue->_m[1][2] = (top + bottom) * reciprocalHeight * scaleT - translateT;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = -1.f;
    pOutValue->_m[2][3] = 0.f;
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float angle = fovyRadian * 0.5f;
    float cotangent = 1.f / nn::util::TanEst(angle);

    pOutValue->_m[0][0] = cotangent / aspect * scaleS;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = -translateS;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = cotangent * scaleT;
    pOutValue->_m[1][2] = -translateT;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = -1.f;
    pOutValue->_m[2][3] = 0.f;
}

NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    pOutValue->_m[0][0] = 2.f * reciprocalWidth * scaleS;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = -(right + left) * reciprocalWidth * scaleS + translateS;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 2.f * reciprocalHeight * scaleT;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = -(top + bottom) * reciprocalHeight * scaleT + translateT;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = 0.f;
    pOutValue->_m[2][3] = 1.f;
}

// T4x4 行列
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = pOutValue->_m[0][1] = pOutValue->_m[0][2] = pOutValue->_m[0][3] =
    pOutValue->_m[1][0] = pOutValue->_m[1][1] = pOutValue->_m[1][2] = pOutValue->_m[1][3] =
    pOutValue->_m[2][0] = pOutValue->_m[2][1] = pOutValue->_m[2][2] = pOutValue->_m[2][3] =
    pOutValue->_m[3][0] = pOutValue->_m[3][1] = pOutValue->_m[3][2] = pOutValue->_m[3][3] = 0.f;
}

NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = 1.f;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 1.f;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = 1.f;
    pOutValue->_m[2][3] = 0.f;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
    pOutValue->_m[3][3] = 1.f;
}

NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] + matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] + matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] + matrix2._m[0][2];
    pOutValue->_m[0][3] = matrix1._m[0][3] + matrix2._m[0][3];

    pOutValue->_m[1][0] = matrix1._m[1][0] + matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] + matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] + matrix2._m[1][2];
    pOutValue->_m[1][3] = matrix1._m[1][3] + matrix2._m[1][3];

    pOutValue->_m[2][0] = matrix1._m[2][0] + matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] + matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] + matrix2._m[2][2];
    pOutValue->_m[2][3] = matrix1._m[2][3] + matrix2._m[2][3];

    pOutValue->_m[3][0] = matrix1._m[3][0] + matrix2._m[3][0];
    pOutValue->_m[3][1] = matrix1._m[3][1] + matrix2._m[3][1];
    pOutValue->_m[3][2] = matrix1._m[3][2] + matrix2._m[3][2];
    pOutValue->_m[3][3] = matrix1._m[3][3] + matrix2._m[3][3];
}

NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix1._m[0][0] - matrix2._m[0][0];
    pOutValue->_m[0][1] = matrix1._m[0][1] - matrix2._m[0][1];
    pOutValue->_m[0][2] = matrix1._m[0][2] - matrix2._m[0][2];
    pOutValue->_m[0][3] = matrix1._m[0][3] - matrix2._m[0][3];

    pOutValue->_m[1][0] = matrix1._m[1][0] - matrix2._m[1][0];
    pOutValue->_m[1][1] = matrix1._m[1][1] - matrix2._m[1][1];
    pOutValue->_m[1][2] = matrix1._m[1][2] - matrix2._m[1][2];
    pOutValue->_m[1][3] = matrix1._m[1][3] - matrix2._m[1][3];

    pOutValue->_m[2][0] = matrix1._m[2][0] - matrix2._m[2][0];
    pOutValue->_m[2][1] = matrix1._m[2][1] - matrix2._m[2][1];
    pOutValue->_m[2][2] = matrix1._m[2][2] - matrix2._m[2][2];
    pOutValue->_m[2][3] = matrix1._m[2][3] - matrix2._m[2][3];

    pOutValue->_m[3][0] = matrix1._m[3][0] - matrix2._m[3][0];
    pOutValue->_m[3][1] = matrix1._m[3][1] - matrix2._m[3][1];
    pOutValue->_m[3][2] = matrix1._m[3][2] - matrix2._m[3][2];
    pOutValue->_m[3][3] = matrix1._m[3][3] - matrix2._m[3][3];
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] * factor;
    pOutValue->_m[0][1] = matrix._m[0][1] * factor;
    pOutValue->_m[0][2] = matrix._m[0][2] * factor;
    pOutValue->_m[0][3] = matrix._m[0][3] * factor;

    pOutValue->_m[1][0] = matrix._m[1][0] * factor;
    pOutValue->_m[1][1] = matrix._m[1][1] * factor;
    pOutValue->_m[1][2] = matrix._m[1][2] * factor;
    pOutValue->_m[1][3] = matrix._m[1][3] * factor;

    pOutValue->_m[2][0] = matrix._m[2][0] * factor;
    pOutValue->_m[2][1] = matrix._m[2][1] * factor;
    pOutValue->_m[2][2] = matrix._m[2][2] * factor;
    pOutValue->_m[2][3] = matrix._m[2][3] * factor;

    pOutValue->_m[3][0] = matrix._m[3][0] * factor;
    pOutValue->_m[3][1] = matrix._m[3][1] * factor;
    pOutValue->_m[3][2] = matrix._m[3][2] * factor;
    pOutValue->_m[3][3] = matrix._m[3][3] * factor;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    MatrixColumnMajor4x4fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[2][0] * matrix2._m[0][2] + matrix1._m[3][0] * matrix2._m[0][3];
    result._m[0][1] = matrix1._m[0][1] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[0][2] + matrix1._m[3][1] * matrix2._m[0][3];
    result._m[0][2] = matrix1._m[0][2] * matrix2._m[0][0] + matrix1._m[1][2] * matrix2._m[0][1] + matrix1._m[2][2] * matrix2._m[0][2] + matrix1._m[3][2] * matrix2._m[0][3];
    result._m[0][3] = matrix1._m[0][3] * matrix2._m[0][0] + matrix1._m[1][3] * matrix2._m[0][1] + matrix1._m[2][3] * matrix2._m[0][2] + matrix1._m[3][3] * matrix2._m[0][3];

    result._m[1][0] = matrix1._m[0][0] * matrix2._m[1][0] + matrix1._m[1][0] * matrix2._m[1][1] + matrix1._m[2][0] * matrix2._m[1][2] + matrix1._m[3][0] * matrix2._m[1][3];
    result._m[1][1] = matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[2][1] * matrix2._m[1][2] + matrix1._m[3][1] * matrix2._m[1][3];
    result._m[1][2] = matrix1._m[0][2] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[1][2] + matrix1._m[3][2] * matrix2._m[1][3];
    result._m[1][3] = matrix1._m[0][3] * matrix2._m[1][0] + matrix1._m[1][3] * matrix2._m[1][1] + matrix1._m[2][3] * matrix2._m[1][2] + matrix1._m[3][3] * matrix2._m[1][3];

    result._m[2][0] = matrix1._m[0][0] * matrix2._m[2][0] + matrix1._m[1][0] * matrix2._m[2][1] + matrix1._m[2][0] * matrix2._m[2][2] + matrix1._m[3][0] * matrix2._m[2][3];
    result._m[2][1] = matrix1._m[0][1] * matrix2._m[2][0] + matrix1._m[1][1] * matrix2._m[2][1] + matrix1._m[2][1] * matrix2._m[2][2] + matrix1._m[3][1] * matrix2._m[2][3];
    result._m[2][2] = matrix1._m[0][2] * matrix2._m[2][0] + matrix1._m[1][2] * matrix2._m[2][1] + matrix1._m[2][2] * matrix2._m[2][2] + matrix1._m[3][2] * matrix2._m[2][3];
    result._m[2][3] = matrix1._m[0][3] * matrix2._m[2][0] + matrix1._m[1][3] * matrix2._m[2][1] + matrix1._m[2][3] * matrix2._m[2][2] + matrix1._m[3][3] * matrix2._m[2][3];

    result._m[3][0] = matrix1._m[3][0];
    result._m[3][1] = matrix1._m[3][1];
    result._m[3][2] = matrix1._m[3][2];
    result._m[3][3] = matrix1._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    MatrixColumnMajor4x4fType result;

    result._m[0][0] = matrix1._m[0][0] * matrix2._m[0][0] + matrix1._m[1][0] * matrix2._m[0][1] + matrix1._m[2][0] * matrix2._m[0][2] + matrix1._m[3][0] * matrix2._m[0][3];
    result._m[0][1] = matrix1._m[0][1] * matrix2._m[0][0] + matrix1._m[1][1] * matrix2._m[0][1] + matrix1._m[2][1] * matrix2._m[0][2] + matrix1._m[3][1] * matrix2._m[0][3];
    result._m[0][2] = matrix1._m[0][2] * matrix2._m[0][0] + matrix1._m[1][2] * matrix2._m[0][1] + matrix1._m[2][2] * matrix2._m[0][2] + matrix1._m[3][2] * matrix2._m[0][3];
    result._m[0][3] = matrix1._m[0][3] * matrix2._m[0][0] + matrix1._m[1][3] * matrix2._m[0][1] + matrix1._m[2][3] * matrix2._m[0][2] + matrix1._m[3][3] * matrix2._m[0][3];

    result._m[1][0] = matrix1._m[0][0] * matrix2._m[1][0] + matrix1._m[1][0] * matrix2._m[1][1] + matrix1._m[2][0] * matrix2._m[1][2] + matrix1._m[3][0] * matrix2._m[1][3];
    result._m[1][1] = matrix1._m[0][1] * matrix2._m[1][0] + matrix1._m[1][1] * matrix2._m[1][1] + matrix1._m[2][1] * matrix2._m[1][2] + matrix1._m[3][1] * matrix2._m[1][3];
    result._m[1][2] = matrix1._m[0][2] * matrix2._m[1][0] + matrix1._m[1][2] * matrix2._m[1][1] + matrix1._m[2][2] * matrix2._m[1][2] + matrix1._m[3][2] * matrix2._m[1][3];
    result._m[1][3] = matrix1._m[0][3] * matrix2._m[1][0] + matrix1._m[1][3] * matrix2._m[1][1] + matrix1._m[2][3] * matrix2._m[1][2] + matrix1._m[3][3] * matrix2._m[1][3];

    result._m[2][0] = matrix1._m[0][0] * matrix2._m[2][0] + matrix1._m[1][0] * matrix2._m[2][1] + matrix1._m[2][0] * matrix2._m[2][2] + matrix1._m[3][0] * matrix2._m[2][3];
    result._m[2][1] = matrix1._m[0][1] * matrix2._m[2][0] + matrix1._m[1][1] * matrix2._m[2][1] + matrix1._m[2][1] * matrix2._m[2][2] + matrix1._m[3][1] * matrix2._m[2][3];
    result._m[2][2] = matrix1._m[0][2] * matrix2._m[2][0] + matrix1._m[1][2] * matrix2._m[2][1] + matrix1._m[2][2] * matrix2._m[2][2] + matrix1._m[3][2] * matrix2._m[2][3];
    result._m[2][3] = matrix1._m[0][3] * matrix2._m[2][0] + matrix1._m[1][3] * matrix2._m[2][1] + matrix1._m[2][3] * matrix2._m[2][2] + matrix1._m[3][3] * matrix2._m[2][3];

    result._m[3][0] = matrix1._m[0][0] * matrix2._m[3][0] + matrix1._m[1][0] * matrix2._m[3][1] + matrix1._m[2][0] * matrix2._m[3][2] + matrix1._m[3][0] * matrix2._m[3][3];
    result._m[3][1] = matrix1._m[0][1] * matrix2._m[3][0] + matrix1._m[1][1] * matrix2._m[3][1] + matrix1._m[2][1] * matrix2._m[3][2] + matrix1._m[3][1] * matrix2._m[3][3];
    result._m[3][2] = matrix1._m[0][2] * matrix2._m[3][0] + matrix1._m[1][2] * matrix2._m[3][1] + matrix1._m[2][2] * matrix2._m[3][2] + matrix1._m[3][2] * matrix2._m[3][3];
    result._m[3][3] = matrix1._m[0][3] * matrix2._m[3][0] + matrix1._m[1][3] * matrix2._m[3][1] + matrix1._m[2][3] * matrix2._m[3][2] + matrix1._m[3][3] * matrix2._m[3][3];

    *pOutValue = result;
}


NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m[0][0] = matrix._m[0][0] / factor;
    pOutValue->_m[0][1] = matrix._m[0][1] / factor;
    pOutValue->_m[0][2] = matrix._m[0][2] / factor;
    pOutValue->_m[0][3] = matrix._m[0][3] / factor;

    pOutValue->_m[1][0] = matrix._m[1][0] / factor;
    pOutValue->_m[1][1] = matrix._m[1][1] / factor;
    pOutValue->_m[1][2] = matrix._m[1][2] / factor;
    pOutValue->_m[1][3] = matrix._m[1][3] / factor;

    pOutValue->_m[2][0] = matrix._m[2][0] / factor;
    pOutValue->_m[2][1] = matrix._m[2][1] / factor;
    pOutValue->_m[2][2] = matrix._m[2][2] / factor;
    pOutValue->_m[2][3] = matrix._m[2][3] / factor;

    pOutValue->_m[3][0] = matrix._m[3][0] / factor;
    pOutValue->_m[3][1] = matrix._m[3][1] / factor;
    pOutValue->_m[3][2] = matrix._m[3][2] / factor;
    pOutValue->_m[3][3] = matrix._m[3][3] / factor;
}

NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    MatrixColumnMajor4x4fType sourceMatrix(matrix);

    MatrixIdentity(pOutValue);

    float (*source)[4] = sourceMatrix._m;
    float (*result)[4] = pOutValue->_m;

    float w;

    for (int i = 0; i < 4; ++i)
    {
        float max = 0.f;
        int swap = i;

        for(int k = i ; k < 4 ; k++)
        {
            float ftmp = ::std::abs(source[i][k]);

            if (ftmp > max)
            {
                max = ftmp;
                swap = k;
            }
        }

        if (max == 0.f)
        {
            MatrixZero(pOutValue);
            return false;
        }

        if (swap != i)
        {
            for (int k = 0; k < 4; k++)
            {
                ::std::swap(source[k][i], source[k][swap]);
                ::std::swap(result[k][i], result[k][swap]);
            }
        }

        w = 1.0f / source[i][i];

        for (int j = 0; j < 4; ++j)
        {
            source[j][i] *= w;
            result[j][i] *= w;
        }

        for (int k = 0; k < 4; ++k )
        {
            if (k == i)
            {
                continue;
            }

            w = source[i][k];

            for (int j = 0; j < 4; ++j)
            {
                source[j][k] -= source[j][i] * w;
                result[j][k] -= result[j][i] * w;
            }
        }
    }

    return true;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    MatrixColumnMajor4x4fType result;

    result._m[0][0] = matrix._m[0][0];
    result._m[0][1] = matrix._m[1][0];
    result._m[0][2] = matrix._m[2][0];
    result._m[0][3] = matrix._m[3][0];

    result._m[1][0] = matrix._m[0][1];
    result._m[1][1] = matrix._m[1][1];
    result._m[1][2] = matrix._m[2][1];
    result._m[1][3] = matrix._m[3][1];

    result._m[2][0] = matrix._m[0][2];
    result._m[2][1] = matrix._m[1][2];
    result._m[2][2] = matrix._m[2][2];
    result._m[2][3] = matrix._m[3][2];

    result._m[3][0] = matrix._m[0][3];
    result._m[3][1] = matrix._m[1][3];
    result._m[3][2] = matrix._m[2][3];
    result._m[3][3] = matrix._m[3][3];

    *pOutValue = result;
}

NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    pOutValue->_m[0][0] = (2 * nearZ) * reciprocalWidth;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = (right + left) * reciprocalWidth;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = (2 * nearZ) * reciprocalHeight;
    pOutValue->_m[1][2] = (top + bottom) * reciprocalHeight;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = farZ * reciprocalDepth;
    pOutValue->_m[2][3] = (farZ * nearZ) * reciprocalDepth;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = -1.f;
    pOutValue->_m[3][3] = 0.f;
}

NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT
{
    float angle = fovyRadian * 0.5f;
    float cotangent = 1.f / nn::util::TanEst(angle);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    pOutValue->_m[0][0] = cotangent / aspect;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = cotangent;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = farZ * reciprocalDepth;
    pOutValue->_m[2][3] = (farZ * nearZ) * reciprocalDepth;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = -1.f;
    pOutValue->_m[3][3] = 0.f;
}

NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    pOutValue->_m[0][0] = 2.f * reciprocalWidth;
    pOutValue->_m[0][1] = 0.f;
    pOutValue->_m[0][2] = 0.f;
    pOutValue->_m[0][3] = -(right + left) * reciprocalWidth;

    pOutValue->_m[1][0] = 0.f;
    pOutValue->_m[1][1] = 2.f * reciprocalHeight;
    pOutValue->_m[1][2] = 0.f;
    pOutValue->_m[1][3] = -(top + bottom) * reciprocalHeight;

    pOutValue->_m[2][0] = 0.f;
    pOutValue->_m[2][1] = 0.f;
    pOutValue->_m[2][2] = reciprocalDepth;
    pOutValue->_m[2][3] = nearZ * reciprocalDepth;

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
    pOutValue->_m[3][3] = 1.f;
}

//
// ベクトル・行列ユーティリティ
//
//
// 4x3 行列
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float s = 2.f;

    float xs = quaternion._v[0] * s;
    float ys = quaternion._v[1] * s;
    float zs = quaternion._v[2] * s;

    float wx = quaternion._v[3] * xs;
    float wy = quaternion._v[3] * ys;
    float wz = quaternion._v[3] * zs;

    float xx = quaternion._v[0] * xs;
    float xy = quaternion._v[0] * ys;
    float xz = quaternion._v[0] * zs;

    float yy = quaternion._v[1] * ys;
    float yz = quaternion._v[1] * zs;
    float zz = quaternion._v[2] * zs;

    pOutValue->_m[0][0] = 1.f - (yy + zz);
    pOutValue->_m[0][1] = xy + wz;
    pOutValue->_m[0][2] = xz - wy;

    pOutValue->_m[1][0] = xy - wz;
    pOutValue->_m[1][1] = 1.f - (xx + zz);
    pOutValue->_m[1][2] = yz + wx;

    pOutValue->_m[2][0] = xz + wy;
    pOutValue->_m[2][1] = yz - wx;
    pOutValue->_m[2][2] = 1.f - (xx + yy);

    pOutValue->_m[3][0] = 0.f;
    pOutValue->_m[3][1] = 0.f;
    pOutValue->_m[3][2] = 0.f;
}

// T4x3 行列
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    float s = 2.f;

    float xs = quaternion._v[0] * s;
    float ys = quaternion._v[1] * s;
    float zs = quaternion._v[2] * s;

    float wx = quaternion._v[3] * xs;
    float wy = quaternion._v[3] * ys;
    float wz = quaternion._v[3] * zs;

    float xx = quaternion._v[0] * xs;
    float xy = quaternion._v[0] * ys;
    float xz = quaternion._v[0] * zs;

    float yy = quaternion._v[1] * ys;
    float yz = quaternion._v[1] * zs;
    float zz = quaternion._v[2] * zs;

    pOutValue->_m[0][0] = 1.f - (yy + zz);
    pOutValue->_m[0][1] = xy - wz;
    pOutValue->_m[0][2] = xz + wy;
    pOutValue->_m[0][3] = 0.f;

    pOutValue->_m[1][0] = xy + wz;
    pOutValue->_m[1][1] = 1.f - (xx + zz);
    pOutValue->_m[1][2] = yz - wx;
    pOutValue->_m[1][3] = 0.f;

    pOutValue->_m[2][0] = xz - wy;
    pOutValue->_m[2][1] = yz + wx;
    pOutValue->_m[2][2] = 1.f - (xx + yy);
    pOutValue->_m[2][3] = 0.f;
}

}}} // namespace nn::util::general
