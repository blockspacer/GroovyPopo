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

#include <nn/nn_StaticAssert.h>

#include <nn/spy/detail/fnd/binary/spyfnd_Binary.h>
#include <nn/spy/detail/spy_PacketUtility.h>

namespace nn {
namespace spy {
namespace detail {

//! @briefprivate
class SpyPlotItemPacket
{
public:
    static const int MaxNameLength = 255;     //< プロットアイテム名の最大値。
    static const int MaxStateValueLength = 255;

#ifdef NN_BUILD_CONFIG_SPY_ENABLED

public:
    static const uint32_t PacketVersion = NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(1, 3, 0, 0);

    enum DataType
    {
        DataType_FloatValue = 0,
        DataType_FloatMetadata = 1,
        DataType_Reset = 2,
        DataType_NodeMetadata = 3,
        DataType_NodeDetach = 4,
        DataType_StateMetadata = 5,
        DataType_StateValue = 6,
    };

    //! @briefprivate
    //! @brief パケット共通データ
    struct PacketCommonData
    {
        uint8_t  dataType;   ///< パケットデータタイプ
        uint8_t  padding[3];
    };

    struct ItemMetaData
    {
        IdData id;
        IdData parentId;
        ColorData color;
        StringData<uint8_t, MaxNameLength> name;
    };

    NN_STATIC_ASSERT(sizeof(ItemMetaData) == 276);

    struct FloatMetadataPacketPayload
    {
        PacketCommonData common;

        DoubleData minValue;
        DoubleData maxValue;

        uint8_t interpolationMode;
        uint8_t padding[3];

        ItemMetaData itemMetaData;
    };

    NN_STATIC_ASSERT(sizeof(FloatMetadataPacketPayload) == 300);

    struct FloatValuePacketPayload
    {
        PacketCommonData common;

        IdData id;
        DoubleData value;
    };

    NN_STATIC_ASSERT(sizeof(FloatValuePacketPayload) == 20);

    struct ResetPacketPayload
    {
        PacketCommonData common;
    };

    NN_STATIC_ASSERT(sizeof(ResetPacketPayload) == 4);

    struct NodeMetadataPacketPayload
    {
        PacketCommonData common;

        ItemMetaData itemMetaData;
    };

    NN_STATIC_ASSERT(sizeof(NodeMetadataPacketPayload) == 280);

    struct NodeDetachPacketPayload
    {
        PacketCommonData common;

        IdData id;
    };

    NN_STATIC_ASSERT(sizeof(NodeDetachPacketPayload) == 12);

    struct StateMetadataPacketPayload
    {
        PacketCommonData common;

        ItemMetaData itemMetaData;
    };

    NN_STATIC_ASSERT(sizeof(StateMetadataPacketPayload) == 280);

    struct StateValuePacketPayload
    {
        PacketCommonData common;

        IdData id;

        ColorData color;

        StringData<uint8_t, MaxStateValueLength> state;
    };

    NN_STATIC_ASSERT(sizeof(StateValuePacketPayload) == 272);

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
