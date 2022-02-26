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

#include <nn/atk/fnd/io/atkfnd_HioStream.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief    HIOで使用するストリームクラスです。
//---------------------------------------------------------------------------
class HioStream
{
public:
    typedef nn::atk::detail::fnd::HioStream::ChannelType ChannelType;

public:
    /* ctor */ HioStream() NN_NOEXCEPT;
    /* dtor */ virtual ~HioStream() NN_NOEXCEPT { }

public:
    //! @brief ストリームの初期化を行います。
    //! @param workBuffer TBD
    //! @param workBufferLength TBD
    void Initialize(void* workBuffer, size_t workBufferLength) NN_NOEXCEPT;

    //! @brief ストリームの破棄を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief ストリームをオープンします。
    //! @param channel TBD
    //! @param buffer TBD
    //! @param length TBD
    //! @return TBD
    bool Open(ChannelType channel, void* buffer, size_t length) NN_NOEXCEPT;

    //! @brief ストリームをクローズします。
    void Close() NN_NOEXCEPT;

    //! @brief ストリームがオープンされているかどうかを取得します。
    //! @return TBD
    bool IsAvailable() const NN_NOEXCEPT;

    //! @brief ストリームから読み込み可能なサイズを取得します。
    //! @return TBD
    size_t GetReadableBytes() NN_NOEXCEPT;

    //! @brief 同期リードします。
    //! @param buffer TBD
    //! @param length TBD
    //! @return TBD
    size_t Read(void* buffer, size_t length) NN_NOEXCEPT;

    //! @brief データをスキップします。
    //! @param length TBD
    //! @return TBD
    size_t Skip(size_t length) NN_NOEXCEPT;

    //! @brief 同期ライトします。
    //! @param buffer TBD
    //! @param length TBD
    //! @return TBD
    size_t Write(const void* buffer, size_t length) NN_NOEXCEPT;

    //! @brief  ポーリングします。
    //! @return TBD
    bool Polling() NN_NOEXCEPT;

private:
    nn::atk::detail::fnd::HioStream m_Stream; //!< 送受信用ストリームです。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
