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

#include <nn/util/util_BinaryFormat.h>
#include <nn/atk/detail/atk_BinaryTypes.h>

namespace nn { namespace atk { namespace detail {

/*
    高機能版ウェーブサウンドファイル (.bawsd) の構造

    bawsd
     |
     +-- FileHeader (nn::util::BinaryFileHeader)
     +-- InfoBlock
          |
          +-- BlockBlockHeader (nn::util::BinaryBlockHeader)
          +-- InfoBlockBody
               |
               +-- uint32_t offsetToTrackTableReference   // ref to TrackTable
               +-- Table<ref to WaveSoundTrack>
                    |
                    +-- uint32_t waveSoundTrackCount;
                    +-- WaveSoundTrack[0] --+
                    +-- WaveSoundTrack[1]   |
                    +-- ...                 |
                                            |
                        +-------------------+
                        |
                        +-->WaveSoundTrack
                             |
                             +-- uint32_t offsetToCurveTableReference;   // Reserved
                             +-- uint32_t offsetToClipTableReference   // ref to ClipTable
                             +-- Table<ref to WaveSoundClip>
                                  |
                                  +-- uint32_t waveSoundClipCount;
                                  +-- WaveSoundClip[0] --+
                                  +-- WaveSoundClip[1]   |
                                  +-- ...                |
                                                         |
                        +--------------------------------+
                        |
                        +-->WaveSoundClip
                             |
                             +-- uint32_t waveIndex
                             +-- uint32_t position
                             +-- uint32_t duration
                             +-- uint32_t startOffset
                             +-- float    pitch
                             +-- uint8_t  volume
                             +-- uint8_t  pan
                             +-- uint8_t  padding[2]

*/
    struct AdvancedWaveSoundFile
    {
        struct WaveSoundClip
        {
            uint32_t waveIndex;
            uint32_t position;
            uint32_t duration;
            uint32_t startOffset;
            float pitch;
            uint8_t volume;
            uint8_t pan;
            uint8_t padding[2];
        };

        struct WaveSoundTrack
        {
            const BinaryTypes::ReferenceTable& GetClipReferenceTable() const NN_NOEXCEPT;
            inline int GetClipCount() const NN_NOEXCEPT
            {
                return GetClipReferenceTable().count;
            }

            const WaveSoundClip& GetWaveSoundClip( int index ) const NN_NOEXCEPT;

            uint32_t offsetToCurveTableReference;   // Reserved
            uint32_t offsetToClipTableReference;
            BinaryTypes::Reference toClipTable;
        };


        struct InfoBlockBody
        {
            const BinaryTypes::ReferenceTable& GetTrackReferenceTable() const NN_NOEXCEPT;
            inline int GetTrackCount() const NN_NOEXCEPT
            {
                return GetTrackReferenceTable().count;
            }

            const WaveSoundTrack& GetWaveSoundTrack( int index ) const NN_NOEXCEPT;

            uint32_t offsetToTrackTableReference;
            BinaryTypes::Reference toTrackTable;
        };

        struct InfoBlock
        {
            nn::util::BinaryBlockHeader blockHeader;
            InfoBlockBody body;
        };

        const InfoBlock* GetBlock() const NN_NOEXCEPT;

        nn::util::BinaryFileHeader fileHeader;
    };

}}} // namespace nn::atk::detail
