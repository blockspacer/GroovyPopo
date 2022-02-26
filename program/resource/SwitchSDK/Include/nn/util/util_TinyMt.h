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
* @brief    TinyMT による擬似乱数を提供します
*/

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace util {

/* ------------------------------------------------------------------------
        クラス定義
   ------------------------------------------------------------------------ */

//----------------------------------------------------------------------
/**
 * @brief  TinyMT による乱数生成クラスです。
 *
 * @details
 *  TinyMT (Tiny Mersenne Twister) はメルセンヌツイスターよりも劣るものの
 *  良質な疑似乱数を生成する乱数生成アルゴリズムです。
 *  2.5 KB の内部状態を必要とするメルセンヌツイスターに対して
 *  TinyMT では内部状態は 16 byte で済みます。
 *
 *  このクラスではパラメータ (mat1, mat2, tmat) は
 *  (0x8f7011ee, 0xfc78ff1f, 0x3793fdff) にハードコーディングされています。
 *
 *  乱数の周期は 2 の 127 乗 - 1 です。
 *  通常、乱数列が一周することはありません。
 *
 */
//----------------------------------------------------------------------
class TinyMt
{
private:
    static const int   ParameterN  =  4;

public:
    //----------------------------------------------------------------------
    //! @brief  乱数生成の内部状態を保存するための構造体です。
    //----------------------------------------------------------------------
    struct State
    {
        //----------------------------------------------------------------------
        //! @brief  内部状態です。
        //----------------------------------------------------------------------
        Bit32 state[ParameterN];
    };

//! @name 初期化と終了
//! @{

    //----------------------------------------------------------------------
    /**
     * @brief  32bit のシードで初期化します。
     *
     * @param[in]    seed      乱数のシードとして使用する値
     */
    //----------------------------------------------------------------------
    void Initialize(Bit32 seed) NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  32bit 列をシードとして初期化します。
     *
     * @param[in]    pSeed     シードとして使用する 32bit データ列へのポインタ。
     * @param[in]    numSeed   pSeed が指す配列の要素数。
     */
    //----------------------------------------------------------------------
    void Initialize(const Bit32* pSeed, int numSeed) NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  終了処理を行います。
     */
    //----------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT {}

//! @}

//! @name 状態の保存と復帰
//! @{

    //----------------------------------------------------------------------
    /**
     * @brief  内部状態を保存します。
     *
     * @param[in]    pStateBuffer  内部状態の保存先となるバッファ。
     *
     * @details
     *  指定されたバッファに内部状態を保存します。
     *  保存された内部状態は @ref RestoreState を使用することで
     *  復帰させることができます。
     *
     *  この関数により得られる値を保存しておくことで、
     *  後で続きから乱数生成を再開させることができます。
     *  また、同じ乱数列を再現させるために使用することもできます。
     */
    //----------------------------------------------------------------------
    void SaveState(TinyMt::State* pStateBuffer) NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  内部状態を復帰します。
     *
     * @param[in]    pStateBuffer  pSeed が指す配列の要素数。
     *
     * @details
     *  @ref SaveState によって保存された内部状態を復帰させます。
     *  これにより保存された状態から乱数生成を再開させることができます。
     *
     *  この関数は @ref Initialize の代わりとして使用することができます。
     */
    //----------------------------------------------------------------------
    void RestoreState(const TinyMt::State* pStateBuffer) NN_NOEXCEPT;

//! @}

//! @name 乱数生成
//! @{

    //----------------------------------------------------------------------
    /**
     * @brief  符号なし 32 bit 整数乱数を生成します。
     *
     * @return 符号なし 32 bit 整数乱数を返します。
     *
     * @details
     * 0x00000000 ～ 0xFFFFFFFF の乱数を生成します。
     */
    //----------------------------------------------------------------------
    uint32_t GenerateRandomU32() NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  符号なし 64 bit 整数乱数を生成します。
     *
     * @return 符号なし 64 bit 整数乱数を返します。
     *
     * @details
     *  0x00000000 00000000 ～ 0xFFFFFFFF FFFFFFFF の乱数を生成します。
     */
    //----------------------------------------------------------------------
    uint64_t GenerateRandomU64() NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  単精度浮動小数点数乱数を生成します。
     *
     * @return 単精度浮動小数点数乱数を返します。
     */
    //----------------------------------------------------------------------
    float GenerateRandomF32() NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  倍精度浮動小数点数乱数を生成します。
     *
     * @return 倍精度浮動小数点数乱数を返します。
     */
    //----------------------------------------------------------------------
    double GenerateRandomF64() NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  指定した数未満で乱数を生成します。
     *
     * @param[in]    num   返り値の種類を指定します。
     *
     * @return 指定した範囲の整数乱数を返します。
     *
     * @details
     *  0 以上 num 未満の整数の範囲で乱数を生成します。
     *  返り値の種類は num 種類になります。
     *
     *  各値の返る確率は完全に均等ではないことに注意してください。
     *  例えば num に 6 を指定した場合、
     *  0, 1, 3, 4 が返る確率は 0x2AAAAAAB / 0x100000000 ですが、
     *  2, 5 が返る確率は 0x2AAAAAAA / 0x100000000 となり、
     *  2, 5 が出る確率は 0, 1, 3, 4 が出る確率の
     *  0x2AAAAAAA / 0x2AAAAAAB 倍ということになります。
     *
     *  確率の偏りは num が大きくなるほど大きくなり
     *  num に最大値である 0xFFFF を指定した場合は
     *  0 が出る確率が 0 以外が出る確率の
     *  65538 / 65537 倍（1.0000153 倍）となります。
     *
     *  num が 2 の整数乗である場合には偏りは発生しません。
     */
    //----------------------------------------------------------------------
    uint32_t GenerateRandomN(uint16_t num) NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief  ランダムなバイト列を生成します。
     *
     * @param[out]   pOutBuffer  ランダムなバイト列を格納するバッファを指定します。
     * @param[in]    size        pOutBuffer に格納するランダムなバイト列のバイトサイズを指定します。
     *
     * @details
     *  size バイトのランダムなバイト列を生成し、
     *  pOutBuffer で指定されるバッファに格納します。
     */
    //----------------------------------------------------------------------
    void GenerateRandomBytes(void* pOutBuffer, size_t size) NN_NOEXCEPT;

//! @}

private:
    static const Bit32 ParameterMat1 = 0x8f7011ee;
    static const Bit32 ParameterMat2 = 0xfc78ff1f;
    static const Bit32 ParameterTmat = 0x3793fdff;

    static const int MinLoop = 8;
    static const int NumSkip = 8;

    static const Bit32 Bit31Mask = ~0u >> 1;

private:
    Bit32   m_State[ParameterN];

private:
    uint32_t GenerateRandomU24() NN_NOEXCEPT
    {
        return GenerateRandomU32() >> 8;
    }

    void FinalizeInitialization() NN_NOEXCEPT;

    static void GenerateInitialValuePlus(Bit32* p, int d, Bit32 k) NN_NOEXCEPT;
    static void GenerateInitialValueXor(Bit32* p, int d) NN_NOEXCEPT;
};



/* ------------------------------------------------------------------------
        インラインメンバ関数定義
   ------------------------------------------------------------------------ */

inline uint64_t TinyMt::GenerateRandomU64() NN_NOEXCEPT
{
    const uint32_t lo = GenerateRandomU32();
    const uint32_t hi = GenerateRandomU32();
    return (static_cast<uint64_t>(hi) << 32) | lo;
}

inline float TinyMt::GenerateRandomF32() NN_NOEXCEPT
{
    return GenerateRandomU24() * (1.0f / (1ull << 24));
}

inline double TinyMt::GenerateRandomF64() NN_NOEXCEPT
{
    static const int Resolution = 53;
    static const int U32Bits    = 32;
    static const int ShiftA     = (2 * U32Bits - Resolution) / 2;
    static const int ShiftB     = (2 * U32Bits - Resolution) - ShiftA;
    //static const int BitsA      = U32Bits - ShiftA;
    static const int BitsB      = U32Bits - ShiftB;

    uint32_t a = (GenerateRandomU32() >> ShiftA);
    uint32_t b = (GenerateRandomU32() >> ShiftB);

    return (1.0 * a * (1u << BitsB) + b) * (1.0 / (1ull << Resolution));
}

inline uint32_t TinyMt::GenerateRandomN(uint16_t max) NN_NOEXCEPT
{
    return static_cast<uint32_t>((static_cast<uint64_t>(GenerateRandomU32()) * max) >> 32);
}

}}
