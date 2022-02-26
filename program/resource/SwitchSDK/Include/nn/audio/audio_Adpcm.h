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

#include <cstdint>
#include <cstddef>
#include <nn/audio/audio_Common.h>

namespace nn {
namespace audio {

/**
 * @brief       ADPCM の係数パラメータ情報です。
 */
struct AdpcmParameter
{
    uint16_t coefficients[8 * 2];
};

/**
 * @brief       ADPCM のコンテキスト情報です。
 */
struct AdpcmContext
{
    uint16_t predScale;
    int16_t history[2];
};

/**
 * @brief       ADPCM ファイルのヘッダサイズです。
 */
const size_t AdpcmHeaderSize = 96;

/**
 * @brief       APDCM のフレームあたりのサンプル数です。
 */
const int AdpcmFrameSampleCount = 14;

/**
 * @brief       APDCM のフレームあたりのサイズです。
 */
const size_t AdpcmFrameSize = 8;

/**
 * @brief       ADPCM ヘッダ情報です。
 */
struct AdpcmHeaderInfo
{
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN nn::audio::AdpcmParameter parameter;    //!< ADPCM の係数パラメータ情報
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN nn::audio::AdpcmContext loopContext;    //!< ループ再生で利用するコンテキスト情報
    int32_t sampleCount;                    //!< サンプル数
    int32_t sampleRate;                     //!< サンプルレート
    bool loop;                              //!< ループ情報の有無
    int32_t loopBeginSamplePosition;        //!< ループ開始位置
    int32_t loopEndSamplePosition;          //!< ループ終了位置
};

/**
 * @name        その他
 * @{
 */

/**
 * @brief       ADPCM ヘッダ情報を解析します。
 * @param[out]  pOutInfo                ADPCM ヘッダ情報
 * @param[in]   header                  ヘッダデータ
 * @param[in]   size                    ヘッダデータサイズ
 * @pre
 * - pOutInfo != nullptr
 * - header != nullptr
 * - header が sizeof(int32_t) にアラインしている
 * - size >= nn::audio::AdpcmHeaderSize
 */
void ParseAdpcmHeader(AdpcmHeaderInfo* pOutInfo, const void* header, std::size_t size) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
