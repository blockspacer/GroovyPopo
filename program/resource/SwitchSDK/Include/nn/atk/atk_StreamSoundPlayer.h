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
 * :include nn/atk/atk_StreamSoundPlayer.h
 *
 * @file atk_StreamSoundPlayer.h
 */

#pragma once

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_StreamTrack.h>
#include <nn/atk/atk_InstancePool.h>
#include <nn/atk/atk_Task.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_StreamSoundLoader.h>
#include <nn/atk/atk_StreamSoundPrefetchFileReader.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief    ストリームデータのパラメータセットです。
//!
//!           この情報は @ref StreamSoundHandle::ReadStreamSoundDataInfo か
//!           @ref SoundArchivePlayer::ReadStreamSoundDataInfo から取得できます。
//!
//! @see StreamSoundHandle::ReadStreamSoundDataInfo
//! @see SoundArchivePlayer::ReadStreamSoundDataInfo
//!
//---------------------------------------------------------------------------
struct StreamSoundDataInfo
{
    //---------------------------------------------------------------------------
    //! @brief    ストリームデータがループするなら true、
    //!           終端で終了するなら false となります。
    //---------------------------------------------------------------------------
    bool loopFlag;

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータのサンプルレートです。
    //---------------------------------------------------------------------------
    int sampleRate;

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータがループする時のループ開始位置を、
    //!           ストリームの先頭からのサンプル数で表します。
    //---------------------------------------------------------------------------
    int64_t loopStart;

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータがループする時のループ終了位置を、
    //!           ストリームの先頭からのサンプル数で表します。
    //!           ループしない時は、データの終端をサンプル数で表します。
    //!           (再生される最後のサンプルの次のサンプルを指します)
    //---------------------------------------------------------------------------
    int64_t loopEnd;

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータがループする時のループ開始位置を、
    //!           ストリームの先頭からのサンプル数で表します。
    //!
    //!           0.3.0 より loopStart の値が、ハードウェアの制約による
    //!           補正がかかる前の値を返すように変更されました。
    //!           以前の値を取得したい場合にはこちらの値を使用してください。
    //!           ただし、このメンバの使用は非推奨で、将来的に削除予定です。
    //---------------------------------------------------------------------------
    int64_t compatibleLoopStart;

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータがループする時のループ終了位置を、
    //!           ストリームの先頭からのサンプル数で表します。
    //!           ループしない時は、データの終端をサンプル数で表します。
    //!
    //!           0.3.0 より loopEnd の値が、ハードウェアの制約による
    //!           補正がかかる前の値を返すように変更されました。
    //!           以前の値を取得したい場合にはこちらの値を使用してください。
    //!           ただし、このメンバの使用は非推奨で、将来的に削除予定です。
    //---------------------------------------------------------------------------
    int64_t compatibleLoopEnd;

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータのチャンネル数です。
    //---------------------------------------------------------------------------
    int channelCount;

    //! @internal
    void Dump() NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("StreamSoundDataInfo::%s loop?(%d) rate(%d) loopStart(%lld) loopEnd(%lld) compatibleLoopStart(%lld) compatibleLoopEnd(%lld) channelCount(%d)\n",
                __FUNCTION__, loopFlag, sampleRate, loopStart, loopEnd, compatibleLoopStart, compatibleLoopEnd, channelCount);
    }
};

//---------------------------------------------------------------------------
//! @brief    ストリームデータのパラメータセットです。
//!
//! @deprecated この構造体は廃止予定です。代わりに @ref StreamSoundDataInfo を使用してください。
//---------------------------------------------------------------------------
NN_DEPRECATED typedef StreamSoundDataInfo StreamDataInfo;

//---------------------------------------------------------------------------
//! @brief    ストリームデータのリージョン情報です。
//!
//!           この情報は @ref SoundArchivePlayer::ReadStreamSoundRegionDataInfo から取得できます。
//!
//! @see SoundArchivePlayer::ReadStreamSoundRegionDataInfo
//!
//---------------------------------------------------------------------------
struct StreamSoundRegionDataInfo
{
    //---------------------------------------------------------------------------
    //! @brief    リージョンの開始サンプル位置です。
    //---------------------------------------------------------------------------
    uint32_t startSamplePosition;

    //---------------------------------------------------------------------------
    //! @brief    リージョンの終了サンプル位置です。
    //---------------------------------------------------------------------------
    uint32_t endSamplePosition;

    //---------------------------------------------------------------------------
    //! @brief    リージョン番号です。
    //---------------------------------------------------------------------------
    int regionNo;

