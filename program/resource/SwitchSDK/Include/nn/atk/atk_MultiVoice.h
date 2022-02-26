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

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Voice.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_OutputAdditionalParam.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class MultiVoiceManager;

class MultiVoice
{
private:
    NN_DISALLOW_COPY(MultiVoice);

    friend class nn::atk::detail::driver::MultiVoiceManager;

public:
    // ------------------------------------------------------------------------
    // 更新フラグ
    static const int UpdateStart  = 0x0001 << 0;
    static const int UpdatePause  = 0x0001 << 1;
    static const int UpdateSrc    = 0x0001 << 2;
    static const int UpdateMix    = 0x0001 << 3;
    static const int UpdateLpf    = 0x0001 << 4;
    static const int UpdateBiquad = 0x0001 << 5;
    static const int UpdateVe     = 0x0001 << 6;


    // ------------------------------------------------------------------------
    // MultiVoice のモード
    enum VoiceMode
    {
        VoiceMode_Normal,
        VoiceMode_StereoLeft,
        VoiceMode_StereoRight
    };

    // ------------------------------------------------------------------------
    // コールバック関数
    enum VoiceCallbackStatus
    {
        VoiceCallbackStatus_FinishWave,
        VoiceCallbackStatus_Cancel,
        VoiceCallbackStatus_DropVoice,
        VoiceCallbackStatus_DropDsp
    };

    typedef void (*VoiceCallback)(
        MultiVoice* voice,
        VoiceCallbackStatus status,
        void* callbackData
    );

    // ------------------------------------------------------------------------
    // パラメータ境界
    static const float VolumeMin;
    static const float VolumeDefault;
    static const float VolumeMax;
    static const float PanLeft;
    static const float PanCenter;
    static const float PanRight;
    static const float SpanFront;
    static const float SpanCenter;
    static const float SpanRear;
    static const float CutoffFreqMin;
    static const float CutoffFreqMax;
    static const float BiquadValueMin;
    static const float BiquadValueMax;
    static const float SendMin;
    static const float SendMax;

    static const uint32_t PriorityNoDrop = Voice::PriorityNoDrop;

    MultiVoice() NN_NOEXCEPT;
    explicit MultiVoice(OutputAdditionalParam* pAdditionalParam) NN_NOEXCEPT;
    ~MultiVoice() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 確保と解放
    bool Alloc(
        int channelCount,       // TODO: voiceOutCount を追加する
        int priority,
        MultiVoice::VoiceCallback callback,
        void* callbackData
    ) NN_NOEXCEPT;
    void Free() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 動作
    void Start() NN_NOEXCEPT;
    void Stop() NN_NOEXCEPT;
    void UpdateVoiceStatus() NN_NOEXCEPT;
    void Pause( bool flag ) NN_NOEXCEPT;

    void Calc() NN_NOEXCEPT;
    void Update() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 動作状態
    bool IsActive() const NN_NOEXCEPT { return m_ChannelCount > 0; }
    bool IsRun()    const NN_NOEXCEPT;
    bool IsPause()  const NN_NOEXCEPT { return m_IsPause == true; }
    bool IsPlayFinished() const NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // パラメータ
    void SetSampleFormat( SampleFormat format ) NN_NOEXCEPT;
    void SetSampleRate( int sampleRate ) NN_NOEXCEPT;

    float GetVolume() const NN_NOEXCEPT { return m_Volume; }
    void SetVolume( float volume ) NN_NOEXCEPT;

    float GetPitch() const NN_NOEXCEPT { return m_Pitch; }
    void SetPitch( float pitch ) NN_NOEXCEPT;

    void SetPanMode( PanMode panMode ) NN_NOEXCEPT;
    void SetPanCurve( PanCurve panCurve ) NN_NOEXCEPT;


    float GetLpfFreq() const NN_NOEXCEPT { return m_LpfFreq; }
    void SetLpfFreq( float lpfFreq ) NN_NOEXCEPT;

    int GetBiquadType() const NN_NOEXCEPT { return m_BiquadType; }
    float GetBiquadValue() const NN_NOEXCEPT { return m_BiquadValue; }
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;

    int GetPriority() const NN_NOEXCEPT { return m_Priority; }
    void SetPriority( int priority ) NN_NOEXCEPT;

    void SetInterpolationType( uint8_t interpolationType ) NN_NOEXCEPT;

    void SetVoiceMode(VoiceMode mode) NN_NOEXCEPT;
    VoiceMode GetVoiceMode() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // ボイスパラメータ
    int GetSdkVoiceCount() const NN_NOEXCEPT { return m_ChannelCount; }
    const Voice& detail_GetSdkVoice( int index ) const NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 出力パラメータ
    void SetOutputLine( uint32_t lineFlag ) NN_NOEXCEPT;
    uint32_t GetOutputLine() const NN_NOEXCEPT { return m_OutputLineFlag; }

    void SetTvParam( const OutputParam& param ) NN_NOEXCEPT;
    const OutputParam& GetTvParam() const NN_NOEXCEPT { return m_TvParam; }

    void SetTvAdditionalParam( const OutputAdditionalParam& param ) NN_NOEXCEPT;
    void SetTvAdditionalParam( const SendArray* const pAdditionalSend, const BusMixVolumePacket* const pBusMixVolumePacket, const OutputBusMixVolume* const pBusMixVolume,
                               const VolumeThroughModePacket* const pVolumeThroughModePacket ) NN_NOEXCEPT;
    const OutputAdditionalParam* const GetTvAdditionalParamAddr() const NN_NOEXCEPT { return m_pTvAdditionalParam; }

