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
* @brief    ベクトル計算を行う API を定義します。
*/

/**
* @brief    nn::util::Float2 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Float2 を初期化できます。
*           @code
*               nn::util::Float2 vector = NN_UTIL_FLOAT_2_INITIALIZER(0.f, 1.f);
*           @endcode
*
* @relates  nn::util::Float2
*/
#define NN_UTIL_FLOAT_2_INITIALIZER(x, y) \
    { \
        { \
            { x, y } \
        } \
    }

/**
* @brief    nn::util::Float3 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Float3 を初期化できます。
*           @code
*               nn::util::Float3 vector = NN_UTIL_FLOAT_3_INITIALIZER(0.f, 1.f, 2.f);
*           @endcode
*
* @relates  nn::util::Float3
*/
#define NN_UTIL_FLOAT_3_INITIALIZER(x, y, z) \
    { \
        { \
            { x, y, z } \
        } \
    }

/**
* @brief    nn::util::Float4 に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Float4 を初期化できます。
*           @code
*               nn::util::Float4 vector = NN_UTIL_FLOAT_4_INITIALIZER(0.f, 1.f, 2.f, 3.f);
*           @endcode
*
* @relates  nn::util::Float4
*/
#define NN_UTIL_FLOAT_4_INITIALIZER(x, y, z, w) \
    { \
        { \
            { x, y, z, w } \
        } \
    }

namespace nn { namespace util {

//! @name 2 次元ベクトル
//! @{

/**
* @brief    指定の値で初期化した nn::util::Float2 を返します。
*
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
*
* @return   指定の値で初期化された nn::util::Float2 を返します。
*
* @relates  nn::util::Float2
*/
NN_FORCEINLINE Float2 MakeFloat2(float x, float y) NN_NOEXCEPT
{
    Float2 vec = NN_UTIL_FLOAT_2_INITIALIZER(x, y);
    return vec;
}

//! @}

//! @name 3 次元ベクトル
//! @{

/**
* @brief    指定の値で初期化した nn::util::Float3 を返します。
*
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
* @param[in]    z           Z 成分
*
* @return   指定の値で初期化された nn::util::Float3 を返します。
*
* @relates  nn::util::Float3
*/
NN_FORCEINLINE Float3 MakeFloat3(float x, float y, float z) NN_NOEXCEPT
{
    Float3 vec = NN_UTIL_FLOAT_3_INITIALIZER(x, y, z);
    return vec;
}

//! @}

//! @name 4 次元ベクトル
//! @{

/**
* @brief    指定の値で初期化した  nn::util::Float4 を返します。
*
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
* @param[in]    z           Z 成分
* @param[in]    w           W 成分
*
* @return   指定の値で初期化された nn::util::Float4 を返します。
*
* @relates  nn::util::Float4
*/
NN_FORCEINLINE Float4 MakeFloat4(float x, float y, float z, float w) NN_NOEXCEPT
{
    Float4 vec = NN_UTIL_FLOAT_4_INITIALIZER(x, y, z, w);
    return vec;
}

//! @}

}}

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

/**
* @brief    nn::util::Vector2fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Vector2fType を初期化できます。
*           @code
*               nn::util::Vector2fType vector = NN_UTIL_VECTOR_2F_INITIALIZER(0.f, 1.f);
*           @endcode
*
* @relates  nn::util::Vector2fType
*/
#define NN_UTIL_VECTOR_2F_INITIALIZER(x, y)

/**
* @brief    nn::util::Vector3fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Vector3fType を初期化できます。
*           @code
*               nn::util::Vector3fType vector = NN_UTIL_VECTOR_3F_INITIALIZER(0.f, 1.f, 2.f);
*           @endcode
*
* @relates  nn::util::Vector3fType
*/
#define NN_UTIL_VECTOR_3F_INITIALIZER(x, y, z)

/**
* @brief    nn::util::Vector4fType に対する初期化子として利用できるマクロです。
*
* @details  本マクロは初期化リストとなり、以下のように記述することで nn::util::Vector4fType を初期化できます。
*           @code
*               nn::util::Vector4fType vector = NN_UTIL_VECTOR_4F_INITIALIZER(0.f, 1.f, 2.f, 3.f);
*           @endcode
*
* @relates  nn::util::Vector4fType
*/
#define NN_UTIL_VECTOR_4F_INITIALIZER(x, y, z, w)

