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
#include <nn/atk/viewer/detail/hio/atk_HioStream.h>
#include <nn/atk/viewer/detail/hio/atk_HioPacketStream.h>
#include <nn/atk/viewer/detail/hio/atk_HioMessageHandler.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class HioManager;

//---------------------------------------------------------------------------
//! @brief    HIOチャンネルを示す基本クラスです。
//---------------------------------------------------------------------------
class HioChannel
{
public:
    static const size_t DefaultWorkBufferLength =  1 * 1024;   //!< HIO通信で使用する作業バッファの大きさのデフォルト値です。基本的に変更する必要はありません。

private:
    typedef util::IntrusiveList<HioMessageHandler::Reference, util::IntrusiveListMemberNodeTraits<HioMessageHandler::Reference, &HioMessageHandler::Reference::node>> HioMessageHandlerRefList;

public:
    //! @brief LinkList 用の参照クラスです。
    struct Reference
    {
        HioChannel* value;      //!< HioChannel です。
        util::IntrusiveListNode node;  //!< IntrusiveListNode です。
    };

    //! @brief HioManager と HioChannel の関連付け用クラスです。
    class HioManagerLinker
    {
    private:
        static void SetHioManager(HioChannel& channel, HioManager* hioManager) NN_NOEXCEPT
        {
            channel.m_HioManager = hioManager;
        }

        friend class HioManager;
    };

protected:
    /* ctor */ HioChannel() NN_NOEXCEPT;
    /* dtor */ virtual ~HioChannel() NN_NOEXCEPT;

public:
    //! @brief  初期化します。
    //! @param[in] recvStreamBuffer 受信ストリームバッファです。
    //! @param[in] recvStreamBufferLength 受信ストリームバッファのサイズです。
    //! @param[in] recvPacketBuffer 受信パケットバッファです。
    //! @param[in] recvPacketBufferLength 受信パケットバッファのサイズです。
    //! @return 結果を返します。
    //! @param[in] workBuffer 作業バッファです。
    //! @param[in] workBufferLength 作業バッファのサイズです。通常は DefaultWorkBufferLength を指定します。
    HioResult Initialize(
        void* recvStreamBuffer,
        size_t recvStreamBufferLength,
        void* recvPacketBuffer,
        size_t recvPacketBufferLength,
        void* workBuffer,
        size_t workBufferLength
    ) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief  初期化の有無を取得します。
    //! @return 初期化済みであれば true を返します。
    bool IsInitialized() const NN_NOEXCEPT { return m_RecvStreamBufferLength > 0; }

    //! @brief  チャンネルが開かれているかどうかを取得します。
    //! @return チャンネルが開かれていれば true を返します。
    bool IsOpened() const NN_NOEXCEPT { return m_Stream.IsAvailable(); }

    //! @brief  非同期チャンネルかどうかを取得します。
    //! @return 非同期チャンネルであれば true を返します。
    virtual bool IsAsync() const NN_NOEXCEPT = 0;

    //! @brief  指定チャンネルを開きます。
    //! @param[in] channel チャンネルの指定です。
    //! @return 開くことができた場合は true を返します。
    bool Open(HioStream::ChannelType channel) NN_NOEXCEPT;

    //! @brief  開かれているチャンネルを閉じます。
    void Close() NN_NOEXCEPT;

    //! @brief  ストリームを取得します。
    //! @return ストリームへの参照を返します。
    HioPacketStream& GetStream() NN_NOEXCEPT { return m_PacketStream; }

    //! @brief  バッファをクリアします。
    void ClearBuffer() NN_NOEXCEPT;

    //! @brief  現在のメモリ使用量を取得します。
    //! @return 現在のメモリ使用量を返します。
    size_t GetMemoryUsage() const NN_NOEXCEPT
    {
        return m_RecvStreamBufferLength + m_RecvPacketBufferLength;
    }

    //! @brief  ポーリングします。
    //! @return チャンネルが切断された時は false を返します。
    bool Polling() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         LinkList 用の参照を取得します。
    //!
    //! @details       この関数の戻り値である HioChannel::Reference を利用して、
    //!                HioChannel を格納する LinkList を構築できます。
    //!
    //! @return        HioChannel::Reference を返します。
    //---------------------------------------------------------------------------
    Reference& GetReference() NN_NOEXCEPT { return m_Reference; }

protected:
    HioManager* GetHioManager() NN_NOEXCEPT { return m_HioManager; }

    HioPacket* GetRecvPacketBuffer() NN_NOEXCEPT { return m_RecvPacketBuffer; }

    size_t GetRecvPacketBufferLength() const NN_NOEXCEPT { return m_RecvPacketBufferLength; }

private:
    HioStream        m_Stream;              //!< 送受信用ストリームです。
    HioPacketStream  m_PacketStream;        //!< パケット送受信用ストリームです。

    void* m_RecvStreamBuffer;               //!< 受信用ストリームのバッファです。
    size_t   m_RecvStreamBufferLength;         //!< 受信用ストリームのバッファサイズです。

    HioPacket* m_RecvPacketBuffer;          //!< 受信用 HioPacket のバッファです。
    size_t        m_RecvPacketBufferLength;    //!< 受信用 HioPacket のバッファサイズです。

    HioManager* m_HioManager;               //!< 関連する HioManager です。
    Reference   m_Reference;                //!< LinkList 用の参照です。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
