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
#include <nn/util/util_Config.h>
#include <nn/util/util_MathTypes.h>

/**
* @file
* @brief    行列計算を行う API を定義します。
*/

/**
* @brief    nn::util::FloatRowMajor3x2 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatRowMajor3x2 を初期化できます。
*           @code
*               nn::util::FloatRowMajor3x2 matrix = NN_UTIL_FLOAT_ROW_MAJOR_3X2_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);
*           @endcode
*
* @relates  nn::util::FloatRowMajor3x2
*/
#define NN_UTIL_FLOAT_ROW_MAJOR_3X2_INITIALIZER(m00, m01, m10, m11, m20, m21) \
    { \
        { \
            { \
                { m00, m01 }, \
                { m10, m11 }, \
                { m20, m21 }, \
            } \
        } \
    }

/**
* @brief    nn::util::FloatColumnMajor3x2 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatColumnMajor3x2 を初期化できます。
*           @code
*               nn::util::FloatColumnMajor3x2 matrix = NN_UTIL_FLOAT_COLUMN_MAJOR_3X2_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);
*           @endcode
*
* @relates  nn::util::FloatColumnMajor3x2
*/
#define NN_UTIL_FLOAT_COLUMN_MAJOR_3X2_INITIALIZER(m00, m01, m10, m11, m20, m21) \
    { \
        { \
            { \
                { m00, m10, m20 }, \
                { m01, m11, m21 }, \
            } \
        } \
    }

/**
* @brief    nn::util::FloatRowMajor4x3 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatRowMajor4x3 を初期化できます。
*           @code
*               nn::util::FloatRowMajor4x3 matrix = NN_UTIL_FLOAT_ROW_MAJOR_4X3_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::FloatRowMajor4x3
*/
#define NN_UTIL_FLOAT_ROW_MAJOR_4X3_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) \
    { \
        { \
            { \
                { m00, m01, m02 }, \
                { m10, m11, m12 }, \
                { m20, m21, m22 }, \
                { m30, m31, m32 }  \
            } \
        } \
    }

/**
* @brief    nn::util::FloatColumnMajor4x3 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatColumnMajor4x3 を初期化できます。
*           @code
*               nn::util::FloatColumnMajor4x3 matrix = NN_UTIL_FLOAT_COLUMN_MAJOR_4X3_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::FloatColumnMajor4x3
*/
#define NN_UTIL_FLOAT_COLUMN_MAJOR_4X3_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) \
    { \
        { \
            { \
                { m00, m10, m20, m30 }, \
                { m01, m11, m21, m31 }, \
                { m02, m12, m22, m32 }  \
            } \
        } \
    }

/**
* @brief    nn::util::FloatRowMajor4x4 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatRowMajor4x4 を初期化できます。
*           @code
*               nn::util::FloatRowMajor4x4 matrix = NN_UTIL_FLOAT_ROW_MAJOR_4X4_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::FloatRowMajor4x4
*/
#define NN_UTIL_FLOAT_ROW_MAJOR_4X4_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
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

/**
* @brief    nn::util::FloatColumnMajor4x4 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatColumnMajor4x4 を初期化できます。
*           @code
*               nn::util::FloatColumnMajor4x4 matrix = NN_UTIL_FLOAT_COLUMN_MAJOR_4X4_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::FloatColumnMajor4x4
*/
#define NN_UTIL_FLOAT_COLUMN_MAJOR_4X4_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
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

/**
* @brief    nn::util::Float3x2 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Float3x2 を初期化できます。
*           @code
*               nn::util::Float3x2 matrix = NN_UTIL_FLOAT_3X2_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);
*           @endcode
*
* @relates  nn::util::FloatRowMajor3x2
*/
#define NN_UTIL_FLOAT_3X2_INITIALIZER     NN_UTIL_FLOAT_ROW_MAJOR_3X2_INITIALIZER

/**
* @brief    nn::util::FloatT3x2 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatT3x2 を初期化できます。
*           @code
*               nn::util::FloatT3x2 matrix = NN_UTIL_FLOAT_T3X2_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);
*           @endcode
*
* @relates  nn::util::FloatColumnMajor3x2
*/
#define NN_UTIL_FLOAT_T3X2_INITIALIZER    NN_UTIL_FLOAT_COLUMN_MAJOR_3X2_INITIALIZER

/**
* @brief    nn::util::Float4x3 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Float4x3 を初期化できます。
*           @code
*               nn::util::Float4x3 matrix = NN_UTIL_FLOAT_4X3_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::FloatRowMajor4x3
*/
#define NN_UTIL_FLOAT_4X3_INITIALIZER     NN_UTIL_FLOAT_ROW_MAJOR_4X3_INITIALIZER

/**
* @brief    nn::util::FloatT4x3 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatT4x3 を初期化できます。
*           @code
*               nn::util::FloatT4x3 matrix = NN_UTIL_FLOAT_T4X3_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::FloatColumnMajor4x3
*/
#define NN_UTIL_FLOAT_T4X3_INITIALIZER    NN_UTIL_FLOAT_COLUMN_MAJOR_4X3_INITIALIZER

/**
* @brief    nn::util::Float4x4 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Float4x4 を初期化できます。
*           @code
*               nn::util::Float4x4 matrix = NN_UTIL_FLOAT_4X4_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::FloatRowMajor4x4
*/
#define NN_UTIL_FLOAT_4X4_INITIALIZER     NN_UTIL_FLOAT_ROW_MAJOR_4X4_INITIALIZER

/**
* @brief    nn::util::FloatT4x4 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::FloatT4x4 を初期化できます。
*           @code
*               nn::util::FloatT4x4 matrix = NN_UTIL_FLOAT_T4X4_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::FloatColumnMajor4x4
*/
#define NN_UTIL_FLOAT_T4X4_INITIALIZER    NN_UTIL_FLOAT_COLUMN_MAJOR_4X4_INITIALIZER

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

