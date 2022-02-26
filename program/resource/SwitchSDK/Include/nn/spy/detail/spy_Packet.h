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

#include <nn/spy/spy_Config.h>
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#include <nn/spy/detail/fnd/basis/spyfnd_Memory.h>
#include <nn/spy/detail/fnd/binary/spyfnd_Binary.h>
#include <nn/spy/detail/spy_Log.h>
#include <nn/spy/detail/spy_Macros.h>
#include <nn/spy/detail/spy_SpyDataId.h>

namespace nn {
namespace spy {
namespace detail {

class ProtocolConstants
{
public:
    static const uint32_t Signature = NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE('S', 'S', 'P', 'Y');     //!< シグネチャ
    static const uint32_t ProtocolVersion = NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(1, 0, 0, 0);
};

//! @briefprivate
//! @brief パケットID
struct PacketId
{
    //! @brief パケットID の種別
    enum Type
    {
        // 無効値
        Type_Invalid = 0,

        // PC -> TARGET
        Type_Initialize,
        Type_InitializeReply,
        Type_Finalize,
        Type_FinalizeReply,
        Type_SelectDataId,
        Type_SelectDataIdReply,
        Type_Ping,
        Type_Pong,
        Type_SetOutputDir,
        Type_SetOutputDirReply,
        Type_NotifyDataRead,
        Type_NotifyDataReadReply,
        Type_QueryDataInfo,
        Type_QueryDataInfoReply,

        // PC <- TARGET
        Type_Data,
        Type_DataEnd,

        // 最大値
        Type_Max = Type_DataEnd
    };

    //----------------------------------------------------------

    // enum は 1 バイトになったりするので、uint32_t で PacketId を表現することにする
    uint32_t value;

    //----------------------------------------------------------

    explicit PacketId(uint32_t value) NN_NOEXCEPT : value(value) { }

    explicit PacketId(Type value) NN_NOEXCEPT : value(value) { }

    const char* ToString() const NN_NOEXCEPT
    {
#if defined(NN_SDK_BUILD_DEBUG)
        static const char* s_PacketIdString[] =
        {
            "Type_Invalid",
            "Type_Initialize",
            "Type_InitializeReply",
            "Type_Finalize",
            "Type_FinalizeReply",
            "Type_SelectDataId",
            "Type_SelectDataIdReply",
            "Type_Ping",
            "Type_Pong",
            "Type_SetOutputDir",
            "Type_SetOutputDirReply",
            "Type_NotifyDataRead",
            "Type_NotifyDataReadReply",
            "Type_QueryDataInfo",
            "Type_QueryDataInfoReply",
            "Type_Data",
            "Type_DataEnd"
        };

        NN_STATIC_ASSERT(sizeof(s_PacketIdString) / sizeof(s_PacketIdString[0]) == Type_Max + 1);

        return s_PacketIdString[value > Type_Max ? Type_Invalid : value];
#else
        return NULL;
#endif
    }
};

//----------------------------------------------------------
//! @ brief パケットヘッダ
struct PacketHeader
{
    enum ResultCode
    {
        ResultCode_Success = 0,
        ResultCode_Error = -1
    };

    //----------------------------------------------------------

    uint32_t      signature;
    PacketId id;
    uint32_t      bodyLength;
    uint32_t      resultCode;

    //----------------------------------------------------------

    PacketHeader() NN_NOEXCEPT
        : signature(ProtocolConstants::Signature), id(PacketId::Type_Invalid), bodyLength(0), resultCode(ResultCode_Success)
    {}

    PacketHeader(PacketId packetId, size_t bodyLength) NN_NOEXCEPT
      : signature(ProtocolConstants::Signature)
      , id(packetId)
      , bodyLength(static_cast<uint32_t>(bodyLength))
      , resultCode(ResultCode_Success)
    {
        NN_SDK_ASSERT_LESS_EQUAL(bodyLength, std::numeric_limits<uint32_t>::max());
    }

    PacketHeader(PacketId::Type packetId, size_t bodyLength) NN_NOEXCEPT
        : signature(ProtocolConstants::Signature)
        , id(packetId)
        , bodyLength(static_cast<uint32_t>(bodyLength))
        , resultCode(ResultCode_Success)
    {
        NN_SDK_ASSERT_LESS_EQUAL(bodyLength, std::numeric_limits<uint32_t>::max());
    }

    //----------------------------------------------------------

    void Dump() const NN_NOEXCEPT
    {
#if defined(NN_SDK_BUILD_DEBUG)
        NN_DETAIL_SPY_INFO("[%-30s](%2d) bodyLength(%4d, 0x%08x)\n", id.ToString(), id.value, bodyLength, bodyLength);
#endif
    }
};

//----------------------------------------------------------
//! @ brief パケット既定クラス
struct PacketBase
{
    PacketHeader header;

