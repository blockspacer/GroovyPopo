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

#include <nn/nn_SdkAssert.h>
#include <nn/util/util_Endian.h>
#include <nn/util/util_VectorApi.h>

/**
* @file
* @brief    色の計算を提供します。
*/

namespace nn
{
namespace util
{

//! @brief 4 要素各 8 ビットの符号なし整数で表現されるカラーをサポートするクラスです。
class Color4u8 : public Color4u8Type
{
public:
    //! @brief コンストラクタです。
    //! @details 各成分の値は初期化されません。
    Color4u8() NN_NOEXCEPT
    {}

    //! @brief 引数付きコンストラクタです。
    //! @param[in]    red     カラーの R 要素
    //! @param[in]    green   カラーの G 要素
    //! @param[in]    blue    カラーの B 要素
    //! @param[in]    alpha   カラーの A 要素
    Color4u8(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = ElementMax) NN_NOEXCEPT
    {
        this->v[0] = red;
        this->v[1] = green;
        this->v[2] = blue;
        this->v[3] = alpha;
    }

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value Color4f の基底クラスの Color4fType です。
    //! @details Color4fType の各成分の値が 0.f〜1.f にクランプされた後変換されます。
    explicit Color4u8(const Color4fType& value) NN_NOEXCEPT
    {
        this->v[0] = static_cast<uint8_t>( ElementMax * Clamp( 0.f, value._v[0], 1.f ) );
        this->v[1] = static_cast<uint8_t>( ElementMax * Clamp( 0.f, value._v[1], 1.f ) );
        this->v[2] = static_cast<uint8_t>( ElementMax * Clamp( 0.f, value._v[2], 1.f ) );
        this->v[3] = static_cast<uint8_t>( ElementMax * Clamp( 0.f, value._v[3], 1.f ) );
    }

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value Color4u8 の基底クラスの Color4u8Type です。
    NN_IMPLICIT Color4u8(const Color4u8Type& value) NN_NOEXCEPT
    : Color4u8Type(value)
    {}

    //! @name 取得／設定
    //! @{

    //! @brief カラーの R 要素を取得します。
    //! @return  カラーの R 要素を返します
    uint8_t GetR() const NN_NOEXCEPT
    {
         return this->v[0];
    }

    //! @brief カラーの G 要素を取得します。
    //! @return  カラーの G 要素を返します
    uint8_t GetG() const NN_NOEXCEPT
    {
         return this->v[1];
    }

    //! @brief カラーの B 要素を取得します。
    //! @return  カラーの B 要素を返します
    uint8_t GetB() const NN_NOEXCEPT
    {
         return this->v[2];
    }

    //! @brief カラーの A 要素を取得します。
    //! @return  カラーの A 要素を返します
    uint8_t GetA() const NN_NOEXCEPT
    {
         return this->v[3];
    }

    //! @brief 各要素の値を取得します。
    //! @param[out] pOutR R 要素の格納先です。
    //! @param[out] pOutG G 要素の格納先です。
    //! @param[out] pOutB B 要素の格納先です。
    //! @param[out] pOutA A 要素の格納先です。
    //! @pre 引数は全て非 NULL であることが必須です。
    void Get(uint8_t* pOutR, uint8_t* pOutG, uint8_t* pOutB, uint8_t* pOutA) const NN_NOEXCEPT
    {
        // 各成分ごとに取り出すよりも，一気に取り出したほうがシャッフルなどが無く速いことを期待して実装。
        // TODO : 要検証（命令数比較，速度比較）
        NN_SDK_REQUIRES_NOT_NULL(pOutR);
        NN_SDK_REQUIRES_NOT_NULL(pOutG);
        NN_SDK_REQUIRES_NOT_NULL(pOutB);
        NN_SDK_REQUIRES_NOT_NULL(pOutA);
        (*pOutR) = this->v[0];
        (*pOutG) = this->v[1];
        (*pOutB) = this->v[2];
        (*pOutA) = this->v[3];
    }

    //! @brief R 要素を設定します。
    //! @param[in] red 設定する R 要素の値です。
    void SetR(uint8_t red) NN_NOEXCEPT
    {
         this->v[0] = red;
    }

    //! @brief G 要素を設定します。
    //! @param[in] green 設定する G 要素の値です。
    void SetG(uint8_t green) NN_NOEXCEPT
    {
         this->v[1] = green;
    }

