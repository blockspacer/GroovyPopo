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

#include <atomic>

#include <nn/spy/spy_Config.h>

#include <nn/os/os_Tick.h>

#include <nn/spy/spy_DebugModule.h>
#include <nn/spy/spy_LogModule.h>
#include <nn/spy/spy_MarkerModule.h>
#include <nn/spy/spy_PlotModule.h>
#include <nn/spy/spy_SpyDataInfo.h>
#include <nn/spy/spy_TimeModule.h>

#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#include <nn/spy/detail/fnd/basis/spyfnd_Memory.h>
#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection.h>
#include <nn/spy/detail/fnd/os/spyfnd_Event.h>
#include <nn/spy/detail/fnd/os/spyfnd_Thread.h>
#include <nn/spy/detail/spy_SpyDataFileChannel.h>
#include <nn/spy/detail/spy_SpySession.h>

#endif

namespace nn {
namespace spy {

namespace detail {
struct DataPacket;
} // nn::spy::detail

class SpyModule;

//! @brief Spy セッションと各モジュールを制御します。
class SpyController
{
public:
    //! @brief  Initialize() のパラメータです。
    //!
    //! @sa Initialize
    struct InitializeArg
    {
        size_t dataBufferLength;    //!< SpyController の通信バッファサイズです。
                                    //!< ダブルバッファ化されるため、指定した数値の2倍以上を要求します。
        size_t syncThreadStackSize; //!< SYNC スレッドのスタックサイズです。
        size_t dataThreadStackSize; //!< DATA スレッドのスタックサイズです。

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)
        NN_FORCEINLINE InitializeArg() NN_NOEXCEPT { }
#else
        //! @brief  コンストラクタです。
        InitializeArg() NN_NOEXCEPT;
#endif

    };

    //! @brief Open() のパラメータです。
    //!
    //! @sa Open
    struct OpenArg
    {
        int syncThreadPriority; //!< SYNC 通信スレッドのプライオリティです。
        int dataThreadPriority; //!< DATA 通信スレッドのプライオリティです。

#if defined(NN_SPY_DATA_FILE_CHANNEL_AVAILABLE)
        SpyDataFileChannelParam dataFileChannelParam; //!< HostFileIOを使ったデータ通信のためのパラメータです。
#endif

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)
        NN_FORCEINLINE OpenArg() NN_NOEXCEPT { }
#else
        //! @brief  コンストラクタです。
        OpenArg() NN_NOEXCEPT;
#endif

    };

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE SpyController() NN_NOEXCEPT {}

    NN_FORCEINLINE static size_t GetRequiredMemorySize(const InitializeArg& arg) NN_NOEXCEPT
    {
        NN_UNUSED(&arg);
        return 0;
    }

    NN_FORCEINLINE void Initialize(const InitializeArg& arg, void* buffer, size_t bufferLength) NN_NOEXCEPT
    {
        NN_UNUSED(&arg);
        NN_UNUSED(buffer);
        NN_UNUSED(bufferLength);
    }

    NN_FORCEINLINE void Finalize() NN_NOEXCEPT {}

    NN_FORCEINLINE bool Open(const OpenArg& arg) NN_NOEXCEPT
    {
        NN_UNUSED(&arg);
        return true;
    }

    NN_FORCEINLINE void Close() NN_NOEXCEPT {}

    NN_FORCEINLINE bool IsInitialized() const NN_NOEXCEPT { return false; }

    NN_FORCEINLINE bool IsOpened() const NN_NOEXCEPT { return false; }

    NN_FORCEINLINE bool IsConnecting() const NN_NOEXCEPT { return false; }

    NN_FORCEINLINE bool IsConnected() const NN_NOEXCEPT { return false; }

    NN_FORCEINLINE int GetCurrentApplicationFrame() const NN_NOEXCEPT { return 0; }

