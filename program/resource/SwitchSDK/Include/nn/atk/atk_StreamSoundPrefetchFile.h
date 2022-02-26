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
#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/atk_BinaryFileFormat.h>

namespace nn {
namespace atk {
namespace detail {

/*
    ストリームプリフェッチファイル (*.bfstp) の構造

    bfstp
     |
     +-- FileHeader
     +-- InfoBlock // bfstm の InfoBlock と同じ
     |
     +-- PrefetchDataBlock
          |
          +-- BinaryBlockHeader header
          +-- PrefetchDataBlockBody
              |
              +-- Table<PrefetchData>------------+
                                                 |
              +----------------------------------+
              |
              +-- uint32_t count ---------------------+
              +-- PrefetchData item[0] ----------+
              +-- PrefetchData item[1] ----------+
                          :                      |
              +----------------------------------+
              |
              +-> PrefetchData
              |
              +-- uint32_t startFrame
              +-- uint32_t prefetchSize // PrefetchSample の data のバイト数
              +-- uint32_t reserved[1]
              +-- reference to PrefetchSample --+
                                                 |
              +----------------------------------+
              |
              +-> PrefetchSample
              +-- uint8_t  data[...] // (サンプルデータ)

*/

struct StreamSoundPrefetchFile
{
    // 前方宣言
    struct PrefetchDataBlock;

    struct FileHeader : public Util::SoundFileHeader
    {
        const StreamSoundFile::InfoBlock* GetInfoBlock() const NN_NOEXCEPT;
        const StreamSoundFile::RegionBlock* GetRegionBlock() const NN_NOEXCEPT;
        const PrefetchDataBlock*          GetPrefetchDataBlock() const NN_NOEXCEPT;
        uint32_t                               GetPrefetchDataBlockSize() const NN_NOEXCEPT;

        bool HasRegionBlock() const NN_NOEXCEPT;
        uint32_t GetRegionBlockSize() const NN_NOEXCEPT;
        uint32_t GetRegionBlockOffset() const NN_NOEXCEPT;
    };

    // --------------------------
    // PREFETCH DATA ブロック (サンプルデータ)
    struct PrefetchSample;

    struct PrefetchData
    {
        // データ
        uint32_t startFrame;
        uint32_t prefetchSize;
        uint32_t reserved[1];
        Util::Reference toPrefetchSample;

        // アクセサ
        const PrefetchSample* GetPrefetchSample() const NN_NOEXCEPT;
    };

    struct PrefetchDataBlockBody
    {
        // データ
        Util::Table<PrefetchData> prefetchDataTable;

        // アクセサ
        inline uint32_t GetPrefetchDataCount() const NN_NOEXCEPT { return prefetchDataTable.count; }
        inline const PrefetchData* GetPrefetchData( uint32_t index ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( index < prefetchDataTable.count );
            return &prefetchDataTable.item[ index ];
        }
    };

    struct PrefetchDataBlock
    {
        BinaryBlockHeader header;
        PrefetchDataBlockBody body;
    };

    struct PrefetchSample
    {
        // データ
        uint8_t  data[1];

        // アクセサ
        const void* GetSampleAddress() const NN_NOEXCEPT;
    };
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

