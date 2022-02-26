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

/**
* @file
* @brief    行列計算を提供します。
*/

#include <nn/util/util_MatrixApi.h>
#include <nn/util/util_Arithmetic.h>

namespace nn
{
namespace util
{

class Vector2f;
class Vector3f;
class Vector4f;
class Quaternion;
class MatrixRowMajor3x2f;
class MatrixRowMajor4x3f;
class MatrixRowMajor4x4f;

// 行列クラスの別名

//! @brief    単精度浮動小数点型による 3 行 2 列の行列を行優先で表現するクラスです。
//!
//! @details  ベクトル・行列計算を提供する型です。
//!
//!  nn::util::MatrixRowMajor3x2fType を継承し、メソッドを追加するユーティリティクラスです。
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
typedef MatrixRowMajor3x2f Matrix3x2f;

//! @brief    単精度浮動小数点型による 4 行 3 列の行列を行優先で表現するクラスです。
//!
//! @details  ベクトル・行列計算を提供する型です。
//!
//!  nn::util::MatrixRowMajor4x3fType を継承し、メソッドを追加するユーティリティクラスです。
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
typedef MatrixRowMajor4x3f Matrix4x3f;

//! @brief    単精度浮動小数点型による 4 行 4 列の行列を行優先で表現するクラスです。
//!
//! @details  ベクトル・行列計算を提供する型です。
//!
//!  nn::util::MatrixRowMajor4x4fType を継承し、メソッドを追加するユーティリティクラスです。
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
typedef MatrixRowMajor4x4f Matrix4x4f;

//! @brief    単精度浮動小数点型による 3 行 2 列の行列を行優先で表現するクラスです。
//!
//! @details  ベクトル・行列計算を提供する型です。
//!
//!  nn::util::MatrixRowMajor3x2fType を継承し、メソッドを追加するユーティリティクラスです。
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
class MatrixRowMajor3x2f : public MatrixRowMajor3x2fType
{
public:
    //! @brief コンストラクタです。
    MatrixRowMajor3x2f() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] m00 1 行 1 列成分です。
    //! @param[in] m01 1 行 2 列成分です。
    //! @param[in] m10 2 行 1 列成分です。
    //! @param[in] m11 2 行 2 列成分です。
    //! @param[in] m20 3 行 1 列成分です。
    //! @param[in] m21 3 行 2 列成分です。
    MatrixRowMajor3x2f(
        float m00, float m01,
        float m10, float m11,
        float m20, float m21) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] axisX X 軸（ 1 行目）成分です。
    //! @param[in] axisY Y 軸（ 2 行目）成分です。
    //! @param[in] axisZ Z 軸（ 3 行目）成分です。
    MatrixRowMajor3x2f(
        const Vector2f& axisX,
        const Vector2f& axisY,
        const Vector2f& axisZ) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 3 行 2 列行列です。
    explicit MatrixRowMajor3x2f(const FloatRowMajor3x2& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 3 行 2 列行列です。
    NN_IMPLICIT MatrixRowMajor3x2f(const MatrixRowMajor3x2fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 軸（ 1 行目）成分を取得します。
    //! @return X軸成分を返します。
    Vector2f GetAxisX() const NN_NOEXCEPT;

    //! @brief Y 軸（ 2 行目）成分を取得します。
    //! @return Y軸成分を返します。
    Vector2f GetAxisY() const NN_NOEXCEPT;

    //! @brief Z 軸（ 3 行目）成分を取得します。
    //! @return Z 軸成分を返します。
    Vector2f GetAxisZ() const NN_NOEXCEPT;

    //! @brief 各軸成分を取得します。
    //! @param[out] pOutX X 軸（ 1 行目）成分の格納先。
    //! @param[out] pOutY Y 軸（ 2 行目）成分の格納先。
    //! @param[out] pOutZ Z 軸（ 3 行目）成分の格納先。
    void Get(Vector2f* pOutX, Vector2f* pOutY, Vector2f* pOutZ) const NN_NOEXCEPT;

    //! @brief X 軸（ 1 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisX(const Vector2f& value) NN_NOEXCEPT;

    //! @brief Y 軸（ 2 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisY(const Vector2f& value) NN_NOEXCEPT;

    //! @brief Z 軸（ 3 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisZ(const Vector2f& value) NN_NOEXCEPT;

    //! @brief 各軸成分を設定します。
    //! @param[in] axisX X 軸（ 1 行目）成分です。
    //! @param[in] axisY Y 軸（ 2 行目）成分です。
    //! @param[in] axisZ Z 軸（ 3 行目）成分です。
    void Set(
        const Vector2f& axisX,
        const Vector2f& axisY,
        const Vector2f& axisZ) NN_NOEXCEPT;

    //! @brief 拡大・縮小成分を設定します。
    //! @param[in] value 設定するスケール値。
    //! @details 無回転のスケール行列を設定します。平行移動成分は操作しません。
    void SetScale(const Vector2f& value) NN_NOEXCEPT;

    //! @brief 平行移動成分を設定します。
    //! @param[in] value 設定する平行移動量。
    //! @details 平行移動成分以外の成分は操作しません。
    void SetTranslate(const Vector2f& value) NN_NOEXCEPT;

    //! @brief オイラー角から回転成分を設定します。
    //! @param[in] value 設定するオイラー角(ラジアン単位)。
    //! @details スケールなしの回転行列を設定します。平行移動成分は操作しません。
    void SetRotate(float value) NN_NOEXCEPT;

    //! @}

    //! @brief 行列を転置します。
    //! @details 2x2行列とみなして転置処理が行われ，3 行目は全て 0 が設定されます。
    void Transpose() NN_NOEXCEPT;

    //! @brief 逆行列を設定します。
    //! @return 逆行列が設定できる場合は true を返却します。
    //! @details 逆行列が存在しない場合、ゼロ行列を設定します。
    bool Inverse() NN_NOEXCEPT;

    //! @brief 逆行列の転置を設定します。
    //! @return 逆行列の転置が設定できる場合は true を返却します。
    //! @details 逆行列が存在しない場合、ゼロ行列を設定します。
    bool InverseTranspose() NN_NOEXCEPT;

    //! @brief ベクトルを行列で変換します。
    //! @param[in] value 変換するベクトル。
    //! @return ベクトルを行列で変換した結果を返します。
    //! @details 指定ベクトルに対し、行列を右から掛けます。
    Vector2f Transform(const Vector2f& value) const NN_NOEXCEPT;

    //! @brief 法線ベクトルを行列で変換します。
    //! @param[in] value 変換する法線ベクトル。
    //! @return 法線ベクトルを行列で変換した結果を返します。
    //! @details 指定ベクトルに対し、行列を右から掛けます。
    Vector2f TransformNormal(const Vector2f& value) const NN_NOEXCEPT;


    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定されている値を返します。
    MatrixRowMajor3x2f operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    MatrixRowMajor3x2f operator - () const NN_NOEXCEPT;

    //! @brief 加算演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return 行列同士の加算結果を返します。
    MatrixRowMajor3x2f operator + (const MatrixRowMajor3x2f& value) const NN_NOEXCEPT;

    //! @brief 減算演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return 行列同士の減算結果を返します。
    MatrixRowMajor3x2f operator - (const MatrixRowMajor3x2f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 行列同士の乗算結果を返します。
    MatrixRowMajor3x2f operator * (const MatrixRowMajor3x2f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算した結果を返します。
    MatrixRowMajor3x2f operator * (float value) const NN_NOEXCEPT;

    //! @brief 除算演算子のオーバーロードです。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    MatrixRowMajor3x2f operator / (float value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] scalar 乗算するスカラー値です。
    //! @param[in] matrix 行列です。
    //! @return スカラー乗算した結果を返します。
    friend MatrixRowMajor3x2f operator * (float scalar, const MatrixRowMajor3x2f& matrix) NN_NOEXCEPT;

    //! @brief 加算代入演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return 行列同士を加算した結果を返します。
    MatrixRowMajor3x2f& operator += (const MatrixRowMajor3x2f& value) NN_NOEXCEPT;

    //! @brief 減算代入演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return 行列同士を減算した結果を返します。
    MatrixRowMajor3x2f& operator -= (const MatrixRowMajor3x2f& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 行列同士を乗算した結果を返します。
    MatrixRowMajor3x2f& operator *= (const MatrixRowMajor3x2f& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算した結果を返します。
    MatrixRowMajor3x2f& operator *= (float value) NN_NOEXCEPT;

    //! @brief 除算代入演算子のオーバーロードです。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    MatrixRowMajor3x2f& operator /= (float value) NN_NOEXCEPT;

    //! @}
    //! @name 生成処理
    //! @{

    //! @brief 平行移動行列を生成します。
    //! @param[in] value 平行移動量。
    //! @return 平行移動行列を返します。
    static MatrixRowMajor3x2f MakeTranslation(const Vector2f& value) NN_NOEXCEPT;

    //! @brief 拡大・縮小行列を生成します。
    //! @param[in] value スケール値。
    //! @return 拡大・縮小行列を返します。
    static MatrixRowMajor3x2f MakeScale(const Vector2f& value) NN_NOEXCEPT;

    //! @brief 回転行列を生成します。
    //! @param[in] radian 回転角（ラジアン単位）。
    //! @return 回転行列を返します。
    static MatrixRowMajor3x2f MakeRotation(float radian) NN_NOEXCEPT;

    //! @brief 転置行列を生成します。
    //! @param[in] value 転置対象の行列。
    //! @return 転置行列を返します。
    static MatrixRowMajor3x2f MakeTranspose(const MatrixRowMajor3x2f& value) NN_NOEXCEPT;

    //! @}
    //! @name 定数
    //! @{

    //! @brief 零行列です。
    //! @return 全ての成分が 0 の行列を返します。
    static const MatrixRowMajor3x2f& Zero() NN_NOEXCEPT;

    //! @brief 単位行列です。
    //! @return 対角成分が 1 で，その他の成分が 0 の行列を返します。
    static const MatrixRowMajor3x2f& Identity() NN_NOEXCEPT;

    //! @}

private:
    static const MatrixRowMajor3x2fType ConstantZero;       //!< 零行列です。
    static const MatrixRowMajor3x2fType ConstantIdentity;   //!< 単位行列です。
};

//! @brief    単精度浮動小数点型による 4 行 3 列の行列を行優先で表現するクラスです。
//!
//! @details  ベクトル・行列計算を提供する型です。
//!
//!  nn::util::MatrixRowMajor4x3fType を継承し、メソッドを追加するユーティリティクラスです。
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
class MatrixRowMajor4x3f : public MatrixRowMajor4x3fType
{
public:
    //! @brief コンストラクタです。
    MatrixRowMajor4x3f() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] m00 1 行 1 列成分です。
    //! @param[in] m01 1 行 2 列成分です。
    //! @param[in] m02 1 行 3 列成分です。
    //! @param[in] m10 2 行 1 列成分です。
    //! @param[in] m11 2 行 2 列成分です。
    //! @param[in] m12 2 行 3 列成分です。
    //! @param[in] m20 3 行 1 列成分です。
    //! @param[in] m21 3 行 2 列成分です。
    //! @param[in] m22 3 行 3 列成分です。
    //! @param[in] m30 4 行 1 列成分です。
    //! @param[in] m31 4 行 2 列成分です。
    //! @param[in] m32 4 行 3 列成分です。
    MatrixRowMajor4x3f(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22,
        float m30, float m31, float m32) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] axisX X 軸（ 1 行目）成分です。
    //! @param[in] axisY Y 軸（ 2 行目）成分です。
    //! @param[in] axisZ Z 軸（ 3 行目）成分です。
    //! @param[in] axisW W 軸（ 4 行目）成分です。
    MatrixRowMajor4x3f(
        const Vector3f& axisX,
        const Vector3f& axisY,
        const Vector3f& axisZ,
        const Vector3f& axisW) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 行 3 列行列です。
    explicit MatrixRowMajor4x3f(const FloatRowMajor4x3& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 行 3 列行列です。
    NN_IMPLICIT MatrixRowMajor4x3f(const MatrixRowMajor4x3fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 軸（ 1 行目）成分を取得します。
    //! @return X軸成分を返します。
    Vector3f GetAxisX() const NN_NOEXCEPT;

    //! @brief Y 軸（ 2 行目）成分を取得します。
    //! @return Y軸成分を返します。
    Vector3f GetAxisY() const NN_NOEXCEPT;

    //! @brief Z 軸（ 3 行目）成分を取得します。
    //! @return Z 軸成分を返します。
    Vector3f GetAxisZ() const NN_NOEXCEPT;

    //! @brief W 軸（ 4 行目）成分を取得します。
    //! @return W軸成分を返します。
    Vector3f GetAxisW() const NN_NOEXCEPT;

    //! @brief 各軸成分を取得します。
    //! @param[out] pOutX X 軸（ 1 行目）成分の格納先。
    //! @param[out] pOutY Y 軸（ 2 行目）成分の格納先。
    //! @param[out] pOutZ Z 軸（ 3 行目）成分の格納先。
    //! @param[out] pOutW W 軸（ 4 行目）成分の格納先。
    void Get(Vector3f* pOutX, Vector3f* pOutY, Vector3f* pOutZ, Vector3f* pOutW) const NN_NOEXCEPT;

    //! @brief X 軸（ 1 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisX(const Vector3f& value) NN_NOEXCEPT;

    //! @brief Y 軸（ 2 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisY(const Vector3f& value) NN_NOEXCEPT;

    //! @brief Z 軸（ 3 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisZ(const Vector3f& value) NN_NOEXCEPT;

    //! @brief W 軸（ 4 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisW(const Vector3f& value) NN_NOEXCEPT;

    //! @brief 各軸成分を設定します。
    //! @param[in] axisX X 軸（ 1 行目）成分です。
    //! @param[in] axisY Y 軸（ 2 行目）成分です。
    //! @param[in] axisZ Z 軸（ 3 行目）成分です。
    //! @param[in] axisW W 軸（ 4 行目）成分です。
    void Set(
        const Vector3f& axisX,
        const Vector3f& axisY,
        const Vector3f& axisZ,
        const Vector3f& axisW) NN_NOEXCEPT;

    //! @brief 拡大・縮小成分を設定します。
    //! @param[in] value 設定するスケール値。
    //! @details 無回転のスケール行列を設定します。平行移動成分は操作しません。
    void SetScale(const Vector3f& value) NN_NOEXCEPT;

    //! @brief 平行移動成分を設定します。
    //! @param[in] value 設定する平行移動量。
    //! @details 平行移動成分以外の成分は操作しません。
    void SetTranslate(const Vector3f& value) NN_NOEXCEPT;

    //! @brief クォータニオンから回転成分を設定します。
    //! @param[in] value 設定するクォータニオン。
    //! @details スケールなしの回転行列を設定します。平行移動成分は操作しません。
    //!  また、指定クォータニオンを正規化しません。
    void SetRotate(const Quaternion& value) NN_NOEXCEPT;

    //! @brief オイラー角から回転成分を設定します。
    //! @param[in] value 設定するオイラー角(ラジアン単位)。
    //! @details スケールなしの回転行列を設定します。平行移動成分は操作しません。
    void SetRotate(const Vector3f& value) NN_NOEXCEPT;

    //! @}

    //! @brief 行列を転置します。
    //! @details 3x3行列とみなして転置処理が行われ，4 行目は全て 0 が設定されます。
    void Transpose() NN_NOEXCEPT;

    //! @brief 逆行列を設定します。
    //! @return 逆行列が設定できる場合は true を返却します。
    //! @details 逆行列が存在しない場合、ゼロ行列を設定します。
    bool Inverse() NN_NOEXCEPT;

    //! @brief 逆行列の転置を設定します。
    //! @return 逆行列の転置が設定できる場合は true を返却します。
    //! @details 逆行列が存在しない場合、ゼロ行列を設定します。
    bool InverseTranspose() NN_NOEXCEPT;

    //! @brief ベクトルを行列で変換します。
    //! @param[in] value 変換するベクトル。
    //! @return ベクトルを行列で変換した結果を返します。
    //! @details 指定ベクトルに対し、行列を右から掛けます。
    Vector3f Transform(const Vector3f& value) const NN_NOEXCEPT;

    //! @brief 法線ベクトルを行列で変換します。
    //! @param[in] value 変換する法線ベクトル。
    //! @return 法線ベクトルを行列で変換した結果を返します。
    //! @details 指定ベクトルに対し、行列を右から掛けます。
    Vector3f TransformNormal(const Vector3f& value) const NN_NOEXCEPT;


    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定されている値を返します。
    MatrixRowMajor4x3f operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    MatrixRowMajor4x3f operator - () const NN_NOEXCEPT;

    //! @brief 加算演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return 行列同士の加算結果を返します。
    MatrixRowMajor4x3f operator + (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT;

    //! @brief 減算演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return 行列同士の減算結果を返します。
    MatrixRowMajor4x3f operator - (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 行列同士の乗算結果を返します。
    MatrixRowMajor4x3f operator * (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算する 4 行 4 列行列です。
    //! @return 4 行 3 列の第４列を (0, 0, 0, 1) とみなして，4 行 4 列同士の乗算結果を返します。
    MatrixRowMajor4x4f operator * (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算した結果を返します。
    MatrixRowMajor4x3f operator * (float value) const NN_NOEXCEPT;

    //! @brief 除算演算子のオーバーロードです。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    MatrixRowMajor4x3f operator / (float value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] scalar 乗算するスカラー値です。
    //! @param[in] matrix 行列です。
    //! @return スカラー乗算した結果を返します。
    friend MatrixRowMajor4x3f operator * (float scalar, const MatrixRowMajor4x3f& matrix) NN_NOEXCEPT;

    //! @brief 加算代入演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return 行列同士を加算した結果を返します。
    MatrixRowMajor4x3f& operator += (const MatrixRowMajor4x3f& value) NN_NOEXCEPT;

    //! @brief 減算代入演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return 行列同士を減算した結果を返します。
    MatrixRowMajor4x3f& operator -= (const MatrixRowMajor4x3f& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 行列同士を乗算した結果を返します。
    MatrixRowMajor4x3f& operator *= (const MatrixRowMajor4x3f& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算した結果を返します。
    MatrixRowMajor4x3f& operator *= (float value) NN_NOEXCEPT;

    //! @brief 除算代入演算子のオーバーロードです。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    MatrixRowMajor4x3f& operator /= (float value) NN_NOEXCEPT;

    //! @}
    //! @name 生成処理
    //! @{

    //! @brief 平行移動行列を生成します。
    //! @param[in] value 平行移動量。
    //! @return 平行移動行列を返します。
    static MatrixRowMajor4x3f MakeTranslation(const Vector3f& value) NN_NOEXCEPT;

    //! @brief 拡大・縮小行列を生成します。
    //! @param[in] value スケール値。
    //! @return 拡大・縮小行列を返します。
    static MatrixRowMajor4x3f MakeScale(const Vector3f& value) NN_NOEXCEPT;

    //! @brief 回転行列を生成します。
    //! @param[in] radian 各軸周りの回転角（ラジアン単位）。
    //! @return 回転行列を返します。
    static MatrixRowMajor4x3f MakeRotation(const Vector3f& radian) NN_NOEXCEPT;

    //! @brief クォータニオンから回転行列を生成します.
    //! @param[in] value クォータニオン。
    //! @return 回転行列を返します。
    //! @details 指定クォータニオンを正規化しません。
    static MatrixRowMajor4x3f MakeRotation(const Quaternion& value) NN_NOEXCEPT;

    //! @brief 転置行列を生成します。
    //! @param[in] value 転置対象の行列。
    //! @return 転置行列を返します。
    static MatrixRowMajor4x3f MakeTranspose(const MatrixRowMajor4x3f& value) NN_NOEXCEPT;

    //! @brief 注視点を基にビュー行列を生成します（右手系）。
    //! @param[in] position 位置座標。
    //! @param[in] target 注視点。
    //! @param[in] upward 上向きベクトル。
    //! @return ビュー行列を返します。
    static MatrixRowMajor4x3f LookAtRightHanded(const Vector3f& position, const Vector3f& target, const Vector3f& upward) NN_NOEXCEPT;

    //! @brief 視線ベクトルを基にビュー行列を生成します（右手系）。
    //! @param[in] position 位置座標。
    //! @param[in] direction 視線ベクトル。
    //! @param[in] upward 上向きベクトル。
    //! @return ビュー行列を返します。
    static MatrixRowMajor4x3f LookToRightHanded(const Vector3f& position, const Vector3f& direction, const Vector3f& upward) NN_NOEXCEPT;

    //! @brief 4 行 4 列列優先行列に変換します。
    //! @param[in] value 変換する行列です。
    //! @return 4 行 4 列列優先行列を返します。
    static MatrixRowMajor4x4f ToMatrix4x4f(const MatrixRowMajor4x3f& value) NN_NOEXCEPT;

    //! @}
    //! @name 定数
    //! @{

    //! @brief 零行列です。
    //! @return 全ての成分が 0 の行列を返します。
    static const MatrixRowMajor4x3f& Zero() NN_NOEXCEPT;

    //! @brief 単位行列です。
    //! @return 対角成分が 1 で，その他の成分が 0 の行列を返します。
    static const MatrixRowMajor4x3f& Identity() NN_NOEXCEPT;

    //! @}

private:
    static const MatrixRowMajor4x3fType ConstantZero;       //!< 零行列です。
    static const MatrixRowMajor4x3fType ConstantIdentity;   //!< 単位行列です。
};


//! @brief    単精度浮動小数点型による 4 行 4 列の行列を行優先で表現するクラスです。
//!
//! @details  ベクトル・行列計算を提供する型です。
//!
//!  nn::util::MatrixRowMajor4x4fType を継承し、メソッドを追加するユーティリティクラスです。
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
class MatrixRowMajor4x4f : public MatrixRowMajor4x4fType
{
public:
    //! @brief コンストラクタです。
    MatrixRowMajor4x4f() NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] m00 1 行 1 列成分です。
    //! @param[in] m01 1 行 2 列成分です。
    //! @param[in] m02 1 行 3 列成分です。
    //! @param[in] m03 1 行 4 列成分です。
    //! @param[in] m10 2 行 1 列成分です。
    //! @param[in] m11 2 行 2 列成分です。
    //! @param[in] m12 2 行 3 列成分です。
    //! @param[in] m13 2 行 4 列成分です。
    //! @param[in] m20 3 行 1 列成分です。
    //! @param[in] m21 3 行 2 列成分です。
    //! @param[in] m22 3 行 3 列成分です。
    //! @param[in] m23 3 行 4 列成分です。
    //! @param[in] m30 4 行 1 列成分です。
    //! @param[in] m31 4 行 2 列成分です。
    //! @param[in] m32 4 行 3 列成分です。
    //! @param[in] m33 4 行 4 列成分です。
    MatrixRowMajor4x4f(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] axisX X 軸（ 1 行目）成分です。
    //! @param[in] axisY Y 軸（ 2 行目）成分です。
    //! @param[in] axisZ Z 軸（ 3 行目）成分です。
    //! @param[in] axisW W 軸（ 4 行目）成分です。
    MatrixRowMajor4x4f(
        const Vector4f& axisX,
        const Vector4f& axisY,
        const Vector4f& axisZ,
        const Vector4f& axisW) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 行 4 列行列です。
    explicit MatrixRowMajor4x4f(const FloatRowMajor4x4& value) NN_NOEXCEPT;

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value 4 行 4 列行列です。
    NN_IMPLICIT MatrixRowMajor4x4f(const MatrixRowMajor4x4fType& value) NN_NOEXCEPT;

    //----------------------------------------
    //! @name 取得／設定
    //! @{

    //! @brief X 軸（ 1 行目）成分を取得します。
    //! @return X 軸成分を返します。
    Vector4f GetAxisX() const NN_NOEXCEPT;

    //! @brief Y 軸（ 2 行目）成分を取得します。
    //! @return Y 軸成分を返します。
    Vector4f GetAxisY() const NN_NOEXCEPT;

    //! @brief Z 軸（ 3 行目）成分を取得します。
    //! @return Z 軸成分を返します。
    Vector4f GetAxisZ() const NN_NOEXCEPT;

    //! @brief W 軸（ 4 行目）成分を取得します。
    //! @return W 軸成分を返します。
    Vector4f GetAxisW() const NN_NOEXCEPT;

    //! @brief 各軸成分を取得します。
    //! @param[out] pOutX X 軸（ 1 行目）成分の格納先。
    //! @param[out] pOutY Y 軸（ 2 行目）成分の格納先。
    //! @param[out] pOutZ Z 軸（ 3 行目）成分の格納先。
    //! @param[out] pOutW W 軸（ 4 行目）成分の格納先。
    void Get(Vector4f* pOutX, Vector4f* pOutY, Vector4f* pOutZ, Vector4f* pOutW) const NN_NOEXCEPT;

    //! @brief X 軸（ 1 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisX(const Vector4f& value) NN_NOEXCEPT;

    //! @brief Y 軸（ 2 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisY(const Vector4f& value) NN_NOEXCEPT;

    //! @brief Z 軸（ 3 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisZ(const Vector4f& value) NN_NOEXCEPT;

    //! @brief W 軸（ 4 行目）成分を設定します。
    //! @param[in] value 設定する値。
    void SetAxisW(const Vector4f& value) NN_NOEXCEPT;

    //! @brief 各軸成分を設定します。
    //! @param[in] axisX X 軸（ 1 行目）成分です。
    //! @param[in] axisY Y 軸（ 2 行目）成分です。
    //! @param[in] axisZ Z 軸（ 3 行目）成分です。
    //! @param[in] axisW W 軸（ 4 行目）成分です。
    void Set(
        const Vector4f& axisX,
        const Vector4f& axisY,
        const Vector4f& axisZ,
        const Vector4f& axisW) NN_NOEXCEPT;

    //! @}

    //! @brief 行列を転置します。
    void Transpose() NN_NOEXCEPT;

    //! @brief 逆行列を設定します。
    //! @return 逆行列が設定できる場合は true を返却します。
    //! @details 逆行列が存在しない場合、ゼロ行列を設定します。
    bool Inverse() NN_NOEXCEPT;

    //! @brief 逆行列の転置を設定します。
    //! @return 逆行列の転置が設定できる場合は true を返却します。
    //! @details 逆行列が存在しない場合、ゼロ行列を設定します。
    bool InverseTranspose() NN_NOEXCEPT;

    //! @brief ベクトルを行列で変換します。
    //! @param[in] value 変換するベクトル。
    //! @return ベクトルを行列で変換した結果を返します。
    //! @details 指定ベクトルに対し、行列を右から掛けます。
    Vector4f Transform(const Vector4f& value) const NN_NOEXCEPT;

    //! @brief ベクトルを行列で変換し， w = 1 に射影します。
    //! @param[in] value 変換するベクトル。
    //! @return ベクトルを行列で変換し，w = 1 に射影した結果を返します。
    //! @details 指定ベクトルに対し、行列を右から掛けます。
    Vector3f TransformCoord(const Vector3f& value) const NN_NOEXCEPT;

    //! @name 演算子
    //! @{

    //! @brief 正符号演算子のオーバーロードです。
    //! @return 設定されている値を返します。
    MatrixRowMajor4x4f operator + () const NN_NOEXCEPT;

    //! @brief 負符号演算子のオーバーロードです。
    //! @return 各成分に負符号をつけた値を返します。
    MatrixRowMajor4x4f operator - () const NN_NOEXCEPT;

    //! @brief 加算演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return 行列同士の加算結果を返します。
    MatrixRowMajor4x4f operator + (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT;

    //! @brief 減算演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return 行列同士の減算結果を返します。
    MatrixRowMajor4x4f operator - (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 行列同士の乗算結果を返します。
    MatrixRowMajor4x4f operator * (const MatrixRowMajor4x4f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 4 行 3 列の第４列を (0, 0, 0, 1) とみなして，4 行 4 列同士の乗算結果を返します。
    MatrixRowMajor4x4f operator * (const MatrixRowMajor4x3f& value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算した結果を返します。
    MatrixRowMajor4x4f operator * (float value) const NN_NOEXCEPT;

    //! @brief 除算演算子のオーバーロードです。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    MatrixRowMajor4x4f operator / (float value) const NN_NOEXCEPT;

    //! @brief 乗算演算子のオーバーロードです。
    //! @param[in] scalar 乗算するスカラー値です。
    //! @param[in] matrix 行列です。
    //! @return スカラー乗算した結果を返します。
    friend MatrixRowMajor4x4f operator * (float scalar, const MatrixRowMajor4x4f& matrix) NN_NOEXCEPT;

    //! @brief 加算代入演算子のオーバーロードです。
    //! @param[in] value 加算する値です。
    //! @return 行列同士を加算した結果を返します。
    MatrixRowMajor4x4f& operator += (const MatrixRowMajor4x4f& value) NN_NOEXCEPT;

    //! @brief 減算代入演算子のオーバーロードです。
    //! @param[in] value 減算する値です。
    //! @return 行列同士を減算した結果を返します。
    MatrixRowMajor4x4f& operator -= (const MatrixRowMajor4x4f& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算する値です。
    //! @return 行列同士を乗算した結果を返します。
    MatrixRowMajor4x4f& operator *= (const MatrixRowMajor4x4f& value) NN_NOEXCEPT;

    //! @brief 乗算代入演算子のオーバーロードです。
    //! @param[in] value 乗算するスカラー値です。
    //! @return スカラー乗算した結果を返します。
    MatrixRowMajor4x4f& operator *= (float value) NN_NOEXCEPT;

    //! @brief 除算代入演算子のオーバーロードです。
    //! @param[in] value 除算するスカラー値です。
    //! @return スカラー除算した結果を返します。
    //! @pre 除算するスカラー値は絶対値が計算機イプシロンより大きいことが必須です。
    MatrixRowMajor4x4f& operator /= (float value) NN_NOEXCEPT;

    //! @}
    //! @name 生成処理
    //! @{

    //! @brief 転置行列を生成します。
    //! @param[in] value 転置対象の行列。
    //! @return 転置行列を返します。
    static MatrixRowMajor4x4f MakeTranspose(const MatrixRowMajor4x4f& value) NN_NOEXCEPT;

    //! @brief 透視投影行列を生成します。
    //! @param[in] width ビューボリュームの横幅。
    //! @param[in] height ビューボリュームの縦幅。
    //! @param[in] nearClip 近クリップ平面までの距離。
    //! @param[in] farClip 遠クリップ平面までの距離。
    //! @return 透視投影行列を返します。
    //! @pre width, height, (farClip - nearClip) の値の絶対値が計算機イプシロンより大きいことが必須です。
    //! @details 深度値の幅を [0, 1] として行列を求めます。
    //!  グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
    static MatrixRowMajor4x4f PerspectiveRightHanded(float width, float height, float nearClip, float farClip) NN_NOEXCEPT;

    //! @brief 視野角に基づいて透視投影行列を生成します。
    //! @param[in] fieldOfView 垂直視野角（ラジアン単位)。
    //! @param[in] aspectRatio アスペクト比。
    //! @param[in] nearClip 近クリップ平面までの距離。
    //! @param[in] farClip 遠クリップ平面までの距離。
    //! @return 透視投影行列を返します。
    //! @details 深度値の幅を [0, 1] として行列を求めます。
    //!  グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
    static MatrixRowMajor4x4f PerspectiveFieldOfViewRightHanded(float fieldOfView, float aspectRatio, float nearClip, float farClip) NN_NOEXCEPT;

    //! @brief カスタマイズした透視投影行列を生成します。
    //! @param[in] left ビューボリュームの最小 X 値。
    //! @param[in] right ビューボリュームの最大 X 値。
    //! @param[in] bottom ビューボリュームの最小 Y 値。
    //! @param[in] top ビューボリュームの最大 Y 値。
    //! @param[in] nearClip 近クリップ平面までの距離。
    //! @param[in] farClip 遠クリップ平面までの距離。
    //! @return 透視投影行列を返します。
    //! @pre (right - left), (top - bottom), (farClip - nearClip) の値の絶対値が計算機イプシロンより大きいことが必須です。
    //! @details 深度値の幅を [0, 1] として行列を求めます。
    //!  グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
    static MatrixRowMajor4x4f PerspectiveOffCenterRightHanded(float left, float right, float bottom, float top, float nearClip, float farClip) NN_NOEXCEPT;

    //! @brief 正射影行列を生成します。
    //! @param[in] width ビューボリュームの横幅。
    //! @param[in] height ビューボリュームの縦幅。
    //! @param[in] nearClip 近クリップ平面までの距離。
    //! @param[in] farClip 遠クリップ平面までの距離。
    //! @return 正射影行列を返します。
    //! @pre width, height, (farClip - nearClip) の値の絶対値が計算機イプシロンより大きいことが必須です。
    //! @details 深度値の幅を [0, 1] として行列を求めます。
    //!  グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
    static MatrixRowMajor4x4f OrthographicRightHanded(float width, float height, float nearClip, float farClip) NN_NOEXCEPT;

    //! @brief カスタマイズした正射影行列を生成します。
    //! @param[in] left ビューボリュームの最小 X 値。
    //! @param[in] right ビューボリュームの最大 X 値。
    //! @param[in] bottom ビューボリュームの最小 Y 値。
    //! @param[in] top ビューボリュームの最大 Y 値。
    //! @param[in] nearClip 近クリップ平面までの距離。
    //! @param[in] farClip 遠クリップ平面までの距離。
    //! @return 正射影行列を返します。
    //! @details 深度値の幅を [0, 1] として行列を求めます。
    //!  グラフィックス API によって、デバイスの深度値の幅を [0, 1] に設定することを推奨します。
    static MatrixRowMajor4x4f OrthographicOffCenterRightHanded(float left, float right, float bottom, float top, float nearClip, float farClip) NN_NOEXCEPT;

    //! @brief 4 行 3 列列優先行列に変換します。
    //! @param[in] value 変換する行列です。
    //! @return 4 行 3列列優先行列を返します。
    static MatrixRowMajor4x3f ToMatrix4x3f(const MatrixRowMajor4x4f& value) NN_NOEXCEPT;

    //! @}
    //! @name 定数
    //! @{

    //! @brief 零行列です。
    //! @return 全ての成分が 0 の行列を返します。
    static const MatrixRowMajor4x4f& Zero() NN_NOEXCEPT;

    //! @brief 単位行列です。
    //! @return 対角成分が 1 で，その他の成分が 0 の行列を返します。
    static const MatrixRowMajor4x4f& Identity() NN_NOEXCEPT;

    //! @}

private:
    static const MatrixRowMajor4x4fType ConstantZero;       //!< 零行列です。
    static const MatrixRowMajor4x4fType ConstantIdentity;   //!< 単位行列です。
};


}
}

#include <nn/util/detail/util_MatrixImpl.h>