    NN_FORCEINLINE void SetCurrentApplicationFrame(int value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE int GetCurrentAudioFrame() const NN_NOEXCEPT { return 0; }

    NN_FORCEINLINE void SetCurrentAudioFrame(int value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool RegisterModule(SpyModule& module) NN_NOEXCEPT
    {
        NN_UNUSED(&module);
        return false;
    }

    NN_FORCEINLINE void UnregisterModule(SpyModule& module) NN_NOEXCEPT
    {
        NN_UNUSED(&module);
    }

#if defined (NN_BUILD_CONFIG_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"
#endif
    NN_FORCEINLINE DebugModule& GetDebugModule() NN_NOEXCEPT
    {
        return *reinterpret_cast<DebugModule*>(NULL);
    }

    NN_FORCEINLINE LogModule& GetLogModule() NN_NOEXCEPT
    {
        return *reinterpret_cast<LogModule*>(NULL);
    }

    NN_FORCEINLINE MarkerModule& GetMarkerModule() NN_NOEXCEPT
    {
        return *reinterpret_cast<MarkerModule*>(NULL);
    }

    NN_FORCEINLINE PlotModule& GetPlotModule() NN_NOEXCEPT
    {
        return *reinterpret_cast<PlotModule*>(NULL);
    }
#if defined (NN_BUILD_CONFIG_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif

#else // NN_BUILD_CONFIG_SPY_ENABLED

private:
    //! @brief コントローラの状態です。
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

    //! @briefprivate
    struct DataBuffer
    {
        void* address;
        size_t length;
        ptrdiff_t currentPosition;

        DataBuffer() NN_NOEXCEPT : address(NULL), length(0), currentPosition(0) { }
    };

    //! @briefprivate
    //! @brief Spy セッションのメッセージハンドラアダプタです。
    class SessionMessageHandlerAdaptor : public detail::SpySession::IMessageHandler
    {
    public:
        void Initialize(SpyController* pOwner) NN_NOEXCEPT
        {
            m_pOwner = pOwner;
        }

        virtual void OnInitializeSession() NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pOwner);
        }

        virtual void OnFinalizeSession() NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pOwner);
#if defined(NN_SPY_DATA_FILE_CHANNEL_AVAILABLE)
            m_pOwner->m_SpyDataFileChannel.EndSession();
#endif
        }

        virtual void OnSelectDataId(int flagsCount, uint32_t flags[]) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pOwner);
            m_pOwner->SelectDataIds(flagsCount, flags);
        }

        virtual void OnSetOutputDirPath(const char* outputDir) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pOwner);
#if defined(NN_SPY_DATA_FILE_CHANNEL_AVAILABLE)
            m_pOwner->m_SpyDataFileChannel.BeginSession(outputDir);
            m_pOwner->SendSetOutputDirPathReplyPacket(m_pOwner->m_SpyDataFileChannel.IsActive());
#else
            NN_UNUSED(outputDir);
            m_pOwner->SendSetOutputDirPathReplyPacket(false);
#endif
        }

        virtual void OnDataRead(int fileIndex) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pOwner);
#if defined(NN_SPY_DATA_FILE_CHANNEL_AVAILABLE)
            m_pOwner->m_SpyDataFileChannel.SetCurrentReadFile(fileIndex);
#else
            NN_UNUSED(fileIndex);
#endif
        }

        virtual const SpyDataInfo* OnQueryDataInfo() NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_pOwner);
            return m_pOwner->QueryDataInfo();
        }

    private:
        SpyController* m_pOwner;
    };

public:
    //! @brief  コンストラクタです。
    SpyController() NN_NOEXCEPT;

