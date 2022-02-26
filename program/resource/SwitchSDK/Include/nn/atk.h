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


//---------------------------------------------------------------------------
//! @namespace nn::atk
//!
//! @brief    Audio Toolkit の名前空間です。
//!
//! @attention 本名前空間の API を利用するには、
//!            NintendoSDK NintendoWare パッケージをインストールする必要があります。
//---------------------------------------------------------------------------


#include <nn/atk/atk_Config.h>

#include <nn/atk/atk_AnimEventPlayer.h>
#include <nn/atk/atk_AnimSound.h>
#include <nn/atk/atk_AnimSoundFile.h>
#include <nn/atk/atk_AnimSoundFileReader.h>
#include <nn/atk/atk_AnimSoundImpl.h>
#include <nn/atk/atk_AudioRendererPerformanceReader.h>
#include <nn/atk/atk_BankFile.h>
#include <nn/atk/atk_BankFileReader.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_BiquadFilterCallback.h>
#include <nn/atk/atk_BiquadFilterPresets.h>
#include <nn/atk/atk_CurveAdshr.h>
#include <nn/atk/atk_CurveLfo.h>
#include <nn/atk/atk_Debug.h>
#include <nn/atk/atk_DriverCommand.h>
#include <nn/atk/atk_DspadpcmReader.h>
#include <nn/atk/atk_Command.h>
#include <nn/atk/atk_CommandManager.h>
#include <nn/atk/atk_Voice.h>
#include <nn/atk/atk_VoiceCommand.h>
#include <nn/atk/atk_ElementType.h>
#include <nn/atk/atk_ExternalSoundPlayer.h>
#include <nn/atk/atk_FinalMix.h>
#include <nn/atk/atk_FinalMixCallback.h>
#include <nn/atk/atk_FrameHeap.h>
#include <nn/atk/atk_FsSoundArchive.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_GroupFile.h>
#include <nn/atk/atk_GroupFileReader.h>
#include <nn/atk/atk_InstancePool.h>
#include <nn/atk/atk_ItemType.h>
#include <nn/atk/atk_MemorySoundArchive.h>
#include <nn/atk/atk_MidiStreamParser.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/atk_PlayerHeap.h>
#include <nn/atk/atk_PlayerHeapDataManager.h>
#include <nn/atk/atk_ProfileReader.h>
#include <nn/atk/atk_Result.h>
#include <nn/atk/atk_SequenceSound.h>
#include <nn/atk/atk_SequenceSoundFile.h>
#include <nn/atk/atk_SequenceSoundFileReader.h>
#include <nn/atk/atk_SequenceSoundHandle.h>
#include <nn/atk/atk_SequenceSoundRuntime.h>
#include <nn/atk/atk_Sound3DActor.h>
#include <nn/atk/atk_Sound3DCalculator.h>
#include <nn/atk/atk_Sound3DEngine.h>
#include <nn/atk/atk_Sound3DListener.h>
#include <nn/atk/atk_Sound3DManager.h>
#include <nn/atk/atk_SoundActor.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundArchiveFile.h>
#include <nn/atk/atk_SoundArchiveFileReader.h>
#include <nn/atk/atk_SoundArchiveLoader.h>
#include <nn/atk/atk_SoundArchivePlayer.h>
#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/atk_SoundHandle.h>
#include <nn/atk/atk_SoundHeap.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_SoundMemoryAllocatable.h>
#include <nn/atk/atk_SoundPlayer.h>
#include <nn/atk/atk_SoundStartable.h>
#include <nn/atk/atk_SoundSystem.h>
#include <nn/atk/atk_StreamSound.h>
#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/atk_StreamSoundFileLoader.h>
#include <nn/atk/atk_StreamSoundFileReader.h>
#include <nn/atk/atk_StreamSoundPrefetchFile.h>
#include <nn/atk/atk_StreamSoundPrefetchFileReader.h>
#include <nn/atk/atk_StreamSoundHandle.h>
#include <nn/atk/atk_StreamSoundRuntime.h>
#include <nn/atk/atk_SubMix.h>
#include <nn/atk/atk_Task.h>
#include <nn/atk/atk_TaskManager.h>
#include <nn/atk/atk_TaskProfileReader.h>
#include <nn/atk/atk_TaskThread.h>
#include <nn/atk/atk_ThreadInfoReader.h>
#include <nn/atk/atk_ThreadStack.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_WaveArchiveFile.h>
#include <nn/atk/atk_WaveArchiveFileReader.h>
#include <nn/atk/atk_WaveFile.h>
#include <nn/atk/atk_WaveFileReader.h>
#include <nn/atk/atk_WaveSound.h>
#include <nn/atk/atk_WaveSoundFile.h>
#include <nn/atk/atk_WaveSoundFileReader.h>
#include <nn/atk/atk_WaveSoundHandle.h>
#include <nn/atk/atk_WaveSoundRuntime.h>
#include <nn/atk/detail/atk_HardwareOpusDecoder.h>
#include <nn/atk/detail/atk_OpusDecoder.h>


// nn::atk::internal::driver

#include <nn/atk/atk_Bank.h>
#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_ChannelManager.h>
#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_DisposeCallbackManager.h>
#include <nn/atk/atk_MidiSequencePlayer.h>
#include <nn/atk/atk_MidiSequenceTrack.h>
#include <nn/atk/atk_MidiSequenceTrackAllocator.h>
#include <nn/atk/atk_MmlCommand.h>
#include <nn/atk/atk_MmlParser.h>
#include <nn/atk/atk_MmlSequenceTrack.h>
#include <nn/atk/atk_MmlSequenceTrackAllocator.h>
#include <nn/atk/atk_NoteOnCallback.h>
#include <nn/atk/atk_SequenceSoundPlayer.h>
#include <nn/atk/atk_SequenceTrack.h>
#include <nn/atk/atk_SequenceTrackAllocator.h>
#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_StreamBufferPool.h>
#include <nn/atk/atk_StreamSoundPlayer.h>
#include <nn/atk/atk_StreamTrack.h>
#include <nn/atk/atk_MultiVoice.h>
#include <nn/atk/atk_MultiVoiceManager.h>
#include <nn/atk/atk_WavePlayer.h>
#include <nn/atk/atk_WaveSoundPlayer.h>
#include <nn/atk/atk_HardwareManager.h>


// エフェクト関連
#include <nn/atk/atk_EffectBase.h>
#include <nn/atk/atk_EffectDelay.h>
#include <nn/atk/atk_EffectReverb.h>
#include <nn/atk/atk_EffectI3dl2Reverb.h>
#include <nn/atk/atk_EffectAux.h>
