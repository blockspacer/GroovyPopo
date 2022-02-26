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

#include <nn/atk/atk_WaveSoundFile.h>
#include <nn/atk/atk_Global.h>      // AuxBus_Count
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace atk {
namespace detail {

struct WaveSoundInfo;
struct WaveSoundNoteInfo;

class WaveSoundFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'W', 'S', 'D' );

    explicit WaveSoundFileReader( const void* waveSoundFile ) NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_pHeader != NULL; }

    uint32_t GetWaveSoundCount() const NN_NOEXCEPT;
    uint32_t GetNoteInfoCount( uint32_t index ) const NN_NOEXCEPT;
    uint32_t GetTrackInfoCount( uint32_t index ) const NN_NOEXCEPT;

    bool ReadWaveSoundInfo( WaveSoundInfo* dst, uint32_t index ) const NN_NOEXCEPT;
    bool ReadNoteInfo( WaveSoundNoteInfo* dst, uint32_t index, uint32_t noteIndex ) const NN_NOEXCEPT;

private:
    bool IsFilterSupportedVersion() const NN_NOEXCEPT;

private:
    const WaveSoundFile::FileHeader*    m_pHeader;
    const WaveSoundFile::InfoBlockBody* m_pInfoBlockBody;
};

struct WaveSoundInfo
{
    float pitch;
    AdshrCurve adshr;
    uint8_t pan;
    uint8_t surroundPan;
    uint8_t mainSend;
    uint8_t fxSend[ AuxBus_Count ];
    uint8_t lpfFreq;
    uint8_t biquadType;
    uint8_t biquadValue;
};

struct WaveSoundNoteInfo
{
    uint32_t waveArchiveId;
    int32_t waveIndex;
    AdshrCurve adshr;
    uint8_t originalKey;
    uint8_t pan;
    uint8_t surroundPan;
    uint8_t volume;
    float pitch;
    // TODO: SendValue を入れる？ (バイナリには入れれる)
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
