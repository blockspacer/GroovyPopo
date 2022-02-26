﻿/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

namespace nn {
namespace audio {

/**
* @brief       リサンプラーを管理するための構造体です。
*/
struct ResamplerType
{
    /**
     * @deprecated  この定数は廃止予定です。代わりに nn::audio::ResamplerType::ChannelCountMax を使用してください。
     * @brief       チャンネル数の上限
     */
    NN_DEPRECATED static const int MaxChannels = 6;

    static const int ChannelCountMax = 6;     //!< チャンネル数の上限

    /**
     * @brief       設定可能な周波数変換比率の最大値 (32768.0f) を返します。
     */
    static inline float GetConvertRatioMax()
    {
        return 32768.0f;
    }

    /**
     * @brief       設定可能な周波数変換比率の最小値 (1.0f / 8.0f) を返します。
     */
    static inline float GetConvertRatioMin()
    {
        return 1.0f / 8.0f;
    }

    void* _workBuffer;
    size_t _bufferSize;

    int _inSampleRate;
    int _outSampleRate;
    int _channelCount;

};

}  // namespace audio
}  // namespace nn
