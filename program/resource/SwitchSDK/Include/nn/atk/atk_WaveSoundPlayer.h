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

#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_CurveLfo.h>
#include <nn/atk/atk_WaveSoundFileReader.h>   // WaveSoundInfo, WaveSoundNoteInfo
#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_WaveSoundLoader.h>

namespace nn { namespace atk {

//---------------------------------------------------------------------------
//! @brief    ウェーブサウンドデータのパラメータセットです。
//!
//!           この情報は @ref WaveSoundHandle::ReadWaveSoundDataInfo か
//!           @ref SoundArchivePlayer::ReadWaveSoundDataInfo から取得できます。
//!
//! @see WaveSoundHandle::ReadWaveSoundDataInfo
//! @see SoundArchivePlayer::ReadWaveSoundDataInfo
//!
//---------------------------------------------------------------------------
struct WaveSoundDataInfo
{
    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータがループするなら true、
    //!           終端で終了するなら false となります。
    //---------------------------------------------------------------------------
    bool loopFlag;

    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータのサンプルレートです。
    //---------------------------------------------------------------------------
    int sampleRate;

    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータがループする時のループ開始位置を、
    //!           ウェーブサウンドの先頭からのサンプル数で表します。
    //---------------------------------------------------------------------------
    int64_t loopStart;

    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータがループする時のループ終了位置を、
    //!           ウェーブサウンドの先頭からのサンプル数で表します。
    //!           ループしない時は、データの終端をサンプル数で表します。
    //!           (再生される最後のサンプルの次のサンプルを指します)
    //---------------------------------------------------------------------------
    int64_t loopEnd;

    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータがループする時のループ開始位置を、
    //!           ウェーブサウンドの先頭からのサンプル数で表します。
    //!
    //!           0.3.0 より loopStart の値が、ハードウェアの制約による
    //!           補正がかかる前の値を返すように変更されました。
    //!           以前の値を取得したい場合にはこちらの値を使用してください。
    //!           ただし、このメンバの使用は非推奨で、将来的に削除予定です。
    //---------------------------------------------------------------------------
    int64_t compatibleLoopStart;

    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータがループする時のループ終了位置を、
    //!           ウェーブサウンドの先頭からのサンプル数で表します。
    //!           ループしない時は、データの終端をサンプル数で表します。
    //!
    //!           0.3.0 より loopEnd の値が、ハードウェアの制約による
    //!           補正がかかる前の値を返すように変更されました。
    //!           以前の値を取得したい場合にはこちらの値を使用してください。
    //!           ただし、このメンバの使用は非推奨で、将来的に削除予定です。
    //---------------------------------------------------------------------------
    int64_t compatibleLoopEnd;

    //---------------------------------------------------------------------------
    //! @brief    ウェーブサウンドデータのチャンネル数です。
    //---------------------------------------------------------------------------
    int channelCount;

    //! @internal
    void Dump() NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("WaveSoundDataInfo::%s loop?(%d) rate(%d) loopStart(%lld) loopEnd(%lld) compatibleLoopStart(%lld) compatibleLoopEnd(%lld) channelCount(%d)\n",
                __FUNCTION__, loopFlag, sampleRate, loopStart, loopEnd, compatibleLoopStart, compatibleLoopEnd, channelCount);
    }
};

}} // namespace nn::atk

namespace nn { namespace atk { namespace detail { namespace driver {

class WaveSoundPlayer : public BasicSoundPlayer, public DisposeCallback, public SoundThread::PlayerCallback
{
public:
    static const int PauseReleaseValue = 127;
    static const int MuteReleaseValue  = 127;
    static const int DefaultPriority   = 64;

    enum StartOffsetType
    {
        StartOffsetType_Sample,
        StartOffsetType_Millisec
    };

    struct LoopInfo
    {
        uint32_t loopFlagBit;
        bool isLoopEnabled;
        uint32_t loopStartFrame;
        uint32_t loopEndFrame;
    };

    struct StartInfo
    {
        int index;
        StartOffsetType startOffsetType;
        int startOffset;
        int delayTime;
        int delayCount;
        int waveSoundParameterFlag;
        int release;
        bool isContextCalculationSkipMode;
        LoopInfo loopInfo;
        UpdateType updateType;
    };

    struct WaveSoundCallbackArg
    {
        const void* wsdFile;    // メモリ上のウェーブサウンドファイル
        int wsdIndex;           // bfwsd 内での当該 WSD インデックス
        int noteIndex;          // 現状ではつねにゼロ
        uint32_t callbackData;       // SoundArchivePlayer::PrepareWaveSoundImpl にて fileId が入る
        const PlayerHeapDataManager* dataMgr;
    };

    class WaveSoundCallback
    {
      public:
        virtual ~WaveSoundCallback() NN_NOEXCEPT {}

        virtual bool GetWaveSoundData(
            WaveSoundInfo* info,
            WaveSoundNoteInfo* noteInfo,
            WaveInfo* waveData,
            const WaveSoundCallbackArg& arg
        ) const NN_NOEXCEPT = 0;
    };

public:
    WaveSoundPlayer() NN_NOEXCEPT;
    virtual ~WaveSoundPlayer() NN_NOEXCEPT NN_OVERRIDE;

