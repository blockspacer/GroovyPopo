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

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_StreamSoundFileLoader.h>
#include <nn/atk/atk_Task.h>
#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_InstancePool.h>
#include <nn/atk/atk_LoaderManager.h>
#include <nn/atk/atk_SoundArchiveFilesHook.h>
#include <nn/atk/detail/atk_IStreamDataDecoder.h>
#include <nn/atk/detail/atk_RegionManager.h>

#include <nn/atk/fnd/io/atkfnd_FileStream.h>
#include <nn/util/util_FormatString.h>

namespace nn {
namespace atk {
namespace detail {

struct DriverCommandStreamSoundLoadHeader;
struct DriverCommandStreamSoundLoadData;

struct TrackDataInfo
{
    uint8_t volume;
    uint8_t pan;
    uint8_t span;
    uint8_t flags;   // StreamSoundFile::SurroundMode が入る
    uint8_t mainSend;
    uint8_t fxSend[ AuxBus_Count ];
    uint8_t lpfFreq;
    uint8_t biquadType;
    uint8_t biquadValue;
    uint8_t channelCount;
    uint8_t channelIndex[ WaveChannelMax ];

    void Dump() const NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("volume(%d)", volume);
        NN_DETAIL_ATK_INFO("pan(%d)", pan);
        NN_DETAIL_ATK_INFO("span(%d)", span);
        NN_DETAIL_ATK_INFO("flags(%d)", flags);
        NN_DETAIL_ATK_INFO("mainSend(%d)", mainSend);
        for ( auto i = 0; i < AuxBus_Count; ++i )
        {
            NN_DETAIL_ATK_INFO("fxSend%d(%d)", i, fxSend[i]);
        }
        NN_DETAIL_ATK_INFO("lpf(%d)", lpfFreq);
        NN_DETAIL_ATK_INFO("biquadType(%d)", biquadType);
        NN_DETAIL_ATK_INFO("biquadValue(%d)", biquadValue);
        NN_DETAIL_ATK_INFO("ch(%d)", channelCount);
        for ( auto i = 0; i < WaveChannelMax; ++i )
        {
            NN_DETAIL_ATK_INFO("chIdx%d(%d)", i, channelIndex[i]);
        }
    }
};

struct TrackDataInfos
{
    TrackDataInfo track[StreamTrackCount];
};

struct StreamDataInfoDetail
{
    SampleFormat sampleFormat;
    int sampleRate;
    bool loopFlag;
    position_t loopStart;
    size_t sampleCount;
    position_t originalLoopStart;
    position_t originalLoopEnd;

    bool isRevisionCheckEnabled;
    bool isRegionIndexCheckEnabled;
    uint32_t revisionValue;

    size_t blockSampleCount;
    size_t blockSize;
    size_t lastBlockSize;
    size_t lastBlockSampleCount;
    size_t preSkipSampleCount;

    int channelCount;
    int trackCount;
    TrackDataInfo trackInfo[StreamTrackCount];
    int regionCount;

    void SetStreamSoundInfo(const StreamSoundFile::StreamSoundInfo& info, bool isCrc32CheckEnabled) NN_NOEXCEPT;

    uint32_t GetLastBlockIndex() const NN_NOEXCEPT
    {
        return static_cast<uint32_t>( ( sampleCount - 1 ) / blockSampleCount );
    }

    position_t GetLoopStartInBlock() const NN_NOEXCEPT
    {
        return loopStart % blockSampleCount;
    }

    uint32_t GetLoopStartBlockIndex(position_t loopStartInBlock) const NN_NOEXCEPT
    {
        return static_cast<uint32_t>( (loopStart - loopStartInBlock) / blockSampleCount );
    }

    void Dump(bool isDumpTrackInfo) const NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("------------ Stream Info ------------\n");
        NN_DETAIL_ATK_INFO("fmt(%d) ", sampleFormat);
        NN_DETAIL_ATK_INFO("rate(%5d) ", sampleRate);
        NN_DETAIL_ATK_INFO("loopFlag(%d) ", loopFlag);
        NN_DETAIL_ATK_INFO("LS(%8lld) ", loopStart);
        NN_DETAIL_ATK_INFO("sampleCount(%8zu) ", sampleCount);
        NN_DETAIL_ATK_INFO("OLS(%8lld) ", originalLoopStart);
        NN_DETAIL_ATK_INFO("OLE(%8lld) ", originalLoopEnd);
        NN_DETAIL_ATK_INFO("ch(%d) ", channelCount);
        NN_DETAIL_ATK_INFO("track(%d)", trackCount);
        NN_DETAIL_ATK_INFO("regionCount(%d) ", regionCount);
        NN_DETAIL_ATK_INFO("\n");
        NN_DETAIL_ATK_INFO("[normal block] sampleCount(%zu) size(%zu)\n", blockSampleCount, blockSize);
        NN_DETAIL_ATK_INFO("[  last block] sampleCount(%zu) size(%zu)\n", blockSampleCount, blockSize);

