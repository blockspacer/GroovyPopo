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
#include <nn/util/detail/util_MatrixApiDetail.neon.h>
#include <cmath>
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

#define NN_UTIL_NEON_MATRIX_ROW_MAJOR_3X2F_INITIALIZER(m00, m01, m10, m11, m20, m21) \
    { \
        { \
            { \
                { m00, m01 }, \
                { m10, m11 }, \
                { m20, m21 }  \
            } \
        } \
    }

#define NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER(m00, m01, m10, m11, m20, m21) \
    { \
       { \
            { \
                { m00, m10, m20, 0 }, \
                { m01, m11, m21, 0 }  \
            } \
       } \
    }

#define NN_UTIL_NEON_MATRIX_ROW_MAJOR_4X3F_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) \
    { \
        { \
            { \
                { m00, m01, m02, 0 }, \
                { m10, m11, m12, 0 }, \
                { m20, m21, m22, 0 }, \
                { m30, m31, m32, 0 }  \
            } \
        } \
    }

#define NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) \
    { \
       { \
            { \
                { m00, m10, m20, m30 }, \
                { m01, m11, m21, m31 }, \
                { m02, m12, m22, m32 }  \
            } \
       } \
    }

#define NN_UTIL_NEON_MATRIX_ROW_MAJOR_4X4F_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
    { \
        { \
            { \
                { m00, m01, m02, m03 }, \
                { m10, m11, m12, m13 }, \
                { m20, m21, m22, m23 }, \
                { m30, m31, m32, m33 }  \
            } \
        } \
    }

#define NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
    { \
        { \
            { \
                { m00, m10, m20, m30 }, \
                { m01, m11, m21, m31 }, \
                { m02, m12, m22, m32 }, \
                { m03, m13, m23, m33 }, \
            } \
        } \
    }

#define NN_UTIL_NEON_MATRIX_3X2F_INITIALIZER     NN_UTIL_NEON_MATRIX_ROW_MAJOR_3X2F_INITIALIZER

#define NN_UTIL_NEON_MATRIX_T3X2F_INITIALIZER    NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER

#define NN_UTIL_NEON_MATRIX_4X3F_INITIALIZER     NN_UTIL_NEON_MATRIX_ROW_MAJOR_4X3F_INITIALIZER

#define NN_UTIL_NEON_MATRIX_T4X3F_INITIALIZER    NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER

#define NN_UTIL_NEON_MATRIX_4X4F_INITIALIZER     NN_UTIL_NEON_MATRIX_ROW_MAJOR_4X4F_INITIALIZER

#define NN_UTIL_NEON_MATRIX_T4X4F_INITIALIZER    NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER


namespace nn { namespace util { namespace neon {

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

// Matrix4x3Type
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

// Matrix4x4Type
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
    pOutValue->_m = detail::MatrixRowMajor3x2fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fLoad(source);
}

NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fLoad(source);
}

//
// 書き出し
//
NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fStore(pOutValue, source._m);
}

NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fStore(pOutValue, source._m);
}

//
// 変換
//

NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fConvertFromRowMajor4x4f(source._m);
}

NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fConvertFromRowMajor4x3f(source._m);
}

NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fConvertFromColumnMajor4x4f(source._m);
}

NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fConvertFromColumnMajor4x3f(source._m);
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
    pOutValue->_m = detail::MatrixRowMajor3x2fSet(
        m00, m01,
        m10, m11,
        m20, m21);
}

NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fSetRow(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetRows(row0._v, row1._v, row2._v);
}

