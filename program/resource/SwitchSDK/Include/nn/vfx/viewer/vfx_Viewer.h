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

//------------------------------------------------------------------------------
//! @namespace  nn::vfx::viewer
//! @brief      エフェクトビューワライブラリの名前空間です。
//!             このライブラリを利用すると、ゲーム中のエフェクトランタイムと
//!             EffectMaker を連携させ、リアルタイム編集が行えるようになります。
//------------------------------------------------------------------------------

#include <nn/vfx/viewer/vfx_CmdReceiver.h>
#include <nn/vfx/viewer/vfx_CmdSender.h>
#include <nn/vfx/viewer/vfx_PacketProcedure.h>
#include <nn/vfx/viewer/vfx_ResourceManager.h>
#include <nn/vfx/viewer/vfx_EffectPreview.h>
#include <nn/vfx/viewer/vfx_ToolConnector.h>
#include <nn/vfx/vfx_Resource.h>

#if (defined NN_ENABLE_HTC) && (defined NN_DISABLE_HTC)
#error can not define NN_ENABLE_HTC and NN_DISABLE_HTC at the same time.
#endif

#if (!defined NN_SDK_BUILD_LIBRARY) && ((!defined NN_ENABLE_HTC) || (defined NN_DISABLE_HTC))
#define NN_VFX_VIEWER_TOOLCONNECTOR_IS_EMPTY (1)   //NOLINT(preprocessor/const)
#else
#define NN_VFX_VIEWER_TOOLCONNECTOR_IS_EMPTY (0)   //NOLINT(preprocessor/const)
#endif

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------
//! @brief      モデルプレビュー生成コールバックの定義です。
//! @param[in]  pHeap           ヒープへのポインタ
//! @param[in]  pViewerSystem   ビューワシステムへのポインタ
//! @param[in]  pBinaryData     バイナリデータの先頭アドレス
//! @param[in]  binarySize      バイナリサイズ
//! @param[in]  guid            Guid
//---------------------------------------------------
typedef nn::vfx::viewer::detail::Preview* ( *CreateModelPreviewCallback )( nn::vfx::Heap* pHeap, ViewerSystem* pViewerSystem, void* pBinaryData, size_t binarySize, nn::vfx::viewer::detail::Guid guid );

//---------------------------------------------------
//! @brief      モデルプレビュー破棄コールバックの定義です。
//! @param[in]  pHeap       ヒープへのポインタ
//! @param[in]  pPreview    プレビューへのポインタ
//---------------------------------------------------
typedef void( *DestroyModelPreviewCallback )( nn::vfx::Heap* pHeap, Preview* pPreview );

} // namespace detail

//---------------------------------------------------------------------------
//! @brief  エフェクトビューアがモデル情報を取得するためのインターフェース。
//---------------------------------------------------------------------------
class ModelEnumerator
{
    NN_DISALLOW_COPY( ModelEnumerator );
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    ModelEnumerator() NN_NOEXCEPT
    {
    }

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~ModelEnumerator() NN_NOEXCEPT
    {
    }

public:
    //---------------------------------------------------------------------------
    //! @brief      モデル数を取得します。
    //! @return     モデル数
    //---------------------------------------------------------------------------
    virtual int GetModelCount() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief      モデル名を取得します。
    //! @param[in]  index   インデックス
    //! @return             モデル名
    //---------------------------------------------------------------------------
    virtual const char* GetModelName( int index ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief      モデルのボーン数を取得します。
    //! @param[in]  modelName   モデル名
    //! @return                 モデルのボーン数
    //---------------------------------------------------------------------------
    virtual int GetBoneCount( const char* modelName ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief      モデルのボーン名を取得します。
    //! @param[in]  modelName   モデル名
    //! @param[in]  index       インデックス
    //! @return                 ボーン名
    //---------------------------------------------------------------------------
    virtual const char* GetBoneName( const char* modelName, int index ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief      ボーンマトリクスを取得します。
    //! @param[out] pOutBoneMatrix  ボーンマトリクスを格納するポインタ
    //! @param[in]  modelName       モデル名
    //! @param[in]  index           インデックス
    //---------------------------------------------------------------------------
    virtual void GetBoneMatrix( nn::util::Matrix4x3fType* pOutBoneMatrix, const char* modelName, int index ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief      モデルのルートマトリクスを取得します。
    //! @param[out] pOutRootMatrix  ルートマトリクスを格納するポインタ
    //! @param[in]  modelName       モデル名
    //---------------------------------------------------------------------------
    virtual void GetModelRootMatrix( nn::util::Matrix4x3fType* pOutRootMatrix, const char* modelName ) const NN_NOEXCEPT = 0;
};

namespace detail {
//------------------------------------------------------------------------------
//! @brief 互換性の目的で定義
//! @details
//! 本関数は互換性の目的で定義されました。 エフェクトビューアがモデル情報を取得するためのインターフェースです。
//------------------------------------------------------------------------------
typedef nn::vfx::viewer::ModelEnumerator  ModelEnumerator;

};

//---------------------------------------------------------------------------
//! @brief        vfx ビューアシステム
//!               詳細は@confluencelink{124007414,ビューアランタイムを組み込む}をご覧下さい。
//---------------------------------------------------------------------------
class ViewerSystem
{
    NN_DISALLOW_COPY( ViewerSystem );
public:
    //---------------------------------------------------------------------------
    //! @brief  ビューア定数。
    //---------------------------------------------------------------------------
    enum ViewerSystemSettings
    {
        ViewerSystemSettings_MaxBindEmitterSetCount = 16    //!< リソースバインド可能な最大数
    };

    //---------------------------------------------------------------------------
    //! @brief      コンストラクタです。
    //! @param[in]  pHeap                   ビューアライブラリ内で利用されるvfxヒープインスタンスを指定します。
    //! @param[in]  pConnectionHeap         通信用途に利用されるvfxヒープインスタンスを指定します。
    //!                                     通信用スレッドから使用されスレッドセーフなヒープを必要としますので渡します。
    //! @param[in]  pSystem                 vfxシステムのインスタンスを指定します。
    //! @param[in]  noUseInternalThread     外部スレッドを利用する場合はtrueを指定します。
    //! @param[in]  priority                スレッドのプライオリティを指定します。デフォル引数では、nn::os::LowestThreadPriority が指定される為、
    //!                                     うまくEffectMakerと通信できない場合はプライオリティを上げる等の調整が必要です。
    //!                                     詳細は@confluencelink{124007414,ビューアランタイムを組み込む}をご覧下さい。
    //! @details                            アプリケーション側でNN_ENABLE_HTC又は、NN_DISABLE_HTCのコンパイルスイッチを定義
    //!                                     すると、通信の接続又は非接続を切り替えられます。
    //---------------------------------------------------------------------------
    explicit ViewerSystem( nn::vfx::Heap* pHeap, nn::vfx::Heap* pConnectionHeap, nn::vfx::System* pSystem, bool noUseInternalThread = false, int priority = nn::os::LowestThreadPriority ) NN_NOEXCEPT : m_pTransmitBuffer ( nullptr )
    {
#if NN_VFX_VIEWER_TOOLCONNECTOR_IS_EMPTY
        m_pToolConnectorPtr = pHeap->Alloc( sizeof( nn::vfx::viewer::detail::ToolConnector ) );
        nn::vfx::viewer::detail::ToolConnector* pToolConnector = new ( m_pToolConnectorPtr ) nn::vfx::viewer::detail::ToolConnector;
#else
        m_pToolConnectorPtr = pHeap->Alloc( sizeof( nn::vfx::viewer::detail::ToolConnectorActual ) );
        nn::vfx::viewer::detail::ToolConnector* pToolConnector = new ( m_pToolConnectorPtr ) nn::vfx::viewer::detail::ToolConnectorActual;
#endif

        Initialize( pHeap, pConnectionHeap, pSystem, pToolConnector, noUseInternalThread, priority );
    }

    //---------------------------------------------------------------------------
    //! @brief      コンストラクタです。
    //! @param[in]  pHeap                   ビューアライブラリ内で利用されるvfxヒープインスタンスを指定します。
    //!                                     transmitBufferSizeサイズ分の通信用ヒープがpHeapから確保されますので十分な大きさで作成してください。
    //! @param[in]  pSystem                 vfxシステムのインスタンスを指定します。
    //! @param[in]  transmitBufferSize      通信用ヒープサイズを指定します。8Mbyte位を目安に必要であれば増やしてください。
    //! @param[in]  noUseInternalThread     外部スレッドを利用する場合はtrueを指定します。
    //! @param[in]  priority                スレッドのプライオリティを指定します。デフォル引数では、nn::os::LowestThreadPriority が指定される為、
    //!                                     うまくEffectMakerと通信できない場合はプライオリティを上げる等の調整が必要です。
    //!                                     詳細は@confluencelink{124007414,ビューアランタイムを組み込む}をご覧下さい。
    //! @details                            アプリケーション側でNN_ENABLE_HTC又は、NN_DISABLE_HTCのコンパイルスイッチを定義
    //!                                     すると、通信の接続又は非接続を切り替えられます。
    //---------------------------------------------------------------------------
    explicit ViewerSystem( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, size_t transmitBufferSize, bool noUseInternalThread = false, int priority = nn::os::LowestThreadPriority ) NN_NOEXCEPT : m_pTransmitBuffer( nullptr )
    {

#if NN_VFX_VIEWER_TOOLCONNECTOR_IS_EMPTY
        m_pToolConnectorPtr = pHeap->Alloc( sizeof( nn::vfx::viewer::detail::ToolConnector ) );
        nn::vfx::viewer::detail::ToolConnector* pToolConnector = new ( m_pToolConnectorPtr ) nn::vfx::viewer::detail::ToolConnector;
#else
        m_pToolConnectorPtr = pHeap->Alloc( sizeof( nn::vfx::viewer::detail::ToolConnectorActual ) );
        nn::vfx::viewer::detail::ToolConnector* pToolConnector = new ( m_pToolConnectorPtr ) nn::vfx::viewer::detail::ToolConnectorActual;
#endif
        Internal_Constructor( pHeap, pSystem, pToolConnector, transmitBufferSize, noUseInternalThread, priority );
    }

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    ~ViewerSystem() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      終了処理です。
    //! @param[in]  pUnregisterTextureSlotCallback  ディスクリプタスロット解放関数です。
    //! @param[in]  pUserData                       ユーザデータです。
    //---------------------------------------------------------------------------
    void Finalize( nn::vfx::UnregisterTextureViewSlot pUnregisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      アプリケーションの初期化完了を通知します。
    //---------------------------------------------------------------------------
    void SetAppIsInitialized() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ビューアが再生中のエフェクトを再生(一時停止解除)します。
    //---------------------------------------------------------------------------
    void Play() NN_NOEXCEPT
    {
        m_IsPaused = false;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアが再生中のエフェクトを一時停止します。
    //---------------------------------------------------------------------------
    void Pause() NN_NOEXCEPT
    {
        m_IsPaused = true;
    }

    //---------------------------------------------------------------------------
    //! @brief      一時停止中かチェックします。
    //! @return     true で停止中、false で再生中。
    //---------------------------------------------------------------------------
    bool IsPause() const NN_NOEXCEPT
    {
        return m_IsPaused;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアが再生中のエフェクトを停止します。
    //---------------------------------------------------------------------------
    void Stop() NN_NOEXCEPT
    {
        ResetTime( false );
        m_IsPaused = true;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアが再生中のエフェクトをコマ送りします。
    //---------------------------------------------------------------------------
    void StepFrame() NN_NOEXCEPT
    {
        m_IsStepMode = true;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアが再生中のエフェクトを強制的にフェードさせます。
    //---------------------------------------------------------------------------
    void ForceFade() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      定期処理を実行します。
    //! @param[in]  frameRate                       再生レート
    //! @param[in]  viewMatrix                      ビューマトリクス。エミッタビルボード利用時に参照されます。
    //! @param[in]  pRegisterTextureSlotCallback    テクスチャディスクリプタスロット確保用コールバック関数。
    //!                                             NULL指定も可能ですが、その場合はユーザー側でリソース内ディスクリプタスロットスロットを
    //!                                             nn::vfx::Resource::RegisterTextureViewToDescriptorPoolを利用して更新してください。
    //! @param[in]  pUserData                       テクスチャディスクリプタスロット確保用コールバック関数に渡すユーザーデータポインタ。
    //! @return                                     再生中の場合は true
    //---------------------------------------------------------------------------
    bool ProcessCalculation( float frameRate, const nn::util::Matrix4x3fType& viewMatrix, nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      タイムを 0 に戻します。
    //! @param[in]  isFade タイムを 0 に戻すタイミングで再生中エフェクトをフェードさせるか指定します。
    //------------------------------------------------------------------------------
    void ResetTime( bool isFade ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      タイムを取得します。
    //! @return     再生タイム
    //------------------------------------------------------------------------------
    float GetTime() const NN_NOEXCEPT
    {
        return m_Time;
    }

    //------------------------------------------------------------------------------
    //! @brief      終了タイムを取得します。
    //! @return     終了タイム
    //------------------------------------------------------------------------------
    float GetEndTime() const NN_NOEXCEPT
    {
        return m_EndTime;
    }

    //---------------------------------------------------------------------------
    //! @brief      ループ再生中か取得します。
    //! @return     true でループ状態、false で通常状態
    //---------------------------------------------------------------------------
    bool IsLoop() const NN_NOEXCEPT
    {
        return m_IsLoop;
    }

    //---------------------------------------------------------------------------
    //! @brief      ProcessCalculationメソッド内でバイナリリロードが発生したか取得します。
    //! @return     true でループ状態、false で通常状態
    //---------------------------------------------------------------------------
    bool IsReloadBinary() const NN_NOEXCEPT
    {
        return m_IsReloadBinary;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生範囲とループの設定をします。
    //! @param[in]  startFrame  開始フレーム( 現状動作しません )
    //! @param[in]  endFrame    終了フレーム
    //! @param[in]  isLoop      ループ再生するか
    //---------------------------------------------------------------------------
    void SetFrameRange( float startFrame, float endFrame, bool isLoop ) NN_NOEXCEPT
    {
        NN_UNUSED( startFrame );
        //mStartFrame = startFrame;
        m_EndTime     = endFrame;
        m_IsLoop      = isLoop;
    }

    //---------------------------------------------------------------------------
    //! @brief      エフェクトシステムのポインタを取得します。
    //! @return     エフェクトシステムのポインタ
    //---------------------------------------------------------------------------
    nn::vfx::System* GetVfxSystem() NN_NOEXCEPT
    {
        return m_pSystem;
    }

    //---------------------------------------------------------------------------
    //! @brief      エフェクトメーカーと接続済みかどうかを取得します。
    //! @return 接続済みかどうか
    //---------------------------------------------------------------------------
    bool IsConnected() NN_NOEXCEPT
    {
        return m_IsConnected;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアデータのコールバックを設定します。
    //! @param[in]  pThis       呼び出されるコールバック関数の引数に渡すオブジェクト
    //! @param[in]  callback    コールバック関数
    //---------------------------------------------------------------------------
    void SetViewerDataCallBack( void* pThis, detail::ViewerDataCallback callback ) NN_NOEXCEPT
    {
        m_PacketProcedure.SetViewerDataCallback(pThis, callback);
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアメッセージのコールバックを設定します。
    //! @param[in]  pThis       呼び出されるコールバック関数の引数に渡すオブジェクト
    //! @param[in]  callback    コールバック関数
    //---------------------------------------------------------------------------
    void SetViewerMessageCallBack(void* pThis, detail::ViewerMessageCallback callback) NN_NOEXCEPT
    {
        m_PacketProcedure.SetViewerMessageCallback(pThis, callback);
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアキャプチャメッセージのコールバックを設定します。
    //! @param[in]  pThis       呼び出されるコールバック関数の引数に渡すオブジェクト
    //! @param[in]  callback    コールバック関数
    //---------------------------------------------------------------------------
    void SetViewerCaptureMessageCallBack( void* pThis, detail::ViewerCaptureMessageCallback callback ) NN_NOEXCEPT
    {
        m_PacketProcedure.SetViewerCaptureMessageCallback( pThis, callback );
    }

    //---------------------------------------------------------------------------
    //! @brief  プレビューの先頭インスタンスを取得します。
    //! @return リスト先頭のプレビュー
    //---------------------------------------------------------------------------
    detail::Preview* GetPreviewHead() NN_NOEXCEPT
    {
        return &m_PreviewHead;
    }

    //---------------------------------------------------------------------------
    //! @brief      バイナリをEffectMakerへ送信します。
    //! @param[in]  pBinaryData 送信したいバイナリの先頭ポインタ
    //! @param[in]  binarySize  バイナリのサイズ
    //---------------------------------------------------------------------------
    void SendBinaryData(void* pBinaryData, size_t binarySize);

    //---------------------------------------------------------------------------
    //! @brief      モデルプレビュー生成コールバックを設定します。
    //! @param[in]  callback    生成コールバック関数
    //---------------------------------------------------------------------------
    void SetCreateModelPreviewCallback(detail::CreateModelPreviewCallback callback) NN_NOEXCEPT
    {
        m_CreateModelPreviewCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief      モデルプレビュー破棄コールバックを設定します。
    //! @param[in]  callback    破棄コールバック関数
    //---------------------------------------------------------------------------
    void SetDestroyModelPreviewCallback(detail::DestroyModelPreviewCallback callback) NN_NOEXCEPT
    {
        m_DestroyModelPreviewCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief      エフェクトプレビューを生成します。
    //! @param[in]  resGuid     リソースの Guid
    //! @param[in]  prevGuid    プレビューの Guid
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool CreateEffectPreview( nn::vfx::viewer::detail::Guid resGuid, nn::vfx::viewer::detail::Guid prevGuid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エフェクトバイナリの読み込みを行います。
    //! @param[in]  pPath                           バイナリのパス
    //! @param[in]  guid                            Guid
    //! @param[in]  pRegisterTextureSlotCallback    テクスチャスロット登録コールバック
    //! @param[in]  pUserData                       ユーザーデータへのポインタ
    //! @return                                     成功した場合 true
    //---------------------------------------------------------------------------
    bool LoadEffectData( const char* pPath, nn::vfx::viewer::detail::Guid guid, nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エフェクトバイナリの破棄を行います。
    //! @param[in]  guid    Guid
    //! @return             成功した場合 true
    //---------------------------------------------------------------------------
    bool UnLoadEffectData( nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      モデルプレビューを生成します。
    //! @param[in]  guid        Guid
    //! @param[in]  pData       データへのポインタ
    //! @param[in]  dataSize    データサイズ
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool CreateModelPreview( nn::vfx::viewer::detail::Guid guid, void* pData, size_t dataSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      モデルプレビューを破棄します。
    //! @param[in]  guid        Guid
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool DestroyModelPreview( nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エフェクトプレビューリソースをエフェクトプレビューに設定します。
    //! @param[in]  prevGuid    プレビューのGuid
    //! @param[in]  pResource   リソースへのポインタ
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool SetResEffectPreview( nn::vfx::viewer::detail::Guid prevGuid, detail::ResEffectPreviewPack* pResource ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      モデルプレビューリソースをモデルプレビューに設定します。
    //! @param[in]  prevGuid    プレビューのGuid
    //! @param[in]  pResource   リソースへのポインタ
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool SetResModelPreview( nn::vfx::viewer::detail::Guid prevGuid, detail::ResModelPreviewPack* pResource ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定リソースをバインドします。
    //! @param[in]  resGuid     リソースの Guid
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool BindResource( nn::vfx::viewer::detail::Guid resGuid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定リソースをアンバインドします。
    //! @param[in]  resGuid     リソースの Guid
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool UnbindResource( nn::vfx::viewer::detail::Guid resGuid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定リソースをアンバインドします。
    //! @param[in]  resourceId  リソース ID
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool UnbindResource( int resourceId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エフェクトプレビューリソースを更新します。
    //! @param[in]  prevGuid            プレビューの Guid
    //! @param[in]  offset              バイナリオフセット
    //! @param[in]  pOverwriteData      上書きするデータの先頭アドレス
    //! @param[in]  overwriteDataSize   上書きするデータのサイズ
    //! @param[in]  endian              エンディアン指定
    //! @return                         成功した場合 true
    //---------------------------------------------------------------------------
    bool UpdateResEffectPreview( nn::vfx::viewer::detail::Guid prevGuid,
                                 size_t                        offset,
                                 void*                         pOverwriteData,
                                 size_t                        overwriteDataSize,
                                 uint8_t                       endian ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      モデルプレビューリソースを更新します。
    //! @param[in]  prevGuid            プレビューの Guid
    //! @param[in]  offset              バイナリオフセット
    //! @param[in]  pOverwriteData      上書きするデータの先頭アドレス
    //! @param[in]  overwriteDataSize   上書きするデータのサイズ
    //! @param[in]  endian              エンディアン指定
    //! @return                         成功した場合 true
    //---------------------------------------------------------------------------
    bool UpdateResModelPreview( nn::vfx::viewer::detail::Guid prevGuid,
                                size_t                        offset,
                                void*                         pOverwriteData,
                                size_t                        overwriteDataSize,
                                uint8_t                       endian ) NN_NOEXCEPT;

    //------------------------------------------------
    //! @brief      指定されたEmitterSetの変数を更新します。
    //! @param[in]  resGuid             リソースの Guid
    //! @param[in]  offset              バイナリオフセット
    //! @param[in]  pOverwriteData      上書きするデータの先頭アドレス
    //! @param[in]  overwriteDataSize   上書きするデータのサイズ
    //! @param[in]  endian              エンディアン指定
    //------------------------------------------------
    void UpdateAssetEmitterSet( nn::vfx::viewer::detail::Guid resGuid,
                                size_t                        offset,
                                void*                         pOverwriteData,
                                size_t                        overwriteDataSize,
                                uint8_t                       endian ) NN_NOEXCEPT;

    //------------------------------------------------
    //! @brief      指定されたemitterの変数を更新します。
    //! @param[in]  resGuid             リソースの Guid
    //! @param[in]  emitterName         エミッタ名
    //! @param[in]  offset              バイナリオフセット
    //! @param[in]  pOverwriteData      上書きするデータの先頭アドレス
    //! @param[in]  overwriteDataSize   上書きするデータのサイズ
    //! @param[in]  withReset           リセットをするかどうか
    //! @param[in]  endian              エンディアン指定
    //------------------------------------------------
    void UpdateAssetEmitter( nn::vfx::viewer::detail::Guid resGuid,
                             const char*                   emitterName,
                             size_t                        offset,
                             void*                         pOverwriteData,
                             size_t                        overwriteDataSize,
                             bool                          withReset,
                             uint8_t                       endian ) NN_NOEXCEPT;

    //------------------------------------------------
    //! @brief      指定されたエミッタセットのビジビリティを設定します。
    //! @param[in]  resGuid     リソースの Guid
    //! @param[in]  visible     ビジビリティ設定
    //------------------------------------------------
    void SetEmitterSetVisibility( nn::vfx::viewer::detail::Guid resGuid, bool visible ) NN_NOEXCEPT;

    //------------------------------------------------
    //! @brief      指定されたエミッタのビジビリティを設定します。
    //! @param[in]  resGuid         リソースの Guid
    //! @param[in]  emitterName     エミッタ名
    //! @param[in]  visible         ビジビリティ設定
    //------------------------------------------------
    void SetEmitterVisibility( nn::vfx::viewer::detail::Guid resGuid, const char* emitterName, bool visible ) NN_NOEXCEPT;

    //------------------------------------------------
    //! @brief      指定されたプレビューのビジビリティを設定します。
    //! @param[in]  prevGuid    プレビューの Guid
    //! @param[in]  visible     ビジビリティ設定
    //------------------------------------------------
    void SetPreviewVisibility( nn::vfx::viewer::detail::Guid prevGuid, bool visible ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      プレビューをリストに追加します。
    //! @param[in]  pPreview    プレビューへのポインタ
    //---------------------------------------------------------------------------
    void AddPreview( detail::Preview* pPreview ) NN_NOEXCEPT
    {
        m_PreviewHead.AddPreview( pPreview );
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットを作成します。
    //! @param[in]  emitterSetName  エミッタセット名
    //---------------------------------------------------------------------------
    void CreateViewSysEmitterSet( const char* emitterSetName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      全てのプレビューを破棄します。
    //---------------------------------------------------------------------------
    void DestroyAllPreview() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      全てのリソースを破棄します。
    //! @param[in]  pUnregisterTextureSlotCallback  テクスチャスロット破棄コールバック
    //! @param[in]  pUserData                       ユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    void DestroyAllResource(nn::vfx::UnregisterTextureViewSlot pUnregisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ビューアセンターを設定します。
    //! @param[in]  centerMatrix    ビューアセンターを指定する行列
    //---------------------------------------------------------------------------
    void SetViewerCenter( const nn::util::Matrix4x3fType& centerMatrix ) NN_NOEXCEPT
    {
        m_ViewerCenterMatrix = centerMatrix;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビューアセンターを取得します。
    //! @return     ビューアセンターを指定する行列
    //---------------------------------------------------------------------------
    const nn::util::Matrix4x3fType& GetViewerCenter() const NN_NOEXCEPT
    {
        return m_ViewerCenterMatrix;
    }

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 共通処理
    //! @param[in]  type                    nn::vfx::viewer::detail::RequestEmitterSetMessageType 型のメッセージタイプ
    //! @param[in]  emitterSetName          エミッタセット名
    //! @param[in]  duplicateEmitterSetName 複製されるエミッタセット名
    //-----------------------------------------------------------------------------------------------------
    void RequestEset( uint32_t type, const char* emitterSetName, const char* duplicateEmitterSetName = NULL ) NN_NOEXCEPT;

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 エミッタセットファイルオープン
    //! @param[in]  emitterSetFilePath  エミッタセットファイルパス
    //-----------------------------------------------------------------------------------------------------
    void RequestEsetFileOpen( const char* emitterSetFilePath ) NN_NOEXCEPT;

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 エミッタセットファイルクローズ
    //! @param[in]  emitterSetFilePath  エミッタセットファイルパス
    //-----------------------------------------------------------------------------------------------------
    void RequestEsetFileClose( const char* emitterSetFilePath ) NN_NOEXCEPT;

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 新規エミッタセット作成
    //! @param[in]  emitterSetFilePath  エミッタセットファイルパス
    //-----------------------------------------------------------------------------------------------------
    void RequestEsetFileCreate( const char* emitterSetFilePath ) NN_NOEXCEPT;

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 エミッタセット複製
    //! @param[in]  srcEmitterSetName           エミッタセットファイルパス
    //! @param[in]  duplicateEmitterSetName     複製されるエミッタセット名
    //-----------------------------------------------------------------------------------------------------
    void RequestEsetFileDuplicate( const char* srcEmitterSetName, const char* duplicateEmitterSetName ) NN_NOEXCEPT;

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 カスタムシェーダのパスを設定する
    //! @param[in]  customShaderPath    ファイルパス
    //! @param[in]  forceAssign         強制アサイン
    //-----------------------------------------------------------------------------------------------------
    void RequestSetCustomShaderPath( const char* customShaderPath, bool forceAssign ) NN_NOEXCEPT
    {
        NN_UNUSED( customShaderPath );
        NN_UNUSED( forceAssign );
    }

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 カスタムアクションのパスを設定する
    //! @param[in]  customActionPath    ファイルパス
    //! @param[in]  forceAssign         強制アサイン
    //-----------------------------------------------------------------------------------------------------
    void RequestSetCustomActionPath( const char* customActionPath, bool forceAssign ) NN_NOEXCEPT
    {
        NN_UNUSED( customActionPath );
        NN_UNUSED( forceAssign );
    }

    //-----------------------------------------------------------------------------------------------------
    //! @brief      エフェクトメーカー操作 ユーザーコンフィグのパスを設定する
    //! @param[in]  userConfigPath  ファイルパス
    //! @param[in]  forceAssign     強制アサイン
    //-----------------------------------------------------------------------------------------------------
    void RequestSetUserConfigPath( const char* userConfigPath, bool forceAssign ) NN_NOEXCEPT
    {
        NN_UNUSED( userConfigPath );
        NN_UNUSED( forceAssign );
    }

    //---------------------------------------------------------------------------
    //! @brief      カスタムシェーダパスを切り替える。
    //! @param[in]  filePath    ファイルパス
    //---------------------------------------------------------------------------
    void SetCustomShaderPath( const char* filePath ) NN_NOEXCEPT
    {
        return SetCustomShaderDefinitionFile( filePath );
    }

    //---------------------------------------------------------------------------
    //! @brief      カスタムシェーダ定義ファイルパスを切り替える。
    //! @param[in]  filePath    ファイルパス
    //---------------------------------------------------------------------------
    void SetCustomShaderDefinitionFile( const char* filePath ) NN_NOEXCEPT
    {
        NN_UNUSED( filePath );
    }

    //---------------------------------------------------------------------------
    //! @brief      カスタムアクションパスを切り替える。
    //! @param[in]  filePath    ファイルパス
    //---------------------------------------------------------------------------
    void SetCustomActionPath( const char* filePath ) NN_NOEXCEPT
    {
        NN_UNUSED( filePath );
    }

    //---------------------------------------------------------------------------
    //! @brief      ユーザーコンフィグファイルを切り替える。
    //! @param[in]  filePath    ファイルパス
    //---------------------------------------------------------------------------
    void SetUserConfigPath( const char* filePath ) NN_NOEXCEPT
    {
        return SetUserConfigFile( filePath );
    }

    //---------------------------------------------------------------------------
    //! @brief      ユーザーコンフィグファイルを切り替える。
    //! @param[in]  filePath    ファイルパス
    //---------------------------------------------------------------------------
    void SetUserConfigFile( const char* filePath ) NN_NOEXCEPT
    {
        NN_UNUSED( filePath );
    }

    //---------------------------------------------------------------------------
    //! @brief  アプリケーション初期化完了メッセージ  を送信する。
    //---------------------------------------------------------------------------
    void SendAppIsInitialized() NN_NOEXCEPT
    {
        nn::vfx::viewer::detail::ViewerHeaderMessage msg;
        msg.type = nn::vfx::viewer::detail::ViewerMessageType_ApplicationInitialized;
        msg.size = 0;

        detail::CommandSender* pCommandSender = m_ToolConnector->GetCommandSender();
        if ( pCommandSender != NULL )
        {
            pCommandSender->SendCommand(
                nn::vfx::viewer::detail::MessageType_SendBinary,
                &msg, sizeof( nn::vfx::viewer::detail::ViewerHeaderMessage ) );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief      SendLinkedEmitterSetV2E  を送信する。
    //! @param[in]  emitterSetName  エミッタセット名
    //---------------------------------------------------------------------------
    void SendLinkedEmitterSetV2E( const char* emitterSetName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      SendUnLinkedEmitterSetV2E  を送信する。
    //! @param[in]  emitterSetName  エミッタセット名
    //---------------------------------------------------------------------------
    void SendUnLinkedEmitterSetV2E( const char* emitterSetName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      コマンド処理を実行します。
    //! @param[in]  pRegisterTextureSlotCallback    テクスチャディスクリプタスロット確保用コールバック関数。
    //! @param[in]  pUserData                       テクスチャディスクリプタスロット確保用コールバック関数に渡すユーザーデータポインタ。
    //---------------------------------------------------------------------------
    void ExecuteCommand(nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定リソースIDのエフェクトプレビューを削除。
    //! @param[in]  resourceId  リソース ID
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool RemoveEffectPreviewFromResId( int resourceId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パケットの受信処理。
    //! @param[in]  pArg  nn::vfx::viewer::ViwerSystemのポインタ
    //---------------------------------------------------------------------------
    static void ReadingPacketProcess( void* pArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     パケットの受信処理の終了。
    //---------------------------------------------------------------------------
    void StopReadingPacketProcessThread() NN_NOEXCEPT;


    //----------------------------------------
    //! @name EffectMakerにアプリ内モデル情報を登録、削除 する
    //@{

    //---------------------------------------------------------------------------
    //! @brief      EffectMakerのアプリ内モデル情報を更新する
    //! @param[in]  pModelEnumerator    モデル情報取得インタフェースクラス
    //---------------------------------------------------------------------------
    void UpdateModelInfo( ModelEnumerator* pModelEnumerator ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      EffectMakerにアプリ内モデル情報を追加する
    //! @param[in]  pModelEnumerator    モデル情報取得インタフェースクラス
    //! @param[in]  pAddModelName       追加するモデル名
    //---------------------------------------------------------------------------
    bool AddModelInfo( ModelEnumerator* pModelEnumerator, const char* pAddModelName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      EffectMakerにアプリ内モデル情報を追加する(内部使用)
    //! @param[in]  pModelEnumerator    モデル情報取得インタフェースクラス
    //! @param[in]  pAddModelName       追加するモデル名
    //! @param[in]  pGuid               追加するモデルのGuid
    //---------------------------------------------------------------------------
    bool AddModelInfoInternal( ModelEnumerator* pModelEnumerator, const char* pAddModelName, detail::Guid* pGuid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      EffectMakerにアプリ内の指定モデル情報を削除する
    //! @param[in]  pModelName       削除するモデル名
    //---------------------------------------------------------------------------
    bool RemoveModelInfo( const char* pModelName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      BeginSendModelInfoV2E を送信する。
    //!             EffectMakerに対してアプリ内モデル情報を送信を開始することを通知します。
    //---------------------------------------------------------------------------
    void SendBeginPreviewModelV2E() NN_NOEXCEPT
    {
        nn::vfx::viewer::detail::ModelHeaderMessage header;
        header.type = nn::vfx::viewer::detail::ModelMessageType_BeginSendModelInfo;
        header.size = 0;

       detail::CommandSender* pCommandSender = m_ToolConnector->GetCommandSender();
        if ( pCommandSender != NULL )
        {
            pCommandSender->SendCommand(
                nn::vfx::viewer::detail::MessageType_ModelInfo,
                &header, sizeof( nn::vfx::viewer::detail::ModelHeaderMessage ) );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief      SendModelInfoV2E を送信する。
    //!             EffectMakerに対してモデル情報を送信します。
    //!             このメソッドの前後で、SendBeginPreviewModelV2E / SendEndPreviewModelV2E が呼ばれている必要があります。
    //! @param[in]  pPacket     モデル情報パケットへのポインタ
    //! @param[in]  packetSize  モデル情報パケットサイズ
    //---------------------------------------------------------------------------
    void SendModelInfoV2E( void* pPacket, size_t packetSize ) NN_NOEXCEPT
    {
        NN_UNUSED( pPacket );
        NN_UNUSED( packetSize );

        detail::CommandSender* pCommandSender = m_ToolConnector->GetCommandSender();
        if ( pCommandSender != NULL )
        {
            pCommandSender->SendCommand( nn::vfx::viewer::detail::MessageType_ModelInfo, pPacket, packetSize );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief      EndSendModelInfoV2E  を送信する。
    //!             EffectMakerに対してアプリ内モデル情報を送信が終了したことを通知します。
    //---------------------------------------------------------------------------
    void SendEndPreviewModelV2E() NN_NOEXCEPT
    {
        nn::vfx::viewer::detail::ModelHeaderMessage header;
        header.type = nn::vfx::viewer::detail::ModelMessageType_EndSendModelInfo;
        header.size = 0;

        detail::CommandSender* pCommandSender = m_ToolConnector->GetCommandSender();
        if ( pCommandSender != NULL )
        {
            pCommandSender->SendCommand(
                nn::vfx::viewer::detail::MessageType_ModelInfo,
                &header, sizeof( nn::vfx::viewer::detail::ModelHeaderMessage ) );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief      ClearPreviewModelV2E を送信する。
    //!             EffectMakerに対してアプリ内モデル情報をクリアすることを通知します。
    //---------------------------------------------------------------------------
    void SendClearPreviewModelV2E() NN_NOEXCEPT
    {
        nn::vfx::viewer::detail::ModelHeaderMessage header;
        header.type = nn::vfx::viewer::detail::ModelMessageType_ClearPreviewModel;
        header.size = 0;

        detail::CommandSender* pCommandSender = m_ToolConnector->GetCommandSender();
        if ( pCommandSender != NULL )
        {
            pCommandSender->SendCommand(
                nn::vfx::viewer::detail::MessageType_ModelInfo,
                &header, sizeof( nn::vfx::viewer::detail::ModelHeaderMessage ) );
        }

        m_SentModelInfoCount = 0;
    }


    //@}

private:
    //---------------------------------------------------------------------------
    //! @brief      コンストラクタの内部処理です。
    //! @param[in]  pHeap                   ビューアライブラリ内で利用されるvfxヒープインスタンスを指定します。
    //!                                     transmitBufferSizeサイズ分の通信用ヒープがpHeapから確保されますので十分な大きさで作成してください。
    //! @param[in]  pSystem                 vfxシステムのインスタンスを指定します。
    //! @param[in]  pToolConnector          ツールコネクタのインスタンスを指定します。
    //! @param[in]  transmitBufferSize      通信用ヒープサイズを指定します。8Mbyte位を目安に必要であれば増やしてください。
    //! @param[in]  noUseInternalThread     外部スレッドを利用する場合はtrueを指定します。
    //! @param[in]  priority                スレッドのプライオリティを指定します。デフォル引数では、nn::os::LowestThreadPriority が指定される為、
    //!                                     うまくEffectMakerと通信できない場合はプライオリティを上げる等の調整が必要です。
    //!                                     詳細は@confluencelink{124007414,ビューアランタイムを組み込む}をご覧下さい。
    //---------------------------------------------------------------------------
    void  Internal_Constructor( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, nn::vfx::viewer::detail::ToolConnector* pToolConnector, size_t transmitBufferSize, bool noUseInternalThread, int priority ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      初期化処理です。
    //! @param[in]  pHeap                   ビューアライブラリ内で利用されるvfxヒープインスタンスを指定します。
    //! @param[in]  pConnectionHeap         通信用途に利用されるvfxヒープインスタンスを指定します。
    //! @param[in]  pSystem                 vfxシステムのインスタンスを指定します。
    //! @param[in]  pToolConnector          ツールコネクタのインスタンスを指定します。
    //! @param[in]  noUseInternalThread     外部スレッドを利用する場合はtrueを指定します。
    //! @param[in]  priority                スレッドのプライオリティを指定します。デフォル引数では、nn::os::LowestThreadPriority が指定される為、
    //!                                     うまくEffectMakerと通信できない場合はプライオリティを上げる等の調整が必要です。
    //!                                     詳細は@confluencelink{124007414,ビューアランタイムを組み込む}をご覧下さい。
    //---------------------------------------------------------------------------
    void Initialize( nn::vfx::Heap* pHeap, nn::vfx::Heap* pConnectionHeap, nn::vfx::System* pSystem, nn::vfx::viewer::detail::ToolConnector* pToolConnector, bool noUseInternalThread, int priority ) NN_NOEXCEPT;


private:
    detail::PacketProcedure             m_PacketProcedure;                  //!< パケットプロシージャ
    detail::ResourceManager             m_ResManager;                       //!< リソースマネージャ
    detail::Preview                     m_PreviewHead;                      //!< プレビューの先頭
    nn::util::Matrix4x3fType            m_ViewerCenterMatrix;               //!< ビューアのセンター
    nn::vfx::System*                    m_pSystem;                          //!< ビューアが再生に利用するエフェクトシステム
    nn::vfx::Heap*                      m_pViewerHeap;                      //!< ビューアヒープ
    nn::vfx::Heap*                      m_pConnectionHeap;                  //!< コネクションヒープ
    float                               m_Time;                             //!< ビューアシステムのタイム
    float                               m_FrameRate;                        //!< 再生フレームレート
    float                               m_EndTime;                          //!< ループ再生中の終了フレーム
    bool                                m_IsLoop;                           //!< ループ再生中かどうか。
    bool                                m_IsPaused;                         //!< ポーズ中かどうか。
    bool                                m_IsStepMode;                       //!< コマ送り
    bool                                m_IsAppInitialized;                 //!< アプリケーションが初期化済みかどうか
    bool                                m_IsConnected;                      //!< 接続中かどうか
    bool                                m_IsResetTime;                      //!< タイムをリセットするかどうか
    bool                                m_IsReloadBinary;                   //!< バイナリがリロードされたか
    bool                                m_IsFinalized;                      //!< 終了処理は行われたか
    detail::CreateModelPreviewCallback  m_CreateModelPreviewCallback;       //!< モデルプレビュー生成コールバック
    detail::DestroyModelPreviewCallback m_DestroyModelPreviewCallback;      //!< モデルプレビュー破棄コールバック
    nn::vfx::viewer::detail::ToolConnector*
                                        m_ToolConnector;                    //!< ツールコネクタクラス
    void*                               m_StackBase;                        //!< パケット受信スレッドスタックポインタ
    nn::os::ThreadType                  m_Thread;                           //!< パケット受信スレッドオブジェクト
    bool                                m_NoUseInternalThread;              //!< 内部スレッドを使用するかどうか

    void*                               m_pTransmitBuffer;                  //!< 通信用ヒープバッファ
    nn::vfx::StandardHeap               m_TransmitHeap;                     //!< 通信用ヒープ

    int                                 m_SentModelInfoCount;               //!< 送信済みモデル情報数
    void*                               m_pToolConnectorPtr;

    friend class nn::vfx::viewer::detail::PacketProcedure;
};

} // namespace viewer
} // namespace vfx
} // namespace nn


