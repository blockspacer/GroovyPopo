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

#include <nn/atk/atk_BinaryFileFormat.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_Global.h>              // PanMode など
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_ElementType.h>
#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/detail/atk_BinaryTypes.h>

namespace nn {
namespace atk {
namespace detail {

struct SoundArchiveFile
{
    // ------------------------------------------------------------------------
    //  ファイルヘッダー

    // 前方宣言
    struct StringBlock;
    struct InfoBlock;
    struct FileBlock;

    // 定数
    static const int BlockCount = 3;

    // メモ : SoundArchiveFile::FileHeader はヘッダだけ個別にロードするので、
    //        Util::SoundFileHeader を継承できない。
    struct FileHeader : public nn::atk::detail::BinaryFileHeader
    {
        Util::ReferenceWithSize toBlocks[ BlockCount ];

#if 0
        // ファイルを全ロードした場合
        const StringBlock*  GetStringBlock() const;
        const InfoBlock*    GetInfoBlock() const;
        const FileBlock*    GetFileBlock() const;
#endif

        // 各ブロックのサイズ
        uint32_t GetStringBlockSize() const NN_NOEXCEPT;
        uint32_t GetInfoBlockSize() const NN_NOEXCEPT;
        uint32_t GetFileBlockSize() const NN_NOEXCEPT;

        // 各ブロックのオフセット
        int GetStringBlockOffset() const NN_NOEXCEPT;
        int GetInfoBlockOffset() const NN_NOEXCEPT;
        int GetFileBlockOffset() const NN_NOEXCEPT;

    private:
        const Util::ReferenceWithSize* GetReferenceBy( uint16_t typeId ) const NN_NOEXCEPT;
    };


    // ------------------------------------------------------------------------
    //  文字列ブロック

    struct StringTable;
    struct PatriciaTree;
    struct StringBlockBody
    {
    private:
        enum Sections
        {
            Sections_StringTable,
            Sections_PatriciaTree,
            Sections_Max = Sections_PatriciaTree
        };

    public:
        // データ
        Util::Reference toSection[ Sections_Max ];

        // アクセサ
        const char* GetString( SoundArchive::StringId stringId ) const NN_NOEXCEPT;
        uint32_t GetStringCount() const NN_NOEXCEPT { return GetStringTable()->GetCount(); }

        inline uint32_t GetItemId( const char* str ) const NN_NOEXCEPT
        {
            return GetItemIdImpl( Sections_PatriciaTree, str );
        }

        // デバッグ
        void DumpTree() const NN_NOEXCEPT;

    private:
        const void* GetSection( Sections section ) const NN_NOEXCEPT;
        inline const StringTable* GetStringTable() const NN_NOEXCEPT
        {
            return reinterpret_cast<const StringTable*>(
                    GetSection( Sections_StringTable ) );
        }
        inline const PatriciaTree* GetPatriciaTree( Sections section ) const NN_NOEXCEPT
        {
            return reinterpret_cast<const PatriciaTree*>(
                    GetSection( section ) );
        }
        uint32_t GetItemIdImpl( Sections section, const char* str ) const NN_NOEXCEPT;
    };

    struct PatriciaTree
    {
        struct NodeData
        {
            uint32_t stringId;        // 文字列 ID
            uint32_t itemId;          // アイテム ID (サウンドID、バンクIDなど)
        };
        struct Node
        {
            static const uint16_t FlagLeaf = ( 1 << 0 );
            uint16_t flags;
            uint16_t bit;
            uint32_t leftIdx;
            uint32_t rightIdx;
            NodeData nodeData;
        };

        uint32_t rootIdx;
        Util::Table<Node> nodeTable;

        const NodeData* GetNodeDataBy( const char* str, std::size_t len = 0 ) const NN_NOEXCEPT;

