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
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/hio.h>
#include <nw/snd/spy/fnd/os/sndspyfnd_Event.h>
#else
#include <nn/htcs.h>
#include <nn/ige/ige_HtcsHelper.h>
#endif

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @brief    HIO チャンネルを制御します。
//---------------------------------------------------------------------------
class HioChannel
{
public: // 型の定義
public: // 定数の定義
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    //! @brief ポートの最大長です。
    static const uint32_t MaxPortLength = 16;
#else
    //! @brief ポートの最大長です。
    static const int MaxPortLength = nn::htcs::PortNameBufferLength;
#endif

public: // コンストラクタ
    HioChannel() NN_NOEXCEPT;

public:
    //! @brief チャンネルを開きます。
    bool Open(const char* port) NN_NOEXCEPT;

    //! @brief チャンネルを閉じます。
    void Close() NN_NOEXCEPT;

    //! @brief 接続します。
    bool Connect() NN_NOEXCEPT;

    //! @brief 切断します。
    void Disconnect() NN_NOEXCEPT;

    //! @brief チャンネルが開かれているかどうかを調べます。
    bool IsOpened() const NN_NOEXCEPT;

    //! @brief チャンネルが接続されているかどうかを調べます。
    bool IsConnected() const NN_NOEXCEPT;

    //! @brief ポートを取得します。
    const char* GetPort() const NN_NOEXCEPT;

    //! @brief チャンネルからデータを読み込みます。
    //! @return 読み込んだバイト数を返します。エラーの場合は-1を返します。
    size_t Read(void* buffer, size_t length) NN_NOEXCEPT;

    //! @brief チャンネルにデータを書き込みます。
    size_t Write(const void* buffer, size_t length) NN_NOEXCEPT;

private:
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    static void ConnectionCallback(HIOStatus status, void* context) NN_NOEXCEPT;
#endif

private:
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    char      m_Port[MaxPortLength];
    HIOHandle m_Handle;
    bool      m_IsConnected;

    Event m_ConnectEvent;
#else
    char m_Port[MaxPortLength];
    int m_Socket;
    int m_ClientSocket;
#endif
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
