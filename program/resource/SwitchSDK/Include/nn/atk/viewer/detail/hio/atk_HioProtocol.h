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

#include <nn/util/util_BinTypes.h>
#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

//#define NN_ATKVIEWER_HIO_PACKET_DEBUG_DUMP

#include <nn/atk/atk_BinaryFileFormat.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/detail/res/atk_ResTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

static const uint32_t HioProtocolVersion = 0x00010000;       //!< SNDEDIT HIO通信プロトコルのバージョン

enum
{
    HioMessage_CategoryMask       = 0x00ff0000,              //!< HIO メッセージカテゴリのマスクです。
    HioMessage_ReplyFlagMask      = 0x00008000,              //!< HIO 返信フラグのマスクです。
    HioMessage_IdMask             = 0x00007fff,              //!< HIO メッセージ ID のマスクです。

    HioMessage_CategoryBitOffset  = 16,                      //!< HIO メッセージカテゴリ用のビットオフセットです。
    HioMessage_ReplyFlagBitOffset = 15,                      //!< HIO 返信フラグ用のビットオフセットです。
    HioMessage_IdBitOffset        = 0,                       //!< HIO メッセージ ID 用のビットオフセットです。

    HioMessage_ReplyFlag          = HioMessage_ReplyFlagMask //!< 返信メッセージであることを示すフラグです。
};

//---------------------------------------------------------------------------
//! @brief  サウンドで利用するメッセージのカテゴリです。
//---------------------------------------------------------------------------
enum HioMessageCategory
{
    HioMessageCategory_System  = 0x01 << HioMessage_CategoryBitOffset,     //!< サウンドシステム用のカテゴリです。
    HioMessageCategory_Sndedit = 0x02 << HioMessage_CategoryBitOffset,     //!< サウンド編集用のカテゴリです。
    HioMessageCategory_Sndctrl = 0x03 << HioMessage_CategoryBitOffset      //!< サウンド操作用のカテゴリです。（暫定）
};

//---------------------------------------------------------------------------
//! @brief  サウンドで利用するチャンネル番号です。
//---------------------------------------------------------------------------
enum HioChannelType
{
    HioChannelType_Base                = 0x8000 + ( 'S' << 8 )+ 'D',
    //HioChannelType_SoundplayerPassive = HioChannelType_Base + 0x0,   //!< PC->実機 方向に要求するチャンネルです。(仮SoundPlayer用)
    //HioChannelType_SoundplayerActive  = HioChannelType_Base + 0x1,   //!< 実機->PC 方向に要求するチャンネルです。(仮SoundPlayer用)
    HioChannelType_SndeditSync        = HioChannelType_Base + 0x2,   //!< SNDEDIT SYNC 用チャンネルです。
    HioChannelType_SndeditFunc        = HioChannelType_Base + 0x3,   //!< SNDEDIT 機能チャンネルです。
    HioChannelType_SndeditCtrl        = HioChannelType_Base + 0x4,   //!< SNDCTRL 操作チャンネルです。（暫定）
    //HioChannelType_PingChannel                = HioChannelType_BaseChannel + 0xf    //!< 実機の生存確認を行うチャンネルです。
};

//---------------------------------------------------------------------------
//! @brief  SNDEDIT が受け取れる HIO メッセージの種類です。
//---------------------------------------------------------------------------
typedef uint32_t HioMessageType;

// HioMessageType の具体値です。
enum
{
    //! @brief  無効なメッセージIDです。
    HioMessageType_Invalid = 0,

    //---------------------------------------------------------------------------
    // システムカテゴリ
    //---------------------------------------------------------------------------

    //! @brief  実機から定期的に送信される同期メッセージです。
    HioMessageType_Sync = 0x00 + HioMessageCategory_System,

    //! @brief  HioMessageType_Sync の返信メッセージです。
    HioMessageType_SyncReply = HioMessageType_Sync + HioMessage_ReplyFlag,

    //! @brief  実機から定期的に送信される無効アイテムの問い合わせ同期メッセージです。
    HioMessageType_QueryItems = 0x01 + HioMessageCategory_System,

