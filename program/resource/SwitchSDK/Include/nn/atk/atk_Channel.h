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

#include <nn/atk/atk_Global.h>  // WaveInfo
#include <nn/atk/atk_MultiVoice.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/atk_CurveAdshr.h>
#include <nn/atk/atk_CurveLfo.h>
#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_HardwareManager.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

/* ========================================================================
        class definition
   ======================================================================== */

class Channel
{
    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    enum LfoTarget
    {
        LfoTarget_Pitch,
        LfoTarget_Volume,
        LfoTarget_Pan,
        LfoTarget_Invalid = 255 // uint8_t の最大値
    };

    enum ChannelCallbackStatus
    {
        ChannelCallbackStatus_Stopped,
        ChannelCallbackStatus_Drop,
        ChannelCallbackStatus_Finish,
        ChannelCallbackStatus_Cancel
    };

    typedef void (*ChannelCallback) (
        Channel *channel,
        ChannelCallbackStatus status,
        void* userData );

    /* ------------------------------------------------------------------------
            constant declaration
       ------------------------------------------------------------------------ */
public:
    static const int ChannelCount          = HardwareManager::AtkVoiceCountMax + 1; // チャンネル数
    static const int ChannelMin            = 0;
    static const int ChannelMax            = ChannelMin + ChannelCount - 1;

    static const int PriorityRelease       = 1;

    private:
    static const int KeyInit               = 60;
    static const int OriginalKeyInit       = 60;
    static const uint8_t SilenceVolumeMax  = 255;
    static const uint8_t SilenceVolumeMin  = 0;

    static const float SilenceVolumeMaxR;

    /* ------------------------------------------------------------------------
            static members
       ------------------------------------------------------------------------ */
public:
    static Channel* AllocChannel(
        int voiceChannelCount,
        int priority,
        ChannelCallback callback,
        void* callbackData
    ) NN_NOEXCEPT;
    static void FreeChannel( Channel* channel ) NN_NOEXCEPT;
    static void DetachChannel( Channel* channel ) NN_NOEXCEPT;

private:
    static void VoiceCallbackFunc(
        MultiVoice* voice,
        MultiVoice::VoiceCallbackStatus status,
        void* arg
    ) NN_NOEXCEPT;

    static const int WaveBufferMax = 2;

    /* ------------------------------------------------------------------------
            class members
       ------------------------------------------------------------------------ */
public:
    Channel() NN_NOEXCEPT;
    ~Channel() NN_NOEXCEPT;

    void Update( bool doPeriodicProc ) NN_NOEXCEPT;
    void CallChannelCallback( ChannelCallbackStatus status ) NN_NOEXCEPT;

    void Start( const WaveInfo& waveParam, int length, position_t startOffsetSamples, bool isContextCalculationSkipMode ) NN_NOEXCEPT;
    void Stop() NN_NOEXCEPT;
    void Pause( bool flag ) NN_NOEXCEPT { m_PauseFlag = flag; m_pVoice->Pause( flag ); }

    void NoteOff() NN_NOEXCEPT;
    void RequestRelease() NN_NOEXCEPT;
    void Release() NN_NOEXCEPT;

    bool IsActive() const NN_NOEXCEPT { return m_ActiveFlag != 0; }
    bool IsPause() const NN_NOEXCEPT { return m_PauseFlag != 0; }

    //------------------------------------------------------------------
    // 初期パラメータ
    void SetKey( uint8_t key ) NN_NOEXCEPT { m_Key = key; }
    void SetKey( uint8_t key, uint8_t originalKey ) NN_NOEXCEPT { m_Key = key; m_OriginalKey = originalKey; }
    void SetInitPan( float pan ) NN_NOEXCEPT { m_InitPan = pan; }
    void SetInitSurroundPan( float surroundPan ) NN_NOEXCEPT { m_InitSurroundPan = surroundPan; }
    void SetTune( float tune ) NN_NOEXCEPT { m_Tune = tune; }

    void SetAttack( int attack ) NN_NOEXCEPT { m_CurveAdshr.SetAttack( attack ); }
    void SetHold( int hold ) NN_NOEXCEPT { m_CurveAdshr.SetHold( hold ); }
    void SetDecay( int decay ) NN_NOEXCEPT { m_CurveAdshr.SetDecay( decay ); }
    void SetSustain( int sustain ) NN_NOEXCEPT { m_CurveAdshr.SetSustain( sustain ); }
    void SetRelease( int release ) NN_NOEXCEPT { m_CurveAdshr.SetRelease( release ); }

    void SetSilence( bool silenceFlag, int fadeTimes ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( fadeTimes >= 0 && fadeTimes <= USHRT_MAX );
        m_SilenceVolume.SetTarget(
            silenceFlag? SilenceVolumeMin : SilenceVolumeMax,
            static_cast<uint16_t>( fadeTimes )
        );
    }

    int GetLength() const NN_NOEXCEPT { return m_Length; }
    void SetLength( int32_t length ) NN_NOEXCEPT { m_Length = length; }
    bool IsRelease() const NN_NOEXCEPT { return m_CurveAdshr.GetStatus() == CurveAdshr::Status_Release; }

    //------------------------------------------------------------------
    // ユーザーパラメータ
    void SetUserVolume( float volume ) NN_NOEXCEPT { m_UserVolume = volume; }
    void SetUserPitch( float pitch ) NN_NOEXCEPT { m_UserPitch = pitch; }
    void SetUserPitchRatio( float pitchRatio ) NN_NOEXCEPT { m_UserPitchRatio = pitchRatio; }

    void SetUserLpfFreq( float lpfFreq ) NN_NOEXCEPT { m_UserLpfFreq = lpfFreq; }
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;
    void SetLfoParam( const CurveLfoParam& param, int i = 0 ) NN_NOEXCEPT { m_Lfo[i].SetParam( param ); }
    void SetLfoTarget( LfoTarget type, int i = 0 ) NN_NOEXCEPT { m_LfoTarget[i] = static_cast<uint8_t>(type); }
    void SetPriority(int priority) NN_NOEXCEPT;
    void SetReleasePriorityFix( bool fix ) NN_NOEXCEPT { m_ReleasePriorityFixFlag = fix; }
    void SetIsIgnoreNoteOff( bool flag ) NN_NOEXCEPT { m_IsIgnoreNoteOff = flag; }

    void SetSweepParam( float sweepPitch, int sweepTime, bool autoUpdate ) NN_NOEXCEPT;
    bool IsAutoUpdateSweep() const NN_NOEXCEPT { return m_AutoSweep != 0 ; }
    void UpdateSweep( int count ) NN_NOEXCEPT;

    void SetPanMode( PanMode panMode ) NN_NOEXCEPT { m_PanMode = panMode; }
    void SetPanCurve( PanCurve panCurve ) NN_NOEXCEPT { m_PanCurve = panCurve; }

    //------------------------------------------------------------------
    // 出力パラメータ
    void SetOutputLine( uint32_t lineFlag ) NN_NOEXCEPT { m_OutputLineFlag = lineFlag; }
    uint32_t GetOutputLine() const NN_NOEXCEPT { return m_OutputLineFlag; }


    void SetTvParam( const OutputParam& param ) NN_NOEXCEPT { m_TvParam = param; }
    const OutputParam& GetTvParam() const NN_NOEXCEPT { return m_TvParam; }

    void SetTvAdditionalParamAddr( OutputAdditionalParam* pParam) NN_NOEXCEPT { m_pTvAdditionalParam = pParam; }
    const OutputAdditionalParam* const GetTvAdditionalParamAddr() const NN_NOEXCEPT { return m_pTvAdditionalParam; }
    OutputAdditionalParam* GetTvAdditionalParamAddr() NN_NOEXCEPT { return m_pTvAdditionalParam; }
    void SetTvAdditionalParam( const OutputAdditionalParam& param ) NN_NOEXCEPT;

    // SoundMaker 用
    void SetMainSend( float send ) NN_NOEXCEPT { m_TvParam.send[Util::GetSubMixBusFromMainBus()] = send; }
    void SetUserPan( float pan ) NN_NOEXCEPT { m_TvParam.pan = pan; }

    //------------------------------------------------------------------
    Channel* GetNextTrackChannel() const NN_NOEXCEPT { return m_pNextLink; }
    void SetNextTrackChannel( Channel* channel ) NN_NOEXCEPT { m_pNextLink = channel; }

