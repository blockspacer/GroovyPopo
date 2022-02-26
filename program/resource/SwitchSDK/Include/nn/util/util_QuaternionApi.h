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
* @brief    クォータニオンの計算を行う API を定義します。
*/

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

namespace nn { namespace util {

//! @name クォータニオン
//! @{

/**
* @brief    指定の回転行列に対応するクォータニオンを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixRowMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    指定の回転行列に対応するクォータニオンを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    matrix      対象行列
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionFromMatrix(Vector4fType* pOutValue, const MatrixColumnMajor4x3fType& matrix) NN_NOEXCEPT;

/**
* @brief    クォータニオンを単位クォータニオンに設定します。
*
* @param[out]   pOutValue   単位クォータニオンの格納先
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionIdentity(Vector4fType* pOutValue) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオンの和を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    quaternion1     第 1 クォータニオン
* @param[in]    quaternion2     第 2 クォータニオン
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionAdd(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオンの差を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    quaternion1     第 1 クォータニオン
* @param[in]    quaternion2     第 2 クォータニオン
*
* @details  quaternion1 - quaternion2 を求めます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionSubtract(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;

/**
* @brief    クォータニオンのスカラー倍を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
* @param[in]    factor      スカラー値
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion, float factor) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオンの積を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    quaternion1     第 1 クォータニオン
* @param[in]    quaternion2     第 2 クォータニオン
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionMultiply(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオンの商を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    quaternion1     第 1 クォータニオン
* @param[in]    quaternion2     第 2 クォータニオン
*
* @details  quaternion1 に対し quaternion2 の逆数を掛けます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionDivide(Vector4fType* pOutValue, const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオンの内積を求めます。
*
* @param[in]    quaternion1     第 1 クォータニオン
* @param[in]    quaternion2     第 2 クォータニオン
*
* @return   2 つのクォータニオンの内積を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      QuaternionDot(const Vector4fType& quaternion1, const Vector4fType& quaternion2) NN_NOEXCEPT;

/**
* @brief    クォータニオンの逆数を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
*
* @details  対象クォータニオンがゼロクォータニオンの場合、結果の格納先にゼロクォータニオンが書き込まれます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionInverse(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    正規化したクォータニオンを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
*
* @return   対象クォータニオンの長さの 2 乗を返します。
*
* @details  対象クォータニオンがゼロクォータニオンの場合、結果の格納先にゼロクォータニオンが書き込まれます。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE float      QuaternionNormalize(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    自然対数の底 e のクォータニオン乗を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionExp(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    クォータニオンの自然対数を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    quaternion  対象クォータニオン
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionLogN(Vector4fType* pOutValue, const Vector4fType& quaternion) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオン間の線形補間を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    from            第 1 クォータニオン
* @param[in]    to              第 2 クォータニオン
* @param[in]    t               補間係数
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
NN_FORCEINLINE void       QuaternionLerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT;

/**
* @brief    2 つのクォータニオン間の球面線形補間を求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    from            第 1 クォータニオン
* @param[in]    to              第 2 クォータニオン
* @param[in]    t               補間係数
*
* @details  from から to に向かって球面線形補間します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionSlerp(Vector4fType* pOutValue, const Vector4fType& from, const Vector4fType& to, float t) NN_NOEXCEPT;

/**
* @brief    球面 3 次補間を求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    p           第 1 クォータニオン
* @param[in]    a           第 2 クォータニオン
* @param[in]    b           第 3 クォータニオン
* @param[in]    q           第 4 クォータニオン
* @param[in]    t           補間係数
*
* @details  球面 3 次補間の結果を返します。
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionSquad(Vector4fType* pOutValue, const Vector4fType& p, const Vector4fType& a, const Vector4fType& b, const Vector4fType& q, float t) NN_NOEXCEPT;

/**
* @brief    超球面上において、第 2 クォータニオンと同じ側に存在する第 1 クォータニオンを求めます。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    quaternion      第 1 クォータニオン
* @param[in]    quaternionTo    第 2 クォータニオン
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionMakeClosest(Vector4fType*  pOutValue, const Vector4fType& quaternion, const Vector4fType& quaternionTo) NN_NOEXCEPT;

/**
* @brief    任意軸まわりの回転を表現するクォータニオンを求めます。角度の表現にラジアンを用います。
*
* @param[out]   pOutValue       結果の格納先
* @param[in]    axis            軸
* @param[in]    rotationRadian  回転（ラジアン表現）
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionRotateAxis(Vector4fType* pOutValue, const Vector3fType& axis, float rotationRadian) NN_NOEXCEPT;

/**
* @brief    第 1 ベクトルを第 2 ベクトルに変換するクォータニオンを求めます。
*
* @param[out]   pOutValue   結果の格納先
* @param[in]    from        第 1 ベクトル
* @param[in]    to          第 2 ベクトル
*
* @relates  nn::util::Vector4fType
*/
NN_FORCEINLINE void       QuaternionMakeVectorRotation(Vector4fType* pOutValue, const Vector3fType& from, const Vector3fType& to) NN_NOEXCEPT;

//! @}

}}

#endif

// 環境に応じて実装を切り替えます。
#if NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_GENERAL
    #include <nn/util/detail/util_QuaternionApi.general.h>

    namespace nn { namespace util {
        using namespace ::nn::util::general;
    }}

#elif NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_NEON
    #include <nn/util/detail/util_QuaternionApi.neon.h>

    namespace nn { namespace util {
        using namespace ::nn::util::neon;
    }}

#else
    #error   "未サポートのターゲットが指定されています。"
#endif