    //! @brief  HioMessageType_QueryItems の返信メッセージです。
    HioMessageType_QueryItemsReply = HioMessageType_QueryItems + HioMessage_ReplyFlag,

    //---------------------------------------------------------------------------
    // サウンド編集カテゴリ
    //---------------------------------------------------------------------------

    //! @brief  アイテム情報を取得します。
    HioMessageType_QueryItemInfo = 0x00 + HioMessageCategory_Sndedit,

    //! @brief  HioMessageType_QueryItemInfo の返信メッセージです。
    HioMessageType_QueryItemInfoReply = HioMessageType_QueryItemInfo + HioMessage_ReplyFlag,

    //! @brief  アイテムのキャッシュ開始を通知します。
    HioMessageType_BeginItemCache = 0x01 + HioMessageCategory_Sndedit,

    //! @brief  HioMessageType_BeginItemCache の返信メッセージです。
    HioMessageType_BeginItemCacheReply  = HioMessageType_BeginItemCache + HioMessage_ReplyFlag,

    //! @brief  アイテムのキャッシュ開始を通知します。
    HioMessageType_EndItemCache = 0x02 + HioMessageCategory_Sndedit,

    //! @brief  HioMessageType_EndItemCache の返信メッセージです。
    HioMessageType_EndItemCacheReply  = HioMessageType_EndItemCache + HioMessage_ReplyFlag,

    //---------------------------------------------------------------------------
    // サウンド操作カテゴリ（暫定）
    //---------------------------------------------------------------------------

    //! @brief  サウンドを再生します。（PC -> TARGET)
    HioMessageType_PlaySound = 0x00 + HioMessageCategory_Sndctrl,

    //! @brief  サウンドを停止します。（PC -> TARGET)
    HioMessageType_StopSound = 0x01 + HioMessageCategory_Sndctrl,

    //! @brief  サウンドを一時停止します。（PC -> TARGET)
    HioMessageType_PauseSound = 0x02 + HioMessageCategory_Sndctrl,

    //! @brief  サウンド情報を更新します。（TARGET -> PC)
    HioMessageType_UpdateSoundInfo = 0x80 + HioMessageCategory_Sndctrl
};

//---------------------------------------------------------------------------
//! @brief  HIO の処理結果です。
//---------------------------------------------------------------------------
enum HioResultType
{
    HioResultType_ErrorFlagMask  = 0x80000000,
    HioResultType_CategoryMask    = 0x7f000000,
    HioResultType_CodeMask        = 0x00ffffff,

    HioResultType_CategoryBitOffset = 24,        //!< HIO 結果カテゴリ用のビットオフセットです。

    HioResultType_ErrorFlag      = HioResultType_ErrorFlagMask,
    HioResultType_CategoryNone   = 0x00 << HioResultType_CategoryBitOffset,
    HioResultType_CategoryTarget = 0x01 << HioResultType_CategoryBitOffset,
    HioResultType_CategoryPc     = 0x02 << HioResultType_CategoryBitOffset,

    //---------------------------------------------------------------------------
    // 成功コード
    //---------------------------------------------------------------------------

    HioResultType_True  = 0x000000 + HioResultType_CategoryNone,                            //!< 成功しました。
    HioResultType_False = 0x000001 + HioResultType_CategoryNone,                            //!< 処理を実行しませんでした。

    //---------------------------------------------------------------------------
    // 失敗コード
    //---------------------------------------------------------------------------

    HioResultType_Failed = 0x000001 + HioResultType_CategoryNone + HioResultType_ErrorFlag,    //!< 失敗しました。

    //---------------------------------------------------------------------------
    // 失敗コード（実機）
    //---------------------------------------------------------------------------

    HioResultType_TargetNotInitialized = 0x000001 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< 初期化されていません。
    HioResultType_TargetOutOfMemory    = 0x000002 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< メモリ不足です。
    HioResultType_TargetNotImplemented = 0x000003 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< 未実装です。
    HioResultType_TargetCacheOverFlow  = 0x000004 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< キャッシュ可能数を超えました。
    HioResultType_TargetNameNotFound   = 0x000005 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< 名前が見つかりません。

