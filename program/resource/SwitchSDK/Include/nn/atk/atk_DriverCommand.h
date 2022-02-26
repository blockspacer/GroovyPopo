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

#include <nn/atk/atk_StreamSoundPlayer.h>
#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/atk_StreamSoundFileReader.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_SequenceSound.h>
#include <nn/atk/atk_WaveSound.h>
#include <nn/atk/atk_StreamSound.h>
#include <nn/atk/atk_Command.h>
#include <nn/atk/atk_CommandManager.h>
#include <nn/atk/atk_OutputReceiver.h>

#include <nn/atk/detail/atk_AdvancedWaveSoundPlayer.h>

namespace nn {
namespace atk {

class EffectBase;
struct WaveBuffer;

namespace detail {

namespace driver {
class BasicSoundPlayer;
class SequenceSoundPlayer;
class SequenceTrackAllocator;
class NoteOnCallback;
class WaveSoundPlayer;
class StreamSoundPlayer;
class StreamBufferPool;
class DisposeCallback;
class MultiVoice;
}

enum DriverCommandId
{
    DriverCommandId_Dummy,
    DriverCommandId_Debug,
    DriverCommandId_Reply,
    DriverCommandId_PlayerInit,
    DriverCommandId_PlayerPanmode,
    DriverCommandId_PlayerPancurve,
    DriverCommandId_PlayerFinalize,
    DriverCommandId_PlayerStart,
    DriverCommandId_PlayerStop,
    DriverCommandId_PlayerPause,
    DriverCommandId_PlayerParam,
    DriverCommandId_PlayerAdditionalSend,
    DriverCommandId_PlayerBusMixVolumeUsed,
    DriverCommandId_PlayerBusMixVolume,
    DriverCommandId_PlayerBusMixVolumeEnabled,
    DriverCommandId_PlayerBinaryVolume,
    DriverCommandId_PlayerVolumeThroughModeUsed,
    DriverCommandId_PlayerVolumeThroughMode,
    DriverCommandId_PlayerClearResourceFlag,
    DriverCommandId_SeqSetup,
    DriverCommandId_SeqLoad,
    DriverCommandId_SeqPrepare,
    DriverCommandId_SeqSkip,
    DriverCommandId_SeqTempoRatio,
    DriverCommandId_SeqChannelPrio,
    DriverCommandId_SeqSetVar,
    DriverCommandId_SeqSetGvar,
    DriverCommandId_SeqSetTvar,
    DriverCommandId_SeqTrackMute,
    DriverCommandId_SeqTrackSilence,
    DriverCommandId_SeqTrackVolume,
    DriverCommandId_SeqTrackPitch,
    DriverCommandId_SeqTrackLpf,
    DriverCommandId_SeqTrackBiquad,
    DriverCommandId_SeqTrackBankIndex,
    DriverCommandId_SeqTrackTranspose,
    DriverCommandId_SeqTrackVelocityRange,
    DriverCommandId_SeqTrackOutputline,
    DriverCommandId_SeqTrackOutputlineReset,
    DriverCommandId_SeqTrackTvVolume,
    DriverCommandId_SeqTrackTvMixParameter,
    DriverCommandId_SeqTrackTvPan,
    DriverCommandId_SeqTrackTvSpan,
    DriverCommandId_SeqTrackTvMainSend,
    DriverCommandId_SeqTrackTvFxSend,
    DriverCommandId_WsdPrepare,
    DriverCommandId_WsdLoad,
    DriverCommandId_WsdChannelPrio,
    DriverCommandId_WsdChannelParam,
    DriverCommandId_AwsdPrepare,
    DriverCommandId_StrmSetup,
    DriverCommandId_StrmPrepare,
    DriverCommandId_StrmPreparePrefetch,
    DriverCommandId_StrmLoadHeader,
    DriverCommandId_StrmLoadData,
    DriverCommandId_StrmForceFinish,
    DriverCommandId_StrmTrackVolume,
    DriverCommandId_StrmTrackInitialVolume,
    DriverCommandId_StrmTrackOutputline,
    DriverCommandId_StrmTrackOutputlineReset,
    DriverCommandId_StrmTrackTvVolume,
    DriverCommandId_StrmTrackTvMixParameter,
    DriverCommandId_StrmTrackTvPan,
    DriverCommandId_StrmTrackTvSpan,
    DriverCommandId_StrmTrackTvMainSend,
    DriverCommandId_StrmTrackTvFxSend,
    DriverCommandId_InvalidateData,
    DriverCommandId_RegistDisposeCallback,
    DriverCommandId_UnregistDisposeCallback,
    DriverCommandId_AppendEffect,
    DriverCommandId_AppendEffectAux,
    DriverCommandId_RemoveEffect,
    DriverCommandId_RemoveEffectAux,
    DriverCommandId_ClearEffect,
    DriverCommandId_SubMixApplyDestination,
    DriverCommandId_SubMixUpdateMixVolume,
    DriverCommandId_AuxBusVolume,
    DriverCommandId_AllVoicesSync,
    DriverCommandId_VoicePlay,
    DriverCommandId_VoiceWaveInfo,
    DriverCommandId_VoiceAdpcmParam,
    DriverCommandId_VoiceAppendWaveBuffer,
    DriverCommandId_VoicePriority,
    DriverCommandId_VoiceVolume,
    DriverCommandId_VoicePitch,
    DriverCommandId_VoicePanMode,
    DriverCommandId_VoicePanCurve,
    DriverCommandId_VoicePan,
    DriverCommandId_VoiceSpan,
    DriverCommandId_VoiceLpf,
    DriverCommandId_VoiceBiquad,
    DriverCommandId_VoiceOutputLine,
    DriverCommandId_VoiceMainOutVolume,
    DriverCommandId_VoiceMainSend,
    DriverCommandId_VoiceFxSend
};

struct DriverCommandDebug : public Command
{
    char str[256];
};

struct DriverCommandReply : public Command
{
    bool* ptr;
};

struct DriverCommandPlayer : public Command
{
    driver::BasicSoundPlayer* player;
    bool flag;
    uint8_t padding[3];
};

struct DriverCommandPlayerInit : public Command
{
    driver::BasicSoundPlayer* player;
    OutputReceiver* pOutputReceiver;
    bool* availableFlagPtr;
};

struct DriverCommandPlayerPanParam : public Command
{
    driver::BasicSoundPlayer* player;
    uint8_t panMode;         // PanMode
    uint8_t panCurve;        // PanCurve
    uint8_t padding[2];
};

struct DriverCommandPlayerParam : public Command
{
    driver::BasicSoundPlayer* player;
    float volume;
    float pitch;
    float lpfFreq;
    int biquadFilterType;
    float biquadFilterValue;
    uint32_t outputLineFlag;

    OutputParam tvParam;
};

struct DriverCommandPlayerBusMixVolumeUsed : public Command
{
    driver::BasicSoundPlayer* player;
    bool isUsed;
};

struct DriverCommandPlayerBusMixVolume : public Command
{
    driver::BasicSoundPlayer* player;
    OutputBusMixVolume tvBusMixVolume;
};

struct DriverCommandPlayerBusMixVolumeEnabled : public Command
{
    driver::BasicSoundPlayer* player;
    int bus;
    bool isEnabled;
};

struct DriverCommandPlayerAdditionalSend : public Command
{
    driver::BasicSoundPlayer* player;
    int bus;
    float send;
};

struct DriverCommandPlayerBinaryVolume : public Command
{
    driver::BasicSoundPlayer* player;
    float volume;
};

struct DriverCommandPlayerVolumeThroughModeUsed : public Command
{
    driver::BasicSoundPlayer* player;
    bool isVolumeThroughModeUsed;
};

struct DriverCommandPlayerVolumeThroughMode : public Command
{
    driver::BasicSoundPlayer* player;
    int bus;
    uint8_t volumeThroughMode;
};

struct DriverCommandPlayerClearResourceFlag : public Command
{
    driver::BasicSoundPlayer* player;
};

struct DriverCommandSequenceSoundSetup : public Command
{
    driver::SequenceSoundPlayer* player;
    driver::SequenceSoundPlayer::SetupArg arg;
    uint8_t channelPriority;
    bool isReleasePriorityFix;
    uintptr_t userproc;
    void* userprocArg;
};

struct DriverCommandSequenceSoundLoad : public Command
{
    driver::SequenceSoundPlayer* player;
    driver::SequenceSoundPlayer::StartInfo startInfo;
    driver::SequenceSoundLoader::Arg arg;
};

struct DriverCommandSequenceSoundPrepare : public Command
{
    driver::SequenceSoundPlayer* player;
    driver::SequenceSoundPlayer::PrepareArg arg;
};

struct DriverCommandSequenceSoundSkip : public Command
{
    driver::SequenceSoundPlayer* player;
    int offsetType;
    int offset;
};

struct DriverCommandSequenceSoundTempoRatio : public Command
{
    driver::SequenceSoundPlayer* player;
    float tempoRatio;
};

struct DriverCommandSequenceSoundChannelPrio : public Command
{
    driver::SequenceSoundPlayer* player;
    uint8_t priority;
    uint8_t padding[3];
};

struct DriverCommandSequenceSoundSetVar : public Command
{
    driver::SequenceSoundPlayer* player;
    int trackNo;
    int varNo;
    int32_t var;
};

struct DriverCommandSequenceSoundTrack : public Command
{
    driver::SequenceSoundPlayer* player;
    uint32_t trackBitFlag;
};

struct DriverCommandSequenceSoundTrackMute : public DriverCommandSequenceSoundTrack
{
    int mute;
};

struct DriverCommandSequenceSoundTrackSilence : public DriverCommandSequenceSoundTrack
{
    bool silenceFlag;
    int fadeFrames;
};

struct DriverCommandSequenceSoundTrackParam : public DriverCommandSequenceSoundTrack
{
    float value;
    uint32_t uint32Value;
};

struct DriverCommandSequenceSoundTrackBiquad : public DriverCommandSequenceSoundTrack
{
    int type;
    float value;
};

struct DriverCommandSequenceSoundTrackBankIndex : public DriverCommandSequenceSoundTrack
{
    int bankIndex;
};

struct DriverCommandSequenceSoundTrackTranspose : public DriverCommandSequenceSoundTrack
{
    int8_t transpose;
};

struct DriverCommandSequenceSoundTrackVelocityRange : public DriverCommandSequenceSoundTrack
{
    uint8_t range;
};

struct DriverCommandSequenceSoundTrackOutputLine : public DriverCommandSequenceSoundTrack
{
    uint32_t outputLine;
};

struct DriverCommandSequenceSoundTrackMixParameter : public DriverCommandSequenceSoundTrack
{
    uint32_t trackBitFlag;
    uint32_t srcChNo;
    MixParameter param;
    uint32_t drcIndex;
};

struct DriverCommandWaveSoundPrepare : public Command
{
    driver::WaveSoundPlayer* player;
    driver::WaveSoundPlayer::StartInfo startInfo;
    driver::WaveSoundPlayer::PrepareArg arg;
};

struct DriverCommandWaveSoundLoad : public Command
{
    driver::WaveSoundPlayer* player;
    driver::WaveSoundPlayer::StartInfo startInfo;
    driver::WaveSoundLoader::Arg arg;
};

struct DriverCommandWaveSoundChannelPrio : public Command
{
    driver::WaveSoundPlayer* player;
    uint8_t priority;
    uint8_t padding[3];
};

struct DriverCommandWaveSoundChannelParam : public Command
{
    driver::WaveSoundPlayer* player;
    uint8_t priority;
    bool isReleasePriorityFix;
    uint8_t padding[2];
};

struct DriverCommandAdvancedWaveSoundPrepare : public Command
{
    driver::AdvancedWaveSoundPlayer* player;
    driver::AdvancedWaveSoundPlayer::PrepareParameter parameter;
};

struct DriverCommandStreamSoundSetup : public Command
{
    driver::StreamSoundPlayer* player;
    driver::StreamSoundPlayer::SetupArg arg;
};

struct DriverCommandStreamSoundPrepare : public Command
{
    driver::StreamSoundPlayer* player;
    driver::StreamSoundPlayer::PrepareArg arg;
};

struct DriverCommandStreamSoundPreparePrefetch : public Command
{
    driver::StreamSoundPlayer* player;
    driver::StreamSoundPlayer::PreparePrefetchArg arg;
};

struct DriverCommandStreamSoundLoadHeader : public Command
{
    driver::StreamSoundPlayer* player;
    AdpcmParam* adpcmParam[StreamChannelCount];
    bool result;
    uint16_t assignNumber;
};

struct DriverCommandStreamSoundLoadData : public Command
{
    driver::StreamSoundPlayer* player;
    LoadDataParam loadDataParam;
    bool result;
    uint16_t assignNumber;
};

struct DriverCommandStreamSoundForceFinish : public Command
{
    driver::StreamSoundPlayer* player;
};

struct DriverCommandStreamSoundTrackParam : public Command
{
    driver::StreamSoundPlayer* player;
    uint32_t trackBitFlag;
    float value;
    uint32_t uint32Value;
    uint32_t drcIndex;
};

struct DriverCommandStreamSoundTrackInitialVolume : public Command
{
    driver::StreamSoundPlayer* player;
    uint32_t trackBitFlag;
    uint32_t value;
};

struct DriverCommandStreamSoundTrackMixParameter : public Command
{
    driver::StreamSoundPlayer* player;
    uint32_t trackBitFlag;
    uint32_t srcChNo;
    MixParameter param;
    uint32_t drcIndex;
};

struct DriverCommandInvalidateData : public Command
{
    const void* mem;
    size_t size;
};

struct DriverCommandDisposeCallback : public Command
{
    driver::DisposeCallback* callback;
};

struct DriverCommandEffect : public Command
{
    int bus;
    EffectBase* effect;
    void* effectBuffer;
    size_t effectBufferSize;
    OutputMixer* pOutputMixer;
};

struct DriverCommandEffectAux : public Command
{
    int bus;
    EffectAux* effect;
    void* effectBuffer;
    size_t effectBufferSize;
    OutputMixer* pOutputMixer;
};

struct DriverCommandSubMixApplyDestination : public Command
{
    OutputReceiver* pReceiver;
};

struct DriverCommandSubMixUpdateMixVolume : public Command
{
    OutputReceiver* pReceiver;
    int srcBus;
    int srcChannel;
    int dstBus;
    int dstChannel;
};

struct DriverCommandAuxBusVolume : public Command
{
    AuxBus bus;
    int subMixIndex;
    float volume;
    int fadeFrames;
};

struct DriverCommandAllVoicesSync : public Command
{
    uint32_t syncFlag;
};


//
// Voice → MultiVoice の操作
//
struct DriverCommandVoice : public Command
{
    driver::MultiVoice* voice;
};

struct DriverCommandVoicePlay : public DriverCommandVoice
{
    enum State
    {
        State_Start,
        State_Stop,
        State_PauseOn,
        State_PauseOff
    };
    State state;
};

struct DriverCommandVoiceWaveInfo : public DriverCommandVoice
{
    SampleFormat format;
    int sampleRate;
    int interpolationType;
};

struct DriverCommandVoiceAdpcmParam : public DriverCommandVoice
{
    int channel;
    int voiceOut;
    const AdpcmParam* param;
};

struct DriverCommandVoiceAppendWaveBuffer : public DriverCommandVoice
{
    int channel;
    int voiceOut;
    WaveBuffer* buffer;
    bool lastFlag;
};

struct DriverCommandVoicePriority : public DriverCommandVoice
{
    int priority;
};

struct DriverCommandVoiceVolume : public DriverCommandVoice
{
    float volume;
};

struct DriverCommandVoicePitch : public DriverCommandVoice
{
    float pitch;
};

struct DriverCommandVoicePanMode : public DriverCommandVoice
{
    PanMode mode;
};

struct DriverCommandVoicePanCurve : public DriverCommandVoice
{
    PanCurve curve;
};

struct DriverCommandVoicePan : public DriverCommandVoice
{
    float pan;
};

struct DriverCommandVoiceSurroundPan: public DriverCommandVoice
{
    float span;
};

struct DriverCommandVoiceLpfFreq : public DriverCommandVoice
{
    float lpfFreq;
};

struct DriverCommandVoiceBiquadFilter : public DriverCommandVoice
{
    int type;
    float value;
};

struct DriverCommandVoiceOutputLine : public DriverCommandVoice
{
    uint32_t lineFlag;
};

struct DriverCommandVoiceMainOutVolume : public DriverCommandVoice
{
    float volume;
};

struct DriverCommandVoiceMainSend : public DriverCommandVoice
{
    float send;
};

struct DriverCommandVoiceFxSend : public DriverCommandVoice
{
    AuxBus bus;
    float send;
};

class DriverCommand : public CommandManager
{
public:
    static NN_NOINLINE DriverCommand& GetInstance() NN_NOEXCEPT;
    static NN_NOINLINE DriverCommand& GetInstanceForTaskThread() NN_NOEXCEPT;

    size_t GetRequiredMemSize( size_t commandBufferSize, int queueCount ) NN_NOEXCEPT;
    void Initialize( void* buffer, size_t bufferSize, size_t commandBufferSize, int queueCount ) NN_NOEXCEPT;

private:
    DriverCommand() NN_NOEXCEPT;

    static void ProcessCommandList( Command* commandList ) NN_NOEXCEPT;
    static void RequestProcessCommand() NN_NOEXCEPT;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