        if (isDumpTrackInfo)
        {
            for ( auto i = 0; i < StreamTrackCount; i++ )
            {
                NN_DETAIL_ATK_INFO("  trk[%d]", i);
                trackInfo[i].Dump();
                NN_DETAIL_ATK_INFO("\n");
            }
        }
    }
};

struct LoadDataParam
{
    uint32_t blockIndex;
    size_t samples;
    position_t sampleBegin;
    position_t sampleOffset;
    size_t sampleBytes;
    bool adpcmContextEnable;
    AdpcmContextNotAligned adpcmContext[StreamChannelCount];
    int loopCount;
    bool lastBlockFlag;
    bool isStartOffsetOfLastBlockApplied;

    LoadDataParam() NN_NOEXCEPT
    {
        Initialize();
    }

    void Initialize() NN_NOEXCEPT
    {
        blockIndex = 0;
        samples = 0;
        sampleBegin = 0;
        sampleOffset = 0;
        sampleBytes = 0;
        adpcmContextEnable = false;
        loopCount = 0;
        lastBlockFlag = false;
        isStartOffsetOfLastBlockApplied = false;
    }

    void Dump() NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("blockIndex(%2d)", blockIndex);
        NN_DETAIL_ATK_INFO("length(%4zu)", samples);
        NN_DETAIL_ATK_INFO("begin(%8lld)", sampleBegin);
        NN_DETAIL_ATK_INFO("offset(%8lld)", sampleOffset);
        NN_DETAIL_ATK_INFO("bytes(%5zu)", sampleBytes);
        NN_DETAIL_ATK_INFO("isAdpcm(%5s)", adpcmContextEnable ? "true" : "false");
        NN_DETAIL_ATK_INFO("loopCount(%3d)", loopCount);
        NN_DETAIL_ATK_INFO("isLast(%5s)", lastBlockFlag ? "true" : "false");
        NN_DETAIL_ATK_INFO("isStartOffsetOfLastBlockApplied(%5s)", isStartOffsetOfLastBlockApplied ? "true" : "false");
        NN_DETAIL_ATK_INFO("\n");
    }
};

struct FileStreamHookParam
{
    FileStreamHookParam() NN_NOEXCEPT
    : pSoundArchiveFilesHook(NULL)
    , itemLabel(NULL)
    {}

    bool IsHookEnabled() const NN_NOEXCEPT { return pSoundArchiveFilesHook != NULL; }

    detail::SoundArchiveFilesHook* pSoundArchiveFilesHook;
    const char* itemLabel;
};


namespace driver {



class StreamSoundPlayer;
class StreamSoundLoader;
typedef LoaderManager<StreamSoundLoader> StreamSoundLoaderManager;

class StreamSoundLoader
{
public:
    // ストリームバッファ 1 ブロックの基本サイズ [byte]
    // (通常はこのサイズ分をロードして再生する)
    static const size_t DataBlockSizeBase = 8 * 1024;

    // 上記のサンプル数を、一番圧縮率の低い PCM16 でバイト数に換算した値
    static const size_t DataBlockSizeMargin = DataBlockSizeMarginSamples * 2;

    // 1 ブロックのサイズ [byte]
    static const size_t DataBlockSizeMax = DataBlockSizeBase + DataBlockSizeMargin;

    static const int FileStreamBufferSize = 512;

    static const int LoadBufferChannelCount = 2; // 同時にロードを行うチャンネルの数
    static const size_t LoadBufferSize = DataBlockSizeMax * LoadBufferChannelCount;

    StreamSoundLoader() NN_NOEXCEPT;
    ~StreamSoundLoader() NN_NOEXCEPT;

    void Initialize() NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    static void RegisterStreamDataDecoderManager(IStreamDataDecoderManager* pManager) NN_NOEXCEPT;
    static void UnregisterStreamDataDecoderManager(IStreamDataDecoderManager* pManager) NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;
    void ForceFinish() NN_NOEXCEPT;