    HioResultType_TargetTimeout        = 0x000101 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< タイムアウトが発生しました。
    HioResultType_TargetSendError      = 0x000102 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< 送信エラーです。
    HioResultType_TargetRecieveError   = 0x000103 + HioResultType_CategoryTarget + HioResultType_ErrorFlag,    //!< 受信エラーです。

    //---------------------------------------------------------------------------
    // 失敗コード（PC）
    //---------------------------------------------------------------------------

    HioResultType_PcNameNotFound   = 0x000001 + HioResultType_CategoryPc + HioResultType_ErrorFlag,    //!< 名前が見つかりません。
    HioResultType_PcItemPreparing  = 0x000002 + HioResultType_CategoryPc + HioResultType_ErrorFlag     //!< アイテムは準備中です。
};

//---------------------------------------------------------------------------
//! @brief  HIO の処理結果を制御します。
//---------------------------------------------------------------------------
class HioResult
{
public:
    HioResult() NN_NOEXCEPT : value(HioResultType_True) { }
    explicit HioResult(uint32_t value)  NN_NOEXCEPT : value(value) { }
    explicit HioResult(HioResultType value) NN_NOEXCEPT : value(value) { }

    static ResultType ToResult(HioResultType value) NN_NOEXCEPT
    {
        switch(value)
        {
        case HioResultType_True:
            return Result(ResultType_True);

        case HioResultType_False:
            return Result(ResultType_False);

        case  HioResultType_Failed:
            return Result(ResultType_Failed);

        case  HioResultType_TargetNotInitialized:
            return Result(ResultType_NotInitialized);

        case  HioResultType_TargetOutOfMemory:
            return Result(ResultType_OutOfMemory);

        case HioResultType_TargetCacheOverFlow:
            return Result(ResultType_CacheOverFlow);

        case  HioResultType_TargetNameNotFound:
            return Result(ResultType_NameNotFound);

        case  HioResultType_TargetTimeout:
            return Result(ResultType_Timeout);

        case  HioResultType_PcNameNotFound:
            return Result(ResultType_NameNotFound);

        case  HioResultType_PcItemPreparing:
            return Result(ResultType_ItemPreparing);

        default:
            break;
        }

        return HioResult(value).IsSucceeded() ?
            Result(ResultType_True) : Result(ResultType_Failed);
    }

    static HioResult FromResult(ResultType value) NN_NOEXCEPT
    {
        switch(uint32_t(value))
        {
        case ResultType_True:
            return HioResult(HioResultType_True);

        case ResultType_False:
            return HioResult(HioResultType_False);

        case ResultType_Failed:
            return HioResult(HioResultType_Failed);

        case ResultType_NotInitialized:
            return HioResult(HioResultType_TargetNotInitialized);

        case ResultType_OutOfMemory:
            return HioResult(HioResultType_TargetOutOfMemory);

        case ResultType_Timeout:
            return HioResult(HioResultType_TargetTimeout);

        case ResultType_CacheOverFlow:
            return HioResult(HioResultType_TargetCacheOverFlow);

        case ResultType_NameNotFound:
            return HioResult(HioResultType_TargetNameNotFound);

        case ResultType_ItemPreparing:
            return HioResult(HioResultType_PcItemPreparing);

        default:
            break;

        }

        return HioResult(value).IsSucceeded() ? HioResult(HioResultType_True) : HioResult(HioResultType_Failed);
    }

    bool IsSucceeded() const NN_NOEXCEPT { return !IsFailed(); }
    bool IsTrue() const NN_NOEXCEPT { return value == HioResultType_True; }
    bool IsFalse() const NN_NOEXCEPT { return value == HioResultType_False; }
    bool IsFailed() const NN_NOEXCEPT { return (value & HioResultType_ErrorFlag) != 0; }

    //--------------------------------------------------------------------------
    //! @brief   uint32_t 型へのキャスト演算子です。
    //!
    //! @return  uint32_t 値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator uint32_t() const NN_NOEXCEPT
    {
        return value;
    }

    //--------------------------------------------------------------------------
    //! @brief   HioResultType 型へのキャスト演算子です。
    //!
    //! @return  HioResultType で表現した値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator HioResultType() const NN_NOEXCEPT
    {
        return static_cast<HioResultType>(value);
    }

