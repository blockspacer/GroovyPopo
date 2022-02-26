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

#include <nn/util/util_StringUtil.h>
#include <nn/atk/viewer/detail/hio/atk_HioProtocol.h>
#include <nn/atk/viewer/detail/res/atk_ResItemInfo.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @internal
//! @brief  アイテム情報を取得するパケットです。
//---------------------------------------------------------------------------
class QueryItemInfoPacket : public HioPacket
{
public:
    class Body : public HioPacketBody
    {
    public:
        Body(const char* itemName, int itemNameLength) NN_NOEXCEPT
        {
           m_ItemName.len = itemNameLength + 1;

           if(itemName != NULL && itemNameLength > 0)
           {
               SetItemName(itemName, itemNameLength);
           }
        }

    public:
        static size_t GetRequiredSize(int maxItemName) NN_NOEXCEPT
        {
            return ResName::GetRequiredSize(maxItemName);
        }

        int GetItemNameLength() const NN_NOEXCEPT { return m_ItemName.len; }
        const char* GetItemName()       const NN_NOEXCEPT { return m_ItemName.str; }

    private:
        void SetItemName(const char* value, int length) NN_NOEXCEPT
        {
            if(m_ItemName.len == 0)
            {
                return;
            }

            if(value == NULL || *value == '\0')
            {
                m_ItemName.str[0] = '\0';
            }

            util::Strlcpy(m_ItemName.str, value, (std::min)(static_cast<int>(m_ItemName.len) + 1, length + 1));
        }

    private:
        ResNameData m_ItemName;
    };

private:
    QueryItemInfoPacket(const char* itemName, int maxItemName) NN_NOEXCEPT :
       HioPacket(HioMessageType_QueryItemInfo, static_cast<uint32_t>(Body::GetRequiredSize(maxItemName))),
        m_Body(itemName, maxItemName)
    {
    }

public:
    static QueryItemInfoPacket* CreateInstance(void* buffer, const char* itemName) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(buffer);

        int itemNameLength = itemName == NULL ? 0 : static_cast<int>(std::strlen(itemName));
        return new(buffer) QueryItemInfoPacket(itemName, itemNameLength);
    }

    static size_t GetRequiredSize(int maxItemName) NN_NOEXCEPT
    {
        // 固定パケットサイズ + 名前の長さ
        return sizeof(HioPacket) + Body::GetRequiredSize(maxItemName);
    }

    Body&       GetBody() NN_NOEXCEPT { return m_Body; }
    const Body& GetBody() const NN_NOEXCEPT { return m_Body; }

private:
    Body m_Body;
};

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(push)
#pragma warning(disable:4200)
#endif
//---------------------------------------------------------------------------
//! @internal
//! @brief  QueryItemInfoPacket の返信パケットです。
//---------------------------------------------------------------------------
class QueryItemInfoReplyPacket : public HioPacket
{
public:
    struct Body : public HioPacketBody
    {
        atk::detail::fnd::BinU32 itemType;
        uint8_t                         itemEditInfo[0];
    };
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(pop)
#endif

public:
    QueryItemInfoReplyPacket() NN_NOEXCEPT :
      HioPacket(HioMessageType_QueryItemInfoReply, sizeof(HioPacketHeader))
    {
    }

public:
    static size_t GetRequiredSize(size_t maxItemInfoSize) NN_NOEXCEPT
    {
        // 固定パケットサイズ + アイテム情報の長さ
        return sizeof(HioPacketHeader) + sizeof(atk::detail::fnd::BinU32) + maxItemInfoSize;
    }

    const void* GetItemInfo() const NN_NOEXCEPT
    {
        return m_Body.itemEditInfo;
    }

    ResDataType GetItemType() const NN_NOEXCEPT
    {
        return static_cast<ResDataType>(static_cast<uint32_t>(m_Body.itemType));
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