/**
* @brief    nn::util::MatrixRowMajor3x2fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixRowMajor3x2fType を初期化できます。
*           @code
*               nn::util::MatrixRowMajor3x2fType matrix = NN_UTIL_MATRIX_ROW_MAJOR_3x2F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);
*           @endcode
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
#define NN_UTIL_MATRIX_ROW_MAJOR_3x2F_INITIALIZER(m00, m01, m10, m11, m20, m21)

/**
* @brief    nn::util::MatrixColumnMajor3x2fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixColumnMajor3x2fType を初期化できます。
*           @code
*               nn::util::MatrixColumnMajor3x2fType matrix = NN_UTIL_MATRIX_COLUMN_MAJOR_3x2F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);
*           @endcode
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
#define NN_UTIL_MATRIX_COLUMN_MAJOR_3x2F_INITIALIZER(m00, m01, m10, m11, m20, m21)

/**
* @brief    nn::util::MatrixRowMajor4x3fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixRowMajor4x3fType を初期化できます。
*           @code
*               nn::util::MatrixRowMajor4x3fType matrix = NN_UTIL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
#define NN_UTIL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32)

/**
* @brief    nn::util::MatrixColumnMajor4x3fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixColumnMajor4x3fType を初期化できます。
*           @code
*               nn::util::MatrixColumnMajor4x3fType matrix = NN_UTIL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
#define NN_UTIL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32)

/**
* @brief    nn::util::MatrixRowMajor4x4fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixRowMajor4x4fType を初期化できます。
*           @code
*               nn::util::MatrixRowMajor4x4fType matrix = NN_UTIL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
#define NN_UTIL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)

/**
* @brief    nn::util::MatrixColumnMajor4x4fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixColumnMajor4x4fType を初期化できます。
*           @code
*               nn::util::MatrixColumnMajor4x4fType matrix = NN_UTIL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
#define NN_UTIL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)

/**
* @brief    nn::util::Matrix3x2fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Matrix3x2fType を初期化できます。
*           @code
*               nn::util::Matrix3x2fType matrix = NN_UTIL_MATRIX_3X2F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
#define NN_UTIL_MATRIX_3X2F_INITIALIZER     NN_UTIL_MATRIX_ROW_MAJOR_3X2F_INITIALIZER

/**
* @brief    nn::util::MatrixT3x2fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixT3x2fType を初期化できます。
*           @code
*               nn::util::MatrixT3x2fType matrix = NN_UTIL_MATRIX_T3X2F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
#define NN_UTIL_MATRIX_T3X2F_INITIALIZER    NN_UTIL_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER

/**
* @brief    nn::util::Matrix4x3fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Matrix4x3fType を初期化できます。
*           @code
*               nn::util::Matrix4x3fType matrix = NN_UTIL_MATRIX_4X3F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
#define NN_UTIL_MATRIX_4X3F_INITIALIZER     NN_UTIL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER

/**
* @brief    nn::util::MatrixT4x3fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixT4x3fType を初期化できます。
*           @code
*               nn::util::MatrixT4x3fType matrix = NN_UTIL_MATRIX_T4X3F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f);
*           @endcode
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
#define NN_UTIL_MATRIX_T4X3F_INITIALIZER    NN_UTIL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER

/**
* @brief    nn::util::Matrix4x4fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Matrix4x4fType を初期化できます。
*           @code
*               nn::util::Matrix4x4fType matrix = NN_UTIL_MATRIX_4X4F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
#define NN_UTIL_MATRIX_4X4F_INITIALIZER     NN_UTIL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER

/**
* @brief    nn::util::MatrixT4x4fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::MatrixT4x4fType を初期化できます。
*           @code
*               nn::util::MatrixT4x4fType matrix = NN_UTIL_MATRIX_T4X4F_INITIALIZER(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
*           @endcode
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
#define NN_UTIL_MATRIX_T4X4F_INITIALIZER    NN_UTIL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER

namespace nn { namespace util {

//! @name 3 行 2 列の行列
//! @{

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  要素の並びを列優先から行優先へ並び替えて読み込みます。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  要素の並びを行優先から列優先へ並び替えて読み込みます。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatRowMajor3x2& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor3x2fType* pOutValue, const FloatColumnMajor3x2& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  要素の並びを列優先から行優先へ並び替えて書き出します。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixStore(FloatRowMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  要素の並びを行優先から列優先へ並び替えて書き出します。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixRowMajor3x2fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor3x2* pOutValue, const MatrixColumnMajor3x2fType& source) NN_NOEXCEPT;

/**
* @brief    行列の各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    m00         (0, 0) 成分
* @param[in]    m01         (0, 1) 成分
* @param[in]    m10         (1, 0) 成分
* @param[in]    m11         (1, 1) 成分
* @param[in]    m20         (2, 0) 成分
* @param[in]    m21         (2, 1) 成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor3x2fType* pOutValue, float m00, float m01, float m10, float m11, float m20, float m21) NN_NOEXCEPT;

/**
* @brief    行列の各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    m00         (0, 0) 成分
* @param[in]    m01         (0, 1) 成分
* @param[in]    m10         (1, 0) 成分
* @param[in]    m11         (1, 1) 成分
* @param[in]    m20         (2, 0) 成分
* @param[in]    m21         (2, 1) 成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor3x2fType* pOutValue, float m00, float m01, float m10, float m11, float m20, float m21) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の行番号
* @param[in]    vector      設定対象の行成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の行番号
* @param[in]    vector      設定対象の行成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    row0        第 0 行成分
* @param[in]    row1        第 1 行成分
* @param[in]    row2        第 2 行成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    row0        第 0 行成分
* @param[in]    row1        第 1 行成分
* @param[in]    row2        第 2 行成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& row0, const Vector2fType& row1, const Vector2fType& row2) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を取得します。
*
* @param[out]   pOutValue   取得した行成分の格納先
* @param[in]    matrix      取得対象の行列
* @param[in]    index       取得対象の行番号
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetRow(Vector2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を取得します。
*
* @param[out]   pOutValue   取得した行成分の格納先
* @param[in]    matrix      取得対象の行列
* @param[in]    index       取得対象の行番号
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetRow(Vector2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を取得します。
*
* @param[out]   pOutRow0    取得した第 0 行成分の格納先
* @param[out]   pOutRow1    取得した第 1 行成分の格納先
* @param[out]   pOutRow2    取得した第 2 行成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を取得します。
*
* @param[out]   pOutRow0    取得した第 0 行成分の格納先
* @param[out]   pOutRow1    取得した第 1 行成分の格納先
* @param[out]   pOutRow2    取得した第 2 行成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetRows(Vector2fType* pOutRow0, Vector2fType* pOutRow1, Vector2fType* pOutRow2, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      設定対象の列成分
* @param[in]    index       設定対象の列番号
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      設定対象の列成分
* @param[in]    index       設定対象の列番号
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor3x2fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    column0     第 0 列成分
* @param[in]    column1     第 1 列成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    column0     第 0 列成分
* @param[in]    column1     第 1 列成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor3x2fType* pOutValue, const Vector3fType& column0, const Vector3fType& column1) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を取得します。
*
* @param[out]   pOutValue   取得した列成分の格納先
* @param[in]    matrix      取得対象の列列
* @param[in]    index       取得対象の列番号
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetColumn(Vector3fType* pOutValue, const MatrixRowMajor3x2fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を取得します。
*
* @param[out]   pOutValue   取得した列成分の格納先
* @param[in]    matrix      取得対象の列列
* @param[in]    index       取得対象の列番号
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetColumn(Vector3fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を取得します。
*
* @param[out]   pOutColumn0 取得した第 0 列成分の格納先
* @param[out]   pOutColumn1 取得した第 1 列成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を取得します。
*
* @param[out]   pOutColumn0 取得した第 0 列成分の格納先
* @param[out]   pOutColumn1 取得した第 1 列成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetColumns(Vector3fType* pOutColumn0, Vector3fType* pOutColumn1, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      X 軸成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      X 軸成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Y 軸成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Y 軸成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Z 軸成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Z 軸成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    axisX       X 軸成分
* @param[in]    axisY       Y 軸成分
* @param[in]    axisZ       Z 軸成分
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    axisX       X 軸成分
* @param[in]    axisY       Y 軸成分
* @param[in]    axisZ       Z 軸成分
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& axisX, const Vector2fType& axisY, const Vector2fType& axisZ) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を取得します。
*
* @param[out]   pOutValue   取得した X 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を取得します。
*
* @param[out]   pOutValue   取得した X 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxisX(Vector2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Y 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Y 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxisY(Vector2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxisZ(Vector2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を取得します。
*
* @param[out]   pOutAxisX   取得した X 軸成分の格納先
* @param[out]   pOutAxisY   取得した Y 軸成分の格納先
* @param[out]   pOutAxisZ   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を取得します。
*
* @param[out]   pOutAxisX   取得した X 軸成分の格納先
* @param[out]   pOutAxisY   取得した Y 軸成分の格納先
* @param[out]   pOutAxisZ   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixGetAxes(Vector2fType* pOutAxisX, Vector2fType* pOutAxisY, Vector2fType* pOutAxisZ, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列をゼロ行列に設定します。
*
* @param[out]   pOutValue   ゼロ行列の格納先
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列をゼロ行列に設定します。
*
* @param[out]   pOutValue   ゼロ行列の格納先
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列を単位行列に設定します。
*
* @param[out]   pOutValue   単位行列の格納先
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor3x2fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列を単位行列に設定します。
*
* @param[out]   pOutValue   単位行列の格納先
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor3x2fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列の和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @details  matrix1 - matrix2 を計算します。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @details  matrix1 - matrix2 を計算します。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    第 1 行列をスカラー倍し、第 2 行列を加えた結果を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    t           スカラー値
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor3x2fType* pOutValue, float t, const MatrixRowMajor3x2fType& matrix1, const MatrixRowMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    第 1 行列をスカラー倍し、第 2 行列を加えた結果を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    t           スカラー値
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor3x2fType* pOutValue, float t, const MatrixColumnMajor3x2fType& matrix1, const MatrixColumnMajor3x2fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    逆行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆行列が見つかったかどうかを返します。
* @retval   true    逆行列が見つかりました。
* @retval   false   逆行列が見つかりませんでした。
*
* @details  逆行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆行列が見つかったかどうかを返します。
* @retval   true    逆行列が見つかりました。
* @retval   false   逆行列が見つかりませんでした。
*
* @details  逆行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆転置行列が見つかったかどうかを返します。
* @retval   true    逆転置行列が見つかりました。
* @retval   false   逆転置行列が見つかりませんでした。
*
* @details  逆転置行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor3x2fType* pOutValue, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆転置行列が見つかったかどうかを返します。
* @retval   true    逆転置行列が見つかりました。
* @retval   false   逆転置行列が見つかりませんでした。
*
* @details  逆転置行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor3x2fType* pOutValue, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の平行移動成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    translate   平行移動量
*
* @details  平行移動成分以外の成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT;

/**
* @brief    行列の平行移動成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    translate   平行移動量
*
* @details  平行移動成分以外の成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& translate) NN_NOEXCEPT;

/**
* @brief    行列のスケール成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    scale       スケール
*
* @details  無回転のスケール行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT;

/**
* @brief    行列のスケール成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    scale       スケール
*
* @details  無回転のスケール行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale) NN_NOEXCEPT;

/**
* @brief    行列の回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  スケールなしの回転行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列の回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  スケールなしの回転行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor3x2fType* pOutValue, float rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列のスケール・回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    scale           スケール
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  平行移動成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列のスケール・回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    scale           スケール
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  平行移動成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor3x2fType
*/
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor3x2fType* pOutValue, const Vector2fType& scale, float rotationRadian) NN_NOEXCEPT;