    //! @brief B 要素を設定します。
    //! @param[in] blue 設定する B 要素の値です。
    void SetB(uint8_t blue) NN_NOEXCEPT
    {
         this->v[2] = blue;
    }

    //! @brief A 要素を設定します。
    //! @param[in] alpha 設定する A 要素の値です。
    void SetA(uint8_t alpha) NN_NOEXCEPT
    {
         this->v[3] = alpha;
    }

    //! @brief RGBA の値を設定します。
    //! @param[in]    red     カラーの R 要素
    //! @param[in]    green   カラーの G 要素
    //! @param[in]    blue    カラーの B 要素
    //! @param[in]    alpha   カラーの A 要素
    void Set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) NN_NOEXCEPT
    {
         this->v[0] = red;
         this->v[1] = green;
         this->v[2] = blue;
         this->v[3] = alpha;
    }

    //! @}
    //! @name 比較演算子
    //! @{

    //! @brief        2つの色の RGBA 成分が等しいかどうかを比較します。
    //! @param[in]    value   比較するカラー値です。
    //! @return       等しい場合は true を返します。
    bool operator == (const Color4u8& value) const NN_NOEXCEPT
    {
        return (this->v[0] == value.v[0] && this->v[1] == value.v[1] && this->v[2] == value.v[2] && this->v[3] == value.v[3]);
    }

    //! @brief        2つの色の RGBA 成分が等しくないかどうか比較します。
    //! @param[in]    value   比較するカラー値です。
    //! @return       異なる場合は true を返します。
    bool operator != (const Color4u8& value) const NN_NOEXCEPT
    {
        return !(*this == value);
    }

    //! @}

    //! @brief 2つの色を ratio の割合で各成分ごとに線形補間して返します。
    //! @param[in]    a   補間する色1
    //! @param[in]    b   補間する色2
    //! @param[in]    t   補間係数
    //! @return  補間後の色 ( a + (b - a) * t の計算結果) を返します。
    static Color4u8 Lerp(const Color4u8& a, const Color4u8& b, float t) NN_NOEXCEPT
    {
        Color4u8 result;
        result.SetLerp(a, b, t);
        return result;
    }

    //! @brief 2つの色を ratio の割合で各成分ごとに線形補間し、結果を this に代入します。
    //! @param[in]    a   補間する色1
    //! @param[in]    b   補間する色2
    //! @param[in]    t   補間係数
    void SetLerp(const Color4u8& a, const Color4u8& b, float t) NN_NOEXCEPT
    {
        this->Set(
            static_cast<uint8_t>(a.v[0] + t * (b.v[0] - a.v[0]) ),
            static_cast<uint8_t>(a.v[1] + t * (b.v[1] - a.v[1]) ),
            static_cast<uint8_t>(a.v[2] + t * (b.v[2] - a.v[2]) ),
            static_cast<uint8_t>(a.v[3] + t * (b.v[3] - a.v[3]) )
        );
    }

    //! @brief カラーのガンマ変換を行った結果を返します。
    //! @param[in]    gamma 補正値です。
    Color4u8 CorrectGamma(float gamma) const NN_NOEXCEPT
    {
        Color4u8 result(
            static_cast<uint8_t>( ElementMax * Clamp( 0.f, std::pow( static_cast<float>(this->v[0]) / ElementMax, gamma ), 1.f ) ),
            static_cast<uint8_t>( ElementMax * Clamp( 0.f, std::pow( static_cast<float>(this->v[1]) / ElementMax, gamma ), 1.f ) ),
            static_cast<uint8_t>( ElementMax * Clamp( 0.f, std::pow( static_cast<float>(this->v[2]) / ElementMax, gamma ), 1.f ) ),
            this->v[3]
        );
        return result;
    }

    //! @brief sRGB カラーに変換した結果を返します。
    Color4u8 ToSrgb() const NN_NOEXCEPT
    {
        return CorrectGamma( SrgbDefault );
    }

    //! @brief リニアカラーに変換した結果を返します。
    Color4u8 ToLinear() const NN_NOEXCEPT
    {
        return CorrectGamma( LinearDefault );
    }

    //! @brief        完全な透明色かどうかを返します。
    //! @return       A 成分の値が 0x00 であれば true が返ります。
    bool IsTransparent() const NN_NOEXCEPT
    {
        return this->v[3] == ElementMin;
    }

