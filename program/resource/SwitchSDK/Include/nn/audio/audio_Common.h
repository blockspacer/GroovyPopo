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
#include <nn/nn_Macro.h>

namespace nn { namespace audio {

/**
 * @brief       VoiceType / SubMixType / FinalMixType のオーディオレンダラ上での ID です。
 */
typedef uint32_t NodeId;

/**
 * @brief       オーディオ用バッファのアライメントサイズです。
 */
const int BufferAlignSize = 64;

/**
 * @brief   オーディオ用バッファのアライメントを指定するマクロです。
 */
#define NN_AUDIO_ALIGNAS_BUFFER_ALIGN NN_ALIGNAS(64)

/**
 * @brief nn::audio における出力チャンネルとインデックスの関係を表します。
 */
enum ChannelMapping
{
    ChannelMapping_FrontLeft = 0,        //!< 左前チャンネルを表します。
    ChannelMapping_FrontRight = 1,       //!< 右前チャンネルを表します。
    ChannelMapping_FrontCenter = 2,      //!< 中央前チャンネルを表します。
    ChannelMapping_LowFrequency = 3,     //!< 低音用チャンネルを表します。
    ChannelMapping_RearLeft = 4,         //!< 左後チャンネルを表します。
    ChannelMapping_RearRight = 5,        //!< 右後チャンネルを表します。
};

}}  // namespace nn::audio
