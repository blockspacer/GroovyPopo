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
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_CurveAdshr.h>

namespace nn {
namespace atk {
namespace detail {

/*
    ウェーブサウンドファイル (.bfwsd) の構造

    「ウェーブサウンド」ファイルと呼んでいるが、実際には、
    「ウェーブサウンドセット」１つ分を格納するファイルとなっている。

    bfwsd
     |
     +-- FileHeader
     +-- InfoBlock
          |
          +-- BinaryBlockHeader
          +-- InfoBlockBody
               |
               +-- Table<ref to WaveSoundData>
                    |
                    +-- uint32_t count
                    +-- ref to WaveSoundData[0] --+ ← このインデックスが、ウェー
                    +-- ref to WaveSoundData[1]   |    ブサウンドセット内でのイン
                    +-- ...                       |    デックスに相当する。
     +--------------------------------------------+
     |
     +-> WaveSoundData
          |
          +-- ref to WaveSoundInfo
          |    |
          |    +--> RawWaveSoundInfo
          |          |
          |          +-- optionParameter (パン、ピッチ、センド量、ADSR)
          |
          +-- ref to TrackInfoTable  <-- 各トラックの楽譜情報が入る
          |    |
          |    +--> Table<ref to TrackInfo>
          |          |
          |          +-- uint32_t count
          |          +-- ref to TrackInfo[0] --+
          |          +-- ref to TrackInfo[1]   |
          |          +-- ...                   |
          |    +-------------------------------+
          |    |
          |    +--> TrackInfo
          |          |
          |          +-- ref to Table<ref to NoteEvent>
          |               |
          |               +--> Table<ref to NoteEvent>
          |                     |
          |                     +-- uint32_t count
          |                     +-- ref to NoteEvent[0] --+
          |                     +-- ref to NoteEvent[1]   |
          |                     +-- ...                   |
          |   +-------------------------------------------+
          |   |
          |   +--> NoteEvent
          |         |
          |         +-- position
          |         +-- length
          |         +-- noteIndex    <-- NoteInfoTable のノートのインデックス
          |         +-- reserved
          |
          +-- ref to NoteInfoTable   <-- トラックで使われる全ノートの
               |                               情報が入る
               +--> Table<ref to NoteInfo>
                     |
                     +-- uint32_t count
                     +-- ref to NoteInfo[0] --+
                     +-- ref to NoteInfo[1]   |
                     +-- ...                     |
              +----------------------------------+
              |
              +--> NoteInfo
                    |
                    +-- waveArchiveId
                    +-- waveIndex
                    +-- optionParameter
                         |
                         +-- originalKey
                         +-- volume
                         +-- pan
                         +-- (surroundPan)
                         +-- pitch
                         +-- sendValue
                         +-- adshrCurve

*/

struct WaveSoundFile
{
    //
    // ヘッダー
    //
    struct InfoBlock;
    struct FileHeader : public Util::SoundFileHeader
    {
        // アクセサ
        const InfoBlock* GetInfoBlock() const NN_NOEXCEPT;
    };

    //
    // INFO ブロック
    //

    struct WaveSoundData;

    struct InfoBlockBody
    {
        // データ
        Util::Reference toWaveIdTable;
        Util::Reference toWaveSoundDataReferenceTable;

        // アクセサ
        // (テーブル取得)
        const Util::WaveIdTable& GetWaveIdTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetWaveSoundDataReferenceTable() const NN_NOEXCEPT;

        // (テーブルアイテム数取得)
        inline uint32_t GetWaveIdCount() const NN_NOEXCEPT
        {
            return GetWaveIdTable().GetCount();
        }
        inline uint32_t GetWaveSoundCount() const NN_NOEXCEPT
        {
            return GetWaveSoundDataReferenceTable().count;
        }

        // (テーブルアイテム取得)
        inline const Util::WaveId* GetWaveId( uint32_t index ) const NN_NOEXCEPT
        {
            return GetWaveIdTable().GetWaveId( index );
        }
        const WaveSoundData& GetWaveSoundData( uint32_t index ) const NN_NOEXCEPT;
    };

    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };


    struct WaveSoundInfo;
    struct TrackInfo;
    struct NoteInfo;

    // ウェーブサウンド 1 つ分のデータ
    struct WaveSoundData
    {
        // データ
        Util::Reference toWaveSoundInfo;
        Util::Reference toTrackInfoReferenceTable;
        Util::Reference toNoteInfoReferenceTable;

        // アクセサ
        const WaveSoundInfo& GetWaveSoundInfo() const NN_NOEXCEPT;

        // (テーブル取得)
        const Util::ReferenceTable& GetTrackInfoReferenceTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetNoteInfoReferenceTable() const NN_NOEXCEPT;

        // (テーブルアイテム数取得)
        inline uint32_t GetTrackCount() const NN_NOEXCEPT
        {
            return GetTrackInfoReferenceTable().count;
        }
        inline uint32_t GetNoteCount() const NN_NOEXCEPT
        {
            return GetNoteInfoReferenceTable().count;
        }

        // (テーブルアイテム取得)
        const TrackInfo& GetTrackInfo( uint32_t index ) const NN_NOEXCEPT;
        const NoteInfo& GetNoteInfo( uint32_t index ) const NN_NOEXCEPT;
    };

    struct WaveSoundInfo
    {
        // データ
        Util::BitFlag optionParameter;

        // アクセサ
        uint8_t GetPan() const NN_NOEXCEPT;
        int8_t GetSurroundPan() const NN_NOEXCEPT;
        float GetPitch() const NN_NOEXCEPT;
        void GetSendValue( uint8_t* mainSend, uint8_t* fxSend, uint8_t fxSendCount ) const NN_NOEXCEPT;
        const AdshrCurve& GetAdshrCurve() const NN_NOEXCEPT;
        uint8_t GetLpfFreq() const NN_NOEXCEPT;
        uint8_t GetBiquadType() const NN_NOEXCEPT;
        uint8_t GetBiquadValue() const NN_NOEXCEPT;
    };

    struct NoteEvent;

    struct TrackInfo
    {
        // データ
        Util::Reference toNoteEventReferenceTable;

        // アクセサ
        // (テーブル取得)
        const Util::ReferenceTable& GetNoteEventReferenceTable() const NN_NOEXCEPT;

        // (テーブルアイテム数取得)
        inline uint32_t GetNoteEventCount() const NN_NOEXCEPT
        {
            return GetNoteEventReferenceTable().count;
        }

        // (テーブルアイテム取得)
        const NoteEvent& GetNoteEvent( uint32_t index ) const NN_NOEXCEPT;
    };

    struct NoteEvent
    {
        float  position;
        float  length;
        uint32_t  noteIndex;
        uint32_t  reserved;
    };

    struct NoteInfo
    {
        // データ
        uint32_t  waveIdTableIndex;
        Util::BitFlag optionParameter;

        // アクセサ
        uint8_t GetOriginalKey() const NN_NOEXCEPT;
        uint8_t GetVolume() const NN_NOEXCEPT;
        uint8_t GetPan() const NN_NOEXCEPT;
        uint8_t GetSurroundPan() const NN_NOEXCEPT;
        float GetPitch() const NN_NOEXCEPT;
        void GetSendValue( uint8_t* mainSend, uint8_t* fxSend[], uint8_t fxSendCount ) const NN_NOEXCEPT;
        const AdshrCurve& GetAdshrCurve() const NN_NOEXCEPT;
    };
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
