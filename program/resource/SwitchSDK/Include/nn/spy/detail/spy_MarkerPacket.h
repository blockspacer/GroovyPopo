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
class MarkerPacket
{
public:
    static const int MaxDescriptionLength = 255; ///< マーカー値の説明の最大値。

#ifdef NN_BUILD_CONFIG_SPY_ENABLED

public:
    static const uint32_t PacketVersion = NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(1, 1, 0, 0);

    //! @briefprivate
    //! @brief 値パケット
    struct ValuePacketPayload
    {
        IdData id;             ///< アイテムID

        ColorData color;
        StringData<uint8_t, MaxDescriptionLength> description;
    };

    NN_STATIC_ASSERT(sizeof(ValuePacketPayload) == 268);

#endif // NN_BUILD_CONFIG_SPY_ENABLED

};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
