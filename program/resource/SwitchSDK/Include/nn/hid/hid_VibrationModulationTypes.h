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
 * @brief   振動値に対する調整係数に関する型の定義
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/hid/hid_VibrationValueTypes.h>

namespace nn { namespace hid {

/**
 * @brief   振動値に対する調整係数を表す構造体です。
 *
 * @details 振動値調整係数は、振動値の各メンバ変数に対して乗算される係数の組です。
 *          振動値調整係数を振動値に適用することによって、振動値を変化させることができます。
 */
struct VibrationModulation
{
    float gainLow;      //!< 低帯域の振幅に対する調整係数
    float pitchLow;     //!< 低帯域の周波数に対する調整係数
    float gainHigh;     //!< 高帯域の振幅に対する調整係数
    float pitchHigh;    //!< 高帯域の周波数に対する調整係数

    /**
     * @brief       デフォルトの振動値調整係数を生成します。
     *
     * @return      デフォルトの振動値調整係数を返します。
     *
     * @details     デフォルトの振動値調整係数はすべての係数が 1.0f のため、元の振動値に何も調整を加えないという意味になります。
     */
    static inline VibrationModulation Make() NN_NOEXCEPT
    {
        VibrationModulation vm;
        vm.gainLow = 1.0f;
        vm.pitchLow = 1.0f;
        vm.gainHigh = 1.0f;
        vm.pitchHigh = 1.0f;
        return vm;
    }

    /**
     * @brief       指定された値で振動値調整係数を生成します。
     *
     * @return      振動値調整係数を返します。
     *
     * @param[in]   gainLow     低帯域の振幅に対する調整係数
     * @param[in]   pitchLow    低帯域の周波数に対する調整係数
     * @param[in]   gainHigh    高帯域の振幅に対する調整係数
     * @param[in]   pitchHigh   高帯域の周波数に対する調整係数
     */
    static inline VibrationModulation Make(
        float gainLow,
        float pitchLow,
        float gainHigh,
        float pitchHigh) NN_NOEXCEPT
    {
        VibrationModulation vm;
        vm.gainLow = gainLow;
        vm.pitchLow = pitchLow;
        vm.gainHigh = gainHigh;
        vm.pitchHigh = pitchHigh;
        return vm;
    }

    /**
     * @brief       振動値調整係数を振動値に適用します。
     *
     * @param[in, out]  pValue  振動値へのポインタ
     */
    inline void ApplyModulation(VibrationValue* pValue) NN_NOEXCEPT
    {
        pValue->amplitudeLow *= gainLow;
        pValue->frequencyLow *= pitchLow;
        pValue->amplitudeHigh *= gainHigh;
        pValue->frequencyHigh *= pitchHigh;
    }
};

}} // namespace nn::hid
