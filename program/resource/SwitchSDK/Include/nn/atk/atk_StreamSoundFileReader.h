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

#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Config.h>

namespace nn {
namespace atk {
namespace detail {

class StreamSoundFileReader
{
public:
    struct TrackInfo
    {
        uint8_t volume;
        uint8_t pan;
        uint8_t span;
        uint8_t flags;   // enum StreamSoundFile::SurroundMode が入る
        uint8_t channelCount;
        uint8_t globalChannelIndex[ WaveChannelMax ];

        TrackInfo() NN_NOEXCEPT : volume(0), pan(0), channelCount(0)
        {
            std::memset(globalChannelIndex, 0, sizeof(uint8_t) * WaveChannelMax);
        }
    };

    StreamSoundFileReader() NN_NOEXCEPT;
    void Initialize( const void* streamSoundFile ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_pHeader != NULL; }
    bool IsTrackInfoAvailable() const NN_NOEXCEPT;
    bool IsOriginalLoopAvailable() const NN_NOEXCEPT;
    bool IsCrc32CheckAvailable() const NN_NOEXCEPT;
    bool IsRegionIndexCheckAvailable() const NN_NOEXCEPT;

    static bool IsValidFileHeader( const void* streamSoundFile ) NN_NOEXCEPT;

    bool ReadStreamSoundInfo( StreamSoundFile::StreamSoundInfo* strmInfo ) const NN_NOEXCEPT;
    bool ReadStreamTrackInfo( TrackInfo* trackInfo, int trackIndex ) const NN_NOEXCEPT;
    bool ReadDspAdpcmChannelInfo(
            DspAdpcmParam* param,
            DspAdpcmLoopParam* loopParam,
            int channelIndex ) const NN_NOEXCEPT;

    inline uint32_t GetChannelCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pInfoBlockBody );
        return m_pInfoBlockBody->GetChannelInfoTable()->GetChannelCount();
    }

    inline uint32_t GetTrackCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pInfoBlockBody );
        return m_pInfoBlockBody->GetTrackInfoTable()->GetTrackCount();
    }

    inline uint32_t GetSeekBlockOffset() const NN_NOEXCEPT
    {
        if ( IsAvailable() && m_pHeader->HasSeekBlock() )
        {
            return m_pHeader->GetSeekBlockOffset();
        }
        return 0;
    }

    inline uint32_t GetSampleDataOffset() const NN_NOEXCEPT
    {
        if ( IsAvailable() )
        {
            uint32_t result =  m_pHeader->GetDataBlockOffset() +
                sizeof( BinaryBlockHeader ) +
                m_pInfoBlockBody->GetStreamSoundInfo()->sampleDataOffset.offset;
            return result;
        }
        return 0;
    }

    // ファイル先頭から REGN ブロック内のデータの開始位置までのオフセットを返す
    // ([ブロックヘッダー] および [ブロックヘッダーのうしろからデータ開始位置] も考慮)
    inline uint32_t GetRegionDataOffset() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pInfoBlockBody );
        if ( IsAvailable() && m_pHeader->HasRegionBlock() )
        {
            uint32_t result = m_pHeader->GetRegionBlockOffset() +
                sizeof( BinaryBlockHeader ) +
                m_pInfoBlockBody->GetStreamSoundInfo()->regionDataOffset.offset;
            return result;
        }
        return 0;
    }

    // リージョン情報 1 つあたりのバイト数を返す
    inline uint32_t GetRegionInfoBytes() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pInfoBlockBody );
        uint16_t result = m_pInfoBlockBody->GetStreamSoundInfo()->regionInfoBytes;
        return result;
    }

    static bool IsOriginalLoopAvailableImpl( const StreamSoundFile::FileHeader* pHeader ) NN_NOEXCEPT;
private:
    const StreamSoundFile::FileHeader*      m_pHeader;
    const StreamSoundFile::InfoBlockBody*   m_pInfoBlockBody;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

