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

#include <nn/atk/atk_AnimSoundFile.h>
#include <nn/atk/atk_AnimSoundFileReader.h>

namespace nn {
namespace atk {

class SoundArchive;
class SoundStartable;

namespace detail {

class AnimEventPlayer;

class AnimSoundImpl
{
    NN_DISALLOW_COPY(AnimSoundImpl);

public:
    //
    // 定義
    //
    enum EventType
    {
        EventType_TriggerStart,
        EventType_TriggerStop,
        EventType_RangeStart,
        EventType_RangeStop
    };

    enum PlayDirection
    {
        PlayDirection_Forward,
        PlayDirection_Backward
    };

    typedef void (*EventCallback)(
        EventType type,
        int32_t frame,
        const char* soundLabel,
        uint32_t userParam,
        void* arg );

    //
    // 関数
    //
    AnimSoundImpl(
            SoundStartable& starter,
            AnimEventPlayer* eventPlayers,
            int eventPlayerCount ) NN_NOEXCEPT;
    ~AnimSoundImpl() NN_NOEXCEPT;

    bool Initialize( const void* bfasdFile ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool ConvertSoundId( const SoundArchive& arc ) NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_Reader.IsAvailable(); }
    void SetBaseStep( float baseStep ) NN_NOEXCEPT { m_BaseStep = baseStep; }

    void SetEventCallback( EventCallback callback, void* arg ) NN_NOEXCEPT
    {
        m_EventCallback = callback;
        m_EventCallbackArg = arg;
    }

    void ResetFrame( float frame, int loopCounter ) NN_NOEXCEPT;
    void UpdateFrame( float frame, PlayDirection direction = PlayDirection_Forward ) NN_NOEXCEPT;

    void StopAllSound() NN_NOEXCEPT;

    uint32_t GetFrameSize() const NN_NOEXCEPT { return m_FrameSize; }
    int GetLoopCount() const NN_NOEXCEPT { return IsAvailable() ? m_LoopCounter : 0; }
    float GetCurrentFrame() const NN_NOEXCEPT { return IsAvailable() ? m_CurrentFrame : 0.0f; }

    static void Dump( const void* bfasdFile ) NN_NOEXCEPT;

private:
    void UpdateForward( float frame ) NN_NOEXCEPT;
    void UpdateBackward( float frame ) NN_NOEXCEPT;
    void UpdateOneFrame( int32_t current, PlayDirection direction ) NN_NOEXCEPT;

    void UpdateTrigger(
            const AnimSoundFile::AnimEvent& event,
            int32_t current,
            PlayDirection direction ) NN_NOEXCEPT;
    void UpdateRange(
            const AnimSoundFile::AnimEvent& event,
            int32_t current,
            PlayDirection direction ) NN_NOEXCEPT;

    void UpdateForwardRange( const AnimSoundFile::AnimEvent& event, int32_t current ) NN_NOEXCEPT;
    void UpdateBackwardRange( const AnimSoundFile::AnimEvent& event, int32_t current ) NN_NOEXCEPT;

    void StartEvent( const AnimSoundFile::EventInfo& info, bool isStopWhenFinalize ) NN_NOEXCEPT;
    void HoldEvent( const AnimSoundFile::EventInfo& info, bool isStopWhenFinalize ) NN_NOEXCEPT;
    void StopEvent( const AnimSoundFile::EventInfo& info ) NN_NOEXCEPT;

    bool IsPlayableLoopCount( const AnimSoundFile::FrameInfo& info ) NN_NOEXCEPT;
    void WritePlaySpeedToSequenceVariable(
            int eventPlayerNo, const AnimSoundFile::EventInfo& info ) NN_NOEXCEPT;

    SoundStartable& m_Starter;
    AnimSoundFileReader m_Reader;
    AnimEventPlayer* m_pEventPlayers;

    EventCallback m_EventCallback;
    void* m_EventCallbackArg;

    float m_CurrentFrame;
    int m_EventPlayerCount;
    bool m_IsActive;
    bool m_IsInitFrame;
    bool m_IsReset;
    int32_t m_LoopCounter;
    float m_BaseStep;
    float m_CurrentSpeed;
    int32_t m_FrameSize;    // 何度も参照するのでキャッシュしておく
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

