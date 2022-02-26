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

#include <nn/vfx/viewer/vfx_Message.h>
#include <nn/vfx/viewer/vfx_Protocol.h>
#include <nn/vfx/viewer/vfx_CmdReceiver.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//------------------------------------------------------------------------------
//! @brief コマンド送信クラス
//------------------------------------------------------------------------------
class CommandSender
{
    NN_DISALLOW_COPY( CommandSender );
    friend class ToolConnector;

public:
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタ
    //---------------------------------------------------------------------------
    CommandSender() NN_NOEXCEPT
    {
        //       ClearToolName();
    }

    //---------------------------------------------------------------------------
    //! @brief     コンストラクタ
    //! @param[in] pHeap        TBD
    //! @param[in] bufferSize   TBD
    //---------------------------------------------------------------------------
    CommandSender( nn::vfx::Heap* pHeap, size_t bufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        デストラクタ
    //---------------------------------------------------------------------------
    virtual ~CommandSender() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        コマンドを送信する
    //! @param[in] type     TBD
    //! @param[in] buffer     TBD
    //! @param[in] bufferSize     TBD
    //---------------------------------------------------------------------------
    virtual void SendCommand( MessageType type, void* buffer, size_t bufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        接続先のエフェクトメーカーを検索する
    //---------------------------------------------------------------------------
    virtual void SearchEffectMaker() NN_NOEXCEPT
    {
    }

    //---------------------------------------------------------------------------
    //! @brief        送信コマンドを追加します。
    //! @param[in] buffer     TBD
    //! @param[in] bufferSize     TBD
    //---------------------------------------------------------------------------
    void AddCommand( const void* buffer, size_t bufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        送信コマンドをpopします。
    //! @return TBD
    //---------------------------------------------------------------------------
    void PopCommand() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        現在の送信コマンドを取得
    //! @return TBD
    //---------------------------------------------------------------------------
    Command* GetCommand() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        ToolConnectorを設定します。
    //! @param[in] toolConnector     ToolConnectorクラスのポインタ
    //---------------------------------------------------------------------------
    void SetToolConnector( ToolConnector* toolConnector ) NN_NOEXCEPT
    {
        m_ToolConnector = toolConnector;
    }

public:
    static CommandSender*   g_pCommandSender;        //!< TBD
private:
    static nn::os::Mutex    m_Mutex;                //!< TBD
    nn::vfx::Heap*          m_pHeap;                //!< TBD
    CmdFixedArray           m_CommandQueue;         //!< ビューアコマンドキュー
    uint8_t*                m_SharedBuffer;         //!< 送信コマンドバッファポインタ
    size_t                  m_SharedBufferSize;     //!< 送信コマンドバッファサイズ
    ToolConnector*          m_ToolConnector;        //!< ToolConnectorのポインタ
};


//------------------------------------------------------------------------------
//! @brief              コマンド送信クラスを生成する
//! @param[in] pHeap     ヒープへのポインタ
//! @param[in] bufferSize バッファサイズ
//! @return             コマンド送信クラスへのポインタ
//------------------------------------------------------------------------------
CommandSender* CreateCommandSender( nn::vfx::Heap* pHeap, size_t bufferSize ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief  コマンド送信クラスを取得する
//! @return コマンド送信クラスへのポインタ
//------------------------------------------------------------------------------
CommandSender* GetCommandSender() NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief  コマンド送信クラスを破棄する
//! @param[in] pHeap     ヒープへのポインタ
//------------------------------------------------------------------------------
void DestroyCommandSender( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn


