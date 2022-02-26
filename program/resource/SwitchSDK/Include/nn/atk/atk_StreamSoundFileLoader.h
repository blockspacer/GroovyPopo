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

#include <nn/atk/detail/atk_IRegionInfoReadable.h>

namespace nn {
namespace atk {
namespace detail {

    class StreamSoundFileReader;

namespace fnd
{
    class FileStream;
}

/* ========================================================================
        StreamSoundFileLoader class
   ======================================================================== */

class StreamSoundFileLoader : public IRegionInfoReadable
{
public:
    StreamSoundFileLoader() NN_NOEXCEPT
        : m_pStream( nullptr )
        , m_SeekBlockOffset(0)
        , m_RegionDataOffset(0)
        , m_RegionInfoBytes(0)
    {}

    explicit StreamSoundFileLoader( nn::atk::detail::fnd::FileStream* stream ) NN_NOEXCEPT
    : m_pStream( stream )
    {}

    void Initialize( nn::atk::detail::fnd::FileStream* stream ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(stream);
        m_pStream = stream;
        m_SeekBlockOffset = 0;
        m_RegionDataOffset = 0;
        m_RegionInfoBytes = 0;
    }
    void Finalize() NN_NOEXCEPT
    {
        m_RegionInfoBytes = 0;
        m_RegionDataOffset = 0;
        m_SeekBlockOffset = 0;
        m_pStream = nullptr;
    }

    bool LoadFileHeader( StreamSoundFileReader* reader, void* buffer, unsigned long size ) NN_NOEXCEPT;

    bool ReadSeekBlockData( uint16_t* yn1, uint16_t* yn2, int blockIndex, int channelCount ) NN_NOEXCEPT;
    virtual bool ReadRegionInfo( StreamSoundFile::RegionInfo* info, uint32_t regionIndex ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    nn::atk::detail::fnd::FileStream* m_pStream;
    uint32_t m_SeekBlockOffset;
    uint32_t m_RegionDataOffset;
    uint16_t m_RegionInfoBytes;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