        // ut_ResDictionary.h の ResDicPatricia のインタフェイスを踏襲
        // のちのち、役立つことがあるかもしれない。
        void* operator[](int idx) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( idx >= 0L && idx <= static_cast<int>( nodeTable.count - 1 ));
            // 辞書引き関連についてはconst correctnessを維持しなくても問題ないだろう
            return const_cast<void*>(
                    reinterpret_cast<const void*>( &nodeTable.item[ idx ] ) );
        }
        void* operator[](uint32_t idx) const NN_NOEXCEPT
        {
            return operator[]( static_cast<int>(idx) );
        }
        void* operator[](const char* s) const NN_NOEXCEPT
        {
            return operator()( s, 0 );
        }
        void* operator()(const char* s, size_t len) const NN_NOEXCEPT
        {
            return const_cast<void*>(
                    reinterpret_cast<const void*>( GetNodeDataBy( s, len ) ) );
        }
        uint32_t GetDataCount() const NN_NOEXCEPT { return nodeTable.count; }
        uint32_t GetCount() const NN_NOEXCEPT { return GetDataCount(); }
    };

    struct StringBlock
    {
        BinaryBlockHeader header;
        StringBlockBody body;
    };

    struct StringTable
    {
        // データ
        Util::ReferenceWithSizeTable table;

        // アクセサ
        const char* GetString( int stringId ) const NN_NOEXCEPT
        {
            return reinterpret_cast<const char*>(
                    util::ConstBytePtr( this, table.item[ stringId ].offset ).Get() );
        }
        uint32_t GetCount() const NN_NOEXCEPT
        {
            return table.count;
        }
    };


    // ------------------------------------------------------------------------
    //  情報ブロック

    // 前方宣言
    struct SoundInfo;
    struct BankInfo;
    struct PlayerInfo;
    struct SoundGroupInfo;
    struct GroupInfo;
    struct WaveArchiveInfo;
    struct FileInfo;
    struct SoundArchivePlayerInfo;

    struct InfoBlockBody
    {
        //
        // データ
        //
        Util::Reference toSoundInfoReferenceTable;
        Util::Reference toSoundGroupInfoReferenceTable;
        Util::Reference toBankInfoReferenceTable;
        Util::Reference toWaveArchiveInfoReferenceTable;
        Util::Reference toGroupInfoReferenceTable;
        Util::Reference toPlayerInfoReferenceTable;
        Util::Reference toFileInfoReferenceTable;
        Util::Reference toSoundArchivePlayerInfo;

        //
        // アクセサ
        //
        inline uint32_t GetSoundCount() const       NN_NOEXCEPT { return GetSoundInfoReferenceTable().count; }
        inline uint32_t GetBankCount() const        NN_NOEXCEPT { return GetBankInfoReferenceTable().count; }
        inline uint32_t GetPlayerCount() const      NN_NOEXCEPT { return GetPlayerInfoReferenceTable().count; }
        inline uint32_t GetSoundGroupCount() const  NN_NOEXCEPT { return GetSoundGroupInfoReferenceTable().count; }
        inline uint32_t GetGroupCount() const       NN_NOEXCEPT { return GetGroupInfoReferenceTable().count; }
        inline uint32_t GetWaveArchiveCount() const NN_NOEXCEPT { return GetWaveArchiveInfoReferenceTable().count; }
        inline uint32_t GetFileCount() const        NN_NOEXCEPT { return GetFileInfoReferenceTable().count; }

        const SoundInfo*        GetSoundInfo( SoundArchive::ItemId soundId ) const NN_NOEXCEPT;
        const BankInfo*         GetBankInfo( SoundArchive::ItemId bankId ) const NN_NOEXCEPT;
        const PlayerInfo*       GetPlayerInfo( SoundArchive::ItemId playerId ) const NN_NOEXCEPT;
        const SoundGroupInfo*   GetSoundGroupInfo( SoundArchive::ItemId soundGroupId ) const NN_NOEXCEPT;
        const GroupInfo*        GetGroupInfo( SoundArchive::ItemId groupId ) const NN_NOEXCEPT;
        const WaveArchiveInfo*  GetWaveArchiveInfo( SoundArchive::ItemId warcId ) const NN_NOEXCEPT;
        const FileInfo*         GetFileInfo( SoundArchive::FileId fileId ) const NN_NOEXCEPT;