    position_t GetCurrentPlayingSample(bool isOriginalSamplePosition) const NN_NOEXCEPT;

    void SetKeyGroupId( uint8_t id ) NN_NOEXCEPT { m_KeyGroupId = id; }
    uint8_t GetKeyGroupId() const NN_NOEXCEPT { return m_KeyGroupId; }

    void SetInterpolationType( uint8_t type ) NN_NOEXCEPT { m_InterpolationType = type; }
    uint8_t GetInterpolationType() const NN_NOEXCEPT { return m_InterpolationType; }

    void SetInstrumentVolume( float instrumentVolume ) NN_NOEXCEPT { m_InstrumentVolume = instrumentVolume; }
    void SetVelocity( float velocity ) NN_NOEXCEPT { m_Velocity = velocity; }

    static const int ModCount = 4;

    void SetUpdateType(UpdateType updateType) NN_NOEXCEPT;
    UpdateType GetUpdateType() const NN_NOEXCEPT
    {
        return m_pVoice != nullptr ? m_pVoice->GetUpdateType() : UpdateType_AudioFrame;
    }
    void SetOutputReceiver(OutputReceiver* pOutputReceiver) NN_NOEXCEPT;
    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        return m_pVoice != nullptr ? m_pVoice->GetProcessTick(profile) : nn::os::Tick(0);
    }
private:
    class Disposer : public DisposeCallback
    {
      public:
        Disposer() NN_NOEXCEPT : m_pChannel(NULL) {}
        void Initialize(Channel* channel) NN_NOEXCEPT
        {
            m_pChannel = channel;
        }
        virtual ~Disposer() NN_NOEXCEPT {}
        virtual void InvalidateData( const void* start, const void* end ) NN_NOEXCEPT NN_OVERRIDE;
      private:
        Channel* m_pChannel;
    };
    friend class Disposer;

    float GetSweepValue() const NN_NOEXCEPT;
    void InitParam( ChannelCallback callback, void* callbackData ) NN_NOEXCEPT;
    void AppendWaveBuffer( const WaveInfo& waveInfo, position_t startOffsetSamples, bool isContextCalculationSkipMode ) NN_NOEXCEPT;

    Disposer m_Disposer;
    CurveAdshr m_CurveAdshr;

    CurveLfo m_Lfo[ModCount];
    uint8_t m_LfoTarget[ModCount]; // enum LfoTarget

    uint8_t m_PauseFlag;
    uint8_t m_ActiveFlag;
    uint8_t m_AllocFlag;
    uint8_t m_AutoSweep;
    uint8_t m_ReleasePriorityFixFlag;
    uint8_t m_IsIgnoreNoteOff;
    uint8_t m_BiquadType;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[1];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

    float m_UserVolume;
    float m_UserPitchRatio;
    float m_UserLpfFreq;
    float m_BiquadValue;
    uint32_t m_OutputLineFlag;

    OutputParam m_TvParam;
    OutputAdditionalParam* m_pTvAdditionalParam;

    float m_UserPitch;
    float m_SweepPitch;
    int m_SweepCounter;
    int m_SweepLength;

    float m_InitPan;
    float m_InitSurroundPan;
    float m_Tune;
    MoveValue<uint8_t, uint16_t> m_SilenceVolume;

    float m_Cent;
    float m_CentPitch;

    int m_Length;

    PanMode m_PanMode;
    PanCurve m_PanCurve;

    uint8_t m_Key;
    uint8_t m_OriginalKey;
    uint8_t m_KeyGroupId;
    uint8_t m_InterpolationType;

    float m_InstrumentVolume;
    float m_Velocity;

    ChannelCallback m_Callback;
    void* m_CallbackData;

    MultiVoice* m_pVoice;

    Channel* m_pNextLink;

    WaveBuffer m_WaveBuffer[WaveChannelMax][WaveBufferMax];
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN AdpcmContext m_AdpcmContext[WaveChannelMax];
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN AdpcmContext m_AdpcmLoopContext[WaveChannelMax];
    position_t m_LoopStartFrame;
    position_t m_OriginalLoopStartFrame;
    bool m_LoopFlag;

    bool m_IsReleaseRequested;
    bool m_IsUpdated;

public:
    util::IntrusiveListNode m_Link;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

