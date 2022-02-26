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

#include <nn/atk/atk_WaveFile.h>
#include <nn/atk/atk_DspadpcmReader.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace atk {
namespace detail {

class WaveFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'W', 'A', 'V' );
    static SampleFormat GetSampleFormat( uint8_t encodeMethod ) NN_NOEXCEPT;

    WaveFileReader( const void* waveFile, int8_t waveType = WaveType_Nwwav ) NN_NOEXCEPT;

    bool IsAvailable() const NN_NOEXCEPT { return m_pHeader != NULL; }

    bool IsOriginalLoopAvailable() const NN_NOEXCEPT;

    bool ReadWaveInfo( WaveInfo* info, const void* waveDataOffsetOrigin = NULL ) const NN_NOEXCEPT;

private:
    const void* GetWaveDataAddress(
            const WaveFile::ChannelInfo* info,
            const void* waveDataOffsetOrigin ) const NN_NOEXCEPT;

    const WaveFile::FileHeader*     m_pHeader;
    const WaveFile::InfoBlockBody*  m_pInfoBlockBody;
    const void*                     m_pDataBlockBody;

    DspadpcmReader m_DspadpcmReader;
    int8_t m_WaveType;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

