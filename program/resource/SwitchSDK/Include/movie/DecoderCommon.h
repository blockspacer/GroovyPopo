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
#include <nn/os/os_EventTypes.h>

namespace movie {

 /**
 * @brief
 * Enumeration of decoder types
 *
 * @details 
 */
enum DecoderType
{
    DecoderType_Unknown,                                    //!< Unknown
    DecoderType_VideoAvc,                                   //!< "video/avc",
    DecoderType_VideoVp8,                                   //!< "video/x-vnd.on2.vp8",
    DecoderType_VideoVp9,                                   //!< "video/x-vnd.on2.vp9",
    DecoderType_AudioAac,                                   //!< "audio/mp4a-latm",
    DecoderType_AudioVorbis,                                //!< "audio/vorbis",
};

/**
 * @brief
 * Enumeration of Output formats for decoded Audio and Video data
 *
 * @details 
 */
enum DecoderOutputFormat
{
    DecoderOutputFormat_None,                              //!< None 
    DecoderOutputFormat_AudioPcm16,                        //!< Audio output format - PCM 16 bit
    DecoderOutputFormat_VideoColorArgb,                    //!< Video output format - ARGB (8:8:8:8 bit) Deprecated, do not use.
    DecoderOutputFormat_VideoColorAbgr,                    //!< Video output format - ABGR (8:8:8:8 bit) 
    DecoderOutputFormat_VideoColorNv12,                    //!< Video output format - NV12
};

/**
 * @brief 
 * Structure to hold DecoderEvents.
 *
 * @details 
 * Application creates and registers "nn::os::EventType" events with decoder. When input buffers are available
 * "inputBufferAvailableEvent" event will be signaled. Application need to use GetEmptyInputBuffer() API to
 * get available input buffer. A buffer data pointer and the index of buffer will be returned. Application 
 * fills the compressed data and use SendFilledInputBufferForDecode() API to send it to decoder for decoding.
 * When decoded buffers are ready, decoder will signal "outputBufferAvailableEvent" event. Application can 
 * use GetDecodedOutputBuffer() to get available decoded output buffer and the buffer index. When the buffer 
 * is consumed and no more needed, release buffer back to decoder by using ReleaseOutputBuffer() API.
 * Decoder signals "formatChangedEvent" event if the decoder output format is changed. When there is any 
 * decoder error "errorEvent" is signaled.
 */
struct DecoderEvents
{
    nn::os::EventType* inputBufferAvailableEvent;
    nn::os::EventType* outputBufferAvailableEvent;
    nn::os::EventType* formatChangedEvent;
    nn::os::EventType* errorEvent;
};

/**
 * @brief
 * Enumeration of Video decoder operating mode
 */
enum DecoderMode
{
    DecoderMode_Unknown      = 0x00,                       //!< Unknown decoder mode.
                                                           //!< If this mode is set, decoder will fail.
    DecoderMode_Cpu          = 0x01,                       //!< Decoder uses CPU accesible buffers.
                                                           //!< This is slower path, pixel data is copied using CPU.
    DecoderMode_NativeTexture = 0x02,                      //!< Decoder uses native buffers for output.                                                                                                                                                               
                                                           //!< Hardware copies data from native buffer to Texture.
                                                           //!< When texture is created NVN_TEXTURE_FLAGS_VIDEO_DECODE_BIT flag need to be passed to NVN texture builder.
                                                           //!< NVNtextureBuilder builder;
                                                           //!< nvnTextureBuilderSetFlags(&builder, NVN_TEXTURE_FLAGS_VIDEO_DECODE_BIT);
};

}  // namespace movie