        const SoundArchivePlayerInfo* GetSoundArchivePlayerInfo() const NN_NOEXCEPT;

        SoundArchive::FileId    GetItemFileId( SoundArchive::ItemId id ) const NN_NOEXCEPT;
        SoundArchive::StringId  GetItemStringId( SoundArchive::ItemId id ) const NN_NOEXCEPT;
        SoundArchive::FileId    GetItemPrefetchFileId( SoundArchive::ItemId id ) const NN_NOEXCEPT;

    private:
        const Util::ReferenceTable& GetSoundInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetBankInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetPlayerInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetSoundGroupInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetGroupInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetWaveArchiveInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetFileInfoReferenceTable() const NN_NOEXCEPT;
    };

    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };


    struct StreamSoundInfo;
    struct WaveSoundInfo;
    struct AdvancedWaveSoundInfo;
    struct SequenceSoundInfo;
    struct Sound3DInfo;

    struct SoundInfo
    {
        // データ
        uint32_t      fileId;
        uint32_t      playerId;
        uint8_t       volume;
        uint8_t       remoteFilter;
        uint8_t       padding[2];
        Util::Reference toDetailSoundInfo;  // {Sequence,Stream,Wave}SoundInfo への参照
        Util::BitFlag   optionParameter;

        // アクセサ
        SoundArchive::SoundType      GetSoundType() const NN_NOEXCEPT;
        const StreamSoundInfo&       GetStreamSoundInfo() const NN_NOEXCEPT;
        const WaveSoundInfo&         GetWaveSoundInfo() const NN_NOEXCEPT;
        const AdvancedWaveSoundInfo& GetAdvancedWaveSoundInfo() const NN_NOEXCEPT;
        const SequenceSoundInfo&     GetSequenceSoundInfo() const NN_NOEXCEPT;
        const Sound3DInfo*           GetSound3DInfo() const NN_NOEXCEPT;
            // いつか、3D サウンド情報を削ることもありそうなので、ポインタを返すようにする

        uint32_t         GetStringId() const NN_NOEXCEPT;
        PanMode          GetPanMode() const NN_NOEXCEPT;
        PanCurve         GetPanCurve() const NN_NOEXCEPT;
        SinglePlayType   GetSinglePlayType() const NN_NOEXCEPT;
        uint16_t         GetSinglePlayEffectiveDuration() const NN_NOEXCEPT;
        uint8_t          GetPlayerPriority() const NN_NOEXCEPT;
        uint8_t          GetActorPlayerId() const NN_NOEXCEPT;
        uint32_t         GetUserParam() const NN_NOEXCEPT;       // 通常の uint32_t ユーザーパラメータ
        bool        ReadUserParam( uint32_t* pOutValue, int index ) const NN_NOEXCEPT;    // index:0-3 まで利用可能
        bool        IsFrontBypass() const NN_NOEXCEPT;

        // TODO: SEND/MOD パラメータの取得関数
    };

    struct StreamTrackInfo;
    struct StreamTrackInfoTable
    {
        // データ
        Util::ReferenceTable table;

        // アクセサ
        const StreamTrackInfo* GetTrackInfo( uint32_t index ) const NN_NOEXCEPT;
        uint32_t GetTrackCount() const NN_NOEXCEPT { return table.count; }
    };

    struct SendValue
    {
        uint8_t mainSend;
        uint8_t fxSend[AuxBus_Count];
    };

    struct StreamTrackInfo
    {
        // データ
        uint8_t volume;
        uint8_t pan;
        uint8_t span;
        uint8_t flags;   // enum SurroundMode が入る
        Util::Reference toGlobalChannelIndexTable;
        Util::Reference toSendValue;
        uint8_t lpfFreq;
        uint8_t biquadType;
        uint8_t biquadValue;
        uint8_t padding[1];

        // アクセサ
        inline uint32_t GetTrackChannelCount() const NN_NOEXCEPT
        {
            return GetGlobalChannelIndexTable().GetCount();
        }
        inline uint8_t GetGlobalChannelIndex( uint32_t index ) const NN_NOEXCEPT
        {
            return GetGlobalChannelIndexTable().GetGlobalIndex( index );
        }
        const SendValue& GetSendValue() const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( toSendValue.typeId ==
                    nn::atk::detail::ElementType_SoundArchiveFile_SendInfo );
            return *reinterpret_cast<const SendValue*>(
                    util::ConstBytePtr( this, toSendValue.offset ).Get() );
        }

    private:
        const StreamSoundFile::GlobalChannelIndexTable& GetGlobalChannelIndexTable() const NN_NOEXCEPT
        {
            return *reinterpret_cast<const StreamSoundFile::GlobalChannelIndexTable*>(
                    util::ConstBytePtr(
                        this,
                        toGlobalChannelIndexTable.offset ).Get() );
        }
    };

    struct StreamSoundExtension
    {
        uint32_t streamTypeInfo;
        uint32_t loopStartFrame;
        uint32_t loopEndFrame;

        SoundArchive::StreamFileType GetStreamFileType() const NN_NOEXCEPT
        {
            return static_cast<SoundArchive::StreamFileType>( Util::DevideBy8bit( streamTypeInfo, 0 ) );
        }

        bool IsLoop() const NN_NOEXCEPT
        {
            return Util::DevideBy8bit( streamTypeInfo, 1 ) == 1;
        }

        SoundArchive::DecodeMode GetDecodeMode() const NN_NOEXCEPT
        {
            return static_cast<SoundArchive::DecodeMode>( Util::DevideBy8bit( streamTypeInfo, 2 ) );
        }
    };

    struct StreamSoundInfo
    {
        // データ
        uint16_t allocateTrackFlags;  // 有効トラックを表すビットフラグ (マルチトラックの設定)
        uint16_t allocateChannelCount;// マルチトラックを加味した総チャンネル数
        Util::Reference toTrackInfoTable;

        float pitch;
        Util::Reference toSendValue;

        Util::Reference toStreamSoundExtension;

        uint32_t prefetchFileId;

        // トラック情報テーブル取得
        const StreamTrackInfoTable* GetTrackInfoTable() const NN_NOEXCEPT;
        // アクセサ
        float GetPitch() const NN_NOEXCEPT;
        const SendValue& GetSendValue() const NN_NOEXCEPT;
        const StreamSoundExtension*  GetStreamSoundExtension() const NN_NOEXCEPT;
    };

    struct WaveSoundInfo
    {
        // データ
        uint32_t index;           // ウェーブサウンドセットファイル (bxwsd) 内で何番目か
        uint32_t allocateTrackCount; // 必要なトラック数
        Util::BitFlag optionParameter;

        // アクセサ
        uint8_t GetChannelPriority() const NN_NOEXCEPT;
        uint8_t GetIsReleasePriorityFix() const NN_NOEXCEPT;

    };

    struct AdvancedWaveSoundInfo
    {
        uint32_t waveArchiveId;
    };

    struct SequenceSoundInfo
    {
        // データ
        Util::Reference toBankIdTable;
        uint32_t      allocateTrackFlags;
        Util::BitFlag   optionParameter;

        // アクセサ
        void GetBankIds( uint32_t* bankIds ) const NN_NOEXCEPT;  // SoundArchive::SequenceBankMax だけコピー
        uint32_t GetStartOffset() const NN_NOEXCEPT;
        uint8_t  GetChannelPriority() const NN_NOEXCEPT;
        bool IsReleasePriorityFix() const NN_NOEXCEPT;
    private:
        const Util::Table<uint32_t>& GetBankIdTable() const NN_NOEXCEPT;
    };