    //---------------------------------------------------------------------------
    //! @brief    リージョン名です。
    //---------------------------------------------------------------------------
    char regionName[RegionNameLengthMax + 1];
};


//---------------------------------------------------------------------------
//! @brief    ストリームデータのマーカー情報です。
//---------------------------------------------------------------------------
struct StreamSoundMarkerInfo
{
    uint32_t position;                //< サンプル位置です
    char name[MarkerNameLengthMax + 1];   //< マーカー名です。
};

namespace detail {
namespace driver {

class StreamSoundPlayer;
class StreamBufferPool;

class StreamSoundPlayer : public BasicSoundPlayer, public SoundThread::PlayerCallback
{
    /* ------------------------------------------------------------------------
            constant definition
       ------------------------------------------------------------------------ */
public:
    enum StartOffsetType
    {
        StartOffsetType_Sample,
        StartOffsetType_Millisec
    };

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
    StreamSoundPlayer() NN_NOEXCEPT;
    virtual ~StreamSoundPlayer() NN_NOEXCEPT NN_OVERRIDE;

    virtual void Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;

    void SetLoaderManager(StreamSoundLoaderManager* manager) NN_NOEXCEPT
    {
        m_pLoaderManager = manager;
    }

    struct SetupArg
    {
        StreamBufferPool* pBufferPool;
        uint32_t allocChannelCount;
        uint16_t allocTrackFlag;
        uint8_t fileType;    // enum StreamFileType
        bool loopFlag;
        bool loopFlagEnabled;
        TrackDataInfos trackInfos;
        position_t loopStart;
        position_t loopEnd;
        float pitch;
        uint8_t mainSend;
        uint8_t fxSend[AuxBus_Count];
        detail::DecodeMode decodeMode;
        bool isAdditionalDecodingOnLoopEnabled;
    };
    void Setup(const SetupArg& arg) NN_NOEXCEPT;

    struct PrepareBaseArg
    {
        driver::StreamSoundPlayer::StartOffsetType startOffsetType;
        position_t offset;
        int delayTime;
        int delayCount;
        UpdateType updateType;
        StreamRegionCallback regionCallback;
        void* regionCallbackArg;
        char filePath[detail::FilePathMax];
        const void* pExternalData;
        size_t externalDataSize;
        FileStreamHookParam fileStreamHookParam;

        PrepareBaseArg() NN_NOEXCEPT
            : startOffsetType(StartOffsetType_Sample)
            , offset(0)
            , delayTime(0)
            , delayCount(0)
            , updateType(UpdateType_AudioFrame)
            , regionCallback(NULL)
            , regionCallbackArg(NULL)
            , pExternalData(nullptr)
            , externalDataSize(0)
            , fileStreamHookParam()
        {
            std::memset(filePath, 0, sizeof(char) * detail::FilePathMax);
        }
    };
    struct PrepareArg
    {
        PrepareBaseArg baseArg;
        void* cacheBuffer;
        size_t cacheSize;

        PrepareArg() NN_NOEXCEPT
            : cacheBuffer(NULL)
            , cacheSize(0)
        {}
    };
    void Prepare(const PrepareArg& arg) NN_NOEXCEPT;
    struct PreparePrefetchArg
    {
        PrepareBaseArg baseArg;
        const void* strmPrefetchFile;

        PreparePrefetchArg() NN_NOEXCEPT
            : strmPrefetchFile(NULL)
        {}
    };
    void PreparePrefetch(const PreparePrefetchArg& arg) NN_NOEXCEPT;

    virtual void Start() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Stop() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Pause( bool flag ) NN_NOEXCEPT NN_OVERRIDE;

    //------------------------------------------------------------------
    bool IsFinalizing() const NN_NOEXCEPT { return m_IsFinalizing; }
    bool IsSuspendByLoadingDelay() const NN_NOEXCEPT { return m_LoadWaitFlag; }
    bool IsLoadingDelayState() const NN_NOEXCEPT;
    bool IsPrepared() const NN_NOEXCEPT { return m_IsPrepared || m_IsPreparedPrefetch; }

