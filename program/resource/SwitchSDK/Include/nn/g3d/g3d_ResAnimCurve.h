/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief アニメーションカーブに関するリソースクラスの宣言
*/

#pragma once

#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/detail/g3d_MathCommon.h>
#include <nn/g3d/g3d_ResCommon.h>

#include <limits>

NN_PRAGMA_PUSH_WARNINGS
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

//! @brief アニメーションに関するフラグです。
enum AnimFlag
{
    AnimFlag_CurveBaked     = 0x1 << 0, //!< @brief カーブがベイク済みであることを表します。
    AnimFlag_PlayPolicyLoop = 0x1 << 2, //!< @brief ループ再生を表します。
    AnimFlag_NotBound       = 0xFFFF //!< @brief 範囲外であることを表します。
};

//! @brief アニメーションフレームのキャッシュ構造体です。
struct AnimFrameCache
{
    float start; //!< 開始フレーム。
    float end; //!< 終了フレーム。
    int keyIndex; //!< キーインデックス。
};

//! @brief 定数アニメーションの構造体です。
struct ResAnimConstantData
{
    uint32_t targetOffset; //!< 対象のオフセット。
    union
    {
        float fValue; //!< 浮動小数点値。
        int32_t iValue; //!< 整数値。
    };
};

//! @brief 定数アニメーションのリソースです。
class ResAnimConstant : public nn::util::AccessorBase< ResAnimConstantData >
{
    NN_DISALLOW_COPY(ResAnimConstant);

public:
    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief Float として値を取得します。
    float GetFloat() const NN_NOEXCEPT
    {
        return ToData().fValue;
    }

    //! @brief Int として値を取得します。
    int GetInt() const NN_NOEXCEPT
    {
        return ToData().iValue;
    }

    //@}
};

//! @brief アニメーションカーブの構造体です。
struct ResAnimCurveData
{
    nn::util::BinPtr pFrameArray; //!< フレームの計算に用いる構造体の配列へのポインター。
    nn::util::BinPtr pKeyArray; //!< キーの計算に用いる構造体の配列へのポインター。
    Bit16            flag; //!< フラグ。
    uint16_t         keyCount; //!< キー数。
    uint32_t         targetOffset; //!< ターゲット構造体内におけるバイトオフセット
    float            startFrame; //!< 開始フレーム。
    float            endFrame; //!< 終了フレーム。
    union
    {
        float   fScale; //!< float の結果に対するスケール。
        int32_t iScale; //!< 未使用。
    };
    union
    {
        float   fOffset; //!< float の結果に対するオフセット。
        int32_t iOffset; //!< int の結果に対するオフセット。
    };
    union
    {
        float   fDelta; //!< デルタ値。
        int32_t iDelta;
    };
    uint8_t reserved[4];
};

//! @brief アニメーションカーブのリソースです。
class ResAnimCurve : public nn::util::AccessorBase< ResAnimCurveData >
{
    NN_DISALLOW_COPY(ResAnimCurve);

public:
    //! @briefprivate アニメーションカーブに関するシフト値です。
    enum Shift
    {
        Shift_Frame = 0,
        Shift_Key   = 2,
        Shift_Curve = 4,
        Shift_PreWrapMode = 8,
        Shift_PostWrapMode = 12
    };

    //! @briefprivate アニメーションカーブに関するマスク値です。
    enum Mask
    {
        Mask_Frame = 0x3 << Shift_Frame,
        Mask_Key   = 0x3 << Shift_Key,
        Mask_Curve = 0x7 << Shift_Curve,
        Mask_WrapMode = 0x3,
    };

    //! @briefprivate
    enum TypeCount
    {
        TypeCount_Frame = 3,
        TypeCount_Key   = 3
    };

    //! @brief アニメーションカーブの frame quantization のタイプです。
    enum FrameType
    {
        // FrameQuantization
        FrameType_Quant32 = 0x0 << Shift_Frame, //!< float 浮動小数点数のフレームです。
        FrameType_Quant16 = 0x1 << Shift_Frame, //!< S10.5 固定小数点数のフレームです。
        FrameType_Quant8  = 0x2 << Shift_Frame, //!< U8 整数のフレームです。
    };

