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
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/vfx_Resource.h>

namespace nn {
namespace vfx {
namespace viewer {

class ViewerSystem;

namespace detail {

typedef void( *ViewerDataCallback )( nn::vfx::Heap* pHeap, void* pThis, void* pBinary, nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback );
typedef void( *ViewerMessageCallback )( nn::vfx::Heap* pHeap, void* pThis, void* pBinary );
typedef void( *ViewerCaptureMessageCallback )( void* pThis, const char* pCaptureFilePath, size_t captureFilePathLength );

//------------------------------------------------------------------------------
//! @brief      パケット処理するクラスです.
//------------------------------------------------------------------------------
class PacketProcedure
{
    NN_DISALLOW_COPY( PacketProcedure );
public:
    //--------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //--------------------------------------------------------------------------
    PacketProcedure() NN_NOEXCEPT
        : m_pViewerSystem( NULL )
    {
    }

    //--------------------------------------------------------------------------
    //! @brief      デストラクタ
    //--------------------------------------------------------------------------
    virtual ~PacketProcedure() NN_NOEXCEPT
    {
        Finalize();
    }

    //--------------------------------------------------------------------------
    //! @brief      初期化
    //!
    //! @param[in]      pViewerSystem        ビューアシステムです.
    //! @param[in]      pHeap      TBD
    //--------------------------------------------------------------------------
    void Initialize( ViewerSystem* pViewerSystem, nn::vfx::Heap* pHeap ) NN_NOEXCEPT
    {
        m_pViewerSystem = pViewerSystem;
        m_pHeap = pHeap;
        m_pViewerTempData = NULL;
        m_ViewerTempDataSize = 0;
        m_ViewerDataCallback = NULL;
        m_ViewerMessageCallback = NULL;
        m_ViewerCaptureMessageCallback = NULL;
        m_pViewerDataCallbackObject = NULL;
        m_pViewerMessageCallbackObject = NULL;
        m_pViewerCaptureMessageCallbackObject = NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief      終了処理です。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT
    {
        if( m_pViewerTempData )
        {
            m_pHeap->Free( m_pViewerTempData );
            m_pViewerTempData = NULL;
            m_ViewerTempDataSize = 0;
        }
    }

    //--------------------------------------------------------------------------
    //! @brief 渡されたパケットを処理します
    //!
    //! @param[in]      buffer                       パケットデータ。
    //! @param[in]      bufferSize                   パケットサイズです。
    //! @param[in]      pRegisterTextureSlotCallback ディスクリプタスロット確保関数です。
    //! @param[in]      pUserData                    ユーザデータです。
    //!
    //! @return         パケット処理内でバイナリリロードが発生する場合、true が返ります。
    //--------------------------------------------------------------------------
    bool DoPacket( char* buffer, size_t bufferSize, nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      ビューアデータ用のデータコールバックを設定します。
    //! @param[in] pThis     TBD
    //! @param[in] callback     TBD
    //--------------------------------------------------------------------------
    void SetViewerDataCallback( void* pThis, ViewerDataCallback callback ) NN_NOEXCEPT
    {
        m_pViewerDataCallbackObject = pThis;
        m_ViewerDataCallback = callback;
    }

    //--------------------------------------------------------------------------
    //! @brief      ビューアメッセージ用のデータコールバックを設定します。
    //! @param[in] pThis     TBD
    //! @param[in] callback     TBD
    //--------------------------------------------------------------------------
    void SetViewerMessageCallback( void* pThis, ViewerMessageCallback callback ) NN_NOEXCEPT
    {
        m_pViewerMessageCallbackObject = pThis;
        m_ViewerMessageCallback = callback;
    }

    //--------------------------------------------------------------------------
    //! @brief      ビューア キャプチャメッセージ用のデータコールバックを設定します。
    //! @param[in] pThis     TBD
    //! @param[in] callback     TBD
    //--------------------------------------------------------------------------
    void SetViewerCaptureMessageCallback( void* pThis, ViewerCaptureMessageCallback callback ) NN_NOEXCEPT
    {
        m_pViewerCaptureMessageCallbackObject = pThis;
        m_ViewerCaptureMessageCallback = callback;
    }

private:
    //--------------------------------------------------------------------------
    //! @brief      指定されたemitterの変数を更新します。
    //!
    //! @param[in]      pData       データ.
    //--------------------------------------------------------------------------
    void UpdateAssetEmitter( void* pData ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      ビューアデータの変数を更新します。
    //!
    //! @param[in]      offset             オフセット.
    //! @param[in]      pOverwriteData     上書きデータ.
    //! @param[in]      overrideDataSize   上書きサイズ.
    //--------------------------------------------------------------------------
    void UpdateViewerData( size_t offset, void* pOverwriteData, size_t overrideDataSize ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      制御メッセージを処理します.
    //!
    //! @param[in]      pBuffer     パケットデータ.
    //--------------------------------------------------------------------------
    void HandleMessageControl( char* pBuffer ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      バイナリ転送メッセージを処理します.
    //!
    //! @param[in]      pBuffer                  パケットデータ。
    //! @param[in]      bufferSize               バッファサイズです。
    //! @param[in]      pRegisterTextureSlotCallback ディスクリプタスロット確保関数です。
    //! @param[in]      pUserData                ユーザデータです。
    //--------------------------------------------------------------------------
    void HandleMessageSendBinary( char* pBuffer, size_t bufferSize, nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      インスタンス生成メッセージを処理します.
    //!
    //! @param[in]      pBuffer     パケットデータ.
    //--------------------------------------------------------------------------
    void HandleMessageCreate( char* pBuffer ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      リアルタイム編集メッセージを処理します。
    //!
    //! @param[in]      pBuffer     パケットデータです。
    //! @param[in]      pRegisterTextureSlotCallback ディスクリプタスロット確保関数です。
    //--------------------------------------------------------------------------
    void HandleMessageModify( char* pBuffer, nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      インスタンス削除メッセージを処理します.
    //!
    //! @param[in]      pBuffer     パケットデータです.
    //--------------------------------------------------------------------------
    void HandleMessageDelete( char* pBuffer ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      バイナリ削除メッセージを処理します.
    //!
    //! @param[in]      pBuffer     パケットデータです.
    //--------------------------------------------------------------------------
    void HandleMessageRemoveBinary( char* pBuffer ) NN_NOEXCEPT;

    //--------------------------------------------------------------------------
    //! @brief      ビューアの再生制御メッセージを処理します.
    //!
    //! @param[in]      pBuffer     パケットデータです.
    //--------------------------------------------------------------------------
    void HandleMessageViewerControl( char* pBuffer ) NN_NOEXCEPT;

    ViewerSystem*           m_pViewerSystem;                        //!< ビューアシステムです.
    nn::vfx::Heap*          m_pHeap;                                //!< ヒープ
    void*                   m_pViewerTempData;                      //!< ビューアテンポラリデータ
    size_t                  m_ViewerTempDataSize;                   //!< ビューアテンポラリデータサイズ
    ViewerDataCallback      m_ViewerDataCallback;                   //!< ビューアデータコールバック関数
    void*                   m_pViewerDataCallbackObject;            //!< ビューアデータコールバックオブジェクト
    ViewerMessageCallback   m_ViewerMessageCallback;                //!< ビューアメッセージコールバック関数
    void*                   m_pViewerMessageCallbackObject;         //!< ビューアメッセージコールバックオブジェクト
    ViewerCaptureMessageCallback  m_ViewerCaptureMessageCallback;   //!< ビューアキャプチャメッセージコールバック関数
    void*                   m_pViewerCaptureMessageCallbackObject;  //!< ビューアキャプチャメッセージコールバックオブジェクト
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn


