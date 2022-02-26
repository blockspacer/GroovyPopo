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

#include <nn/util/util_Matrix.h>
#include <nn/util/util_Vector.h>
#include <nn/util/util_Quaternion.h>
#include <nn/util/util_Endian.h>

namespace nn { namespace g3d { namespace detail {

inline void MatrixExtractScaleBase(nn::util::Vector3fType* pVectorScale, const nn::util::Matrix4x3fType& mtx)
{
    nn::util::Vector3fType axisX;
    nn::util::Vector3fType axisY;
    nn::util::Vector3fType axisZ;
    MatrixGetAxisX(&axisX, mtx);
    MatrixGetAxisY(&axisY, mtx);
    MatrixGetAxisZ(&axisZ, mtx);
    VectorSet(pVectorScale,
              VectorLength(axisX),
              VectorLength(axisY),
              VectorLength(axisZ));
}

inline void MatrixScaleBase(nn::util::Matrix4x3fType* pDstMtx, const nn::util::Matrix4x3fType& srcMtx, const nn::util::Vector3fType& vectorScale)
{
    nn::util::Vector3fType axisX;
    nn::util::Vector3fType axisY;
    nn::util::Vector3fType axisZ;
    nn::util::Vector3fType axisW;

    MatrixGetAxisX(&axisX, srcMtx);
    MatrixGetAxisY(&axisY, srcMtx);
    MatrixGetAxisZ(&axisZ, srcMtx);
    MatrixGetAxisW(&axisW, srcMtx);
    VectorMultiply(&axisX, axisX, VectorGetX(vectorScale));
    VectorMultiply(&axisY, axisY, VectorGetY(vectorScale));
    VectorMultiply(&axisZ, axisZ, VectorGetZ(vectorScale));
    MatrixSetAxisX(pDstMtx, axisX);
    MatrixSetAxisY(pDstMtx, axisY);
    MatrixSetAxisZ(pDstMtx, axisZ);
    MatrixSetAxisW(pDstMtx, axisW);
}

inline void MatrixSetColumn0(nn::util::Matrix4x3fType* pDstMtx, const nn::util::Vector3fType& srcVector)
{
    nn::util::Float4x3 mtx;
    MatrixStore(&mtx, *pDstMtx);
    mtx.m00 = VectorGetX(srcVector);
    mtx.m10 = VectorGetY(srcVector);
    mtx.m20 = VectorGetZ(srcVector);
    MatrixLoad(pDstMtx, mtx);
}

inline void MatrixSetColumn1(nn::util::Matrix4x3fType* pDstMtx, const nn::util::Vector3fType& srcVector)
{
    nn::util::Float4x3 mtx;
    MatrixStore(&mtx, *pDstMtx);
    mtx.m01 = VectorGetX(srcVector);
    mtx.m11 = VectorGetY(srcVector);
    mtx.m21 = VectorGetZ(srcVector);
    MatrixLoad(pDstMtx, mtx);
}

inline void MatrixSetColumn2(nn::util::Matrix4x3fType* pDstMtx, const nn::util::Vector3fType& srcVector)
{
    nn::util::Float4x3 mtx;
    MatrixStore(&mtx, *pDstMtx);
    mtx.m02 = VectorGetX(srcVector);
    mtx.m12 = VectorGetY(srcVector);
    mtx.m22 = VectorGetZ(srcVector);
    MatrixLoad(pDstMtx, mtx);
}

inline void MatrixSwapEndian(nn::util::FloatColumnMajor4x3* pDstMtx, const nn::util::FloatColumnMajor4x3& srcMtx)
{
    for (int row = 0 ; row < 3 ; ++row)
    {
        for (int column = 0 ; column < 4 ; ++column)
        {
            nn::util::StoreEndianReversed(&pDstMtx->m[row][column], srcMtx.m[row][column]);
        }
    }
}

inline void VectorQuaternionRotateXyz(nn::util::Float3* pFloat3, nn::util::Vector4fType& q, const nn::util::Float3& v)
{
    nn::util::Vector4fType r;
    VectorSet(&r,
              v.v[2] * VectorGetY(q) + VectorGetW(q) * v.v[0] - v.v[1] * VectorGetZ(q),
              v.v[0] * VectorGetZ(q) + VectorGetW(q) * v.v[1] - v.v[2] * VectorGetX(q),
              v.v[1] * VectorGetX(q) + VectorGetW(q) * v.v[2] - v.v[0] * VectorGetY(q),
              v.v[0] * VectorGetX(q) + VectorGetY(q) * v.v[1] + v.v[2] * VectorGetZ(q));

    pFloat3->v[0] = VectorGetY(q) * VectorGetZ(r) - VectorGetZ(q) * VectorGetY(r) +
                    VectorGetW(r) * VectorGetX(q) + VectorGetW(q) * VectorGetX(r);
    pFloat3->v[1] = VectorGetZ(q) * VectorGetX(r) - VectorGetX(q) * VectorGetZ(r) +
                    VectorGetW(r) * VectorGetY(q) + VectorGetW(q) * VectorGetY(r);
    pFloat3->v[2] = VectorGetX(q) * VectorGetY(r) - VectorGetY(q) * VectorGetX(r) +
                    VectorGetW(r) * VectorGetZ(q) + VectorGetW(q) * VectorGetZ(r);
    /*
    r.x =  v.z * q.y + q.w * v.x -v.y * q.z;
    r.y =  v.x * q.z + q.w * v.y -v.z * q.x;
    r.z =  v.y * q.x + q.w * v.z -v.x * q.y;
    r.w = v.x * q.x + v.y * q.y + v.z * q.z;

    this->x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    this->y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    this->z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;
    */
}

}}}
