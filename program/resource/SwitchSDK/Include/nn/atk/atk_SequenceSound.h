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

#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_SequenceSoundPlayer.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_Task.h>
#include <nn/atk/atk_Debug.h>
#include <nn/atk/atk_PlayerHeapDataManager.h>

namespace nn {
namespace atk {

/* ========================================================================
        type declaration
   ======================================================================== */

class SequenceSoundHandle;

//! @briefprivate
struct SequenceSoundResourceStatus
{
    bool isLoadBanks[SoundArchive::SequenceBankMax];
    bool isLoadWarcs[SoundArchive::SequenceBankMax];

    SequenceSoundResourceStatus() NN_NOEXCEPT
    {
        for ( int i = 0; i < SoundArchive::SequenceBankMax; i++ )
        {
            isLoadBanks[i] = isLoadWarcs[i] = false;
        }
    }

    void Dump() const NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("[SEQ] isLoaded : bank(%d:%d:%d:%d) warc(%d:%d:%d:%d)\n",
                isLoadBanks[0], isLoadBanks[1], isLoadBanks[2], isLoadBanks[3],
                isLoadWarcs[0], isLoadWarcs[1], isLoadWarcs[2], isLoadWarcs[3] );
    }
};

namespace detail {

namespace driver {

class NoteOnCallback;

} // namespace nn::atk:detail::driver

class SequenceSound;

typedef SoundInstanceManager<SequenceSound> SequenceSoundInstanceManager;


class SequenceSound : public BasicSound
{
    friend class nn::atk::SequenceSoundHandle;

public:
    NN_ATK_DETAIL_RUNTIME_TYPEINFO(BasicSound)


    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    static const int BankIndexMin = 0;
    static const int BankIndexMax = 3;
    static const int8_t TransposeMin = -64;
    static const int8_t TransposeMax = 63;
    static const uint8_t VelocityRangeMin = 0;
    static const uint8_t VelocityRangeMax = 127;

    explicit SequenceSound( SequenceSoundInstanceManager& manager ) NN_NOEXCEPT;

    void Setup(
        driver::SequenceTrackAllocator* trackAllocator,
        uint32_t allocTracks,
        driver::NoteOnCallback* callback,
        int channelPriority,
        bool isReleasePriorityFix,
        SequenceUserProcCallback userproc,
        void* userprocArg
    ) NN_NOEXCEPT;

    struct Resource
    {
        const void* seq;
        const void* banks[SoundArchive::SequenceBankMax];   // TODO: バンクの数は SoundArchive への依存ではなく、SequenceSound 側で定義すべき
        const void* warcs[SoundArchive::SequenceBankMax];
        bool warcIsIndividuals[SoundArchive::SequenceBankMax];

        Resource() NN_NOEXCEPT { Initialize(); }
        void Initialize() NN_NOEXCEPT
        {
            seq = NULL;
            for ( int i = 0; i < SoundArchive::SequenceBankMax; i++ )
            {
                banks[i] = NULL;
                warcs[i] = NULL;
                warcIsIndividuals[i] = false;
            }
        }
    };
    void Prepare( const Resource& res,
            const driver::SequenceSoundPlayer::StartInfo& startInfo ) NN_NOEXCEPT;

    void RegisterDataLoadTask(
            const driver::SequenceSoundLoader::LoadInfo& loadInfo,
            const driver::SequenceSoundPlayer::StartInfo& startInfo ) NN_NOEXCEPT;


    bool Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
    void Finalize() NN_NOEXCEPT NN_OVERRIDE;
    bool IsPrepared() const NN_NOEXCEPT NN_OVERRIDE
    {
        if ( m_IsCalledPrepare ) return true;

        if (IsPlayerAvailable() == false)
        {
            return false;
        }
        return m_PlayerInstance.IsPrepared();
    }

    // パラメータ設定
    void SetTempoRatio( float tempoRatio ) NN_NOEXCEPT;
    void SetChannelPriority( int priority ) NN_NOEXCEPT;

    // パラメータ取得
    uint32_t GetTick() const NN_NOEXCEPT;