public:
    //! @brief  SpyController を初期化します。
    //!
    //! @param[in]  arg           SpyController の初期化パラメータ構造体を指定します。
    //! @param[in]  buffer        SpyController が利用するバッファへのポインタを指定します。
    //! @param[in]  bufferLength  バッファサイズを指定します。
    //!
    //! @details
    //! 初期化に必要なバッファサイズは GetRequiredMemorySize() で取得することができます。
    //!
    //! @sa GetRequiredMemorySize
    //! @sa Finalize
    //! @sa InitializeArg
    void Initialize(const InitializeArg& arg, void* buffer, size_t bufferLength) NN_NOEXCEPT;

    //! @brief  SpyController を破棄できるようにします。
    //!
    //! @sa Initialize
    void Finalize() NN_NOEXCEPT;

    //! @brief  このクラスを利用するのに必要なメモリサイズを取得します。
    //!
    //! @param[in]  arg  初期化パラメータ構造体を指定します。
    //!
    //! @return  このクラスを利用するのに必要なメモリサイズを返します。
    //!
    //! @sa Initialize
    static size_t GetRequiredMemorySize(const InitializeArg& arg) NN_NOEXCEPT;

    //! @brief  Spy 通信セッションを開きます。
    //!
    //! @param[in]  arg  Open() パラメータ構造体を指定します。
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    bool Open(const OpenArg& arg) NN_NOEXCEPT;

    //! @brief  Spy 通信セッションを閉じます。
    void Close() NN_NOEXCEPT;

    //! @brief  初期化の有無を取得します。
    //!
    //! @return  初期化されている場合は true、されていない場合は false を返します。
    bool IsInitialized() const NN_NOEXCEPT { return m_State > State_NotInitialized; }

    //! @brief  Spy 通信セッションが開かれているかどうかを取得します。
    //!
    //! @return  セッションが開かれている場合は true、されていない場合は false を返します。
    bool IsOpened() const NN_NOEXCEPT { return m_State > State_Opening; }

    //! @brief  Spy.exe と接続処理の途中かどうかを取得します。
    //!
    //! @return  接続処理途中の場合は true、されていない場合は false を返します。
    bool IsConnecting() const NN_NOEXCEPT { return State_Connecting <= m_State && !IsPrepared(); }

    //! @brief  Spy.exe と接続済みどうかを取得します。
    //!
    //! @return  接続済みの場合は true、されていない場合は false を返します。
    bool IsConnected() const NN_NOEXCEPT { return IsPrepared(); }

    //! @brief  現在のアプリケーションフレームを取得します。
    //!
    //! @return  現在のアプリケーションフレームを返します。
    //!
    //! @details
    //! SetCurrentApplicationFrame() で設定した値が返ります。
    //!
    //! @sa SetCurrentApplicationFrame
    int GetCurrentApplicationFrame() const NN_NOEXCEPT
    {
        return m_TimeModule.GetCurrentApplicationFrame();
    }

    //! @brief  現在のアプリケーションフレームを設定します。
    //!
    //! @param[in]  value  現在のアプリケーションフレーム値を指定します。
    //!
    //! @details
    //! nn::spy が送信するデータには、この関数で設定したアプリケーションフレームが付加されます。
    //! アプリケーションフレームを設定しておくと、Spy.exe のタイムラインにてアプリケーションフレーム単位のグリッド表示が可能になります。
    //! この関数は、できるだけアプリケーションフレーム処理の先頭で呼び出すようにしてください。
    //!
    //! @sa GetCurrentApplicationFrame
    void SetCurrentApplicationFrame(int value) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  現在のオーディオフレームを取得します。（この関数は使用しないでください。）
    //!
    //! @return  現在のオーディオフレームを返します。
    //!
    //! @details
    //! この関数は、互換性維持のために残されています。
    //! アプリケーションからは呼び出さないでください。
    int GetCurrentAudioFrame() const NN_NOEXCEPT
    {
        return m_TimeModule.GetCurrentAudioFrame();
    }

    //! @briefprivate
    //! @brief 現在のオーディオフレームを設定します。（この関数は使用しないでください。）
    //!
    //! @param[in]  value  現在のオーディオフレーム値を指定します。
    //!
    //! @details
    //! この関数は、互換性維持のために残されています。
    //! アプリケーションからは呼び出さないでください。
    void SetCurrentAudioFrame(int value) NN_NOEXCEPT;

    //! @brief  Spy モジュールを登録します。
    //!
    //! @details
    //! モジュールの登録は Initialize() と Open() の間に行います。
    //!
    //! @param[in]  module  Spy モジュールです。
    //!
    //! @returns  登録に成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - Initialize() は呼ばれた。
    //! - Finalize() は呼ばれていない。
    //! - Open() は呼ばれていない、または Open() が呼ばれた後に Close() が呼ばれた。
    bool RegisterModule(SpyModule& module) NN_NOEXCEPT;

    //! @brief  Spy モジュールの登録を解除します。
    //!
    //! @details
    //! モジュールの登録の解除は Close() と Finalize() の間に行います。
    //!
    //! @param[in]  module  Spy モジュールです。
    //!
    //! @pre
    //! - Initialize() は呼ばれた。
    //! - Finalize() は呼ばれていない。
    //! - Open() は呼ばれていない、または Open() が呼ばれた後に Close() が呼ばれた。
    void UnregisterModule(SpyModule& module) NN_NOEXCEPT;

    //! @brief  nn::spy 自身のデバッグ用モジュールを取得します。
    //!
    //! @returns  デバッグモジュールを返します。
    DebugModule& GetDebugModule() NN_NOEXCEPT
    {
        return m_DebugModule;
    }

    //! @brief  ログモジュールを取得します。
    //!
    //! @returns  ログモジュールを返します。
    LogModule& GetLogModule() NN_NOEXCEPT
    {
        return m_LogModule;
    }

    //! @briefprivate
    //! @brief (調整中の機能です)
    MarkerModule& GetMarkerModule() NN_NOEXCEPT
    {
        return m_MarkerModule;
    }

    //! @briefprivate
    //! @brief (調整中の機能です)
    PlotModule& GetPlotModule() NN_NOEXCEPT
    {
        return m_PlotModule;
    }

