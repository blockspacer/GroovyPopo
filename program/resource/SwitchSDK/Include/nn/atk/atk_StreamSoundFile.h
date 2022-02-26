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

#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_BinaryFileFormat.h>

namespace nn {
namespace atk {

namespace detail {

/*
    ストリームファイル (*.bfstm) の構造

    bfstm
     |
     +-- FileHeader
     +-- InfoBlock
     |    |
     |    +-- BinaryBlockHeader
     |    +-- InfoBlockBody
     |         |
     |         +-- reference to StreamSoundInfo --+
     |         |                                  |
     |         |   +------------------------------+
     |         |   |
     |         |   +-> StreamSoundInfo
     |         |        |
     |         |        +-- uint8_t     encodeMethod
     |         |        +-- bool   isLoop
     |         |        +-- uint8_t     channelCount
     |         |        +-- uint8_t     regionCount
     |         |        +-- uint32_t    sampleRate
     |         |        +-- uint32_t    loopStart
     |         |        +-- uint32_t    frameCount
     |         |        +-- uint32_t    blockCount
     |         |        +-- uint32_t    oneBlockBytes
     |         |        +-- uint32_t    oneBlockSamples
     |         |        +-- uint32_t    lastBlockBytes
     |         |        +-- uint32_t    lastBlockSamples
     |         |        +-- uint32_t    lastBlockPaddedBytes
     |         |        +-- uint32_t    sizeofSeekInfoAtom
     |         |        +-- uint32_t    seekInfoIntervalSamples
     |         |        +-- reference to sampleDataOffset
     |         |        +-- uint16_t    regionInfoBytes
     |         |        +-- uint8_t     padding[2]
     |         |        +-- reference to regionDataOffset
     |         |        +-- uint32_t    originalLoopStart
     |         |        +-- uint32_t    originalLoopEnd
     |         |
     |         +-- reference to TrackInfoTable --+
     |         |                                 |
     |         |   +-----------------------------+
     |         |   |
     |         |   +-> TrackInfoTable
     |         |        |
     |         |        +-- int32_t count
     |         |        +-- reference to TrackInfo[0] --+
     |         |                      :                 |
     |         |   +------------------------------------+
     |         |   |
     |         |   +-> TrackInfo
     |         |        |
     |         |        +-- uint8_t  volume
     |         |        +-- uint8_t  pan
     |         |        +-- uint8_t  span
     |         |        +-- uint8_t  flags
     |         |        +-- reference to GlobalChannelIndexTable --+
     |         |                                                   |
     |         |   +-----------------------------------------------+
     |         |   |
     |         |   +-> GlobalChannelIndexTable
     |         |        |
     |         |        +-- uint32_t count
     |         |        +-- uint8_t globalChannelIndex[0]
     |         |                   :
     |         |
     |         +-- reference to ChannelInfoTable --+
     |                                             |
     |             +-------------------------------+
     |             |
     |             +-> ChannelInfoTable
     |                  |
     |                  +-- uint32_t count
     |                  +-- reference to ChannelInfo[0] --+
     |                                :                   |
     |             +--------------------------------------+
     |             |
     |             +-> ChannelInfo
     |                  |
     |                  +-- reference to detailChannelInfo --+
     |                                                       |
     |             +-----------------------------------------+
     |             |
     |             +-> DspAdpcmChannelInfo
     |             .    |
     |             .    +-- DspAdpcm
     |             .    |    |
     |             .    |    +-- uint16_t coef[16]
     |             .    |    +-- uint16_t predScale
     |             .    |    +-- uint16_t yn1
     |             .    |    +-- uint16_t yn2
     |             .    |
     |             .    +-- DspAdpcmLoopParam
     |             .         |
     |             .         +-- uint16_t loopPredScale
     |             .         +-- uint16_t loopYn1
     |             .         +-- uint16_t loopYn2
     |             .
     |             +-> (他のエンコード情報もあるかも)
     |
     +-- SeekBlock (途中再生用のヒストリーデータが入る。現在は DSP ADPCM 専用)
     |    |        (ブロック全体はメモリに展開されない。
     |    |        必要な部分のみ StreamSoundFileLoader でロードされる)
     |    +-- BinaryBlockHeader
     |    +-- { uint16_t yn1, uint16_t yn2 } が、チャンネル数 * ブロック数だけ並ぶ
     |
     +-- RegionBlock
     |    |
     |    +-- BinaryBlockHeader
     |    +-- RegionBlockBody
     |         |
     |         +-- RegionInfo item[regionCount] // regionCount は INFO ブロックに格納済み
     |              |
     |              +-- uint32_t start
     |              +-- uint32_t end
     |              +-- DspAdpcmContext item[channelCount]
     |              ------ RegionInfo 1 つあたり 256 バイトになるように、パディングする ----
     |
     +-- MarkerBlock
     |    |
     |    +-- BinaryBlockHeader
     |    +-- MarkerBlockBody
     |         |
     |         +-- int32_t markerInfoCount;
     |         +-- MarkerInfo info[markerInfoCount]
     |              |
     |              +-- uint32_t position
     |              +-- uint32_t padding
     |              +-- char name[MarkerNameLengthMax + 1]
     |
     +-- DataBlock
          |
          +-- BinaryBlockHeader
          +-- (ブロック全体はメモリに展開されない。
               必要な部分のみ StreamSoundFileLoader でロードされる)

*/
struct StreamSoundFile
{
    // 前方宣言
    struct InfoBlock;
    struct InfoBlockBody;

    // ファイルヘッダー
    // メモ : ストリームファイルは、ヘッダーおよび INFO ブロックのみ先にロードするので、
    //        Util::SoundFileHeader を継承できない。
    struct FileHeader : public nn::atk::detail::BinaryFileHeader
    {
    private:
        // 定数
        static const int BlockCount = 5;

    public:
        // データ
        Util::ReferenceWithSize toBlocks[ BlockCount ];

        bool HasSeekBlock() const NN_NOEXCEPT;
        bool HasRegionBlock() const NN_NOEXCEPT;
        bool HasMarkerBlock() const NN_NOEXCEPT;

        // 各ブロックのサイズ
        uint32_t GetInfoBlockSize() const NN_NOEXCEPT;
        uint32_t GetSeekBlockSize() const NN_NOEXCEPT;
        uint32_t GetDataBlockSize() const NN_NOEXCEPT;
        uint32_t GetRegionBlockSize() const NN_NOEXCEPT;
        uint32_t GetMarkerBlockSize() const NN_NOEXCEPT;

        // ファイル先頭から各ブロックへのオフセット
        uint32_t GetInfoBlockOffset() const NN_NOEXCEPT;
        uint32_t GetSeekBlockOffset() const NN_NOEXCEPT;
        uint32_t GetDataBlockOffset() const NN_NOEXCEPT;
        uint32_t GetRegionBlockOffset() const NN_NOEXCEPT;
        uint32_t GetMarkerBlockOffset() const NN_NOEXCEPT;

        const InfoBlock* GetInfoBlock() const NN_NOEXCEPT
        {
            return static_cast<const InfoBlock*>(
                    util::ConstBytePtr( this, GetInfoBlockOffset() ).Get() );
        }

    private:
        const Util::ReferenceWithSize* GetReferenceBy( uint16_t typeId ) const NN_NOEXCEPT;
    };

    // --------------------------
    // INFO ブロック
    struct StreamSoundInfo;
    struct TrackInfoTable;
    struct ChannelInfoTable;
    struct InfoBlockBody
    {
        // データ
        Util::Reference toStreamSoundInfo;
        Util::Reference toTrackInfoTable;
        Util::Reference toChannelInfoTable;

        // アクセサ
        const StreamSoundInfo* GetStreamSoundInfo() const NN_NOEXCEPT;
        const TrackInfoTable* GetTrackInfoTable() const NN_NOEXCEPT;
        const ChannelInfoTable* GetChannelInfoTable() const NN_NOEXCEPT;
    };

    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct StreamSoundInfo
    {
        uint8_t          encodeMethod;   // WaveFile::EncodeMethod が入る
        bool        isLoop;
        uint8_t          channelCount;
        uint8_t          regionCount;    // REGN ブロックにいくつ RegionInfo が含まれているか？
        uint32_t  sampleRate;
        uint32_t  loopStart;
        uint32_t  frameCount;
        uint32_t  blockCount;

        uint32_t  oneBlockBytes;
        uint32_t  oneBlockSamples;

        uint32_t  lastBlockBytes;
        uint32_t  lastBlockSamples;
        uint32_t  lastBlockPaddedBytes;

        uint32_t  sizeofSeekInfoAtom;         // シーク情報のサイズ (1ch 分)
        uint32_t  seekInfoIntervalSamples;

        Util::Reference sampleDataOffset;       // DATA ブロックボディ先頭から、
                                                // サンプルデータへのオフセット
        uint16_t  regionInfoBytes;    // RegionInfo 1 つあたりのサイズ (バイト数)
        uint8_t   padding[2];
        Util::Reference regionDataOffset;       // REGN ブロックボディ先頭から、
                                                // RegionInfo[] へのオフセット
        uint32_t   originalLoopStart;
        uint32_t   originalLoopEnd;

        uint32_t   crc32Value;                  // プリフェッチ使用時、 stm と stp のリビジョンが一致しているかを確認するハッシュ値
    };

    struct TrackInfo;
    struct TrackInfoTable
    {
        // データ
        Util::ReferenceTable table;

        // アクセサ
        const TrackInfo* GetTrackInfo( uint32_t index ) const NN_NOEXCEPT;
        uint32_t GetTrackCount() const NN_NOEXCEPT { return table.count; }
    };

    struct GlobalChannelIndexTable;
    struct TrackInfo
    {
        // データ
        uint8_t volume;
        uint8_t pan;
        uint8_t span;
        uint8_t flags;   // enum SurroundMode が入る
        Util::Reference toGlobalChannelIndexTable;

        // アクセサ
        inline uint32_t GetTrackChannelCount() const NN_NOEXCEPT
        {
            return GetGlobalChannelIndexTable().GetCount();
        }
        inline uint8_t GetGlobalChannelIndex( uint32_t index ) const NN_NOEXCEPT
        {
            return GetGlobalChannelIndexTable().GetGlobalIndex( index );
        }
    private:
        const GlobalChannelIndexTable& GetGlobalChannelIndexTable() const NN_NOEXCEPT
        {
            return *reinterpret_cast<const GlobalChannelIndexTable*>(
                    util::ConstBytePtr(
                        this,
                        toGlobalChannelIndexTable.offset ).Get() );
        }
    };
    struct GlobalChannelIndexTable
    {
        // データ
        Util::Table<uint8_t> table;
            // トラック内のローカルチャンネルインデックスと、
            // ストリームサウンド全体のグローバルチャンネルインデックスを紐付けます

        // アクセサ
        inline uint32_t GetCount() const NN_NOEXCEPT { return table.count; }
        inline uint8_t GetGlobalIndex( uint32_t index ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( index < table.count );
            return table.item[ index ];
        }
    };

    struct ChannelInfo;
    struct ChannelInfoTable
    {
        // データ
        Util::ReferenceTable table;

        // アクセサ
        inline uint32_t GetChannelCount() const NN_NOEXCEPT { return table.count; }
        const ChannelInfo* GetChannelInfo( uint32_t index ) const NN_NOEXCEPT;
    };

    struct DspAdpcmChannelInfo; // エンコードによっては、ほかにもあるかも
    struct ChannelInfo
    {
        Util::Reference toDetailChannelInfo;
        const DspAdpcmChannelInfo* GetDspAdpcmChannelInfo() const NN_NOEXCEPT;
    };

    struct DspAdpcmChannelInfo
    {
        // データ
        DspAdpcmParam       param;
        DspAdpcmLoopParam   loopParam;
    };

    // --------------------------
    // SEEK ブロック (途中再生用の情報)
    struct SeekBlock
    {
        BinaryBlockHeader header;

        // ブロック全体はメモリに展開されない。
        // ファイルストリーム経由で、Seek, Read するため、
        // アクセサは StreamSoundFileLoader に任せる。
    };

    // --------------------------
    // DATA ブロック (サンプルデータ)
    struct DataBlock
    {
        BinaryBlockHeader header;

        // ブロック全体はメモリに展開されない。
        // ファイルストリーム経由で、Seek, Read するため、
        // アクセサは StreamSoundFileLoader に任せる。
    };

    // --------------------------
    // REGN ブロック (リージョンデータ)
    struct RegionBlock
    {
        BinaryBlockHeader header;
        // このうしろ + regionDataOffset だけ離れた先に、
        // RegionInfo が regionCount 個並んでいる
    };

    struct RegionInfo
    {
        uint32_t start;                           // リージョン開始サンプル位置
        uint32_t end;                             // リージョン終了サンプル位置
        DspAdpcmLoopParam adpcmContext[16];       // 16 チャンネル分 (＝ 2ch/trk * 8trk 分) 並ぶ
        bool isEnabled;                           // 有効なリージョンかどうか
        uint8_t padding[87];                      // RegionInfo 1 つあたり 256 バイトになるように調整
        char regionName[RegionNameLengthMax + 1]; // リージョン名
    };
    NN_STATIC_ASSERT(sizeof(RegionInfo) == 256);

    // --------------------------
    // MRKR ブロック (マーカーデータ)
    struct MarkerInfoTable;
    struct MarkerBlockBody
    {
        // データ
        int32_t markerInfoCount;
        // この後ろに MarkerInfo が markerInfoCount 個並ぶ
    };

    struct MarkerBlock
    {
        BinaryBlockHeader header;
        MarkerBlockBody body;
    };

    struct MarkerInfo
    {
        uint32_t position;
        uint32_t padding;
        char name[MarkerNameLengthMax + 1];
    };
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

