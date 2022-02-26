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

#include <nn/os/os_Event.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_HardwareManager.h>
#include <nn/atk/atk_PlayerHeapDataManager.h>
#include <nn/atk/atk_ValueArray.h>
#include <nn/atk/atk_OutputAdditionalParam.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

/* ========================================================================
        class definition
   ======================================================================== */

// ------------------------------------------------------------------------
// プレイヤーパラメータセット
struct PlayerParamSet
{
    float volume;
    float pitch;
    float lpfFreq;
    float biquadValue;
    int8_t biquadType;
    PanMode panMode;
    PanCurve panCurve;
    uint32_t outputLineFlag;

    OutputParam tvParam;

    PlayerParamSet() NN_NOEXCEPT { Initialize(); }
    void Initialize() NN_NOEXCEPT;
};

class BasicSoundPlayer
{
public:
    BasicSoundPlayer() NN_NOEXCEPT;
    virtual ~BasicSoundPlayer() NN_NOEXCEPT {};

    virtual void Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT;
    virtual void Finalize() NN_NOEXCEPT;

    virtual void Start() NN_NOEXCEPT = 0;
    virtual void Stop() NN_NOEXCEPT = 0;
    virtual void Pause( bool flag ) NN_NOEXCEPT = 0;

    bool IsActive() const NN_NOEXCEPT { return m_ActiveFlag; }
    bool IsStarted() const NN_NOEXCEPT { return m_StartedFlag; }
    bool IsPause() const NN_NOEXCEPT { return m_PauseFlag; }
    bool IsPlayFinished() const NN_NOEXCEPT { return m_FinishFlag; }
    bool IsFinalizedForCannotAllocateResource() const NN_NOEXCEPT
    {
        return m_IsFinalizedForCannotAllocateResource;
    }

    //------------------------------------------------------------------
    // プレイヤーパラメータ
    void SetVolume( float volume ) NN_NOEXCEPT { m_PlayerParamSet.volume = volume; }
    void SetPitch( float pitch ) NN_NOEXCEPT { m_PlayerParamSet.pitch = pitch; }
    void SetLpfFreq( float lpfFreq ) NN_NOEXCEPT { m_PlayerParamSet.lpfFreq = lpfFreq; }
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;
    void SetPanMode( PanMode mode ) NN_NOEXCEPT { m_PlayerParamSet.panMode = mode; }
    void SetPanCurve( PanCurve curve ) NN_NOEXCEPT { m_PlayerParamSet.panCurve = curve; }

    float GetVolume() const NN_NOEXCEPT { return m_PlayerParamSet.volume; }
    float GetPitch() const NN_NOEXCEPT { return m_PlayerParamSet.pitch; }
    float GetLpfFreq() const NN_NOEXCEPT { return m_PlayerParamSet.lpfFreq; }
    int GetBiquadFilterType() const NN_NOEXCEPT { return m_PlayerParamSet.biquadType; }
    float GetBiquadFilterValue() const NN_NOEXCEPT { return m_PlayerParamSet.biquadValue; }
    PanMode GetPanMode() const NN_NOEXCEPT { return m_PlayerParamSet.panMode; }
    PanCurve GetPanCurve() const NN_NOEXCEPT { return m_PlayerParamSet.panCurve; }

    //------------------------------------------------------------------
    // 出力パラメータ
    void SetOutputLine( uint32_t outputLineFlag ) NN_NOEXCEPT { m_PlayerParamSet.outputLineFlag = outputLineFlag; }
    uint32_t GetOutputLine() const NN_NOEXCEPT { return m_PlayerParamSet.outputLineFlag; }

    void SetTvParam( const OutputParam& param ) NN_NOEXCEPT
    {
        m_PlayerParamSet.tvParam = param;
    }
    const OutputParam& GetTvParam() const NN_NOEXCEPT
    {
        return m_PlayerParamSet.tvParam;
    }

    void SetTvAdditionalParamAddr( OutputAdditionalParam* pParam ) NN_NOEXCEPT
    {
        m_pTvAdditionalParam = pParam;
    }
    const OutputAdditionalParam* const GetTvAdditionalParamAddr() const NN_NOEXCEPT
    {
        return m_pTvAdditionalParam;
    }
    void SetTvAdditionalSend( int bus, float send ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsAdditionalSendEnabled())
        {
            m_pTvAdditionalParam->TrySetAdditionalSend(bus, send);
        }
    }
    void SetTvBusMixVolumeUsed( bool isUsed ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsBusMixVolumeEnabled())
        {
            m_pTvAdditionalParam->SetBusMixVolumeUsed(isUsed);
        }
    }
    void SetTvBusMixVolume( const OutputBusMixVolume& busMixVolume ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsBusMixVolumeEnabled())
        {
            m_pTvAdditionalParam->SetBusMixVolume(busMixVolume);
        }
    }
    void SetTvBusMixVolumeEnabled( int bus, bool isEnabled ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsBusMixVolumeEnabled())
        {
            m_pTvAdditionalParam->SetBusMixVolumeEnabledForBus(bus, isEnabled);
        }
    }
    void SetTvBinaryVolume( float volume ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsVolumeThroughModeEnabled())
        {
            m_pTvAdditionalParam->SetBinaryVolume(volume);
        }
    }
    void SetTvVolumeThroughModeUsed( bool isVolumeThroughModeEnabled ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsVolumeThroughModeEnabled())
        {
            m_pTvAdditionalParam->SetVolumeThroughModeUsed(isVolumeThroughModeEnabled);
        }
    }
    void SetTvVolumeThroughMode( int bus, uint8_t volumeThroughMode ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam != nullptr && m_pTvAdditionalParam->IsVolumeThroughModeEnabled())
        {
            m_pTvAdditionalParam->GetVolumeThroughModePacketAddr()->TrySetVolumeThroughMode(bus, volumeThroughMode);
        }
    }

    void SetTvAdditionalParam( const OutputAdditionalParam& param ) NN_NOEXCEPT
    {
        if(m_pTvAdditionalParam == nullptr)
        {
            return;
        }

        *m_pTvAdditionalParam = param;
    }

    bool TryWaitInstanceFree() NN_NOEXCEPT
    {
        return m_Event.TryWait();
    }

    void WaitInstanceFree() NN_NOEXCEPT
    {
        m_Event.Wait();
    }

    void InitializeEvent() NN_NOEXCEPT
    {
        m_Event.Clear();
    }

    const PlayerHeapDataManager* GetPlayerHeapDataManager() const NN_NOEXCEPT
    {
        return m_pPlayerHeapDataManager;
    }

    void ClearIsFinalizedForCannotAllocatedResourceFlag() NN_NOEXCEPT
    {
        m_IsFinalizedForCannotAllocateResource = false;
    }

protected:
    void SetPlayerHeapDataManagerImpl( const PlayerHeapDataManager* mgr ) NN_NOEXCEPT
    {
        m_pPlayerHeapDataManager = mgr;
    }

    static int32_t ToDelayCount( int32_t delayTimeMilliSec ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(delayTimeMilliSec % HardwareManager::SoundFrameIntervalMsec == 0);
        return delayTimeMilliSec / HardwareManager::SoundFrameIntervalMsec;
    }

    virtual void SetActiveFlag(bool isActive) NN_NOEXCEPT
    {
        m_ActiveFlag = isActive;
    }

    void SetStartedFlag(bool isStarted) NN_NOEXCEPT
    {
        m_StartedFlag = isStarted;
    }

    void SetPauseFlag(bool isPause) NN_NOEXCEPT
    {
        m_PauseFlag = isPause;
    }

    void SetFinishFlag(bool isPlayFinished) NN_NOEXCEPT
    {
        m_FinishFlag = isPlayFinished;
    }

    void SetFinalizedForCannotAllocateResourceFlag(bool isFinalizedForCannotAllocateResource) NN_NOEXCEPT
    {
        m_IsFinalizedForCannotAllocateResource = isFinalizedForCannotAllocateResource;
    }

    inline OutputReceiver* GetOutputReceiver() NN_NOEXCEPT
    {
        return m_pOutputReceiver;
    }

private:
    nn::os::Event m_Event;
    OutputReceiver* m_pOutputReceiver;
    bool m_ActiveFlag; // ディスポースコールバック登録されているかどうか
    bool m_StartedFlag; // プレイヤーコールバック登録されているかどうか
    bool m_PauseFlag;
    bool m_FinishFlag;

    // リソース (各種トラックなど) 不足で Finalizeを呼んだか
    bool m_IsFinalizedForCannotAllocateResource;

private:
    PlayerParamSet m_PlayerParamSet;
    OutputAdditionalParam* m_pTvAdditionalParam;
    const PlayerHeapDataManager* m_pPlayerHeapDataManager;  // SEQ/WSD で利用される
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