private:
    static void SessionStateChangedCallback(void* param) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(param);
        reinterpret_cast<SpyController*>(param)->OnSessionStateChanged();
    }

    bool PushDataAt(
        SpyModule& module,
        const void* buffer,
        size_t length,
        nn::os::Tick tick) NN_NOEXCEPT;

    bool PushDataAt(
        SpyModule& module,
        const void* const buffers[],
        const size_t lengths[],
        int count,
        nn::os::Tick tick) NN_NOEXCEPT;

    void ClearDataBuffer() NN_NOEXCEPT;

    void OnSessionStateChanged() NN_NOEXCEPT;

    void ResetSpyFrameBase() NN_NOEXCEPT;

    bool StartSyncThread(int priority) NN_NOEXCEPT;
    bool StartDataThread(int priority) NN_NOEXCEPT;

    void SyncThreadMain(void* param) NN_NOEXCEPT;
    void DataThreadMain(void* param) NN_NOEXCEPT;

    bool IsOpening() const NN_NOEXCEPT { return m_State == State_Opening; }
    bool IsPreparing() const NN_NOEXCEPT { return m_State == State_Preparing; }
    bool IsPrepared() const NN_NOEXCEPT { return m_State >= State_Prepared; }

    void SetState(State value) NN_NOEXCEPT;

    void EstablishConnection() NN_NOEXCEPT;

    void DismissConnection() NN_NOEXCEPT;

    bool PushDataPacketAt(
        nn::spy::detail::SpyDataId dataId,
        const void* buffer,
        size_t length,
        nn::os::Tick tick) NN_NOEXCEPT;

    bool PushDataPacketAt(
        nn::spy::detail::SpyDataId dataId,
        const void* const buffers[],
        const size_t lengths[],
        int count,
        nn::os::Tick tick) NN_NOEXCEPT;

    detail::DataPacket* AllocateDataPacketBuffer(size_t payloadLength) NN_NOEXCEPT;
    void SendDataPacket() NN_NOEXCEPT;
    bool SendVersionPacket() NN_NOEXCEPT;
    bool SendSetOutputDirPathReplyPacket(bool active) NN_NOEXCEPT;
    void SwapBuffer() NN_NOEXCEPT;
    size_t GetDataBufferUsageMax() NN_NOEXCEPT;

    const DataBuffer& GetCurrentDataBuffer() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentDataBuffer);
        return *m_pCurrentDataBuffer;
    }

    DataBuffer& GetCurrentDataBuffer() NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentDataBuffer);
        return *m_pCurrentDataBuffer;
    }

    const DataBuffer& GetSendDataPacketBuffer() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentDataBuffer);
        return m_pCurrentDataBuffer == &m_DataBuffer1 ? m_DataBuffer2 : m_DataBuffer1;
    }

    DataBuffer& GetSendDataPacketBuffer() NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentDataBuffer);
        return m_pCurrentDataBuffer == &m_DataBuffer1 ? m_DataBuffer2 : m_DataBuffer1;
    }

    void SelectDataIds(int flagsCount, uint32_t flags[]) NN_NOEXCEPT;

    void InitializeDataInfo() NN_NOEXCEPT;
    void FinalizeDataInfo() NN_NOEXCEPT;
    const SpyDataInfo* QueryDataInfo() NN_NOEXCEPT;

    void DumpAllModules() NN_NOEXCEPT;
    static const char* StateToString(State value) NN_NOEXCEPT;

    void UnregisterModule(SpyModule& module, bool force) NN_NOEXCEPT;

private: // メンバ変数
    nn::os::Tick m_TickBase;
    nn::os::Tick m_LastOutOfMemoryWarningTick;

    std::atomic<State> m_State;
    SpyModule* m_pModuleTop;
    SpyModule* m_pModuleLast;
    SpyModule* m_pModule;

    TimeModule m_TimeModule;
    LogModule m_LogModule;
    MarkerModule m_MarkerModule;
    PlotModule m_PlotModule;
    DebugModule m_DebugModule;

    detail::SpySession m_Session;
    SessionMessageHandlerAdaptor m_SessionMsgHandlerAdaptor;

    nn::spy::detail::fnd::CriticalSection m_StateLock;
    nn::spy::detail::fnd::CriticalSection m_DataBufferLock;

    DataBuffer* m_pCurrentDataBuffer;
    DataBuffer  m_DataBuffer1;
    DataBuffer  m_DataBuffer2;
    size_t m_DataBufferUsageMax;

    nn::spy::detail::fnd::Event m_PushDataEvent;

    std::atomic<bool> m_IsSyncThreadEnabled;
    std::atomic<bool> m_IsDataThreadEnabled;
    std::atomic<bool> m_IsDataSelected; //!< 1つでもデータが要求されている。

    nn::spy::detail::fnd::Thread m_SyncThread;
    nn::spy::detail::fnd::Thread m_DataThread;
    size_t m_SyncThreadStackSize;
    size_t m_DataThreadStackSize;

    nn::spy::detail::fnd::Thread::HandlerDelegate<SpyController> m_SyncThreadHandler;
    nn::spy::detail::fnd::Thread::HandlerDelegate<SpyController> m_DataThreadHandler;

    void* m_pSyncThreadStack;
    void* m_pDataThreadStack;

#if defined(NN_SPY_DATA_FILE_CHANNEL_AVAILABLE)
    detail::SpyDataFileChannel m_SpyDataFileChannel;
#endif

    friend class SpyModule;
    friend class DebugModule;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
