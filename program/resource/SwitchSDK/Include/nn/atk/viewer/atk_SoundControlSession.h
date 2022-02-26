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

#include <nn/atk/fnd/basis/atkfnd_Time.h>
#include <nn/atk/viewer/atk_Types.h>
#include <nn/atk/viewer/detail/hio/atk_HioManager.h>
#include <nn/atk/viewer/detail/hio/atk_HioAsyncChannel.h>
#include <nn/atk/viewer/detail/handler/atk_PlaySoundHandler.h>
#include <nn/atk/viewer/detail/handler/atk_StopSoundHandler.h>
#include <nn/atk/viewer/detail/handler/atk_PauseSoundHandler.h>

namespace nn {
namespace atk {

namespace detail {
namespace fnd {
class FrameHeap;
}
}

namespace viewer {

class SoundObjectController;
class SoundControlSession;

//---------------------------------------------------------------------------
//! @brief  【β版】サウンドのリモート制御セッションを管理します。
//!
//!         セッションを開くと、SoundMaker からのリモート制御要求を受け付けます。
//---------------------------------------------------------------------------
class SoundControlSession
{
    NN_DISALLOW_COPY(SoundControlSession);

public: // 定数の定義
    static const size_t DefaultChannelStreamBufferSize   = 64 * 1024;   //!< チャンネルバッファサイズのデフォルト値です。
    static const uint32_t DefaultSyncTimeout             = 200;         //!< SYNC タイムアウトのデフォルト値です。
    static const int DefaultMaxItemName             = 256;         //!< アイテム名の長さのデフォルト上限値です。

public: // 型の定義
    //---------------------------------------------------------------------------
    //! @brief  SoundControlSession 設定を格納する構造体です。
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
            maxItemName(DefaultMaxItemName),
            port(viewer::detail::HioChannelType_SndeditCtrl)
        {
        }

        size_t channelStreamBufferSize;    //!< チャンネル毎の受信用ストリームのバッファサイズです。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultChannelStreamBufferSize が使用されます。
                                        //!< @n
                                        //!< 通常の利用において、値を変更する必要はありません。
        uint32_t syncTimeout;           //!< SYNC タイムアウト（ミリ秒）です。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultSyncTimeout が使用されます。
                                        //!< @n
                                        //!< 通常の利用において、値を変更する必要はありません。
        int maxItemName;           //!< アイテム名の長さの上限値です。
                                        //!< @n
                                        //!< 指定しないとデフォルト値 DefaultMaxItemName が使用されます。
                                        //!< @n
                                        //!< 非常に長いアイテム名を対象にしたい場合や、使用する名前が短いことが事前にわかっていて、
                                        //!< 少しでもメモリ使用量を減らしたい場合に設定を変更してください。
        viewer::PORT port;              //!< 内部で使用するポートです。
    };

private: // 型の定義
    enum State
    {
        State_NotInitialized = 0,  //!< 未初期化状態です。
        State_Initialized = 1,      //!< 初期化済み状態です。
        State_Opened                //!< セッションが開かれている状態です。
    };

public: // コンストラクタ
    //===========================================================================
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    SoundControlSession() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    ~SoundControlSession() NN_NOEXCEPT;

    //@}
    // コンストラクタ/デストラクタ
    //===========================================================================

public: // メソッド
    //===========================================================================
    //! @name 初期化
    //@{

    //---------------------------------------------------------------------------
    //! @brief      SoundControlSession を初期化します。
    //!
    //!             この関数は、SoundControlSession で利用するメモリを初期化し、
    //!             SoundMaker との接続を開始するための準備を行います。
    //!             @n
    //!             SoundControlSession を利用する前に、必ずこの関数を呼び出してください。
    //!
    //!             引数 buffer は、通信の作業領域として利用されます。
    //!             @n
    //!             SoundControlSession::GetRequiredMemorySize() が返すより大きいサイズを指定する必要があります。
    //!
    //! @param[in]  buffer           通信処理の作業バッファです。
    //! @param[in]  bufferLength     buffer の長さを指定します。
    //! @param[in]  configs          SoundControlSession の設定を指定します。
    //!                              GetRequiredMemorySize() に渡した Configs を指定します。
    //!
    //! @return     処理結果を返します。
    //!
    //! @see        Configs
    //! @see        GetRequiredMemorySize()
    //! @see        Finalize()
    //---------------------------------------------------------------------------
    viewer::Result Initialize(
        void* buffer,
        size_t bufferLength,
        const Configs& configs) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  SoundControlSession の終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      SoundControlSession の利用に必要なメモリサイズを取得します。
    //!
    //!             SoundControlSession を利用するには、
    //!             この関数で取得したサイズ分のバッファを確保し、
    //!             Initialize() の引数 buffer、bufferLength に指定する必要があります。
    //!             @n
    //!             その際 Initialize() の引数 configs には、
    //!             GetRequiredMemorySize() に渡した Configs と同じものを指定してください。
    //!
    //! @param[in]  configs  SoundControlSession の設定を指定します。
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
    //! @brief      指定 SoundObjectController を登録します。
    //!
    //!             ここで指定した SoundObjectController が SoundMaker と接続する対象となります。
    //!             @n
    //!             SoundObjectController は１つだけ登録できます。
    //!
    //! @param[in]  soundObjectController  編集対象の SoundObjectController を指定します。
    //!
    //! @return     結果を返します。
    //!
    //! @see        UnregisterSoundObjectController()
    //---------------------------------------------------------------------------
    viewer::Result RegisterSoundObjectController(SoundObjectController* soundObjectController) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定 SoundObjectController の登録を解除します。
    //!
    //! @param[in]  soundObjectController  編集対象の SoundObjectController を指定します。
    //!
    //! @see        RegisterSoundObjectController()
    //---------------------------------------------------------------------------
    void UnregisterSoundObjectController(SoundObjectController* soundObjectController) NN_NOEXCEPT;

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
        return IsInitialized() && m_State > State_Initialized;
    }

    //---------------------------------------------------------------------------
    //! @brief   SoundMaker との接続の有無を取得します。
    //!
    //! @return  SoundMaker と接続済みの場合は true、接続されていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsConnected() const NN_NOEXCEPT
    {
        return IsInitialized() && m_Channel.IsOpened();
    }

    //@}
    // 状態の取得
    //===========================================================================

    //===========================================================================
    //! @name 開始と終了
    //@{

    //---------------------------------------------------------------------------
    //! @brief  サウンドリモート制御セッションを開きます。
    //!
    //!         この関数が呼び出されると、Update() のタイミングで
    //!         SoundMaker との通信が行われるようになります。
    //!
    //! @see    Close()
    //! @see    Update()
    //---------------------------------------------------------------------------
    void Open() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンドリモート制御セッションを閉じます。
    //!
    //!         SoundMakerとの通信も終了します。
    //!
    //! @see    Open()
    //---------------------------------------------------------------------------
    void Close() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンドリモート制御セッションを更新します。
    //!
    //!         セッションが開かれている間、SoundMaker からの要求を受け付けます。
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

private: // メソッド
    //! @brief  チャンネルを初期化します。
    viewer::Result InitializeChannel(
        atk::detail::fnd::FrameHeap& allocator,
        size_t recvStreamBufferSize,
        size_t recvPacketBufferSize) NN_NOEXCEPT;

    //! @brief  チャンネルの終了処理を行います。
    void FinalizeChannel() NN_NOEXCEPT;

    //! @brief  メッセージハンドラを初期化します。
    void InitializeHandlers() NN_NOEXCEPT;

    //! @brief  メッセージハンドラの終了処理を行います。
    void FinalizeHandlers() NN_NOEXCEPT;

    //! @brief  チャンネルの受信パケットバッファサイズを取得します。
    size_t GetChannelRecvPacketBufferSize(int maxItemName) const NN_NOEXCEPT;

    //! @brief  通信バッファをクリアします。
    void ClearBuffer() NN_NOEXCEPT;

    //! @brief  同期します。
    void Sync() NN_NOEXCEPT;

    //! @brief 内部で使用する作業バッファのサイズを取得します。
    size_t GetRequiredWorkBufferSize() const NN_NOEXCEPT;

    //! @brief チャンネル識別番号からプラットフォームごとに必要なチャンネル情報を取得します。
    viewer::detail::HioStream::ChannelType GetChannelInfo(viewer::detail::HioChannelType channel) const NN_NOEXCEPT;

private: // メンバ変数
    State m_State;                                          //!< セッションの状態です。
    viewer::PORT m_Port;                                      //!< 内部で使用するポートです。

    viewer::detail::HioManager      m_HioManager;           //!< HostIOマネージャです。
    viewer::detail::HioAsyncChannel m_Channel;              //!< 非同期チャンネルです。

    uint32_t                           m_SyncTimeout;            //!< SYNC タイムアウト時間です。
    atk::detail::fnd::StopWatch m_SyncStopWatch;          //!< SYNC 間隔を示すストップウォッチです。

    detail::PlaySoundHandler  m_PlaySoundHandler;         //!< PlaySoundPacket のハンドラです。
    detail::StopSoundHandler  m_StopSoundHandler;         //!< StopSoundPacket のハンドラです。
    detail::PauseSoundHandler m_PauseSoundHandler;        //!< PauseSoundPacket のハンドラです。

    SoundObjectController* m_SoundObjectController;         //!< サウンドオブジェクトコントローラです。
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