    //! @name 定数
    //! @{

    static const uint8_t ElementMax = 0xff;
    static const uint8_t ElementMin = 0x00;

    //! @brief 黒色を表すカラー値です。
    //! @return 黒色を表す Color4u8 を返します。
    static const Color4u8& Black() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantBlack);
    }

    //! @brief 灰色を表すカラー値です。
    //! @return 灰色を表す Color4u8 を返します。
    static const Color4u8& Gray() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantGray);
    }

    //! @brief 白色を表すカラー値です。
    //! @return 白色を表す Color4u8 を返します。
    static const Color4u8& White() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantWhite);
    }

    //! @brief 赤色を表すカラー値です。
    //! @return 赤色を表す Color4u8 を返します。
    static const Color4u8& Red() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantRed);
    }

    //! @brief 緑色を表すカラー値です。
    //! @return 緑色を表す Color4u8 を返します。
    static const Color4u8& Green() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantGreen);
    }

    //! @brief 青色を表すカラー値です。
    //! @return 青色を表す Color4u8 を返します。
    static const Color4u8& Blue() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantBlue);
    }

    //! @brief 黄色を表すカラー値です。
    //! @return 黄色を表す Color4u8 を返します。
    static const Color4u8& Yellow() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantYellow);
    }

    //! @brief マゼンタ色を表すカラー値です。
    //! @return マゼンタ色を表す Color4u8 を返します。
    static const Color4u8& Magenta() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantMagenta);
    }

    //! @brief シアン色を表すカラー値です。
    //! @return シアン色を表す Color4u8 を返します。
    static const Color4u8& Cyan() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4u8&>(ConstantCyan);
    }

    //! @}

private:
    static const Color4u8Type ConstantBlack;
    static const Color4u8Type ConstantGray;
    static const Color4u8Type ConstantWhite;
    static const Color4u8Type ConstantRed;
    static const Color4u8Type ConstantGreen;
    static const Color4u8Type ConstantBlue;
    static const Color4u8Type ConstantYellow;
    static const Color4u8Type ConstantMagenta;
    static const Color4u8Type ConstantCyan;

    static const float SrgbDefault;
    static const float LinearDefault;

    inline static float Clamp(float min, float value, float max )
    {
        if ( value < min )
        {
            value = min;
        }
        else if ( value > max )
        {
            value = max;
        }
        return value;
    }
};

//! @brief SIMD 4 要素各 32 ビットの浮動小数点数で表現されるカラーをサポートするクラスです。
class Color4f : public Color4fType
{
public:
    //! @brief コンストラクタです。
    //! @details 値は初期化されません。
    Color4f() NN_NOEXCEPT
    {}

    //! @brief 引数付きコンストラクタです。
    //! @param[in]    red     カラーの R 要素
    //! @param[in]    green   カラーの G 要素
    //! @param[in]    blue    カラーの B 要素
    //! @param[in]    alpha   カラーの A 要素
    Color4f(float red, float green, float blue, float alpha = 1.0f) NN_NOEXCEPT
    {
        VectorSet(this, red, green, blue, alpha);
    }

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value Color4u8 の基底クラスの Color4u8Type です。
    //! @details 0.0～1.0 の範囲に変換されます。
    explicit Color4f(const Color4u8Type& value) NN_NOEXCEPT
    {
        VectorSet(
            this,
            static_cast<float>( value.v[0] ) / Color4u8::ElementMax,
            static_cast<float>( value.v[1] ) / Color4u8::ElementMax,
            static_cast<float>( value.v[2] ) / Color4u8::ElementMax,
            static_cast<float>( value.v[3] ) / Color4u8::ElementMax
        );
    }

    //! @brief 引数付きコンストラクタです。
    //! @param[in] value Color4f の基底クラスの Color4fType です。
    NN_IMPLICIT Color4f(const Color4fType& value) NN_NOEXCEPT
    : Color4fType(value)
    {}

    //! @name 取得／設定
    //! @{

    //! @brief カラーの R 要素を取得します。
    //! @return  カラーの R 要素を返します
    float GetR() const NN_NOEXCEPT
    {
        return VectorGetX(*this);
    }

    //! @brief カラーの G 要素を取得します。
    //! @return  カラーの G 要素を返します
    float GetG() const NN_NOEXCEPT
    {
        return VectorGetY(*this);
    }