//! @}

//! @name 4 行 3 列の行列
//! @{

/**
* @brief    指定のクォータニオンに対応する回転行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
*
* @details  与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    指定のクォータニオンに対応する回転行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
*
* @details  与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixFromQuaternion(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  要素の並びを列優先から行優先へ並び替えて読み込みます。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  要素の並びを行優先から列優先へ並び替えて読み込みます。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatRowMajor4x3& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x3fType* pOutValue, const FloatColumnMajor4x3& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  要素の並びを列優先から行優先へ並び替えて書き出します。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  要素の並びを行優先から列優先へ並び替えて書き出します。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x3* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT;

/**
* @brief    行列を他の形式の行列へ変換します。
*
* @param[out]   pOutValue   変換先
* @param[in]    source      変換元
*
* @details  変換先である 4x4 行列の W 軸成分は、 (0, 0, 0, 1) で埋めます。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& source) NN_NOEXCEPT;

/**
* @brief    行列を他の形式の行列へ変換します。
*
* @param[out]   pOutValue   変換先
* @param[in]    source      変換元
*
* @details  変換先である 4x4 行列の W 軸成分は、 (0, 0, 0, 1) で埋めます。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& source) NN_NOEXCEPT;

/**
* @brief    行列の各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    m00         (0, 0) 成分
* @param[in]    m01         (0, 1) 成分
* @param[in]    m02         (0, 2) 成分
* @param[in]    m10         (1, 0) 成分
* @param[in]    m11         (1, 1) 成分
* @param[in]    m12         (1, 2) 成分
* @param[in]    m20         (2, 0) 成分
* @param[in]    m21         (2, 1) 成分
* @param[in]    m22         (2, 2) 成分
* @param[in]    m30         (3, 0) 成分
* @param[in]    m31         (3, 1) 成分
* @param[in]    m32         (3, 2) 成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x3fType* pOutValue, float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32) NN_NOEXCEPT;

/**
* @brief    行列の各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    m00         (0, 0) 成分
* @param[in]    m01         (0, 1) 成分
* @param[in]    m02         (0, 2) 成分
* @param[in]    m10         (1, 0) 成分
* @param[in]    m11         (1, 1) 成分
* @param[in]    m12         (1, 2) 成分
* @param[in]    m20         (2, 0) 成分
* @param[in]    m21         (2, 1) 成分
* @param[in]    m22         (2, 2) 成分
* @param[in]    m30         (3, 0) 成分
* @param[in]    m31         (3, 1) 成分
* @param[in]    m32         (3, 2) 成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x3fType* pOutValue, float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の行番号
* @param[in]    vector      設定対象の行成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の行番号
* @param[in]    vector      設定対象の行成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    row0        第 0 行成分
* @param[in]    row1        第 1 行成分
* @param[in]    row2        第 2 行成分
* @param[in]    row3        第 3 行成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    row0        第 0 行成分
* @param[in]    row1        第 1 行成分
* @param[in]    row2        第 2 行成分
* @param[in]    row3        第 3 行成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& row0, const Vector3fType& row1, const Vector3fType& row2, const Vector3fType& row3) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を取得します。
*
* @param[out]   pOutValue   取得した行成分の格納先
* @param[in]    matrix      取得対象の行列
* @param[in]    index       取得対象の行番号
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetRow(Vector3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を取得します。
*
* @param[out]   pOutValue   取得した行成分の格納先
* @param[in]    matrix      取得対象の行列
* @param[in]    index       取得対象の行番号
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetRow(Vector3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を取得します。
*
* @param[out]   pOutRow0    取得した第 0 行成分の格納先
* @param[out]   pOutRow1    取得した第 1 行成分の格納先
* @param[out]   pOutRow2    取得した第 2 行成分の格納先
* @param[out]   pOutRow3    取得した第 3 行成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を取得します。
*
* @param[out]   pOutRow0    取得した第 0 行成分の格納先
* @param[out]   pOutRow1    取得した第 1 行成分の格納先
* @param[out]   pOutRow2    取得した第 2 行成分の格納先
* @param[out]   pOutRow3    取得した第 3 行成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetRows(Vector3fType* pOutRow0, Vector3fType* pOutRow1, Vector3fType* pOutRow2, Vector3fType* pOutRow3, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の列番号
* @param[in]    vector      設定対象の列成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の列番号
* @param[in]    vector      設定対象の列成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x3fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    column0     第 0 列成分
* @param[in]    column1     第 1 列成分
* @param[in]    column2     第 2 列成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    column0     第 0 列成分
* @param[in]    column1     第 1 列成分
* @param[in]    column2     第 2 列成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を取得します。
*
* @param[out]   pOutValue   取得した列成分の格納先
* @param[in]    matrix      取得対象の列列
* @param[in]    index       取得対象の列番号
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType* pOutValue, const MatrixRowMajor4x3fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を取得します。
*
* @param[out]   pOutValue   取得した列成分の格納先
* @param[in]    matrix      取得対象の列列
* @param[in]    index       取得対象の列番号
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を取得します。
*
* @param[out]   pOutColumn0 取得した第 0 列成分の格納先
* @param[out]   pOutColumn1 取得した第 1 列成分の格納先
* @param[out]   pOutColumn2 取得した第 2 列成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を取得します。
*
* @param[out]   pOutColumn0 取得した第 0 列成分の格納先
* @param[out]   pOutColumn1 取得した第 1 列成分の格納先
* @param[out]   pOutColumn2 取得した第 2 列成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      X 軸成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      X 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Y 軸成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Y 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Z 軸成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Z 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      W 軸成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      W 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    axisX       X 軸成分
* @param[in]    axisY       Y 軸成分
* @param[in]    axisZ       Z 軸成分
* @param[in]    axisW       W 軸成分
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    axisX       X 軸成分
* @param[in]    axisY       Y 軸成分
* @param[in]    axisZ       Z 軸成分
* @param[in]    axisW       W 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& axisX, const Vector3fType& axisY, const Vector3fType& axisZ, const Vector3fType& axisW) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を取得します。
*
* @param[out]   pOutValue   取得した X 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を取得します。
*
* @param[out]   pOutValue   取得した X 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisX(Vector3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Y 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Y 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisY(Vector3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisZ(Vector3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を取得します。
*
* @param[out]   pOutValue   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を取得します。
*
* @param[out]   pOutValue   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxisW(Vector3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を取得します。
*
* @param[out]   pOutAxisX   取得した X 軸成分の格納先
* @param[out]   pOutAxisY   取得した Y 軸成分の格納先
* @param[out]   pOutAxisZ   取得した Z 軸成分の格納先
* @param[out]   pOutAxisW   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を取得します。
*
* @param[out]   pOutAxisX   取得した X 軸成分の格納先
* @param[out]   pOutAxisY   取得した Y 軸成分の格納先
* @param[out]   pOutAxisZ   取得した Z 軸成分の格納先
* @param[out]   pOutAxisW   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixGetAxes(Vector3fType* pOutAxisX, Vector3fType* pOutAxisY, Vector3fType* pOutAxisZ, Vector3fType* pOutAxisW, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列をゼロ行列に設定します。
*
* @param[out]   pOutValue   ゼロ行列の格納先
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列をゼロ行列に設定します。
*
* @param[out]   pOutValue   ゼロ行列の格納先
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列を単位行列に設定します。
*
* @param[out]   pOutValue   単位行列の格納先
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x3fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列を単位行列に設定します。
*
* @param[out]   pOutValue   単位行列の格納先
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x3fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列の和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @details  matrix1 - matrix2 を計算します。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @details  matrix1 - matrix2 を計算します。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    第 1 行列をスカラー倍し、第 2 行列を加えた結果を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    t           スカラー値
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixRowMajor4x3fType* pOutValue, float t, const MatrixRowMajor4x3fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    第 1 行列をスカラー倍し、第 2 行列を加えた結果を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    t           スカラー値
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixMultiplyAdd(MatrixColumnMajor4x3fType* pOutValue, float t, const MatrixColumnMajor4x3fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    逆行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆行列が見つかったかどうかを返します。
* @retval   true    逆行列が見つかりました。
* @retval   false   逆行列が見つかりませんでした。
*
* @details  逆行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆行列が見つかったかどうかを返します。
* @retval   true    逆行列が見つかりました。
* @retval   false   逆行列が見つかりませんでした。
*
* @details  逆行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆転置行列が見つかったかどうかを返します。
* @retval   true    逆転置行列が見つかりました。
* @retval   false   逆転置行列が見つかりませんでした。
*
* @details  逆転置行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆転置行列が見つかったかどうかを返します。
* @retval   true    逆転置行列が見つかりました。
* @retval   false   逆転置行列が見つかりませんでした。
*
* @details  逆転置行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE bool       MatrixInverseTranspose(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    カメラ位置、上方向、および注視点を用いて、右手座標系におけるビュー行列を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    cameraPosition  カメラ位置
* @param[in]    target          注視点
* @param[in]    up              上方向
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT;

/**
* @brief    カメラ位置、上方向、および注視点を用いて、右手座標系におけるビュー行列を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    cameraPosition  カメラ位置
* @param[in]    target          注視点
* @param[in]    up              上方向
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, const Vector3fType& up) NN_NOEXCEPT;

/**
* @brief    カメラ位置、ツイスト、および注視点を用いて、右手座標系におけるビュー行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    cameraPosition  カメラ位置
* @param[in]    target          注視点
* @param[in]    twistRadian     ツイスト（ラジアン表現）
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT;

/**
* @brief    カメラ位置、ツイスト、および注視点を用いて、右手座標系におけるビュー行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    cameraPosition  カメラ位置
* @param[in]    target          注視点
* @param[in]    twistRadian     ツイスト（ラジアン表現）
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixLookAtRightHanded(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& target, float twistRadian) NN_NOEXCEPT;

/**
* @brief    カメラ位置と各軸の回転角度を用いて、ビュー行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    cameraPosition  カメラ位置
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT;

/**
* @brief    カメラ位置と各軸の回転角度を用いて、ビュー行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    cameraPosition  カメラ位置
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixCameraRotateZxy(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& cameraPosition, const Vector3fType& rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列の平行移動成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    translate   平行移動量
*
* @details  平行移動成分以外の成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetTranslate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT;

/**
* @brief    行列の平行移動成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    translate   平行移動量
*
* @details  平行移動成分以外の成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetTranslate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& translate) NN_NOEXCEPT;

/**
* @brief    行列のスケール成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    scale       スケール
*
* @details  無回転のスケール行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetScale(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT;

/**
* @brief    行列のスケール成分を設定します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    scale       スケール
*
* @details  無回転のスケール行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetScale(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale) NN_NOEXCEPT;

/**
* @brief    行列の回転成分を設定します。回転をクォータニオンで指定します。
*
* @param[out]   pOutValue           結果の格納先
* @param[in]    rotationQuaternion  回転
*
* @details  スケールなしの回転行列を設定します。平行移動成分は操作しません。
*           また、与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRotate(MatrixRowMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;

/**
* @brief    行列の回転成分を設定します。回転をクォータニオンで指定します。
*
* @param[out]   pOutValue           結果の格納先
* @param[in]    rotationQuaternion  回転
*
* @details  スケールなしの回転行列を設定します。平行移動成分は操作しません。
*           また、与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;

/**
* @brief    行列の回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  スケールなしの回転行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列の回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  スケールなしの回転行列を設定します。平行移動成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列のスケール・回転成分を設定します。回転をクォータニオンで指定します。
*
* @param[out]   pOutValue           結果の格納先
* @param[in]    scale               スケール
* @param[in]    rotationQuaternion  回転
*
* @details  平行移動成分は操作しません。
*           また、与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;

/**
* @brief    行列のスケール・回転成分を設定します。回転をクォータニオンで指定します。
*
* @param[out]   pOutValue           結果の格納先
* @param[in]    scale               スケール
* @param[in]    rotationQuaternion  回転
*
* @details  平行移動成分は操作しません。
*           また、与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetScaleRotate(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector4fType& rotationQuaternion) NN_NOEXCEPT;

/**
* @brief    行列のスケール・回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    scale           スケール
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  平行移動成分は操作しません。
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixRowMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT;

/**
* @brief    行列のスケール・回転成分を設定します。回転を各軸の回転角度で指定します。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    scale           スケール
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @details  平行移動成分は操作しません。
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixSetScaleRotateXyz(MatrixColumnMajor4x3fType* pOutValue, const Vector3fType& scale, const Vector3fType& rotationRadian) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系におけるテクスチャ用の透視投影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
* @param[in]    nearZ       近平面までの距離
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系におけるテクスチャ用の透視投影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
* @param[in]    nearZ       近平面までの距離
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top, float nearZ) NN_NOEXCEPT;

/**
* @brief    視野角度を用いて、右手座標系におけるテクスチャ用の透視投影変換行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    fovyRadian  垂直視野角度（ラジアン表現）
* @param[in]    aspect      アスペクト比
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT;

/**
* @brief    視野角度を用いて、右手座標系におけるテクスチャ用の透視投影変換行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    fovyRadian  垂直視野角度（ラジアン表現）
* @param[in]    aspect      アスペクト比
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTextureProjectionPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x3fType* pOutValue, float fovyRadian, float aspect) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系におけるテクスチャ用の正射影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
*
* @relates  nn::util::MatrixRowMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixRowMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系におけるテクスチャ用の正射影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
*
* @relates  nn::util::MatrixColumnMajor4x3fType
*/
NN_FORCEINLINE void       MatrixTextureProjectionOrthographicOffCenterRightHanded(MatrixColumnMajor4x3fType* pOutValue, float left, float right, float bottom, float top) NN_NOEXCEPT;

