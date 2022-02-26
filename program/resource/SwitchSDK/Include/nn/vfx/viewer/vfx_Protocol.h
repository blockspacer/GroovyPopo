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

#include <nn/vfx/viewer/vfx_Guid.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------------------------
//! @brief      メッセージタイプです.
//---------------------------------------------------------------------------------------------
enum MessageType
{
    MessageType_Control = 1,                   //!< 制御用データです.
    MessageType_SendBinary,                    //!< バイナリ転送を通知するメッセージです.
    MessageType_Create,                        //!< インスタンスを生成を通知するメッセージです.
    MessageType_Modify,                        //!< リアルタイム編集を通知するメッセージです.
    MessageType_Delete,                        //!< インスタンスを削除を通知するメッセージです.
    MessageType_RemoveBinary,                  //!< バイナリ削除を通知するメッセージです.
    MessageType_ModelInfo,                     //!< モデルデータを送信するメッセージです。
    MessageType_EmitterSetRequest,             //!< エミッタセットリクエストを送信するメッセージです。
    MessageType_BinaryData,                    //!< バイナリです.
};

//---------------------------------------------------------------------------------------------
//! @brief      アセットタイプです.
//---------------------------------------------------------------------------------------------
enum AssetType
{
    AssetType_EmitterSet = 0,                  //!< エミッタセットです.
    AssetType_Emitter,                         //!< エミッタです.
    AssetType_Texture,                         //!< テクスチャです.
    AssetType_Primitive,                       //!< プリミティブです.
    AssetType_Shader,                          //!< シェーダです.
    AssetType_Preview,                         //!< プレービューです.
    AssetType_ViewerData,                     //!< ビューアデータです.
    AssetType_ViewerModel,                    //!< ビューアモデルです.
    AssetType_ViewerMessage,                  //!< ビューアメッセージです.
};

//--------------------------------------------------------------------------------------------
//! @brief      ビューアメッセージタイプです.
//--------------------------------------------------------------------------------------------
enum ViewerMessageType
{
    ViewerMessageType_Config = 0,             //!< ビューア初期設定.
    ViewerMessageType_ResetFrame,             //!< フレームを先頭に戻します.
    ViewerMessageType_Play,                   //!< 再生します.
    ViewerMessageType_Pause,                  //!< 一時停止します.
    ViewerMessageType_Stop,                   //!< 停止します.
    ViewerMessageType_StepFrame,              //!< 1フレーム進めます.
    ViewerMessageType_ForceFade,              //!< 強制フェードします.
    ViewerMessageType_SetRange,               //!< 再生範囲を設定します.
    ViewerMessageType_Camera,                 //!< カメラデータを設定します.
    ViewerMessageType_BackgroundImage,       //!< 背景画像を設定します.
    ViewerMessageType_RequestToEditor,      //!< エディタへの要求パケット.
    ViewerMessageType_Visibility,             //!< 表示/非表示の切り替え.
    ViewerMessageType_ApplicationInitialized,        //!< アプリケーションの初期化完了を通知するメッセージです.
    ViewerMessageType_DrawingOrder,             //!< 描画順を設定します.
    ViewerMessageType_Capture,                //!< Captureをします。
};

//--------------------------------------------------------------------------------------------
//! @brief      モデルメッセージタイプです.
//--------------------------------------------------------------------------------------------
enum ModelMessageType
{
    ModelMessageType_ClearPreviewModel = 0,  //!< モデル情報の削除を通知するメッセージです。
    ModelMessageType_BeginSendModelInfo,     //!< モデル情報の送信開始を通知するメッセージです.
    ModelMessageType_SendModelInfo,          //!< モデル情報の送信するメッセージです.
    ModelMessageType_EndSendModelInfo,       //!< モデル情報の送信終了を通知するメッセージです.
    ModelMessageType_RemovePreviewModel,     //!< 指定のモデル情報の削除を通知するメッセージです.
};

//--------------------------------------------------------------------------------------------
//! @brief      ビューアメッセージタイプです.
//--------------------------------------------------------------------------------------------
enum RequestEmitterSetMessageType
{
    RequestEmitterSetMessageType_FileOpen = 0,                //!< TBD
    RequestEmitterSetMessageType_Close,                        //!< TBD
    RequestEmitterSetMessageType_Create,                       //!< TBD
    RequestEmitterSetMessageType_Duplicate,                    //!< TBD
    RequestEmitterSetMessageType_LinkEmitterSet,                     //!< TBD
    RequestEmitterSetMessageType_UnlinkEmitterSet,                   //!< TBD
};

//--------------------------------------------------------------------------------------------
//! @brief      バイナリ転送メッセージです.
//--------------------------------------------------------------------------------------------
struct SendBinaryMessage
{
    NN_DISALLOW_COPY( SendBinaryMessage );
public:
    SendBinaryMessage() NN_NOEXCEPT
    {

    }
    uint32_t        assetType;      //!< アセットタイプです.
    Guid                guid;           //!< アセットのGUIDです.
};

//--------------------------------------------------------------------------------------------
//! @brief      インスタンス生成メッセージです.
//--------------------------------------------------------------------------------------------
struct CreateMessage
{
    NN_DISALLOW_COPY( CreateMessage );
public:
    CreateMessage() NN_NOEXCEPT
    {

    }
    uint32_t        assetType;      //!< アセットタイプです.
    Guid                resGuid;        //!< リソースのGUIDです.
    Guid                previewGuid;    //!< プレビューのGUIDです.
};

//--------------------------------------------------------------------------------------------
//! @brief      インスタンス削除メッセージです.
//--------------------------------------------------------------------------------------------
struct DeleteMessage
{
    NN_DISALLOW_COPY( DeleteMessage );
public:
    DeleteMessage() NN_NOEXCEPT
    {

    }
    uint32_t        assetType;      //!< アセットタイプです.
    Guid                resGuid;        //!< リソースのGUIDです.
    Guid                previewGuid;    //!< プレビューのGUIDです.
};

//--------------------------------------------------------------------------------------------
//! @brief      バイナリ削除メッセージです.
//--------------------------------------------------------------------------------------------
struct RemoveBinaryMessage
{
    NN_DISALLOW_COPY( RemoveBinaryMessage );
public:
    RemoveBinaryMessage() NN_NOEXCEPT
    {

    }
    uint32_t        assetType;      //!< アセットタイプです.
    Guid                guid;           //!< アセットのGUIDです.
};

//--------------------------------------------------------------------------------------------
//! @brief      データ変更メッセージです.
//--------------------------------------------------------------------------------------------
struct ModifyMessage
{
    NN_DISALLOW_COPY( ModifyMessage );
public:
    ModifyMessage() NN_NOEXCEPT
    {

    }
    uint32_t        assetType;      //!< アセットタイプです.
    Guid                guid;           //!< アセットのGUIDです.
    uint32_t        offset;         //!< データオフセットです.
    uint32_t        overwriteSize;   //!< 上書きするデータサイズです.
    uint16_t            isRequiredReset;//!< リセットが必要かどうか.
    uint16_t            endian;         //!< 対象スペックのCPUエンディアン.
    char                emitterName[ 64 ];//!< ※暫定
};

static const int MaxModelName = 260 * 2;        //!< TBD
static const int MaxEmitterSetName = 260 * 2;   //!< TBD

//--------------------------------------------------------------------------------------------
//! @brief      ビューアヘッダーのメッセージです。
//--------------------------------------------------------------------------------------------
struct ViewerHeaderMessage
{
    NN_DISALLOW_COPY( ViewerHeaderMessage );
public:
    ViewerHeaderMessage() NN_NOEXCEPT
    {

    }
    uint32_t type;        //!< TBD
    uint32_t size;        //!< TBD
};

//--------------------------------------------------------------------------------------------
//! @brief      モデルヘッダーのメッセージです。
//--------------------------------------------------------------------------------------------
struct ModelHeaderMessage
{
    NN_DISALLOW_COPY( ModelHeaderMessage );
public:
    ModelHeaderMessage() NN_NOEXCEPT
    {

    }
    uint32_t type;        //!< TBD
    uint32_t size;        //!< TBD
};

//--------------------------------------------------------------------------------------------
//! @brief      モデルデータのメッセージです。
//--------------------------------------------------------------------------------------------
struct ModelDataMessage
{
    NN_DISALLOW_COPY( ModelDataMessage );
public:
    ModelDataMessage() NN_NOEXCEPT
    {

    }
    Guid modelGuid;                    //!< モデルノードID
    uint32_t  isGame;                       //!< Game接続かどうか
    char modelName[ MaxModelName ];     //!< モデル名
    uint32_t  boneCount;                    //!< ボーン数
    uint32_t  boneDataSize;                 //!< ボーンデータサイズ
    char boneData;                     //!< データ
};

//--------------------------------------------------------------------------------------------
//! @brief      Binary data ヘッダーのメッセージです。
//--------------------------------------------------------------------------------------------
struct BinaryDataHeaderMessage
{
    NN_DISALLOW_COPY( BinaryDataHeaderMessage );
public:
    BinaryDataHeaderMessage() NN_NOEXCEPT
    {

    }
    uint32_t type;        //!< TBD
    uint32_t size;        //!< TBD
};

//--------------------------------------------------------------------------------------------
//! @brief      エミッタセットリクエストヘッダーのメッセージです。
//--------------------------------------------------------------------------------------------
struct EmitterSetRequestHeaderMessage
{
    NN_DISALLOW_COPY( EmitterSetRequestHeaderMessage );
public:
    EmitterSetRequestHeaderMessage() NN_NOEXCEPT
    {

    }
    uint32_t type;    //!< TBD
    uint32_t size;    //!< TBD
};

//--------------------------------------------------------------------------------------------
//! @brief      エミッタセットリクエストのメッセージです。
//--------------------------------------------------------------------------------------------
struct EmitterSetRequestMessage
{
    NN_DISALLOW_COPY( EmitterSetRequestMessage );
public:
    EmitterSetRequestMessage() NN_NOEXCEPT
    {

    }
    char emitterSetName[ MaxEmitterSetName ];       //!< エミッタセット名
    char dupEmitterSetName[ MaxEmitterSetName ];    //!< 複製エミッタセット名
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn

