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

namespace nn {
namespace atk {
namespace detail {

/*
    バンクファイル (.bfbnk) の構造

    bfbnk
     |
     +-- FileHeader
     +-- InfoBlock
          |
          +-- BinaryBlockHeader
          +-- InfoBlockBody
               |
               +-- reference to Table of InnerWave
               |    |
               |    +-> Table<InnerWave>
               |         |
               |         +-- int32_t count
               |         +-- InnerWave[0]
               |         +-- InnerWave[1]
               |         +-- ...
               |
               +-- reference to Table of [reference to Instrument]
                    |
                    +-> Table<reference to Instrument>
                         |
                         +-- int32_t count
                         +-- Reference to Instrument[0] --+ ← このインデックスが
                         +-- Reference to Instrument[1]   |    prg No として扱われる。
                         +-- ...                          |
    +-----------------------------------------------------+
    |
    +-> Instrument
         |
         +-- reference to KeyRegionChunk
              |
             typeId ?
              |
              +-- Direct --> DirectChunk ← キーリージョンの分割数によって
              +-- Range  --> RangeChunk     格納されるデータ構造が異なる
              +-- Index  --> IndexChunk
                              |
                              +-> KeyRegion を取得する
                                   |
    +------------------------------+
    |
    +-- KeyRegion
         |
         +-- reference to VelocityRegionChunk
              |
             typeId ?
              +-- Direct --> DirectChunk
              +-- Range  --> RangeChunk
              +-- Index  --> IndexChunk
                              |
                              +-> VelocityRegion を取得する
*/

struct BankFile
{
    //-----------------------------------------------------------------
    // ファイルヘッダー

    struct InfoBlock;

    struct FileHeader : public Util::SoundFileHeader
    {
        const InfoBlock* GetInfoBlock() const NN_NOEXCEPT;
    };

    //-----------------------------------------------------------------
    // 情報ブロック

    struct Instrument;

    struct InfoBlockBody
    {
        // データ
        Util::Reference toWaveIdTable;              // "バンク内波形 ID" テーブル
        Util::Reference toInstrumentReferenceTable; // "インストへの参照" テーブル

        // "バンク内波形 ID" テーブルアクセサ

        // アクセサ
        // (テーブル取得)
        const Util::WaveIdTable& GetWaveIdTable() const NN_NOEXCEPT;
        const Util::ReferenceTable& GetInstrumentReferenceTable() const NN_NOEXCEPT;

        // (テーブルアイテム数取得)
        inline uint32_t GetWaveIdCount() const NN_NOEXCEPT
        {
            return GetWaveIdTable().GetCount();
        }
        inline int32_t GetInstrumentCount() const NN_NOEXCEPT
        {
            return GetInstrumentReferenceTable().count;
        }

        // (テーブルアイテム取得)
        inline const Util::WaveId* GetWaveId( uint32_t index ) const NN_NOEXCEPT
        {
            return GetWaveIdTable().GetWaveId( index );
        }
        const Instrument* GetInstrument( int programNo ) const NN_NOEXCEPT;
    };

    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };


    struct KeyRegion;
    struct Instrument
    {
        // データ
        Util::Reference toKeyRegionChunk;

        // アクセサ
        const KeyRegion* GetKeyRegion( uint32_t key ) const NN_NOEXCEPT;
    };

    struct VelocityRegion;
    struct KeyRegion
    {
        // データ
        Util::Reference toVelocityRegionChunk;

        // アクセサ
        const VelocityRegion* GetVelocityRegion( uint32_t velocity ) const NN_NOEXCEPT;
    };

    struct RegionParameter;
    struct VelocityRegion
    {
        // データ
        uint32_t  waveIdTableIndex;
        Util::BitFlag   optionParameter;

        // アクセサ
        uint8_t GetOriginalKey() const NN_NOEXCEPT;
        uint8_t GetVolume() const NN_NOEXCEPT;
        uint8_t GetPan() const NN_NOEXCEPT;
        float GetPitch() const NN_NOEXCEPT;
        bool IsIgnoreNoteOff() const NN_NOEXCEPT;
        uint8_t GetKeyGroup() const NN_NOEXCEPT;
        uint8_t GetInterpolationType() const NN_NOEXCEPT;
        const AdshrCurve& GetAdshrCurve() const NN_NOEXCEPT;

        const RegionParameter* GetRegionParameter() const NN_NOEXCEPT;
    };

#if 1
    // VelocityRegion::optionParameter に格納されている値
    struct RegionParameter
    {
        // --------------------------------------------
        // VelocityRegionBitFlag_Key
        uint8_t originalKey;
        uint8_t padding1[3];
        // --------------------------------------------
        // VelocityRegionBitFlag_Volume
        uint8_t volume;
        uint8_t padding2[3];
        // --------------------------------------------
        // VelocityRegionBitFlag_Pan
        uint8_t pan;
        int8_t surroundPan;
        uint8_t padding3[2];
        // --------------------------------------------
        // VelocityRegionBitFlag_Pitch
        float pitch;
        // --------------------------------------------
        // VelocityRegionBitFlag_InstrumentNoteParam
        bool isIgnoreNoteOff;
        uint8_t keyGroup;
        uint8_t interpolationType;
        uint8_t padding4[1];
        // --------------------------------------------
        // VelocityRegionBitFlag_Envelope
        uint32_t offset;                         // ignore
        Util::Reference refToAdshrCurve;    // ignore
        AdshrCurve adshrCurve;
    };
#else
    // HACK : BigEndian の仮対応です。
    // VelocityRegion::optionParameter に格納されている値
    struct RegionParameter
    {
        // --------------------------------------------
        // VelocityRegionBitFlag_Key
        uint8_t padding1[3];
        uint8_t originalKey;
        // --------------------------------------------
        // VelocityRegionBitFlag_Volume
        uint8_t padding2[3];
        uint8_t volume;
        // --------------------------------------------
        // VelocityRegionBitFlag_Pan
        uint8_t padding3[2];
        int8_t surroundPan;
        uint8_t pan;
        // --------------------------------------------
        // VelocityRegionBitFlag_Pitch
        float pitch;
        // --------------------------------------------
        // VelocityRegionBitFlag_InstrumentNoteParam
        uint8_t padding4[1];
        uint8_t interpolationType;
        uint8_t keyGroup;
        bool isIgnoreNoteOff;
        // --------------------------------------------
        // VelocityRegionBitFlag_Envelope
        uint32_t offset;                         // ignore
        Util::Reference refToAdshrCurve;    // ignore
        AdshrCurve adshrCurve;
    };
#endif
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

