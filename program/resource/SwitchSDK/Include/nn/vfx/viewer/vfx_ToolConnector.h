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

#include <nn/vfx/vfx_TargetDef.h>
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/viewer/vfx_CmdReceiver.h>
#include <nn/vfx/viewer/vfx_CmdSender.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

static const char           STR_HEADER[] = "NW_FE2";


//------------------------------------------------------------------------------
//! @brief  ToolConnectorクラス 仮想関数
//------------------------------------------------------------------------------
class ToolConnector
{
    NN_DISALLOW_COPY( ToolConnector );

public:
    enum
    {
        taskAlignSize = 0x1000,
        taskMemsize = 0x2000,
        InvalidSocket = -1
    };
    //------------------------------------------------------------------------------
    //! @brief     ToolConnectorを初期化します。
    //! @param[in] pHeap     　     ヒープへのポインタ
    //! @return 　 成功した場合 true
    //------------------------------------------------------------------------------
    virtual bool Initialize( nn::vfx::Heap* pHeap ) NN_NOEXCEPT
    {
        NN_UNUSED( pHeap );
        return false;
    }

    //------------------------------------------------------------------------------
    //! @brief     終了処理
    //------------------------------------------------------------------------------
    virtual void Finalize() NN_NOEXCEPT {};

    //------------------------------------------------------------------------------
    //! @brief     ThreadToolReceiverを終了
    //------------------------------------------------------------------------------
    virtual void StopThread() NN_NOEXCEPT {};

    //------------------------------------------------------------------------------
    //! @brief     接続状況の確認
    //! @return    接続している場合は true
    //------------------------------------------------------------------------------
    virtual bool IsConnected() NN_NOEXCEPT { return false; }

    //------------------------------------------------------------------------------
    //! @brief　   パケットの送信
    //------------------------------------------------------------------------------
    virtual void SendPacket() NN_NOEXCEPT {}

    //------------------------------------------------------------------------------
    //! @brief     パケット受信スレッド
    //! @param[in] intArg     TBD
    //! @param[in] ptrArg     TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    virtual int ThreadToolReceiver( int intArg, void* ptrArg ) NN_NOEXCEPT
    {
        NN_UNUSED( intArg );
        NN_UNUSED( ptrArg );
        return false;
    }

    //------------------------------------------------------------------------------
    //! @brief     Connecotorの設定の確認
    //! @return    Connectorが空の場合は true
    //------------------------------------------------------------------------------
    virtual bool IsConnectorEmpty() NN_NOEXCEPT { return true; }

    //------------------------------------------------------------------------------
    //! @brief     コマンド受信クラスを取得する
    //! @return    常にNULL
    //------------------------------------------------------------------------------
    virtual CommandReceiver* GetCommandReceiver() NN_NOEXCEPT { return NULL; }

    //------------------------------------------------------------------------------
    //! @brief     コマンド送信クラスを取得する
    //! @return    常にNULL
    //------------------------------------------------------------------------------
    virtual CommandSender*   GetCommandSender() NN_NOEXCEPT { return NULL; }

    //------------------------------------------------------------------------------
    //! @brief     コンストラクタ
    //------------------------------------------------------------------------------
    ToolConnector() NN_NOEXCEPT {}

    //------------------------------------------------------------------------------
    //! @brief     デストラクタ
    //------------------------------------------------------------------------------
    virtual ~ToolConnector() NN_NOEXCEPT {}
};

//------------------------------------------------------------------------------
//! @brief  ToolConnectorActualクラス
//------------------------------------------------------------------------------
class ToolConnectorActual : public ToolConnector
{
    NN_DISALLOW_COPY( ToolConnectorActual );
public:

