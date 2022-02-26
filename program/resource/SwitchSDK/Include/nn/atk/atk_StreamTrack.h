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

#include <nn/atk/atk_StreamSoundFileReader.h>
#include <nn/atk/atk_MultiVoice.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

struct StreamChannel
{
    // 関数
    void AppendWaveBuffer( WaveBuffer* pBuffer, bool lastFlag ) NN_NOEXCEPT;
    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        return (m_pVoice != nullptr) ? m_pVoice->GetProcessTick(profile) : nn::os::Tick(0);
    }

    // データ
    void*             m_pBufferAddress;   // ストリームデータバッファ
    MultiVoice*       m_pVoice;
    WaveBuffer        m_WaveBuffer[StreamDataLoadTaskMax];
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN AdpcmContext m_AdpcmContext[StreamDataLoadTaskMax];
    UpdateType   m_UpdateType;
} NN_AUDIO_ALIGNAS_BUFFER_ALIGN;


struct StreamTrack
{
public:
    bool m_ActiveFlag;
    StreamChannel* m_pChannels[WaveChannelMax];

    // データ
    uint8_t channelCount;
    uint8_t volume;
    uint8_t pan;
    uint8_t span;
    uint8_t mainSend;
    uint8_t fxSend[AuxBus_Count];
    uint8_t lpfFreq;
    int8_t biquadType;
    uint8_t biquadValue;
    uint8_t flags; // StreamSoundFile::SurroundMode が入る

    float m_Volume;
    int32_t m_OutputLine;

    OutputParam m_TvParam;
};



} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

