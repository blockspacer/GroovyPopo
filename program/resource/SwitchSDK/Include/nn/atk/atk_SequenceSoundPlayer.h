/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * :include nn/atk/atk_SequenceSoundPlayer.h
 *
 * @file atk_SequenceSoundPlayer.h
 */

#pragma once

#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_NoteOnCallback.h>      // NoteOnCallback, NoteOnInfo
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_BankFileReader.h>
#include <nn/atk/atk_WaveArchiveFileReader.h>
#include <nn/atk/atk_LoaderManager.h>
#include <nn/atk/atk_Task.h>

namespace nn {
namespace atk {

// シーケンスコマンド'userproc'用コールバック
//---------------------------------------------------------------------------
//! @brief    'userproc' コマンドで呼び出されるコールバックで使用される構造体です。
//!
//!           'userproc' コマンドの詳細については、
//!           シーケンスデータマニュアルを参照してください。
//!
//! @see SequenceUserProcCallback
//!
//---------------------------------------------------------------------------
struct SequenceUserProcCallbackParam
{
    //---------------------------------------------------------------------------
    //! @brief    ローカル変数の配列です。
    //!           localVariable[0] から localVariable[15] まで指定できます。
    //!           変数の値を参照、あるいは変更することができます。
    //---------------------------------------------------------------------------
    volatile int16_t* localVariable;

    //---------------------------------------------------------------------------
    //! @brief    グローバル変数の配列です。
    //!           globalVariable[0] から globalVariable[15] まで指定できます。
    //!           変数の値を参照、あるいは変更することができます。
    //---------------------------------------------------------------------------
    volatile int16_t* globalVariable;

    //---------------------------------------------------------------------------
    //! @brief    トラック変数の配列です。
    //!           trackVariable[0] から trackVariable[15] まで指定できます。
    //!           変数の値を参照、あるいは変更することができます。
    //---------------------------------------------------------------------------
    volatile int16_t* trackVariable;

    //---------------------------------------------------------------------------
    //! @brief    比較コマンドで設定される真偽のフラグです。
    //!           フラグの状態を参照、あるいは変更することができます。
    //---------------------------------------------------------------------------
    bool cmpFlag;
};

//---------------------------------------------------------------------------
//! @brief    シーケンスサウンドで使用可能なユーザープロシージャのコールバックです。
//!
//!           このコールバックはシーケンスデータ内の 'userproc'
//!           コマンドによって呼び出されます。
//!           'userproc' コマンドが処理されたフレームで、
//!           サウンドスレッドから呼び出されます。
//!
//!           procId は、シーケンスデータで 'userproc'
//!           コマンドのパラメータとして指定された値が渡されます。
//!
//!           param は、コールバック内で利用できるパラメータです。
//!           データで設定されてる値を参照することができます。
//!           また、この値を書き換えることでサウンドデータに反映することが出来ます。
//!
//!           'userproc' コマンドの詳細については、
//!           シーケンスデータマニュアルを参照してください。
//!
//! @param[in] procId     シーケンスデータで指定されたプロシージャ ID です。
//! @param[in] param      コールバック内で利用できるパラメータです。
//! @param[in] arg        ユーザー引数です。
//!
//! @see SoundArchivePlayer::SetSequenceUserProcCallback
//! @see SequenceUserProcCallbackParam
//!
//---------------------------------------------------------------------------
typedef void (*SequenceUserProcCallback)(
    uint16_t procId,
    SequenceUserProcCallbackParam* param,
    void* arg
);

class SoundDataManager;
class SoundPlayer;

namespace detail {

class PlayerHeap;

namespace driver {

/* ========================================================================
        typename declaration
   ======================================================================== */

class SequenceTrack;
class SequenceTrackAllocator;

/* ========================================================================
        class definition
   ======================================================================== */
class SequenceSoundLoader;
typedef LoaderManager<SequenceSoundLoader> SequenceSoundLoaderManager;

// プレイヤーヒープへのロードをつかさどる
class SequenceSoundLoader
{
public:
    struct LoadInfo
    {
        const SoundArchive* soundArchive;
        const SoundDataManager* soundDataManager;
        LoadItemInfo* loadInfoSeq;
        LoadItemInfo* loadInfoBanks[SoundArchive::SequenceBankMax];
        SoundPlayer* soundPlayer;

