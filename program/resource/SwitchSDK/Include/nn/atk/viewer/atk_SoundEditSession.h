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

#include <nn/atk/viewer/atk_Types.h>
#include <nn/atk/viewer/detail/atk_SoundEditConnection.h>
#include <nn/atk/fnd/basis/atkfnd_Time.h>
#include <nn/atk/viewer/detail/hio/atk_HioManager.h>
#include <nn/atk/viewer/detail/hio/atk_HioSyncChannel.h>
#include <nn/atk/viewer/detail/hio/atk_HioAsyncChannel.h>
#include <nn/atk/viewer/detail/handler/atk_SyncReplyHandler.h>
#include <nn/atk/viewer/detail/handler/atk_QueryItemsReplyHandler.h>

namespace nn {
namespace atk {

namespace detail {
namespace fnd {
class FrameHeap;
}
}

namespace viewer {

class SoundArchiveEditor;
class SoundEditSession;

//---------------------------------------------------------------------------
//! @brief  【β版】サウンド編集のセッションを管理します。
//!
//!         セッションを開くと、SoundMaker に定期的に接続要求を投げます。
//!         @n
//!         SoundMaker から接続を許可されると、指定 SoundArchiveEditor に
//!         関連付けられた SoundArchive の内容を SoundMaker 上のプロジェクト内容に置き換えます。
//---------------------------------------------------------------------------
class SoundEditSession
{
    NN_DISALLOW_COPY(SoundEditSession);

public: // 定数の定義
    static const size_t   DefaultChannelStreamBufferSize = 64 * 1024;   //!< チャンネルバッファサイズのデフォルト値です。
    static const uint32_t DefaultSyncTimeout             =  3 * 1000;   //!< SYNC タイムアウトのデフォルト値です。
    static const uint32_t DefaultCacheSyncInterval       =  3 * 1000;   //!< キャッシュの同期間隔のデフォルト値です。
    static const uint32_t DefaultSendTimeout             =  5 * 1000;   //!< Send タイムアウトのデフォルト値です。
    static const int      DefaultMaxItemName             = 256;         //!< アイテム名の長さのデフォルト上限値です。

public: // 型の定義
    //---------------------------------------------------------------------------
    //! @brief  SoundEditSession 設定を格納する構造体です。
    //!
    //!         各パラメータには、コンストラクタでデフォルト値が設定されます。
    //!
    //! @see    Initialize()
    //! @see    GetRequiredMemorySize()
    //---------------------------------------------------------------------------
    struct Configs
    {
        //---------------------------------------------------------------------------
        //! @brief  コンストラクタです。
        //---------------------------------------------------------------------------
        Configs() NN_NOEXCEPT :
            channelStreamBufferSize(DefaultChannelStreamBufferSize),
            syncTimeout(DefaultSyncTimeout),
            cacheSyncInterval(DefaultCacheSyncInterval),
            sendTimeout(DefaultSendTimeout),
            maxItemName(DefaultMaxItemName),
            port0(detail::HioChannelType_SndeditSync),
            port1(detail::HioChannelType_SndeditFunc)
        {
        }

        size_t channelStreamBufferSize; //!< チャンネル毎の受信用ストリームのバッファサイズです。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultChannelStreamBufferSize が使用されます。
                                        //!< @n
                                        //!< 通常の利用において、値を変更する必要はありません。
        uint32_t syncTimeout;           //!< SYNC タイムアウト（ミリ秒）です。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultSyncTimeout が使用されます。
                                        //!< @n
                                        //!< 通常の利用において、値を変更する必要はありません。
        uint32_t cacheSyncInterval;     //!< キャッシュの同期間隔（ミリ秒）です。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultCacheSyncInterval が使用されます。
                                        //!< @n
                                        //!< 通常の利用において、値を変更する必要はありません。
        uint32_t sendTimeout;           //!< 送信タイムアウト（ミリ秒）です。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultSendTimeout が使用されます。
                                        //!< @n
                                        //!< 通常の利用において、値を変更する必要はありません。
        int maxItemName;           //!< アイテム名の長さの上限値です。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultMaxItemName が使用されます。
                                        //!< @n
                                        //!< 非常に長いアイテム名を対象にしたい場合や、使用する名前が短いことが事前にわかっていて、
                                        //!< 少しでもメモリ使用量を減らしたい場合に設定を変更してください。

        PORT port0;                     //!< 内部で使用するポートです。
        PORT port1;                     //!< 内部で使用するポートです。
    };

private: // 型の定義
    enum State
    {
        State_NotInitialized = 0,  //!< 未初期化状態です。
        State_Stopped,              //!< 停止状態です。
        State_SyncRequesting,      //!< SYNC 要求中（オープンされていない状態）です。
        State_SyncUpdating,        //!< SYNC 更新中（オープンされている状態）です。
        State_SyncCompleted        //!< SYNC 済み（オープンされている状態）です。
    };

public: // コンストラクタ
    //===========================================================================
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    SoundEditSession() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    ~SoundEditSession() NN_NOEXCEPT;

    //@}
    // コンストラクタ/デストラクタ
    //===========================================================================

public: // メソッド
    //===========================================================================
    //! @name 初期化
    //@{

