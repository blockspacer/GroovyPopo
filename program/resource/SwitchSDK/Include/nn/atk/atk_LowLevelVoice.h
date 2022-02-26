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

//#define ENABLE_NN_ATK_LOWLEVELVOICE_APPEND_PER_BUFFERJUMP

#include <nn/audio.h>

#include <nn/atk/atk_Global.h>
#include <atomic>

namespace nn {
namespace atk {

    class OutputReceiver;

namespace detail {

    class Voice;

    class LowLevelVoice
    {
    public:
        LowLevelVoice() NN_NOEXCEPT;

        void Initialize() NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        bool IsAvailable() const NN_NOEXCEPT;
        void SetAvailable(bool isAvailable) NN_NOEXCEPT;
        bool IsVoiceDroppedFlagOn() const NN_NOEXCEPT;

        void AppendWaveBuffer( WaveBuffer* waveBuffer ) NN_NOEXCEPT;
        void FreeAllWaveBuffer() NN_NOEXCEPT;

        void UpdateState(OutputMode outputMode) NN_NOEXCEPT;

        void SetVoiceParam(const VoiceParam& voiceParam ) NN_NOEXCEPT
        {
            m_VoiceParam = voiceParam;
#ifdef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
            m_IsVoiceParamDirty = true;
#endif
        }

        void SetPriority( int32_t priority ) NN_NOEXCEPT;

        void SetState( VoiceState state ) NN_NOEXCEPT;

        void SetSampleRate( int32_t sampleRate ) NN_NOEXCEPT
        {
            m_SampleRate = sampleRate;
        }

        void SetSampleFormat( SampleFormat sampleFormat ) NN_NOEXCEPT
        {
            m_SampleFormat = sampleFormat;
        }

        void SetAdpcmParam( const AdpcmParam& adpcmParam ) NN_NOEXCEPT
        {
            m_AdpcmParam = adpcmParam;
        }

        position_t GetPlayPosition() const NN_NOEXCEPT
        {
            return m_PlayPosition;
        }

        void SetVoice( Voice* pVoice ) NN_NOEXCEPT
        {
            m_pVoice = pVoice;
        }

        void SetOutputReceiver( OutputReceiver* pOutputReceiver ) NN_NOEXCEPT
        {
            m_pOutputReceiver = pOutputReceiver;
        }

        void UpdateVoiceInfo( VoiceInfo* pVoiceInfo ) const NN_NOEXCEPT;

        nn::audio::NodeId GetNodeId() NN_NOEXCEPT
        {
            return m_NodeId;
        }

#ifndef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
        void UpdateVoiceParam(OutputMode outputMode) NN_NOEXCEPT;
        void ApplyVoiceStatus(OutputMode outputMode) NN_NOEXCEPT;
#endif

    private:
        void UpdateVoiceParamImpl(const VoiceParam& voiceParam, OutputMode outputMode) NN_NOEXCEPT;

        bool AllocVoice() NN_NOEXCEPT;
        void UpdateStatePlay( bool isRun, OutputMode outputMode ) NN_NOEXCEPT;
        void UpdateStateStop( bool isRun ) NN_NOEXCEPT;
        void UpdateStatePause( bool isRun, OutputMode outputMode ) NN_NOEXCEPT;
        void UpdatePlayPosition() NN_NOEXCEPT;

        void UpdateWaveBuffer( bool isRun, OutputMode outputMode ) NN_NOEXCEPT;
        void UpdateWaveBufferOnPlayState() NN_NOEXCEPT;
        void UpdateWaveBufferOnStopState( OutputMode outputMode ) NN_NOEXCEPT;
        bool AppendWaveBufferToVoice(WaveBuffer* waveBuffer) NN_NOEXCEPT;

        void UpdateMixVolume(const OutputMix& outputMix, OutputMode outputMode) NN_NOEXCEPT;
        void UpdateVolume(const VoiceParam& voiceParam) NN_NOEXCEPT;
        void UpdatePitch(const VoiceParam& voiceParam) NN_NOEXCEPT;
        void UpdateBiquadFilter(const VoiceParam& voiceParam) NN_NOEXCEPT;
        void UpdateLowPassFilter(const VoiceParam& voiceParam) NN_NOEXCEPT;

        void SetVoiceMixVolume(float mixVolume, int destinationIndex) NN_NOEXCEPT;
        float GetClampedVoiceVolume(float volume) NN_NOEXCEPT;

    private:
        NN_AUDIO_ALIGNAS_BUFFER_ALIGN AdpcmParam m_AdpcmParam;
        nn::audio::VoiceType m_Voice;
        bool            m_IsAvailable;
        bool            m_IsSetVoiceSlot;
#if defined(ENABLE_NN_ATK_LOWLEVELVOICE_APPEND_PER_BUFFERJUMP)
        bool            m_IsWaitingForBufferJump;
#endif

        VoiceParam      m_VoiceParam;
        int32_t         m_Priority;
        VoiceState      m_State;
        uint32_t        m_SampleRate;
        SampleFormat    m_SampleFormat;
        position_t      m_PlayPosition;
        OutputReceiver* m_pOutputReceiver;

        WaveBuffer*         m_WaveBufferListBegin;
        WaveBuffer*         m_WaveBufferListEnd;
        WaveBuffer*         m_LastAppendBuffer;
        Voice*              m_pVoice;
        nn::audio::NodeId   m_NodeId;

#ifdef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
        bool                m_IsVoiceParamDirty;
#endif
    } NN_AUDIO_ALIGNAS_BUFFER_ALIGN;

    class LowLevelVoiceAllocator
    {
    public:
        LowLevelVoiceAllocator() NN_NOEXCEPT;

        static size_t GetRequiredMemSize(int voiceCount) NN_NOEXCEPT;
        void Initialize(int voiceCount, void* mem, size_t memSize) NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        void UpdateAllVoiceState(OutputMode outputMode) NN_NOEXCEPT;
#ifndef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
        void UpdateAllVoiceParam(OutputMode outputMode) NN_NOEXCEPT;
#endif

        LowLevelVoice* AllocVoice() NN_NOEXCEPT;
        void FreeVoice(LowLevelVoice* pVoice) NN_NOEXCEPT;
        int GetDroppedVoiceCount() const NN_NOEXCEPT;

    private:
        static const int Unassigned = -1;

        ptrdiff_t GetVoiceArrayIndex(LowLevelVoice* pVoice) NN_NOEXCEPT;

        //  ボイスの実態の配列です。
        void* m_pVoiceArray;

        //  m_pVoiceTable[i] について
        //  0 <= i < m_UsingCount : 使用中のボイスへのポインタ
        //  m_UsingCount <= i < MaxVoiceCount : 使用していないボイスへのポインタ
        LowLevelVoice** m_ppVoiceTable;
        int m_UsingCount;

        //  m_VoiceArray[i] が m_pVoiceTable の何番目に割り当てられたかを表します。
        //  未割り当てのときは Unassigned になります。
        int* m_pAssignedTableIndex;

        int m_VoiceCount;
        std::atomic<int> m_DroppedVoiceCount;
    };

}
}
} // namespace nn::atk::detail