//! @}

//! @name 4 行 4 列の行列
//! @{

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixRowMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatRowMajor4x4& source) NN_NOEXCEPT;

/**
* @brief    行列へ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixLoad(MatrixColumnMajor4x4fType* pOutValue, const FloatColumnMajor4x4& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixStore(FloatRowMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT;

/**
* @brief    行列から値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixStore(FloatColumnMajor4x4* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT;

/**
* @brief    行列を他の形式の行列へ変換します。
*
* @param[out]   pOutValue   変換先
* @param[in]    source      変換元
*
* @details  変換元である 4x4 行列の W 軸成分は、切り捨てます。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixConvert(MatrixRowMajor4x3fType* pOutValue, const MatrixRowMajor4x4fType& source) NN_NOEXCEPT;

/**
* @brief    行列を他の形式の行列へ変換します。
*
* @param[out]   pOutValue   変換先
* @param[in]    source      変換元
*
* @details  変換元である 4x4 行列の W 軸成分は、切り捨てます。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixConvert(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x4fType& source) NN_NOEXCEPT;

/**
* @brief    行列の各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    m00         (0, 0) 成分
* @param[in]    m01         (0, 1) 成分
* @param[in]    m02         (0, 2) 成分
* @param[in]    m03         (0, 3) 成分
* @param[in]    m10         (1, 0) 成分
* @param[in]    m11         (1, 1) 成分
* @param[in]    m12         (1, 2) 成分
* @param[in]    m13         (1, 3) 成分
* @param[in]    m20         (2, 0) 成分
* @param[in]    m21         (2, 1) 成分
* @param[in]    m22         (2, 2) 成分
* @param[in]    m23         (2, 3) 成分
* @param[in]    m30         (3, 0) 成分
* @param[in]    m31         (3, 1) 成分
* @param[in]    m32         (3, 2) 成分
* @param[in]    m33         (3, 3) 成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSet(MatrixRowMajor4x4fType* pOutValue, float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) NN_NOEXCEPT;

/**
* @brief    行列の各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    m00         (0, 0) 成分
* @param[in]    m01         (0, 1) 成分
* @param[in]    m02         (0, 2) 成分
* @param[in]    m03         (0, 3) 成分
* @param[in]    m10         (1, 0) 成分
* @param[in]    m11         (1, 1) 成分
* @param[in]    m12         (1, 2) 成分
* @param[in]    m13         (1, 3) 成分
* @param[in]    m20         (2, 0) 成分
* @param[in]    m21         (2, 1) 成分
* @param[in]    m22         (2, 2) 成分
* @param[in]    m23         (2, 3) 成分
* @param[in]    m30         (3, 0) 成分
* @param[in]    m31         (3, 1) 成分
* @param[in]    m32         (3, 2) 成分
* @param[in]    m33         (3, 3) 成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSet(MatrixColumnMajor4x4fType* pOutValue, float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の行番号
* @param[in]    vector      設定対象の行成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetRow(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の行番号
* @param[in]    vector      設定対象の行成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetRow(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    row0        第 0 行成分
* @param[in]    row1        第 1 行成分
* @param[in]    row2        第 2 行成分
* @param[in]    row3        第 3 行成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetRows(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    row0        第 0 行成分
* @param[in]    row1        第 1 行成分
* @param[in]    row2        第 2 行成分
* @param[in]    row3        第 3 行成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetRows(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& row0, const Vector4fType& row1, const Vector4fType& row2, const Vector4fType& row3) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を取得します。
*
* @param[out]   pOutValue   取得した行成分の格納先
* @param[in]    matrix      取得対象の行列
* @param[in]    index       取得対象の行番号
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetRow(Vector4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の行成分を取得します。
*
* @param[out]   pOutValue   取得した行成分の格納先
* @param[in]    matrix      取得対象の行列
* @param[in]    index       取得対象の行番号
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetRow(Vector4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を取得します。
*
* @param[out]   pOutRow0    取得した第 0 行成分の格納先
* @param[out]   pOutRow1    取得した第 1 行成分の格納先
* @param[out]   pOutRow2    取得した第 2 行成分の格納先
* @param[out]   pOutRow3    取得した第 3 行成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各行成分を取得します。
*
* @param[out]   pOutRow0    取得した第 0 行成分の格納先
* @param[out]   pOutRow1    取得した第 1 行成分の格納先
* @param[out]   pOutRow2    取得した第 2 行成分の格納先
* @param[out]   pOutRow3    取得した第 3 行成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetRows(Vector4fType* pOutRow0, Vector4fType* pOutRow1, Vector4fType* pOutRow2, Vector4fType* pOutRow3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の列番号
* @param[in]    vector      設定対象の列成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetColumn(MatrixRowMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    index       設定対象の列番号
* @param[in]    vector      設定対象の列成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetColumn(MatrixColumnMajor4x4fType* pOutValue, int index, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    column0     第 0 列成分
* @param[in]    column1     第 1 列成分
* @param[in]    column2     第 2 列成分
* @param[in]    column3     第 3 列成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetColumns(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    column0     第 0 列成分
* @param[in]    column1     第 1 列成分
* @param[in]    column2     第 2 列成分
* @param[in]    column3     第 3 列成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetColumns(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& column0, const Vector4fType& column1, const Vector4fType& column2, const Vector4fType& column3) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を取得します。
*
* @param[out]   pOutValue   取得した列成分の格納先
* @param[in]    matrix      取得対象の列列
* @param[in]    index       取得対象の列番号
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の指定された番号の列成分を取得します。
*
* @param[out]   pOutValue   取得した列成分の格納先
* @param[in]    matrix      取得対象の列列
* @param[in]    index       取得対象の列番号
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetColumn(Vector4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, int index) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を取得します。
*
* @param[out]   pOutColumn0    取得した第 0 列成分の格納先
* @param[out]   pOutColumn1    取得した第 1 列成分の格納先
* @param[out]   pOutColumn2    取得した第 2 列成分の格納先
* @param[out]   pOutColumn3    取得した第 3 列成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各列成分を取得します。
*
* @param[out]   pOutColumn0    取得した第 0 列成分の格納先
* @param[out]   pOutColumn1    取得した第 1 列成分の格納先
* @param[out]   pOutColumn2    取得した第 2 列成分の格納先
* @param[out]   pOutColumn3    取得した第 3 列成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetColumns(Vector4fType* pOutColumn0, Vector4fType* pOutColumn1, Vector4fType* pOutColumn2, Vector4fType* pOutColumn3, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;


/**
* @brief    行列の X 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      X 軸成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisX(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      X 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisX(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Y 軸成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisY(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Y 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisY(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Z 軸成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      Z 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisZ(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      W 軸成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisW(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    vector      W 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxisW(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    axisX       X 軸成分
* @param[in]    axisY       Y 軸成分
* @param[in]    axisZ       Z 軸成分
* @param[in]    axisW       W 軸成分
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxes(MatrixRowMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    axisX       X 軸成分
* @param[in]    axisY       Y 軸成分
* @param[in]    axisZ       Z 軸成分
* @param[in]    axisW       W 軸成分
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSetAxes(MatrixColumnMajor4x4fType* pOutValue, const Vector4fType& axisX, const Vector4fType& axisY, const Vector4fType& axisZ, const Vector4fType& axisW) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を取得します。
*
* @param[out]   pOutValue   取得した X 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の X 軸成分を取得します。
*
* @param[out]   pOutValue   取得した X 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisX(Vector4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Y 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Y 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Y 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisY(Vector4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の Z 軸成分を取得します。
*
* @param[out]   pOutValue   取得した Z 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisZ(Vector4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を取得します。
*
* @param[out]   pOutValue   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の W 軸成分を取得します。
*
* @param[out]   pOutValue   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxisW(Vector4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を取得します。
*
* @param[out]   pOutAxisX   取得した X 軸成分の格納先
* @param[out]   pOutAxisY   取得した Y 軸成分の格納先
* @param[out]   pOutAxisZ   取得した Z 軸成分の格納先
* @param[out]   pOutAxisW   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列の各軸成分を取得します。
*
* @param[out]   pOutAxisX   取得した X 軸成分の格納先
* @param[out]   pOutAxisY   取得した Y 軸成分の格納先
* @param[out]   pOutAxisZ   取得した Z 軸成分の格納先
* @param[out]   pOutAxisW   取得した W 軸成分の格納先
* @param[in]    matrix      取得対象の行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixGetAxes(Vector4fType* pOutAxisX, Vector4fType* pOutAxisY, Vector4fType* pOutAxisZ, Vector4fType* pOutAxisW, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列をゼロ行列に設定します。
*
* @param[out]   pOutValue   ゼロ行列の格納先
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixZero(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列をゼロ行列に設定します。
*
* @param[out]   pOutValue   ゼロ行列の格納先
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixZero(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列を単位行列に設定します。
*
* @param[out]   pOutValue   単位行列の格納先
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixIdentity(MatrixRowMajor4x4fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列を単位行列に設定します。
*
* @param[out]   pOutValue   単位行列の格納先
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixIdentity(MatrixColumnMajor4x4fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    行列の和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixAdd(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixAdd(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @details  matrix1 - matrix2 を計算します。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSubtract(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 ベクトル
* @param[in]    matrix2     第 2 ベクトル
*
* @details  matrix1 - matrix2 を計算します。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixSubtract(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x3fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix1, const MatrixRowMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix1     第 1 行列
* @param[in]    matrix2     第 2 行列
*
* @details  matrix1 に対し matrix2 を右から掛けます。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixMultiply(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix1, const MatrixColumnMajor4x4fType& matrix2) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixDivide(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    行列とスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
* @param[in]    factor      スカラー値
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixDivide(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix, float factor) NN_NOEXCEPT;

/**
* @brief    逆行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆行列が見つかったかどうかを返します。
* @retval   true    逆行列が見つかりました。
* @retval   false   逆行列が見つかりませんでした。
*
* @details  逆行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE bool       MatrixInverse(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    逆行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @return   逆行列が見つかったかどうかを返します。
* @retval   true    逆行列が見つかりました。
* @retval   false   逆行列が見つかりませんでした。
*
* @details  逆行列が見つからない場合、結果の格納先にゼロ行列を書き込みます。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE bool       MatrixInverse(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixTranspose(MatrixRowMajor4x4fType* pOutValue, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    転置行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixTranspose(MatrixColumnMajor4x4fType* pOutValue, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系における透視投影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
* @param[in]    nearZ       近平面までの距離
* @param[in]    farZ        遠平面までの距離
*
* @details  深度値の幅を [0, 1] として行列を求めます。
*           グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系における透視投影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
* @param[in]    nearZ       近平面までの距離
* @param[in]    farZ        遠平面までの距離
*
* @details  深度値の幅を [0, 1] として行列を求めます。
*           グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixPerspectiveOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

/**
* @brief    視野角度を用いて、右手座標系における透視投影変換行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    fovyRadian  垂直視野角度（ラジアン表現）
* @param[in]    aspect      アスペクト比
* @param[in]    nearZ       近平面までの距離
* @param[in]    farZ        遠平面までの距離
*
* @details  深度値の幅を [0, 1] として行列を求めます。
*           グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixRowMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT;

/**
* @brief    視野角度を用いて、右手座標系における透視投影変換行列を求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    fovyRadian  垂直視野角度（ラジアン表現）
* @param[in]    aspect      アスペクト比
* @param[in]    nearZ       近平面までの距離
* @param[in]    farZ        遠平面までの距離
*
* @details  深度値の幅を [0, 1] として行列を求めます。
*           グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixPerspectiveFieldOfViewRightHanded(MatrixColumnMajor4x4fType* pOutValue, float fovyRadian, float aspect, float nearZ, float farZ) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系における正射影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
* @param[in]    nearZ       近平面までの距離
* @param[in]    farZ        遠平面までの距離
*
* @details  深度値の幅を [0, 1] として行列を求めます。
*           グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
*
* @relates  nn::util::MatrixRowMajor4x4fType
*/
NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixRowMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

