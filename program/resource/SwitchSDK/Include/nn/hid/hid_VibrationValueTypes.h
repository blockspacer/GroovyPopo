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
 * @brief   振動値に関する定数や型の定義
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace hid {

/**
 * @brief   低帯域のデフォルト周波数です。単位は Hz です。
 */
const int VibrationFrequencyLowDefault = 160;

/**
 * @brief   高帯域のデフォルト周波数です。単位は Hz です。
 */
const int VibrationFrequencyHighDefault = 320;

/**
 * @brief   振動値を表す構造体です。
 *
 * @details 振動値はある瞬間の振動の振幅と周波数の組です。
 *          低帯域と高帯域それぞれの振幅と周波数の値をセットで表します。
 */
struct VibrationValue
{
    float amplitudeLow;     //!< 低帯域の振幅 (最大振幅を 1.0f とする単位)
    float frequencyLow;     //!< 低帯域の周波数 (単位は Hz)
    float amplitudeHigh;    //!< 高帯域の振幅 (最大振幅を 1.0f とする単位)
    float frequencyHigh;    //!< 高帯域の周波数 (単位は Hz)

    /**
     * @brief       デフォルトの振動値を生成します。
     *
     * @return      デフォルトの振動値を返します。
     *
     * @details     デフォルトの振動値は振幅が 0.0f, 周波数はそれぞれデフォルト値になります。
     */
    static inline VibrationValue Make() NN_NOEXCEPT
    {
        VibrationValue v;
        v.amplitudeLow = 0.0f;
        v.frequencyLow = static_cast<float>(VibrationFrequencyLowDefault);
        v.amplitudeHigh = 0.0f;
        v.frequencyHigh = static_cast<float>(VibrationFrequencyHighDefault);
        return v;
    }

    /**
     * @brief       指定された値で振動値を生成します。
     *
     * @return      振動値を返します。
     *
     * @param[in]   amplitudeLow    低帯域の振幅 (最大振幅を 1.0f とする単位)
     * @param[in]   frequencyLow    低帯域の周波数 (単位は Hz)
     * @param[in]   amplitudeHigh   高帯域の振幅 (最大振幅を 1.0f とする単位)
     * @param[in]   frequencyHigh   高帯域の周波数 (単位は Hz)
     */
    static inline VibrationValue Make(
        float amplitudeLow,
        float frequencyLow,
        float amplitudeHigh,
        float frequencyHigh) NN_NOEXCEPT
    {
        VibrationValue v;
        v.amplitudeLow = amplitudeLow;
        v.frequencyLow = frequencyLow;
        v.amplitudeHigh = amplitudeHigh;
        v.frequencyHigh = frequencyHigh;
        return v;
    }
};

}} // namespace nn::hid