        LoadInfo(
            const SoundArchive* arc,
            const SoundDataManager* mgr,
            LoadItemInfo* seq,
            LoadItemInfo* banks,
            SoundPlayer* player) NN_NOEXCEPT;
    };

    struct Data
    {
        const void* seqFile;
        const void* bankFiles[SoundArchive::SequenceBankMax];   // TODO: バンクの数は SoundArchive への依存ではなく、SequenceSound 側で定義すべき
        const void* warcFiles[SoundArchive::SequenceBankMax];
        bool warcIsIndividuals[SoundArchive::SequenceBankMax];

        Data() NN_NOEXCEPT { Initialize(); }
        void Initialize() NN_NOEXCEPT
        {
            seqFile = NULL;
            for ( int i = 0; i < SoundArchive::SequenceBankMax; i++ )
            {
                bankFiles[i] = NULL;
                warcFiles[i] = NULL;
                warcIsIndividuals[i] = false;
            }
        }
    };

    struct Arg
    {
        const SoundArchive* soundArchive;
        const SoundDataManager* soundDataManager;
        SoundPlayer* soundPlayer;
        LoadItemInfo loadInfoSeq;
        LoadItemInfo loadInfoBanks[SoundArchive::SequenceBankMax];
        // LoadItemInfo loadInfoWarcs[SoundArchive::SeqBankMax];
        // bool warcIsIndividuals は必要？？？

        Arg() NN_NOEXCEPT : soundArchive(NULL), soundDataManager(NULL), soundPlayer(NULL)
        {}
    };

    class DataLoadTask : public Task
    {
    public:
        void Initialize() NN_NOEXCEPT;
        virtual void Execute(TaskProfileLogger& logger) NN_NOEXCEPT NN_OVERRIDE;
        bool TryAllocPlayerHeap() NN_NOEXCEPT;

        Arg m_Arg;
        Data m_Data;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
        bool m_IsLoadSuccess;
        uint8_t m_Padding[3];
    };

    class FreePlayerHeapTask : public Task
    {
    public:
        void Initialize() NN_NOEXCEPT;
        virtual void Execute(TaskProfileLogger& logger) NN_NOEXCEPT NN_OVERRIDE;

        Arg m_Arg;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
    };

    ~SequenceSoundLoader() NN_NOEXCEPT;

    bool IsInUse() NN_NOEXCEPT;
    void Initialize(const Arg& arg) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool TryWait() NN_NOEXCEPT;
    bool IsLoadSuccess() const NN_NOEXCEPT { return m_Task.m_IsLoadSuccess; }

    const Data& GetData() const NN_NOEXCEPT { return m_Task.m_Data; }

private:
    DataLoadTask m_Task;
    FreePlayerHeapTask m_FreePlayerHeapTask;
    PlayerHeapDataManager m_PlayerHeapDataManager;

public:
    util::IntrusiveListNode m_LinkForLoaderManager; // for SequenceSoundLoaderManager
};


class SequenceSoundPlayer : public BasicSoundPlayer, public DisposeCallback, public SoundThread::PlayerCallback
{
    /* ------------------------------------------------------------------------
            constant variable
       ------------------------------------------------------------------------ */
public:
    static const int PlayerVariableCount  = 16;
    static const int GlobalVariableCount  = 16;
    static const int TrackCountPerPlayer  = 16;
    static const uint32_t AllTrackBitFlag = (1 << TrackCountPerPlayer) - 1; // 全てのトラックを含むビットフラグ
    static const int VariableDefaultValue = -1;

    static const int DefaultTimebase      = 48; /* 四分音符分解能 */
    static const int DefaultTempo         = 120;

    // オフセット再生時、1 オーディオフレームあたりのデフォルトスキップ幅 (tick単位)
    static const int DefaultSkipIntervalTick = 48 * 4 * 4;

    /* ------------------------------------------------------------------------
            type definition
       ------------------------------------------------------------------------ */
public:
    struct ParserPlayerParam
    {
        uint8_t priority;
        uint8_t timebase;
        uint16_t tempo;
        MoveValue<uint8_t,int16_t> volume;