    struct Sound3DInfo
    {
        uint32_t flags;       // [0]Vol [1]Priority [2]Pan [3]SPan [4]Filter
                                // (SoundArchive::Sound3DInfo::FlagControl_*** に対応しています)
        float decayRatio;  // 減衰率
        uint8_t decayCurve;          // 減衰カーブタイプ (nn::atk::DecayCurve が入る)
        uint8_t dopplerFactor;       // ドップラーファクター
        uint8_t padding[2];
        Util::BitFlag optionParameter;      // 予備
    };

    struct BankInfo
    {
        // データ
        uint32_t      fileId;
        Util::Reference toWaveArchiveItemIdTable;
        Util::BitFlag   optionParameter;

        // アクセサ
        uint32_t GetStringId() const NN_NOEXCEPT;

        inline const Util::Table<uint32_t>*
        GetWaveArchiveItemIdTable() const NN_NOEXCEPT
        {
            return reinterpret_cast<const Util::Table<uint32_t>*>(
                    util::ConstBytePtr( this, toWaveArchiveItemIdTable.offset ).Get() );
        }
    };

    struct PlayerInfo
    {
        // データ
        uint32_t      playableSoundMax;   // プレイヤー最大再生数
        Util::BitFlag   optionParameter;

        // アクセサ
        uint32_t GetStringId() const NN_NOEXCEPT;
        uint32_t GetPlayerHeapSize() const NN_NOEXCEPT;
    };

    struct WaveSoundGroupInfo;
    struct SoundGroupInfo
    {
        // データ
        uint32_t      startId;    // アイテム ID (startId と endId の ItemType は同一)
        uint32_t      endId;
        Util::Reference toFileIdTable;
        Util::Reference toDetailSoundGroupInfo;
        Util::BitFlag   optionParameter;

        // アクセサ
        uint32_t GetStringId() const NN_NOEXCEPT;

        inline const Util::Table<uint32_t>* GetFileIdTable() const NN_NOEXCEPT
        {
            return reinterpret_cast<const Util::Table<uint32_t>*>(
                    util::ConstBytePtr( this, toFileIdTable.offset ).Get() );
        }
        inline const WaveSoundGroupInfo* GetWaveSoundGroupInfo() const NN_NOEXCEPT
        {
            if ( toDetailSoundGroupInfo.typeId !=
                    ElementType_SoundArchiveFile_WaveSoundGroupInfo )
            {
                return NULL;
            }

            return reinterpret_cast<const WaveSoundGroupInfo*>(
                    util::ConstBytePtr( this, toDetailSoundGroupInfo.offset ).Get() );
        }

#if 0   // 必要になるかもしれない
        uint32_t GetFileIdCount() const NN_NOEXCEPT { return GetFileIdTable().count; }
        uint32_t GetFileId( uint32_t index ) const NN_NOEXCEPT
        {
            if ( index >= GetFileIdCount() ) return SoundArchive::InvalidId;
            return GetFileIdTable().item[ index ];
        }
#endif
    };

    struct WaveSoundGroupInfo
    {
        // データ
        Util::Reference toWaveArchiveItemIdTable;
        Util::BitFlag   optionParameter;    // 仮。なにが割り当てられるかは未定。

        // アクセサ
        inline const Util::Table<uint32_t>*
        GetWaveArchiveItemIdTable() const NN_NOEXCEPT
        {
            return reinterpret_cast<const Util::Table<uint32_t>*>(
                    util::ConstBytePtr( this, toWaveArchiveItemIdTable.offset ).Get() );
        }
    };

    struct GroupInfo
    {
        // データ
        uint32_t      fileId;
        Util::BitFlag   optionParameter;

        // アクセサ
        uint32_t GetStringId() const NN_NOEXCEPT;
    };

    struct WaveArchiveInfo
    {
        // データ
        uint32_t      fileId;
        bool          isLoadIndividual;
        uint8_t       padding[3];
        Util::BitFlag   optionParameter;

