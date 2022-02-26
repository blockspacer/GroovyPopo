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

#include <nn/os/os_Tick.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/audio/audio_PerformanceMetrics.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_HardwareManager.h>
#include <nn/atk/atk_ThreadStack.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_ProfileReader.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/atk_ThreadInfoReader.h>

namespace nn {
namespace atk {

class AudioRendererPerformanceReader;

namespace detail {

namespace driver {

class SoundThread : public fnd::Thread::Handler
{
    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    typedef void (*ProfileFunc)( nn::os::Tick* pTick );

    class SoundFrameCallback
    {
      public:
        util::IntrusiveListNode m_Link;

        virtual ~SoundFrameCallback() NN_NOEXCEPT {}
        virtual void OnBeginSoundFrame() NN_NOEXCEPT {}
        virtual void OnEndSoundFrame() NN_NOEXCEPT {}
    };

    class PlayerCallback
    {
      public:
        util::IntrusiveListNode m_Link;

        virtual ~PlayerCallback() NN_NOEXCEPT {}
        virtual void OnUpdateFrameSoundThread(int updateFrameCount) NN_NOEXCEPT = 0;
        virtual void OnUpdateFrameSoundThreadWithAudioFrameFrequency(int updateFrameCount) NN_NOEXCEPT = 0;
        virtual void OnShutdownSoundThread() NN_NOEXCEPT {}
    };

    /* ------------------------------------------------------------------------
            constant definition
       ------------------------------------------------------------------------ */
private:
    static const int ThreadMessageBufferSize = 32;
    static const int RendererEventWaitTimeoutMilliSeconds = 100;

    // 上位 4ビットがタイプ
    // 下位28ビットがデータ
    enum Message
    {
        Message_HwCallback  = 0x10000000,
        Message_Shutdown    = 0x20000000,
        Message_ForceWakeup = 0x30000000
    };

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    static NN_NOINLINE SoundThread& GetInstance() NN_NOEXCEPT;

    bool CreateSoundThread(
            int threadPriority,
            void* stackBase,
            size_t stackSize,
            int idealCoreNumber,
            uint32_t affinityMask ) NN_NOEXCEPT;
    void Destroy() NN_NOEXCEPT;