namespace nn { namespace util {

//! @name 2 次元ベクトル
//! @{

/**
* @brief    指定の値で初期化した nn::util::Vector2fType を返します。
*
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
*
* @return   指定の値で初期化された nn::util::Vector2fType を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE Vector2fType MakeVector2fType(float x, float y) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    pSource     読み込み元
*
* @details  ポインタの示す領域から、2 要素を順にベクトルへ読み込みます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const float* pSource) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorLoad(Vector2fType* pOutValue, const Float2& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  ポインタの示す領域に対し、ベクトルの 3 要素を順に書き出します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector2fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorStore(Float2* pOutValue, const Vector2fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルの各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorSet(Vector2fType* pOutValue, float x, float y) NN_NOEXCEPT;

/**
* @brief    ベクトルの X 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       X 成分
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorSetX(Vector2fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの Y 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       Y 成分
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorSetY(Vector2fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの X 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの X 成分を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorGetX(const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの Y 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの Y 成分を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorGetY(const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルをゼロベクトルに設定します。
*
* @param[out]   pOutValue   ゼロベクトルの格納先
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorZero(Vector2fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    ベクトルがゼロベクトルかどうかを判定します。
*
* @param[in]    vector   判定対象
*
* @return   判定結果を返します。
* @retval   true    ゼロベクトルです。
* @retval   false   ゼロベクトルではありません。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    ゼロベクトルと比較して、ベクトルが指定の誤差内に収まっているかどうかを判定します。
*
* @param[in]    vector   判定対象
* @param[in]    error    誤差
*
* @return   判定結果を返します。
* @retval   true    指定の誤差内に収まっています。
* @retval   false   指定の誤差内に収まっていません。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE bool       VectorIsZero(const Vector2fType& vector, float error) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorAdd(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  vector1 - vector2 を求めます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorSubtract(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルとスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの対応する要素同士の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = vector1.x * vector2.x;
*               pOutValue->v.y = vector1.y * vector2.y;
*           @endcode
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorMultiply(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルとスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの対応する要素同士の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = vector1.x / vector2.x;
*               pOutValue->v.y = vector1.y / vector2.y;
*           @endcode
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorDivide(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの内積を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの内積を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorDot(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの外積を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   vector1 x vector2 を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorCross(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルの長さを求めます。
*
* @param[in]    vector      対象ベクトル
*
* @return   ベクトルの長さを返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorLength(const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの長さの 2 乗を求めます。
*
* @param[in]    vector      対象ベクトル
*
* @return   ベクトルの長さの 2 乗を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorLengthSquared(const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの距離を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの距離を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorDistance(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの 2 乗を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの距離の 2 乗を返します。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorDistanceSquared(const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    正規化したベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
*
* @return   対象ベクトルの長さの 2 乗を返します。
*
* @details  対象ベクトルがゼロベクトルの場合、結果の格納先にゼロベクトルを書き込みます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE float      VectorNormalize(Vector2fType* pOutValue, const Vector2fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの各成分の最大値から構成されるベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = max(vector1.x, vector2.x);
*               pOutValue->v.y = max(vector1.y, vector2.y);
*           @endcode
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorMaximize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの各成分の最小値から構成されるベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = min(vector1.x, vector2.x);
*               pOutValue->v.y = min(vector1.y, vector2.y);
*           @endcode
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorMinimize(Vector2fType* pOutValue, const Vector2fType& vector1, const Vector2fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトル間の線形補間を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    from        第 1 ベクトル
* @param[in]    to          第 2 ベクトル
* @param[in]    t           補間係数
*
* @details  2 つのベクトル from と to の間を t : 1 - t の割合で線形補間します。
*
*           本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = from.x + t * (to.x - from.x);
*               pOutValue->v.y = from.y + t * (to.y - from.y);
*           @endcode
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorLerp(Vector2fType* pOutValue, const Vector2fType& from, const Vector2fType& to, float t) NN_NOEXCEPT;

/**
* @brief    行列を用いて 2 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 2 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorTransform(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 2 次元ベクトルを変換します。ベクトルの第 3 成分を 0 として計算します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixRowMajor3x2fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 2 次元ベクトルを変換します。ベクトルの第 3 成分を 0 として計算します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector2fType
*/
NN_FORCEINLINE void       VectorTransformNormal(Vector2fType* pOutValue, const Vector2fType& vector, const MatrixColumnMajor3x2fType& matrix) NN_NOEXCEPT;

//! @}

//! @name 3 次元ベクトル
//! @{

/**
* @brief    指定の値で初期化した nn::util::Vector3fType を返します。
*
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
* @param[in]    z           Z 成分
*
* @return   指定の値で初期化された nn::util::Vector3fType を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE Vector3fType MakeVector3fType(float x, float y, float z) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    pSource     読み込み元
*
* @details  ポインタの示す領域から、3 要素を順にベクトルへ読み込みます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const float* pSource) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorLoad(Vector3fType* pOutValue, const Float3& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  ポインタの示す領域に対し、ベクトルの 3 要素を順に書き出します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector3fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorStore(Float3* pOutValue, const Vector3fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルの各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
* @param[in]    z           Z 成分
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorSet(Vector3fType* pOutValue, float x, float y, float z) NN_NOEXCEPT;

/**
* @brief    ベクトルの X 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       X 成分
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorSetX(Vector3fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの Y 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       Y 成分
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorSetY(Vector3fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの Z 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       Z 成分
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorSetZ(Vector3fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの X 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの X 成分を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorGetX(const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの Y 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの Y 成分を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorGetY(const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの Z 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの Z 成分を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorGetZ(const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルをゼロベクトルに設定します。
*
* @param[out]   pOutValue   ゼロベクトルの格納先
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorZero(Vector3fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    ベクトルがゼロベクトルかどうかを判定します。
*
* @param[in]    vector   判定対象
*
* @return   判定結果を返します。
* @retval   true    ゼロベクトルです。
* @retval   false   ゼロベクトルではありません。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    ゼロベクトルと比較して、ベクトルが指定の誤差内に収まっているかどうかを判定します。
*
* @param[in]    vector   判定対象
* @param[in]    error    誤差
*
* @return   判定結果を返します。
* @retval   true    指定の誤差内に収まっています。
* @retval   false   指定の誤差内に収まっていません。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE bool       VectorIsZero(const Vector3fType& vector, float error) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorAdd(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  vector1 - vector2 を求めます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorSubtract(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルとスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの対応する要素同士の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = vector1.x * vector2.x;
*               pOutValue->v.y = vector1.y * vector2.y;
*               pOutValue->v.z = vector1.z * vector2.z;
*           @endcode
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorMultiply(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルとスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの対応する要素同士の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = vector1.x / vector2.x;
*               pOutValue->v.y = vector1.y / vector2.y;
*               pOutValue->v.z = vector1.z / vector2.z;
*           @endcode
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorDivide(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの内積を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの内積を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorDot(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの外積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  vector1 x vector2 を求めます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorCross(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルの長さを求めます。
*
* @param[in]    vector      対象ベクトル
*
* @return   ベクトルの長さを返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorLength(const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの長さの 2 乗を求めます。
*
* @param[in]    vector      対象ベクトル
*
* @return   ベクトルの長さの 2 乗を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorLengthSquared(const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの距離を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの距離を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorDistance(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの 2 乗を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの距離の 2 乗を返します。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorDistanceSquared(const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    正規化したベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
*
* @return   対象ベクトルの長さの 2 乗を返します。
*
* @details  対象ベクトルがゼロベクトルの場合、結果の格納先にゼロベクトルを書き込みます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE float      VectorNormalize(Vector3fType* pOutValue, const Vector3fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの各成分の最大値から構成されるベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = max(vector1.x, vector2.x);
*               pOutValue->v.y = max(vector1.y, vector2.y);
*               pOutValue->v.z = max(vector1.z, vector2.z);
*           @endcode
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorMaximize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの各成分の最小値から構成されるベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = min(vector1.x, vector2.x);
*               pOutValue->v.y = min(vector1.y, vector2.y);
*               pOutValue->v.z = min(vector1.z, vector2.z);
*           @endcode
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorMinimize(Vector3fType* pOutValue, const Vector3fType& vector1, const Vector3fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトル間の線形補間を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    from        第 1 ベクトル
* @param[in]    to          第 2 ベクトル
* @param[in]    t           補間係数
*
* @details  2 つのベクトル from と to の間を t : 1 - t の割合で線形補間します。
*
*           本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = from.x + t * (to.x - from.x);
*               pOutValue->v.y = from.y + t * (to.y - from.y);
*               pOutValue->v.z = from.z + t * (to.z - from.z);
*           @endcode
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorLerp(Vector3fType* pOutValue, const Vector3fType& from, const Vector3fType& to, float t) NN_NOEXCEPT;

/**
* @brief    クォータニオンを用いて 3 次元ベクトルを回転します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      回転対象のベクトル
* @param[in]    quaternion  クォータニオン
*
* @details  vector に対し quaternion を右から掛けます。
*           また、与えられたクォータニオンを正規化しません。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorRotate(Vector3fType* pOutValue, const Vector3fType& vector, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransform(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。ベクトルの第 4 成分を 0 として計算します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。ベクトルの第 4 成分を 0 として計算します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransformNormal(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。ベクトルの第 4 成分を 1 として計算します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 3 次元ベクトルを変換します。ベクトルの第 4 成分を 1 として計算します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector3fType
*/
NN_FORCEINLINE void       VectorTransformCoord(Vector3fType* pOutValue, const Vector3fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

//! @}

//! @name 4 次元ベクトル
//! @{

/**
* @brief    指定の値で初期化した nn::util::Vector4fType を返します。
*
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
* @param[in]    z           Z 成分
* @param[in]    w           W 成分
*
* @return   指定の値で初期化された nn::util::Vector4fType を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE Vector4fType MakeVector4fType(float x, float y, float z, float w) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Uint8x4& source) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  各要素は、0.0f ~ 1.0f の値域に正規化されます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Unorm8x4& source) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    pSource     読み込み元
*
* @details  ポインタの示す領域から、4 要素を順にベクトルへ読み込みます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const float* pSource) NN_NOEXCEPT;

/**
* @brief    ベクトルへ値を読み込みます。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorLoad(Vector4fType* pOutValue, const Float4& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  読み込み元の各要素は、0.0f ~ 255.0f の値域に関数内でクランプ処理されます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorStore(Uint8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   読み込み先
* @param[in]    source      読み込み元
*
* @details  読み込み元の各要素は、0.0f ~ 1.0f の値域に、関数内でクランプ処理されたあと、値を 0 ~ 255 にスケールして書き出します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorStore(Unorm8x4* pOutValue, const Vector4fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @details  ポインタの示す領域に対し、ベクトルの 4 要素を順に書き出します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorStore(float* pOutValue, const Vector4fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルから値を書き出します。
*
* @param[out]   pOutValue   書き出し先
* @param[in]    source      書き出し元
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorStore(Float4* pOutValue, const Vector4fType& source) NN_NOEXCEPT;

/**
* @brief    ベクトルの各成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    x           X 成分
* @param[in]    y           Y 成分
* @param[in]    z           Z 成分
* @param[in]    w           W 成分
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorSet(Vector4fType* pOutValue, float x, float y, float z, float w) NN_NOEXCEPT;

/**
* @brief    ベクトルの X 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       X 成分
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorSetX(Vector4fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの Y 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       Y 成分
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorSetY(Vector4fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの Z 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       Z 成分
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorSetZ(Vector4fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの W 成分を設定します。
*
* @param[out]   pOutValue   成分の格納先
* @param[in]    value       W 成分
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorSetW(Vector4fType* pOutValue, float value) NN_NOEXCEPT;

/**
* @brief    ベクトルの X 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの X 成分を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorGetX(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの Y 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの Y 成分を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorGetY(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの Z 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの Z 成分を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorGetZ(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの W 成分を取得します。
*
* @param[in]    vector      取得対象のベクトル
*
* @return   ベクトルの W 成分を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorGetW(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルをゼロベクトルに設定します。
*
* @param[out]   pOutValue   ゼロベクトルの格納先
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorZero(Vector4fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    ベクトルがゼロベクトルかどうかを判定します。
*
* @param[in]    vector   判定対象
*
* @return   判定結果を返します。
* @retval   true    ゼロベクトルです。
* @retval   false   ゼロベクトルではありません。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE bool       VectorIsZero(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルが同次座標におけるゼロベクトルかどうかを判定します。
*
* @param[in]    vector   判定対象
*
* @return   判定結果を返します。
* @retval   true    ゼロベクトルです。
* @retval   false   ゼロベクトルではありません。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE bool       VectorIsZeroWOne(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの和を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorAdd(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの差を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details vector1 - vector2 を求めます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorSubtract(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルとスカラー値の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの対応する要素同士の積を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = vector1.x * vector2.x;
*               pOutValue->v.y = vector1.y * vector2.y;
*               pOutValue->v.z = vector1.z * vector2.z;
*               pOutValue->v.w = vector1.w * vector2.w;
*           @endcode
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorMultiply(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルとスカラー値の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの対応する要素同士の商を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = vector1.x / vector2.x;
*               pOutValue->v.y = vector1.y / vector2.y;
*               pOutValue->v.z = vector1.z / vector2.z;
*               pOutValue->v.w = vector1.w / vector2.w;
*           @endcode
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorDivide(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの内積を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの内積を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorDot(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    ベクトルの長さを求めます。
*
* @param[in]    vector      長さを求めるベクトル
*
* @return   ベクトルの長さを返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorLength(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    ベクトルの長さの 2 乗を求めます。
*
* @param[in]    vector      長さの 2 乗を求めるベクトル
*
* @return   ベクトルの長さの 2 乗を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorLengthSquared(const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの距離を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの距離を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorDistance(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの 2 乗を求めます。
*
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @return   2 つのベクトルの距離の 2 乗を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorDistanceSquared(const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    正規化したベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      対象ベクトル
*
* @return   対象ベクトルの長さの 2 乗を返します。
*
* @details  対象ベクトルがゼロベクトルの場合、結果の格納先にゼロベクトルを書き込みます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      VectorNormalize(Vector4fType* pOutValue, const Vector4fType& vector) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの各成分の最大値から構成されるベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = max(vector1.x, vector2.x);
*               pOutValue->v.y = max(vector1.y, vector2.y);
*               pOutValue->v.z = max(vector1.z, vector2.z);
*               pOutValue->v.w = max(vector1.w, vector2.w);
*           @endcode
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorMaximize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトルの各成分の最小値から構成されるベクトルを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector1     第 1 ベクトル
* @param[in]    vector2     第 2 ベクトル
*
* @details  本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = min(vector1.x, vector2.x);
*               pOutValue->v.y = min(vector1.y, vector2.y);
*               pOutValue->v.z = min(vector1.z, vector2.z);
*               pOutValue->v.w = min(vector1.w, vector2.w);
*           @endcode
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorMinimize(Vector4fType* pOutValue, const Vector4fType& vector1, const Vector4fType& vector2) NN_NOEXCEPT;

/**
* @brief    2 つのベクトル間の線形補間を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    from        第 1 ベクトル
* @param[in]    to          第 2 ベクトル
* @param[in]    t           補間係数
*
* @details  2 つのベクトル from と to の間を t : 1 - t の割合で線形補間します。
*
*           本関数の処理を擬似コードにて示します。
*           @code
*               pOutValue->v.x = from.x + t * (to.x - from.x);
*               pOutValue->v.y = from.y + t * (to.y - from.y);
*               pOutValue->v.z = from.z + t * (to.z - from.z);
*               pOutValue->v.w = from.w + t * (to.w - from.w);
*           @endcode
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT;

/**
* @brief    行列を用いて 4 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixRowMajor4x4fType& matrix) NN_NOEXCEPT;

/**
* @brief    行列を用いて 4 次元ベクトルを変換します。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    vector      変換対象のベクトル
* @param[in]    matrix      変換行列
*
* @details  vector に対し、matrix を右から掛けます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       VectorTransform(Vector4fType* pOutValue, const Vector4fType& vector, const MatrixColumnMajor4x4fType& matrix) NN_NOEXCEPT;

//! @}

}}

#endif

// 環境に応じて実装を切り替えます。
#if NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_GENERAL
    #include <nn/util/detail/util_VectorApi.general.h>

    namespace nn { namespace util {
        using namespace ::nn::util::general;
    }}

    #define NN_UTIL_VECTOR_2F_INITIALIZER NN_UTIL_GENERAL_VECTOR_2F_INITIALIZER
    #define NN_UTIL_VECTOR_3F_INITIALIZER NN_UTIL_GENERAL_VECTOR_3F_INITIALIZER
    #define NN_UTIL_VECTOR_4F_INITIALIZER NN_UTIL_GENERAL_VECTOR_4F_INITIALIZER

#elif NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_NEON
    #include <nn/util/detail/util_VectorApi.neon.h>

    namespace nn { namespace util {
        using namespace ::nn::util::neon;
    }}

    #define NN_UTIL_VECTOR_2F_INITIALIZER NN_UTIL_NEON_VECTOR_2F_INITIALIZER
    #define NN_UTIL_VECTOR_3F_INITIALIZER NN_UTIL_NEON_VECTOR_3F_INITIALIZER
    #define NN_UTIL_VECTOR_4F_INITIALIZER NN_UTIL_NEON_VECTOR_4F_INITIALIZER
#else
    #error   "未サポートのターゲットが指定されています。"
#endif
