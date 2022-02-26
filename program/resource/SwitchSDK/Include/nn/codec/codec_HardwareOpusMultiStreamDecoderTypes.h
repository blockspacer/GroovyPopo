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

namespace nn { namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       HardwareOpusMultiStreamDecoder を扱うための構造体です。
 */
struct HardwareOpusMultiStreamDecoderType
{
    void* _handle;
    void* _buffer;
    size_t _size;
    int _sampleRate;
    int _channelCount;
    int _totalStreamCount;
    int _stereoStreamCount;
    int64_t _perf;
    bool _isResetRequested;
};

/**
 * @}
 */

}}  // namespace nn::codec
