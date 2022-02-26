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

#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/viewer/vfx_Message.h>
#include <nn/vfx/vfx_PtrArray.h>
#include <nn/os.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

class CommandReceiver;
class CommandSender;
class ToolConnector;
class PacketProcedure;

//------------------------------------------------------------------------------
//! @briefprivate TBD
//------------------------------------------------------------------------------
enum CommandReceiverSettings
{
    CommandReceiverSettings_MaxReceivedBufferSize   = 256 * 10,     //!< 受信バッファの配列サイズ
    CommandReceiverSettings_CallbackItemBufferSize  = 32  * 10,     //!< 受信時のコールバック登録用の配列サイズ
    CommandReceiverSettings_CommandId               = 101,          //!< TBD
};

//------------------------------------------------------------------------------
//! @brief コマンド
//------------------------------------------------------------------------------
class Command
{
    NN_DISALLOW_COPY( Command );
    friend class CommandReceiver;
    friend class CommandSender;

public:
    //---------------------------------------------------------------------------
    //! @brief TBD
    //---------------------------------------------------------------------------
    enum CommandType
    {
        CommandType_String = 0, //!< TBD
        CommandType_Buffer = 1, //!< TBD
        CommandType_None = 2,   //!< TBD
    };

    //---------------------------------------------------------------------------
    //! @brief        コンストラクタ
    //---------------------------------------------------------------------------
    Command() NN_NOEXCEPT
    {
        Initialize();
    }

