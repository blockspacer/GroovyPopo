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

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Command.h>
#include <nn/atk/atk_CommandManager.h>

/*! @file
    @brief      Voice に関する関数、およびクラス定義
*/

namespace nn  { namespace atk {

class OutputReceiver;

namespace detail
{

class LowLevelVoice;

enum VoiceCommandId
{
    VoiceCommandId_AllocVoice,
    VoiceCommandId_FreeVoice,
    VoiceCommandId_SetPriority,
    VoiceCommandId_Play,
    VoiceCommandId_Pause,
    VoiceCommandId_Stop,
    VoiceCommandId_AppendWaveBuffer,
    VoiceCommandId_UpdateParam
};

struct VoiceCommandAlloc : public Command
{
    uint32_t voiceId;
    uint32_t priority;
    void* userId;
};

struct VoiceCommandFree : public Command
{
    uint32_t voiceId;
};

struct VoiceCommandParam : public Command
{
    uint32_t voiceId;
    VoiceParam voiceParam;
};

struct VoiceCommandPriority : public Command
{
    uint32_t voiceId;
    uint32_t priority;
};

struct VoiceCommandPlay : public Command
{
    uint32_t            voiceId;
    SampleFormat        sampleFormat;
    uint32_t            sampleRate;
    AdpcmParam          adpcmParam;
    OutputReceiver*     pOutputReceiver;
};

struct VoiceCommandPause : public Command
{
    uint32_t voiceId;
};

struct VoiceCommandAppendWaveBuffer : public Command
{
    uint32_t voiceId;
    void* tag;
    const void* bufferAddress;
    size_t bufferSize;
    size_t sampleLength;
    detail::position_t sampleOffset;
    bool adpcmContextEnable;
    AdpcmContext adpcmContext;
    bool loopFlag;
};

enum VoiceReplyCommandId
{
    VoiceReplyCommandId_WaveBufferUpdate,
};

struct VoiceReplyCommandWaveBuffer : public Command
{
    const void* tag;
    WaveBuffer::Status status;
//    uint32_t playPosition;
};

class LowLevelVoiceCommand : public CommandManager
{
public:
    static NN_NOINLINE LowLevelVoiceCommand& GetInstance() NN_NOEXCEPT;

    void Initialize( void* buffer, size_t bufferSize, size_t commandBufferSize, int queueCount, void* waveBuffer, size_t waveBufferSize, int waveBufferPacketCount ) NN_NOEXCEPT;

    static size_t GetRequiredWaveBufferMemSize(int waveBufferPacketCount) NN_NOEXCEPT;

private:
    struct WaveBufferPacket
    {
        AdpcmContext adpcmContext;
        WaveBuffer waveBuffer;
    };

    LowLevelVoiceCommand() NN_NOEXCEPT {}
    WaveBufferPacket* GetFreeWaveBuffer() NN_NOEXCEPT;

    static void ProcessCommandList( Command* commandList ) NN_NOEXCEPT;
    static void LowLevelVoiceDisposeCallback( LowLevelVoice* voice, void* arg ) NN_NOEXCEPT;

    WaveBufferPacket* m_pWaveBufferPacket;
    int m_WaveBufferPacketCount;
};

class VoiceReplyCommand : public CommandManager
{
public:
    static NN_NOINLINE VoiceReplyCommand& GetInstance() NN_NOEXCEPT;

    size_t GetRequiredMemSize( size_t commandBufferSize, int queueCount )
    {
        return CommandManager::GetRequiredMemSize(commandBufferSize, queueCount);
    }

    void Initialize( void* buffer, size_t bufferSize, size_t commandBufferSize, int queueCount ) NN_NOEXCEPT
    {
        CommandManager::Initialize( buffer, bufferSize, commandBufferSize, queueCount, ProcessCommandList );
    }

private:
    VoiceReplyCommand() NN_NOEXCEPT {}

    static void ProcessCommandList( Command* commandList ) NN_NOEXCEPT;
};

}}} // namespace nn::atk
