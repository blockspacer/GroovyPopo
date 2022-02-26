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
#include <nn/atk/viewer/detail/hio/atk_HioMessageHandler.h>
#include <nn/atk/viewer/detail/protocol/atk_SyncPacket.h>

namespace nn {
namespace atk {
namespace viewer {

class SoundEditSession;

namespace detail {

class HioPacketStream;

class SyncReplyHandler : public HioMessageHandlerImpl<HioMessageType_SyncReply, SyncReplyPacket>
{
public:
    SyncReplyHandler() NN_NOEXCEPT { }
    virtual ~SyncReplyHandler() NN_NOEXCEPT NN_OVERRIDE { }

public:
    void Initialize(SoundEditSession& session) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

protected:
    //---------------------------------------------------------------------------
    //! @brief         読み込み済みの対象HIOメッセージを処理します。
    //!
    //! @param[in]     packet  HIO パケットを指定します。
    //! @param[in]     stream  返信用の出力ストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    virtual HioResult OnInvokeForReadTargetPacket(
        const SyncReplyPacket& packet,
        HioPacketStream& stream) NN_NOEXCEPT NN_OVERRIDE;

private:
    SoundEditSession* m_Session;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
