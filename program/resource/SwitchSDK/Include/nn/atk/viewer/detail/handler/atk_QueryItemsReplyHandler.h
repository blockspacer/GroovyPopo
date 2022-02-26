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

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class HioPacketStream;
class SoundEditConnection;

class QueryItemsReplyHandler : public HioMessageHandler
{
public:
    QueryItemsReplyHandler() NN_NOEXCEPT;
    virtual ~QueryItemsReplyHandler() NN_NOEXCEPT NN_OVERRIDE { }

public:
    void Initialize(SoundEditConnection& connection, int maxItemName) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

protected:
    //---------------------------------------------------------------------------
    //! @brief         処理対象のHIOメッセージを取得します。
    //!
    //! @return        処理対象のHIOメッセージを返します。
    //---------------------------------------------------------------------------
    virtual HioMessageType GetTargetMessage() const NN_NOEXCEPT NN_OVERRIDE { return HioMessageType_QueryItemsReply; }

    //---------------------------------------------------------------------------
    //! @brief         HIOメッセージを処理します。
    //!
    //! @details       パケットを逐次読み込みながら処理する場合は、このメソッドをオーバーライドします。
    //!
    //! @param[in]     stream  パケット送受信用のストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    virtual HioResult OnInvoke(HioPacketStream& stream) NN_NOEXCEPT NN_OVERRIDE;

private:
    HioResult ReadBodyFixedLengthArea(HioPacketStream& stream) NN_NOEXCEPT;
    HioResult ReadItemsOffsets(ResOffsetTableData& items, HioPacketStream& stream) NN_NOEXCEPT;

    size_t ReadItemLength(
        HioPacketStream& stream,
        ResOffsetTableData& itemsData,
        uint32_t index) NN_NOEXCEPT;

private:
    SoundEditConnection* m_Connection;
    size_t               m_ItemSize;
    int             m_ReadingItemIndex;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