    //------------------------------------------------------------------------------
    //! @brief     ToolConnectorを初期化します。
    //! @param[in] pHeap     　     ヒープへのポインタ
    //! @return 　 成功した場合 true
    //------------------------------------------------------------------------------
     bool Initialize( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     終了処理
    //------------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     ThreadToolReceiverを終了
    //------------------------------------------------------------------------------
    void StopThread() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     接続状況の確認
    //! @return    接続している場合は true
    //------------------------------------------------------------------------------
    bool IsConnected() NN_NOEXCEPT
    {
        return m_IsConnected;
    }

    //------------------------------------------------------------------------------
    //! @brief　   パケットの送信
    //------------------------------------------------------------------------------
    void SendPacket() NN_NOEXCEPT;


    //------------------------------------------------------------------------------
    //! @brief     パケット受信スレッド
    //! @param[in] intArg     TBD
    //! @param[in] ptrArg     TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    int ThreadToolReceiver( int intArg, void* ptrArg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     Connecotorの設定の確認
    //! @return    Connectorがある場合は false
    //------------------------------------------------------------------------------
    bool IsConnectorEmpty() NN_NOEXCEPT { return false; }

    //------------------------------------------------------------------------------
    //! @brief     コンストラクタ
    //------------------------------------------------------------------------------
    ToolConnectorActual() NN_NOEXCEPT : m_IsOK(false)
                                       ,m_IsConnected(false)
                                       ,m_IsDetermined(true)
                                       ,m_Handle(InvalidSocket)
                                       ,m_pReceive(NULL)
                                       ,m_pSender(NULL) {}

    //------------------------------------------------------------------------------
    //! @brief     コマンド受信クラスを取得する
    //! @return    コマンド受信クラスへのハンドル
    //------------------------------------------------------------------------------
    CommandReceiver* GetCommandReceiver() NN_NOEXCEPT { return m_pReceive; }

    //------------------------------------------------------------------------------
    //! @brief     コマンド受信クラスを取得する
    //! @return    コマンド受信クラスへのハンドル
    //------------------------------------------------------------------------------
    CommandSender*   GetCommandSender() NN_NOEXCEPT { return m_pSender; }

private:
    //------------------------------------------------------------------------------
    //! @brief 　　パケットの確認
    //! @param[in] pBuffer 　　    受信バッファ
    //! @return 　 正しいパケットは true
    //------------------------------------------------------------------------------
    bool IsHandlePacket( uint8_t* pBuffer ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief 　  パケットのスキップ
    //! @param[in] pBuffer     受信バッファ
    //! @param[out] pOutSize   バッファスキップしたサイズ
    //! @return    メッセージヘッダーサイズをスキップした受信バッファのポインタ
    //------------------------------------------------------------------------------
    uint8_t* SkipPacket( size_t* pOutSize, uint8_t* pBuffer ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     パケットの受信
    //! @param[in] handle          ソケットハンドル
    //! @param[in] pReceiveBuffer  受信バッファ
    //! @param[in] receiveSize     受信サイズ
    //! @return    1以上は受信したデータのバイト数
    //------------------------------------------------------------------------------
    uint32_t ReceivePacket( int handle, void* pReceiveBuffer, size_t receiveSize ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     パケットデータの取得
    //! @param[in] param           ソケットハンドル
    //! @return TBD
    //------------------------------------------------------------------------------
    bool ReadPacket( int param ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     パケットの送信
    //! @param[in] param ソケットハンドル     T
    //------------------------------------------------------------------------------
    void SendPacket( int param ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     ソケットのオープン
    //! @param[in] listenSocket    サービス状況確認ソケットハンドル     T
    //! @param[in] sock 　　　　   データ送受信ソケットハンドル
    //! @return    ソケットのオープンに成功したら　true
    //------------------------------------------------------------------------------
    bool OpenSocket( int *listenSocket, int *sock ) NN_NOEXCEPT;

private:

    bool                 m_IsOK;             //!< 初期化済みフラグ
    bool                 m_IsConnected;      //!< EffectMakerとの接続状態をあわすフラグ
    bool                 m_IsDetermined;     //!< 処理中かどうか

    size_t               m_ReadBufferSize;   //!< 受信バッファサイズ
    nn::vfx::Heap*       m_pVfxHeap;         //!< ヒープポインタ
    uint8_t*             m_pReadBuffer;      //!< 受信バッファポインタ
    int                  m_Handle;           //!< ソケットパンドル

    CommandReceiver*     m_pReceive;         //!< 受信クラス
    CommandSender*       m_pSender;          //!< 送信クラス
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