    //--------------------------------------------------------------------------
    //! @brief   ResultType 型へのキャスト演算子です。
    //!
    //! @return  ResultType で表現した値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator ResultType() const NN_NOEXCEPT
    {
        return HioResult::ToResult(static_cast<HioResultType>(value));
    }

private:
    uint32_t value;
};

//---------------------------------------------------------------------------
//! @brief   HIOメッセージです。
//!
//! @details 32bit の内訳は以下の通りです。
//!          31 - 24 : Reserved
//!          23 - 16 : メッセージカテゴリ
//!          15 -  0 : メッセージID
//---------------------------------------------------------------------------
struct HioMessage
{
    atk::detail::fnd::BinU32 value;

    //---------------------------------------------------------------------------

    HioMessage() NN_NOEXCEPT : value(0) { }
    explicit HioMessage(HioMessageType value) NN_NOEXCEPT : value(value) { }

    //---------------------------------------------------------------------------
    //! @brief   HIOメッセージカテゴリを取得します。
    //!
    //! @return  HIOメッセージカテゴリを返します。
    //---------------------------------------------------------------------------
    HioMessageType GetCategory() const NN_NOEXCEPT
    {
        return (value & HioMessage_IdMask) >> HioMessage_CategoryBitOffset;
    }

    //---------------------------------------------------------------------------
    //! @brief   HIOメッセージID を取得します。
    //!
    //! @return  HIOメッセージID を返します。
    //---------------------------------------------------------------------------
    HioMessageType GetID() const NN_NOEXCEPT
    {
        return (value & HioMessage_IdMask) >> HioMessage_IdBitOffset;
    }

    //--------------------------------------------------------------------------
    //! @brief   HioMessageType 型へのキャスト演算子です。
    //!
    //! @return  HioMessageType で表現した値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator HioMessageType() const NN_NOEXCEPT
    {
        return value;
    }

#if !defined(NN_SDK_BUILD_RELEASE)
    //--------------------------------------------------------------------------
    //! @brief   指定メッセージを文字列に変換します。
    //!
    //! @param[in]  message  メッセージを指定します。
    //!
    //! @return  変換した文字列を返します。
    //---------------------------------------------------------------------------
    static const char* ToString(HioMessageType message) NN_NOEXCEPT
    {
        switch(message)
        {
        case HioMessageType_Sync:
            return "HioMessageType_Sync";

        case HioMessageType_SyncReply:
            return "HioMessageType_SyncReply";

        case HioMessageType_QueryItems:
            return "HioMessageType_QueryItems";

        case HioMessageType_QueryItemsReply:
            return "HioMessageType_QueryItemsReply";

        case HioMessageType_QueryItemInfo:
            return "HioMessageType_QueryItemInfo";

        case HioMessageType_QueryItemInfoReply:
            return "HioMessageType_QueryItemInfoReply";

        case HioMessageType_BeginItemCache:
            return "HioMessageType_BeginItemCache";

        case HioMessageType_BeginItemCacheReply:
            return "HioMessageType_BeginItemCacheReply";

        case HioMessageType_EndItemCache:
            return "HioMessageType_EndItemCache";

        case HioMessageType_EndItemCacheReply:
            return "HioMessageType_EndItemCacheReply";

        case HioMessageType_PlaySound:
            return "HioMessageType_PlaySound";

        case HioMessageType_StopSound:
            return "HioMessageType_StopSound";

        case HioMessageType_PauseSound:
            return "HioMessageType_PauseSound";

        case HioMessageType_UpdateSoundInfo:
            return "HioMessageType_UpdateSoundInfo";

        default:
            break;

        }

        return "Unknown Message";
    }

    //--------------------------------------------------------------------------
    //! @brief   メッセージを文字列に変換します。
    //!
    //! @return  変換した文字列を返します。
    //---------------------------------------------------------------------------
    const char* ToString() const NN_NOEXCEPT
    {
        return ToString(value);
    }
#endif
};

//---------------------------------------------------------------------------
//! @brief  HIO パケットヘッダです。
//---------------------------------------------------------------------------
struct HioPacketHeader
{
    static const uint32_t ValidSignature = NN_UTIL_CREATE_SIGNATURE_4('S', 'E', 'D', 'T');