    //! @brief アニメーションカーブの　key quantizationのタイプです。
    enum KeyType
    {
        KeyType_Quant32 = 0x0 << Shift_Key, //! float 浮動小数点数 * fScale + fOffset のキーです。
        KeyType_Quant16 = 0x1 << Shift_Key, //! S16 整数 * fScale + fOffset のキーです。
        KeyType_Quant8  = 0x2 << Shift_Key //! S8 整数 * fScale + fOffset のキーです。
    };

    //! @brief アニメーションカーブのタイプです。
    enum CurveType
    {
        // float
        CurveType_Cubic        = 0x0 << Shift_Curve, //!< 3次多項式のカーブです。
        CurveType_Linear       = 0x1 << Shift_Curve, //!< 1次多項式のカーブです。
        CurveType_BakedFloat   = 0x2 << Shift_Curve, //!< コマ形式のカーブです。
        // int
        CurveType_StepInt      = 0x4 << Shift_Curve, //!< ステップ形式のカーブです。
        CurveType_BakedInt     = 0x5 << Shift_Curve, //!< コマ形式のカーブです。
        CurveType_StepBool     = 0x6 << Shift_Curve, //!< ステップ形式のカーブです。
        CurveType_BakedBool    = 0x7 << Shift_Curve, //!< コマ形式のカーブです。
        CurveType_IntBase      = CurveType_StepInt //! @briefprivate
    };


    //! @brief 繰り返し方法です。
    enum WrapMode
    {
        WrapMode_Clamp, //!< 最初または最後のフレームにクランプします。
        WrapMode_Repeat, //!< カーブを繰り返します。
        WrapMode_Mirror, //!< カーブを反転しつつ繰り返します。
        WrapMode_RelativeRepeat //!< 端を起点として相対的にカーブを繰り返します。
    };

    //! @brief コマ化時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = 8, //!< コマ化時に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //!@ brief カーブをコマ化します。
    void Bake(void* pBuffer, size_t bufferSize) NN_NOEXCEPT
    {
        IsFloatCurve() ? BakeFloat(pBuffer, bufferSize) : BakeInt(pBuffer, bufferSize);
    }

