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
#include <nn/atk/atk_WaveSoundPlayer.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_Debug.h>
#include <nn/atk/atk_Task.h>

namespace nn {
namespace atk {

/* ========================================================================
        type declaration
   ======================================================================== */

class WaveSoundHandle;

/* ========================================================================
        class definition
   ======================================================================== */

namespace detail {

class WaveSound;

typedef SoundInstanceManager<WaveSound> WaveSoundInstanceManager;

class WaveSound : public BasicSound
{
    friend class nn::atk::WaveSoundHandle;

public:
    NN_ATK_DETAIL_RUNTIME_TYPEINFO(BasicSound)

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    explicit WaveSound( WaveSoundInstanceManager& manager ) NN_NOEXCEPT;

    void Prepare(
            const void* wsdFile,
            const void* waveFile,
            const driver::WaveSoundPlayer::StartInfo& info,
            int8_t waveType = WaveType_Nwwav) NN_NOEXCEPT;

    void RegisterDataLoadTask(
            const driver::WaveSoundLoader::LoadInfo& loadInfo,
            const driver::WaveSoundPlayer::StartInfo& startInfo ) NN_NOEXCEPT;

    bool Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
    void Finalize() NN_NOEXCEPT NN_OVERRIDE;
    bool IsPrepared() const NN_NOEXCEPT NN_OVERRIDE
    {
        if ( m_IsCalledPrepare ) return true;

        if (!IsPlayerAvailable()) return false;
        return m_PlayerInstance.IsPrepared();
    }

    // パラメータ設定
    void InitializeChannelParam(int priority, bool isReleasePriorityFix) NN_NOEXCEPT;
    void SetChannelPriority( int priority ) NN_NOEXCEPT;

    // パラメータ取得
    bool ReadWaveSoundDataInfo( WaveSoundDataInfo* info ) const NN_NOEXCEPT;
    position_t GetPlaySamplePosition(bool isOriginalSamplePosition) const NN_NOEXCEPT;
    inline uint32_t GetChannelCount() const NN_NOEXCEPT { return m_ChannelCount; }

    void SetLoaderManager(driver::WaveSoundLoaderManager& manager) NN_NOEXCEPT
    {
        m_PlayerInstance.SetLoaderManager(&manager);
    }

    // デバッグ関数
    DebugSoundType GetSoundType() const NN_NOEXCEPT { return DebugSoundType_Wavesound; }

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

    WaveSoundHandle* m_pTempSpecialHandle;
    WaveSoundInstanceManager& m_Manager;

    const void* m_pWaveFile;
    int8_t m_WaveType;
    bool m_InitializeFlag;
    bool m_IsCalledPrepare; // Prepare() が呼ばれたかどうか
    uint8_t m_Padding[1];
    uint32_t m_ChannelCount;

    driver::WaveSoundPlayer m_PlayerInstance;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