    // ------------------------------------------------------------------------
    // サンプルの調整
    position_t GetCurrentPlayingSample() const NN_NOEXCEPT;

    nn::atk::SampleFormat GetFormat() const NN_NOEXCEPT;

    static size_t FrameToByte( position_t sample, SampleFormat format ) NN_NOEXCEPT;
    static void CalcOffsetAdpcmParam(
        nn::atk::AdpcmContext* context, // in/out
        const nn::atk::AdpcmParam& param,
        position_t offsetSamples,
        const void* dataAddress ) NN_NOEXCEPT;

    void AppendWaveBuffer(
        int channel,
        /*int voiceOut,*/
        WaveBuffer* waveBuffer,
        bool lastFlag
    ) NN_NOEXCEPT;

    void SetAdpcmParam(
        int channelIndex,
        const AdpcmParam& param
    ) NN_NOEXCEPT;

    void SetUpdateType( UpdateType updateType ) NN_NOEXCEPT
    {
        m_UpdateType = updateType;
    }
    UpdateType GetUpdateType() const NN_NOEXCEPT
    {
        return m_UpdateType;
    }
    void SetOutputReceiver( OutputReceiver* pOutputReceiver ) NN_NOEXCEPT;

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        nn::os::Tick totalTick(0);
        for ( int channelIndex = 0; channelIndex < m_ChannelCount; channelIndex++ )
        {
            // 全ボイスの DSP 負荷の総和を取得
            totalTick += m_Voice[channelIndex].GetProcessTick( profile );
        }
        return totalTick;
    }

private:
    struct PreMixVolume
    {
        float volume[OutputMix::ChannelCountMax];
    };

    void InitParam(
        MultiVoice::VoiceCallback callback,
        void* callbackData
    ) NN_NOEXCEPT;

    // Update関数
    void CalcSrc( bool initialUpdate ) NN_NOEXCEPT;
    void CalcVe() NN_NOEXCEPT;
    void CalcMix() NN_NOEXCEPT;
    void CalcLpf() NN_NOEXCEPT;
    void CalcBiquadFilter() NN_NOEXCEPT;

    void CalcPreMixVolume(
            PreMixVolume* mix,
            const OutputParam& param,
            const OutputAdditionalParam* const pAdditionalParam,
            int channelIndex,
            OutputDevice device ) NN_NOEXCEPT;
    void CalcTvMix( OutputMix* mix, const PreMixVolume& preMix ) NN_NOEXCEPT;
    void CalcMixImpl(
            OutputMix* mix,
            uint32_t outputDeviceIndex,
            const OutputParam& param,
            const OutputAdditionalParam* const pAdditionalParam,
            const PreMixVolume& pre
            ) NN_NOEXCEPT;

    void RunAllSdkVoice() NN_NOEXCEPT;
    void StopAllSdkVoice() NN_NOEXCEPT;
    void PauseAllSdkVoice() NN_NOEXCEPT;

    void SetOutputParamImpl( const OutputParam& in, OutputParam& out ) NN_NOEXCEPT;
    void SetOutputAdditionalParamImpl( const SendArray* const pAdditionalSend, const BusMixVolumePacket* const pBusMixVolumePacket, const OutputBusMixVolume* const pBusMixVolume,
                                        const VolumeThroughModePacket* const pVolumeThroughModePacket ) NN_NOEXCEPT;
    void SetOutputBusMixVolumeImpl( const BusMixVolumePacket& in, const OutputBusMixVolume& busMixVolume, BusMixVolumePacket& out ) NN_NOEXCEPT;
    void SetOutputVolumeThroughModePacketImpl( const VolumeThroughModePacket& in, VolumeThroughModePacket& out) NN_NOEXCEPT;

private:
    Voice    m_Voice[ WaveChannelMax ];
    VoiceMode m_VoiceMode;

    int                 m_ChannelCount;

    VoiceCallback       m_Callback;
    void*               m_pCallbackData;

    bool                m_IsActive;
    bool                m_IsStart;
    bool                m_IsStarted;
    bool                m_IsPause;
    bool                m_IsPausing;
    bool                m_IsInitialized;

    WaveBuffer* m_pLastWaveBuffer;

    uint16_t                 m_SyncFlag;

    uint8_t                  m_BiquadType;

    bool                m_IsEnableFrontBypass;

    float                 m_Volume;
    float                 m_Pitch;
    PanMode             m_PanMode;
    PanCurve            m_PanCurve;
    float                 m_LpfFreq;
    float                 m_BiquadValue;
    int                 m_Priority;

    uint32_t                 m_OutputLineFlag;

    // L/R: -1.0f(left) - 0.0f(center) - 1.0f(right)
    // F/R: 0.0f(front) - 2.0f(rear)
    OutputParam m_TvParam;
    OutputAdditionalParam* m_pTvAdditionalParam;

    nn::atk::SampleFormat m_Format;

    uintptr_t m_VoiceUser;    // Channel or StreamTrack

    UpdateType m_UpdateType;

    OutputReceiver* m_pOutputReceiver;

public:
    util::IntrusiveListNode    m_LinkNode;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
