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

#include <nn/atk/atk_StreamSoundPrefetchFile.h>
#include <nn/atk/atk_StreamSoundFileReader.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/detail/atk_IRegionInfoReadable.h>

namespace nn {
namespace atk {
namespace detail {

class StreamSoundPrefetchFileReader : public IRegionInfoReadable
{
public:
    struct PrefetchDataInfo
    {
        uint32_t startFrame;
        uint32_t prefetchSize;
        const void* dataAddress;
    };

    StreamSoundPrefetchFileReader() NN_NOEXCEPT;
    virtual ~StreamSoundPrefetchFileReader() NN_NOEXCEPT;
    void Initialize( const void* streamSoundPrefetchFile ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_pHeader != NULL; }

    bool IsIncludeRegionInfo() const NN_NOEXCEPT;
    bool IsCrc32CheckAvailable() const NN_NOEXCEPT;
    bool IsRegionIndexCheckAvailable() const NN_NOEXCEPT;

    bool IsValidFileHeader( const void* streamSoundFile ) const NN_NOEXCEPT;

    bool ReadStreamSoundInfo( StreamSoundFile::StreamSoundInfo* strmInfo ) const NN_NOEXCEPT;
    bool ReadDspAdpcmChannelInfo(
            DspAdpcmParam* param,
            DspAdpcmLoopParam* loopParam,
            int channelIndex ) const NN_NOEXCEPT;
    bool ReadPrefetchDataInfo( PrefetchDataInfo* pDataInfo, int prefetchIndex ) const NN_NOEXCEPT;

    virtual bool ReadRegionInfo(StreamSoundFile::RegionInfo* info, uint32_t regionIndex) const NN_NOEXCEPT NN_OVERRIDE;

    inline uint32_t GetChannelCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pInfoBlockBody );
        return m_pInfoBlockBody->GetChannelInfoTable()->GetChannelCount();
    }

    inline uint32_t GetPrefetchDataCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pPrefetchDataBlockBody );
        return m_pPrefetchDataBlockBody->GetPrefetchDataCount();
    }

private:
    uint32_t GetRegionDataOffset() const NN_NOEXCEPT;
    uint16_t GetRegionInfoBytes() const NN_NOEXCEPT;

private:
    const StreamSoundPrefetchFile::FileHeader*            m_pHeader;
    const StreamSoundFile::InfoBlockBody*                 m_pInfoBlockBody;
    const StreamSoundPrefetchFile::PrefetchDataBlockBody* m_pPrefetchDataBlockBody;
    uint32_t m_RegionDataOffset;
    uint16_t m_RegionInfoBytes;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

