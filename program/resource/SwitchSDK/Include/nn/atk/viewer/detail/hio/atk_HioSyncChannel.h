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

#include <nn/atk/viewer/detail/hio/atk_HioChannel.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class HioManager;

//---------------------------------------------------------------------------
//! @brief    HIO同期チャンネルを示すクラスです。
//---------------------------------------------------------------------------
class HioSyncChannel : public HioChannel
{
public:
    static const uint32_t TimeoutInfinity = UINT_MAX;

public:
    /* ctor */ HioSyncChannel() NN_NOEXCEPT { }
    /* dtor */ virtual ~HioSyncChannel() NN_NOEXCEPT NN_OVERRIDE { }

public:
    //! @brief  非同期チャンネルかどうかを取得します。
    //! @return TBD
    virtual bool IsAsync() const NN_NOEXCEPT NN_OVERRIDE { return false; }

    //! @brief  HIO パケットを同期送信します。
    //! @param replyPacket TBD
    //! @param message TBD
    //! @param timeout TBD
    //! @return TBD
    HioResult Send(const HioPacket*& replyPacket, const HioPacket& message, uint32_t timeout = TimeoutInfinity) NN_NOEXCEPT;

    //! @brief  HIO パケットの受信します。
    //! @param packet TBD
    //! @param timeout TBD
    //! @return TBD
    HioResult Receive(const HioPacket*& packet, uint32_t timeout = TimeoutInfinity) NN_NOEXCEPT;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
