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

#include <nn/util/util_QuaternionApi.h>

/**
* @file
* @brief    クォータニオンの計算を提供します。
*/

namespace nn
{
namespace util
{

class Vector3f;
class MatrixRowMajor4x3f;
class MatrixRowMajor4x4f;

//! @brief 単精度浮動小数点型によるクォータニオンを表現するクラスです。
//!
//! @details    ベクトル・行列計算を提供する型です。
//!
//!  nn::util::Vector4fType を継承し、メソッドを追加するユーティリティクラスです。
//!
//!  ベクトルの要素を先頭から順に x, y, z, w、クォータニオンの虚数成分を i, j, k としたとき、
//!  クォータニオン q は、q = xi + yj + zk + w と表されます。
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
class Quaternion : public Vector4fType
{
public:
    //! @brief コンストラクタです。
    Quaternion() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] x X 成分です。
    //! @param[in] y Y 成分です。
    //! @param[in] z Z 成分です。
    //! @param[in] w W 成分です。
    Quaternion(float x, float y, float z, float w) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 次元ベクトルです。
    explicit Quaternion(const Float4& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 次元ベクトルです。
    NN_IMPLICIT Quaternion(const Vector4fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 成分を取得します。
    //! @return X 成分を返します。
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
    //! @pre 引数はすべて非 NULL であることが必須です。
    void Get(float* pOutX, float* pOutY, float* pOutZ, float* pOutW) const NN_NOEXCEPT;

    //! @brief X 成分を設定します。
    //! @param[in] value 設定する X 成分の値です。
    void SetX(float value) NN_NOEXCEPT;

    //! @brief Y 成分を設定します。
    //! @param[in] value 設定する Y 成分の値です。
    void SetY(float value) NN_NOEXCEPT;

    //! @brief Z 成分を設定します。
    //! @param[in] value 設定する Z 成分の値です。
    void SetZ(float value) NN_NOEXCEPT;

    //! @brief W 成分を設定します。
    //! @param[in] value 設定する W 成分の値です。
    void SetW(float value) NN_NOEXCEPT;

    //! @brief 各成分の値を設定します。
    //! @param[in] x 設定する X 成分。
    //! @param[in] y 設定する Y 成分。
    //! @param[in] z 設定する Z 成分。
    //! @param[in] w 設定する W 成分。
    void Set(float x, float y, float z, float w) NN_NOEXCEPT;

    //! @}
    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定する値を返します。
    Quaternion operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    Quaternion operator - () const NN_NOEXCEPT;

    //! @brief 2 つのクォータニオンの和を求めます。
    //! @param[in] value 加算する値です。
    //! @return 2 つのクォータニオンの和を返します。
    Quaternion operator + (const Quaternion& value) const NN_NOEXCEPT;

    //! @brief 2 つのクォータニオンの差を求めます。
    //! @param[in] value 減算する値です。
    //! @return 2 つのクォータニオンの差を返します。
    Quaternion operator - (const Quaternion& value) const NN_NOEXCEPT;

    //! @brief 2 つのクォータニオンの積を求めます。
    //! @param[in] value 乗算する値です。
    //! @return 2 つのクォータニオンの積を返します。
    Quaternion operator * (const Quaternion& value) const NN_NOEXCEPT;

    //! @brief 2 つのクォータニオンの商を求めます。
    //! @param[in] value 除算する値です。
    //! @return 2 つのクォータニオンの商を返します。
    Quaternion operator / (const Quaternion& value) const NN_NOEXCEPT;

    //! @brief 加算代入演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return クォータニオンを加算した結果を返します。
    Quaternion& operator += (const Quaternion& value) NN_NOEXCEPT;

    //! @brief 減算代入演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return クォータニオンを減算した結果を返します。
    Quaternion& operator -= (const Quaternion& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return クォータニオンを乗算した結果を返します。
    Quaternion& operator *= (const Quaternion& value) NN_NOEXCEPT;

    //! @brief 除算代入演算子のオーバーロードです。
    //! @param[in] value 除算する値です。
    //! @return クォータニオンを除算した結果を返します。
    Quaternion& operator /= (const Quaternion& value) NN_NOEXCEPT;

    //! @}

    //! @brief 共役なクォータニオンを設定します。
    void Conjugate() NN_NOEXCEPT;

    //! @brief 逆クォータニオンを設定します。
    void Inverse() NN_NOEXCEPT;

    //! @brief ベクトルを回転させます。
    //! @param[in] value 回転させるベクトル。
    //! @return ベクトルを回転させた結果を返します。
    Vector3f Rotate(const Vector3f& value) const NN_NOEXCEPT;

    //! @brief クォータニオンを正規化します。
    //! @return 正規化に成功したら true を返します。
    //! @details 指定クォータニオンの長さの 2 乗が 0.00001f 未満の場合、
    //!  結果の格納先にゼロクォータニオンが書き込まれます。
    bool Normalize() NN_NOEXCEPT;

    //! @brief 内積を求めます。
    //! @param[in] value ベクトル。
    //! @return 内積を計算した結果を返します。
    float Dot(const Quaternion& value) const NN_NOEXCEPT;

    //! @brief クォータニオンの長さを求めます。
    //! @return クォータニオンの長さを返します。
    float Length() const NN_NOEXCEPT;

    //! @brief クォータニオンの長さの 2 乗値を求めます。
    //! @return クォータニオンの長さの 2 乗値を返します。
    float LengthSquared() const NN_NOEXCEPT;

    //! @brief オイラー角 (x-y-z) を計算により求めます。
    //! @return オイラー角 (x-y-z) をラジアン単位で [-π/2, π/2] の範囲で返します。
    Vector3f ToAxisAngle() const NN_NOEXCEPT;

    //! @brief 内積を求めます。
    //! @param[in] a クォータニオン 1
    //! @param[in] b クォータニオン 2
    //! @return 内積を計算した結果を返します。
    static float Dot(const Quaternion& a, const Quaternion& b) NN_NOEXCEPT;

    //! @brief クォータニオンを正規化します。
    //! @param[in] value 正規化を行うクォータニオン。
    //! @return 正規化した結果を返します。
    //! @details 指定クォータニオンがゼロクォータニオンの場合、ゼロクォータニオンを返します。
    static Quaternion Normalize(const Quaternion& value) NN_NOEXCEPT;

    //! @brief 逆クォータニオンを求めます。
    //! @param[in] value 逆クォータニオンを求める値。
    //! @return 逆クォータニオンを返します。
    //! @details 指定クォータニオンがゼロクォータニオンの場合、ゼロクォータニオンを返します。
    static Quaternion Inverse(const Quaternion& value) NN_NOEXCEPT;

    //! @brief 2 つのクォータニオンを線形補間します。
    //! @param[in] a クォータニオン 1
    //! @param[in] b クォータニオン 2
    //! @param[in] t 補間係数
    //! @return 2つのクォータニオンを線形補間した結果 (a + (b - a) * t の計算結果 ) を返します。
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) NN_NOEXCEPT;

    //! @brief 2 つのクォータニオンを球面線形補間します。
    //! @param[in] a クォータニオン 1
    //! @param[in] b クォータニオン 2
    //! @param[in] t 補間係数
    //! @return 2つのクォータニオンを球面線形補間した結果を返します。
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) NN_NOEXCEPT;

    //! @brief 球面 3 次補間をします。
    //! @param[in] a クォータニオン 1
    //! @param[in] b クォータニオン 2
    //! @param[in] c クォータニオン 3
    //! @param[in] d クォータニオン 4
    //! @param[in] t 補間係数
    //! @return 球面 3 次補間した結果を返します。
    static Quaternion Squad(const Quaternion& a, const Quaternion& b, const Quaternion& c, const Quaternion& d, float t) NN_NOEXCEPT;

    //! @brief ヨー・ピッチ・ロール角からクォータニオンを生成します。
    //! @param[in] yaw Y 方向周りの回転を表すヨー角(ラジアン単位)。
    //! @param[in] pitch X 方向周りの回転を表すピッチ角(ラジアン単位)。
    //! @param[in] roll Z 方向周りの回転を表すロール角(ラジアン単位)。
    //! @return 指定されたヨー・ピッチ・ロール角から生成したクォータニオンを返します。
    static Quaternion FromYawPitchRoll(float yaw, float pitch, float roll) NN_NOEXCEPT;

    //! @brief 回転行列からクォータニオンを生成します。
    //! @param[in] value 回転行列です。
    //! @return 指定された回転行列から生成されたクォータニオンを返します。
    static Quaternion FromRotationMatrix(const MatrixRowMajor4x3f& value) NN_NOEXCEPT;

    //! @brief 指定された軸と角度からクォータニオンを生成します。
    //! @param[in] axis 回転軸。
    //! @param[in] radian 回転角（ラジアン単位）。
    //! @return 指定軸と角度から生成されたクォータニオンを返します。
    static Quaternion FromAxisAngle(const Vector3f& axis, float radian) NN_NOEXCEPT;

    //! @brief 第1ベクトルを第2ベクトルに変換するクォータニオンを生成します。
    //! @param[in] from 第 1 ベクトル
    //! @param[in] to 第 2 ベクトル
    //! @relates 第1ベクトルを第2ベクトルに変換するクォータニオンを返します。
    static Quaternion MakeVectorRotation(const Vector3f& from, const Vector3f& to) NN_NOEXCEPT;

    //! @name 定数
    //! @{

    //! @brief 単位クォータニオンです。
    //! @return W 成分が 1 で，その他の成分が 0 の単位クォータニオンを返します。
    static const Quaternion& Identity() NN_NOEXCEPT;

    //! @}

private:
    static const Vector4fType  ConstantIdentity;
};

}
}

#include <nn/util/detail/util_QuaternionImpl.h>
