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

namespace nn {
namespace ut {

class IoStream;

} // namespace nn::ut
} // namespace nn

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class HioStream;

//---------------------------------------------------------------------------
//! @brief    HIO パケットストリームクラスです。
//---------------------------------------------------------------------------
class HioPacketStream
{
private:
    enum ReadState
    {
        ReadState_NotRead = 0,    //!< 何も読み込まれていません。
        ReadState_HeaderRead,     //!< ヘッダを読み込み済みです。
        ReadState_BodyReading,    //!< ボディを読み込み中です。
        ReadState_BodyRead,       //!< ボディを読み込み済みです。
        ReadState_Error            //!< エラーが発生しました。
    };

public:
    /* ctor */ HioPacketStream() NN_NOEXCEPT;
    /* dtor */ ~HioPacketStream() NN_NOEXCEPT { }

public:
    //! @brief  初期化します。
    //! @param stream TBD
    //! @param buffer TBD
    //! @param bufferSize TBD
    //! @return TBD
    HioResult Initialize(HioStream& stream, void* buffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief  初期化の有無を取得します。
    //! @return TBD
    bool IsInitialized() const NN_NOEXCEPT { return m_Stream != NULL; }

    //! @brief  ストリームの有効性を取得します。
    //! @return TBD
    bool IsAvailable() const NN_NOEXCEPT;

    //! @brief  読み込み可能なバイト数を取得します。
    //! @return TBD
    size_t GetReadableBytes() const NN_NOEXCEPT;

    //! @brief  バッファを取得します。
    //! @return TBD
    void* GetBuffer() const NN_NOEXCEPT { return m_Buffer; }

    //! @brief  バッファサイズを取得します。
    //! @return TBD
    size_t GetBufferSize() const NN_NOEXCEPT { return m_BufferSize; }

    //! @brief  読み込み済みのヘッダを取得します。
    //! @return TBD
    const HioPacketHeader* GetReadHeader() const NN_NOEXCEPT;

    //! @brief  読み込み中のパケットを取得します。
    //! @return TBD
    HioPacket* GetReadingPacket() const NN_NOEXCEPT;

    //! @brief  読み込み済みのパケットを取得します。
    //! @return TBD
    const HioPacket* GetReadPacket() const NN_NOEXCEPT;

    //! @brief  読み込み済みのボディサイズを取得します。
    //! @return TBD
    size_t GetReadBodySize() const NN_NOEXCEPT { return m_ReadBodySize; }

    //! @brief  新しいパケットを読み込み可能かどうかを取得します。
    //! @return TBD
    bool CanReadNewPacket() const NN_NOEXCEPT;

    //! @brief  パケットボディを読み込み可能かどうかを取得します。
    //! @return TBD
    bool CanReadBody() const NN_NOEXCEPT;

    //! @brief  パケット全体の読み込みを試みます。
    //! @return TBD
    HioResult TryReadPacket() NN_NOEXCEPT;

    //! @brief  パケットヘッダの取得を試みます。
    //! @return TBD
    HioResult TryReadHeader() NN_NOEXCEPT;

    //! @brief  パケットボディの読み込みを試みます。
    //! @return TBD
    HioResult TryReadBody() NN_NOEXCEPT;

    //! @brief  パケットボディの一部の読み込みます。
    //! @param buffer TBD
    //! @param readLength TBD
    //! @return TBD
    HioResult TryReadBodyPart(void* buffer, size_t readLength) NN_NOEXCEPT;

    //! @brief  パケットボディの一部の読み込みます。
    //!         読み込みが完了するまでの間、ブロッキングします。
    //! @param buffer TBD
    //! @param readLength TBD
    //! @return TBD
    HioResult ReadBodyPart(void* buffer, size_t readLength) NN_NOEXCEPT;

    //! @brief  パケットヘッダをスキップします。
    //!         スキップが完了するまでの間、ブロッキングします。
    //! @return TBD
    HioResult SkipHeader() NN_NOEXCEPT;

    //! @brief  パケットボディをスキップします。
    //!         スキップが完了するまでの間、ブロッキングします。
    //! @return TBD
    HioResult SkipBody() NN_NOEXCEPT;

    //! @brief  パケットボディの一部をスキップします。
    //!         スキップが完了するまでの間、ブロッキングします。
    //! @param length TBD
    //! @return TBD
    HioResult SkipBodyPart(size_t length) NN_NOEXCEPT;

    //! @brief  パケットを書き込みます。
    //! @param packet TBD
    //! @return TBD
    HioResult WritePacket(const HioPacket& packet) NN_NOEXCEPT;

    //! @brief  バッファをクリアします。
    void ClearBuffer() NN_NOEXCEPT;

private:
    ReadState         m_ReadState;          //!< 読み込み状態です。
    HioStream*        m_Stream;             //!< HIO ストリームです。
    HioPacket*        m_Buffer;             //!< 受信用 HioPacket のバッファです。
    size_t            m_BufferSize;         //!< 受信用 HioPacket のバッファサイズです。
    size_t            m_ReadBodySize;       //!< 読み込み済みのボディサイズです。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