/**
* @brief    近平面の座標を用いて、右手座標系における正射影変換行列を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    left        近平面の左側 X 座標
* @param[in]    right       近平面の右側 X 座標
* @param[in]    bottom      近平面の下側 Y 座標
* @param[in]    top         近平面の上側 Y 座標
* @param[in]    nearZ       近平面までの距離
* @param[in]    farZ        遠平面までの距離
*
* @details  深度値の幅を [0, 1] として行列を求めます。
*           グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
*
* @relates  nn::util::MatrixColumnMajor4x4fType
*/
NN_FORCEINLINE void       MatrixOrthographicOffCenterRightHanded(MatrixColumnMajor4x4fType* pOutValue, float left, float right, float bottom, float top, float nearZ, float farZ) NN_NOEXCEPT;

//! @}

}}

#endif

// 環境に応じて実装を切り替えます。
#if NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_GENERAL
    #include <nn/util/detail/util_MatrixApi.general.h>

    namespace nn { namespace util {
        using namespace ::nn::util::general;
    }}

    #define NN_UTIL_MATRIX_ROW_MAJOR_3X2F_INITIALIZER NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER NN_UTIL_GENERAL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER
    #define NN_UTIL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER NN_UTIL_GENERAL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER
    #define NN_UTIL_MATRIX_3X2F_INITIALIZER NN_UTIL_GENERAL_MATRIX_3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_T3X2F_INITIALIZER NN_UTIL_GENERAL_MATRIX_T3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_4X3F_INITIALIZER NN_UTIL_GENERAL_MATRIX_4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_T4X3F_INITIALIZER NN_UTIL_GENERAL_MATRIX_T4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_4X4F_INITIALIZER NN_UTIL_GENERAL_MATRIX_4X4F_INITIALIZER
    #define NN_UTIL_MATRIX_T4X4F_INITIALIZER NN_UTIL_GENERAL_MATRIX_T4X4F_INITIALIZER

