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

#include <movie/Utils.h>
#include <movie/DecoderCommon.h>
#include <movie/Status.h>

namespace movie {

class IDecoder
{
public:
    virtual ~IDecoder() {}

    virtual Status Configure(MediaData* config) = 0;

    virtual Status RegisterClientEvents(DecoderEvents* decoderEvents) = 0;

    virtual Status GetOutputFormat(MediaData* format) = 0;

    virtual Status Start() = 0;

    virtual Status Stop() = 0;

    virtual Status Reset() = 0;

    virtual Status Flush() = 0;

    virtual Status AcquireInputBufferIndex(int32_t* index, int32_t* remainingBufferIndices) = 0;

    virtual Status GetInputBuffer(int32_t index, Buffer* buffer) = 0;

    virtual Status SendInputBufferForDecode(int32_t index, int32_t offset, int32_t size, int64_t presentationTimeUs, uint32_t flags) = 0;

    virtual Status AcquireOutputBufferIndex(int32_t* index, int64_t* presentationTimeUs, uint32_t* flags, int32_t* remainingBufferIndices) = 0;

    virtual Status GetOutputBuffer(int32_t index, Buffer* buffer) = 0;

    virtual Status GetOutputBufferSize(int32_t* bufferSize, int32_t* alignment) = 0;

    virtual Status ReleaseOutputBufferIndex(int32_t index) = 0;

    virtual Status GetLastError() = 0;

    virtual Status GetOutputBuffer(int32_t index, Buffer* buffer, MediaData* format) = 0;

    virtual Status GetOutputBufferFormat(int32_t index, MediaData* format) = 0;
};

}  // namespace movie