    //------------------------------------------------------------------
    // トラックパラメータ
    void SetTrackVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackInitialVolume( uint32_t trackBitFlag, uint32_t volume ) NN_NOEXCEPT;
    void SetTrackOutputLine( uint32_t trackBitFlag, uint32_t outputLine ) NN_NOEXCEPT;
    void ResetTrackOutputLine( uint32_t trackBitFlag ) NN_NOEXCEPT;

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
    // 情報取得
    bool ReadStreamSoundDataInfo( StreamSoundDataInfo* info ) const NN_NOEXCEPT;
    int GetPlayLoopCount() const NN_NOEXCEPT { return IsActive() ? m_LoopCounter + m_PlayingBlockLoopCounter : -1; }
    position_t GetPlaySamplePosition(bool isOriginalSamplePosition) const NN_NOEXCEPT;
    float GetFilledBufferPercentage() const NN_NOEXCEPT;
    int GetBufferBlockCount(nn::atk::WaveBuffer::Status status) const NN_NOEXCEPT;
    int GetTotalBufferBlockCount() const NN_NOEXCEPT;
    int GetActiveChannelCount() const NN_NOEXCEPT { return IsActive() ? m_ChannelCount : 0; }
    int GetActiveTrackCount() const NN_NOEXCEPT { return IsActive() ? m_TrackCount : 0; }

    //------------------------------------------------------------------
    // トラック取得
    StreamTrack* GetPlayerTrack( int trackNo ) NN_NOEXCEPT;
    const StreamTrack* GetPlayerTrack( int trackNo ) const NN_NOEXCEPT;

    //------------------------------------------------------------------
    // コマンド処理
    bool LoadHeader(
        bool result,
        AdpcmParam* adpcmParam[],
        uint16_t assignNumber
    ) NN_NOEXCEPT;
    bool LoadStreamData(
        bool result,
        const LoadDataParam& loadDataParam,
        uint16_t assignNumber
    ) NN_NOEXCEPT;
    bool LoadStreamData(
        bool result,
        const LoadDataParam& loadDataParam,
        uint16_t assignNumber,
        bool usePrefetchFlag,
        uint32_t currentPrefetchBlockIndex,
        size_t currentPrefetchBlockBytes
    ) NN_NOEXCEPT;

    void ForceFinish() NN_NOEXCEPT { SetFinishFlag(true); }

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        nn::os::Tick totalTick(0);
        for ( auto channelIndex = 0; channelIndex < m_ChannelCount; channelIndex++ )
        {
            // 内部で使用する nn::audio:::AudioRenderer 上の全チャンネルの負荷の総和を取得
            totalTick += m_Channels[channelIndex].GetProcessTick(profile);
        }
        return totalTick;
    }

    void* detail_SetFsAccessLog( fnd::FsAccessLog* pFsAccessLog ) NN_NOEXCEPT
    {
        if( m_pLoader != nullptr )
        {
            return m_pLoader->detail_SetFsAccessLog(pFsAccessLog);
        }

        return nullptr;
    }

    //------------------------------------------------------------------
protected:
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

    virtual void SetActiveFlag(bool isActive) NN_NOEXCEPT NN_OVERRIDE;

    //------------------------------------------------------------------
private:
    struct ItemData
    {
        float pitch;
        float mainSend;
        float fxSend[ AuxBus_Count ];

        void Set( const SetupArg& arg ) NN_NOEXCEPT;
    };

    struct TrackData
    {
        float volume;
        float lpfFreq;
        int   biquadType;
        float biquadValue;
        float pan;
        float span;
        float mainSend;
        float fxSend[ AuxBus_Count ];

        void Set( const StreamTrack* track ) NN_NOEXCEPT;
    };

    struct PrefetchLoadDataParam : public LoadDataParam
    {
        uint32_t prefetchBlockIndex;
        size_t prefetchBlockBytes;
    };

    struct PrefetchIndexInfo
    {
        uint32_t lastBlockIndex;
        position_t loopStartInBlock;
        uint32_t loopStartBlockIndex;
        int loopBlockCount;

        void Initialize( const StreamDataInfoDetail& streamDataInfo ) NN_NOEXCEPT;

        bool IsOverLastBlock( uint32_t blockIndex ) NN_NOEXCEPT
        {
            return blockIndex > lastBlockIndex;
        }

        uint32_t GetBlockOffsetFromLoopEnd( uint32_t blockIndex ) NN_NOEXCEPT
        {
            return ( blockIndex - lastBlockIndex ) % loopBlockCount;
        }

        bool IsLoopStartBlock( uint32_t blockOffsetFromLoopEnd ) NN_NOEXCEPT
        {
            return blockOffsetFromLoopEnd == 1;
        }

        bool IsLastBlock( uint32_t blockIndex, uint32_t blockOffsetFromLoopEnd ) NN_NOEXCEPT
        {
            return ( lastBlockIndex == 0 ) || ( blockIndex == lastBlockIndex ) || ( IsOverLastBlock( blockIndex ) && ( blockOffsetFromLoopEnd == 0 ) );
        }
    };

