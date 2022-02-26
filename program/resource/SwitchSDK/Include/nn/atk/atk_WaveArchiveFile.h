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
#include <nn/atk/detail/atk_BinaryTypes.h>

namespace nn {
namespace atk {
namespace detail {

//
// MEMO: WaveArchiveFile (.bxwar ＝ 波形アーカイブファイル)
//  * ヘッダ、INFO ブロックは最初に読む
//  * FILE ブロックをまるごと読むこともある
//  * FILE ブロックのうち、必要な波形ファイルのみ読むことも鳴る
//

struct WaveArchiveFile
{
    //
    // ヘッダー
    //
    struct InfoBlock;
    struct FileBlock;

    static const int BlockCount = 2;

    // メモ : WaveArchiveFile::FileHeader はヘッダだけ個別にロードするので、
    //        Util::SoundFileHeader を継承できない。
    struct FileHeader : public nn::atk::detail::BinaryFileHeader
    {
        // データ
        Util::ReferenceWithSize toBlocks[ BlockCount ];

        // アクセサ (一括ロードの場合に有効)
        const InfoBlock* GetInfoBlock() const NN_NOEXCEPT;
        const FileBlock* GetFileBlock() const NN_NOEXCEPT;

        // アクセサ (個別ロードのときに利用される)
        uint32_t GetInfoBlockSize() const NN_NOEXCEPT;
        uint32_t GetFileBlockSize() const NN_NOEXCEPT;
        uint32_t GetInfoBlockOffset() const NN_NOEXCEPT;
        uint32_t GetFileBlockOffset() const NN_NOEXCEPT;

      private:
        const Util::ReferenceWithSize* GetReferenceBy( uint16_t typeId ) const NN_NOEXCEPT;
    };

    //
    // INFO ブロック
    //
    struct InfoBlockBody
    {
        // データ
        Util::Table<Util::ReferenceWithSize> table;

        // アクセサ
        inline uint32_t GetWaveFileCount() const NN_NOEXCEPT { return table.count; }
        uint32_t GetSize( uint32_t index ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( index < GetWaveFileCount() );
            return table.item[ index ].size;
        }
        uint32_t GetOffsetFromFileBlockBody( uint32_t index ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( index < GetWaveFileCount() );
            return table.item[ index ].offset;
        }

        static const uint32_t InvalidOffset = BinaryTypes::InvalidOffset;

    };
    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    //
    // FILE ブロック
    //
    struct FileBlockBody
    {
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

