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

#include <nn/util/util_BinTypes.h>
#include <nn/atk/atk_Global.h>      // AdshrCurve
#include <nn/atk/atk_BankFile.h>

namespace nn {
namespace atk {
namespace detail {

struct VelocityRegionInfo
{
    uint32_t waveArchiveId;      // 波形アーカイブ ID
    uint32_t waveIndex;          // 波形アーカイブ内インデックス

    float pitch;
    AdshrCurve adshrCurve;

    uint8_t originalKey;
    uint8_t volume;
    uint8_t pan;
    bool isIgnoreNoteOff;   // true: ノートオフを無視する (＝パーカッションモード)
    uint8_t keyGroup;
    uint8_t interpolationType;
};

class BankFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'B', 'N', 'K' );

    BankFileReader() NN_NOEXCEPT;
    explicit BankFileReader( const void* bankFile ) NN_NOEXCEPT;
    void Initialize( const void* bankFile ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT { return m_IsInitialized; }

    bool ReadVelocityRegionInfo(
            VelocityRegionInfo* info,
            int programNo,
            int key,
            int velocity
    ) const NN_NOEXCEPT;

    const Util::WaveIdTable* GetWaveIdTable() const NN_NOEXCEPT;
    const void* GetBankFileAddress() const NN_NOEXCEPT { return m_pHeader; }

    // デバッグ用
    int GetInstrumentCount() const NN_NOEXCEPT
    {
        if ( m_IsInitialized == false )
        {
            return 0;
        }
        return m_pInfoBlockBody->GetInstrumentCount();
    }

private:
    const BankFile::FileHeader*     m_pHeader;
    const BankFile::InfoBlockBody*  m_pInfoBlockBody;
    bool m_IsInitialized;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

