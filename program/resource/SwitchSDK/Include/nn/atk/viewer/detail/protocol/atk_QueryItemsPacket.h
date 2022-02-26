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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/viewer/detail/hio/atk_HioProtocol.h>
#include <nn/atk/viewer/detail/res/atk_ResTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @internal
//! @brief  アイテム一覧を問い合わせるパケットです。
//---------------------------------------------------------------------------
class QueryItemsPacket : public HioPacket
{
public:
    class Body : public HioPacketBody
    {
    };

public:
    QueryItemsPacket() NN_NOEXCEPT : HioPacket(HioMessageType_QueryItems, 0)
    {
    }

public:
    static size_t GetRequiredSize() NN_NOEXCEPT
    {
        return sizeof(QueryItemsPacket);
    }

    Body&       GetBody() NN_NOEXCEPT { return m_Body; }
    const Body& GetBody() const NN_NOEXCEPT { return m_Body; }

private:
    Body m_Body;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  QueryItemsPacket の返信パケットです。
//---------------------------------------------------------------------------
class QueryItemsReplyPacket : public HioPacket
{
private:
    // HACK : ResOffsetTable のアイテム数は、とりあえず最大256個とします。
    static const uint32_t MaxItemCount = 256;

public:
    enum ActionType
    {
        ActionType_None = 0,
        ActionType_UpdateItem = 1,
        ActionType_RemoveItem = 2
    };

    //! @brief  アイテム情報を格納します。
    struct ItemInfo
    {
        atk::detail::fnd::BinU32 action;
        ResName                    name;

        static size_t GetRequiredSize(int maxItemName) NN_NOEXCEPT
        {
            return nn::util::align_up(
                sizeof(atk::detail::fnd::BinU32) + ResName::GetRequiredSize(maxItemName),
                atk::detail::fnd::MemoryTraits::DefaultAlignment
            );
        }
    };

    struct Body : public HioPacketBody
    {
        Body() NN_NOEXCEPT : actionForAllItems(ActionType_None) { }

        atk::detail::fnd::BinU32 actionForAllItems;
        ResOffsetTable<ItemInfo> items;
    };

public:
    QueryItemsReplyPacket() NN_NOEXCEPT : HioPacket(HioMessageType_QueryItemInfoReply, sizeof(Body))
    {
    }

public:
    static size_t GetRequiredSize(int maxItemName) NN_NOEXCEPT
    {
        // ItemInfo は逐次読み込みするので、１つ分のメモリ領域で計算します。
        return nn::util::align_up(
            sizeof(QueryItemsReplyPacket) +
            sizeof(Offset) * MaxItemCount +
            ItemInfo::GetRequiredSize(maxItemName),
            atk::detail::fnd::MemoryTraits::DefaultAlignment);
    }

    Body&       GetBody() NN_NOEXCEPT { return m_Body; }
    const Body& GetBody() const NN_NOEXCEPT { return m_Body; }

private:
    Body m_Body;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
