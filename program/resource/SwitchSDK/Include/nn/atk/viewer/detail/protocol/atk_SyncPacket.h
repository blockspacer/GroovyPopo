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
//! @brief  実機から定期的に送信される同期パケットです。
//---------------------------------------------------------------------------
class SyncPacket : public HioPacket
{
public:
    class Body : public HioPacketBody
    {
    public:
        Body() NN_NOEXCEPT :
          version(HioProtocolVersion),
          m_MemoryMax(0),
          m_MemoryUsage(0),
          m_MaxEditableItemCount(0),
          m_EditItemCount(0),
          m_MaxEditableFileCount(0),
          m_EditFileCount(0),
          m_IsOutOfMemory(false),
          m_IsItemsOverflow(false),
          m_IsFilesOverflow(false),
          interim_isSoundPlayer(false)
        {
        }

        uint32_t GetMemoryMax() const NN_NOEXCEPT { return m_MemoryMax; }
        void SetMemoryMax(uint32_t value) NN_NOEXCEPT { m_MemoryMax = value; }

        uint32_t GetMemoryUsage() const NN_NOEXCEPT { return m_MemoryUsage; }
        void SetMemoryUsage(uint32_t value) NN_NOEXCEPT { m_MemoryUsage = value; }

        uint32_t GetMaxEditableItemCount() const NN_NOEXCEPT { return m_MaxEditableItemCount; }
        void SetMaxEditableItemCount(uint32_t value) NN_NOEXCEPT { m_MaxEditableItemCount = value; }

        uint32_t GetEditItemCount() const NN_NOEXCEPT { return m_EditItemCount; }
        void SetEditItemCount(uint32_t value) NN_NOEXCEPT { m_EditItemCount = value; }

        uint32_t GetMaxEditableFileCount() const NN_NOEXCEPT { return m_MaxEditableFileCount; }
        void SetMaxEditableFileCount(uint32_t value) NN_NOEXCEPT { m_MaxEditableFileCount = value; }

        uint32_t GetEditFileCount() const NN_NOEXCEPT { return m_EditFileCount; }
        void SetEditFileCount(uint32_t value) NN_NOEXCEPT { m_EditFileCount = value; }

        bool GetIsOutOfMemory() const NN_NOEXCEPT { return m_IsOutOfMemory; }
        void SetIsOutOfMemory(bool value) NN_NOEXCEPT { m_IsOutOfMemory = value; }

        bool GetIsItemsOverflow() const NN_NOEXCEPT { return m_IsItemsOverflow; }
        void SetIsItemsOverflow(bool value) NN_NOEXCEPT { m_IsItemsOverflow = value; }

        bool GetIsFilesOverflow() const NN_NOEXCEPT { return m_IsFilesOverflow; }
        void SetIsFilesOverflow(bool value) NN_NOEXCEPT { m_IsFilesOverflow = value; }

// HACK : ★SoundMaker がゲームアプリと SoundPlayer の区別がつくようになるまでの暫定コード
#if 1
        void Interim_SetIsSoundPlayer(bool value) NN_NOEXCEPT
        {
            interim_isSoundPlayer = value;
        }
#endif

    private:
        ResVersion                  version;                 //!< バージョン情報です。
        atk::detail::fnd::BinU32  m_MemoryMax;             //!< メモリの上限値（バイト）です。
        atk::detail::fnd::BinU32  m_MemoryUsage;           //!< メモリ使用量（バイト）です。
        atk::detail::fnd::BinU32  m_MaxEditableItemCount;  //!< 差し替え可能なアイテムの最大数です。
        atk::detail::fnd::BinU32  m_EditItemCount;         //!< 現在差し替え中のアイテム数です。
        atk::detail::fnd::BinU32  m_MaxEditableFileCount;  //!< 差し替え可能なファイルの最大数です。
        atk::detail::fnd::BinU32  m_EditFileCount;         //!< 現在差し替え中の現在のファイル数です。
        atk::detail::fnd::BinBool m_IsOutOfMemory;         //!< メモリ不足フラグです。
        atk::detail::fnd::BinBool m_IsItemsOverflow;       //!< アイテム数のオーバーフローフラグです。
        atk::detail::fnd::BinBool m_IsFilesOverflow;       //!< ファイル数のオーバーフローフラグです。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t m_Padding0[1];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

// HACK : ★SoundMaker がゲームアプリと SoundPlayer の区別がつくようになるまでの暫定コード
#if 1
        atk::detail::fnd::BinBool interim_isSoundPlayer;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t m_Padding1[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
#endif
    };

public:
    SyncPacket() NN_NOEXCEPT : HioPacket(HioMessageType_Sync, 0)
    {
    }

// HACK : ★SoundMaker がゲームアプリと SoundPlayer の区別がつくようになるまでの暫定コード
#if 1
    explicit SyncPacket(bool interim_isSoundPlayer) NN_NOEXCEPT : HioPacket(HioMessageType_SyncReply, sizeof(Body))
    {
        m_Body.Interim_SetIsSoundPlayer(interim_isSoundPlayer);
    }
#endif

public:
    static size_t GetRequiredSize() NN_NOEXCEPT
    {
        return sizeof(SyncPacket);
    }

    Body&       GetBody() NN_NOEXCEPT { return m_Body; }
    const Body& GetBody() const NN_NOEXCEPT { return m_Body; }

private:
    Body m_Body;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  SyncPacket の返信パケットです。
//---------------------------------------------------------------------------
class SyncReplyPacket : public HioPacket
{
public:
    struct Body : public HioPacketBody
    {
        Body() NN_NOEXCEPT : isAccepted(false) { }

        atk::detail::fnd::BinBool isAccepted;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t _padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    };

public:
    SyncReplyPacket() NN_NOEXCEPT : HioPacket(HioMessageType_SyncReply, sizeof(Body))
    {
    }

public:
    static size_t GetRequiredSize() NN_NOEXCEPT
    {
        return sizeof(SyncReplyPacket);
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
