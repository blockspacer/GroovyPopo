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

namespace nn {
namespace codec {

class IOpusDecoder
{
public:
    virtual ~IOpusDecoder() {}
    virtual size_t GetWorkBufferSize(int sampleRate, int channelCount) const NN_NOEXCEPT = 0;
    virtual bool IsInitialized() const NN_NOEXCEPT = 0;
    virtual OpusResult Initialize(int sampleRate, int channelCount, void* buffer, size_t size) NN_NOEXCEPT = 0;
    virtual void Finalize() NN_NOEXCEPT = 0;
    virtual int GetSampleRate() const NN_NOEXCEPT = 0;
    virtual int GetChannelCount() const NN_NOEXCEPT = 0;
    virtual void Reset() NN_NOEXCEPT = 0;
    virtual OpusResult DecodeInterleaved(size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT = 0;
};

}  // namespace codec
}  // namespace nn