    bool IsBusy() const NN_NOEXCEPT;
    bool IsInUse() NN_NOEXCEPT;

    void RequestLoadHeader() NN_NOEXCEPT;
    void RequestLoadData( void* bufferAddress[], uint32_t bufferBlockIndex, position_t startOffsetSamples, position_t prefetchOffsetSamples, int priority ) NN_NOEXCEPT;
    void RequestClose() NN_NOEXCEPT;
    void CancelRequest() NN_NOEXCEPT;

    RegionManager& GetRegionManager() NN_NOEXCEPT
    {
        return m_RegionManager;
    }

    void SetStreamSoundPlayer(StreamSoundPlayer* pPlayer) NN_NOEXCEPT
    {
        m_PlayerHandle = pPlayer;
    }

    void SetFileType(StreamFileType fileType) NN_NOEXCEPT
    {
        m_FileType = fileType;
    }

    void SetDecodeMode(DecodeMode mode) NN_NOEXCEPT
    {
        m_DecodeMode = mode;
    }

    void SetAssignNumber(uint16_t assignNumber) NN_NOEXCEPT
    {
        m_AssignNumber = assignNumber;
    }

    void SetStreamDataInfo(StreamDataInfoDetail* pStreamDataInfo) NN_NOEXCEPT
    {
        m_DataInfo = pStreamDataInfo;
    }

    void SetRegionCallback(const StreamRegionCallback& function, void* argument) NN_NOEXCEPT
    {
        m_RegionManager.SetRegionCallback( function, argument );
    }

    void SetLoopParameter(bool loopFlag, bool isLoopFlagEnabled, position_t loopStart, position_t loopEnd) NN_NOEXCEPT
    {
        m_LoopFlag = loopFlag;
        m_IsLoopFlagEnabled = isLoopFlagEnabled;
        m_LoopStart = loopStart;
        m_LoopEnd = loopEnd;
    }

    void SetAdditionalDecodingOnLoopEnabled(bool isEnabled)
    {
        m_IsAdditionalDecodingOnLoopEnabled = isEnabled;
    }

    void SetExternalData(const void* pData, size_t size) NN_NOEXCEPT
    {
        m_pExternalData = pData;
        m_ExternalDataSize = size;
    }

    void SetCacheBuffer(void* buffer, size_t size) NN_NOEXCEPT
    {
        m_pCacheBuffer = buffer;
        m_CacheSize = size;
    }

    void InitializeFileStream(bool isStreamOpenFailureHalt) NN_NOEXCEPT
    {
        m_pFileStream = NULL;
        m_IsStreamOpenFailureHalt = isStreamOpenFailureHalt;
    }

    void SetFilePath(const char* filePath, int filePathLength) NN_NOEXCEPT
    {
        std::strncpy(m_FilePath, filePath, filePathLength);
    }

    const char* GetFilePath() const NN_NOEXCEPT
    {
        return m_FilePath;
    }

    void SetFileStreamHookParam(const FileStreamHookParam& fileStreamHookParam) NN_NOEXCEPT
    {
        m_FileStreamHookParam = fileStreamHookParam;
    }

    void* detail_SetFsAccessLog( fnd::FsAccessLog* pFsAccessLog ) NN_NOEXCEPT;

    position_t detail_GetCurrentPosition() NN_NOEXCEPT;

    position_t detail_GetCachePosition() NN_NOEXCEPT;

    size_t detail_GetCachedLength() NN_NOEXCEPT;

private:
    class StreamHeaderLoadTask : public Task
    {
    public:
        void Execute(TaskProfileLogger&) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLoader);
            fnd::FndResult result = m_pLoader->Open();

            if (result.IsFailed())
            {
                // TODO: nn::fs の result 値を直接返すようにする
                const int ResultStringLengthMax = 32;
                char resultString[ResultStringLengthMax];
                switch (static_cast<fnd::FndResultType>(result))
                {
                case fnd::FndResultType_IoFileNotFound:
                    nn::util::SNPrintf(resultString, ResultStringLengthMax, "fs::ResultPathNotFound");
                    break;
                case fnd::FndResultType_IoInvalidAccess:
                    nn::util::SNPrintf(resultString, ResultStringLengthMax, "fs::ResultDataCorrupted");
                    break;
                case fnd::FndResultType_IoTargetLocked:
                    nn::util::SNPrintf(resultString, ResultStringLengthMax, "fs::ResultTargetLocked");
                    break;
                case fnd::FndResultType_IoError:
                    nn::util::SNPrintf(resultString, ResultStringLengthMax, "");
                    break;
                default:
                    NN_UNEXPECTED_DEFAULT;
                }

                if (m_pLoader->m_IsStreamOpenFailureHalt)
                {
                    NN_ABORT("StreamSoundLoader::Open(%s) is Failed. %s\n", m_pLoader->GetFilePath(), resultString);
                }
                else
                {
                    NN_ATK_WARNING("StreamSound was stopped forcibly because file open(%s) failed. %s", m_pLoader->GetFilePath(), resultString);
                    m_pLoader->ForceFinish();
                    return;
                }
            }

