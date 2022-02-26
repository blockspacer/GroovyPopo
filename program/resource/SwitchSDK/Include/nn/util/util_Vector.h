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

#include <nn/nn_Macro.h>
#include <nn/util/util_VectorApi.h>

/**
* @file
* @brief    ベクトル計算を提供します。
*/

namespace nn
{
namespace util
{

class Vector2f;
class Vector3f;
class Vector4f;
class MatrixRowMajor3x2f;
class MatrixRowMajor4x3f;
class MatrixRowMajor4x4f;
class Quaternion;

//! @brief 単精度浮動小数点型による 2 次元ベクトルを表現するクラスです。
//!
//! @details    ベクトル・行列計算を提供する型です。
//!
//!  nn::util::Vector2fType を継承し、メソッドを追加するユーティリティクラスです。
//!
//!  この型を用いた計算は、SIMD ユニットを使える環境においては、SIMD ユニットを利用した実装に切り替わることを想定しています。
//!  SIMD ユニットを使えない環境では、汎用の実装を提供します。
//!
//! @attention
//!  この型は、計算のために最適化されています。このため、メモリレイアウトもサイズも環境によって
//!  異なる場合があります。よって、 @b 内部要素への直接のアクセスは禁止します 。
//!  要素へのアクセスは、専用のマクロ、および、関数を利用してください。
//!
//! @attention
//!  また、SIMD ユニットでの計算を最適化するため、この型は、 @b メモリアライメントとして16byte境界に配置されることを要求します 。
//!  この型をメモリ上に確保する場合には、要求されるアライメントを満たすよう注意してください。
class Vector2f : public Vector2fType
{
public:
    //! @brief コンストラクタです。
    Vector2f() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] x X 成分
    //! @param[in] y Y 成分
    Vector2f(float x, float y) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 2 次元ベクトルです。
    explicit Vector2f(const Float2& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 2 次元ベクトルです。
    NN_IMPLICIT Vector2f(const Vector2fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 成分を取得します。
    //! @return X 成分を返します
    float GetX() const NN_NOEXCEPT;

    //! @brief Y 成分を取得します。
    //! @return Y 成分を返します。
    float GetY() const NN_NOEXCEPT;

    //! @brief 各成分の値を取得します。
    //! @param[out] pOutX X 成分の格納先です。
    //! @param[out] pOutY Y 成分の格納先です。
    //! @pre 引数は全て非 NULL であることが必須です。
    void Get(float* pOutX, float* pOutY) const NN_NOEXCEPT;

    //! @brief X 成分を設定します。
    //! @param[in] x 設定する X 成分の値です。
    void SetX(float x) NN_NOEXCEPT;

    //! @brief Y 成分を設定します。
    //! @param[in] y 設定する Y 成分の値です。
    void SetY(float y) NN_NOEXCEPT;

    //! @brief 各成分の値を設定します。
    //! @param[in] x X 成分
    //! @param[in] y Y 成分
    void Set(float x, float y) NN_NOEXCEPT;

    //! @}
    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定されている値を返します。
    Vector2f operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    Vector2f operator - () const NN_NOEXCEPT;

    //! @brief 各成分同士のベクトル加算を行います。
    //! @param[in] value 加算する値です。
    //! @return 各成分同士のベクトル加算を行った結果を返します。
    Vector2f operator + (const Vector2f& value) const NN_NOEXCEPT;

    //! @brief 各成分同士のベクトル減算を行います。
    //! @param[in] value 減算する値です。
    //! @return 各成分同士のベクトル減算を行った結果を返します。
    Vector2f operator - (const Vector2f& value) const NN_NOEXCEPT;

    //! @brief 各成分にスカラー乗算を行います。
    //! @param[in] value 乗算するスカラー値です。
    //! @return 各成分をスカラー乗算した結果を返します。
    Vector2f operator * (float value) const NN_NOEXCEPT;

    //! @brief スカラー乗算を行います。
    //! @param[in] scalar スカラー値。
    //! @param[in] vector ベクトル値。
    //! @return スカラー乗算した結果を返却します。
    friend Vector2f operator * (float scalar, const Vector2f& vector) NN_NOEXCEPT;

    //! @brief 各成分にスカラー除算を行います。
    //! @param[in] value 除算するスカラー値です。
    //! @return 各成分をスカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    Vector2f operator / (float value) const NN_NOEXCEPT;

    //! @brief 各成分ごとに加算代入を行います。
    //! @param[in] value 加算する値です。
    //! @return ベクトル加算の結果を返します。
    Vector2f& operator += (const Vector2f& value) NN_NOEXCEPT;

    //! @brief 各成分ごとに減算代入を行います。
    //! @param[in] value 減算する値です。
    //! @return ベクトル減算の結果を返します。
    Vector2f& operator -= (const Vector2f& value) NN_NOEXCEPT;

    //! @brief 各成分にスカラー乗算をします。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算の結果を返します。
    Vector2f& operator *= (float value) NN_NOEXCEPT;

    //! @brief 各成分にスカラー除算をします。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算の結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    Vector2f& operator /= (float value) NN_NOEXCEPT;

    //! @}

    //! @brief ベクトルを正規化します。
    //! @return 正規化に成功したら true を返します。
    //! @details ゼロベクトルであった場合は、ゼロベクトルを設定します。
    bool Normalize() NN_NOEXCEPT;

    //! @brief 内積を計算します。
    //! @param[in] value ベクトル。
    //! @return 内積を計算した結果を返します。
    float Dot(const Vector2f& value) const NN_NOEXCEPT;

    //! @brief 外積を計算します。
    //! @param[in] value ベクトル。
    //! @return 外積を計算した結果を返します。
    float Cross(const Vector2f& value) const NN_NOEXCEPT;

    //! @brief ベクトルの長さを求めます。
    //! @return ベクトルの長さを返します。
    float Length() const NN_NOEXCEPT;

    //! @brief ベクトルの長さの2乗値を求めます。
    //! @return ベクトルの長さの2乗値を返します。
    float LengthSquared() const NN_NOEXCEPT;

    //! @brief 内積を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 内積を計算した結果を返します。
    static float Dot(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT;

    //! @brief 2点間距離を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2点間距離を計算した結果を返します。
    static float Distance(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT;

    //! @brief 2点間距離の2乗値を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2点間距離の2乗値を計算した結果を返します。
    static float DistanceSquared(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT;

    //! @brief ベクトルを正規化します。
    //! @param[in] value 正規化を行うベクトル。
    //! @return 正規化した結果を返します。
    //! @details 指定ベクトルがゼロベクトルの場合、ゼロベクトルを返します。
    static Vector2f Normalize(const Vector2f& value) NN_NOEXCEPT;

    //! @brief 外積を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 外積を計算した結果を返します。
    static float Cross(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT;

    //! @brief 2 つのベクトルの各成分の最大値から構成されるベクトルを求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2 つのベクトルの各成分の最大値から構成されるベクトルを返します。
    static Vector2f Maximize(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT;

    //! @brief 2 つのベクトルの各成分の最小値から構成されるベクトルを求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2 つのベクトルの各成分の最小値から構成されるベクトルを返します。
    static Vector2f Minimize(const Vector2f& a, const Vector2f& b) NN_NOEXCEPT;

    //! @brief ２つのベクトルを線形補間します。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @param[in] t 補間係数
    //! @return 2つのベクトルを線形補間した結果( a + (b - a) * t の計算結果) を返します。
    static Vector2f Lerp(const Vector2f& a, const Vector2f& b, float t) NN_NOEXCEPT;

    //! @brief 指定行列でベクトルを変換します。
    //! @param[in] value 変換対象となるベクトル
    //! @param[in] transform 変換行列
    //! @return 指定行列で変換したベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector2f Transform(const Vector2f& value, const MatrixRowMajor3x2f& transform) NN_NOEXCEPT;

    //! @brief 指定行列で法線ベクトルを変換します。ベクトルの第 3 成分を 0 として計算します。
    //! @param[in] value 変換対象となるベクトル
    //! @param[in] transform 変換行列
    //! @return 指定行列で変換したベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector2f TransformNormal(const Vector2f& value, const MatrixRowMajor3x2f& transform) NN_NOEXCEPT;

    //! @name 定数
    //! @{

    //! @brief 零ベクトルです.
    //! @return 全ての成分が零のベクトルを返します。
    static const Vector2f& Zero() NN_NOEXCEPT;

    //! @brief 全ての成分が 1 のベクトルです。
    //! @return 全ての成分が 1 のベクトルを返します。
    static const Vector2f& One() NN_NOEXCEPT;

    //! @brief X軸を表す単位ベクトルです。
    //! @return X成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector2f& UnitX() NN_NOEXCEPT;

    //! @brief Y軸を表す単位ベクトルです。
    //! @return Y成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector2f& UnitY() NN_NOEXCEPT;

    //! @}

private:
    static const Vector2fType ConstantZero;
    static const Vector2fType ConstantOne;
    static const Vector2fType ConstantUnitX;
    static const Vector2fType ConstantUnitY;
};

//! @brief 単精度浮動小数点型による 3 次元ベクトルを表現するクラスです。
//!
//! @details    ベクトル・行列計算を提供する型です。
//!
//!  nn::util::Vector3fType を継承し、メソッドを追加するユーティリティクラスです。
//!
//!  この型を用いた計算は、SIMD ユニットを使える環境においては、SIMD ユニットを利用した実装に切り替わることを想定しています。
//!  SIMD ユニットを使えない環境では、汎用の実装を提供します。
//!
//! @attention
//!  この型は、計算のために最適化されています。このため、メモリレイアウトもサイズも環境によって
//!  異なる場合があります。よって、 @b 内部要素への直接のアクセスは禁止します 。
//!  要素へのアクセスは、専用のマクロ、および、関数を利用してください。
//!
//! @attention
//!  また、SIMD ユニットでの計算を最適化するため、この型は、 @b メモリアライメントとして16byte境界に配置されることを要求します 。
//!  この型をメモリ上に確保する場合には、要求されるアライメントを満たすよう注意してください。
class Vector3f : public Vector3fType
{
public:
    //! @brief コンストラクタです。
    Vector3f() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] x X 成分
    //! @param[in] y Y 成分
    //! @param[in] z Z 成分
    Vector3f(float x, float y, float z) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value ３次元ベクトルです。
    explicit Vector3f(const Float3& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value ３次元ベクトルです。
    NN_IMPLICIT Vector3f(const Vector3fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 成分を取得します。
    //! @return X 成分を返します
    float GetX() const NN_NOEXCEPT;

    //! @brief Y 成分を取得します。
    //! @return Y 成分を返します。
    float GetY() const NN_NOEXCEPT;

    //! @brief Z 成分を取得します。
    //! @return Z 成分を返します。
    float GetZ() const NN_NOEXCEPT;

    //! @brief 各成分の値を取得します。
    //! @param[out] pOutX X 成分の格納先です。
    //! @param[out] pOutY Y 成分の格納先です。
    //! @param[out] pOutZ Z 成分の格納先です。
    //! @pre 引数は全て非 NULL であることが必須です。
    void Get(float* pOutX, float* pOutY, float* pOutZ) const NN_NOEXCEPT;

    //! @brief X 成分を設定します。
    //! @param[in] x 設定する X 成分の値です。
    void SetX(float x) NN_NOEXCEPT;

    //! @brief Y 成分を設定します。
    //! @param[in] y 設定する Y 成分の値です。
    void SetY(float y) NN_NOEXCEPT;

    //! @brief Z 成分を設定します。
    //! @param[in] z 設定する Z 成分の値です。
    void SetZ(float z) NN_NOEXCEPT;

    //! @brief 各成分の値を設定します。
    //! @param[in] x X 成分
    //! @param[in] y Y 成分
    //! @param[in] z Z 成分
    void Set(float x, float y, float z) NN_NOEXCEPT;

    //! @}
    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定されている値を返します。
    Vector3f operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    Vector3f operator - () const NN_NOEXCEPT;

    //! @brief 各成分同士のベクトル加算を行います。
    //! @param[in] value 加算する値です。
    //! @return 各成分同士のベクトル加算を行った結果を返します。
    Vector3f operator + (const Vector3f& value) const NN_NOEXCEPT;

    //! @brief 各成分同士のベクトル減算を行います。
    //! @param[in] value 減算する値です。
    //! @return 各成分同士のベクトル減算を行った結果を返します。
    Vector3f operator - (const Vector3f& value) const NN_NOEXCEPT;

    //! @brief 各成分にスカラー乗算を行います。
    //! @param[in] value 乗算するスカラー値です。
    //! @return 各成分をスカラー乗算した結果を返します。
    Vector3f operator * (float value) const NN_NOEXCEPT;

    //! @brief スカラー乗算を行います。
    //! @param[in] scalar スカラー値。
    //! @param[in] vector ベクトル値。
    //! @return スカラー乗算した結果を返却します。
    friend Vector3f operator * (float scalar, const Vector3f& vector) NN_NOEXCEPT;

    //! @brief 各成分にスカラー除算を行います。
    //! @param[in] value 除算するスカラー値です。
    //! @return 各成分をスカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    Vector3f operator / (float value) const NN_NOEXCEPT;

    //! @brief 各成分ごとに加算代入を行います。
    //! @param[in] value 加算する値です。
    //! @return ベクトル加算の結果を返します。
    Vector3f& operator += (const Vector3f& value) NN_NOEXCEPT;

    //! @brief 各成分ごとに減算代入を行います。
    //! @param[in] value 減算する値です。
    //! @return ベクトル減算の結果を返します。
    Vector3f& operator -= (const Vector3f& value) NN_NOEXCEPT;

    //! @brief 各成分にスカラー乗算をします。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算の結果を返します。
    Vector3f& operator *= (float value) NN_NOEXCEPT;

    //! @brief 各成分にスカラー除算をします。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算の結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    Vector3f& operator /= (float value) NN_NOEXCEPT;

    //! @}

    //! @brief ベクトルを正規化します。
    //! @return 正規化に成功したら true を返します。
    //! @details ゼロベクトルであった場合は、ゼロベクトルを設定します。
    bool Normalize() NN_NOEXCEPT;

    //! @brief 内積を計算します。
    //! @param[in] value ベクトル。
    //! @return 内積を計算した結果を返します。
    float Dot(const Vector3f& value) const NN_NOEXCEPT;

    //! @brief 外積を計算します。
    //! @param[in] value ベクトル。
    //! @return 外積を計算した結果を返します。
    Vector3f Cross(const Vector3f& value) const NN_NOEXCEPT;

    //! @brief ベクトルの長さを求めます。
    //! @return ベクトルの長さを返します。
    float Length() const NN_NOEXCEPT;

    //! @brief ベクトルの長さの2乗値を求めます。
    //! @return ベクトルの長さの2乗値を返します。
    float LengthSquared() const NN_NOEXCEPT;

    //! @brief 内積を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 内積を計算した結果を返します。
    static float Dot(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT;

    //! @brief 2点間距離を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2点間距離を計算した結果を返します。
    static float Distance(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT;

    //! @brief 2点間距離の2乗値を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2点間距離の2乗値を計算した結果を返します。
    static float DistanceSquared(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT;

    //! @brief ベクトルを正規化します。
    //! @param[in] value 正規化を行うベクトル。
    //! @return 正規化した結果を返します。
    //! @details 指定ベクトルがゼロベクトルの場合、ゼロベクトルを返します。
    static Vector3f Normalize(const Vector3f& value) NN_NOEXCEPT;

    //! @brief 外積を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 外積を計算した結果を返します。
    static Vector3f Cross(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT;

    //! @brief 2 つのベクトルの各成分の最大値から構成されるベクトルを求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2 つのベクトルの各成分の最大値から構成されるベクトルを返します。
    static Vector3f Maximize(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT;

    //! @brief 2 つのベクトルの各成分の最小値から構成されるベクトルを求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2 つのベクトルの各成分の最小値から構成されるベクトルを返します。
    static Vector3f Minimize(const Vector3f& a, const Vector3f& b) NN_NOEXCEPT;

    //! @brief ２つのベクトルを線形補間します。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @param[in] t 補間係数
    //! @return 2つのベクトルを線形補間した結果( a + (b - a) * t の計算結果) を返します。
    static Vector3f Lerp(const Vector3f& a, const Vector3f& b, float t) NN_NOEXCEPT;

    //! @brief クォータニオンを用いてベクトルを回転します。
    //! @param[in] value 回転対象となるベクトル
    //! @param[in] rotation クォータニオン
    //! @return クォータニオンで回転したベクトルを返します。
    //! @details ベクトルに対し、指定のクォータニオンを右から掛けます。
    //!  また、指定クォータニオンを正規化しません。
    static Vector3f Rotate(const Vector3f& value, const Quaternion& rotation) NN_NOEXCEPT;

    //! @brief 指定行列でベクトルを変換します。
    //! @param[in] value 変換対象となるベクトル
    //! @param[in] transform 変換行列
    //! @return 指定行列で変換したベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector3f Transform(const Vector3f& value, const MatrixRowMajor4x3f& transform) NN_NOEXCEPT;

    //! @brief 指定行列で法線ベクトルを変換します。ベクトルの第４成分を 0 として計算します。
    //! @param[in] value 変換対象となるベクトル
    //! @param[in] transform 変換行列
    //! @return 指定行列で変換したベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector3f TransformNormal(const Vector3f& value, const MatrixRowMajor4x3f& transform) NN_NOEXCEPT;

    //! @brief 指定行列でベクトルを変換し， w = 1 に射影します。
    //! @param[in] value 変換対象となるベクトル。
    //! @param[in] transform 変換行列。
    //! @return 指定行列変換し w = 1 に射影されたベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector3f TransformCoord(const Vector3f& value, const MatrixRowMajor4x4f& transform) NN_NOEXCEPT;

    //! @name 定数
    //! @{

    //! @brief 零ベクトルです.
    //! @return 全ての成分が零のベクトルを返します。
    static const Vector3f& Zero() NN_NOEXCEPT;

    //! @brief 全ての成分が 1 のベクトルです。
    //! @return 全ての成分が 1 のベクトルを返します。
    static const Vector3f& One() NN_NOEXCEPT;

    //! @brief X軸を表す単位ベクトルです。
    //! @return X成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector3f& UnitX() NN_NOEXCEPT;

    //! @brief Y軸を表す単位ベクトルです。
    //! @return Y成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector3f& UnitY() NN_NOEXCEPT;

    //! @brief Z軸を表す単位ベクトルです。
    //! @return Z成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector3f& UnitZ() NN_NOEXCEPT;

    //! @}

private:
    static const Vector3fType ConstantZero;
    static const Vector3fType ConstantOne;
    static const Vector3fType ConstantUnitX;
    static const Vector3fType ConstantUnitY;
    static const Vector3fType ConstantUnitZ;
};


//! @brief 単精度浮動小数点型による 4 次元ベクトルを表現するクラスです。
//!
//! @details    ベクトル・行列計算を提供する型です。
//!
//!  nn::util::Vector4fType を継承し、メソッドを追加するユーティリティクラスです。
//!
//!  クォータニオンは、別の型で表現されます。 nn::util::Quaternion を参照してください。
//!
//!  この型を用いた計算は、SIMD ユニットを使える環境においては、SIMD ユニットを利用した実装に切り替わることを想定しています。
//!  SIMD ユニットを使えない環境では、汎用の実装を提供します。
//!
//! @attention
//!  この型は、計算のために最適化されています。このため、メモリレイアウトもサイズも環境によって
//!  異なる場合があります。よって、 @b 内部要素への直接のアクセスは禁止します 。
//!  要素へのアクセスは、専用のマクロ、および、関数を利用してください。
//!
//! @attention
//!  また、SIMD ユニットでの計算を最適化するため、この型は、 @b メモリアライメントとして16byte境界に配置されることを要求します 。
//!  この型をメモリ上に確保する場合には、要求されるアライメントを満たすよう注意してください。
class Vector4f : public Vector4fType
{
public:
    //! @brief コンストラクタです。
    Vector4f() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] x X 成分
    //! @param[in] y Y 成分
    //! @param[in] z Z 成分
    //! @param[in] w W 成分
    Vector4f(float x, float y, float z, float w) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] v 3次元ベクトルです。
    //! @param[in] w W 成分です。
    Vector4f(const Vector3f& v, float w) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 次元ベクトルです。
    explicit Vector4f(const Float4& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 次元ベクトルです。
    NN_IMPLICIT Vector4f(const Vector4fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 成分を取得します。
    //! @return X 成分を返します
    float GetX() const NN_NOEXCEPT;

    //! @brief Y 成分を取得します。
    //! @return Y 成分を返します。
    float GetY() const NN_NOEXCEPT;

    //! @brief Z 成分を取得します。
    //! @return Z 成分を返します。
    float GetZ() const NN_NOEXCEPT;

    //! @brief W 成分を取得します。
    //! @return W 成分を返します。
    float GetW() const NN_NOEXCEPT;

    //! @brief 各成分の値を取得します。
    //! @param[out] pOutX X 成分の格納先です。
    //! @param[out] pOutY Y 成分の格納先です。
    //! @param[out] pOutZ Z 成分の格納先です。
    //! @param[out] pOutW W 成分の格納先です。
    //! @pre 引数は全て非 NULL であることが必須です。
    void Get(float* pOutX, float* pOutY, float* pOutZ, float* pOutW) const NN_NOEXCEPT;

    //! @brief X 成分を設定します。
    //! @param[in] x 設定する X 成分の値です。
    void SetX(float x) NN_NOEXCEPT;

    //! @brief Y成分を設定します。
    //! @param[in] y 設定する Y 成分の値です。
    void SetY(float y) NN_NOEXCEPT;

    //! @brief Z成分を設定します。
    //! @param[in] z 設定する Z 成分の値です。
    void SetZ(float z) NN_NOEXCEPT;

    //! @brief W成分を設定します。
    //! @param[in] w 設定する W 成分の値です。
    void SetW(float w) NN_NOEXCEPT;

    //! @brief 各成分の値を設定します。
    //! @param[in] x X 成分
    //! @param[in] y Y 成分
    //! @param[in] z Z 成分
    //! @param[in] w W 成分
    void Set(float x, float y, float z, float w) NN_NOEXCEPT;

    //! @}
    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定されている値を返します。
    Vector4f operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    Vector4f operator - () const NN_NOEXCEPT;

    //! @brief 各成分同士のベクトル加算を行います。
    //! @param[in] value 加算する値です。
    //! @return 各成分同士のベクトル加算を行った結果を返します。
    Vector4f operator + (const Vector4f& value) const NN_NOEXCEPT;

    //! @brief 各成分同士のベクトル減算を行います。
    //! @param[in] value 減算する値です。
    //! @return 各成分同士のベクトル減算を行った結果を返します。
    Vector4f operator - (const Vector4f& value) const NN_NOEXCEPT;

    //! @brief 各成分にスカラー乗算を行います。
    //! @param[in] value 乗算するスカラー値です。
    //! @return 各成分をスカラー乗算した結果を返します。
    Vector4f operator * (float value) const NN_NOEXCEPT;

    //! @brief スカラー乗算を行います。
    //! @param[in] scalar スカラー値。
    //! @param[in] vector ベクトル値。
    //! @return スカラー乗算した結果を返却します。
    friend Vector4f operator * (float scalar, const Vector4f& vector) NN_NOEXCEPT;

    //! @brief 各成分にスカラー除算を行います。
    //! @param[in] value 除算するスカラー値です。
    //! @return 各成分をスカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    Vector4f operator / (float value) const NN_NOEXCEPT;

    //! @brief 各成分ごとに加算代入を行います。
    //! @param[in] value 加算する値です。
    //! @return ベクトル加算の結果を返します。
    Vector4f& operator += (const Vector4f& value) NN_NOEXCEPT;

    //! @brief 各成分ごとに減算代入を行います。
    //! @param[in] value 減算する値です。
    //! @return ベクトル減算の結果を返します。
    Vector4f& operator -= (const Vector4f& value) NN_NOEXCEPT;

    //! @brief 各成分にスカラー乗算をします。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算の結果を返します。
    Vector4f& operator *= (float value) NN_NOEXCEPT;

    //! @brief 各成分にスカラー除算をします。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算の結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    Vector4f& operator /= (float value) NN_NOEXCEPT;

    //! @}

    //! @brief ベクトルを正規化します。
    //! @return 正規化に成功したら true を返します。
    //! @details ゼロベクトルであった場合は、ゼロベクトルを設定します。
    bool Normalize() NN_NOEXCEPT;

    //! @brief 内積を計算します。
    //! @param[in] value ベクトル。
    //! @return 内積を計算した結果を返します。
    float Dot(const Vector4f& value) const NN_NOEXCEPT;

    //! @brief ベクトルの長さを求めます。
    //! @return ベクトルの長さを返します。
    float Length() const NN_NOEXCEPT;

    //! @brief ベクトルの長さの 2 乗値を求めます。
    //! @return ベクトルの長さの 2 乗値を返します。
    float LengthSquared() const NN_NOEXCEPT;

    //! @brief 内積を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 内積を計算した結果を返します。
    static float Dot(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT;

    //! @brief 2点間距離を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2点間距離を計算した結果を返します。
    static float Distance(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT;

    //! @brief 2点間距離の2乗値を求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2点間距離の2乗値を計算した結果を返します。
    static float DistanceSquared(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT;

    //! @brief ベクトルを正規化します。
    //! @param[in] value 正規化を行うベクトル。
    //! @return 正規化した結果を返します。
    //! @details 指定ベクトルがゼロベクトルの場合、ゼロベクトルを返します。
    static Vector4f Normalize(const Vector4f& value) NN_NOEXCEPT;

    //! @brief 2 つのベクトルの各成分の最大値から構成されるベクトルを求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2 つのベクトルの各成分の最大値から構成されるベクトルを返します。
    static Vector4f Maximize(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT;

    //! @brief 2 つのベクトルの各成分の最小値から構成されるベクトルを求めます。
    //! @param[in] a ベクトル 1
    //! @param[in] b ベクトル 2
    //! @return 2 つのベクトルの各成分の最小値から構成されるベクトルを返します。
    static Vector4f Minimize(const Vector4f& a, const Vector4f& b) NN_NOEXCEPT;

    //! @brief ２つのベクトルを線形補間します。
    //! @param[in] a ベクトル1
    //! @param[in] b ベクトル2
    //! @param[in] t 補間係数
    //! @return 2つのベクトルを線形補間した結果( a + (b - a) * t の計算結果) を返します。
    static Vector4f Lerp(const Vector4f& a, const Vector4f& b, float t) NN_NOEXCEPT;

    //! @brief 指定行列でベクトルを変換します。
    //! @param[in] value 変換対象となるベクトル
    //! @param[in] transform 変換行列
    //! @return 指定行列で変換したベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector4f Transform(const Vector4f& value, const MatrixRowMajor4x4f& transform) NN_NOEXCEPT;

    //! @brief 指定行列でベクトルを変換します。
    //! @param[in] value 変換対象となるベクトル
    //! @param[in] transform 変換行列
    //! @return 指定行列で変換したベクトルを返します。
    //! @details ベクトルに対し、指定行列を右から掛けます。
    static Vector4f Transform(const Vector3f& value, const MatrixRowMajor4x4f& transform) NN_NOEXCEPT;

    //! @name 定数
    //! @{

    //! @brief 零ベクトルです.
    //! @return 全ての成分が零のベクトルを返します。
    static const Vector4f& Zero() NN_NOEXCEPT;

    //! @brief 全ての成分が 1 のベクトルです。
    //! @return 全ての成分が 1 のベクトルを返します。
    static const Vector4f& One() NN_NOEXCEPT;

    //! @brief X軸を表す単位ベクトルです。
    //! @return X成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector4f& UnitX() NN_NOEXCEPT;

    //! @brief Y軸を表す単位ベクトルです。
    //! @return Y成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector4f& UnitY() NN_NOEXCEPT;

    //! @brief Z軸を表す単位ベクトルです。
    //! @return Z成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector4f& UnitZ() NN_NOEXCEPT;

    //! @brief W軸を表す単位ベクトルです。
    //! @return W成分が 1 でその他の成分が 0 の単位ベクトルです。
    static const Vector4f& UnitW() NN_NOEXCEPT;

    //! @}

private:
    static const Vector4fType ConstantZero;
    static const Vector4fType ConstantOne;
    static const Vector4fType ConstantUnitX;
    static const Vector4fType ConstantUnitY;
    static const Vector4fType ConstantUnitZ;
    static const Vector4fType ConstantUnitW;
};

}
}

#include <nn/util/detail/util_VectorImpl.h>