    void Initialize( void* performanceFrameBuffer, size_t performanceFrameBufferSize, bool isProfilingEnabled, bool isDetailSoundThreadProfileEnabled, bool IsUserThreadRenderingEnabled ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void Pause( bool pauseFlag ) NN_NOEXCEPT { m_PauseFlag = pauseFlag; }

    uint32_t GetAxCallbackCounter() const NN_NOEXCEPT { return m_AxCallbackCounter; }

    // SoundMaker の波形出力処理で使用するため、public である必要があります。
#ifndef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
    void UpdateElapsedFrameCount() NN_NOEXCEPT;
#endif
    void UpdateLowLevelVoices() NN_NOEXCEPT;
    void FrameProcess( UpdateType updateType = UpdateType_AudioFrame ) NN_NOEXCEPT;
#ifndef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
    void UpdateLowLevelVoicesParam() NN_NOEXCEPT;
#endif
    void EffectFrameProcess() NN_NOEXCEPT;

    void RegisterSoundFrameUserCallback( SoundFrameUserCallback callback, uintptr_t arg ) NN_NOEXCEPT;
    void ClearSoundFrameUserCallback() NN_NOEXCEPT;

    void RegisterThreadBeginUserCallback(SoundThreadUserCallback callback, uintptr_t arg) NN_NOEXCEPT;
    void ClearThreadBeginUserCallback() NN_NOEXCEPT;
    void RegisterThreadEndUserCallback(SoundThreadUserCallback callback, uintptr_t arg) NN_NOEXCEPT;
    void ClearThreadEndUserCallback() NN_NOEXCEPT;

    void RegisterSoundThreadInfoRecorder(SoundThreadInfoRecorder& recorder) NN_NOEXCEPT;
    void UnregisterSoundThreadInfoRecorder(SoundThreadInfoRecorder& recorder) NN_NOEXCEPT;

    void RegisterSoundFrameCallback( SoundFrameCallback* callback ) NN_NOEXCEPT;
    void UnregisterSoundFrameCallback( SoundFrameCallback* callback ) NN_NOEXCEPT;

    void RegisterPlayerCallback( PlayerCallback* callback ) NN_NOEXCEPT;
    void UnregisterPlayerCallback( PlayerCallback* callback ) NN_NOEXCEPT;

    void Lock() NN_NOEXCEPT { m_CriticalSection.Lock(); }
    void Unlock() NN_NOEXCEPT { m_CriticalSection.Unlock(); }

    void LockAtkStateAndParameterUpdate() NN_NOEXCEPT;
    void UnlockAtkStateAndParameterUpdate() NN_NOEXCEPT;

    // プロファイル
#if 0 // 参考用 : NW4F 実装
    int GetDspCycles() const NN_NOEXCEPT
    {
        return m_DspCycles;
    }
    void ClearDspCycles() NN_NOEXCEPT
    {
        m_DspCycles = 0;
    }
#endif

    void RegisterProfileReader( ProfileReader& profileReader ) NN_NOEXCEPT
    {
        m_ProfileReaderList.push_back(profileReader);
    }

    void UnregisterProfileReader( ProfileReader& profileReader ) NN_NOEXCEPT
    {
        m_ProfileReaderList.erase(m_ProfileReaderList.iterator_to(profileReader));
    }

    void RegisterAudioRendererPerformanceReader( AudioRendererPerformanceReader& performanceReader ) NN_NOEXCEPT;

    void UnregisterAudioRendererPerformanceReader() NN_NOEXCEPT
    {
        m_pAudioRendererPerformanceReader = nullptr;
    }

    void RegisterSoundThreadUpdateProfileReader( SoundThreadUpdateProfileReader& profileReader ) NN_NOEXCEPT;
    void UnregisterSoundThreadUpdateProfileReader( SoundThreadUpdateProfileReader& profileReader ) NN_NOEXCEPT;

    void ForceWakeup() NN_NOEXCEPT;

#if defined(NN_ATK_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    int GetThreadCoreNumber() const NN_NOEXCEPT
    {
        return m_Thread.GetCoreNumber();
    }
#endif

    int GetRendererEventWaitTimeMilliSeconds()
    {
        return m_RendererEventWaitTimeMilliSeconds;
    }

private:
    typedef
        util::IntrusiveList<SoundFrameCallback, util::IntrusiveListMemberNodeTraits<SoundFrameCallback,&SoundFrameCallback::m_Link>>
        SoundFrameCallbackList;
    typedef
        util::IntrusiveList<PlayerCallback, util::IntrusiveListMemberNodeTraits<PlayerCallback,&PlayerCallback::m_Link>>
        PlayerCallbackList;

private:
    SoundThread() NN_NOEXCEPT;
    virtual ~SoundThread() NN_NOEXCEPT NN_OVERRIDE {}

    virtual uint32_t Run(void* param) NN_NOEXCEPT NN_OVERRIDE;

    void RecordPerformanceInfo( nn::audio::PerformanceInfo& src, nn::os::Tick beginTick,  nn::os::Tick endTick, uint32_t nwVoiceCount ) NN_NOEXCEPT;
    void RecordUpdateProfile( const SoundThreadUpdateProfile& updateProfile ) NN_NOEXCEPT;

private:
    nn::atk::detail::fnd::Thread m_Thread;
    nn::os::MessageQueue         m_BlockingQueue;
    uintptr_t                    m_MsgBuffer[ ThreadMessageBufferSize ];

#if 0 // 参考用 : NW4F 実装
    int                     m_DspCycles;            // 累積 DSP サイクル
#endif

    uint32_t m_AxCallbackCounter;

    mutable detail::fnd::CriticalSection m_CriticalSection;
    mutable detail::fnd::CriticalSection m_UpdateAtkStateAndParameterSection;

    SoundFrameCallbackList  m_SoundFrameCallbackList;
    PlayerCallbackList      m_PlayerCallbackList;

    // 最適化で m_UserCallbackArgと設定順が逆にならないように volatile をつける
    volatile SoundFrameUserCallback  m_UserCallback;
    volatile uintptr_t               m_UserCallbackArg;

    // 最適化で UserCallbackArg と設定順が逆にならないように volatile をつける
    volatile SoundThreadUserCallback m_ThreadBeginUserCallback;
    volatile uintptr_t               m_ThreadBeginUserCallbackArg;
    volatile SoundThreadUserCallback m_ThreadEndUserCallback;
    volatile uintptr_t               m_ThreadEndUserCallbackArg;

    int32_t                 m_SoundThreadAffinityMask;
    bool                    m_CreateFlag;
    bool                    m_PauseFlag;

    nn::os::Tick m_LastPerformanceFrameBegin;
    nn::os::Tick m_LastPerformanceFrameEnd;
    void* m_pPerformanceFrameUpdateBuffer[detail::MaxPerformanceBufferCount];
    size_t m_PerformanceFrameUpdateBufferSize;
    int m_CurrentPerformanceFrameBufferIndex;
    bool m_IsProfilingEnabled;
    ProfileFunc m_pSoundThreadProfileFunc;
    bool m_IsUserThreadRenderingEnabled;
    ProfileReaderList m_ProfileReaderList;
    AudioRendererPerformanceReader* m_pAudioRendererPerformanceReader;
    SoundThreadInfoRecorderList m_InfoRecorderList;
    nn::atk::detail::fnd::CriticalSection m_LockRecordInfo;

    SoundThreadUpdateProfile m_LastUpdateProfile;
    SoundThreadUpdateProfileReaderList m_UpdateProfileReaderList;
    nn::atk::detail::fnd::CriticalSection m_LockUpdateProfile;

    std::atomic<int> m_RendererEventWaitTimeMilliSeconds;
#ifndef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
    int64_t m_PrevAudioFrameCount;
    int64_t m_CurrentAudioFrameCount;
#endif
};

class SoundThreadLock
{
public:
    SoundThreadLock() NN_NOEXCEPT
    {
        SoundThread::GetInstance().Lock();
    }

    ~SoundThreadLock() NN_NOEXCEPT
    {
        SoundThread::GetInstance().Unlock();
    }

private:
    NN_DISALLOW_COPY( SoundThreadLock );
};

class AtkStateAndParameterUpdateLock
{
public:
    AtkStateAndParameterUpdateLock() NN_NOEXCEPT
    {
        SoundThread::GetInstance().LockAtkStateAndParameterUpdate();
    }

    ~AtkStateAndParameterUpdateLock() NN_NOEXCEPT
    {
        SoundThread::GetInstance().UnlockAtkStateAndParameterUpdate();
    }

private:
    NN_DISALLOW_COPY( AtkStateAndParameterUpdateLock );
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