    //---------------------------------------------------------------------------
    //! @brief      SoundEditSession を初期化します。
    //!
    //!             この関数は、SoundEditSession で利用するメモリを初期化し、
    //!             SoundMaker との接続を開始するための準備を行います。
    //!             @n
    //!             SoundEditSession を利用する前に、必ずこの関数を呼び出してください。
    //!
    //!             引数 buffer は、通信の作業領域として利用されます。
    //!             @n
    //!             SoundEditSession::GetRequiredMemorySize() が返すより大きいサイズを指定する必要があります。
    //!
    //! @param[in]  buffer           通信処理の作業バッファです。
    //! @param[in]  bufferLength     buffer の長さを指定します。
    //! @param[in]  configs          SoundEditSession の設定を指定します。
    //!                              GetRequiredMemorySize() に渡した Configs を指定します。
    //!
    //! @return     処理結果を返します。
    //!
    //! @see        Configs
    //! @see        GetRequiredMemorySize()
    //! @see        Finalize()
    //---------------------------------------------------------------------------
    Result Initialize(
        void* buffer,
        size_t bufferLength,
        const Configs& configs) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  SoundEditSession の終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      SoundEditSession の利用に必要なメモリサイズを取得します。
    //!
    //!             SoundEditSession を利用するには、
    //!             この関数で取得したサイズ分のバッファを確保し、
    //!             Initialize() の引数 buffer、bufferLength に指定する必要があります。
    //!             @n
    //!             その際 Initialize() の引数 configs には、
    //!             GetRequiredMemorySize() に渡した Configs と同じものを指定してください。
    //!
    //! @param[in]  configs  SoundEditSession の設定を指定します。
    //!
    //! @return     必要なメモリサイズを返します。
    //!
    //! @see        Configs
    //! @see        Initialize()
    //---------------------------------------------------------------------------
    size_t GetRequiredMemorySize(const Configs& configs) const NN_NOEXCEPT;

    //@}
    // 初期化
    //===========================================================================

    //===========================================================================
    //! @name 編集対象の登録
    //@{

    //---------------------------------------------------------------------------
    //! @brief      指定 SoundArchiveEditor を登録します。
    //!
    //!             ここで指定した SoundArchiveEditor が SoundMaker と接続する対象となります。
    //!
    //!             【★注意】
    //!             @n
    //!             現状では、SoundArchiveEditorを１つだけ登録でき、
    //!             複数登録しようとすると失敗します。
    //!             @n
    //!             今後、SoundMaker上のサウンドプロジェクトとサウンドアーカイブバイナリの
    //!             マッチングを可能する予定で、その際に、
    //!             複数登録が可能になる（マルチサウンドアーカイブ対応）予定です。
    //!
    //! @param[in]  soundArchiveEditor  編集対象の SoundArchiveEditor を指定します。
    //!
    //! @return     結果を返します。
    //!
    //! @see        UnregisterSoundArchiveEditor()
    //---------------------------------------------------------------------------
    Result RegisterSoundArchiveEditor(SoundArchiveEditor* soundArchiveEditor) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定 SoundArchiveEditor の登録を解除します。
    //!
    //!             【★注意】
    //!             @n
    //!             現状では、SoundArchiveEditorを１つだけ登録でき、
    //!             複数登録しようとすると失敗します。
    //!             @n
    //!             今後、SoundMaker上のサウンドプロジェクトとサウンドアーカイブバイナリの
    //!             マッチングを可能する予定で、その際に、
    //!             複数登録が可能になる（マルチサウンドアーカイブ対応）予定です。
    //!
    //! @param[in]  soundArchiveEditor  編集対象の SoundArchiveEditor を指定します。
    //!
    //! @see        RegisterSoundArchiveEditor()
    //---------------------------------------------------------------------------
    void UnregisterSoundArchiveEditor(SoundArchiveEditor* soundArchiveEditor) NN_NOEXCEPT;

    //@}
    // 編集対象の登録
    //===========================================================================

    //===========================================================================
    //! @name 状態の取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief   初期化の有無を取得します。
    //!
    //! @return  初期化済みの場合は true、初期化されていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_State > State_NotInitialized;
    }

    //---------------------------------------------------------------------------
    //! @brief  セッションが開かれているかどうかを取得します。
    //!
    //! @return セッションが開かれている場合は true、開らかれていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsOpened() const NN_NOEXCEPT
    {
        return IsInitialized() && m_State > State_Stopped;
    }

    //---------------------------------------------------------------------------
    //! @brief   SoundMaker との接続の有無を取得します。
    //!
    //! @return  SoundMaker と接続済みの場合は true、接続されていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsConnected() const NN_NOEXCEPT
    {
        return IsInitialized() && m_Connection.IsOpened();
    }

    //@}
    // 状態の取得
    //===========================================================================

    //===========================================================================
    //! @name 開始と終了
    //@{