            m_pLoader->LoadHeader();
        }
        StreamSoundLoader* m_pLoader;
    };

    class StreamDataLoadTask : public Task
    {
    public:
        void Execute(TaskProfileLogger& logger) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLoader);
            m_pLoader->LoadData( m_BufferAddress, m_BufferBlockIndex, m_StartOffsetSamples, m_PrefetchOffsetSamples, logger );
        }

        void* m_BufferAddress[ StreamChannelCount ];
        uint32_t m_BufferBlockIndex;
        position_t m_StartOffsetSamples;
        position_t m_PrefetchOffsetSamples;

        StreamSoundLoader* m_pLoader;
        util::IntrusiveListNode m_Link;
    };

    class StreamCloseTask : public Task
    {
    public:
        void Execute(TaskProfileLogger&) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLoader);
            m_pLoader->Close();
        }
        StreamSoundLoader* m_pLoader;
    };

    typedef util::IntrusiveList<StreamDataLoadTask, util::IntrusiveListMemberNodeTraits<StreamDataLoadTask,&StreamDataLoadTask::m_Link>> StreamDataLoadTaskList;


    struct BlockInfo
    {
        BlockInfo() NN_NOEXCEPT
        : size(0)
        , samples(0)
        , startOffsetSamples(0)
        , startOffsetSamplesAlign(0)
        , startOffsetByte(0)
        , copyByte(0)
        {
        }

        size_t size;
        size_t samples;
        size_t startOffsetSamples;
        size_t startOffsetSamplesAlign;
        size_t startOffsetByte;
        size_t copyByte;

        size_t GetStartOffsetInFrame() const NN_NOEXCEPT { return startOffsetSamples - startOffsetSamplesAlign; }
    };

private:
    void WaitFinalize() NN_NOEXCEPT;

    fnd::FndResult Open() NN_NOEXCEPT;
    void Close() NN_NOEXCEPT;

    void LoadHeader() NN_NOEXCEPT;
    bool LoadHeader1( DriverCommandStreamSoundLoadHeader* command ) NN_NOEXCEPT;
    bool LoadHeaderForOpus( DriverCommandStreamSoundLoadHeader* command, StreamFileType type, DecodeMode decodeMode ) NN_NOEXCEPT;
    IStreamDataDecoderManager* SelectStreamDataDecoderManager( StreamFileType type, DecodeMode decodeMode ) NN_NOEXCEPT;

    void LoadData( void* bufferAddress[], uint32_t bufferBlockIndex, size_t startOffsetSamples, size_t prefetchOffsetSamples, TaskProfileLogger& logger ) NN_NOEXCEPT;
    bool LoadData1( DriverCommandStreamSoundLoadData* command, void* bufferAddress[], uint32_t bufferBlockIndex, size_t startOffsetSamples, size_t prefetchOffsetSamples, TaskProfileLogger& logger ) NN_NOEXCEPT;
    bool LoadDataForOpus( DriverCommandStreamSoundLoadData* command, void* bufferAddress[], uint32_t bufferBlockIndex, size_t startOffsetSamples, size_t prefetchOffsetSamples, TaskProfileLogger& logger ) NN_NOEXCEPT;
    void SetStreamSoundInfoForOpus(const IStreamDataDecoder::DataInfo& info) NN_NOEXCEPT;

    bool ApplyStartOffset(position_t startOffsetSamples, int* loopCount) NN_NOEXCEPT;
    bool MoveNextRegion(int* loopCount) NN_NOEXCEPT;

    bool ReadTrackInfoFromStreamSoundFile(StreamSoundFileReader& reader) NN_NOEXCEPT;

    bool IsLoopStartFilePos(uint32_t loadingDataBlockIndex) NN_NOEXCEPT;
    void UpdateLoadingDataBlockIndex() NN_NOEXCEPT;
    void UpdateLoadingDataBlockIndexForOpus( void* bufferAddress[] ) NN_NOEXCEPT;

    int GetLoadChannelCount(int loadStartChannel) NN_NOEXCEPT;
    bool LoadStreamBuffer(uint8_t* buffer, const BlockInfo& blockInfo, uint32_t loadChannelCount) NN_NOEXCEPT;
    bool LoadStreamBuffer(uint8_t* buffer, size_t loadSize) NN_NOEXCEPT;
    bool SkipStreamBuffer(size_t skipSize) NN_NOEXCEPT;
    void CalculateBlockInfo(BlockInfo& blockInfo) NN_NOEXCEPT;
    bool LoadOneBlockDataViaCache(void* bufferAddress[], const BlockInfo& blockInfo, position_t destAddressOffset, bool firstBlock, bool updateAdpcmContext) NN_NOEXCEPT;
    bool LoadOneBlockData(void* bufferAddress[], const BlockInfo& blockInfo, position_t destAddressOffset, bool firstBlock, bool updateAdpcmContext) NN_NOEXCEPT;

    bool LoadAdpcmContextForStartOffset() NN_NOEXCEPT;
    void UpdateAdpcmInfoForStartOffset(const void* blockBegin, int channelIndex, const BlockInfo& blockInfo) NN_NOEXCEPT;
    bool SetAdpcmInfo(
        StreamSoundFileReader& reader,
        int channelCount,
        AdpcmParam* adpcmParam[]) NN_NOEXCEPT;

    bool DecodeStreamData(void* pOutBufferAddresses[], IStreamDataDecoder::DecodeType decodeType) NN_NOEXCEPT;
    void ResetDecoder() NN_NOEXCEPT;

    bool IsStreamCacheEnabled() const NN_NOEXCEPT
    {
        return m_pCacheBuffer != nullptr && m_CacheSize > 0;
    }

