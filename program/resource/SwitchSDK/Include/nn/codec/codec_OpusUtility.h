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
 * @brief       Opus に関するユーティリティ API を定義します。
 */

#include <nn/nn_Common.h>

namespace nn {
namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       フレームからサンプル数を計算します。
 * @param[in]   frame       フレーム (単位は [us])
 * @param[in]   sampleRate  サンプルレート
 * @return      サンプル数を返します。
 * @retval      0以上の値 サンプル数
 * @retval      -1 エラー値
 * @pre
 * - frame == 2500 || frame == 5000 || frame == 10000 || frame == 20000
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * @details
 * 本関数から -1 が返る場合は、引数が事前条件を満たしていない可能性があります。
 */
int CalculateOpusFrameSampleCount(int frame, int sampleRate) NN_NOEXCEPT;

/**
 * @}
 */

}}  // nn::codec