    //---------------------------------------------------------------------------
    //! @brief  サウンド編集セッションを開きます。
    //!
    //!         この関数が呼び出されると、Update() のタイミングで
    //!         SoundMaker との通信が行われるようになります。
    //!
    //! @see    Close()
    //! @see    Update()
    //---------------------------------------------------------------------------
    void Open() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンド編集セッションを閉じます。
    //!
    //!         SoundMakerとの通信も終了します。
    //!
    //! @see    Open()
    //---------------------------------------------------------------------------
    void Close() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンド編集セッションを更新します。
    //!
    //!         セッションが開かれている間、SoundMaker への接続要求を投げたり、
    //!         SoundMaker に編集のあったアイテムを問い合わせたりします。
    //!
    //!         セッションが開かれていない場合は、この関数を呼び出しても何も処理しません。
    //!
    //! @see    Open()
    //! @see    Close()
    //---------------------------------------------------------------------------
    void Update() NN_NOEXCEPT;

    //@}
    // 開始と終了
    //===========================================================================

    // HACK : ★SoundMaker がゲームアプリと SoundPlayer の区別がつくようになるまでの暫定コード
#if 1
    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  SoundPlayer モードの設定を行います。
    //!         ★SoundMaker がゲームアプリと SoundPlayer の区別がつくようになるまでの暫定コードです。
    //! @param value :private
    //---------------------------------------------------------------------------
    void Interim_SetIsSoundPlayer(bool value) NN_NOEXCEPT
    {
        m_Interim_IsSoundPlayer = value;
    }
#endif

private: // メソッド
    //! @brief  SYNC 用チャンネルを初期化します。
    Result InitializeSyncChannel(
        atk::detail::fnd::FrameHeap& allocator,
        size_t recvStreamBufferSize,
        size_t recvPacketBufferSize) NN_NOEXCEPT;

    //! @brief  SYNC 用チャンネルの終了処理を行います。
    void FinalizeSyncChannel() NN_NOEXCEPT;

    //! @brief  SYNC 用メッセージハンドラを初期化します。
    void InitializeSyncHandlers() NN_NOEXCEPT;

    //! @brief  SYNC 用メッセージハンドラの終了処理を行います。
    void FinalizeSyncHandlers() NN_NOEXCEPT;

    //! @brief  実機->PCチャンネルを初期化します。
    Result InitializeFuncChannel(
        atk::detail::fnd::FrameHeap& allocator,
        size_t recvStreamBufferSize,
        size_t recvPacketBufferSize) NN_NOEXCEPT;

    //! @brief  実機->PCチャンネルの終了処理を行います。
    void FinalizeFuncChannel() NN_NOEXCEPT;

    //! @brief  実機->PCメッセージハンドラを初期化します。
    void InitializeFuncHandlers() NN_NOEXCEPT;

    //! @brief  実機->PCメッセージハンドラの終了処理を行います。
    void FinalizeFuncHandlers() NN_NOEXCEPT;

    //! @brief  SYNC チャンネルの受信パケットバッファサイズを取得します。
    size_t GetSyncChannelRecvPacketBufferSize(int maxItemName) const NN_NOEXCEPT;

    //! @brief  FUNC チャンネルの受信パケットバッファサイズを取得します。
    size_t GetFuncChannelRecvPacketBufferSize(int maxItemName, size_t maxItemInfoSize) const NN_NOEXCEPT;

    //! @brief  接続します。
    Result Connect() NN_NOEXCEPT;

    //! @brief  切断します。
    void Disconnect() NN_NOEXCEPT;

    //! @brief  通信バッファをクリアします。
    void ClearBuffer() NN_NOEXCEPT;

    bool RequestSync() NN_NOEXCEPT;

    //! @brief 内部で使用する作業バッファのサイズを取得します。
    size_t GetRequiredWorkBufferSize() const NN_NOEXCEPT;

    //! @brief チャンネル識別番号からプラットフォームごとに必要なチャンネル情報を取得します。
    detail::HioStream::ChannelType GetChannelInfo(detail::HioChannelType channel) const NN_NOEXCEPT;

private: // メンバ変数
    // HACK : ★SoundMaker がゲームアプリと SoundPlayer の区別がつくようになるまでの暫定コード
#if 1
    bool m_Interim_IsSoundPlayer;
#endif

    State m_State;                                          //!< セッションの状態です。
    PORT m_Port0;                                           //!< 内部で使用するポートです。
    PORT m_Port1;                                           //!< 内部で使用するポートです。

    detail::HioManager       m_HioManager;                //!< HostIOマネージャです。
    detail::HioAsyncChannel  m_SyncChannel;               //!< SYNC 用非同期チャンネルです。
    detail::HioSyncChannel   m_FuncChannel;               //!< 機能チャンネルです。
    detail::SyncReplyHandler m_SyncReplyHandler;          //!< SyncReplyPacket のハンドラです。

    uint32_t                           m_SyncTimeout;            //!< SYNC タイムアウト時間です。
    atk::detail::fnd::StopWatch m_SyncStopWatch;          //!< SYNC 間隔を示すストップウォッチです。

    detail::SoundEditConnection  m_Connection;            //!< サウンド編集コネクションです。
    SoundArchiveEditor*            m_SoundArchiveEditor;    //!< サウンドアーカイブエディタです。

    friend class detail::SyncReplyHandler;
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