    //! @brief カラーの B 要素を取得します。
    //! @return  カラーの B 要素を返します
    float GetB() const NN_NOEXCEPT
    {
        return VectorGetZ(*this);
    }

    //! @brief カラーの A 要素を取得します。
    //! @return  カラーの A 要素を返します
    float GetA() const NN_NOEXCEPT
    {
        return VectorGetW(*this);
    }

    //! @brief 各要素の値を取得します。
    //! @param[out] pOutR R 要素の格納先です。
    //! @param[out] pOutG G 要素の格納先です。
    //! @param[out] pOutB B 要素の格納先です。
    //! @param[out] pOutA A 要素の格納先です。
    //! @pre 引数は全て非 NULL であることが必須です。
    void Get(float* pOutR, float* pOutG, float* pOutB, float* pOutA) const NN_NOEXCEPT
    {
        // 各成分ごとに取り出すよりも，一気に取り出したほうがシャッフルなどが無く速いことを期待して実装。
        // TODO : 要検証（命令数比較，速度比較）
        NN_SDK_REQUIRES_NOT_NULL(pOutR);
        NN_SDK_REQUIRES_NOT_NULL(pOutG);
        NN_SDK_REQUIRES_NOT_NULL(pOutB);
        NN_SDK_REQUIRES_NOT_NULL(pOutA);
        (*pOutR) = this->_v[0];
        (*pOutG) = this->_v[1];
        (*pOutB) = this->_v[2];
        (*pOutA) = this->_v[3];
    }

    //! @brief R 要素を設定します。
    //! @param[in] red 設定する R 要素の値です。
    void SetR(float red) NN_NOEXCEPT
    {
        VectorSetX(this, red);
    }

    //! @brief G 要素を設定します。
    //! @param[in] green 設定する G 要素の値です。
    void SetG(float green) NN_NOEXCEPT
    {
        VectorSetY(this, green);
    }

    //! @brief B 要素を設定します。
    //! @param[in] blue 設定する B 要素の値です。
    void SetB(float blue) NN_NOEXCEPT
    {
        VectorSetZ(this, blue);
    }

    //! @brief A 要素を設定します。
    //! @param[in] alpha 設定する A 要素の値です。
    void SetA(float alpha) NN_NOEXCEPT
    {
        VectorSetW(this, alpha);
    }

    //! @brief 各成分の値を浮動小数で設定します。
    //! @param[in]    red     カラーの R 要素
    //! @param[in]    green   カラーの G 要素
    //! @param[in]    blue    カラーの B 要素
    //! @param[in]    alpha   カラーの A 要素
    void Set(float red, float green, float blue, float alpha) NN_NOEXCEPT
    {
        VectorSet(this, red, green, blue, alpha);
    }

    //! @}
    //! @name 比較演算子
    //! @{

    //! @brief        2つの色の RGBA 成分が等しいかどうかを比較します。
    //! @param[in]    value   比較するカラー値です。
    //! @return       等しい場合は true を返します。
    bool operator == (const Color4f& value) const NN_NOEXCEPT
    {
        return (this->_v[0] == value._v[0] && this->_v[1] == value._v[1] && this->_v[2] == value._v[2] && this->_v[3] == value._v[3]);
    }

    //! @brief        2つの色の RGBA 成分が等しくないかどうか比較します。
    //! @param[in]    value   比較するカラー値です。
    //! @return       異なる場合は true を返します。
    bool operator != (const Color4f& value) const NN_NOEXCEPT
    {
        return !(*this == value);
    }

    //! @}

    //! @brief 2つの色を ratio の割合で各成分ごとに線形補間して返します。
    //! @param[in]    a   補間する色1
    //! @param[in]    b   補間する色2
    //! @param[in]    t   補間係数
    //! @return  補間後の色 ( a + (b - a) * t の計算結果) を返します。
    static Color4f Lerp(const Color4f& a, const Color4f& b, float t) NN_NOEXCEPT
    {
        Color4f result;
        result.SetLerp(a, b, t);
        return result;
    }

    //! @brief 2つの色を ratio の割合で各成分ごとに線形補間し、結果を this に代入します。
    //! @param[in]    a   補間する色1
    //! @param[in]    b   補間する色2
    //! @param[in]    t   補間係数
    void SetLerp(const Color4f& a, const Color4f& b, float t) NN_NOEXCEPT
    {
        VectorLerp(this, a, b, t);
    }