    uint32_t signature;
    HioMessage message;
    atk::detail::fnd::BinU32 bodySize;
    atk::detail::fnd::BinU32 result;

    //---------------------------------------------------------------------------

    HioPacketHeader() NN_NOEXCEPT
    {}

    explicit HioPacketHeader(HioMessage message) NN_NOEXCEPT
        : signature(ValidSignature),
          message(message),
          bodySize(0),
          result(HioResultType_True)
    {
    }

    HioPacketHeader(HioMessage message, uint32_t bodySize) NN_NOEXCEPT
        : signature(ValidSignature),
          message(message),
          bodySize(bodySize),
          result(HioResultType_True)
    {}

    bool IsValid() const NN_NOEXCEPT { return signature == ValidSignature; }

    HioResult GetResult() const NN_NOEXCEPT { return HioResult(static_cast<uint32_t>(result)); }
};

//---------------------------------------------------------------------------
//! @brief  HIO パケットボディです。
//---------------------------------------------------------------------------
struct HioPacketBody
{
};

//---------------------------------------------------------------------------
//! @brief  HIO パケットです。
//---------------------------------------------------------------------------
class HioPacket
{
public:
    HioPacket() NN_NOEXCEPT { }
    explicit HioPacket(HioMessageType message) NN_NOEXCEPT :
        m_Header(HioMessage(message), sizeof(HioPacketHeader)) { }

    HioPacket(HioMessageType message, uint32_t bodySize) NN_NOEXCEPT :
        m_Header(HioMessage(message), bodySize) { }

public:
    static HioPacket& Null() NN_NOEXCEPT
    {
        static HioPacket nullInstance(HioMessageType_Invalid, 0);
        return nullInstance;
    }

    HioPacketHeader&       GetHeader()       NN_NOEXCEPT { return m_Header; }
    const HioPacketHeader& GetHeader() const NN_NOEXCEPT { return m_Header; }

    HioPacketBody* GetBody() NN_NOEXCEPT
    {
        return m_Header.bodySize == 0 ?
            NULL : reinterpret_cast<HioPacketBody*>(util::BytePtr(this, sizeof(HioPacketHeader)).Get());
    }

    const HioPacketBody* GetBody() const NN_NOEXCEPT
    {
        return m_Header.bodySize == 0 ?
            NULL : reinterpret_cast<const HioPacketBody*>(util::ConstBytePtr(this, sizeof(HioPacketHeader)).Get());
    }

    size_t GetSize() const NN_NOEXCEPT
    {
        return sizeof(HioPacketHeader) + m_Header.bodySize;
    }

#if defined(NN_ATKVIEWER_HIO_PACKET_DEBUG_DUMP)
public:
    void Dump() NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("--------------- HioPacketDumpStart ---------------\n");
        DumpHeader();
        NN_DETAIL_ATK_INFO("--------------------------------------------------\n");
        DumpBody();
        NN_DETAIL_ATK_INFO("---------------- HioPacketDumpEnd ----------------\n");
    }

private:
    void DumpHeader() NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("signature = %x, message = %d\n", m_Header.signature, m_Header.message.value);
        NN_DETAIL_ATK_INFO("bodySize = %d, result = %d\n", m_Header.bodySize, m_Header.result);
    }

    void DumpBody() NN_NOEXCEPT
    {
        const void* body = reinterpret_cast<const void*>(GetBody());
        const uint8_t* startAddress = reinterpret_cast<const uint8_t*>(body);

        for (auto i = 0; i < m_Header.bodySize; ++i)
        {
            NN_DETAIL_ATK_INFO("%02x ", *startAddress);
            startAddress++;
        }
        NN_DETAIL_ATK_INFO("\n");

        startAddress = reinterpret_cast<const uint8_t*>(body);
        for (auto i = 0; i < m_Header.bodySize; ++i)
        {
            NN_DETAIL_ATK_INFO("%c ", *startAddress);
            startAddress++;
        }
        NN_DETAIL_ATK_INFO("\n");
    }
#endif

private:
    HioPacketHeader m_Header;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
