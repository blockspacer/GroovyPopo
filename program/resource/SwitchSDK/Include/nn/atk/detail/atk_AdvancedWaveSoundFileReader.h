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

#include <nn/atk/detail/atk_AdvancedWaveSoundFile.h>

namespace nn { namespace atk { namespace detail {

    namespace driver {
        class Channel;
    }

    struct AdvancedWaveSoundClipInfo;
    struct AdvancedWaveSoundTrackInfo;
    struct AdvancedWaveSoundTrackInfoSet;

    class AdvancedWaveSoundFileReader
    {
    public:
        explicit AdvancedWaveSoundFileReader( const void* pFile ) NN_NOEXCEPT;

        bool IsAvailable() const NN_NOEXCEPT { return m_pInfoBlockBody != NULL; }

        int GetWaveSoundTrackCount() const NN_NOEXCEPT;
        int GetWaveSoundClipCount( int trackIndex ) const NN_NOEXCEPT;

        bool ReadWaveSoundTrackInfoSet( AdvancedWaveSoundTrackInfoSet* pTrackInfoSet ) NN_NOEXCEPT;

    private:
        const AdvancedWaveSoundFile::InfoBlockBody* m_pInfoBlockBody;
    };

    struct AdvancedWaveSoundClipInfo
    {
        uint32_t waveIndex;
        uint32_t position;
        uint32_t duration;
        uint32_t startOffset;
        float pitch;
        uint8_t volume;
        uint8_t pan;
    };

    struct AdvancedWaveSoundTrackInfo
    {
        static const int AdvancedWaveSoundClipInfoCountMax = 10;

        int waveSoundClipCount;
        AdvancedWaveSoundClipInfo waveSoundClipInfo[AdvancedWaveSoundClipInfoCountMax];
    };

    struct AdvancedWaveSoundTrackInfoSet
    {
        static const int AdvancedWaveSoundTrackInfoCountMax = 4;

        int waveSoundTrackCount;
        AdvancedWaveSoundTrackInfo waveSoundTrackInfo[AdvancedWaveSoundTrackInfoCountMax];
    };

}}} // namespace nn::atk::detail
