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

namespace nn {
namespace atk {
namespace detail {

struct SequenceSoundFile
{
    //-----------------------------------------------------------------
    // ファイルヘッダー

    struct DataBlock;
    struct LabelBlock;

    struct FileHeader : public Util::SoundFileHeader
    {
        const DataBlock* GetDataBlock() const NN_NOEXCEPT;
        const LabelBlock* GetLabelBlock() const NN_NOEXCEPT;
    };

    //-----------------------------------------------------------------
    // データブロック

    struct DataBlockBody
    {
        uint8_t sequenceData[1];
        const void* GetSequenceData() const NN_NOEXCEPT { return reinterpret_cast<const void*>(sequenceData); }
    };

    struct DataBlock
    {
        BinaryBlockHeader header;
        DataBlockBody body;
    };

    //-----------------------------------------------------------------
    // ラベルブロック

    struct LabelInfo;
    struct LabelBlockBody
    {
        // データ
        Util::ReferenceTable labelInfoReferenceTable;

        // アクセサ
        inline int GetLabelCount() const NN_NOEXCEPT { return labelInfoReferenceTable.count; }
        const LabelInfo* GetLabelInfo( int index ) const NN_NOEXCEPT;
        const char* GetLabel( int index ) const NN_NOEXCEPT;
        const char* GetLabelByOffset( uint32_t offset ) const NN_NOEXCEPT;
        bool GetOffset( int index, uint32_t* offsetPtr ) const NN_NOEXCEPT;
        bool GetOffsetByLabel( const char* label, uint32_t* offsetPtr ) const NN_NOEXCEPT;
    };

    struct LabelBlock
    {
        BinaryBlockHeader header;
        LabelBlockBody body;           // ラベルデータ本体
    };

    struct LabelInfo
    {
        Util::Reference referToSequenceData;    // DataBlockBody を基点とするリファレンス
        uint32_t labelStringLength;           // ラベル文字列の長さ
        char label[1];
    };

};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