    //! @brief カラーのガンマ変換を行った結果を返します。
    //! @param[in]    gamma 補正値です。
    //! @pre 変換前の RGB 値は 0.0f～1.0f の範囲に収まっている必要があります。
    Color4f CorrectGamma(float gamma) const NN_NOEXCEPT
    {
        Color4f result(
            std::pow(this->_v[0], gamma),
            std::pow(this->_v[1], gamma),
            std::pow(this->_v[2], gamma),
            this->_v[3]
        );
        return result;
    }

    //! @brief sRGB カラーに変換した結果を返します。
    //! @pre 変換前の RGB 値は 0.0f～1.0f の範囲に収まっている必要があります。
    Color4f ToSrgb() const NN_NOEXCEPT
    {
        return CorrectGamma( SrgbDefault );
    }

    //! @brief リニアカラーに変換した結果を返します。
    //! @pre 変換前の RGB 値は 0.0f～1.0f の範囲に収まっている必要があります。
    Color4f ToLinear() const NN_NOEXCEPT
    {
        return CorrectGamma( LinearDefault );
    }

    //! @brief 各成分を 0.0〜1.0 の範囲にクランプします
    void Clamp() NN_NOEXCEPT
    {
        this->Clamp(0.0f, 1.0f);
    }

    //! @brief 各成分を a〜b の範囲にクランプします
    //! @param[in]    a     下限値
    //! @param[in]    b     上限値
    void Clamp(float a, float b) NN_NOEXCEPT
    {
        this->_v[0] = Clamp( a, this->_v[0], b );
        this->_v[1] = Clamp( a, this->_v[1], b );
        this->_v[2] = Clamp( a, this->_v[2], b );
        this->_v[3] = Clamp( a, this->_v[3], b );
    }

    //! @brief        完全な透明色かどうかを返します。
    //! @return       A 成分の値が 0.0f 以下の場合は true が返ります。
    bool IsTransparent() const NN_NOEXCEPT
    {
        return this->_v[3] <= 0.0f;
    }

    //! @name 定数
    //! @{

    //! @brief 黒色を表すカラー値です。
    //! @return 黒色を表す Color4f を返します。
    static const Color4f& Black() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantBlack);
    }

    //! @brief 灰色を表すカラー値です。
    //! @return 灰色を表す Color4f を返します。
    static const Color4f& Gray() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantGray);
    }

    //! @brief 白色を表すカラー値です。
    //! @return 白色を表す Color4f を返します。
    static const Color4f& White() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantWhite);
    }

    //! @brief 赤色を表すカラー値です。
    //! @return 赤色を表す Color4f を返します。
    static const Color4f& Red() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantRed);
    }

    //! @brief 緑色を表すカラー値です。
    //! @return 緑色を表す Color4f を返します。
    static const Color4f& Green() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantGreen);
    }

    //! @brief 青色を表すカラー値です。
    //! @return 青色を表す Color4f を返します。
    static const Color4f& Blue() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantBlue);
    }

    //! @brief 黄色を表すカラー値です。
    //! @return 黄色を表す Color4f を返します。
    static const Color4f& Yellow() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantYellow);
    }

    //! @brief マゼンタ色を表すカラー値です。
    //! @return マゼンタ色を表す Color4f を返します。
    static const Color4f& Magenta() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantMagenta);
    }

    //! @brief シアン色を表すカラー値です。
    //! @return シアン色を表す Color4f を返します。
    static const Color4f& Cyan() NN_NOEXCEPT
    {
        // 非ローカル変数の動的初期化は初期化順が不定なので，関数にして静的初期化済みの変数を返すように実装.
        return static_cast<const Color4f&>(ConstantCyan);
    }

    //! @}

private:
    static const Color4fType ConstantBlack;
    static const Color4fType ConstantGray;
    static const Color4fType ConstantWhite;
    static const Color4fType ConstantRed;
    static const Color4fType ConstantGreen;
    static const Color4fType ConstantBlue;
    static const Color4fType ConstantYellow;
    static const Color4fType ConstantMagenta;
    static const Color4fType ConstantCyan;

    static const float SrgbDefault;
    static const float LinearDefault;

    inline static float Clamp(float min, float value, float max )
    {
        if ( value < min )
        {
            value = min;
        }
        else if ( value > max )
        {
            value = max;
        }
        return value;
    }
};

}
}