        // アクセサ
        uint32_t GetStringId() const NN_NOEXCEPT;
        uint32_t GetWaveCount() const NN_NOEXCEPT;
    };

    enum FileLocationType
    {
        FileLocationType_Internal,    // サウンドアーカイブ内に配置
        FileLocationType_External,    // サウンドアーカイブ外に配置
        FileLocationType_None
            // ファイル単体にアクセスできない (＝ロードできない)
            //  * バンクファイルかウェーブサウンドセットファイルのうち、
            //    次の条件を満たすものが該当
            //      * 波形アーカイブ設定が【自動(共有)】で、
            //      * 出力設定が【埋め込み】のグループに属している
    };

    struct InternalFileInfo;    // サウンドアーカイブ FILE ブロック内に格納
    struct ExternalFileInfo;    // サウンドアーカイブ外に格納

    struct FileInfo
    {
        // データ
        Util::Reference toFileLocation;
        Util::BitFlag   optionParameter;    // 仮。なにが割り当てられるかは未定。

        // アクセサ
        FileLocationType GetFileLocationType() const NN_NOEXCEPT;
        const InternalFileInfo* GetInternalFileInfo() const NN_NOEXCEPT;
        const ExternalFileInfo* GetExternalFileInfo() const NN_NOEXCEPT;

#if 0
        // サウンドアーカイブファイル上のアドレスを取得
        // ROM 上のバイナリサウンドアーカイブファイルの先頭をアドレス 0 とする。
        const void* GetFileAddress( const void* dataBlockBodyAddress ) const NN_NOEXCEPT;
            // TODO: ↑ これは必要ない？
#endif
    };

    struct InternalFileInfo
    {
        static const uint32_t InvalidOffset = BinaryTypes::InvalidOffset;
        static const uint32_t InvalidSize = BinaryTypes::InvalidSize;

        // データ
        Util::ReferenceWithSize toFileImageFromFileBlockBody;
        Util::Reference         toAttachedGroupIdTable; // 所属するグループIDのテーブル

        // アクセサ
        inline uint32_t GetFileSize() const NN_NOEXCEPT
        {
            return toFileImageFromFileBlockBody.size;
        }
        inline uint32_t GetOffsetFromFileBlockHead() const NN_NOEXCEPT
        {
            if(toFileImageFromFileBlockBody.offset == InvalidOffset)
            {
                return InvalidOffset;
            }
            return toFileImageFromFileBlockBody.offset + sizeof(BinaryBlockHeader);
        }
        inline const Util::Table<uint32_t>* GetAttachedGroupTable() const NN_NOEXCEPT
        {
            // どのグループに属しているかを示すテーブルを返す
            // (どこにも属していない場合は、count = 0 のテーブルが返される)
            return reinterpret_cast<const Util::Table<uint32_t>*>(
                    util::ConstBytePtr( this, toAttachedGroupIdTable.offset ).Get() );
        }
    };

    struct ExternalFileInfo
    {
        // データ
        // TODO: uint32_t fileSize や uint16_t? filePathLength が入るかも
        char filePath[1];
    };

    struct SoundArchivePlayerInfo
    {
        uint16_t  sequenceSoundCount;
        uint16_t  sequenceTrackCount;
        uint16_t  streamSoundCount;
        uint16_t  streamTrackCount;
        uint16_t  streamChannelCount;
        uint16_t  waveSoundCount;
        uint16_t  waveTrackCount;
        uint8_t   streamBufferTimes;
        uint8_t   developFlags;   // 開発用フラグ
        uint32_t  options;        // TODO: 仮
    };


    // ------------------------------------------------------------------------
    //  ファイルブロック

    struct FileBlockBody
    {
        // ボディのアドレスを取るためだけ
    };

    struct FileBlock
    {
        BinaryBlockHeader header;
        FileBlockBody body;
    };

};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
