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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/viewer/detail/hio/atk_HioChannel.h>
#include <nn/atk/viewer/detail/hio/atk_HioMessageHandler.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class HioManager;

//---------------------------------------------------------------------------
//! @brief    HIO非同期チャンネルを示すクラスです。
//!
//! @details  複数のHIOメッセージハンドラを管理するとともに、
//!           指定チャンネルに流れてきたパケットを適合したハンドラに処理を振り分けます。
//---------------------------------------------------------------------------
class HioAsyncChannel : public HioChannel
{
private:
    typedef util::IntrusiveList<HioMessageHandler::Reference, util::IntrusiveListMemberNodeTraits<HioMessageHandler::Reference, &HioMessageHandler::Reference::node>> HioMessageHandlerRefList;

public:
    /* ctor */ HioAsyncChannel() NN_NOEXCEPT { }
    /* dtor */ virtual ~HioAsyncChannel() NN_NOEXCEPT NN_OVERRIDE { }

public:
    //! @brief  非同期チャンネルかどうかを取得します。
    //! @return true を返します。
    virtual bool IsAsync() const NN_NOEXCEPT NN_OVERRIDE { return true; }

    //! @brief  HIO メッセージハンドラを登録します。
    //! @param[in] handler HIO メッセージハンドラです。
    void RegisterMessageHandler(HioMessageHandler& handler) NN_NOEXCEPT;

    //! @brief  HIO メッセージハンドラの登録を削除します。
    //! @param[in] handler HIO メッセージハンドラです。
    void UnregisterMessageHandler(HioMessageHandler& handler) NN_NOEXCEPT;

    //! @brief  HIO チャンネルを更新します。
    //! @return 結果を返します。
    HioResult Update() NN_NOEXCEPT;

    //! @brief  HIO パケットを非同期送信します。
    //! @param[in] message 送信するパケットです。
    //! @return 結果を返します。
    HioResult Send(const HioPacket& message) NN_NOEXCEPT;

private:
    //! @brief  指定HIOメッセージヘッダに適合するハンドラを取得します。
    HioMessageHandler* GetMessageHandler(const HioPacketHeader& header) const NN_NOEXCEPT;

private:
    HioMessageHandlerRefList m_MessageHandlerRefs;  //!< HIOメッセージハンドラ参照リストです。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