    void StartPlayer() NN_NOEXCEPT;
    void FinishPlayer() NN_NOEXCEPT;
    bool SetupPlayer() NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;
    void UpdateBuffer() NN_NOEXCEPT;
    void UpdateVoiceParams( StreamTrack* track ) NN_NOEXCEPT;

    void SetOutputParam( OutputParam* pOutOutputParam, const OutputParam& trackParam, const TrackData& trackData ) NN_NOEXCEPT;

    void MixSettingForOutputParam( OutputParam* pOutOutputParam, OutputBusMixVolume* pOutOutputBusMixVolume, int channelIndex, MixMode mixMode ) NN_NOEXCEPT;
    void ApplyTvOutputParamForMultiChannel( const OutputParam& outputParam, const OutputAdditionalParam* const pOutputAdditionalParam, MultiVoice* pVoice, int channelIndex, MixMode mixMode ) NN_NOEXCEPT;
    void ApplyDrcOutputParamForMultiChannel( const OutputParam& outputParam, const OutputAdditionalParam* const pOutputAdditionalParam, MultiVoice* pVoice, int channelIndex, MixMode mixMode, uint32_t drcIndex ) NN_NOEXCEPT;

    bool AllocVoices() NN_NOEXCEPT;
    void FreeVoices() NN_NOEXCEPT;

    bool TryAllocLoader() NN_NOEXCEPT;
    void FreeLoader() NN_NOEXCEPT;

    bool AllocStreamBuffers() NN_NOEXCEPT;
    void FreeStreamBuffers() NN_NOEXCEPT;

    void UpdateLoadingBlockIndex() NN_NOEXCEPT;
    void UpdatePauseStatus() NN_NOEXCEPT;
    bool CheckDiskDriveError() const NN_NOEXCEPT;

    void UpdateTrackActiveFlag(int trackIndex, bool isActive) NN_NOEXCEPT;
    void UpdatePlaySamplePosition() NN_NOEXCEPT;
    void UpdatePlaySamplePosition(position_t playSamplePosition, position_t originalPlaySamplePosition) NN_NOEXCEPT;

    bool IsBufferEmpty() const NN_NOEXCEPT;
    bool IsStoppedByLoadingDelay() const NN_NOEXCEPT;

    bool SetupTrack( const SetupArg& arg ) NN_NOEXCEPT;
    void SetPrepareBaseArg( const PrepareBaseArg& baseArg ) NN_NOEXCEPT;
    void RequestLoadHeader( const PrepareArg& arg ) NN_NOEXCEPT;
    bool ReadPrefetchFile( StreamSoundPrefetchFileReader& reader ) NN_NOEXCEPT;
    bool ApplyStreamDataInfo( const StreamDataInfoDetail& streamDataInfo ) NN_NOEXCEPT;
    void SetPrepared(bool isPrepared) NN_NOEXCEPT;

    position_t GetOriginalPlaySamplePosition( position_t playSamplePosition, const StreamDataInfoDetail& streamDataInfo ) const NN_NOEXCEPT;
    int GetOriginalLoopCount(position_t playSamplePosition, const StreamDataInfoDetail & streamDataInfo) const NN_NOEXCEPT;
    position_t GetStartOffsetSamples( const StreamDataInfoDetail& streamDataInfo ) NN_NOEXCEPT;
    bool IsValidStartOffset( const StreamDataInfoDetail& streamDataInfo ) NN_NOEXCEPT;
    void ApplyTrackDataInfo( const StreamDataInfoDetail& streamDataInfo ) NN_NOEXCEPT;

    bool CheckPrefetchRevision(const StreamDataInfoDetail& streamDataInfo) const NN_NOEXCEPT;
    bool LoadPrefetchBlocks(StreamSoundPrefetchFileReader& reader) NN_NOEXCEPT;
    void PreparePrefetchOnLastBlock(PrefetchLoadDataParam* param, const PrefetchIndexInfo& indexInfo) NN_NOEXCEPT;
    bool PreparePrefetchOnLoopStartBlock(PrefetchLoadDataParam* param, const PrefetchIndexInfo& indexInfo, StreamSoundPrefetchFileReader& reader) NN_NOEXCEPT;
    void PreparePrefetchOnLoopBlock(PrefetchLoadDataParam* param, const PrefetchIndexInfo& indexInfo, uint32_t blockOffsetFromLoopEnd) NN_NOEXCEPT;
    bool PreparePrefetchOnNormalBlock(PrefetchLoadDataParam* param, uint32_t blockIndex, StreamSoundPrefetchFileReader& reader) NN_NOEXCEPT;

