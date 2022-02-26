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
 * @brief       Adpcm データに関するデータ型を定義します。
 */

#include <cstdint>
#include <cstddef>

namespace nn {
namespace codec {

/**
 * @brief       Adpcm データのフレームあたりのバイトサイズです。
 */
const std::size_t AdpcmFrameSize = 8;

/**
 * @brief       Adpcm データのフレームあたりのサンプル数です。
 */
const int AdpcmFrameSampleCount = 14;

const int AdpcmPredictionOrder = 2;
const int AdpcmCoefficientsTableCount = 8;

/**
 * @brief       Adpcm の係数パラメータ情報です。
 */
struct AdpcmParameter
{
    int16_t _coefficients[AdpcmCoefficientsTableCount * AdpcmPredictionOrder];
};

/**
 * @brief       Adpcm のコンテキスト情報です。
 */
struct AdpcmContext
{
    uint16_t _predScale;
    int16_t _history[AdpcmPredictionOrder];
};

}  // namespace codec
}  // namespace nn
