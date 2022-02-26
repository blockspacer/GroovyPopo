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

namespace nn { namespace atk { namespace detail {
    namespace fnd {
        class FileStream;
    }
    namespace driver {
        class StreamSoundPlayer;
    }
}}}

namespace nn { namespace atk { namespace detail {

class IStreamDataDecoder
{
public:
    enum DecodeType
    {
        DecodeType_Normal,
        DecodeType_Loop,
        DecodeType_Idling,
        DecodeType_Count
    };

    struct DataInfo
    {
        int channelCount;
        int sampleRate;
        int blockSampleCount;
        size_t blockSize;
        int preSkipSampleCount;
    };

    struct DecodeProfile
    {
        nn::os::Tick decodeTick;
        int          decodedSampleCount;
        nn::os::Tick fsAccessTick;
        std::size_t  fsReadSize;
    };

    struct CacheProfile
    {
        position_t   cacheStartPosition;
        std::size_t  cachedLength;
        position_t   cacheCurrentPosition;
        nn::atk::detail::driver::StreamSoundPlayer* player;
    };

public:
    virtual ~IStreamDataDecoder() NN_NOEXCEPT {}

    virtual bool ReadDataInfo( DataInfo* pOutValue, fnd::FileStream* pFileStream ) NN_NOEXCEPT = 0;
    virtual bool Skip( fnd::FileStream* pFileStream ) NN_NOEXCEPT = 0;
    virtual bool Decode( int16_t* pOutDecodedBufferAddresses[], fnd::FileStream* pFileStream, int channelCount, DecodeType decodeType ) NN_NOEXCEPT = 0;
    virtual void Reset() NN_NOEXCEPT = 0;

    virtual void ResetDecodeProfile() NN_NOEXCEPT = 0;
    virtual DecodeProfile GetDecodeProfile() const NN_NOEXCEPT = 0;
};

class IStreamDataDecoderManager
{
public:
    virtual ~IStreamDataDecoderManager() NN_NOEXCEPT {}

    virtual IStreamDataDecoder* AllocDecoder() NN_NOEXCEPT = 0;
    virtual void FreeDecoder(IStreamDataDecoder* pDecoder) NN_NOEXCEPT = 0;
    virtual int GetStreamFileType() const NN_NOEXCEPT = 0;
    virtual int GetDecodeMode() const NN_NOEXCEPT = 0;

    nn::util::IntrusiveListNode m_Link;
};

}}} // nn::atk::detail