NN_FORCEINLINE void       MatrixGetRow(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor3x2fGetRow(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fGetRows(&pOutRow0->_v, &pOutRow1->_v, &pOutRow2->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fSetColumn(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetColumns(column0._v, column1._v);
}

NN_FORCEINLINE void       MatrixGetColumn(Vector3fType *pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor3x2fGetColumn(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fGetColumns(&pOutColumn0->_v, &pOutColumn1->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fSetAxisX(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fSetAxisY(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fSetAxisZ(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetAxes(axisX._v, axisY._v, axisZ._v);
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor3x2fGetAxisX(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor3x2fGetAxisY(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType *pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor3x2fGetAxisZ(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor3x2fGetAxes(&pOutAxisX->_v, &pOutAxisY->_v, &pOutAxisZ->_v, matrix._m);
}

// 4x3 行列
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x3fType* pOutValue,
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSet(
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22,
        m30, m31, m32);
}

NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fSetRow(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetRows(row0._v, row1._v, row2._v, row3._v);
}

NN_FORCEINLINE void       MatrixGetRow(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x3fGetRow(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fGetRows(&pOutRow0->_v, &pOutRow1->_v, &pOutRow2->_v, &pOutRow3->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fSetColumn(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetColumns(column0._v, column1._v, column2._v);
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x3fGetColumn(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fGetColumns(&pOutColumn0->_v, &pOutColumn1->_v, &pOutColumn2->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fSetAxisX(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fSetAxisY(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fSetAxisZ(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fSetAxisW(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetAxes(axisX._v, axisY._v, axisZ._v, axisW._v);
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x3fGetAxisX(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x3fGetAxisY(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x3fGetAxisZ(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType *pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x3fGetAxisW(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x3fGetAxes(&pOutAxisX->_v, &pOutAxisY->_v, &pOutAxisZ->_v, &pOutAxisW->_v, matrix._m);
}

// 4x4 行列
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x4fType* pOutValue,
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fSet(
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33);
}

NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fSetRow(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fSetRows(row0._v, row1._v, row2._v, row3._v);
}

NN_FORCEINLINE void       MatrixGetRow(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x4fGetRow(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fGetRows(&pOutRow0->_v, &pOutRow1->_v, &pOutRow2->_v, &pOutRow3->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fSetColumn(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fSetColumns(column0._v, column1._v, column2._v, column3._v);
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x4fGetColumn(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fGetColumns(&pOutColumn0->_v, &pOutColumn1->_v, &pOutColumn2->_v, &pOutColumn3->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fSetAxisX(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fSetAxisY(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fSetAxisZ(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fSetAxisW(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fSetAxes(axisX._v, axisY._v, axisZ._v, axisW._v);
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x4fGetAxisX(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x4fGetAxisY(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x4fGetAxisZ(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType *pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixRowMajor4x4fGetAxisW(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    detail::MatrixRowMajor4x4fGetAxes(&pOutAxisX->_v, &pOutAxisY->_v, &pOutAxisZ->_v, &pOutAxisW->_v, matrix._m);
}

// T3x2 行列
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor3x2fType* pOutValue,
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSet(
        m00, m01,
        m10, m11,
        m20, m21);
}

NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fSetRow(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetRows(row0._v, row1._v, row2._v);
}

NN_FORCEINLINE void       MatrixGetRow(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor3x2fGetRow(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fGetRows(&pOutRow0->_v, &pOutRow1->_v, &pOutRow2->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fSetColumn(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetColumns(column0._v, column1._v);
}

NN_FORCEINLINE void       MatrixGetColumn(Vector3fType *pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor3x2fGetColumn(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fGetColumns(&pOutColumn0->_v, &pOutColumn1->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fSetAxisX(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fSetAxisY(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fSetAxisZ(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetAxes(axisX._v, axisY._v, axisZ._v);
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor3x2fGetAxisX(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor3x2fGetAxisY(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType *pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor3x2fGetAxisZ(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor3x2fGetAxes(&pOutAxisX->_v, &pOutAxisY->_v, &pOutAxisZ->_v, matrix._m);
}

// T4x3 行列
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x3fType* pOutValue,
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSet(
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22,
        m30, m31, m32);
}

NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fSetRow(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetRows(row0._v, row1._v, row2._v, row3._v);
}

NN_FORCEINLINE void       MatrixGetRow(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x3fGetRow(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fGetRows(&pOutRow0->_v, &pOutRow1->_v, &pOutRow2->_v, &pOutRow3->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fSetColumn(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetColumns(column0._v, column1._v, column2._v);
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x3fGetColumn(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fGetColumns(&pOutColumn0->_v, &pOutColumn1->_v, &pOutColumn2->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fSetAxisX(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fSetAxisY(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fSetAxisZ(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fSetAxisW(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetAxes(axisX._v, axisY._v, axisZ._v, axisW._v);
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x3fGetAxisX(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x3fGetAxisY(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x3fGetAxisZ(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType *pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x3fGetAxisW(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x3fGetAxes(&pOutAxisX->_v, &pOutAxisY->_v, &pOutAxisZ->_v, &pOutAxisW->_v, matrix._m);
}

// T4x4 行列
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x4fType* pOutValue,
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fSet(
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33);
}

NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fSetRow(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fSetRows(row0._v, row1._v, row2._v, row3._v);
}

NN_FORCEINLINE void       MatrixGetRow(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x4fGetRow(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fGetRows(&pOutRow0->_v, &pOutRow1->_v, &pOutRow2->_v, &pOutRow3->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fSetColumn(&pOutValue->_m, index, vector._v);
}

NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fSetColumns(column0._v, column1._v, column2._v, column3._v);
}

NN_FORCEINLINE void       MatrixGetColumn(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x4fGetColumn(matrix._m, index);
}

NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fGetColumns(&pOutColumn0->_v, &pOutColumn1->_v, &pOutColumn2->_v, &pOutColumn3->_v, matrix._m);
}

NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fSetAxisX(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fSetAxisY(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fSetAxisZ(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fSetAxisW(&pOutValue->_m, vector._v);
}

NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fSetAxes(axisX._v, axisY._v, axisZ._v, axisW._v);
}

NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x4fGetAxisX(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x4fGetAxisY(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x4fGetAxisZ(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType *pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_v = detail::MatrixColumnMajor4x4fGetAxisW(matrix._m);
}

NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    detail::MatrixColumnMajor4x4fGetAxes(&pOutAxisX->_v, &pOutAxisY->_v, &pOutAxisZ->_v, &pOutAxisW->_v, matrix._m);
}

//
// 行列
//
// 3x2 行列
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fZero();
}

NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fIdentity();
}

NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fAdd(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSubtract(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fMultiply(matrix._m, factor);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fMultiply(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor3x2fType* pOutValue, float t, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fMultiplyAdd(t, matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fDivide(matrix._m, factor);
}

NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixRowMajor3x2fInverse(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fTranspose(matrix._m);
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixRowMajor3x2fInverseTranspose(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetTranslate(pOutValue->_m, translate._v);
}

NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetScale(pOutValue->_m, scale._v);
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetRotate(pOutValue->_m, rotationRadian);
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor3x2fSetScaleRotate(pOutValue->_m, scale._v, rotationRadian);
}

// 4x3 行列
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fZero();
}

NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fIdentity();
}

NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fAdd(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSubtract(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fMultiply(matrix._m, factor);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fMultiply(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fMultiplyRowMajor4x4f(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor4x3fType* pOutValue, float t, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fMultiplyAdd(t, matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fDivide(matrix._m, factor);
}

NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixRowMajor4x3fInverse(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fTranspose(matrix._m);
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixRowMajor4x3fInverseTranspose(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fLookAtRightHanded(cameraPosition._v, target._v, up._v);
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fLookAtRightHanded(cameraPosition._v, target._v, twistRadian);
}

NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fCameraRotateZxy(cameraPosition._v, rotationRadian._v);
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetTranslate(pOutValue->_m, translate._v);
}

NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetScale(pOutValue->_m, scale._v);
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetRotate(pOutValue->_m, rotationQuaternion._v);
}

NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetRotateXyz(pOutValue->_m, rotationRadian._v);
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetScaleRotate(pOutValue->_m, scale._v, rotationQuaternion._v);
}

NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fSetScaleRotateXyz(pOutValue->_m, scale._v, rotationRadian._v);
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fTextureProjectionPerspectiveOffCenterRightHanded(left, right, bottom, top, nearZ);
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fTextureProjectionPerspectiveFieldOfViewRightHanded(fovyRadian, aspect);
}

NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fTextureProjectionOrthographicOffCenterRightHanded(left, right, bottom, top);
}

// 4x4 行列
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fZero();
}

NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fIdentity();
}

NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fAdd(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fSubtract(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fMultiply(matrix._m, factor);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fMultiplyRowMajor4x3f(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fMultiply(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fDivide(matrix._m, factor);
}

NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixRowMajor4x4fInverse(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fTranspose(matrix._m);
}

NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fPerspectiveOffCenterRightHanded(left, right, bottom, top, nearZ, farZ);
}

NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fPerspectiveFieldOfViewRightHanded(fovyRadian, aspect, nearZ, farZ);
}

NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x4fOrthographicOffCenterRightHanded(left, right, bottom, top, nearZ, farZ);
}

// T3x2 行列
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fZero();
}

NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fIdentity();
}

NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fAdd(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSubtract(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fMultiply(matrix._m, factor);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fMultiply(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor3x2fType* pOutValue, float t, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fMultiplyAdd(t, matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fDivide(matrix._m, factor);
}

NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixColumnMajor3x2fInverse(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fTranspose(matrix._m);
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixColumnMajor3x2fInverseTranspose(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetTranslate(pOutValue->_m, translate._v);
}

NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetScale(pOutValue->_m, scale._v);
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetRotate(pOutValue->_m, rotationRadian);
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor3x2fSetScaleRotate(pOutValue->_m, scale._v, rotationRadian);
}

// T4x3 行列
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fZero();
}

NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fIdentity();
}

NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fAdd(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSubtract(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fMultiply(matrix._m, factor);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fMultiply(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fMultiplyColumnMajor4x4f(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor4x3fType* pOutValue, float t, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fMultiplyAdd(t, matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fDivide(matrix._m, factor);
}

NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixColumnMajor4x3fInverse(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fTranspose(matrix._m);
}

NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixColumnMajor4x3fInverseTranspose(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;

}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fLookAtRightHanded(cameraPosition._v, target._v, up._v);
}

NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fLookAtRightHanded(cameraPosition._v, target._v, twistRadian);
}

NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fCameraRotateZxy(cameraPosition._v, rotationRadian._v);
}

NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetTranslate(pOutValue->_m, translate._v);
}

NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetScale(pOutValue->_m, scale._v);
}

NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetRotate(pOutValue->_m, rotationQuaternion._v);
}

NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetRotateXyz(pOutValue->_m, rotationRadian._v);
}

NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetScaleRotate(pOutValue->_m, scale._v, rotationQuaternion._v);
}

NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fSetScaleRotateXyz(pOutValue->_m, scale._v, rotationRadian._v);
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fTextureProjectionPerspectiveOffCenterRightHanded(left, right, bottom, top, nearZ);
}

NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fTextureProjectionPerspectiveFieldOfViewRightHanded(fovyRadian, aspect);
}

NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fTextureProjectionOrthographicOffCenterRightHanded(left, right, bottom, top);
}

// T4x4 行列
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fZero();
}

NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fIdentity();
}

NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fAdd(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fSubtract(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fMultiply(matrix._m, factor);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fMultiplyColumnMajor4x3f(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fMultiply(matrix1._m, matrix2._m);
}

NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fDivide(matrix._m, factor);
}

NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminant;

    pOutValue->_m = detail::MatrixColumnMajor4x4fInverse(&vDeterminant, matrix._m);

    return detail::Vector4fGetX(vDeterminant) != 0.f;
}

NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fTranspose(matrix._m);
}

NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fPerspectiveOffCenterRightHanded(left, right, bottom, top, nearZ, farZ);
}

NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fPerspectiveFieldOfViewRightHanded(fovyRadian, aspect, nearZ, farZ);
}

NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x4fOrthographicOffCenterRightHanded(left, right, bottom, top, nearZ, farZ);
}

//
// ベクトル・行列ユーティリティ
//
//
// 4x3 行列
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixRowMajor4x3fFromQuaternion(quaternion._v);
}

// T4x3 行列
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT
{
    pOutValue->_m = detail::MatrixColumnMajor4x3fFromQuaternion(quaternion._v);
}

}}} // namespace nn::util::neon