    bool SetAdpcmInfo(
        StreamSoundPrefetchFileReader& reader,
        const StreamDataInfoDetail& streamDataInfo,
        AdpcmParam* adpcmParam,
        AdpcmContextNotAligned* adpcmContext) NN_NOEXCEPT;
    bool SetAdpcmLoopInfo(
        StreamSoundPrefetchFileReader& reader,
        const StreamDataInfoDetail& streamDataInfo,
        AdpcmParam* adpcmParam,
        AdpcmContextNotAligned* adpcmContext) NN_NOEXCEPT;

    static void VoiceCallbackFunc(
        MultiVoice* voice,
        MultiVoice::VoiceCallbackStatus status,
        void* arg
    ) NN_NOEXCEPT;

    bool m_IsInitialized;       // Setup が呼ばれた
    bool m_IsPrepared;          // 準備完了フラグ
    bool m_IsFinalizing;
    bool m_IsPreparedPrefetch;

    bool m_PauseStatus;
    bool m_LoadWaitFlag;        // バッファが溜まるまで、一時停止解除を遅延させる
    bool m_LoadFinishFlag;
    bool m_ReportLoadingDelayFlag;      // ロード遅延メッセージ
    bool m_IsStoppedByLoadingDelay;

    bool m_IsRegisterPlayerCallback;
    bool m_UseDelayCount;               // 互換性保持のため、DelayCountを使うかどうかの判定に使用します。

    int m_LoopCounter;
    int m_PlayingBlockLoopCounter;
    int m_PrepareCounter;

    StreamSoundLoaderManager* m_pLoaderManager;
    StreamSoundLoader* m_pLoader;

    // ストリームバッファ
    StreamBufferPool* m_pBufferPool;
    int m_BufferBlockCount;  // 波形バッファ数

    // NOTE: StreamBufferPool からロード用のバッファが与えられるが、
    //       これを、bfstm 内の 1 ブロック (現在は 8KB で固定) に分割して利用する。
    //       以下に出てくる「ブロック」とは、この 8KB で区切られたメモリ領域を指す。
    uint32_t m_LoadingBufferBlockIndex;        // ロード用バッファのうち、ロード中のブロックの通し番号

    uint32_t m_PlayingBufferBlockIndex;        // ロード用バッファのうち、再生中のブロックの通し番号
    uint32_t m_LastPlayFinishBufferBlockIndex; // ロード用バッファのうち、最後に再生完了したブロックの通し番号


    StartOffsetType m_StartOffsetType;
    position_t m_StartOffset;
    int m_DelayCount;

    uint16_t m_AssignNumber;

    // ストリームデータ情報
    uint8_t m_FileType;  // enum StreamFileType
    bool m_IsAdditionalDecodingOnLoopEnabled;
    detail::DecodeMode m_DecodeMode;
    bool m_LoopFlag;
    bool m_IsLoopFlagEnabled;
    StreamDataInfoDetail m_StreamDataInfo;  // snd_StreamSoundLoader.h
    position_t m_LoopStart;
    position_t m_LoopEnd;
    ItemData m_ItemData;

    // プリフェッチデータ情報
    const void*                                     m_pStreamPrefetchFile;
    AdpcmParam                                      m_PrefetchAdpcmParam[StreamChannelCount];
    StreamSoundPrefetchFileReader::PrefetchDataInfo m_PrefetchDataInfo;
    position_t                                      m_PrefetchOffset;

    bool                                            m_IsPrefetchRevisionCheckEnabled;
    uint32_t                                        m_PrefetchRevisionValue;

    // トラックとチャンネル
    int m_ChannelCount;
    int m_TrackCount;
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN StreamChannel m_Channels[ StreamChannelCount ];
    StreamTrack m_Tracks[ StreamTrackCount ];
    UpdateType m_UpdateType;

    struct WaveBufferInfo
    {
        position_t sampleBegin;
        size_t sampleLength;
        int loopCount; // このバッファの再生が完了したら、何回ループ回数が加算されるか
    };
    WaveBufferInfo m_WaveBufferInfo[ StreamDataLoadTaskMax ];

    StreamSoundPlayer::PrepareArg m_PrepareArg;
    bool m_IsSucceedPrepare;
    StreamSoundPlayer::PreparePrefetchArg m_PreparePrefetchArg;
    bool m_IsPreparePrefetchRequested;
    StreamSoundPlayer::SetupArg m_SetupArg;

    static uint16_t g_AssignNumberCount;

    // サンプル再生位置
    position_t m_PlaySamplePosition;
    position_t m_OriginalPlaySamplePosition;

    mutable detail::fnd::CriticalSection m_UpdateCriticalSection;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