    //! @brief Float カーブをコマ化します。
    void BakeFloat(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief Int カーブをコマ化します。
    void BakeInt(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief カーブをコマ化するのに必要なバッファーサイズを返します。
    //!
    //! @return カーブをコマ化するのに必要なバッファーサイズを返します。
    //!
    size_t CalculateBakedSize() const NN_NOEXCEPT
    {
        return IsFloatCurve() ? CalculateBakedFloatSize() : CalculateBakedIntSize();
    }

    //! @brief Float カーブをコマ化するのに必要なバッファーサイズを返します。
    //!
    //! @return Float カーブをコマ化するのに必要なバッファーサイズを返します。
    //!
    size_t CalculateBakedFloatSize() const NN_NOEXCEPT;

    //! @brief Int カーブをコマ化するのに必要なバッファーサイズを返します。
    //!
    //! @return Int カーブをコマ化するのに必要なバッファーサイズを返します。
    //!
    size_t CalculateBakedIntSize() const NN_NOEXCEPT;

    //! @brief コマ化したアニメーションをカーブに戻します。
    void Reset() NN_NOEXCEPT
    {
        IsFloatCurve() ? ResetFloat() : ResetInt();
    }

    //! @brief コマ化したアニメーションを Float カーブに戻します。
    void ResetFloat() NN_NOEXCEPT;

    //! @brief コマ化したアニメーションを Int カーブに戻します。
    void ResetInt() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief Float カーブを評価します。
    //!
    //! @return Float カーブを評価した結果の値を返します。
    //!
    float EvaluateFloat(float frame) const NN_NOEXCEPT
    {
        AnimFrameCache frameCache;
        frameCache.start = std::numeric_limits<float>::infinity();
        return EvaluateFloat(frame, &frameCache);
    }

    //! @brief Int カーブを評価します。
    //!
    //! @return Int カーブを評価した結果の値を返します。
    //!
    int EvaluateInt(float frame) const NN_NOEXCEPT
    {
        AnimFrameCache frameCache;
        frameCache.start = std::numeric_limits<float>::infinity();
        return EvaluateInt(frame, &frameCache);
    }

    //! @brief Float カーブを評価します。
    //!
    //! @param[in] frame カーブを評価するフレーム。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    //! @return Int カーブを評価した結果の値を返します。
    //!
    float EvaluateFloat(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @brief Int カーブを評価します。
    //!
    //! @param[in] frame カーブを評価するフレーム。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    //! @return Int カーブを評価した結果の値を返します。
    //!
    int EvaluateInt(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief Float カーブかどうかを取得します。
    //!
    //! @return Float カーブの場合は true、それ以外の場合は false を返します。
    //!
    bool IsFloatCurve() const NN_NOEXCEPT
    {
        return (ToData().flag & Mask_Curve) < CurveType_IntBase;
    }

    //! @brief Int カーブかどうかを取得します。
    //!
    //! @return Int カーブの場合は true、それ以外の場合は false を返します。
    //!
    bool IsIntCurve() const NN_NOEXCEPT
    {
        return (ToData().flag & Mask_Curve) >= CurveType_IntBase;
    }

    //! @brief 最初のキーよりも前のフレームで評価した際の動作を取得します。
    //!
    //! @return 最初のキーよりも前のフレームで評価した際の動作モードを返します。
    //!
    WrapMode GetPreWrapMode() const NN_NOEXCEPT
    {
        return WrapMode((ToData().flag >> Shift_PreWrapMode) & Mask_WrapMode);
    }

    //! @brief 最後のキーよりも後のフレームで評価した際の動作を取得します。
    //!
    //! @return 最後のキーよりも後のフレームで評価した際の動作モードを返します。
    //!
    WrapMode GetPostWrapMode() const NN_NOEXCEPT
    {
        return WrapMode((ToData().flag >> Shift_PostWrapMode) & Mask_WrapMode);
    }

    //! @brief 最初のフレームを取得します。
    //!
    //! @return 最初のフレームを返します。
    //!
    float GetStartFrame() const NN_NOEXCEPT
    {
        return ToData().startFrame;
    }

    //! @brief 最後のフレームを取得します。
    //!
    //! @return 最後のフレームを返します。
    //!
    float GetEndFrame() const NN_NOEXCEPT
    {
        return ToData().endFrame;
    }

    template<typename T>
    T* GetFrameArray() NN_NOEXCEPT
    {
        return static_cast<T*>(ToData().pFrameArray.Get());
    }

    template<typename T>
    const T* GetFrameArray() const NN_NOEXCEPT
    {
        return static_cast<const T*>(ToData().pFrameArray.Get());
    }

    template<typename T>
    T* GetKeyArray() NN_NOEXCEPT
    {
        return static_cast<T*>(ToData().pKeyArray.Get());
    }

    template<typename T>
    const T* GetKeyArray() const NN_NOEXCEPT
    {
        return static_cast<const T*>(ToData().pKeyArray.Get());
    }

    //@}
protected:
    class Impl;

    //! @briefprivate
    Bit32 GetFrameType() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Frame;
    }

    //! @briefprivate
    Bit32 GetKeyType() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Key;
    }

    //! @briefprivate
    Bit32 GetCurveType() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Curve;
    }

    //! @briefprivate
    template <typename T>
    float WrapFrame(T *pOutOffset, float frame) const NN_NOEXCEPT;

    //! @briefprivate
    void UpdateFrameCache(AnimFrameCache* pFrameCache, float frame) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    void FindFrame(AnimFrameCache *pFrameCache, float frame) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    float EvaluateCubic(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    float EvaluateLinear(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    float EvaluateBakedFloat(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    int EvaluateStepInt(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    int EvaluateBakedInt(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    int EvaluateStepBool(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    int EvaluateBakedBool(float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @briefprivate
    template <typename T>
    void BakeImpl(void* pBuffer, float start, int KeyCount) NN_NOEXCEPT;
};

//--------------------------------------------------------------------------------------------------

//! @brief S10.5 固定小数点数を float 浮動小数点数に変換します。
//!
//! @return float 浮動小数点数を返します。
//!
NN_FORCEINLINE
float CastS10_5ToF32(int16_t value) NN_NOEXCEPT
{
    return FastCast<float>(value) * (1.0f / 32.0f);
}

//! @brief float 浮動小数点数を S10.5 固定小数点数に変換します。
//!
//! @return float S10.5 固定小数点数を返します。
//!
NN_FORCEINLINE
int16_t CastF32ToS10_5(float value) NN_NOEXCEPT
{
    return Math::Floor<int16_t>(value * 32.0f);
}

//! @brief 三次方程式を計算します。
//!
//! @param[in] t 時間。
//! @param[in] c0 0次の係数。
//! @param[in] c1 1次の係数。
//! @param[in] c2 2次の係数。
//! @param[in] c3 3次の係数。
//!
//! @return 三次方程式の計算結果を返します。
//!
NN_FORCEINLINE
float CalculateCubic(float t, float c0, float c1, float c2, float c3) NN_NOEXCEPT
{
    // 精度低下を防ぐため t 同士をかけないよう注意。
    return ((c3 * t + c2) * t) * t + (c1 * t + c0);
}

//! @brief 線形方程式を計算します。
//!
//! @param[in] t 時間。
//! @param[in] c0 0次の係数。
//! @param[in] c1 1次の係数。
//!
//! @return 線形方程式の計算結果を返します。
//!
NN_FORCEINLINE
float CalculateLinear(float t, float c0, float c1) NN_NOEXCEPT
{
    return c1 * t + c0;
}

//--------------------------------------------------------------------------------------------------

//! @brief Frame の計算に用いる構造体です。
template <typename T>
struct Frame
{
    T frame; //!< フレーム数。

    //! @brief フレームを float で取得します。
    //!
    //! @return フレーム数を返します。
    //!
    float Get() const NN_NOEXCEPT
    {
        return FastCast<float>(frame);
    }

    //! @brief フレーム区間の探索用にフレームを量子化します。
    //!
    //! @param[in] frame フレーム数。
    //!
    //! @return 量子化後のフレーム数を返します。
    //!
    //! @details
    //! フレーム区間の探索に使用するので、Round ではなく Floor で変換を行います。
    //!
    static T Quantize(float frame) NN_NOEXCEPT
    {
        return Math::Floor<T>(frame);
    }
};

//! @brief float の Frame の計算に用いる構造体です。
template <>
struct Frame<float>
{
    float frame; //!< フレーム数。

    //! @brief フレームを float で取得します。
    //!
    //! @return フレーム数を返します。
    //!
    float Get() const NN_NOEXCEPT
    {
        return frame;
    }

    //! @brief フレームを量子化します。
    //!
    //! @param[in] frame フレーム数。
    //!
    //! @return 量子化後のフレーム数を返します。
    //!
    static float Quantize(float frame) NN_NOEXCEPT
    {
        return frame;
    }
};

//! @brief int16_t の Frame の計算に用いる構造体です。
template <>
struct Frame<int16_t>
{
    int16_t frame; //!< フレーム数。

    //! @brief フレームを float で取得します。
    //!
    //! @return フレーム数を返します。
    //!
    float Get() const NN_NOEXCEPT
    {
        return CastS10_5ToF32(frame);
    }

    //! @brief フレームを量子化します。
    //!
    //! @param[in] frame フレーム数。
    //!
    //! @return 量子化後のフレーム数を返します。
    //!
    static int16_t Quantize(float frame) NN_NOEXCEPT
    {
        return CastF32ToS10_5(frame);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief キュービックキーの計算に用いる構造体です。
template <typename T>
struct ResCubicKey
{
    T coef[4]; //!< 係数。

    //! @brief 計算結果を取得します。
    //!
    //! @param[in] ratio 割合。
    //!
    //! @return 計算したキー値を返します。
    //!
    float Get(float ratio) const NN_NOEXCEPT
    {
        return CalculateCubic(ratio, static_cast<float>(coef[0]), static_cast<float>(coef[1]),
            static_cast<float>(coef[2]), static_cast<float>(coef[3]));
    }
};

//! @brief リニアキーの計算に用いる構造体です。
template <typename T>
struct ResLinearKey
{
    T coef[2];  //!< 係数。

    //! @brief 計算結果を取得します。
    //!
    //! @param[in] ratio 割合。
    //!
    //! @return 計算したキー値を返します。
    //!
    float Get(float ratio) const NN_NOEXCEPT
    {
        return CalculateLinear(ratio, static_cast<float>(coef[0]), static_cast<float>(coef[1]));
    }
};

//! @brief Float キーの計算に用いる構造体です。
template <typename T>
struct ResFloatKey
{
    T value;  //!< キー値。

    //! @brief 計算結果を取得します。
    //!
    //! @return キー値を返します。
    //!
    float Get() const NN_NOEXCEPT
    {
        return StaticCast<float>(value);
    }
};

//! @brief Int キーの計算に用いる構造体です。
template <typename T>
struct ResIntKey
{
    T value; //!< キー値。

    //! @brief 計算結果を取得します。
    //!
    //! @return キー値を返します。
    //!
    int Get() const NN_NOEXCEPT
    {
        return static_cast<int>(value);
    }
};

#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )

template <>
NN_FORCEINLINE
float ResCubicKey<float>::Get(float param) const NN_NOEXCEPT
{
    f32x2 term23 = __PSQ_LX(coef, 8, 0, 0);
    f32x2 term01 = __PSQ_LX(coef, 0, 0, 0);
    term23 = __PS_MULS0F(term23, param);
    term23 = __PS_MULS0F(term23, param);
    f32x2 one_t = { 1.0f, param };
    term01 = __PS_MUL(term01, one_t);
    term23 = __PS_MUL(term23, one_t);
    f32x2 sum = __PS_ADD(term01, term23);
    sum = __PS_SUM0(sum, sum, sum);
    return sum[0];
};

template <>
NN_FORCEINLINE
float ResCubicKey<int16_t>::Get(float param) const NN_NOEXCEPT
{
    f32x2 term23 = __PSQ_LX(coef, 4, 0, OS_FASTCAST_S16);
    f32x2 term01 = __PSQ_LX(coef, 0, 0, OS_FASTCAST_S16);
    term23 = __PS_MULS0F(term23, param);
    term23 = __PS_MULS0F(term23, param);
    f32x2 one_t = { 1.0f, param };
    term01 = __PS_MUL(term01, one_t);
    term23 = __PS_MUL(term23, one_t);
    f32x2 sum = __PS_ADD(term01, term23);
    sum = __PS_SUM0(sum, sum, sum);
    return sum[0];
};

template <>
NN_FORCEINLINE
float ResCubicKey<int8_t>::Get(float param) const NN_NOEXCEPT
{
    f32x2 term23 = __PSQ_LX(coef, 2, 0, OS_FASTCAST_S8);
    f32x2 term01 = __PSQ_LX(coef, 0, 0, OS_FASTCAST_S8);
    term23 = __PS_MULS0F(term23, param);
    term23 = __PS_MULS0F(term23, param);
    f32x2 one_t = { 1.0f, param };
    term01 = __PS_MUL(term01, one_t);
    term23 = __PS_MUL(term23, one_t);
    f32x2 sum = __PS_ADD(term01, term23);
    sum = __PS_SUM0(sum, sum, sum);
    return sum[0];
};

template <>
NN_FORCEINLINE
float ResLinearKey<float>::Get(float param) const NN_NOEXCEPT
{
    f32x2 coef01 = __PSQ_L(coef, 0, 0);
    return coef01[0] + coef01[1] * param;
};

template <>
NN_FORCEINLINE
float ResLinearKey<int16_t>::Get(float param) const NN_NOEXCEPT
{
    f32x2 coef01 = __PSQ_L(coef, 0, OS_FASTCAST_S16);
    return coef01[0] + coef01[1] * param;
};

template <>
NN_FORCEINLINE
float ResLinearKey<int8_t>::Get(float param) const NN_NOEXCEPT
{
    f32x2 coef01 = __PSQ_L(coef, 0, OS_FASTCAST_S8);
    return coef01[0] + coef01[1] * param;
};

#endif

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
