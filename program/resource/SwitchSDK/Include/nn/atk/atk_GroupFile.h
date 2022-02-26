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

namespace nn {
namespace atk {
namespace detail {

struct GroupFile
{
    struct InfoBlock;
    struct FileBlock;
    struct InfoExBlock;
    struct InfoBlockBody;
    struct FileBlockBody;
    struct InfoExBlockBody;
    struct GroupItemInfo;
    struct GroupItemInfoEx;

    struct FileHeader : public Util::SoundFileHeader
    {
        const InfoBlock* GetInfoBlock() const NN_NOEXCEPT;  // ファイル ID をキーとしたテーブルを含む
        const FileBlock* GetFileBlock() const NN_NOEXCEPT;
        const InfoExBlock* GetInfoExBlock() const NN_NOEXCEPT;  // アイテム ID をキーとしたテーブルを含む
    };

    struct InfoBlockBody
    {
        // TODO: リファレンスがひとつ多いと思う。
        //       GroupItemInfo 自体が固定長なので、
        //       ここに直接 GroupItemInfo テーブルを埋めればいいのでは？

        // データ
        Util::ReferenceTable referenceTableOfGroupItemInfo;

        // アクセサ
        inline uint32_t GetGroupItemInfoCount() const NN_NOEXCEPT
        {
            return referenceTableOfGroupItemInfo.count;
        }
        const GroupItemInfo* GetGroupItemInfo( uint32_t index ) const NN_NOEXCEPT
        {
            if ( index >= GetGroupItemInfoCount() )
            {
                return NULL;
            }
            return static_cast<const GroupItemInfo*>( util::ConstBytePtr(
                        this, referenceTableOfGroupItemInfo.item[ index ].offset ).Get() );
        }
#if 0
        // データ
        Util::Table<GroupItemInfo> table;

        // アクセサ
        inline uint32_t GetItemCount() const NN_NOEXCEPT { return table.count; }
        const GroupItemInfo* GetItemInfo( uint32_t index ) const NN_NOEXCEPT
        {
            if ( index > GetItemCount() )
            {
                return NULL;
            }
            return &table.item[ index ];
        }
#endif
    };

    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct GroupItemInfo
    {
        // データ
        uint32_t fileId;
        Util::ReferenceWithSize embeddedItemInfo;
            // 埋め込まれたアイテムの場所やサイズを表現します。
            // [場所] ＝ [offset の起点] は、FileBlockBody です。

        static const uint32_t OffsetForLink    = 0xffffffff;
        static const uint32_t SizeForLink      = 0xffffffff;

        // アクセサ
        const void* GetFileLocation( const FileBlockBody* fileBlockBody ) const NN_NOEXCEPT
        {
            // ファイルブロック (ボディ？) のアドレスに、
            // offset 値を足して返す。
            if ( embeddedItemInfo.offset == OffsetForLink )
            {
                return NULL;
            }
            return util::ConstBytePtr( fileBlockBody,
                    embeddedItemInfo.offset ).Get();
        }
    };

    struct FileBlockBody
    {
        // プレースホルダ
    };

    struct FileBlock
    {
        BinaryBlockHeader header;
        FileBlockBody body;
    };


    struct InfoExBlockBody
    {
        // データ
        Util::ReferenceTable referenceTableOfGroupItemInfoEx;

        // アクセサ
        inline uint32_t GetGroupItemInfoExCount() const NN_NOEXCEPT
        {
            return referenceTableOfGroupItemInfoEx.count;
        }
        const GroupItemInfoEx* GetGroupItemInfoEx( uint32_t index ) const NN_NOEXCEPT
        {
            if ( index >= GetGroupItemInfoExCount() )
            {
                return NULL;
            }
            return static_cast<const GroupItemInfoEx*>( util::ConstBytePtr(
                        this, referenceTableOfGroupItemInfoEx.item[ index ].offset ).Get() );
        }
    };

    struct InfoExBlock
    {
        BinaryBlockHeader header;
        InfoExBlockBody body;
    };

    struct GroupItemInfoEx
    {
        // データ
        uint32_t itemId;
        uint32_t loadFlag;  // SoundArchiveLoader::LoadFlag の論理和が入る
    };
};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