private:
    StreamSoundFileLoader m_FileLoader;
    StreamSoundPlayer* m_PlayerHandle;
    detail::fnd::FileStream* m_pFileStream;
    StreamDataInfoDetail* m_DataInfo;
    StreamFileType m_FileType;
    DecodeMode m_DecodeMode;
    FileStreamHookParam m_FileStreamHookParam;

    int m_ChannelCount;
    uint16_t m_AssignNumber;
    bool m_LoopFlag;
    bool m_IsLoopFlagEnabled;
    bool m_IsStreamOpenFailureHalt;
    bool m_IsAdditionalDecodingOnLoopEnabled;
    position_t m_LoopStart;
    position_t m_LoopEnd;

    char m_FilePath[FilePathMax];
    const void* m_pExternalData;
    size_t m_ExternalDataSize;
    void* m_pCacheBuffer;
    size_t m_CacheSize;

    uint32_t m_LoadingDataBlockIndex;    // 全データのうち、現在ロード中のブロックの通し番号
    uint32_t m_LastBlockIndex;
    uint32_t m_LoopStartBlockIndex;
    position_t m_DataStartFilePos;
    position_t m_LoopStartFilePos;
    position_t m_LoopStartBlockSampleOffset;

    bool m_LoopJumpFlag;
    bool m_LoadFinishFlag;

    RegionManager m_RegionManager;

    StreamHeaderLoadTask m_StreamHeaderLoadTask;
    StreamCloseTask m_StreamCloseTask;
    StreamDataLoadTaskList m_StreamDataLoadTaskList;
    InstancePool<StreamDataLoadTask> m_StreamDataLoadTaskPool;
    uint8_t m_StreamDataLoadTaskArea[StreamDataLoadTaskMax*sizeof(StreamDataLoadTask)];

    SampleFormat m_SampleFormat;
    struct AdpcmInfo
    {
        AdpcmParam param;
        AdpcmContext beginContext;
        AdpcmContext loopContext;
    };
    AdpcmInfo m_AdpcmInfo[StreamChannelCount];

    uint32_t m_FileStreamBuffer[ FileStreamBufferSize / sizeof(uint32_t) ];

    IStreamDataDecoder* m_pStreamDataDecoder;
    IStreamDataDecoderManager* m_pStreamDataDecoderManager;

    static uint8_t g_LoadBuffer[ LoadBufferSize ];

public:
    util::IntrusiveListNode m_LinkForLoaderManager;  // for StreamSoundLoaderManager
};

} // nn::atk::detail::driver
} // nn::atk::detail
} // nn::atk
} // nn