        NoteOnCallback* callback;

        ParserPlayerParam() NN_NOEXCEPT
        : priority(64),
          timebase(DefaultTimebase), tempo(DefaultTempo),
          callback(NULL)
        {
            volume.InitValue(127);
        }
    };

    enum StartOffsetType
    {
        StartOffsetType_Tick,
        StartOffsetType_Millisec
    };

    struct StartInfo
    {
        int32_t seqOffset;
        StartOffsetType startOffsetType;
        int startOffset;
        int32_t delayTime;
        int32_t delayCount;
        UpdateType updateType;
    };

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    static void InitSequenceSoundPlayer() NN_NOEXCEPT;

    static void SetSkipIntervalTick( int intervalTick ) NN_NOEXCEPT;
    static int GetSkipIntervalTick() NN_NOEXCEPT;

    SequenceSoundPlayer() NN_NOEXCEPT;
    virtual ~SequenceSoundPlayer() NN_NOEXCEPT;

    void Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
    void Finalize() NN_NOEXCEPT NN_OVERRIDE;

    void SetLoaderManager(SequenceSoundLoaderManager* manager) NN_NOEXCEPT
    {
        m_pLoaderManager = manager;
    }

    struct SetupArg
    {
        SequenceTrackAllocator* trackAllocator;
        uint32_t allocTracks;
        NoteOnCallback* callback;

        SetupArg() NN_NOEXCEPT : trackAllocator(NULL), allocTracks(0), callback(NULL)
        {}
    };
    void Setup(const SetupArg& arg) NN_NOEXCEPT;

    bool IsPrepared() const NN_NOEXCEPT
    {
        if (m_ResState >= ResState_Assigned)
        {
            return true;
        }
        return false;
    }

    struct PrepareArg
    {
        const void* seqFile; // シーケンスファイル
        const void* bankFiles[SeqBankMax];  // バンクファイル
        const void* warcFiles[SeqBankMax];  // 波形アーカイブファイル
        bool warcIsIndividuals[SeqBankMax];
        int32_t seqOffset;
        int32_t delayTime;
        int32_t delayCount;
        UpdateType updateType;

        PrepareArg() NN_NOEXCEPT : seqFile(NULL), seqOffset(NULL), delayTime(0), delayCount(0), updateType(UpdateType_AudioFrame)
        {
            for (int i = 0; i < SeqBankMax; i++)
            {
                bankFiles[i] = NULL;
                warcFiles[i] = NULL;
                warcIsIndividuals[i] = false;
            }
        }
    };
    void Prepare(const PrepareArg& arg) NN_NOEXCEPT;
    void RequestLoad(const StartInfo& info, const SequenceSoundLoader::Arg& arg) NN_NOEXCEPT;

    // (Atktool 用)
    // トラックを強制的にミュートします。シーケンスコマンドによる設定より優先されます。
    // (初期化後、使用開始の前に一度だけ設定される想定です)
    void ForceTrackMute(uint32_t trackMask) NN_NOEXCEPT;

    virtual void Start() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Stop() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Pause( bool flag ) NN_NOEXCEPT NN_OVERRIDE;
    void Skip( StartOffsetType offsetType, int offset ) NN_NOEXCEPT;

    Channel* NoteOn(
        uint8_t bankIndex,
        const NoteOnInfo& noteOnInfo
    ) NN_NOEXCEPT;

    void SetSequenceUserprocCallback( SequenceUserProcCallback callback, void* arg ) NN_NOEXCEPT;
    void CallSequenceUserprocCallback( uint16_t procId, SequenceTrack* track ) NN_NOEXCEPT;

    //------------------------------------------------------------------
    // プレイヤーパラメータ
    void SetTempoRatio( float tempoRatio ) NN_NOEXCEPT;
    void SetPanRange( float panRange ) NN_NOEXCEPT;
    void SetChannelPriority( int priority ) NN_NOEXCEPT;
    void SetReleasePriorityFix( bool fix ) NN_NOEXCEPT;

    float GetTempoRatio() const NN_NOEXCEPT { return m_TempoRatio; }
    float GetPanRange() const NN_NOEXCEPT { return m_PanRange; }
    int GetChannelPriority() const NN_NOEXCEPT { return m_ParserParam.priority; }

