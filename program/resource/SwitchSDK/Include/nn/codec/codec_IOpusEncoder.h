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

#include <nn/nn_Macro.h>
#include <nn/codec/codec_OpusCommon.h>
#include <nn/codec/codec_OpusEncoderTypes.h>

namespace nn {
namespace codec {

class IOpusEncoder
{
public:
    virtual ~IOpusEncoder() {}
    virtual size_t GetWorkBufferSize(int sampleRate, int channelCount) const NN_NOEXCEPT = 0;
    virtual bool IsInitialized() const NN_NOEXCEPT = 0;
    virtual OpusResult Initialize(int sampleRate, int channelCount, void* buffer, size_t size) NN_NOEXCEPT = 0;
    virtual OpusResult Initialize(int sampleRate, int channelCount, OpusApplication application, void* buffer, size_t size) NN_NOEXCEPT = 0;
    virtual void Finalize() NN_NOEXCEPT = 0;
    virtual int GetSampleRate() const NN_NOEXCEPT = 0;
    virtual int GetChannelCount() const NN_NOEXCEPT = 0;
    virtual OpusResult EncodeInterleaved(size_t* pOutputSize, void* outputBuffer, size_t outputBufferSize, const int16_t* inputBuffer, int inputSampleCountPerChannel) NN_NOEXCEPT = 0;
    virtual void SetBitRate(int bitRate) NN_NOEXCEPT = 0;
    virtual int GetBitRate() const NN_NOEXCEPT = 0;
    virtual void SetBitRateControl(OpusBitRateControl bitRateControl) NN_NOEXCEPT = 0;
    virtual OpusBitRateControl GetBitRateControl() const NN_NOEXCEPT = 0;
    virtual void BindCodingMode(OpusCodingMode mode) NN_NOEXCEPT = 0;
    virtual OpusCodingMode GetCodingMode() const NN_NOEXCEPT = 0;
    virtual void SetWaveformType(OpusWaveformType waveformType) NN_NOEXCEPT = 0;
    virtual OpusWaveformType GetWaveformType() const NN_NOEXCEPT = 0;
    virtual int CalculateFrameSampleCount(int frame) const NN_NOEXCEPT = 0;
    virtual int GetPreSkipSampleCount() const NN_NOEXCEPT = 0;
};

}  // namespace codec
}  // namespace nn