    PacketBase(PacketId packetId, size_t packetLength) NN_NOEXCEPT
        : header(packetId, packetLength - sizeof(PacketHeader))
    {}

    PacketBase(PacketId::Type packetId, size_t packetLength) NN_NOEXCEPT
        : header(packetId, (packetLength == 0) ? 0 : packetLength - sizeof(PacketHeader))
    {
        NN_SDK_ASSERT(packetLength == 0 || packetLength >= sizeof(PacketHeader));
    }
};

//----------------------------------------------------------
//! @ brief 初期化パケット
struct InitializePacket : public PacketBase
{
    struct Body
    {
        uint32_t reserved;

        //----------------------------------------------------------
        Body() NN_NOEXCEPT :
        reserved(0)
        {
        }
    };

    Body body;

    NN_STATIC_ASSERT(sizeof(Body) == 4);

    InitializePacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_Initialize, sizeof(InitializePacket))
    {}
};

//----------------------------------------------------------
//! @ brief 初期化返信パケット
struct InitializeReplyPacket : public PacketBase
{
    struct Body
    {
        uint32_t protocolVersion;
    };

    NN_STATIC_ASSERT(sizeof(Body) == 4);

    Body body;

    InitializeReplyPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_InitializeReply, sizeof(InitializeReplyPacket))
    {
        body.protocolVersion = ProtocolConstants::ProtocolVersion;
    }
};

//----------------------------------------------------------
//! @ brief 終了パケット
struct FinalizePacket : public PacketBase
{
    FinalizePacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_Finalize, sizeof(FinalizePacket))
    {}
};

//----------------------------------------------------------
//! @ brief 終了返信パケット
struct FinalizeReplyPacket : public PacketBase
{
    FinalizeReplyPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_FinalizeReply, sizeof(FinalizeReplyPacket))
    {}
};

//----------------------------------------------------------
//! @ brief SpyDataId 選択パケット
struct SelectDataIdPacket : public PacketBase
{
    static const int MaxFlagsCount = 32;

    struct Body
    {
        //! selectionFlags メンバーの配列長です。
        uint32_t selectionFlagsLength;

        //! データ要求の有無を表すビットフィールドです。
        //!
        //! データID(i)の要求の有無は selectionFlags[i / 32] & (1 << (i % 32)) != 0 で判断されます。
        //!
        uint32_t selectionFlags[MaxFlagsCount];
    };

    NN_STATIC_ASSERT(sizeof(Body) == 132);

    Body body;

    SelectDataIdPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_SelectDataId, sizeof(0)) // 可変長
    {}
};

//----------------------------------------------------------
//! @ brief SpyDataId 選択返信パケット
struct SelectDataIdReplyPacket : public PacketBase
{
    SelectDataIdReplyPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_SelectDataIdReply, sizeof(SelectDataIdReplyPacket))
    {}
};

//----------------------------------------------------------
//! @ brief ping パケット
struct PingPacket : public PacketBase
{
    PingPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_Ping, sizeof(PingPacket))
    {}
};

//----------------------------------------------------------
//! @ brief pong パケット
struct PongPacket : public PacketBase
{
    PongPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_Pong, sizeof(PongPacket))
    {}
};

//----------------------------------------------------------
//! @ brief 出力ディレクトリ指定パケット
struct SetOutputDirPacket : public PacketBase
{
    // 出力ディレクトリのパス文字列の最大長です。ヌル文字を含みます。
    static const int OutputDirPathSize = 512;

    struct Body
    {
        uint32_t length;
        char path[OutputDirPathSize];
    };

    NN_STATIC_ASSERT(sizeof(Body) == 516);

    Body body;

    SetOutputDirPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_SetOutputDir, sizeof(SetOutputDirPacket))
    {}
};

//----------------------------------------------------------
//! @ brief 出力ディレクトリ指定返信パケット
//!
//! Sync, Data両チャンネルで返信されます。
//! Dataチャンネルでの返信によりファイルによるデータ転送が有効化されます。
//!
struct SetOutputDirReplyPacket : public PacketBase
{
    SetOutputDirReplyPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_SetOutputDirReply, sizeof(SetOutputDirReplyPacket))
    {}
};

//----------------------------------------------------------
//! @ brief データリード開始パケット
//!
//! Spy.exeがデータファイルの読み出しを開始したことを示します。
//! アプリのデータ書き込みがSpy.exeのデータ読み出しを追い越さないようにします。
//!
struct NotifyDataReadPacket : public PacketBase
{
    struct Body
    {
        uint32_t fileIndex; //!< 読み込みを開始したファイル番号です(0 or 1)。
    };

    NN_STATIC_ASSERT(sizeof(Body) == 4);

    Body body;

    NotifyDataReadPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_NotifyDataRead, sizeof(NotifyDataReadPacket))
    {}
};