    bool IsReleasePriorityFix() const NN_NOEXCEPT { return m_ReleasePriorityFixFlag; }

    //------------------------------------------------------------------
    // トラックパラメータ
    void SetTrackMute( uint32_t trackBitFlag, SequenceMute mute ) NN_NOEXCEPT;
    void SetTrackSilence( unsigned long trackBitFlag, bool silenceFlag, int fadeTimes ) NN_NOEXCEPT;
    void SetTrackVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackPitch( uint32_t trackBitFlag, float pitch ) NN_NOEXCEPT;
    void SetTrackLpfFreq( uint32_t trackBitFlag, float lpfFreq ) NN_NOEXCEPT;
    void SetTrackBiquadFilter( uint32_t trackBitFlag, int type, float value ) NN_NOEXCEPT;
    bool SetTrackBankIndex( uint32_t trackBitFlag, int bankIndex ) NN_NOEXCEPT;
    void SetTrackTranspose( uint32_t trackBitFlag, int8_t transpose ) NN_NOEXCEPT;
    void SetTrackVelocityRange( uint32_t trackBitFlag, uint8_t range ) NN_NOEXCEPT;
    void SetTrackOutputLine( uint32_t trackBitFlag, uint32_t outputLine ) NN_NOEXCEPT;
    void ResetTrackOutputLine( uint32_t trackBitFlag ) NN_NOEXCEPT;

    const BankFileReader& GetBankFileReader( uint8_t bankIndex ) const NN_NOEXCEPT
    {
        return m_BankFileReader[bankIndex];
    }
    const WaveArchiveFileReader& GetWaveArchiveFileReader( uint8_t bankIndex ) const NN_NOEXCEPT
    {
        // 波形アーカイブは、バンクと１：１対応している
        return m_WarcFileReader[bankIndex];
    }

    void SetTrackTvVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackChannelTvMixParameter( uint32_t trackBitFlag, uint32_t srcChNo, const MixParameter& param ) NN_NOEXCEPT;
    void SetTrackTvPan( uint32_t trackBitFlag, float pan ) NN_NOEXCEPT;
    void SetTrackTvSurroundPan( uint32_t trackBitFlag, float span ) NN_NOEXCEPT;
    void SetTrackTvMainSend( uint32_t trackBitFlag, float send ) NN_NOEXCEPT;
    void SetTrackTvFxSend( uint32_t trackBitFlag, AuxBus bus, float send ) NN_NOEXCEPT;

    void SetTrackDrcVolume( uint32_t drcIndex, uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackChannelDrcMixParameter( uint32_t drcIndex, uint32_t trackBitFlag, uint32_t srcChNo, const MixParameter& param ) NN_NOEXCEPT;
    void SetTrackDrcPan( uint32_t drcIndex, uint32_t trackBitFlag, float pan ) NN_NOEXCEPT;
    void SetTrackDrcSurroundPan( uint32_t drcIndex, uint32_t trackBitFlag, float span ) NN_NOEXCEPT;
    void SetTrackDrcMainSend( uint32_t drcIndex, uint32_t trackBitFlag, float send ) NN_NOEXCEPT;
    void SetTrackDrcFxSend( uint32_t drcIndex, uint32_t trackBitFlag, AuxBus bus, float send ) NN_NOEXCEPT;

    //------------------------------------------------------------------
    // シーケンス変数
    int16_t GetLocalVariable( int varNo ) const NN_NOEXCEPT;
    static int16_t GetGlobalVariable( int varNo ) NN_NOEXCEPT;
    void SetLocalVariable( int varNo, int16_t var ) NN_NOEXCEPT;
    static void SetGlobalVariable( int varNo, int16_t var ) NN_NOEXCEPT;

    volatile int16_t* GetVariablePtr( int varNo ) NN_NOEXCEPT;

    //------------------------------------------------------------------
    // invalidate
    virtual void InvalidateData( const void* start, const void* end ) NN_NOEXCEPT NN_OVERRIDE;

    //------------------------------------------------------------------
    // info
    const ParserPlayerParam& GetParserPlayerParam() const NN_NOEXCEPT { return m_ParserParam; }
    ParserPlayerParam& GetParserPlayerParam() NN_NOEXCEPT { return m_ParserParam; }
    uint32_t GetTickCounter() const NN_NOEXCEPT { return m_TickCounter; }
    UpdateType GetUpdateType() const NN_NOEXCEPT
    {
        return m_UpdateType;
    }
    OutputReceiver* GetOutputReceiver() NN_NOEXCEPT
    {
        return BasicSoundPlayer::GetOutputReceiver();
    }


    //------------------------------------------------------------------
    SequenceTrack* GetPlayerTrack( int trackNo ) NN_NOEXCEPT;
    const SequenceTrack* GetPlayerTrack( int trackNo ) const NN_NOEXCEPT;
    void SetPlayerTrack( int trackNo, SequenceTrack* track ) NN_NOEXCEPT;

    const SequenceTrackAllocator* GetTrackAllocator() NN_NOEXCEPT { return m_pSequenceTrackAllocator; }

    void Update(int updateFrameCount) NN_NOEXCEPT;

    virtual void ChannelCallback( Channel* channel ) NN_NOEXCEPT { (void)channel; }

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT;

protected:
    // バンクと波形アーカイブのみを対象とした Prepare() です。
    void PrepareForMidi(
        const void* banks[],    // バンクファイル (4つ)
        const void* warcs[],    // 波形アーカイブファイル (4つ)
        bool warcIsIndividuals[] ) NN_NOEXCEPT;

private:
    virtual void OnUpdateFrameSoundThread(int updateFrameCount) NN_NOEXCEPT NN_OVERRIDE
    {
        Update(updateFrameCount);
    }
    virtual void OnUpdateFrameSoundThreadWithAudioFrameFrequency(int updateFrameCount) NN_NOEXCEPT NN_OVERRIDE
    {
        if ( m_UpdateType == UpdateType_AudioFrame )
        {
            Update(updateFrameCount);
        }
    }

    virtual void OnShutdownSoundThread() NN_NOEXCEPT NN_OVERRIDE { Stop(); }

    void PrepareForPlayerHeap(const PrepareArg& arg) NN_NOEXCEPT;
    bool TryAllocLoader() NN_NOEXCEPT;
    void FreeLoader() NN_NOEXCEPT;

    template< typename T >
    void SetTrackParam( uint32_t trackBitFlag, void (SequenceTrack::*func)( T ), T param ) NN_NOEXCEPT;

    template< typename T1, typename T2 >
    void SetTrackParam(
            uint32_t trackBitFlag,
            void (SequenceTrack::*func)( T1, T2 ),
            T1 t1, T2 t2 ) NN_NOEXCEPT;

    template< typename T1, typename T2, typename T3 >
    void SetTrackParam(
            uint32_t trackBitFlag,
            void (SequenceTrack::*func)( T1, T2, T3 ),
            T1 t1, T2 t2, T3 t3 ) NN_NOEXCEPT;

    template< typename T1, typename T2, typename T3, typename T4 >
    void SetTrackParam(
        uint32_t trackBitFlag,
        void (SequenceTrack::*func)( T1, T2, T3, T4 ),
        T1 t1, T2 t2, T3 t3, T4 t4 ) NN_NOEXCEPT;

    int  ParseNextTick( bool doNoteOn ) NN_NOEXCEPT;

    void UpdateChannelParam() NN_NOEXCEPT;
    void UpdateTick(int updateFrameCount) NN_NOEXCEPT;
    void SkipTick() NN_NOEXCEPT;

    void CloseTrack( int trackNo ) NN_NOEXCEPT;
    void FinishPlayer() NN_NOEXCEPT;

    float CalcTickPerMinute() const NN_NOEXCEPT { return m_ParserParam.timebase * m_ParserParam.tempo * m_TempoRatio; }
    float CalcTickPerMsec() const NN_NOEXCEPT { return CalcTickPerMinute() / ( 60 * 1000.0f ); }

    static volatile int16_t m_GlobalVariable[ GlobalVariableCount ];
    static volatile int32_t m_SkipIntervalTickPerFrame;

    bool m_ReleasePriorityFixFlag;
    bool m_IsPrepared;

    float m_PanRange;
    float m_TempoRatio;
    float m_TickFraction;
    uint32_t m_SkipTickCounter;
    float m_SkipTimeCounter;

    int32_t m_DelayCount;

    ParserPlayerParam m_ParserParam;
    SequenceTrackAllocator* m_pSequenceTrackAllocator;

    SequenceUserProcCallback m_SequenceUserprocCallback;
    void* m_pSequenceUserprocCallbackArg;

    SequenceTrack* m_pTracks[ TrackCountPerPlayer ];

    volatile int16_t m_LocalVariable[ PlayerVariableCount ];
    volatile uint32_t m_TickCounter;

    WaveArchiveFileReader m_WarcFileReader[SoundArchive::SequenceBankMax];
    BankFileReader m_BankFileReader[SoundArchive::SequenceBankMax];

    enum ResState
    {
        ResState_Invalid,        // 未初期化
        ResState_RecvLoadReq,    // ロードリクエストを受けた
        ResState_AppendLoadTask, // ロードタスクを投げた
        ResState_Assigned        // ロードされ、SequenceSoundPlayer にセットされた
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
    StartInfo m_StartInfo;

    SequenceSoundLoaderManager* m_pLoaderManager;
    SequenceSoundLoader* m_pLoader;
    SequenceSoundLoader::Arg m_LoaderArg;
    UpdateType m_UpdateType;
}; // class SequenceSoundPlayer



template< typename T >
void SequenceSoundPlayer::SetTrackParam( uint32_t trackBitFlag, void (SequenceTrack::*func)( T ), T param ) NN_NOEXCEPT
{
    for( int trackNo = 0;
         trackNo < TrackCountPerPlayer && trackBitFlag != 0 ;
         trackNo++, trackBitFlag >>= 1
    )
    {
        if ( ( trackBitFlag & 0x01 ) == 0 ) continue;
        SequenceTrack* track = GetPlayerTrack( trackNo );
        if ( track != NULL ) (track->*func)( param );
    }
}

template< typename T1, typename T2 >
void SequenceSoundPlayer::SetTrackParam(
        uint32_t trackBitFlag,
        void (SequenceTrack::*func)( T1, T2 ),
        T1 t1, T2 t2 ) NN_NOEXCEPT
{
    for( int trackNo = 0;
         trackNo < TrackCountPerPlayer && trackBitFlag != 0 ;
         trackNo++, trackBitFlag >>= 1
    )
    {
        if ( ( trackBitFlag & 0x01 ) == 0 ) continue;
        SequenceTrack* track = GetPlayerTrack( trackNo );
        if ( track != NULL ) (track->*func)( t1, t2 );
    }
}

template< typename T1, typename T2, typename T3 >
void SequenceSoundPlayer::SetTrackParam(
        uint32_t trackBitFlag,
        void (SequenceTrack::*func)( T1, T2, T3 ),
        T1 t1, T2 t2, T3 t3 ) NN_NOEXCEPT
{
    for( int trackNo = 0;
         trackNo < TrackCountPerPlayer && trackBitFlag != 0 ;
         trackNo++, trackBitFlag >>= 1
    )
    {
        if ( ( trackBitFlag & 0x01 ) == 0 ) continue;
        SequenceTrack* track = GetPlayerTrack( trackNo );
        if ( track != NULL ) (track->*func)( t1, t2, t3 );
    }
}

template< typename T1, typename T2, typename T3, typename T4 >
void SequenceSoundPlayer::SetTrackParam(
    uint32_t trackBitFlag,
    void (SequenceTrack::*func)( T1, T2, T3, T4 ),
    T1 t1, T2 t2, T3 t3, T4 t4 ) NN_NOEXCEPT
{
    for( int trackNo = 0;
        trackNo < TrackCountPerPlayer && trackBitFlag != 0 ;
        trackNo++, trackBitFlag >>= 1
        )
    {
        if ( ( trackBitFlag & 0x01 ) == 0 ) continue;
        SequenceTrack* track = GetPlayerTrack( trackNo );
        if ( track != NULL ) (track->*func)( t1, t2, t3, t4 );
    }
}

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