    void Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
    void Finalize() NN_NOEXCEPT NN_OVERRIDE;

    bool IsPrepared() const NN_NOEXCEPT
    {
        if (m_ResState >= ResState_Assigned)
        {
            return true;
        }
        return false;
    }

    void SetLoaderManager(WaveSoundLoaderManager* manager) NN_NOEXCEPT
    {
        m_pLoaderManager = manager;
    }

    struct PrepareArg
    {
        const void* wsdFile;
        const void* waveFile;
        int8_t waveType;
        uint8_t padding[3];

        PrepareArg() NN_NOEXCEPT
            : wsdFile(NULL)
            , waveFile(NULL)
            , waveType(0)
        {}
    };
    void Prepare(const StartInfo& info, const PrepareArg& arg) NN_NOEXCEPT;
    void RequestLoad(const StartInfo& info, const WaveSoundLoader::Arg& arg) NN_NOEXCEPT;

    virtual void Start() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Stop() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Pause( bool flag ) NN_NOEXCEPT NN_OVERRIDE;

    //------------------------------------------------------------------
    // プレイヤーパラメータ

    void SetPanRange( float panRange ) NN_NOEXCEPT;
    void SetChannelPriority( int priority ) NN_NOEXCEPT;
    void SetReleasePriorityFix( bool fix ) NN_NOEXCEPT;

    float GetPanRange() const NN_NOEXCEPT { return m_PanRange; }
    int GetChannelPriority() const NN_NOEXCEPT { return m_Priority; }

    virtual void InvalidateData( const void* start, const void* end ) NN_NOEXCEPT NN_OVERRIDE;

    position_t GetPlaySamplePosition(bool isOriginalSamplePosition) const NN_NOEXCEPT;
    const void* GetWaveFile() const NN_NOEXCEPT { return m_pWaveFile; }

    UpdateType GetUpdateType() const NN_NOEXCEPT { return m_UpdateType; }

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        if ( m_pChannel != nullptr )
        {
            return m_pChannel->GetProcessTick(profile);
        }
        return nn::os::Tick(0);
    }

public:
    void DebugUpdate() NN_NOEXCEPT { if ( IsActive() ) { Update(); } }

private:
    virtual void OnUpdateFrameSoundThread(int updateFrameCount) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED(updateFrameCount);
        Update();
    }
    virtual void OnUpdateFrameSoundThreadWithAudioFrameFrequency(int updateFrameCount) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED(updateFrameCount);
        if ( m_UpdateType == UpdateType_AudioFrame )
        {
            Update();
        }
    }

    virtual void OnShutdownSoundThread() NN_NOEXCEPT NN_OVERRIDE { Stop(); }
    void PrepareForPlayerHeap(const PrepareArg& arg) NN_NOEXCEPT;
    bool TryAllocLoader() NN_NOEXCEPT;
    void FreeLoader() NN_NOEXCEPT;

    bool m_WavePlayFlag; // チャンネルスタートしたかどうか
    bool m_ReleasePriorityFixFlag;

    uint8_t m_Priority;
    int8_t m_WaveType;
    float m_PanRange;

    const void* m_pWsdFile;
    const void* m_pWaveFile;
    int m_WaveSoundIndex;
    StartOffsetType m_StartOffsetType;
    position_t m_StartOffset;
    int m_DelayCount;
    int m_Release;
    int m_WaveSoundParameterFlag;
    bool m_IsContextCalculationSkipMode;
    LoopInfo m_LoopInfo;

    CurveLfoParam m_LfoParam;
    WaveSoundInfo m_WaveSoundInfo;
    Channel* m_pChannel;
    UpdateType m_UpdateType;

    void FinishPlayer() NN_NOEXCEPT;
    void Update() NN_NOEXCEPT;
    bool IsChannelActive() const NN_NOEXCEPT { return ( m_pChannel != NULL ) && m_pChannel->IsActive(); }
    bool StartChannel() NN_NOEXCEPT;
    void CloseChannel() NN_NOEXCEPT;
    void UpdateChannel() NN_NOEXCEPT;
    static void ChannelCallbackFunc(
        Channel* dropChannel,
        Channel::ChannelCallbackStatus status,
        void* userData
    ) NN_NOEXCEPT;


    enum ResState
    {
        ResState_Invalid,        // 未初期化
        ResState_ReceiveLoadReq,    // ロードリクエストを受けた
        ResState_AppendLoadTask, // ロードタスクを投げた
        ResState_Assigned        // ロードされ、WaveSoundPlayer にセットされた
    };
    uint8_t m_ResState; // enum ResState
    bool m_IsInitialized;
    bool m_IsRegisterPlayerCallback;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[1];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

    WaveSoundLoaderManager* m_pLoaderManager;
    WaveSoundLoader* m_pLoader;
    WaveSoundLoader::Arg m_LoaderArg;
};

}}}} // namespace nn::atk::detail::driver

