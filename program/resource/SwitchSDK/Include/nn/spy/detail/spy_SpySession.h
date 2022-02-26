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

#include <atomic>

#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection.h>
#include <nn/spy/detail/fnd/os/spyfnd_Thread.h>
#include <nn/spy/detail/fnd/hio/spyfnd_HioChannel.h>
#include <nn/spy/detail/spy_SpyPacketReader.h>
#include <nn/spy/spy_SpyDataInfo.h>

namespace nn {
namespace spy {
namespace detail {

struct PacketHeader;

//! @briefprivate
//! @brief Spy セッションを制御します。
//!
//! @details
//! すべて同期関数です。
class SpySession
{
public: // 型
    //! @brief 状態変更コールバック関数です。
    //! @param[in]  param  任意のパラメータを指定します。
    typedef void (*StateChangedCallback)(void* param);

    //! @brief メッセージハンドラです。
    class IMessageHandler
    {
    public:
        virtual void OnInitializeSession() NN_NOEXCEPT = 0;
        virtual void OnFinalizeSession() NN_NOEXCEPT = 0;

        virtual void OnSelectDataId(int flagsCount, uint32_t flags[]) NN_NOEXCEPT = 0;
        virtual void OnSetOutputDirPath(const char* outputDir) NN_NOEXCEPT = 0;
        virtual void OnDataRead(int fileIndex) NN_NOEXCEPT = 0;

        //! データタイプを問い合わせます。
        virtual const SpyDataInfo* OnQueryDataInfo() NN_NOEXCEPT = 0;
    };

private: // 型
    //! @brief セッションの状態です。
    enum State
    {
        State_NotInitialized,
        State_Initialized,
        State_Opening,
        State_Opened,
        State_Disconnecting,
        State_Connecting,
        State_Preparing,
        State_Prepared
    };

public: // コンストラクタ
    SpySession() NN_NOEXCEPT;

public: // メソッド
    void Initialize(void* buffer, size_t bufferLength) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize() NN_NOEXCEPT;

    void SetStateChangedCallback(StateChangedCallback callback, void* param) NN_NOEXCEPT
    {
        m_StateChangedCallback = callback;
        m_StateChangedCallbackParam = param;
    }

    bool Open(const char* syncPort, const char* dataPort) NN_NOEXCEPT;
    void Close() NN_NOEXCEPT;

    bool Connect() NN_NOEXCEPT;
    void Disconnect() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT { return m_State > State_NotInitialized; }
    bool IsOpened() const NN_NOEXCEPT { return m_State > State_Opening; }
    bool IsConnecting() const NN_NOEXCEPT { return m_State == State_Connecting; }
    bool IsConnected() const NN_NOEXCEPT { return m_State >= State_Preparing; }
    bool IsPreparing() const NN_NOEXCEPT { return m_State == State_Preparing; }
    bool IsPrepared() const NN_NOEXCEPT { return m_State >= State_Prepared; }

    //! @brief 同期チャンネルのパケットを受信して処理します。
    //!        通信のレスポンスを上げるには、できるだけ頻繁に呼び出してください。
    bool ProcessSyncPacket(IMessageHandler* msgHandler) NN_NOEXCEPT;

    size_t SendData(const void* buffer, size_t length) NN_NOEXCEPT;

private: // メソッド
    void SetState(State value) NN_NOEXCEPT;

    bool OpenSyncChannel(const char* port) NN_NOEXCEPT;
    void CloseSyncChannel() NN_NOEXCEPT;

    void* GetPacketBodyBuffer() NN_NOEXCEPT;
    size_t GetPacketBodyBufferSize() NN_NOEXCEPT { return m_SyncChannelBufferLength - sizeof(PacketHeader); }

    bool OnInitialize(IMessageHandler* msgHandler) NN_NOEXCEPT;
    bool OnFinalize(IMessageHandler* msgHandler) NN_NOEXCEPT;
    bool OnSelectDataId(IMessageHandler* msgHandler) NN_NOEXCEPT;
    bool OnSetOutputDir(IMessageHandler* msgHandler) NN_NOEXCEPT;
    bool OnNotifyDataRead(IMessageHandler* msgHandler) NN_NOEXCEPT;
    bool OnPing() NN_NOEXCEPT;
    bool OnQueryDataInfo(IMessageHandler* msgHandler) NN_NOEXCEPT;

    static const char* StateToString(State value) NN_NOEXCEPT;

private: // メンバ変数
    std::atomic<State>   m_State;
    StateChangedCallback m_StateChangedCallback;
    void*                m_StateChangedCallbackParam;

    nn::spy::detail::fnd::CriticalSection m_StateLock;

    nn::spy::detail::fnd::HioChannel m_SyncChannel;
    nn::spy::detail::fnd::HioChannel m_DataChannel;

    detail::SpyPacketReader m_SyncPacketReader;

    void* m_SyncChannelBuffer;          //!< SyncChannel のパケット送受信に利用します。
    size_t m_SyncChannelBufferLength;
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
