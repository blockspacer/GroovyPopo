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
#include <nn/util/detail/util_MatrixApiDetail.neon.h>
#include <nn/util/detail/util_VectorApiDetail.neon.h>
#include <cmath>
#include <arm_neon.h>

namespace nn { namespace util { namespace neon { namespace detail {

//
// 4x4, T4x4 共通の処理をまとめた関数
//

NN_FORCEINLINE float32x4x4_t Matrix4x4fInverse(float32x4_t *pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4x4_t matrixTransposed = Matrix4x4fTranspose(matrix);

    float32x4_t xAxis1000 = Vector4fSwizzle(matrixTransposed.val[0], 1, 0, 0, 0);
    float32x4_t xAxis2211 = Vector4fSwizzle(matrixTransposed.val[0], 2, 2, 1, 1);
    float32x4_t xAxis3332 = Vector4fSwizzle(matrixTransposed.val[0], 3, 3, 3, 2);
    float32x4_t yAxis1000 = Vector4fSwizzle(matrixTransposed.val[1], 1, 0, 0, 0);
    float32x4_t yAxis2211 = Vector4fSwizzle(matrixTransposed.val[1], 2, 2, 1, 1);
    float32x4_t yAxis3332 = Vector4fSwizzle(matrixTransposed.val[1], 3, 3, 3, 2);
    float32x4_t zAxis1000 = Vector4fSwizzle(matrixTransposed.val[2], 1, 0, 0, 0);
    float32x4_t zAxis2211 = Vector4fSwizzle(matrixTransposed.val[2], 2, 2, 1, 1);
    float32x4_t zAxis3332 = Vector4fSwizzle(matrixTransposed.val[2], 3, 3, 3, 2);
    float32x4_t wAxis2211 = Vector4fSwizzle(matrixTransposed.val[3], 2, 2, 1, 1);
    float32x4_t wAxis1000 = Vector4fSwizzle(matrixTransposed.val[3], 1, 0, 0, 0);
    float32x4_t wAxis3332 = Vector4fSwizzle(matrixTransposed.val[3], 3, 3, 3, 2);

    float32x4_t tmp0, tmp1, tmp2;

    const float32x4_t Row0Row2Sign = { 1.f, -1.f, 1.f, -1.f };
    const float32x4_t Row1Row3Sign = { -1.f, 1.f, -1.f, 1.f };

    tmp0 = vsubq_f32(vmulq_f32(zAxis2211, wAxis3332), vmulq_f32(zAxis3332, wAxis2211));
    tmp1 = vsubq_f32(vmulq_f32(zAxis1000, wAxis3332), vmulq_f32(zAxis3332, wAxis1000));
    tmp2 = vsubq_f32(vmulq_f32(zAxis1000, wAxis2211), vmulq_f32(zAxis2211, wAxis1000));

    float32x4_t row0 = vmulq_f32(yAxis1000, tmp0);
    row0 = Vector4fMultiplySubtract(row0, yAxis2211, tmp1);
    row0 = Vector4fMultiplyAdd(row0, yAxis3332, tmp2);
    row0 = vmulq_f32(Row0Row2Sign, row0);

    float32x4_t row1 = vmulq_f32(xAxis1000, tmp0);
    row1 = Vector4fMultiplySubtract(row1, xAxis2211, tmp1);
    row1 = Vector4fMultiplyAdd(row1, xAxis3332, tmp2);
    row1 = vmulq_f32(Row1Row3Sign, row1);

    tmp0 = vsubq_f32(vmulq_f32(xAxis2211, yAxis3332), vmulq_f32(xAxis3332, yAxis2211));
    tmp1 = vsubq_f32(vmulq_f32(xAxis1000, yAxis3332), vmulq_f32(xAxis3332, yAxis1000));
    tmp2 = vsubq_f32(vmulq_f32(xAxis1000, yAxis2211), vmulq_f32(xAxis2211, yAxis1000));

    float32x4_t row2 = vmulq_f32(wAxis1000, tmp0);
    row2 = Vector4fMultiplySubtract(row2, wAxis2211, tmp1);
    row2 = Vector4fMultiplyAdd(row2, wAxis3332, tmp2);
    row2 = vmulq_f32(Row0Row2Sign, row2);

    float32x4_t row3 = vmulq_f32(zAxis1000, tmp0);
    row3 = Vector4fMultiplySubtract(row3, zAxis2211, tmp1);
    row3 = Vector4fMultiplyAdd(row3, zAxis3332, tmp2);
    row3 = vmulq_f32(Row1Row3Sign, row3);

    float32x4_t vDeterminant = Vector4fDot(matrixTransposed.val[0], row0);
    *pDeterminant = vDeterminant;

    float32x4_t vReciprocalDeterminant = Vector4fRecp(vDeterminant);

    float32x4x4_t vResult;
    vResult.val[0] = vmulq_f32(vReciprocalDeterminant, row0);
    vResult.val[1] = vmulq_f32(vReciprocalDeterminant, row1);
    vResult.val[2] = vmulq_f32(vReciprocalDeterminant, row2);
    vResult.val[3] = vmulq_f32(vReciprocalDeterminant, row3);

    float32x4_t vZero = Vector4fZero();
    uint32x4_t vDeterminantEqualZero = vceqq_f32(vDeterminant, vZero);

    vResult.val[0] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[0]);
    vResult.val[1] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[1]);
    vResult.val[2] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[2]);
    vResult.val[3] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t Matrix4x4fTranspose(float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4x2_t vTmp0 = vzipq_f32(matrix.val[0], matrix.val[2]);
    float32x4x2_t vTmp1 = vzipq_f32(matrix.val[1], matrix.val[3]);
    float32x4x2_t vTranspose0 = vzipq_f32(vTmp0.val[0], vTmp1.val[0]);
    float32x4x2_t vTranspose1 = vzipq_f32(vTmp0.val[1], vTmp1.val[1]);

    float32x4x4_t vResult;
    vResult.val[0] = vTranspose0.val[0];
    vResult.val[1] = vTranspose0.val[1];
    vResult.val[2] = vTranspose1.val[0];
    vResult.val[3] = vTranspose1.val[1];

    return vResult;
}

//
// 4x4 行列
//

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fConvertFromRowMajor4x3f(float32x4x4_t source) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = source.val[0];
    vResult.val[1] = source.val[1];
    vResult.val[2] = source.val[2];
    vResult.val[3] = vsetq_lane_f32(1.f, source.val[3], 3);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fLoad(const FloatRowMajor4x4& source) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fLoad(source.m[0]);
    vResult.val[1] = Vector4fLoad(source.m[1]);
    vResult.val[2] = Vector4fLoad(source.m[2]);
    vResult.val[3] = Vector4fLoad(source.m[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fLoad(const FloatColumnMajor4x4& source) NN_NOEXCEPT
{
    float32x4_t vColumn1 = Vector4fLoad(source.m[0]);
    float32x4_t vColumn2 = Vector4fLoad(source.m[1]);
    float32x4_t vColumn3 = Vector4fLoad(source.m[2]);
    float32x4_t vColumn4 = Vector4fLoad(source.m[3]);

    float32x4x2_t vTmp1 = vzipq_f32(vColumn1, vColumn2);
    float32x4x2_t vTmp2 = vzipq_f32(vColumn3, vColumn4);

    float32x4x4_t vResult;
    vResult.val[0] = vcombine_f32(vget_low_f32(vTmp1.val[0]), vget_low_f32(vTmp2.val[0]));
    vResult.val[1] = vcombine_f32(vget_high_f32(vTmp1.val[0]), vget_high_f32(vTmp2.val[0]));
    vResult.val[2] = vcombine_f32(vget_low_f32(vTmp1.val[1]), vget_low_f32(vTmp2.val[1]));
    vResult.val[3] = vcombine_f32(vget_high_f32(vTmp1.val[1]), vget_high_f32(vTmp2.val[1]));

    return vResult;
}

NN_FORCEINLINE void MatrixRowMajor4x4fStore(FloatRowMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT
{
    Vector4fStore(pOutValue->m[0], source.val[0]);
    Vector4fStore(pOutValue->m[1], source.val[1]);
    Vector4fStore(pOutValue->m[2], source.val[2]);
    Vector4fStore(pOutValue->m[3], source.val[3]);
}

NN_FORCEINLINE void MatrixRowMajor4x4fStore(FloatColumnMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT
{
    float32x4x4_t matrixT = MatrixRowMajor4x4fTranspose(source);

    Vector4fStore(pOutValue->m[0], matrixT.val[0]);
    Vector4fStore(pOutValue->m[1], matrixT.val[1]);
    Vector4fStore(pOutValue->m[2], matrixT.val[2]);
    Vector4fStore(pOutValue->m[3], matrixT.val[3]);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSet(
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSet(m00, m01, m02, m03);
    vResult.val[1] = Vector4fSet(m10, m11, m12, m13);
    vResult.val[2] = Vector4fSet(m20, m21, m22, m23);
    vResult.val[3] = Vector4fSet(m30, m31, m32, m33);

    return vResult;
}

NN_FORCEINLINE void MatrixRowMajor4x4fSetRow(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[index] = vector;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = row0;
    vResult.val[1] = row1;
    vResult.val[2] = row2;
    vResult.val[3] = row3;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetRow(const float32x4x4_t& matrix, int index) NN_NOEXCEPT
{
    return matrix.val[index];
}

NN_FORCEINLINE void MatrixRowMajor4x4fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x4_t& matrix) NN_NOEXCEPT
{
    *pOutRow0 = matrix.val[0];
    *pOutRow1 = matrix.val[1];
    *pOutRow2 = matrix.val[2];
    *pOutRow3 = matrix.val[3];
}

NN_FORCEINLINE void MatrixRowMajor4x4fSetColumn(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetElement(pOutValue->val[0], index, Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetElement(pOutValue->val[1], index, Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetElement(pOutValue->val[2], index, Vector4fGetZ(vector));
    pOutValue->val[3] = Vector4fSetElement(pOutValue->val[3], index, Vector4fGetW(vector));
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2, const float32x4_t& column3) NN_NOEXCEPT
{
    float32x4x4_t vTmp;
    vTmp.val[0] = column0;
    vTmp.val[1] = column1;
    vTmp.val[2] = column2;
    vTmp.val[3] = column3;

    return Matrix4x4fTranspose(vTmp);
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetColumn(const float32x4x4_t& matrix, int index) NN_NOEXCEPT
{
    float x = Vector4fGetElement(matrix.val[0], index);
    float y = Vector4fGetElement(matrix.val[1], index);
    float z = Vector4fGetElement(matrix.val[2], index);
    float w = Vector4fGetElement(matrix.val[3], index);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE void MatrixRowMajor4x4fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, float32x4_t* pOutColumn3, const float32x4x4_t& matrix) NN_NOEXCEPT
{
    float32x4x4_t vTranspose = Matrix4x4fTranspose(matrix);

    *pOutColumn0 = vTranspose.val[0];
    *pOutColumn1 = vTranspose.val[1];
    *pOutColumn2 = vTranspose.val[2];
    *pOutColumn3 = vTranspose.val[3];
}

NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisX(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = vector;
}

NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisY(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[1] = vector;
}

NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisZ(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[2] = vector;
}

NN_FORCEINLINE void MatrixRowMajor4x4fSetAxisW(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[3] = vector;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = axisX;
    vResult.val[1] = axisY;
    vResult.val[2] = axisZ;
    vResult.val[3] = axisW;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisX(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[0];
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisY(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[1];
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisZ(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[2];
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x4fGetAxisW(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[3];
}

NN_FORCEINLINE void MatrixRowMajor4x4fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x4_t matrix) NN_NOEXCEPT
{
    *pOutAxisX = matrix.val[0];
    *pOutAxisY = matrix.val[1];
    *pOutAxisZ = matrix.val[2];
    *pOutAxisW = matrix.val[3];
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fZero() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = vZero;
    vResult.val[1] = vZero;
    vResult.val[2] = vZero;
    vResult.val[3] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fIdentity() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 1.f);
    vResult.val[1] = Vector4fSetY(vZero, 1.f);
    vResult.val[2] = Vector4fSetZ(vZero, 1.f);
    vResult.val[3] = Vector4fSetW(vZero, 1.f);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fAdd(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fAdd(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fAdd(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fAdd(matrix1.val[2], matrix2.val[2]);
    vResult.val[3] = Vector4fAdd(matrix1.val[3], matrix2.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fSubtract(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSubtract(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fSubtract(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fSubtract(matrix1.val[2], matrix2.val[2]);
    vResult.val[3] = Vector4fSubtract(matrix1.val[3], matrix2.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fMultiply(float32x4x4_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fMultiply(matrix.val[0], factor);
    vResult.val[1] = Vector4fMultiply(matrix.val[1], factor);
    vResult.val[2] = Vector4fMultiply(matrix.val[2], factor);
    vResult.val[3] = Vector4fMultiply(matrix.val[3], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fMultiplyRowMajor4x3f(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;

    float32x4_t matrix2Row3 = Vector4fSelect(matrix2.val[3], Vector4fValueIdentityAxisW, Vector4uValueSelect1110);

    vLow = vget_low_f32(matrix1.val[0]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2Row3, vHigh);
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix1.val[1]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2Row3, vHigh);
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix1.val[2]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2Row3, vHigh);
    vResult.val[2] = vTmp;

    vLow = vget_low_f32(matrix1.val[3]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[3]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2Row3, vHigh);
    vResult.val[3] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fMultiply(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;

    vLow = vget_low_f32(matrix1.val[0]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[3], vHigh);
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix1.val[1]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[3], vHigh);
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix1.val[2]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[3], vHigh);
    vResult.val[2] = vTmp;

    vLow = vget_low_f32(matrix1.val[3]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[3]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[3], vHigh);
    vResult.val[3] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fDivide(float32x4x4_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fDivide(matrix.val[0], factor);
    vResult.val[1] = Vector4fDivide(matrix.val[1], factor);
    vResult.val[2] = Vector4fDivide(matrix.val[2], factor);
    vResult.val[3] = Vector4fDivide(matrix.val[3], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fInverse(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT
{
    return Matrix4x4fInverse(pDeterminant, matrix);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fTranspose(float32x4x4_t matrix) NN_NOEXCEPT
{
    return Matrix4x4fTranspose(matrix);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, (2 * nearZ) * reciprocalWidth);
    vResult.val[1] = Vector4fSetY(vZero, (2 * nearZ) * reciprocalHeight);
    vResult.val[2] = Vector4fSet((right + left) * reciprocalWidth, (top + bottom) * reciprocalHeight, farZ * reciprocalDepth, -1.f);
    vResult.val[3] = Vector4fSetZ(vZero, (farZ * nearZ) * reciprocalDepth);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT
{
    float angle = fovyRadian * 0.5f;
    float tangent = nn::util::TanEst(angle);
    float cotangent = 1.f / tangent;
    float reciprocalDepth = -1.f / (farZ - nearZ);

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, cotangent / aspect);
    vResult.val[1] = Vector4fSetY(vZero, cotangent);
    vResult.val[2] = Vector4fSet(0.f, 0.f, farZ * reciprocalDepth, -1.f);
    vResult.val[3] = Vector4fSetZ(vZero, (farZ * nearZ) * reciprocalDepth);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x4fOrthographicOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 2.f * reciprocalWidth);
    vResult.val[1] = Vector4fSetY(vZero, 2.f * reciprocalHeight);
    vResult.val[2] = Vector4fSetZ(vZero, reciprocalDepth);
    vResult.val[3] = Vector4fSet(-(right + left) * reciprocalWidth, -(top + bottom) * reciprocalHeight, nearZ * reciprocalDepth, 1.f);

    return vResult;
}

//
// T4x4 行列
//

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fConvertFromColumnMajor4x3f(float32x4x3_t source) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = source.val[0];
    vResult.val[1] = source.val[1];
    vResult.val[2] = source.val[2];
    vResult.val[3] = Vector4fValueIdentityAxisW;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fLoad(const FloatRowMajor4x4& source) NN_NOEXCEPT
{
    float32x4_t vRow1 = Vector4fLoad(source.m[0]);
    float32x4_t vRow2 = Vector4fLoad(source.m[1]);
    float32x4_t vRow3 = Vector4fLoad(source.m[2]);
    float32x4_t vRow4 = Vector4fLoad(source.m[3]);

    float32x4x2_t vTmp1 = vzipq_f32(vRow1, vRow2);
    float32x4x2_t vTmp2 = vzipq_f32(vRow3, vRow4);

    float32x4x4_t vResult;
    vResult.val[0] = vcombine_f32(vget_low_f32(vTmp1.val[0]), vget_low_f32(vTmp2.val[0]));
    vResult.val[1] = vcombine_f32(vget_high_f32(vTmp1.val[0]), vget_high_f32(vTmp2.val[0]));
    vResult.val[2] = vcombine_f32(vget_low_f32(vTmp1.val[1]), vget_low_f32(vTmp2.val[1]));
    vResult.val[3] = vcombine_f32(vget_high_f32(vTmp1.val[1]), vget_high_f32(vTmp2.val[1]));

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fLoad(const FloatColumnMajor4x4& source) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fLoad(source.m[0]);
    vResult.val[1] = Vector4fLoad(source.m[1]);
    vResult.val[2] = Vector4fLoad(source.m[2]);
    vResult.val[3] = Vector4fLoad(source.m[3]);

    return vResult;
}

NN_FORCEINLINE void MatrixColumnMajor4x4fStore(FloatRowMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT
{
    float32x4x4_t matrixT = MatrixColumnMajor4x4fTranspose(source);

    Vector4fStore(pOutValue->m[0], matrixT.val[0]);
    Vector4fStore(pOutValue->m[1], matrixT.val[1]);
    Vector4fStore(pOutValue->m[2], matrixT.val[2]);
    Vector4fStore(pOutValue->m[3], matrixT.val[3]);
}

NN_FORCEINLINE void MatrixColumnMajor4x4fStore(FloatColumnMajor4x4* pOutValue, float32x4x4_t source) NN_NOEXCEPT
{
    Vector4fStore(pOutValue->m[0], source.val[0]);
    Vector4fStore(pOutValue->m[1], source.val[1]);
    Vector4fStore(pOutValue->m[2], source.val[2]);
    Vector4fStore(pOutValue->m[3], source.val[3]);
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSet(
                            float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSet(m00, m10, m20, m30);
    vResult.val[1] = Vector4fSet(m01, m11, m21, m31);
    vResult.val[2] = Vector4fSet(m02, m12, m22, m32);
    vResult.val[3] = Vector4fSet(m03, m13, m23, m33);

    return vResult;
}

NN_FORCEINLINE void MatrixColumnMajor4x4fSetRow(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetElement(pOutValue->val[0], index, Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetElement(pOutValue->val[1], index, Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetElement(pOutValue->val[2], index, Vector4fGetZ(vector));
    pOutValue->val[3] = Vector4fSetElement(pOutValue->val[3], index, Vector4fGetW(vector));
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT
{
    float32x4x4_t vTmp;
    vTmp.val[0] = row0;
    vTmp.val[1] = row1;
    vTmp.val[2] = row2;
    vTmp.val[3] = row3;

    return Matrix4x4fTranspose(vTmp);
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetRow(const float32x4x4_t& matrix, int index) NN_NOEXCEPT
{
    float x = Vector4fGetElement(matrix.val[0], index);
    float y = Vector4fGetElement(matrix.val[1], index);
    float z = Vector4fGetElement(matrix.val[2], index);
    float w = Vector4fGetElement(matrix.val[3], index);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE void MatrixColumnMajor4x4fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x4_t& matrix) NN_NOEXCEPT
{
    float32x4x4_t vTranspose = Matrix4x4fTranspose(matrix);

    *pOutRow0 = vTranspose.val[0];
    *pOutRow1 = vTranspose.val[1];
    *pOutRow2 = vTranspose.val[2];
    *pOutRow3 = vTranspose.val[3];
}

NN_FORCEINLINE void MatrixColumnMajor4x4fSetColumn(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[index] = vector;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2, const float32x4_t& column3) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = column0;
    vResult.val[1] = column1;
    vResult.val[2] = column2;
    vResult.val[3] = column3;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetColumn(const float32x4x4_t& matrix, int index) NN_NOEXCEPT
{
    return matrix.val[index];
}

NN_FORCEINLINE void MatrixColumnMajor4x4fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, float32x4_t* pOutColumn3, const float32x4x4_t& matrix) NN_NOEXCEPT
{
    *pOutColumn0 = matrix.val[0];
    *pOutColumn1 = matrix.val[1];
    *pOutColumn2 = matrix.val[2];
    *pOutColumn3 = matrix.val[3];
}

NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisX(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetX(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetX(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetX(pOutValue->val[2], Vector4fGetZ(vector));
    pOutValue->val[3] = Vector4fSetX(pOutValue->val[3], Vector4fGetW(vector));
}

NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisY(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetY(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetY(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetY(pOutValue->val[2], Vector4fGetZ(vector));
    pOutValue->val[3] = Vector4fSetY(pOutValue->val[3], Vector4fGetW(vector));
}

NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisZ(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetZ(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetZ(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetZ(pOutValue->val[2], Vector4fGetZ(vector));
    pOutValue->val[3] = Vector4fSetZ(pOutValue->val[3], Vector4fGetW(vector));
}

NN_FORCEINLINE void MatrixColumnMajor4x4fSetAxisW(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetW(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetW(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetW(pOutValue->val[2], Vector4fGetZ(vector));
    pOutValue->val[3] = Vector4fSetW(pOutValue->val[3], Vector4fGetW(vector));
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT
{
    float32x4x4_t vTmp;
    vTmp.val[0] = axisX;
    vTmp.val[1] = axisY;
    vTmp.val[2] = axisZ;
    vTmp.val[3] = axisW;

    return Matrix4x4fTranspose(vTmp);
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisX(float32x4x4_t matrix) NN_NOEXCEPT
{
    float x = Vector4fGetX(matrix.val[0]);
    float y = Vector4fGetX(matrix.val[1]);
    float z = Vector4fGetX(matrix.val[2]);
    float w = Vector4fGetX(matrix.val[3]);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisY(float32x4x4_t matrix) NN_NOEXCEPT
{
    float x = Vector4fGetY(matrix.val[0]);
    float y = Vector4fGetY(matrix.val[1]);
    float z = Vector4fGetY(matrix.val[2]);
    float w = Vector4fGetY(matrix.val[3]);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisZ(float32x4x4_t matrix) NN_NOEXCEPT
{
    float x = Vector4fGetZ(matrix.val[0]);
    float y = Vector4fGetZ(matrix.val[1]);
    float z = Vector4fGetZ(matrix.val[2]);
    float w = Vector4fGetZ(matrix.val[3]);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x4fGetAxisW(float32x4x4_t matrix) NN_NOEXCEPT
{
    float x = Vector4fGetW(matrix.val[0]);
    float y = Vector4fGetW(matrix.val[1]);
    float z = Vector4fGetW(matrix.val[2]);
    float w = Vector4fGetW(matrix.val[3]);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE void MatrixColumnMajor4x4fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4x4_t vTranspose = Matrix4x4fTranspose(matrix);

    *pOutAxisX = vTranspose.val[0];
    *pOutAxisY = vTranspose.val[1];
    *pOutAxisZ = vTranspose.val[2];
    *pOutAxisW = vTranspose.val[3];
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fZero() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = vZero;
    vResult.val[1] = vZero;
    vResult.val[2] = vZero;
    vResult.val[3] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fIdentity() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 1.f);
    vResult.val[1] = Vector4fSetY(vZero, 1.f);
    vResult.val[2] = Vector4fSetZ(vZero, 1.f);
    vResult.val[3] = Vector4fSetW(vZero, 1.f);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fAdd(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fAdd(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fAdd(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fAdd(matrix1.val[2], matrix2.val[2]);
    vResult.val[3] = Vector4fAdd(matrix1.val[3], matrix2.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fSubtract(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSubtract(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fSubtract(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fSubtract(matrix1.val[2], matrix2.val[2]);
    vResult.val[3] = Vector4fSubtract(matrix1.val[3], matrix2.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fMultiply(float32x4x4_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fMultiply(matrix.val[0], factor);
    vResult.val[1] = Vector4fMultiply(matrix.val[1], factor);
    vResult.val[2] = Vector4fMultiply(matrix.val[2], factor);
    vResult.val[3] = Vector4fMultiply(matrix.val[3], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fMultiplyColumnMajor4x3f(float32x4x4_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;

    vLow = vget_low_f32(matrix2.val[0]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix2.val[1]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix2.val[2]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[2] = vTmp;

    vResult.val[3] = matrix1.val[3];

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fMultiply(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;

    vLow = vget_low_f32(matrix2.val[0]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix2.val[1]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix2.val[2]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[2] = vTmp;

    vLow = vget_low_f32(matrix2.val[3]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[3]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[3], vHigh);
    vResult.val[3] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fDivide(float32x4x4_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fDivide(matrix.val[0], factor);
    vResult.val[1] = Vector4fDivide(matrix.val[1], factor);
    vResult.val[2] = Vector4fDivide(matrix.val[2], factor);
    vResult.val[3] = Vector4fDivide(matrix.val[3], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fInverse(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT
{
    return Matrix4x4fInverse(pDeterminant, matrix);
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fTranspose(float32x4x4_t matrix) NN_NOEXCEPT
{
    return Matrix4x4fTranspose(matrix);
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSet((2 * nearZ) * reciprocalWidth, 0.f, (right + left) * reciprocalWidth, 0.f);
    vResult.val[1] = Vector4fSet(0.f, (2 * nearZ) * reciprocalHeight, (top + bottom) * reciprocalHeight, 0.f);
    vResult.val[2] = Vector4fSet(0.f, 0.f, farZ * reciprocalDepth, farZ * nearZ * reciprocalDepth);
    vResult.val[3] = Vector4fValueNegativeIdentityAxisZ;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT
{
    float angle = fovyRadian * 0.5f;
    float tangent = nn::util::TanEst(angle);
    float cotangent = 1.f / tangent;
    float reciprocalDepth = -1.f / (farZ - nearZ);

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, cotangent / aspect);
    vResult.val[1] = Vector4fSetY(vZero, cotangent);
    vResult.val[2] = Vector4fSet(0.f, 0.f, farZ * reciprocalDepth, (farZ * nearZ) * reciprocalDepth);
    vResult.val[3] = Vector4fValueNegativeIdentityAxisZ;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x4fOrthographicOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT
{
    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);
    float reciprocalDepth = -1.f / (farZ - nearZ);

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSet(2.f * reciprocalWidth, 0.f, 0.f, -(right + left) * reciprocalWidth);
    vResult.val[1] = Vector4fSet(0.f, 2.f * reciprocalHeight, 0.f, -(top + bottom) * reciprocalHeight);
    vResult.val[2] = Vector4fSet(0.f, 0.f, reciprocalDepth, nearZ * reciprocalDepth);
    vResult.val[3] = Vector4fValueIdentityAxisW;

    return vResult;
}

//
// 4x3 行列
//

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fFromQuaternion(float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t xyz = Vector4fMask(quaternion, Vector4uValueMaskXYZ);

    float32x4_t yxx = Vector4fSwizzle(xyz, 1, 0, 0, 3);
    float32x4_t y2x2x2 = vmulq_f32(yxx, yxx);
    float32x4_t y2x2x2Double = vaddq_f32(y2x2x2, y2x2x2);

    float32x4_t zzy = Vector4fSwizzle(xyz, 2, 2, 1, 3);
    float32x4_t z2z2y2 = vmulq_f32(zzy, zzy);
    float32x4_t z2z2y2Double = vaddq_f32(z2z2y2, z2z2y2);

    float32x4_t v0 = vsubq_f32(Vector4fValueOneWZero, vaddq_f32(y2x2x2Double, z2z2y2Double));


    float32x4_t yzx = Vector4fSwizzle(xyz, 1, 2, 0, 3);
    float32x4_t xYyZzX = vmulq_f32(xyz, yzx);
    float32x4_t xYyZxXDouble = vaddq_f32(xYyZzX, xYyZzX);

    float32x4_t zxy = Vector4fSwizzle(xyz, 2, 0, 1, 3);
    float32x4_t wZwXwY = vmulq_n_f32(zxy, Vector4fGetW(quaternion));
    float32x4_t wZwXwYDouble = vaddq_f32(wZwXwY, wZwXwY);

    float32x4_t v1 = vaddq_f32(xYyZxXDouble, wZwXwYDouble);
    float32x4_t v2 = vsubq_f32(xYyZxXDouble, wZwXwYDouble);


    float32x4_t m01m02m10m12 = Vector4fPermute(v1, v2, 0, 6, 4, 1);
    float32x4_t m20m21 = Vector4fPermute(v1, v2, 2, 5, 7, 7);

    float32x4x4_t matrix;

    matrix.val[0] = Vector4fPermute(v0, m01m02m10m12, 0, 4, 5, 3);
    matrix.val[1] = Vector4fPermute(v0, m01m02m10m12, 6, 1, 7, 3);
    matrix.val[2] = Vector4fPermute(v0, m20m21, 4, 5, 2, 3);
    matrix.val[3] = Vector4fZero();

    return matrix;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fConvertFromRowMajor4x4f(float32x4x4_t source) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fMask(source.val[0], Vector4uValueMaskXYZ);
    vResult.val[1] = Vector4fMask(source.val[1], Vector4uValueMaskXYZ);
    vResult.val[2] = Vector4fMask(source.val[2], Vector4uValueMaskXYZ);
    vResult.val[3] = Vector4fMask(source.val[3], Vector4uValueMaskXYZ);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLoad(const FloatRowMajor4x3& source) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector3fLoad(source.m[0]);
    vResult.val[1] = Vector3fLoad(source.m[1]);
    vResult.val[2] = Vector3fLoad(source.m[2]);
    vResult.val[3] = Vector3fLoad(source.m[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLoad(const FloatColumnMajor4x3& source) NN_NOEXCEPT
{
    float32x4_t vColumn1 = Vector4fLoad(source.m[0]);
    float32x4_t vColumn2 = Vector4fLoad(source.m[1]);
    float32x4_t vColumn3 = Vector4fLoad(source.m[2]);

    float32x4x2_t vTmp1 = vzipq_f32(vColumn1, vColumn2);
    float32x4x2_t vTmp2 = vzipq_f32(vColumn3, Vector4fZero());

    float32x4x4_t vResult;
    vResult.val[0] = vcombine_f32(vget_low_f32(vTmp1.val[0]), vget_low_f32(vTmp2.val[0]));
    vResult.val[1] = vcombine_f32(vget_high_f32(vTmp1.val[0]), vget_high_f32(vTmp2.val[0]));
    vResult.val[2] = vcombine_f32(vget_low_f32(vTmp1.val[1]), vget_low_f32(vTmp2.val[1]));
    vResult.val[3] = vcombine_f32(vget_high_f32(vTmp1.val[1]), vget_high_f32(vTmp2.val[1]));

    return vResult;
}

NN_FORCEINLINE void MatrixRowMajor4x3fStore(FloatRowMajor4x3* pOutValue, float32x4x4_t source) NN_NOEXCEPT
{
    Vector4fStore(pOutValue->m[0], source.val[0]);
    Vector4fStore(pOutValue->m[1], source.val[1]);
    Vector4fStore(pOutValue->m[2], source.val[2]);

    pOutValue->m[3][0] = vgetq_lane_f32(source.val[3], 0);
    pOutValue->m[3][1] = vgetq_lane_f32(source.val[3], 1);
    pOutValue->m[3][2] = vgetq_lane_f32(source.val[3], 2);
}

NN_FORCEINLINE void MatrixRowMajor4x3fStore(FloatColumnMajor4x3* pOutValue, float32x4x4_t source) NN_NOEXCEPT
{
    float32x4x4_t matrixT = Matrix4x4fTranspose(source);

    Vector4fStore(pOutValue->m[0], matrixT.val[0]);
    Vector4fStore(pOutValue->m[1], matrixT.val[1]);
    Vector4fStore(pOutValue->m[2], matrixT.val[2]);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSet(
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector3fSet(m00, m01, m02);
    vResult.val[1] = Vector3fSet(m10, m11, m12);
    vResult.val[2] = Vector3fSet(m20, m21, m22);
    vResult.val[3] = Vector3fSet(m30, m31, m32);

    return vResult;
}

NN_FORCEINLINE void MatrixRowMajor4x3fSetRow(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[index] = vector;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = row0;
    vResult.val[1] = row1;
    vResult.val[2] = row2;
    vResult.val[3] = row3;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetRow(const float32x4x4_t& matrix, int index) NN_NOEXCEPT
{
    return matrix.val[index];
}

NN_FORCEINLINE void MatrixRowMajor4x3fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x4_t& matrix) NN_NOEXCEPT
{
    *pOutRow0 = matrix.val[0];
    *pOutRow1 = matrix.val[1];
    *pOutRow2 = matrix.val[2];
    *pOutRow3 = matrix.val[3];
}

NN_FORCEINLINE void MatrixRowMajor4x3fSetColumn(float32x4x4_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector3fSetElement(pOutValue->val[0], index, Vector4fGetX(vector));
    pOutValue->val[1] = Vector3fSetElement(pOutValue->val[1], index, Vector4fGetY(vector));
    pOutValue->val[2] = Vector3fSetElement(pOutValue->val[2], index, Vector4fGetZ(vector));
    pOutValue->val[3] = Vector3fSetElement(pOutValue->val[3], index, Vector4fGetW(vector));
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2) NN_NOEXCEPT
{
    float32x4x4_t vTmp;
    vTmp.val[0] = column0;
    vTmp.val[1] = column1;
    vTmp.val[2] = column2;
    vTmp.val[3] = Vector4fZero();

    return Matrix4x4fTranspose(vTmp);
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetColumn(const float32x4x4_t& matrix, int index) NN_NOEXCEPT
{
    float x = Vector3fGetElement(matrix.val[0], index);
    float y = Vector3fGetElement(matrix.val[1], index);
    float z = Vector3fGetElement(matrix.val[2], index);
    float w = Vector3fGetElement(matrix.val[3], index);

    return Vector4fSet(x, y, z, w);
}

NN_FORCEINLINE void MatrixRowMajor4x3fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, const float32x4x4_t& matrix) NN_NOEXCEPT
{
    float32x4x4_t vTranspose = Matrix4x4fTranspose(matrix);

    *pOutColumn0 = vTranspose.val[0];
    *pOutColumn1 = vTranspose.val[1];
    *pOutColumn2 = vTranspose.val[2];
}

NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisX(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = vector;
}

NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisY(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[1] = vector;
}

NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisZ(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[2] = vector;
}

NN_FORCEINLINE void MatrixRowMajor4x3fSetAxisW(float32x4x4_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[3] = vector;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = axisX;
    vResult.val[1] = axisY;
    vResult.val[2] = axisZ;
    vResult.val[3] = axisW;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisX(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[0];
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisY(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[1];
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisZ(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[2];
}

NN_FORCEINLINE float32x4_t MatrixRowMajor4x3fGetAxisW(float32x4x4_t matrix) NN_NOEXCEPT
{
    return matrix.val[3];
}

NN_FORCEINLINE void MatrixRowMajor4x3fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x4_t matrix) NN_NOEXCEPT
{
    *pOutAxisX = matrix.val[0];
    *pOutAxisY = matrix.val[1];
    *pOutAxisZ = matrix.val[2];
    *pOutAxisW = matrix.val[3];
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fZero() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = vZero;
    vResult.val[1] = vZero;
    vResult.val[2] = vZero;
    vResult.val[3] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fIdentity() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 1.f);
    vResult.val[1] = Vector4fSetY(vZero, 1.f);
    vResult.val[2] = Vector4fSetZ(vZero, 1.f);
    vResult.val[3] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fAdd(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fAdd(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fAdd(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fAdd(matrix1.val[2], matrix2.val[2]);
    vResult.val[3] = Vector4fAdd(matrix1.val[3], matrix2.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSubtract(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSubtract(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fSubtract(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fSubtract(matrix1.val[2], matrix2.val[2]);
    vResult.val[3] = Vector4fSubtract(matrix1.val[3], matrix2.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiply(float32x4x4_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fMultiply(matrix.val[0], factor);
    vResult.val[1] = Vector4fMultiply(matrix.val[1], factor);
    vResult.val[2] = Vector4fMultiply(matrix.val[2], factor);
    vResult.val[3] = Vector4fMultiply(matrix.val[3], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiply(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;

    vLow = vget_low_f32(matrix1.val[0]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix1.val[1]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix1.val[2]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vResult.val[2] = vTmp;

    vLow = vget_low_f32(matrix1.val[3]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[3]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, matrix2.val[3]);
    vResult.val[3] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiplyRowMajor4x4f(float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;

    vLow = vget_low_f32(matrix1.val[0]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix1.val[1]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix1.val[2]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vResult.val[2] = vTmp;

    vLow = vget_low_f32(matrix1.val[3]);
    vTmp = vmulq_lane_f32(matrix2.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix2.val[1], vLow);
    vHigh = vget_high_f32(matrix1.val[3]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix2.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, matrix2.val[3]);
    vResult.val[3] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fMultiplyAdd(float t, float32x4x4_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fMultiplyAdd(matrix2.val[0], matrix1.val[0], t);
    vResult.val[1] = Vector4fMultiplyAdd(matrix2.val[1], matrix1.val[1], t);
    vResult.val[2] = Vector4fMultiplyAdd(matrix2.val[2], matrix1.val[2], t);
    vResult.val[3] = Vector4fMultiplyAdd(matrix2.val[3], matrix1.val[3], t);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fDivide(float32x4x4_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fDivide(matrix.val[0], factor);
    vResult.val[1] = Vector4fDivide(matrix.val[1], factor);
    vResult.val[2] = Vector4fDivide(matrix.val[2], factor);
    vResult.val[3] = Vector4fDivide(matrix.val[3], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fInverse(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4_t vDeterminantTmp0 = matrix.val[0];
    vDeterminantTmp0 = vmulq_f32(vDeterminantTmp0, Vector4fSwizzle(matrix.val[1], 1, 2, 0, 3));
    vDeterminantTmp0 = vmulq_f32(vDeterminantTmp0, Vector4fSwizzle(matrix.val[2], 2, 0, 1, 3));

    float32x4_t vDeterminantTmp1 = matrix.val[0];
    vDeterminantTmp1 = vmulq_f32(vDeterminantTmp1, Vector4fSwizzle(matrix.val[1], 2, 0, 1, 3));
    vDeterminantTmp1 = vmulq_f32(vDeterminantTmp1, Vector4fSwizzle(matrix.val[2], 1, 2, 0, 3));
    vDeterminantTmp1 = vnegq_f32(vDeterminantTmp1);

    vDeterminantTmp0 = vaddq_f32(vDeterminantTmp0, vDeterminantTmp1);
    float32x2_t vDeterminantTmp2 = vpadd_f32(vget_low_f32(vDeterminantTmp0), vget_high_f32(vDeterminantTmp0));

    float32_t determinant = vget_lane_f32(vDeterminantTmp2, 0) + vget_lane_f32(vDeterminantTmp2, 1);

    float32x4_t vDeterminant = vdupq_n_f32(determinant);
    *pDeterminant = vDeterminant;

    float32x4_t vReciprocalDeterminant = Vector4fRecp(vDeterminant);

    float32x4x4_t vTranspose = MatrixRowMajor4x3fTranspose(matrix);
    float32x4x4_t vResult;
    float32x4_t vTmp;

    vTmp = vmulq_f32(
        Vector4fSwizzle(vTranspose.val[1], 1, 2, 0, 3),
        Vector4fSwizzle(vTranspose.val[2], 2, 0, 1, 3)
        );
    vTmp = Vector4fMultiplySubtract(
        vTmp,
        Vector4fSwizzle(vTranspose.val[2], 1, 2, 0, 3),
        Vector4fSwizzle(vTranspose.val[1], 2, 0, 1, 3)
        );
    vTmp = vmulq_f32(vTmp, vReciprocalDeterminant);
    vResult.val[0] = vTmp;

    vTmp = vmulq_f32(
        Vector4fSwizzle(vTranspose.val[2], 1, 2, 0, 3),
        Vector4fSwizzle(vTranspose.val[0], 2, 0, 1, 3)
        );
    vTmp = Vector4fMultiplySubtract(
        vTmp,
        Vector4fSwizzle(vTranspose.val[0], 1, 2, 0, 3),
        Vector4fSwizzle(vTranspose.val[2], 2, 0, 1, 3)
        );
    vTmp = vmulq_f32(vTmp, vReciprocalDeterminant);
    vResult.val[1] = vTmp;

    vTmp = vmulq_f32(
        Vector4fSwizzle(vTranspose.val[0], 1, 2, 0, 3),
        Vector4fSwizzle(vTranspose.val[1], 2, 0, 1, 3)
        );
    vTmp = Vector4fMultiplySubtract(
        vTmp,
        Vector4fSwizzle(vTranspose.val[1], 1, 2, 0, 3),
        Vector4fSwizzle(vTranspose.val[0], 2, 0, 1, 3)
        );
    vTmp = vmulq_f32(vTmp, vReciprocalDeterminant);
    vResult.val[2] = vTmp;

    float32x2_t vLow = vget_low_f32(matrix.val[3]);
    float32x2_t vHigh = vget_high_f32(matrix.val[3]);

    vTmp = vmulq_lane_f32(vnegq_f32(vResult.val[0]), vLow, 0);
    vTmp = Vector4fMultiplySubtractLane1(vTmp, vResult.val[1], vLow);
    vTmp = Vector4fMultiplySubtractLane0(vTmp, vResult.val[2], vHigh);

    vResult.val[3] = vTmp;

    float32x4_t vZero = Vector4fZero();
    uint32x4_t vDeterminantEqualZero = vceqq_f32(vDeterminant, vZero);

    vResult.val[0] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[0]);
    vResult.val[1] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[1]);
    vResult.val[2] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[2]);
    vResult.val[3] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTranspose(float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4x4_t vTemp = matrix;
    vTemp.val[3] = Vector4fZero();

    return Matrix4x4fTranspose(vTemp);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fInverseTranspose(float32x4_t* pDeterminant, float32x4x4_t matrix) NN_NOEXCEPT
{
    float32x4x4_t vResult;

    vResult = MatrixRowMajor4x3fInverse(pDeterminant, matrix);
    vResult = MatrixRowMajor4x3fTranspose(vResult);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float32x4_t up) NN_NOEXCEPT
{
    float32x4_t vLook = Vector4fSubtract(cameraPosition, target);
    Vector4fNormalize(&vLook, vLook);

    float32x4_t vRight = Vector3fCross(up, vLook);
    Vector4fNormalize(&vRight, vRight);

    float32x4_t vUp = Vector3fCross(vLook, vRight);

    float32x4_t vCameraPositionVectorNegative = Vector4fNegate(cameraPosition);

    float32x4_t vDotRight = Vector4fDot(vCameraPositionVectorNegative, vRight);
    float32x4_t vDotUp = Vector4fDot(vCameraPositionVectorNegative, vUp);
    float32x4_t vDotLook = Vector4fDot(vCameraPositionVectorNegative, vLook);

    float32x4x4_t matrix;
    matrix.val[0] = Vector4fSelect(vRight, vDotRight, Vector4uValueSelect1110);
    matrix.val[1] = Vector4fSelect(vUp, vDotUp, Vector4uValueSelect1110);
    matrix.val[2] = Vector4fSelect(vLook, vDotLook, Vector4uValueSelect1110);
    matrix.val[3] = Vector4fZero();

    return Matrix4x4fTranspose(matrix);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float twistRadian) NN_NOEXCEPT
{
    float32x4_t vLookReverse = Vector4fSubtract(cameraPosition, target);

    float32x4_t vRightTmp = Vector4fSet(Vector4fGetZ(vLookReverse), 0.f, -Vector4fGetX(vLookReverse), 0.f);

    Vector4fNormalize(&vLookReverse, vLookReverse);
    Vector4fNormalize(&vRightTmp, vRightTmp);

    float32x4_t vUpTmp = Vector3fCross(vLookReverse, vRightTmp);

    float32x4_t vTwistRadian = vdupq_n_f32(twistRadian);
    float32x4_t vSinTwist, vCosTwist;
    Vector4fSinCos(&vSinTwist, &vCosTwist, vTwistRadian);

    float32x4_t vRight = vaddq_f32(vmulq_f32(vUpTmp, vSinTwist), vmulq_f32(vRightTmp, vCosTwist));
    float32x4_t vUp = vsubq_f32(vmulq_f32(vUpTmp, vCosTwist), vmulq_f32(vRightTmp, vSinTwist));

    float32x4_t vCameraPositionVectorNegative = Vector4fNegate(cameraPosition);

    float32x4_t vDotRight = Vector4fDot(vCameraPositionVectorNegative, vRight);
    float32x4_t vDotUp = Vector4fDot(vCameraPositionVectorNegative, vUp);
    float32x4_t vDotLook = Vector4fDot(vCameraPositionVectorNegative, vLookReverse);

    float32x4x4_t vResult1;
    vResult1.val[0] = Vector4fSelect(vRight, vDotRight, Vector4uValueSelect1110);
    vResult1.val[1] = Vector4fSelect(vUp, vDotUp, Vector4uValueSelect1110);
    vResult1.val[2] = Vector4fSelect(vLookReverse, vDotLook, Vector4uValueSelect1110);
    vResult1.val[3] = Vector4fZero();
    vResult1 = Matrix4x4fTranspose(vResult1);

    float32x4_t vTranslateSign = { -1.f, 1.f, -1.f, 0.f };
    float32x4_t vSign = Vector4fSetX(Vector4fSign(vdupq_n_f32(Vector4fGetY(vLookReverse))), 1.f);

    float32x4x4_t vResult2;
    vResult2.val[0] = Vector4fValueIdentityAxisX;
    vResult2.val[1] = vmulq_f32(vSign, Vector4fValueIdentityAxisZ);
    vResult2.val[2] = vmulq_f32(vSign, Vector4fValueNegativeIdentityAxisY);
    vResult2.val[3] = vmulq_f32(vSign, vmulq_f32(vTranslateSign, Vector4fSwizzle(cameraPosition, 0, 2, 1, 3)));

    float32x4_t vZero = Vector4fZero();
    uint32x4_t vXEqualZero = vceqq_f32(vdupq_n_f32(Vector4fGetX(vLookReverse)), vZero);
    uint32x4_t vZEqualZero = vceqq_f32(vdupq_n_f32(Vector4fGetZ(vLookReverse)), vZero);
    uint32x4_t vXZEqualZero = vandq_u32(vXEqualZero, vZEqualZero);

    float32x4x4_t vResult;
    vResult.val[0] = vbslq_f32(vXZEqualZero, vResult2.val[0], vResult1.val[0]);
    vResult.val[1] = vbslq_f32(vXZEqualZero, vResult2.val[1], vResult1.val[1]);
    vResult.val[2] = vbslq_f32(vXZEqualZero, vResult2.val[2], vResult1.val[2]);
    vResult.val[3] = vbslq_f32(vXZEqualZero, vResult2.val[3], vResult1.val[3]);

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fCameraRotateZxy(float32x4_t cameraPosition, float32x4_t rotationRadian) NN_NOEXCEPT
{
    float32x4_t vSinRadian, vCosRadian;
    Vector4fSinCos(&vSinRadian, &vCosRadian, rotationRadian);

    float sinX = Vector4fGetX(vSinRadian);
    float sinY = Vector4fGetY(vSinRadian);
    float sinZ = Vector4fGetZ(vSinRadian);
    float cosX = Vector4fGetX(vCosRadian);
    float cosY = Vector4fGetY(vCosRadian);
    float cosZ = Vector4fGetZ(vCosRadian);

    float32x4_t sinXOneSinX = Vector4fSet(sinX, 1.f, sinX, 0.f);
    float32x4_t sinYCosXCosY = Vector4fSet(sinY, cosX, cosY, 0.f);
    float32x4_t sinXOneSinX_x_sinYCosXCosY = vmulq_f32(sinXOneSinX, sinYCosXCosY);
    float32x4_t cosYZeroSinY = Vector4fSet(cosY, 0.f, sinY, 0.f);
    float32x4_t cosZZeroCosZ = Vector4fSet(cosZ, 0.f, cosZ, 0.f);
    float32x4_t sinZZeroSinZ = Vector4fSet(sinZ, 0.f, sinZ, 0.f);
    float32x4_t OneOneNegOne = Vector4fSet(1.f, 1.f, -1.f, 0.f);
    float32x4_t NegOneOneOne = Vector4fSet(-1.f, 1.f, 1.f, 0.f);
    float32x4_t cosXNegOnecosX = Vector4fSet(cosX, -1.f, cosX, 0.f);
    float32x4_t sinYSinXcosY = Vector4fSet(sinY, sinX, cosY, 0.f);


    float32x4_t right = vaddq_f32(
        vmulq_n_f32(
        sinXOneSinX_x_sinYCosXCosY,
        sinZ
        ),
        vmulq_f32(
        OneOneNegOne,
        vmulq_f32(cosYZeroSinY, cosZZeroCosZ)
        )
        );

    float32x4_t up = vaddq_f32(
        vmulq_n_f32(
        sinXOneSinX_x_sinYCosXCosY,
        cosZ
        ),
        vmulq_f32(
        NegOneOneOne,
        vmulq_f32(cosYZeroSinY, sinZZeroSinZ)
        )
        );

    float32x4_t back = vmulq_f32(cosXNegOnecosX, sinYSinXcosY);

    float32x4_t cameraPositionVectorNegative = Vector4fNegate(cameraPosition);

    float32x4_t dotRight = Vector4fDot(cameraPositionVectorNegative, right);
    float32x4_t dotUp = Vector4fDot(cameraPositionVectorNegative, up);
    float32x4_t dotBack = Vector4fDot(cameraPositionVectorNegative, back);

    float32x4x4_t matrix;
    matrix.val[0] = Vector4fSelect(right, dotRight, Vector4uValueSelect1110);
    matrix.val[1] = Vector4fSelect(up, dotUp, Vector4uValueSelect1110);
    matrix.val[2] = Vector4fSelect(back, dotBack, Vector4uValueSelect1110);
    matrix.val[3] = Vector4fZero();

    return Matrix4x4fTranspose(matrix);
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetTranslate(float32x4x4_t matrix, float32x4_t translate) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = matrix.val[0];
    vResult.val[1] = matrix.val[1];
    vResult.val[2] = matrix.val[2];
    vResult.val[3] = translate;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetScale(float32x4x4_t matrix, float32x4_t scale) NN_NOEXCEPT
{
    float32x4x4_t vResult;
    vResult.val[0] = Vector4fMask(scale, Vector4uValueMaskX);
    vResult.val[1] = Vector4fMask(scale, Vector4uValueMaskY);
    vResult.val[2] = Vector4fMask(scale, Vector4uValueMaskZ);
    vResult.val[3] = matrix.val[3];

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetRotate(float32x4x4_t matrix, float32x4_t rotationQuaternion) NN_NOEXCEPT
{
    float32x4x4_t rotationMatrix = MatrixRowMajor4x3fFromQuaternion(rotationQuaternion);

    float32x4x4_t vResult;
    vResult.val[0] = rotationMatrix.val[0];
    vResult.val[1] = rotationMatrix.val[1];
    vResult.val[2] = rotationMatrix.val[2];
    vResult.val[3] = matrix.val[3];

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetRotateXyz(float32x4x4_t matrix, float32x4_t rotationRadian) NN_NOEXCEPT
{
    float32x4_t vSinRadian, vCosRadian;
    Vector4fSinCos(&vSinRadian, &vCosRadian, rotationRadian);

    float sinX = Vector4fGetX(vSinRadian);
    float sinY = Vector4fGetY(vSinRadian);
    float sinZ = Vector4fGetZ(vSinRadian);
    float cosX = Vector4fGetX(vCosRadian);
    float cosY = Vector4fGetY(vCosRadian);
    float cosZ = Vector4fGetZ(vCosRadian);

    float32x4x4_t vResult;

    float32x4_t cosYcosYNegativeOne = Vector4fSet(cosY, cosY, -1.f, 0.f);
    float32x4_t cosZsinZsinY = Vector4fSet(cosZ, sinZ, sinY, 0.f);

    vResult.val[0] = vmulq_f32(cosYcosYNegativeOne, cosZsinZsinY);

    float32x4_t sinYsinYOne = Vector4fSet(sinY, sinY, 1.f, 0.f);
    float32x4_t cosZsinZcosY = Vector4fSet(cosZ, sinZ, cosY, 0.f);
    float32x4_t sinYsinYOne_x_cosZsinZcosY = vmulq_f32(sinYsinYOne, cosZsinZcosY);

    float32x4_t sinZcosZ = Vector4fSet(sinZ, cosZ, 0.f, 0.f);
    float32x4_t NegativeOneOne = Vector4fSet(-1.f, 1.f, 0.f, 0.f);
    float32x4_t OneNegativeOne = Vector4fSet(1.f, -1.f, 0.f, 0.f);

    vResult.val[1] = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, sinX),
        vmulq_f32(NegativeOneOne, vmulq_n_f32(sinZcosZ, cosX))
        );

    vResult.val[2] = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, cosX),
        vmulq_f32(OneNegativeOne, vmulq_n_f32(sinZcosZ, sinX))
        );

    vResult.val[3] = matrix.val[3];

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetScaleRotate(float32x4x4_t matrix, float32x4_t scale, float32x4_t rotationQuaternion) NN_NOEXCEPT
{
    float32x4x4_t rotationMatrix = MatrixRowMajor4x3fFromQuaternion(rotationQuaternion);

    float32x4x4_t vResult;
    vResult.val[0] = vmulq_n_f32(rotationMatrix.val[0], Vector4fGetX(scale));
    vResult.val[1] = vmulq_n_f32(rotationMatrix.val[1], Vector4fGetY(scale));
    vResult.val[2] = vmulq_n_f32(rotationMatrix.val[2], Vector4fGetZ(scale));
    vResult.val[3] = matrix.val[3];

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fSetScaleRotateXyz(float32x4x4_t matrix, float32x4_t scale, float32x4_t rotationRadian) NN_NOEXCEPT
{
    float32x4_t vSinRadian, vCosRadian;
    Vector4fSinCos(&vSinRadian, &vCosRadian, rotationRadian);

    float sinX = Vector4fGetX(vSinRadian);
    float sinY = Vector4fGetY(vSinRadian);
    float sinZ = Vector4fGetZ(vSinRadian);
    float cosX = Vector4fGetX(vCosRadian);
    float cosY = Vector4fGetY(vCosRadian);
    float cosZ = Vector4fGetZ(vCosRadian);

    float32x4x4_t vResult;

    float32x4_t cosYcosYNegativeOne = Vector4fSet(cosY, cosY, -1.f, 0.f);
    float32x4_t cosZsinZsinY = Vector4fSet(cosZ, sinZ, sinY, 0.f);

    float32x4_t row0 = vmulq_f32(cosYcosYNegativeOne, cosZsinZsinY);
    vResult.val[0] = vmulq_n_f32(row0, Vector4fGetX(scale));

    float32x4_t sinYsinYOne = Vector4fSet(sinY, sinY, 1.f, 0.f);
    float32x4_t cosZsinZcosY = Vector4fSet(cosZ, sinZ, cosY, 0.f);
    float32x4_t sinYsinYOne_x_cosZsinZcosY = vmulq_f32(sinYsinYOne, cosZsinZcosY);

    float32x4_t sinZcosZ = Vector4fSet(sinZ, cosZ, 0.f, 0.f);
    float32x4_t NegativeOneOne = Vector4fSet(-1.f, 1.f, 0.f, 0.f);
    float32x4_t OneNegativeOne = Vector4fSet(1.f, -1.f, 0.f, 0.f);

    float32x4_t row1 = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, sinX),
        vmulq_f32(NegativeOneOne, vmulq_n_f32(sinZcosZ, cosX))
        );
    vResult.val[1] = vmulq_n_f32(row1, Vector4fGetY(scale));

    float32x4_t row2 = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, cosX),
        vmulq_f32(OneNegativeOne, vmulq_n_f32(sinZcosZ, sinX))
        );
    vResult.val[2] = vmulq_n_f32(row2, Vector4fGetZ(scale));

    vResult.val[3] = matrix.val[3];

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTextureProjectionPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, (2.f * nearZ) * reciprocalWidth * scaleS);
    vResult.val[1] = Vector4fSetY(vZero, (2.f * nearZ) * reciprocalHeight * scaleT);
    vResult.val[2] = Vector4fSet((right + left) * reciprocalWidth * scaleS - translateS, (top + bottom) * reciprocalHeight * scaleT - translateT, -1.f, 0.f);
    vResult.val[3] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTextureProjectionPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float angle = fovyRadian * 0.5f;
    float tangent = nn::util::TanEst(angle);
    float cotangent = 1.f / tangent;

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, cotangent / aspect * scaleS);
    vResult.val[1] = Vector4fSetY(vZero, cotangent * scaleT);
    vResult.val[2] = Vector4fSet(-translateS, -translateT, -1.f, 0.f);
    vResult.val[3] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixRowMajor4x3fTextureProjectionOrthographicOffCenterRightHanded(float left, float right, float bottom, float top) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    float32x4_t vZero = Vector4fZero();

    float32x4x4_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 2.f * reciprocalWidth * scaleS);
    vResult.val[1] = Vector4fSetY(vZero, 2.f * reciprocalHeight * scaleT);
    vResult.val[2] = vZero;
    vResult.val[3] = Vector4fSet(-(right + left) * reciprocalWidth * scaleS + translateS, -(top + bottom) * reciprocalHeight * scaleT + translateT, 1.f, 0.f);

    return vResult;
}

//
// T4x3 行列
//

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fFromQuaternion(float32x4_t quaternion) NN_NOEXCEPT
{
    float32x4_t xyz = Vector4fMask(quaternion, Vector4uValueMaskXYZ);

    float32x4_t yxx = Vector4fSwizzle(xyz, 1, 0, 0, 3);
    float32x4_t y2x2x2 = vmulq_f32(yxx, yxx);
    float32x4_t y2x2x2Double = vaddq_f32(y2x2x2, y2x2x2);

    float32x4_t zzy = Vector4fSwizzle(xyz, 2, 2, 1, 3);
    float32x4_t z2z2y2 = vmulq_f32(zzy, zzy);
    float32x4_t z2z2y2Double = vaddq_f32(z2z2y2, z2z2y2);

    float32x4_t v0 = vsubq_f32(Vector4fValueOneWZero, vaddq_f32(y2x2x2Double, z2z2y2Double));


    float32x4_t yzx = Vector4fSwizzle(xyz, 1, 2, 0, 3);
    float32x4_t xYyZzX = vmulq_f32(xyz, yzx);
    float32x4_t xYyZxXDouble = vaddq_f32(xYyZzX, xYyZzX);

    float32x4_t zxy = Vector4fSwizzle(xyz, 2, 0, 1, 3);
    float32x4_t wZwXwY = vmulq_n_f32(zxy, Vector4fGetW(quaternion));
    float32x4_t wZwXwYDouble = vaddq_f32(wZwXwY, wZwXwY);

    float32x4_t v1 = vsubq_f32(xYyZxXDouble, wZwXwYDouble);
    float32x4_t v2 = vaddq_f32(xYyZxXDouble, wZwXwYDouble);


    float32x4_t m01m02m10m12 = Vector4fPermute(v1, v2, 0, 6, 4, 1);
    float32x4_t m20m21 = Vector4fPermute(v1, v2, 2, 5, 7, 7);


    float32x4x3_t vResult;

    vResult.val[0] = Vector4fPermute(v0, m01m02m10m12, 0, 4, 5, 3);
    vResult.val[1] = Vector4fPermute(v0, m01m02m10m12, 6, 1, 7, 3);
    vResult.val[2] = Vector4fPermute(v0, m20m21, 4, 5, 2, 3);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fConvertFromColumnMajor4x4f(float32x4x4_t source) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = source.val[0];
    vResult.val[1] = source.val[1];
    vResult.val[2] = source.val[2];

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLoad(const FloatRowMajor4x3& source) NN_NOEXCEPT
{
    float32x4_t vRow1 = Vector3fLoad(source.m[0]);
    float32x4_t vRow2 = Vector3fLoad(source.m[1]);
    float32x4_t vRow3 = Vector3fLoad(source.m[2]);
    float32x4_t vRow4 = Vector3fLoad(source.m[3]);

    float32x4x2_t vTmp1 = vzipq_f32(vRow1, vRow2);
    float32x4x2_t vTmp2 = vzipq_f32(vRow3, vRow4);

    float32x4x3_t vResult;
    vResult.val[0] = vcombine_f32(vget_low_f32(vTmp1.val[0]), vget_low_f32(vTmp2.val[0]));
    vResult.val[1] = vcombine_f32(vget_high_f32(vTmp1.val[0]), vget_high_f32(vTmp2.val[0]));
    vResult.val[2] = vcombine_f32(vget_low_f32(vTmp1.val[1]), vget_low_f32(vTmp2.val[1]));

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLoad(const FloatColumnMajor4x3& source) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fLoad(source.m[0]);
    vResult.val[1] = Vector4fLoad(source.m[1]);
    vResult.val[2] = Vector4fLoad(source.m[2]);

    return vResult;
}


NN_FORCEINLINE void MatrixColumnMajor4x3fStore(FloatRowMajor4x3* pOutValue, float32x4x3_t source) NN_NOEXCEPT
{
    float32x4x4_t matrix;
    matrix.val[0] = source.val[0];
    matrix.val[1] = source.val[1];
    matrix.val[2] = source.val[2];
    matrix.val[3] = Vector4fZero();

    float32x4x4_t matrixT = Matrix4x4fTranspose(matrix);

    Vector4fStore(pOutValue->m[0], matrixT.val[0]);
    Vector4fStore(pOutValue->m[1], matrixT.val[1]);
    Vector4fStore(pOutValue->m[2], matrixT.val[2]);

    pOutValue->m[3][0] = vgetq_lane_f32(matrixT.val[3], 0);
    pOutValue->m[3][1] = vgetq_lane_f32(matrixT.val[3], 1);
    pOutValue->m[3][2] = vgetq_lane_f32(matrixT.val[3], 2);
}

NN_FORCEINLINE void MatrixColumnMajor4x3fStore(FloatColumnMajor4x3* pOutValue, float32x4x3_t source) NN_NOEXCEPT
{
    Vector4fStore(pOutValue->m[0], source.val[0]);
    Vector4fStore(pOutValue->m[1], source.val[1]);
    Vector4fStore(pOutValue->m[2], source.val[2]);
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSet(
                            float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSet(m00, m10, m20, m30);
    vResult.val[1] = Vector4fSet(m01, m11, m21, m31);
    vResult.val[2] = Vector4fSet(m02, m12, m22, m32);

    return vResult;
}

NN_FORCEINLINE void MatrixColumnMajor4x3fSetRow(float32x4x3_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetElement(pOutValue->val[0], index, Vector3fGetX(vector));
    pOutValue->val[1] = Vector4fSetElement(pOutValue->val[1], index, Vector3fGetY(vector));
    pOutValue->val[2] = Vector4fSetElement(pOutValue->val[2], index, Vector3fGetZ(vector));
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetRows(const float32x4_t& row0, const float32x4_t& row1, const float32x4_t& row2, const float32x4_t& row3) NN_NOEXCEPT
{
    float32x4x4_t vTmp;
    vTmp.val[0] = row0;
    vTmp.val[1] = row1;
    vTmp.val[2] = row2;
    vTmp.val[3] = row3;

    float32x4x4_t vTranspose = Matrix4x4fTranspose(vTmp);

    float32x4x3_t vResult;
    vResult.val[0] = vTranspose.val[0];
    vResult.val[1] = vTranspose.val[1];
    vResult.val[2] = vTranspose.val[2];

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetRow(const float32x4x3_t& matrix, int index) NN_NOEXCEPT
{
    float x = Vector4fGetElement(matrix.val[0], index);
    float y = Vector4fGetElement(matrix.val[1], index);
    float z = Vector4fGetElement(matrix.val[2], index);

    return Vector3fSet(x, y, z);
}

NN_FORCEINLINE void MatrixColumnMajor4x3fGetRows(float32x4_t* pOutRow0, float32x4_t* pOutRow1, float32x4_t* pOutRow2, float32x4_t* pOutRow3, const float32x4x3_t& matrix) NN_NOEXCEPT
{
    float32x4x4_t vTmp;
    vTmp.val[0] = matrix.val[0];
    vTmp.val[1] = matrix.val[1];
    vTmp.val[2] = matrix.val[2];
    vTmp.val[3] = Vector4fZero();

    float32x4x4_t vTranspose = Matrix4x4fTranspose(vTmp);

    *pOutRow0 = vTranspose.val[0];
    *pOutRow1 = vTranspose.val[1];
    *pOutRow2 = vTranspose.val[2];
    *pOutRow3 = vTranspose.val[3];
}

NN_FORCEINLINE void MatrixColumnMajor4x3fSetColumn(float32x4x3_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[index] = vector;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetColumns(const float32x4_t& column0, const float32x4_t& column1, const float32x4_t& column2) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = column0;
    vResult.val[1] = column1;
    vResult.val[2] = column2;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetColumn(const float32x4x3_t& matrix, int index) NN_NOEXCEPT
{
    return matrix.val[index];
}

NN_FORCEINLINE void MatrixColumnMajor4x3fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, float32x4_t* pOutColumn2, const float32x4x3_t& matrix) NN_NOEXCEPT
{
    *pOutColumn0 = matrix.val[0];
    *pOutColumn1 = matrix.val[1];
    *pOutColumn2 = matrix.val[2];
}

NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisX(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetX(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetX(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetX(pOutValue->val[2], Vector4fGetZ(vector));
}

NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisY(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetY(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetY(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetY(pOutValue->val[2], Vector4fGetZ(vector));
}

NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisZ(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetZ(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetZ(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetZ(pOutValue->val[2], Vector4fGetZ(vector));
}

NN_FORCEINLINE void MatrixColumnMajor4x3fSetAxisW(float32x4x3_t* pOutValue, float32x4_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetW(pOutValue->val[0], Vector4fGetX(vector));
    pOutValue->val[1] = Vector4fSetW(pOutValue->val[1], Vector4fGetY(vector));
    pOutValue->val[2] = Vector4fSetW(pOutValue->val[2], Vector4fGetZ(vector));
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetAxes(float32x4_t axisX, float32x4_t axisY, float32x4_t axisZ, float32x4_t axisW) NN_NOEXCEPT
{
    float32x4x2_t vTmp1 = vzipq_f32(axisX, axisY);
    float32x4x2_t vTmp2 = vzipq_f32(axisZ, axisW);

    float32x4x3_t vResult;
    vResult.val[0] = vcombine_f32(vget_low_f32(vTmp1.val[0]), vget_low_f32(vTmp2.val[0]));
    vResult.val[1] = vcombine_f32(vget_high_f32(vTmp1.val[0]), vget_high_f32(vTmp2.val[0]));
    vResult.val[2] = vcombine_f32(vget_low_f32(vTmp1.val[1]), vget_low_f32(vTmp2.val[1]));

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisX(float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t v = Vector4fZero();
    v = Vector4fSetX(v, Vector4fGetX(matrix.val[0]));
    v = Vector4fSetY(v, Vector4fGetX(matrix.val[1]));
    v = Vector4fSetZ(v, Vector4fGetX(matrix.val[2]));

    return v;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisY(float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t v = Vector4fZero();
    v = Vector4fSetX(v, Vector4fGetY(matrix.val[0]));
    v = Vector4fSetY(v, Vector4fGetY(matrix.val[1]));
    v = Vector4fSetZ(v, Vector4fGetY(matrix.val[2]));

    return v;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisZ(float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t v = Vector4fZero();
    v = Vector4fSetX(v, Vector4fGetZ(matrix.val[0]));
    v = Vector4fSetY(v, Vector4fGetZ(matrix.val[1]));
    v = Vector4fSetZ(v, Vector4fGetZ(matrix.val[2]));

    return v;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor4x3fGetAxisW(float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4_t v = Vector4fZero();
    v = Vector4fSetX(v, Vector4fGetW(matrix.val[0]));
    v = Vector4fSetY(v, Vector4fGetW(matrix.val[1]));
    v = Vector4fSetZ(v, Vector4fGetW(matrix.val[2]));

    return v;
}

NN_FORCEINLINE void MatrixColumnMajor4x3fGetAxes(float32x4_t* pOutAxisX, float32x4_t* pOutAxisY, float32x4_t* pOutAxisZ, float32x4_t* pOutAxisW, float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4x2_t vTmp1 = vzipq_f32(matrix.val[0], matrix.val[1]);
    float32x4x2_t vTmp2 = vzipq_f32(matrix.val[2], Vector4fZero());

    *pOutAxisX = vcombine_f32(vget_low_f32(vTmp1.val[0]), vget_low_f32(vTmp2.val[0]));
    *pOutAxisY = vcombine_f32(vget_high_f32(vTmp1.val[0]), vget_high_f32(vTmp2.val[0]));
    *pOutAxisZ = vcombine_f32(vget_low_f32(vTmp1.val[1]), vget_low_f32(vTmp2.val[1]));
    *pOutAxisW = vcombine_f32(vget_high_f32(vTmp1.val[1]), vget_high_f32(vTmp2.val[1]));
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fZero() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x3_t vResult;
    vResult.val[0] = vZero;
    vResult.val[1] = vZero;
    vResult.val[2] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fIdentity() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 1.f);
    vResult.val[1] = Vector4fSetY(vZero, 1.f);
    vResult.val[2] = Vector4fSetZ(vZero, 1.f);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fAdd(float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fAdd(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fAdd(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fAdd(matrix1.val[2], matrix2.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSubtract(float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSubtract(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fSubtract(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector4fSubtract(matrix1.val[2], matrix2.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fMultiply(float32x4x3_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fMultiply(matrix.val[0], factor);
    vResult.val[1] = Vector4fMultiply(matrix.val[1], factor);
    vResult.val[2] = Vector4fMultiply(matrix.val[2], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fMultiply(float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x3_t vResult;
    float32x4_t vZero = vdupq_n_f32(0);

    vLow = vget_low_f32(matrix2.val[0]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix2.val[1]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix2.val[2]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[2] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x4_t MatrixColumnMajor4x3fMultiplyColumnMajor4x4f(float32x4x3_t matrix1, float32x4x4_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x4_t vResult;
    float32x4_t vZero = vdupq_n_f32(0);

    vLow = vget_low_f32(matrix2.val[0]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[0]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix2.val[1]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[1]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[1] = vTmp;

    vLow = vget_low_f32(matrix2.val[2]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[2]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[2] = vTmp;

    vLow = vget_low_f32(matrix2.val[3]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[3]);
    vTmp = Vector4fMultiplyAddLane0(vTmp, matrix1.val[2], vHigh);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 1), vZero, 3));
    vResult.val[3] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fMultiplyAdd(float t, float32x4x3_t matrix1, float32x4x3_t matrix2) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fMultiplyAdd(matrix2.val[0], matrix1.val[0], t);
    vResult.val[1] = Vector4fMultiplyAdd(matrix2.val[1], matrix1.val[1], t);
    vResult.val[2] = Vector4fMultiplyAdd(matrix2.val[2], matrix1.val[2], t);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fDivide(float32x4x3_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fDivide(matrix.val[0], factor);
    vResult.val[1] = Vector4fDivide(matrix.val[1], factor);
    vResult.val[2] = Vector4fDivide(matrix.val[2], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fInverse(float32x4_t* pDeterminant, float32x4x3_t matrix) NN_NOEXCEPT
{
    float m[3][4];
    Vector4fStore(m[0], matrix.val[0]);
    Vector4fStore(m[1], matrix.val[1]);
    Vector4fStore(m[2], matrix.val[2]);

    float determinant =
        m[0][0] * m[1][1] * m[2][2]
        + m[0][1] * m[1][2] * m[2][0]
        + m[0][2] * m[1][0] * m[2][1]
        - m[2][0] * m[1][1] * m[0][2]
        - m[1][0] * m[0][1] * m[2][2]
        - m[0][0] * m[2][1] * m[1][2];

    *pDeterminant = vdupq_n_f32(determinant);

    float reciprocalDeterminant = determinant == 0.f ? 0.f : 1.f / determinant;

    float result[3][4];

    result[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * reciprocalDeterminant;
    result[0][1] = (m[2][1] * m[0][2] - m[0][1] * m[2][2]) * reciprocalDeterminant;
    result[0][2] = (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * reciprocalDeterminant;

    result[1][0] = (m[2][0] * m[1][2] - m[1][0] * m[2][2]) * reciprocalDeterminant;
    result[1][1] = (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * reciprocalDeterminant;
    result[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * reciprocalDeterminant;

    result[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * reciprocalDeterminant;
    result[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * reciprocalDeterminant;
    result[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * reciprocalDeterminant;

    result[0][3] = -result[0][0] * m[0][3] - result[0][1] * m[1][3] - result[0][2] * m[2][3];
    result[1][3] = -result[1][0] * m[0][3] - result[1][1] * m[1][3] - result[1][2] * m[2][3];
    result[2][3] = -result[2][0] * m[0][3] - result[2][1] * m[1][3] - result[2][2] * m[2][3];

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fLoad(result[0]);
    vResult.val[1] = Vector4fLoad(result[1]);
    vResult.val[2] = Vector4fLoad(result[2]);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTranspose(float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4x2_t vTmp0 = vzipq_f32(matrix.val[0], matrix.val[2]);
    float32x4x2_t vTmp1 = vzipq_f32(matrix.val[1], vdupq_n_f32(0));
    float32x4x2_t vTranspose0 = vzipq_f32(vTmp0.val[0], vTmp1.val[0]);
    float32x4x2_t vTranspose1 = vzipq_f32(vTmp0.val[1], vTmp1.val[1]);

    float32x4x3_t vResult;
    vResult.val[0] = vTranspose0.val[0];
    vResult.val[1] = vTranspose0.val[1];
    vResult.val[2] = vTranspose1.val[0];

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fInverseTranspose(float32x4_t* pDeterminant, float32x4x3_t matrix) NN_NOEXCEPT
{
    float32x4x3_t vResult;

    vResult = MatrixColumnMajor4x3fInverse(pDeterminant, matrix);
    vResult = MatrixColumnMajor4x3fTranspose(vResult);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float32x4_t up) NN_NOEXCEPT
{
    float32x4_t lookVector = Vector4fSubtract(cameraPosition, target);
    Vector4fNormalize(&lookVector, lookVector);

    float32x4_t rightVector = Vector3fCross(up, lookVector);
    Vector4fNormalize(&rightVector, rightVector);

    float32x4_t upVector = Vector3fCross(lookVector, rightVector);

    float32x4_t cameraPositionVectorNegative = Vector4fNegate(cameraPosition);

    float32x4_t dotRight = Vector4fDot(cameraPositionVectorNegative, rightVector);
    float32x4_t dotUp = Vector4fDot(cameraPositionVectorNegative, upVector);
    float32x4_t dotLook = Vector4fDot(cameraPositionVectorNegative, lookVector);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSelect(rightVector, dotRight, Vector4uValueSelect1110);
    vResult.val[1] = Vector4fSelect(upVector, dotUp, Vector4uValueSelect1110);
    vResult.val[2] = Vector4fSelect(lookVector, dotLook, Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fLookAtRightHanded(float32x4_t cameraPosition, float32x4_t target, float twistRadian) NN_NOEXCEPT
{
    float32x4_t vLookReverse = Vector4fSubtract(cameraPosition, target);

    float32x4_t vRightTmp = Vector4fSet(Vector4fGetZ(vLookReverse), 0.f, -Vector4fGetX(vLookReverse), 0.f);

    Vector4fNormalize(&vLookReverse, vLookReverse);
    Vector4fNormalize(&vRightTmp, vRightTmp);

    float32x4_t vUpTmp = Vector3fCross(vLookReverse, vRightTmp);

    float32x4_t vTwistRadian = vdupq_n_f32(twistRadian);
    float32x4_t vSinTwist, vCosTwist;
    Vector4fSinCos(&vSinTwist, &vCosTwist, vTwistRadian);

    float32x4_t vRight = vaddq_f32(vmulq_f32(vUpTmp, vSinTwist), vmulq_f32(vRightTmp, vCosTwist));
    float32x4_t vUp = vsubq_f32(vmulq_f32(vUpTmp, vCosTwist), vmulq_f32(vRightTmp, vSinTwist));

    float32x4_t vCameraPositionVectorNegative = Vector4fNegate(cameraPosition);

    float32x4_t vDotRight = Vector4fDot(vCameraPositionVectorNegative, vRight);
    float32x4_t vDotUp = Vector4fDot(vCameraPositionVectorNegative, vUp);
    float32x4_t vDotLook = Vector4fDot(vCameraPositionVectorNegative, vLookReverse);

    float32x4x3_t vResult1;
    vResult1.val[0] = Vector4fSelect(vRight, vDotRight, Vector4uValueSelect1110);
    vResult1.val[1] = Vector4fSelect(vUp, vDotUp, Vector4uValueSelect1110);
    vResult1.val[2] = Vector4fSelect(vLookReverse, vDotLook, Vector4uValueSelect1110);

    float32x4_t vTranslateSign = { -1.f, 1.f, -1.f, 0.f };
    float32x4_t vSign = Vector4fSetX(Vector4fSign(vdupq_n_f32(Vector4fGetY(vLookReverse))), 1.f);

    float32x4x4_t vResult2;
    vResult2.val[0] = Vector4fValueIdentityAxisX;
    vResult2.val[1] = vmulq_f32(vSign, Vector4fValueIdentityAxisZ);
    vResult2.val[2] = vmulq_f32(vSign, Vector4fValueNegativeIdentityAxisY);
    vResult2.val[3] = vmulq_f32(vSign, vmulq_f32(vTranslateSign, Vector4fSwizzle(cameraPosition, 0, 2, 1, 3)));
    vResult2 = Matrix4x4fTranspose(vResult2);

    float32x4_t vZero = Vector4fZero();
    uint32x4_t vXEqualZero = vceqq_f32(vdupq_n_f32(Vector4fGetX(vLookReverse)), vZero);
    uint32x4_t vZEqualZero = vceqq_f32(vdupq_n_f32(Vector4fGetZ(vLookReverse)), vZero);
    uint32x4_t vXZEqualZero = vandq_u32(vXEqualZero, vZEqualZero);

    float32x4x3_t vResult;
    vResult.val[0] = vbslq_f32(vXZEqualZero, vResult2.val[0], vResult1.val[0]);
    vResult.val[1] = vbslq_f32(vXZEqualZero, vResult2.val[1], vResult1.val[1]);
    vResult.val[2] = vbslq_f32(vXZEqualZero, vResult2.val[2], vResult1.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fCameraRotateZxy(float32x4_t cameraPosition, float32x4_t rotationRadian) NN_NOEXCEPT
{
    float32x4_t vSinRadian, vCosRadian;
    Vector4fSinCos(&vSinRadian, &vCosRadian, rotationRadian);

    float sinX = Vector4fGetX(vSinRadian);
    float sinY = Vector4fGetY(vSinRadian);
    float sinZ = Vector4fGetZ(vSinRadian);
    float cosX = Vector4fGetX(vCosRadian);
    float cosY = Vector4fGetY(vCosRadian);
    float cosZ = Vector4fGetZ(vCosRadian);

    float32x4_t sinXOneSinX = Vector4fSet(sinX, 1.f, sinX, 0.f);
    float32x4_t sinYCosXCosY = Vector4fSet(sinY, cosX, cosY, 0.f);
    float32x4_t sinXOneSinX_x_sinYCosXCosY = vmulq_f32(sinXOneSinX, sinYCosXCosY);
    float32x4_t cosYZeroSinY = Vector4fSet(cosY, 0.f, sinY, 0.f);
    float32x4_t cosZZeroCosZ = Vector4fSet(cosZ, 0.f, cosZ, 0.f);
    float32x4_t sinZZeroSinZ = Vector4fSet(sinZ, 0.f, sinZ, 0.f);
    float32x4_t OneOneNegOne = Vector4fSet(1.f, 1.f, -1.f, 0.f);
    float32x4_t NegOneOneOne = Vector4fSet(-1.f, 1.f, 1.f, 0.f);
    float32x4_t cosXNegOnecosX = Vector4fSet(cosX, -1.f, cosX, 0.f);
    float32x4_t sinYSinXcosY = Vector4fSet(sinY, sinX, cosY, 0.f);


    float32x4_t right = vaddq_f32(
        vmulq_n_f32(
            sinXOneSinX_x_sinYCosXCosY,
            sinZ
        ),
        vmulq_f32(
            OneOneNegOne,
            vmulq_f32(cosYZeroSinY, cosZZeroCosZ)
        )
    );

    float32x4_t up = vaddq_f32(
        vmulq_n_f32(
            sinXOneSinX_x_sinYCosXCosY,
            cosZ
        ),
        vmulq_f32(
            NegOneOneOne,
            vmulq_f32(cosYZeroSinY, sinZZeroSinZ)
        )
    );

    float32x4_t back = vmulq_f32(cosXNegOnecosX, sinYSinXcosY);

    float32x4_t cameraPositionVectorNegative = Vector4fNegate(cameraPosition);

    float32x4_t dotRight = Vector4fDot(cameraPositionVectorNegative, right);
    float32x4_t dotUp = Vector4fDot(cameraPositionVectorNegative, up);
    float32x4_t dotBack = Vector4fDot(cameraPositionVectorNegative, back);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSelect(right, dotRight, Vector4uValueSelect1110);
    vResult.val[1] = Vector4fSelect(up, dotUp, Vector4uValueSelect1110);
    vResult.val[2] = Vector4fSelect(back, dotBack, Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetTranslate(float32x4x3_t matrix, float32x4_t translate) NN_NOEXCEPT
{
    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSetW(matrix.val[0], Vector4fGetX(translate));
    vResult.val[1] = Vector4fSetW(matrix.val[1], Vector4fGetY(translate));
    vResult.val[2] = Vector4fSetW(matrix.val[2], Vector4fGetZ(translate));

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetScale(float32x4x3_t matrix, float32x4_t scale) NN_NOEXCEPT
{
    float32x4_t tmp;
    float32x4x3_t vResult;

    tmp = Vector4fSetX(Vector4fZero(), Vector4fGetX(scale));
    vResult.val[0] = Vector4fSelect(tmp, matrix.val[0], Vector4uValueSelect1110);

    tmp = Vector4fSetY(Vector4fZero(), Vector4fGetY(scale));
    vResult.val[1] = Vector4fSelect(tmp, matrix.val[1], Vector4uValueSelect1110);

    tmp = Vector4fSetZ(Vector4fZero(), Vector4fGetZ(scale));
    vResult.val[2] = Vector4fSelect(tmp, matrix.val[2], Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetRotate(float32x4x3_t matrix, float32x4_t rotationQuaternion) NN_NOEXCEPT
{
    float32x4x3_t rotationMatrix = MatrixColumnMajor4x3fFromQuaternion(rotationQuaternion);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSelect(rotationMatrix.val[0], matrix.val[0], Vector4uValueSelect1110);
    vResult.val[1] = Vector4fSelect(rotationMatrix.val[1], matrix.val[1], Vector4uValueSelect1110);
    vResult.val[2] = Vector4fSelect(rotationMatrix.val[2], matrix.val[2], Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetRotateXyz(float32x4x3_t matrix, float32x4_t rotationRadian) NN_NOEXCEPT
{
    float32x4_t vSinRadian, vCosRadian;
    Vector4fSinCos(&vSinRadian, &vCosRadian, rotationRadian);

    float sinX = Vector4fGetX(vSinRadian);
    float sinY = Vector4fGetY(vSinRadian);
    float sinZ = Vector4fGetZ(vSinRadian);
    float cosX = Vector4fGetX(vCosRadian);
    float cosY = Vector4fGetY(vCosRadian);
    float cosZ = Vector4fGetZ(vCosRadian);

    float32x4x3_t vTmp;

    float32x4_t cosYcosYNegativeOne = Vector4fSet(cosY, cosY, -1.f, 0.f);
    float32x4_t cosZsinZsinY = Vector4fSet(cosZ, sinZ, sinY, 0.f);

    vTmp.val[0] = vmulq_f32(cosYcosYNegativeOne, cosZsinZsinY);

    float32x4_t sinYsinYOne = Vector4fSet(sinY, sinY, 1.f, 0.f);
    float32x4_t cosZsinZcosY = Vector4fSet(cosZ, sinZ, cosY, 0.f);
    float32x4_t sinYsinYOne_x_cosZsinZcosY = vmulq_f32(sinYsinYOne, cosZsinZcosY);

    float32x4_t sinZcosZ = Vector4fSet(sinZ, cosZ, 0.f, 0.f);
    float32x4_t NegativeOneOne = Vector4fSet(-1.f, 1.f, 0.f, 0.f);
    float32x4_t OneNegativeOne = Vector4fSet(1.f, -1.f, 0.f, 0.f);

    vTmp.val[1] = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, sinX),
        vmulq_f32(NegativeOneOne, vmulq_n_f32(sinZcosZ, cosX))
    );

    vTmp.val[2] = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, cosX),
        vmulq_f32(OneNegativeOne, vmulq_n_f32(sinZcosZ, sinX))
    );

    vTmp = MatrixColumnMajor4x3fTranspose(vTmp);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSelect(vTmp.val[0], matrix.val[0], Vector4uValueSelect1110);
    vResult.val[1] = Vector4fSelect(vTmp.val[1], matrix.val[1], Vector4uValueSelect1110);
    vResult.val[2] = Vector4fSelect(vTmp.val[2], matrix.val[2], Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetScaleRotate(float32x4x3_t matrix, float32x4_t scale, float32x4_t rotationQuaternion) NN_NOEXCEPT
{
    float32x4x3_t rotationMatrix = MatrixColumnMajor4x3fFromQuaternion(rotationQuaternion);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSelect(vmulq_f32(rotationMatrix.val[0], scale), matrix.val[0], Vector4uValueSelect1110);
    vResult.val[1] = Vector4fSelect(vmulq_f32(rotationMatrix.val[1], scale), matrix.val[1], Vector4uValueSelect1110);
    vResult.val[2] = Vector4fSelect(vmulq_f32(rotationMatrix.val[2], scale), matrix.val[2], Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fSetScaleRotateXyz(float32x4x3_t matrix, float32x4_t scale, float32x4_t rotationRadian) NN_NOEXCEPT
{
    float32x4_t vSinRadian, vCosRadian;
    Vector4fSinCos(&vSinRadian, &vCosRadian, rotationRadian);

    float sinX = Vector4fGetX(vSinRadian);
    float sinY = Vector4fGetY(vSinRadian);
    float sinZ = Vector4fGetZ(vSinRadian);
    float cosX = Vector4fGetX(vCosRadian);
    float cosY = Vector4fGetY(vCosRadian);
    float cosZ = Vector4fGetZ(vCosRadian);

    float32x4x3_t vTmp;

    float32x4_t cosYcosYNegativeOne = Vector4fSet(cosY, cosY, -1.f, 0.f);
    float32x4_t cosZsinZsinY = Vector4fSet(cosZ, sinZ, sinY, 0.f);

    vTmp.val[0] = vmulq_f32(cosYcosYNegativeOne, cosZsinZsinY);

    float32x4_t sinYsinYOne = Vector4fSet(sinY, sinY, 1.f, 0.f);
    float32x4_t cosZsinZcosY = Vector4fSet(cosZ, sinZ, cosY, 0.f);
    float32x4_t sinYsinYOne_x_cosZsinZcosY = vmulq_f32(sinYsinYOne, cosZsinZcosY);

    float32x4_t sinZcosZ = Vector4fSet(sinZ, cosZ, 0.f, 0.f);
    float32x4_t NegativeOneOne = Vector4fSet(-1.f, 1.f, 0.f, 0.f);
    float32x4_t OneNegativeOne = Vector4fSet(1.f, -1.f, 0.f, 0.f);

    vTmp.val[1] = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, sinX),
        vmulq_f32(NegativeOneOne, vmulq_n_f32(sinZcosZ, cosX))
    );

    vTmp.val[2] = vaddq_f32(
        vmulq_n_f32(sinYsinYOne_x_cosZsinZcosY, cosX),
        vmulq_f32(OneNegativeOne, vmulq_n_f32(sinZcosZ, sinX))
    );

    vTmp = MatrixColumnMajor4x3fTranspose(vTmp);

    vTmp.val[0] = vmulq_f32(scale, vTmp.val[0]);
    vTmp.val[1] = vmulq_f32(scale, vTmp.val[1]);
    vTmp.val[2] = vmulq_f32(scale, vTmp.val[2]);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSelect(vTmp.val[0], matrix.val[0], Vector4uValueSelect1110);
    vResult.val[1] = Vector4fSelect(vTmp.val[1], matrix.val[1], Vector4uValueSelect1110);
    vResult.val[2] = Vector4fSelect(vTmp.val[2], matrix.val[2], Vector4uValueSelect1110);

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTextureProjectionPerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSet((2.f * nearZ) * reciprocalWidth * scaleS, 0.f, (right + left) * reciprocalWidth * scaleS - translateS, 0.f);
    vResult.val[1] = Vector4fSet(0.f, (2.f * nearZ) * reciprocalHeight * scaleT, (top + bottom) * reciprocalHeight * scaleT - translateT, 0.f);
    vResult.val[2] = Vector4fValueNegativeIdentityAxisZ;

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTextureProjectionPerspectiveFieldOfViewRightHanded(float fovyRadian, float aspect) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float angle = fovyRadian * 0.5f;
    float tangent = nn::util::TanEst(angle);
    float cotangent = 1.f / tangent;

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSet(cotangent / aspect * scaleS, 0.f, -translateS, 0.f);
    vResult.val[1] = Vector4fSet(0.f, cotangent * scaleT, -translateT, 0.f);
    vResult.val[2] = Vector4fValueNegativeIdentityAxisZ;

    return vResult;
}

NN_FORCEINLINE float32x4x3_t MatrixColumnMajor4x3fTextureProjectionOrthographicOffCenterRightHanded(float left, float right, float bottom, float top) NN_NOEXCEPT
{
    float scaleS = 0.5f;
    float scaleT = 0.5f;
    float translateS = 0.5f;
    float translateT = 0.5f;

    float reciprocalWidth = 1.f / (right - left);
    float reciprocalHeight = 1.f / (top - bottom);

    float32x4x3_t vResult;
    vResult.val[0] = Vector4fSet(2.f * reciprocalWidth * scaleS, 0.f, 0.f, -(right + left) * reciprocalWidth * scaleS + translateS);
    vResult.val[1] = Vector4fSet(0.f, 2.f * reciprocalHeight * scaleT, 0.f, -(top + bottom) * reciprocalHeight * scaleT + translateT);
    vResult.val[2] = Vector4fValueIdentityAxisW;

    return vResult;
}

//
// 3x2 行列
//
NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fLoad(const FloatRowMajor3x2& source) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fLoad(source.m[0]);
    vResult.val[1] = Vector2fLoad(source.m[1]);
    vResult.val[2] = Vector2fLoad(source.m[2]);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fLoad(const FloatColumnMajor3x2& source) NN_NOEXCEPT
{
    return vld3_f32(&source.m[0][0]);
}

NN_FORCEINLINE void MatrixRowMajor3x2fStore(FloatRowMajor3x2* pOutValue, float32x2x3_t source) NN_NOEXCEPT
{
    Vector2fStore(pOutValue->m[0], source.val[0]);
    Vector2fStore(pOutValue->m[1], source.val[1]);
    Vector2fStore(pOutValue->m[2], source.val[2]);
}

NN_FORCEINLINE void MatrixRowMajor3x2fStore(FloatColumnMajor3x2* pOutValue, float32x2x3_t source) NN_NOEXCEPT
{
    vst3_f32(&pOutValue->m[0][0], source);
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSet(
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fSet(m00, m01);
    vResult.val[1] = Vector2fSet(m10, m11);
    vResult.val[2] = Vector2fSet(m20, m21);

    return vResult;
}

NN_FORCEINLINE void MatrixRowMajor3x2fSetRow(float32x2x3_t* pOutValue, int index, const float32x2_t& vector) NN_NOEXCEPT
{
    pOutValue->val[index] = vector;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetRows(const float32x2_t& row0, const float32x2_t& row1, const float32x2_t& row2) NN_NOEXCEPT
{
    float32x2x3_t vResult;

    vResult.val[0] = row0;
    vResult.val[1] = row1;
    vResult.val[2] = row2;

    return vResult;
}

NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetRow(const float32x2x3_t& matrix, int index) NN_NOEXCEPT
{
    return matrix.val[index];
}

NN_FORCEINLINE void MatrixRowMajor3x2fGetRows(float32x2_t* pOutRow0, float32x2_t* pOutRow1, float32x2_t* pOutRow2, const float32x2x3_t& matrix) NN_NOEXCEPT
{
    *pOutRow0 = matrix.val[0];
    *pOutRow1 = matrix.val[1];
    *pOutRow2 = matrix.val[2];
}

NN_FORCEINLINE void MatrixRowMajor3x2fSetColumn(float32x2x3_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector2fSetElement(pOutValue->val[0], index, Vector3fGetX(vector));
    pOutValue->val[1] = Vector2fSetElement(pOutValue->val[1], index, Vector3fGetY(vector));
    pOutValue->val[2] = Vector2fSetElement(pOutValue->val[2], index, Vector3fGetZ(vector));
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetColumns(const float32x4_t& column0, const float32x4_t& column1) NN_NOEXCEPT
{
    float32x4x2_t vZip = vzipq_f32(column0, column1);

    float32x2x3_t vResult;
    vResult.val[0] = vget_low_f32(vZip.val[0]);
    vResult.val[1] = vget_high_f32(vZip.val[0]);
    vResult.val[2] = vget_low_f32(vZip.val[1]);

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixRowMajor3x2fGetColumn(const float32x2x3_t& matrix, int index) NN_NOEXCEPT
{
    float x = Vector2fGetElement(matrix.val[0], index);
    float y = Vector2fGetElement(matrix.val[1], index);
    float z = Vector2fGetElement(matrix.val[2], index);

    return Vector3fSet(x, y, z);
}

NN_FORCEINLINE void MatrixRowMajor3x2fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, const float32x2x3_t& matrix) NN_NOEXCEPT
{
    float32x4_t vTmp1 = vcombine_f32(matrix.val[0], matrix.val[1]);
    float32x4_t vTmp2 = vcombine_f32(matrix.val[2], Vector2fZero());
    float32x4x2_t vUnzip = vuzpq_f32(vTmp1, vTmp2);

    *pOutColumn0 = vUnzip.val[0];
    *pOutColumn1 = vUnzip.val[1];
}

NN_FORCEINLINE void MatrixRowMajor3x2fSetAxisX(float32x2x3_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = vector;
}

NN_FORCEINLINE void MatrixRowMajor3x2fSetAxisY(float32x2x3_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    pOutValue->val[1] = vector;
}

NN_FORCEINLINE void MatrixRowMajor3x2fSetAxisZ(float32x2x3_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    pOutValue->val[2] = vector;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetAxes(float32x2_t axisX, float32x2_t axisY, float32x2_t axisZ) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = axisX;
    vResult.val[1] = axisY;
    vResult.val[2] = axisZ;

    return vResult;
}

NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetAxisX(float32x2x3_t matrix) NN_NOEXCEPT
{
    return matrix.val[0];
}

NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetAxisY(float32x2x3_t matrix) NN_NOEXCEPT
{
    return matrix.val[1];
}

NN_FORCEINLINE float32x2_t MatrixRowMajor3x2fGetAxisZ(float32x2x3_t matrix) NN_NOEXCEPT
{
    return matrix.val[2];
}

NN_FORCEINLINE void MatrixRowMajor3x2fGetAxes(float32x2_t* pOutAxisX, float32x2_t* pOutAxisY, float32x2_t* pOutAxisZ, float32x2x3_t matrix) NN_NOEXCEPT
{
    *pOutAxisX = matrix.val[0];
    *pOutAxisY = matrix.val[1];
    *pOutAxisZ = matrix.val[2];
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fZero() NN_NOEXCEPT
{
    float32x2_t vZero = Vector2fZero();

    float32x2x3_t vResult;
    vResult.val[0] = vZero;
    vResult.val[1] = vZero;
    vResult.val[2] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fIdentity() NN_NOEXCEPT
{
    float32x2_t vZero = Vector2fZero();

    float32x2x3_t vResult;
    vResult.val[0] = Vector2fSetX(vZero, 1.f);
    vResult.val[1] = Vector2fSetY(vZero, 1.f);
    vResult.val[2] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fAdd(float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fAdd(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector2fAdd(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector2fAdd(matrix1.val[2], matrix2.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSubtract(float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fSubtract(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector2fSubtract(matrix1.val[1], matrix2.val[1]);
    vResult.val[2] = Vector2fSubtract(matrix1.val[2], matrix2.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fMultiply(float32x2x3_t matrix, float factor) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fMultiply(matrix.val[0], factor);
    vResult.val[1] = Vector2fMultiply(matrix.val[1], factor);
    vResult.val[2] = Vector2fMultiply(matrix.val[2], factor);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fMultiply(float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT
{
    float32x2_t vTmp;
    float32x2x3_t vResult;

    vTmp = vmul_lane_f32(matrix2.val[0], matrix1.val[0], 0);
    vTmp = Vector2fMultiplyAddLane1(vTmp, matrix2.val[1], matrix1.val[0]);
    vResult.val[0] = vTmp;

    vTmp = vmul_lane_f32(matrix2.val[0], matrix1.val[1], 0);
    vTmp = Vector2fMultiplyAddLane1(vTmp, matrix2.val[1], matrix1.val[1]);
    vResult.val[1] = vTmp;

    vTmp = vmul_lane_f32(matrix2.val[0], matrix1.val[2], 0);
    vTmp = Vector2fMultiplyAddLane1(vTmp, matrix2.val[1], matrix1.val[2]);
    vTmp = vadd_f32(vTmp, matrix2.val[2]);
    vResult.val[2] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fMultiplyAdd(float t, float32x2x3_t matrix1, float32x2x3_t matrix2) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fMultiplyAdd(matrix2.val[0], matrix1.val[0], t);
    vResult.val[1] = Vector2fMultiplyAdd(matrix2.val[1], matrix1.val[1], t);
    vResult.val[2] = Vector2fMultiplyAdd(matrix2.val[2], matrix1.val[2], t);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fDivide(float32x2x3_t matrix, float factor) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fDivide(matrix.val[0], factor);
    vResult.val[1] = Vector2fDivide(matrix.val[1], factor);
    vResult.val[2] = Vector2fDivide(matrix.val[2], factor);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fInverse(float32x4_t* pDeterminant, float32x2x3_t matrix) NN_NOEXCEPT
{
    float32x2_t vDeterminant = Vector2fCross(matrix.val[0], matrix.val[1]);
    *pDeterminant = vcombine_f32(vDeterminant, vDeterminant);

    float32x2_t vReciprocalDeterminant = Vector2fRecp(vDeterminant);

    float32x2x3_t vResult;
    float32x2_t vTmp;

    float32x2x2_t vZip = vzip_f32(matrix.val[1], matrix.val[0]);

    vTmp = vmul_f32(vZip.val[1], vReciprocalDeterminant);
    vTmp = vmul_f32(vTmp, Vector2fValueNegateY);
    vResult.val[0] = vTmp;

    vTmp = vmul_f32(vZip.val[0], vReciprocalDeterminant);
    vTmp = vmul_f32(vTmp, Vector2fValueNegateX);
    vResult.val[1] = vTmp;

    vTmp = vmul_lane_f32(vneg_f32(vResult.val[0]), matrix.val[2], 0);
    vTmp = Vector2fMultiplySubtractLane1(vTmp, vResult.val[1], matrix.val[2]);
    vResult.val[2] = vTmp;

    float32x2_t vZero = Vector2fZero();
    uint32x2_t vDeterminantEqualZero = vceq_f32(vDeterminant, vZero);

    vResult.val[0] = vbsl_f32(vDeterminantEqualZero, vZero, vResult.val[0]);
    vResult.val[1] = vbsl_f32(vDeterminantEqualZero, vZero, vResult.val[1]);
    vResult.val[2] = vbsl_f32(vDeterminantEqualZero, vZero, vResult.val[2]);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fTranspose(float32x2x3_t matrix) NN_NOEXCEPT
{
    float32x2x2_t vTrans = vtrn_f32(matrix.val[0], matrix.val[1]);

    float32x2x3_t vResult;
    vResult.val[0] = vTrans.val[0];
    vResult.val[1] = vTrans.val[1];
    vResult.val[2] = Vector2fZero();

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fInverseTranspose(float32x4_t* pDeterminant, float32x2x3_t matrix) NN_NOEXCEPT
{
    float32x2x3_t vResult;

    vResult = MatrixRowMajor3x2fInverse(pDeterminant, matrix);
    vResult = MatrixRowMajor3x2fTranspose(vResult);

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetTranslate(float32x2x3_t matrix, float32x2_t translate) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = matrix.val[0];
    vResult.val[1] = matrix.val[1];
    vResult.val[2] = translate;

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetScale(float32x2x3_t matrix, float32x2_t scale) NN_NOEXCEPT
{
    float32x2x3_t vResult;
    vResult.val[0] = Vector2fSetY(scale, 0.f);
    vResult.val[1] = Vector2fSetX(scale, 0.f);
    vResult.val[2] = matrix.val[2];

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetRotate(float32x2x3_t matrix, float rotationRadian) NN_NOEXCEPT
{
    float32x2_t vSinRadian, vCosRadian;
    Vector2fSinCos(&vSinRadian, &vCosRadian, vdup_n_f32(rotationRadian));

    float32x2x2_t vTrans = vtrn_f32(vCosRadian, vSinRadian);

    float32x2x3_t vResult;
    vResult.val[0] = vTrans.val[0];
    vResult.val[1] = vmul_f32(vrev64_f32(vTrans.val[1]), Vector2fValueNegateX);
    vResult.val[2] = matrix.val[2];

    return vResult;
}

NN_FORCEINLINE float32x2x3_t MatrixRowMajor3x2fSetScaleRotate(float32x2x3_t matrix, float32x2_t scale, float rotationRadian) NN_NOEXCEPT
{
    float32x2_t vSinRadian, vCosRadian;
    Vector2fSinCos(&vSinRadian, &vCosRadian, vdup_n_f32(rotationRadian));

    float32x2_t vScaleSinRadian = vmul_f32(scale, vSinRadian);
    float32x2_t vScaleCosRadian = vmul_f32(scale, vCosRadian);

    float32x2x2_t vTrans = vtrn_f32(vScaleCosRadian, vScaleSinRadian);

    float32x2x3_t vResult;
    vResult.val[0] = vTrans.val[0];
    vResult.val[1] = vmul_f32(vrev64_f32(vTrans.val[1]), Vector2fValueNegateX);
    vResult.val[2] = matrix.val[2];

    return vResult;
}

//
// T3x2 行列
//
NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fLoad(const FloatRowMajor3x2& source) NN_NOEXCEPT
{
    float32x2x3_t vSource = MatrixRowMajor3x2fLoad(source);

    float32x4_t vTmp1 = vcombine_f32(vSource.val[0], vSource.val[1]);
    float32x4_t vTmp2 = vcombine_f32(vSource.val[2], Vector2fZero());

    return vuzpq_f32(vTmp1, vTmp2);
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fLoad(const FloatColumnMajor3x2& source) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector3fLoad(source.m[0]);
    vResult.val[1] = Vector3fLoad(source.m[1]);

    return vResult;
}

NN_FORCEINLINE void MatrixColumnMajor3x2fStore(FloatRowMajor3x2* pOutValue, float32x4x2_t source) NN_NOEXCEPT
{
    float32x4x2_t vZip = vzipq_f32(source.val[0], source.val[1]);

    Vector2fStore(pOutValue->m[0], vget_low_f32(vZip.val[0]));
    Vector2fStore(pOutValue->m[1], vget_high_f32(vZip.val[0]));
    Vector2fStore(pOutValue->m[2], vget_low_f32(vZip.val[1]));
}

NN_FORCEINLINE void MatrixColumnMajor3x2fStore(FloatColumnMajor3x2* pOutValue, float32x4x2_t source) NN_NOEXCEPT
{
    Vector3fStore(pOutValue->m[0], source.val[0]);
    Vector3fStore(pOutValue->m[1], source.val[1]);
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSet(
    float m00, float m01,
    float m10, float m11,
    float m20, float m21) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector3fSet(m00, m10, m20);
    vResult.val[1] = Vector3fSet(m01, m11, m21);

    return vResult;
}

NN_FORCEINLINE void MatrixColumnMajor3x2fSetRow(float32x4x2_t* pOutValue, int index, const float32x2_t& vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector3fSetElement(pOutValue->val[0], index, Vector2fGetX(vector));
    pOutValue->val[1] = Vector3fSetElement(pOutValue->val[1], index, Vector2fGetY(vector));
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetRows(const float32x2_t& row0, const float32x2_t& row1, const float32x2_t& row2) NN_NOEXCEPT
{
    float32x4_t vTmp1 = vcombine_f32(row0, row1);
    float32x4_t vTmp2 = vcombine_f32(row2, Vector2fZero());

    return vuzpq_f32(vTmp1, vTmp2);
}

NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetRow(const float32x4x2_t& matrix, int index) NN_NOEXCEPT
{
    float x = Vector3fGetElement(matrix.val[0], index);
    float y = Vector3fGetElement(matrix.val[1], index);

    return Vector2fSet(x, y);
}

NN_FORCEINLINE void MatrixColumnMajor3x2fGetRows(float32x2_t* pOutRow0, float32x2_t* pOutRow1, float32x2_t* pOutRow2, const float32x4x2_t& matrix) NN_NOEXCEPT
{
    float32x4x2_t vZip = vzipq_f32(matrix.val[0], matrix.val[1]);

    *pOutRow0 = vget_low_f32(vZip.val[0]);
    *pOutRow1 = vget_high_f32(vZip.val[0]);
    *pOutRow2 = vget_low_f32(vZip.val[1]);
}

NN_FORCEINLINE void MatrixColumnMajor3x2fSetColumn(float32x4x2_t* pOutValue, int index, const float32x4_t& vector) NN_NOEXCEPT
{
    pOutValue->val[index] = vector;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetColumns(const float32x4_t& column0, const float32x4_t& column1) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = column0;
    vResult.val[1] = column1;

    return vResult;
}

NN_FORCEINLINE float32x4_t MatrixColumnMajor3x2fGetColumn(const float32x4x2_t& matrix, int index) NN_NOEXCEPT
{
    return matrix.val[index];
}

NN_FORCEINLINE void MatrixColumnMajor3x2fGetColumns(float32x4_t* pOutColumn0, float32x4_t* pOutColumn1, const float32x4x2_t& matrix) NN_NOEXCEPT
{
    *pOutColumn0 = matrix.val[0];
    *pOutColumn1 = matrix.val[1];
}

NN_FORCEINLINE void MatrixColumnMajor3x2fSetAxisX(float32x4x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetX(pOutValue->val[0], Vector2fGetX(vector));
    pOutValue->val[1] = Vector4fSetX(pOutValue->val[1], Vector2fGetY(vector));
}

NN_FORCEINLINE void MatrixColumnMajor3x2fSetAxisY(float32x4x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetY(pOutValue->val[0], Vector2fGetX(vector));
    pOutValue->val[1] = Vector4fSetY(pOutValue->val[1], Vector2fGetY(vector));
}

NN_FORCEINLINE void MatrixColumnMajor3x2fSetAxisZ(float32x4x2_t* pOutValue, float32x2_t vector) NN_NOEXCEPT
{
    pOutValue->val[0] = Vector4fSetZ(pOutValue->val[0], Vector2fGetX(vector));
    pOutValue->val[1] = Vector4fSetZ(pOutValue->val[1], Vector2fGetY(vector));
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetAxes(float32x2_t axisX, float32x2_t axisY, float32x2_t axisZ) NN_NOEXCEPT
{
    float32x4_t vTmp1 = vcombine_f32(axisX, axisY);
    float32x4_t vTmp2 = vcombine_f32(axisZ, Vector2fZero());

    return vuzpq_f32(vTmp1, vTmp2);
}

NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetAxisX(float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x2x2_t vTrans = vtrn_f32(vget_low_f32(matrix.val[0]), vget_low_f32(matrix.val[1]));

    return vTrans.val[0];
}

NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetAxisY(float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x2x2_t vTrans = vtrn_f32(vget_low_f32(matrix.val[0]), vget_low_f32(matrix.val[1]));

    return vTrans.val[1];
}

NN_FORCEINLINE float32x2_t MatrixColumnMajor3x2fGetAxisZ(float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x2x2_t vTrans = vtrn_f32(vget_high_f32(matrix.val[0]), vget_high_f32(matrix.val[1]));

    return vTrans.val[0];
}

NN_FORCEINLINE void MatrixColumnMajor3x2fGetAxes(float32x2_t* pOutAxisX, float32x2_t* pOutAxisY, float32x2_t* pOutAxisZ, float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x4x2_t vZip = vzipq_f32(matrix.val[0], matrix.val[1]);

    *pOutAxisX = vget_low_f32(vZip.val[0]);
    *pOutAxisY = vget_high_f32(vZip.val[0]);
    *pOutAxisZ = vget_low_f32(vZip.val[1]);
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fZero() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x2_t vResult;
    vResult.val[0] = vZero;
    vResult.val[1] = vZero;

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fIdentity() NN_NOEXCEPT
{
    float32x4_t vZero = Vector4fZero();

    float32x4x2_t vResult;
    vResult.val[0] = Vector4fSetX(vZero, 1.f);
    vResult.val[1] = Vector4fSetY(vZero, 1.f);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fAdd(float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector4fAdd(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fAdd(matrix1.val[1], matrix2.val[1]);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSubtract(float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector4fSubtract(matrix1.val[0], matrix2.val[0]);
    vResult.val[1] = Vector4fSubtract(matrix1.val[1], matrix2.val[1]);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fMultiply(float32x4x2_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector4fMultiply(matrix.val[0], factor);
    vResult.val[1] = Vector4fMultiply(matrix.val[1], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fMultiply(float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT
{
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x2_t vResult;
    float32x4_t vZero = vdupq_n_f32(0);

    vLow = vget_low_f32(matrix2.val[0]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[0]);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 0), vZero, 2));
    vResult.val[0] = vTmp;

    vLow = vget_low_f32(matrix2.val[1]);
    vTmp = vmulq_lane_f32(matrix1.val[0], vLow, 0);
    vTmp = Vector4fMultiplyAddLane1(vTmp, matrix1.val[1], vLow);
    vHigh = vget_high_f32(matrix2.val[1]);
    vTmp = vaddq_f32(vTmp, vsetq_lane_f32(vget_lane_f32(vHigh, 0), vZero, 2));
    vResult.val[1] = vTmp;

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fMultiplyAdd(float t, float32x4x2_t matrix1, float32x4x2_t matrix2) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector4fMultiplyAdd(matrix2.val[0], matrix1.val[0], t);
    vResult.val[1] = Vector4fMultiplyAdd(matrix2.val[1], matrix1.val[1], t);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fDivide(float32x4x2_t matrix, float factor) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector4fDivide(matrix.val[0], factor);
    vResult.val[1] = Vector4fDivide(matrix.val[1], factor);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fInverse(float32x4_t* pDeterminant, float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x4x2_t vZip = vzipq_f32(matrix.val[0], matrix.val[1]);
    float32x2_t vX = vget_low_f32(vZip.val[0]);
    float32x2_t vY = vget_high_f32(vZip.val[0]);
    float32x2_t vZ = vget_low_f32(vZip.val[1]);

    float32x2_t vDeterminant = Vector2fCross(vX, vY);
    float32x4_t vDeterminant4 = vcombine_f32(vDeterminant, vDeterminant);
    *pDeterminant = vDeterminant4;

    float32x2_t vReciprocalDeterminant = Vector2fRecp(vDeterminant);

    float32x4x2_t vResult;

    float32x2_t vRow0Low = vmul_f32(vrev64_f32(vY), vReciprocalDeterminant);
    vRow0Low = vmul_f32(vRow0Low, Vector2fValueNegateY);

    float32x2_t vRow1Low = vmul_f32(vrev64_f32(vX), vReciprocalDeterminant);
    vRow1Low = vmul_f32(vRow1Low, Vector2fValueNegateX);

    float32x2_t vRow0High = vmul_f32(vRow0Low, vZ);
    float32x2_t vRow1High = vmul_f32(vRow1Low, vZ);
    float32x2_t vRow01High = vneg_f32(vpadd_f32(vRow0High, vRow1High));

    vResult.val[0] = Vector4fSetW(vcombine_f32(vRow0Low, vRow01High), 0.f);
    vResult.val[1] = Vector4fSetW(vcombine_f32(vRow1Low, vrev64_f32(vRow01High)), 0.f);

    float32x4_t vZero = Vector4fZero();
    uint32x4_t vDeterminantEqualZero = vceqq_f32(vDeterminant4, vZero);

    vResult.val[0] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[0]);
    vResult.val[1] = vbslq_f32(vDeterminantEqualZero, vZero, vResult.val[1]);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fTranspose(float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x2x2_t vTrans = vtrn_f32(vget_low_f32(matrix.val[0]), vget_low_f32(matrix.val[1]));
    float32x2_t vZero = Vector2fZero();

    float32x4x2_t vResult;
    vResult.val[0] = vcombine_f32(vTrans.val[0], vZero);
    vResult.val[1] = vcombine_f32(vTrans.val[1], vZero);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fInverseTranspose(float32x4_t* pDeterminant, float32x4x2_t matrix) NN_NOEXCEPT
{
    float32x4x2_t vResult;

    vResult = MatrixColumnMajor3x2fInverse(pDeterminant, matrix);
    vResult = MatrixColumnMajor3x2fTranspose(vResult);

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetTranslate(float32x4x2_t matrix, float32x2_t translate) NN_NOEXCEPT
{
    float32x4x2_t vResult;
    vResult.val[0] = Vector4fSetZ(matrix.val[0], Vector2fGetX(translate));
    vResult.val[1] = Vector4fSetZ(matrix.val[1], Vector2fGetY(translate));

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetScale(float32x4x2_t matrix, float32x2_t scale) NN_NOEXCEPT
{
    float32x2x2_t vTrans = vtrn_f32(scale, Vector2fZero());

    float32x4x2_t vResult;
    vResult.val[0] = vcombine_f32(vTrans.val[0], vget_high_f32(matrix.val[0]));
    vResult.val[1] = vcombine_f32(vrev64_f32(vTrans.val[1]), vget_high_f32(matrix.val[1]));

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetRotate(float32x4x2_t matrix, float rotationRadian) NN_NOEXCEPT
{
    float32x2_t vSinRadian, vCosRadian;
    Vector2fSinCos(&vSinRadian, &vCosRadian, vdup_n_f32(rotationRadian));

    float32x2x2_t vTrans = vtrn_f32(vCosRadian, vSinRadian);
    float32x2_t vRow0Low = vmul_f32(vTrans.val[0], Vector2fValueNegateY);
    float32x2_t vRow1Low = vrev64_f32(vTrans.val[1]);

    float32x4x2_t vResult;
    vResult.val[0] = vcombine_f32(vRow0Low, vget_high_f32(matrix.val[0]));
    vResult.val[1] = vcombine_f32(vRow1Low, vget_high_f32(matrix.val[1]));

    return vResult;
}

NN_FORCEINLINE float32x4x2_t MatrixColumnMajor3x2fSetScaleRotate(float32x4x2_t matrix, float32x2_t scale, float rotationRadian) NN_NOEXCEPT
{
    float32x2_t vSinRadian, vCosRadian;
    Vector2fSinCos(&vSinRadian, &vCosRadian, vdup_n_f32(rotationRadian));

    float32x2x2_t vTrans = vtrn_f32(vCosRadian, vSinRadian);
    float32x2_t vRow0Low = vmul_f32(scale, vmul_f32(vTrans.val[0], Vector2fValueNegateY));
    float32x2_t vRow1Low = vmul_f32(scale, vrev64_f32(vTrans.val[1]));

    float32x4x2_t vResult;
    vResult.val[0] = vcombine_f32(vRow0Low, vget_high_f32(matrix.val[0]));
    vResult.val[1] = vcombine_f32(vRow1Low, vget_high_f32(matrix.val[1]));

    return vResult;
}

}}}} // namespace nn::util::neon::detail