//----------------------------------------------------------
//! @ brief データリード開始返信パケット
struct NotifyDataReadReplyPacket : public PacketBase
{
    NotifyDataReadReplyPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_NotifyDataReadReply, sizeof(NotifyDataReadReplyPacket))
    {}
};

//----------------------------------------------------------
//! @brief データ情報要求パケット(Sync)
//!
//! アプリが対応しているデータ情報を問い合わせます。
//!
struct QueryDataInfoPacket : public PacketBase
{
    QueryDataInfoPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_QueryDataInfo, sizeof(QueryDataInfoPacket))
    {}
};

//----------------------------------------------------------
//! @brief データ情報要求返信パケット(Sync)
//!
//! dataId に 0 を指定した場合には、データ情報問合せのシーケンスを終わります。
//!
struct QueryDataInfoReplyPacket : public PacketBase
{
    struct Body
    {
        uint32_t dataVersion; //!< データのバージョンです。
        uint16_t dataId; //!< セッション中にのみ有効なデータIDです。
        uint16_t dataNameLength; //! dataNameフィールドに格納された文字列の長さです。パディングは含みません。

        // 以下に可変長フィールドが続きます。
        // dataName[dataNameLength]; //!< データタイプを表す文字列です（ヌル終端は不要。4バイトアライメント）。
    };

    NN_STATIC_ASSERT(sizeof(Body) == 8);

    Body body;

    QueryDataInfoReplyPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_QueryDataInfoReply, 0) // 可変長
    {}

    void SetPacketSize() NN_NOEXCEPT
    {
        header.bodyLength = sizeof(Body) + ((body.dataNameLength + 3) & ~3);
    }
};

//----------------------------------------------------------
//! @ brief データパケット
struct DataPacket : public PacketBase
{
    struct Body
    {
        // timestamp の型に uint64_t を使うとバイト境界が8バイトになってしまうので２つに分割します。
        uint32_t timestampLow;        //!< パケットの記録時間。単位は[usec]。下位4バイト。
        uint32_t timestampHigh;       //!< パケットの記録時間。単位は[usec]。上位4バイト。
        int32_t dataId;             //!< データID
        uint32_t payloadLength;      //!< データペイロードの長さ
        uint32_t paddingLength;      //!< パディングの長さ

        // 以下に可変長フィールドが続く
        // payload;             //!< データペイロード

        //----------------------------------------------------------
        Body() NN_NOEXCEPT :
        timestampLow(0),
        timestampHigh(0),
        dataId(SpyDataId_Invalid),
        payloadLength(0),
        paddingLength(0)
        {
        }
    };

    NN_STATIC_ASSERT(sizeof(Body) == 20);

    Body body;

    DataPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_Data, 0)  // 可変長
    {}

    static size_t GetRequiredMemorySize(size_t payloadLength) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_LESS_EQUAL(payloadLength, GetMaxPayloadLength());

        return nn::spy::detail::fnd::RoundUp(
            sizeof(DataPacket) + payloadLength,
            nn::spy::detail::fnd::MemoryTraits::DefaultAlignment);
    }

    //! @briefprivate
    //!
    //! @brief パケットの送信時間を設定します。
    //!
    //! @details
    //! SpyController が使用します。
    //!
    //! @param[in] timestamp  時間情報[usec]です。
    //!
    void SetTimestamp(uint64_t timestamp) NN_NOEXCEPT
    {
        body.timestampLow = static_cast<uint32_t>(timestamp);
        body.timestampHigh = static_cast<uint32_t>(timestamp >> 32);
    }

    void SetLengths(size_t payloadLength) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_LESS_EQUAL(payloadLength, GetMaxPayloadLength());

        body.payloadLength = static_cast<uint32_t>(payloadLength);
        body.paddingLength = static_cast<uint32_t>(
            nn::spy::detail::fnd::RoundUp(payloadLength, nn::spy::detail::fnd::MemoryTraits::DefaultAlignment) - payloadLength);

        header.bodyLength = static_cast<uint32_t>(sizeof(Body) + payloadLength + body.paddingLength);
    }

    void* GetPayload() NN_NOEXCEPT
    {
        return nn::spy::detail::fnd::AddOffsetToPtr(this, sizeof(DataPacket));
    }

    static size_t GetMaxPayloadLength() NN_NOEXCEPT
    {
        return std::numeric_limits<uint32_t>::max() - sizeof(Body) - nn::spy::detail::fnd::MemoryTraits::DefaultAlignment;
    }
};

//----------------------------------------------------------
//! @ brief データファイル終端パケット
//!
//! 現在のファイルへのデータ書き込みが終了したことを示します。
//! 出力は次のファイルに切り替わります。
//!
struct DataEndPacket : public PacketBase
{
    DataEndPacket() NN_NOEXCEPT
        : PacketBase(PacketId::Type_DataEnd, sizeof(DataEndPacket))
    {}
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