    //---------------------------------------------------------------------------
    //! @brief        デストラクタ
    //---------------------------------------------------------------------------
    ~Command() NN_NOEXCEPT
    {
        if( m_CommandBuffer )
        {
            m_pHeap->Free( m_CommandBuffer );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンドを取得する
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetCommand() NN_NOEXCEPT
    {
        return m_CommandId;
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンドタイプを取得する
    //! @return TBD
    //---------------------------------------------------------------------------
    CommandType GetCommandType() NN_NOEXCEPT
    {
        return m_CommandType;
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンド文字列を取得する
    //! @return TBD
    //---------------------------------------------------------------------------
    const char* GetCommandString() NN_NOEXCEPT
    {
        return static_cast< const char* >( m_CommandBuffer );
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンドバッファを取得する
    //! @return TBD
    //---------------------------------------------------------------------------
    void* GetCommandBuffer() NN_NOEXCEPT
    {
        return static_cast< void* >( m_CommandBuffer );
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンドバッファサイズを取得する
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetCommandBufferSize() NN_NOEXCEPT
    {
        return m_CommandBufferSize;
    }

private:
    //---------------------------------------------------------------------------
    //! @brief        コマンドを設定する
    //! @param[in] pHeap     TBD
    //! @param[in] commandId     TBD
    //! @param[in] string     TBD
    //---------------------------------------------------------------------------
    void SetCommand( nn::vfx::Heap* pHeap, int commandId, const char* string ) NN_NOEXCEPT
    {
        m_pHeap = pHeap;
        m_CommandBuffer = m_pHeap->Alloc( strlen( string ) + 1 );
        NN_SDK_ASSERT_NOT_NULL( m_CommandBuffer );

        m_CommandId = commandId;
        m_CommandType = CommandType_String;
#if defined( NN_BUILD_CONFIG_OS_WIN )
        strcpy_s( static_cast< char* >( m_CommandBuffer ), strlen( string ) + 1, string );
#else
        strcpy( static_cast< char* >( m_CommandBuffer ), string );
#endif
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンドを設定する
    //! @param[in] pHeap     TBD
    //! @param[in] commandId     TBD
    //! @param[in] buffer     TBD
    //! @param[in] bufferSize     TBD
    //---------------------------------------------------------------------------
    void SetCommand( nn::vfx::Heap* pHeap, int commandId, const void* buffer, size_t bufferSize ) NN_NOEXCEPT
    {
        m_pHeap = pHeap;
        m_CommandBuffer = m_pHeap->Alloc( bufferSize );
        NN_SDK_ASSERT_NOT_NULL( m_CommandBuffer );

        m_CommandId = commandId;
        m_CommandType = CommandType_Buffer;
        m_CommandBufferSize = bufferSize;
        memcpy( m_CommandBuffer, buffer, bufferSize );
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンドを初期化する
    //---------------------------------------------------------------------------
    void Initialize() NN_NOEXCEPT
    {
        m_CommandId = 0;
        m_CommandType = CommandType_None;
        m_CommandBuffer = NULL;
        m_CommandBufferSize = 0;
    }

private:
    int                 m_CommandId;            //!< TBD
    CommandType         m_CommandType;          //!< TBD
    void*               m_CommandBuffer;        //!< TBD
    size_t              m_CommandBufferSize;    //!< TBD
    nn::vfx::Heap*      m_pHeap;                //!< TBD
};


//------------------------------------------------------------------------------
//! @brief 定義
//------------------------------------------------------------------------------
typedef void( *CBFuncReceive )( void* pThis, Message* pHeader, const void* pBody );

typedef nn::vfx::detail::PointerFixedSizeArray<Command*, CommandReceiverSettings_MaxReceivedBufferSize> CmdFixedArray;

//------------------------------------------------------------------------------
//! @brief コマンド受信クラス
//------------------------------------------------------------------------------
class CommandReceiver
{
    NN_DISALLOW_COPY( CommandReceiver );

private:
    //---------------------------------------------------------------------------
    //! @briefprivate TBD
    //---------------------------------------------------------------------------
    struct CallbackItem
    {
        //! @briefprivate TBD
        //! @param[in] pThisValue     TBD
        //! @param[in] type           TBD
        //! @param[in] func           TBD
        void Initialize( void* pThisValue, Message type, CBFuncReceive func ) NN_NOEXCEPT
        {
            this->pThis = pThisValue;
            this->messageType = type;
            this->callbackFunc = func;
        }

        Message         messageType;        //!< TBD
        CBFuncReceive   callbackFunc;         //!< TBD
        void*           pThis;          //!< TBD
    };

public:
    //---------------------------------------------------------------------------
    //! @brief TBD
    //---------------------------------------------------------------------------
    enum CallbackType
    {
        CallbackType_Before,  //!< プロシージャ前にコールされる
        CallbackType_After,   //!< プロシージャ後にコールされる
    };

    //---------------------------------------------------------------------------
    //! @brief        コンストラクタ
    //! @param[in] pHeap     TBD
    //---------------------------------------------------------------------------
    explicit CommandReceiver( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        デストラクタ
    //---------------------------------------------------------------------------
    ~CommandReceiver() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        エフェクトコマンドを取得する
    //! @return TBD
    //---------------------------------------------------------------------------
    Command* GetEffectCommand() NN_NOEXCEPT
    {
        // エフェクトコマンドを取得する
        if( m_EffectCommandQueue.Empty() || m_EffectIterator == m_EffectCommandQueue.End() )
        {
            return NULL;
        }

        return *m_EffectIterator;
    }

    //---------------------------------------------------------------------------
    //! @brief        エフェクトコマンドを追加する(文字列版)
    //! @param[in] commandId     TBD
    //! @param[in] string     TBD
    //---------------------------------------------------------------------------
    void AddEffectCommand( int commandId, const char* string ) NN_NOEXCEPT
    {
        m_Mutex.Lock();

        Command* pCommand = static_cast< Command* >( m_pHeap->Alloc( sizeof( Command ) ) );
        NN_SDK_ASSERT_NOT_NULL( pCommand );

        pCommand->Initialize();
        pCommand->SetCommand( m_pHeap, commandId, string );
        if( m_IsProcesing == false )
        {
            m_EffectCommandQueue.PushBack( pCommand );
        }
        else
        {
            // 処理中のときは一時バッファ追加します。
            m_TempEffectCommandQueue.PushBack( pCommand );
        }

        m_Mutex.Unlock();
    }

    //---------------------------------------------------------------------------
    //! @brief        エフェクトコマンドを追加する(バッファ版)
    //! @param[in] commandId     TBD
    //! @param[in] buffer     TBD
    //! @param[in] bufferSize     TBD
    //---------------------------------------------------------------------------
    void AddEffectCommand( int commandId, const void* buffer, size_t bufferSize ) NN_NOEXCEPT
    {
        m_Mutex.Lock();

        Command* pCommand = static_cast< Command* >( m_pHeap->Alloc( sizeof( Command ) ) );
        NN_SDK_ASSERT_NOT_NULL( pCommand );

        pCommand->Initialize();
        pCommand->SetCommand( m_pHeap, commandId, buffer, bufferSize );
        if( m_IsProcesing == false )
        {
            m_EffectCommandQueue.PushBack( pCommand );
        }
        else
        {
            // 処理中のときは一時バッファ追加します。
            m_TempEffectCommandQueue.PushBack( pCommand );
        }

        m_Mutex.Unlock();
    }

    //---------------------------------------------------------------------------
    //! @brief        エフェクトコマンドをpopする
    //---------------------------------------------------------------------------
    void PopEffectCommand() NN_NOEXCEPT
    {
        m_Mutex.Lock();

        if( m_EffectCommandQueue.Empty() || m_EffectIterator == m_EffectCommandQueue.End() )
        {
            return;
        }

        Command* pCommand = *m_EffectIterator;
        pCommand->~Command();

        m_pHeap->Free( pCommand );

        ++m_EffectIterator;
        if( m_EffectIterator == m_EffectCommandQueue.End() )
        {
            m_EffectCommandQueue.Clear();
        }

        m_Mutex.Unlock();
    }

    //---------------------------------------------------------------------------
    //! @brief        ロックする
    //---------------------------------------------------------------------------
    void Lock() NN_NOEXCEPT
    {
        // ロック中であれば、解除を待つ
        while( m_IsLocked )
        {
        }
        m_IsLocked = true;
    }

    //---------------------------------------------------------------------------
    //! @brief        アンロックする
    //---------------------------------------------------------------------------
    void Unlock() NN_NOEXCEPT
    {
        m_IsLocked = false;
    }

    //---------------------------------------------------------------------------
    //! @brief        処理開始（処理中にリストに追加されるかどうかを判定します）
    //---------------------------------------------------------------------------
    void BeginProc() NN_NOEXCEPT
    {
        m_Mutex.Lock();

        m_IsProcesing = true;
        m_EffectIterator = m_EffectCommandQueue.Begin();
    }

    //---------------------------------------------------------------------------
    //! @brief        処理終了
    //---------------------------------------------------------------------------
    void EndProc() NN_NOEXCEPT
    {
        m_IsProcesing = false;
        m_EffectIterator = m_EffectCommandQueue.End();

        // 一時的なキュー → 本番キューへ移行います。
        if( m_TempEffectCommandQueue.Empty() == false )
        {
            CmdFixedArray::iterator it = m_TempEffectCommandQueue.Begin();
            CmdFixedArray::iterator end = m_TempEffectCommandQueue.End();
            for( ; it != end; ++it )
            {
                m_EffectCommandQueue.PushBack( *it );
            }
            m_TempEffectCommandQueue.Clear();
        }

        m_Mutex.Unlock();
    }

    //---------------------------------------------------------------------------
    //! @brief        コマンド受信用プロシジャー
    //! @param[in] pData     TBD
    //! @param[in] size     TBD
    //---------------------------------------------------------------------------
    static void ReceiveProc( const void* pData, size_t size ) NN_NOEXCEPT;

private:
    CmdFixedArray           m_EffectCommandQueue;        //!< エフェクトコマンドキュー
    CmdFixedArray           m_TempEffectCommandQueue;    //!< 一時的なエフェクトコマンドキュー
    CmdFixedArray::iterator m_EffectIterator;        //!< コマンドイテレター
    nn::vfx::Heap*          m_pHeap;                  //!< TODO:コマンド専用のアロケータを別枠で作成して使用するように修正。
    volatile bool           m_IsLocked;                  //!< コマンド受信中かどうか
    volatile bool           m_IsProcesing;             //!< コマンド処理中かどうか
    nn::os::Mutex           m_Mutex;                 //!< TBD
public:
    static CommandReceiver* g_pCommandReceiver;           //!< TBD
};


//------------------------------------------------------------------------------
//! @brief コマンド受信クラスを生成する
//! @param[in] pHeap     vfxヒープへのポインタ
//! @return             生成したコマンド受信クラス
//------------------------------------------------------------------------------
CommandReceiver* CreateCommandReceiver( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief コマンド受信クラスを破棄する
//! @param[in] pHeap     vfxヒープへのポインタ
//------------------------------------------------------------------------------
void DestroyCommandReceiver( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
