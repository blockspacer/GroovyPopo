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
#include <nn/atk/viewer/detail/hio/atk_HioProtocol.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class HioChannel;
class HioPacketStream;

//---------------------------------------------------------------------------
//! @brief    HIOメッセージを処理するクラスです。
//!
//! @details  GetTargetMessageID() が返すHIOメッセージを対象に
//!           OnInvoke にてメッセージ処理します。
//---------------------------------------------------------------------------
class HioMessageHandler
{
public:
    //! @brief LinkList 用の参照クラスです。
    struct Reference
    {
        HioMessageHandler* value;   //!< HioMessageHandler です。
        util::IntrusiveListNode node;      //!< IntrusiveListNode です。
    };

public:
    HioMessageHandler() NN_NOEXCEPT
    {
        m_Reference.value = this;
    }

    virtual ~HioMessageHandler() NN_NOEXCEPT { }

public:
    //---------------------------------------------------------------------------
    //! @brief         HIOメッセージを処理します。
    //!
    //! @param[in]     stream  パケット送受信用のストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    HioResult Invoke(HioPacketStream& stream) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         HIOメッセージを処理可能かどうかを調べます。
    //!
    //! @param[in]     header  パケットヘッダを指定します。
    //!
    //! @return        HIOメッセージを処理可能な場合は true、不可能な場合は false を返します。
    //---------------------------------------------------------------------------
    virtual bool CanRespond(const HioPacketHeader& header) const NN_NOEXCEPT
    {
        return header.message == GetTargetMessage();
    }

    //---------------------------------------------------------------------------
    //! @brief         LinkList 用の参照を取得します。
    //!
    //! @details       この関数の戻り値である HioMessageHandler::Reference を利用して、
    //!                HioMessageHandler を格納する LinkList を構築できます。
    //!
    //! @return        HioMessageHandler::Reference を返します。
    //---------------------------------------------------------------------------
    Reference& GetReference() NN_NOEXCEPT { return m_Reference; }

protected:
    //---------------------------------------------------------------------------
    //! @brief         処理対象のHIOメッセージを取得します。
    //!
    //! @return        処理対象のHIOメッセージを返します。
    //---------------------------------------------------------------------------
    virtual HioMessageType GetTargetMessage() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief         HIOメッセージを処理します。
    //!
    //! @details       パケットを逐次読み込みながら処理する場合は、このメソッドをオーバーライドします。
    //!
    //! @param[in]     stream  パケット送受信用のストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    virtual HioResult OnInvoke(HioPacketStream& stream) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         読み込み済みのHIOメッセージを処理します。
    //!
    //! @details       一括読み込みされたパケットを処理する場合は、このメソッドをオーバーライドします。
    //!
    //! @param[in]     packet  パケットを指定します。
    //! @param[in]     stream  パケット返信用のストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    virtual HioResult OnInvokeForReadPacket(const HioPacket& packet, HioPacketStream& stream) NN_NOEXCEPT;

private:
    Reference    m_Reference;       //!< LinkList 用の参照です。
};

//---------------------------------------------------------------------------
//! @brief    HIOメッセージを処理するクラスです。
//---------------------------------------------------------------------------
template <HioMessageType TMessageType, class TPacket>
class HioMessageHandlerImpl : public HioMessageHandler
{
protected:
    //---------------------------------------------------------------------------
    //! @brief         処理対象のHIOメッセージを取得します。
    //!
    //! @return        処理対象のHIOメッセージを返します。
    //---------------------------------------------------------------------------
    virtual HioMessageType GetTargetMessage() const NN_NOEXCEPT NN_OVERRIDE { return TMessageType; }

    //---------------------------------------------------------------------------
    //! @brief         読み込み済みのHIOメッセージを処理します。
    //!
    //! @details       一括読み込みされたパケットを処理する場合は、このメソッドをオーバーライドします。
    //!
    //! @param[in]     packet  HIO パケットを指定します。
    //! @param[in]     stream  パケット返信用のストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    virtual HioResult OnInvokeForReadPacket(const HioPacket& packet, HioPacketStream& stream) NN_NOEXCEPT NN_OVERRIDE
    {
        return OnInvokeForReadTargetPacket(*reinterpret_cast<const TPacket*>(&packet), stream);
    }

    //---------------------------------------------------------------------------
    //! @brief         読み込み済みの対象HIOメッセージを処理します。
    //!
    //! @param[in]     packet  HIO パケットを指定します。
    //! @param[in]     stream  パケット返信用のストリームを指定します。
    //!
    //! @return        HIOメッセージの処理結果を返します。
    //---------------------------------------------------------------------------
    virtual HioResult OnInvokeForReadTargetPacket(
        const TPacket& packet,
        HioPacketStream& stream) NN_NOEXCEPT = 0;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
