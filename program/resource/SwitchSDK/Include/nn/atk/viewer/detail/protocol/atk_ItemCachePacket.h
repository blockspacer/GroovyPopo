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
#include <nn/util/util_StringUtil.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//! @internal
//! @brief  キャッシュ操作の種類を示します。
enum CacheAction
{
    CacheAction_Update = 1,      //!< キャッシュを更新します。
    CacheAction_Remove = 2       //!< キャッシュを削除します。
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  アイテムのキャッシュ開始を通知するパケットです。
//---------------------------------------------------------------------------
class BeginItemCachePacket : public HioPacket
{
public:
    class Body : public HioPacketBody
    {
    public:
        Body(const char* itemName, int itemNameLength) NN_NOEXCEPT :
          m_Action(CacheAction_Update),
          m_IsTargetAll(false)
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
            // m_Action, m_IsTargetAll, m_Padding[2], m_ItemName
            return 4 + ResName::GetRequiredSize(maxItemName);
        }

        CacheAction GetAction() const NN_NOEXCEPT { return static_cast<CacheAction>(m_Action); }
        void SetAction(CacheAction value) NN_NOEXCEPT { m_Action = static_cast<uint8_t>(value); }

        bool GetIsTargetAll() const NN_NOEXCEPT { return m_IsTargetAll; }
        void SetIsTargetAll(bool value) NN_NOEXCEPT { m_IsTargetAll = value; }

        uint32_t         GetItemNameLength() const NN_NOEXCEPT { return m_ItemName.len; }
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
        uint8_t          m_Action;       //!< キャッシュ操作の種類です。
        bool        m_IsTargetAll;  //!< すべてのアイテムを対象とするフラグです。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t     m_Padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

        ResNameData m_ItemName;     //!< 対象アイテム名です。
    };

public:
    BeginItemCachePacket(const char* itemName, int maxItemName) NN_NOEXCEPT :
      HioPacket(HioMessageType_BeginItemCache, static_cast<uint32_t>(Body::GetRequiredSize(maxItemName))),
          m_Body(itemName, maxItemName)
    {
    }

public:
    static BeginItemCachePacket* CreateInstance(
        void* buffer,
        const char* itemName) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(buffer);

        int itemNameLength = itemName == NULL ? 0 : static_cast<int>(std::strlen(itemName));
        return new(buffer) BeginItemCachePacket(itemName, itemNameLength);
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

//---------------------------------------------------------------------------
//! @internal
//! @brief  BeginItemCachePacket の返信パケットです。
//---------------------------------------------------------------------------
class BeginItemCacheReplyPacket : public HioPacket
{
public:
    struct Body : public HioPacketBody
    {
    };

public:
    BeginItemCacheReplyPacket() NN_NOEXCEPT : HioPacket(HioMessageType_BeginItemCacheReply, sizeof(Body))
    {
    }

public:
    static uint32_t GetRequiredSize() NN_NOEXCEPT
    {
        return sizeof(BeginItemCacheReplyPacket);
    }

    Body&       GetBody() NN_NOEXCEPT { return m_Body; }
    const Body& GetBody() const NN_NOEXCEPT { return m_Body; }

private:
    Body m_Body;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  アイテムのキャッシュ開始を通知するパケットです。
//---------------------------------------------------------------------------
class EndItemCachePacket : public HioPacket
{
public:
    class Body : public HioPacketBody
    {
    public:
        Body(const char* itemName, int itemNameLength) NN_NOEXCEPT :
          m_Action(CacheAction_Update),
          m_IsTargetAll(false)
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
            // m_Action, m_IsTargetAll, m_Padding[2], m_ItemName
            return 4 + ResName::GetRequiredSize(maxItemName);
        }

        CacheAction GetAction() const NN_NOEXCEPT { return static_cast<CacheAction>(m_Action); }
        void SetAction(CacheAction value) NN_NOEXCEPT { m_Action = static_cast<uint8_t>(value); }

        bool GetIsTargetAll() const NN_NOEXCEPT { return m_IsTargetAll; }
        void SetIsTargetAll(bool value) NN_NOEXCEPT { m_IsTargetAll = value; }

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
        uint8_t          m_Action;           //!< キャッシュ操作の種類です。
        bool        m_IsTargetAll;      //!< すべてのアイテムを対象とするフラグです。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t     m_Padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
        ResNameData m_ItemName;
    };

public:
    EndItemCachePacket(const char* itemName, int maxItemName) NN_NOEXCEPT :
      HioPacket(HioMessageType_EndItemCache, static_cast<uint32_t>(Body::GetRequiredSize(maxItemName))),
          m_Body(itemName, maxItemName)
      {
      }

public:
    static EndItemCachePacket* CreateInstance(void* buffer, const char* itemName, Result itemCacheResult) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(buffer);

        int itemNameLength = itemName == NULL ? 0 : static_cast<int>(std::strlen(itemName));

        EndItemCachePacket* result = new(buffer) EndItemCachePacket(itemName, itemNameLength);

        result->GetHeader().result = HioResult::FromResult(itemCacheResult);

        return result;
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

//---------------------------------------------------------------------------
//! @internal
//! @brief  EndItemCachePacket の返信パケットです。
//---------------------------------------------------------------------------
class EndItemCacheReplyPacket : public HioPacket
{
public:
    struct Body : public HioPacketBody
    {
    };

public:
    EndItemCacheReplyPacket() NN_NOEXCEPT : HioPacket(HioMessageType_EndItemCacheReply, sizeof(Body))
    {
    }

public:
    static uint32_t GetRequiredSize() NN_NOEXCEPT
    {
        return sizeof(EndItemCacheReplyPacket);
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