    // トラックパラメータ設定
    void SetTrackMute( uint32_t trackBitFlag, SequenceMute mute ) NN_NOEXCEPT;
    void SetTrackMute( uint32_t trackBitFlag, bool muteFlag ) NN_NOEXCEPT;
    void SetTrackSilence( uint32_t trackBitFlag, bool silenceFlag, int fadeTimes ) NN_NOEXCEPT;

    void SetTrackBiquadFilter( uint32_t trackBitFlag, int type, float value ) NN_NOEXCEPT;
    void SetTrackBankIndex( uint32_t trackBitFlag, int bankIndex ) NN_NOEXCEPT;

    void SetTrackVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackPitch( uint32_t trackBitFlag, float pitch ) NN_NOEXCEPT;
    void SetTrackLpfFreq( uint32_t trackBitFlag, float lpfFreq ) NN_NOEXCEPT;
    void SetTrackTranspose( uint32_t trackBitFlag, int8_t transpose ) NN_NOEXCEPT;
    void SetTrackVelocityRange( uint32_t trackBitFlag, uint8_t range ) NN_NOEXCEPT;
    void SetTrackOutputLine( uint32_t trackBitFlag, uint32_t lineFlag ) NN_NOEXCEPT;
    void ResetTrackOutputLine( uint32_t trackBitFlag ) NN_NOEXCEPT;

    void SetTrackChannelMixParameter( uint32_t trackBitFlag, uint32_t srcChNo, const MixParameter& mixParam ) NN_NOEXCEPT;
    void SetTrackMainOutVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackPan( uint32_t trackBitFlag, float pan ) NN_NOEXCEPT;
    void SetTrackSurroundPan( uint32_t trackBitFlag, float surroundPan ) NN_NOEXCEPT;
    void SetTrackMainSend( uint32_t trackBitFlag, float send ) NN_NOEXCEPT;
    void SetTrackFxSend( uint32_t trackBitFlag, AuxBus bus, float send ) NN_NOEXCEPT;

    // シーケンス変数
    bool ReadVariable( int varNo, int16_t* var ) const NN_NOEXCEPT;
    static bool ReadGlobalVariable( int varNo, int16_t* var ) NN_NOEXCEPT;
    bool ReadTrackVariable( int trackNo, int varNo, int16_t* var ) const NN_NOEXCEPT;
    void WriteVariable( int varNo, int16_t var ) NN_NOEXCEPT;
    static void WriteGlobalVariable( int varNo, int16_t var ) NN_NOEXCEPT;
    void WriteTrackVariable( int trackNo, int varNo, int16_t var ) NN_NOEXCEPT;

    void SetLoaderManager(driver::SequenceSoundLoaderManager& manager) NN_NOEXCEPT
    {
        m_PlayerInstance.SetLoaderManager(&manager);
    }

    // デバッグ関数
    DebugSoundType GetSoundType() const NN_NOEXCEPT { return DebugSoundType_Seqsound; }

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        return m_PlayerInstance.GetProcessTick(profile);
    }

    util::IntrusiveListNode m_PriorityLink; // for SoundInstanceManager

private:
    bool IsAttachedTempSpecialHandle() NN_NOEXCEPT NN_OVERRIDE;
    void DetachTempSpecialHandle() NN_NOEXCEPT NN_OVERRIDE;
    void OnUpdatePlayerPriority() NN_NOEXCEPT NN_OVERRIDE;

    void OnUpdateParam() NN_NOEXCEPT NN_OVERRIDE {} // あとで消す
    driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() NN_NOEXCEPT NN_OVERRIDE
    {
        return &m_PlayerInstance;
    }

    void Skip( driver::SequenceSoundPlayer::StartOffsetType offsetType, int offset ) NN_NOEXCEPT;

    SequenceSoundHandle* m_pTempSpecialHandle;
    SequenceSoundInstanceManager& m_Manager;

    bool m_InitializeFlag;
    bool m_CanUseTask;
    bool m_IsCalledPrepare; // Prepare() が呼ばれたかどうか
    uint8_t m_Padding[1];

    driver::SequenceSoundPlayer m_PlayerInstance;

};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