#elif NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_NEON
    #include <nn/util/detail/util_MatrixApi.neon.h>

    namespace nn { namespace util {
        using namespace ::nn::util::neon;
    }}

    #define NN_UTIL_MATRIX_ROW_MAJOR_3X2F_INITIALIZER NN_UTIL_NEON_MATRIX_ROW_MAJOR_3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_ROW_MAJOR_4X3F_INITIALIZER NN_UTIL_NEON_MATRIX_ROW_MAJOR_4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_ROW_MAJOR_4X4F_INITIALIZER NN_UTIL_NEON_MATRIX_ROW_MAJOR_4X4F_INITIALIZER
    #define NN_UTIL_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER NN_UTIL_NEON_MATRIX_COLUMN_MAJOR_4X4F_INITIALIZER
    #define NN_UTIL_MATRIX_3X2F_INITIALIZER NN_UTIL_NEON_MATRIX_3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_T3X2F_INITIALIZER NN_UTIL_NEON_MATRIX_T3X2F_INITIALIZER
    #define NN_UTIL_MATRIX_4X3F_INITIALIZER NN_UTIL_NEON_MATRIX_4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_T4X3F_INITIALIZER NN_UTIL_NEON_MATRIX_T4X3F_INITIALIZER
    #define NN_UTIL_MATRIX_4X4F_INITIALIZER NN_UTIL_NEON_MATRIX_4X4F_INITIALIZER
    #define NN_UTIL_MATRIX_T4X4F_INITIALIZER NN_UTIL_NEON_MATRIX_T4X4F_INITIALIZER

#else
    #error   "未サポートのターゲットが指定されています。"
#endif
