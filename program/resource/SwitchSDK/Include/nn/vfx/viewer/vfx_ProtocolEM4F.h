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

// TODO : なるべく : nw::ut::strncpy から卒業

#if defined( NN_BUILD_CONFIG_OS_COS )
#include <nw/ut/ut_String.h>
#else
#include <nn/util/util_StringUtil.h>
#endif

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {
namespace ProtocolEM4F {

//---------------------------------------------------------------------------
//! @brief      定数
//---------------------------------------------------------------------------
struct Constants
{
    static const uint32_t VERSION_EM4F = 0x03000000;                        //!< プロトコルバージョン
    static const uint32_t VERSION_MAJOR_EM4C_MASK = 0xFF000000;             //!< プロトコルのメジャーバージョンマスク
    static const uint32_t VERSION_MINOR_EM4C_MASK = 0x00FF0000;             //!< プロトコルのマイナーバージョンマスク
    static const uint32_t VERSION_REVISION_EM4C_MASK = 0x0000FF00;          //!< プロトコルのリビジョンマスク
    static const uint32_t MESSAGE_CATEGORY_EM4C_MASK = 0x0000FF00;          //!< メッセージのカテゴリーマスク
    static const uint32_t CHANNEL_ID_EFFECTMAKER_ACTIVE = 0x02;             //!< EffectMaker用チャンネルＩＤです。送信用
    static const uint32_t CHANNEL_ID_EFFECTMAKER_PASSIVE = 0x03;            //!< EffectMaker用チャンネルＩＤです。受信用
};

//---------------------------------------------------------------------------
//! @brief      編集ターゲットのオブジェクトタイプ
//---------------------------------------------------------------------------
enum EditTargetType
{
    EditTargetType_None = 0x00,                     //!< 未選択
    EditTargetType_EmitterSet = 0x01,               //!< エミッタセット・ターゲット
    EditTargetType_Emitter = 0x02,                  //!< エミッタ・ターゲット
    EditTargetType_GameSetting = 0x10,              //!< ゲーム設定（現プレビューノード）
    EditTargetType_PreviewSetting = 0x20,           //!< プレビュー設定（現ビューアノード）
    EditTargetType_PreviewModel = 0x30,             //!< モデル設定（現ビューアモデルノード）
    EditTargetType_Mask = 0xFF,                     //!< マスク

    EditTargetType_FORCE32 = 0xFFFFFFFF             //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      バイナリーのタイプ
//---------------------------------------------------------------------------
enum BinaryDataType
{
    BinaryDataType_None = 0x00,             //!< 未設定
    BinaryDataType_Particle = 0x01,             //!< EmitterSet 用
    BinaryDataType_Model = 0x02,            //!< Model 用

    BinaryDataType_FORCE32 = 0xFFFFFFFF     //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      Flag用ビット定義（=IsEnabled|IsResourceCopyEnabledのように使用します）
//---------------------------------------------------------------------------
enum FlagBitType
{
    FlagBitType_None = 0x00,                //!< 未設定
    FlagBitType_IsEnabled = 0x01,           //!< Initializer/Updater用フラグ：許可／不許可フラグ

    FlagBitType_FORCE32 = 0xFFFFFFFF        //!< TBD
};

//---------------------------------------------------------------------------
//! @brief     ビューア側で利用する文字コードのエンコーディングタイプ
//---------------------------------------------------------------------------
enum ViewEncodingType
{
    ViewEncodingType_UTF8 = 0x00,               //!< ビューアで使用する文字をUTF8とする
    ViewEncodingType_Unicode = 0x01,            //!< ビューアで使用する文字をUnicodeとする
    ViewEncodingType_ShiftJIS = 0x02,           //!< ビューアで使用する文字をShiftJISとする

    ViewEncodingType_FORCE32 = 0xFFFFFFFF       //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      文字列のタイプ
//---------------------------------------------------------------------------
enum StringType
{
    StringType_Name = 0x00,             //!< 文字列を名前として使用
    StringType_Path = 0x01,             //!< 文字列をファイルとして使用

    StringType_FORCE32 = 0xFFFFFFFF     //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      メッセージカテゴリ
//---------------------------------------------------------------------------
enum MessageCategory
{
    MessageCategory_File = 0x0000,          //!< ファイル
    MessageCategory_Object = 0x0100,        //!< オブジェクト
    MessageCategory_Emitter = 0x0200,       //!< エミッタ
    MessageCategory_Preview = 0x0300,       //!< プレビュー設定
    MessageCategory_Viewer = 0x0400,        //!< ビューア

    MessageCategory_FORCE32 = 0xFFFFFFFF   //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      メッセージタイプ
//---------------------------------------------------------------------------
enum MessageType
{
    MessageType_File_EndOfPacket = 0x00 | MessageCategory_File,                     //!< 最後のパケットであることを示すパケット
    MessageType_File_Connect = 0x01 | MessageCategory_File,                         //!< 接続時に送信するパケット
    MessageType_File_CloseAll = 0x02 | MessageCategory_File,                        //!< すべてのファイルを閉じます。
    MessageType_File_Close = 0x03 | MessageCategory_File,                           //!< 選択されたモデルを閉じます。
    MessageType_File_CloseV2E = 0x04 | MessageCategory_File,                        //!< Viewer側からファイルを閉じます。
    MessageType_File_LoadBinaryData = 0x05 | MessageCategory_File,                  //!< バイナリデータを読み込みます。
    MessageType_File_LoadFileV2E = 0x06 | MessageCategory_File,                     //!< 指定ファイルの読み込みをビューア→EffectMakerへリクエストします。
    MessageType_File_LoadFileE2V = 0x07 | MessageCategory_File,                     //!< 読み込んだファイルを Byte[] としてEffectMaker→ビューアへ送ります。
    MessageType_File_RequireEmitterSetV2E = 0x08 | MessageCategory_File,            //!< Viewer側で利用しているEmitterSetをEffectMaker側へ通知
    MessageType_File_ConnectInfoV2E = 0x09 | MessageCategory_File,                  //!< 接続時にビューア情報をEffectMakerへ通知するパケット
    MessageType_File_LinkEmitterSetV2E = 0x0A | MessageCategory_File,               //!< Viewer側からエミッタセットのリンク開始を通知するパケット
    MessageType_File_UnlinkEmitterSetV2E = 0x0B | MessageCategory_File,             //!< Viewer側からエミッタセットのリンク解除を通知するパケット
    MessageType_File_SendEmitterSet = 0x0C | MessageCategory_File,                  //!< 選択しているエミッタセットのデータを送信します。
    MessageType_Object_SelectEM = 0x00 | MessageCategory_Object,                    //!< 編集ターゲットを選択します。
    MessageType_Emitter_SendEmitter = 0x00 | MessageCategory_Emitter,               //!< 選択しているエミッタのデータを送信します。
    MessageType_Emitter_SendAnimTable = 0x01 | MessageCategory_Emitter,             //!< 編集しているエミッタのアニメバイナリを送信します。
    MessageType_Emitter_SendShaderParam = 0x02 | MessageCategory_Emitter,           //!< 編集しているエミッタのカスタムシェーダパラメータを送信します。
    MessageType_Preview_SetPreviewEmitterSet = 0x00 | MessageCategory_Preview,      //!< エミッタセットごとのプレビュー設定
    MessageType_Preview_SendFunctionInfo = 0x01 | MessageCategory_Preview,          //!< Function送信
    MessageType_Preview_AddEmitterSetInst = 0x02 | MessageCategory_Preview,         //!< エフェクトのインスタンスを生成します。
    MessageType_Preview_RemoveEmitterSetInst = 0x03 | MessageCategory_Preview,      //!< エフェクトのインスタンスを削除します。
    MessageType_Preview_RewindToFirst = 0x04 | MessageCategory_Preview,             //!< 開始フレームまでフレーム移動します。
    MessageType_Preview_Play = 0x05 | MessageCategory_Preview,                      //!< 全てのアニメーションの更新を開始/一時停止します。
    MessageType_Preview_Stop = 0x06 | MessageCategory_Preview,                      //!< 全てのアニメーションを停止し、フレームを最初に戻します。
    MessageType_Preview_StepFrame = 0x07 | MessageCategory_Preview,                 //!< 全てのアニメーションの１フレーム更新（前に進める）します。
    MessageType_Preview_ForceFade = 0x08 | MessageCategory_Preview,                 //!< 再生を強制Fadeします。
    MessageType_Preview_SetFrameRange = 0x09 | MessageCategory_Preview,             //!< 再生するフレーム範囲を指定します。
    MessageType_Preview_SetVisibility = 0x0A | MessageCategory_Preview,             //!< エミッタセット/エミッタ/プレビューノードの表示/非表示を切り替えます
    MessageType_Viewer_InitSettings = 0x00 | MessageCategory_Viewer,                //!< 初期設定です。
    MessageType_Viewer_SendBackgroundImage = 0x01 | MessageCategory_Viewer,         //!< 背景画像の設定です。
    MessageType_Viewer_SendPreviewModel = 0x02 | MessageCategory_Viewer,            //!< プレビュー設定のモデル情報を送信します
    MessageType_Viewer_ClosePreviewModel = 0x03 | MessageCategory_Viewer,           //!< モデル情報を削除します。
    MessageType_Viewer_SendModelInfoV2E = 0x04 | MessageCategory_Viewer,            //!< モデル情報を送信します（Viewer -> EM4C）
    MessageType_Viewer_ClearPreviewModelV2E = 0x05 | MessageCategory_Viewer,        //!< モデル情報を初期化します。
    MessageType_Viewer_BeginSendModelInfoV2E = 0x06 | MessageCategory_Viewer,       //!< モデル情報の転送を開始します。
    MessageType_Viewer_EndSendModelInfoV2E = 0x07 | MessageCategory_Viewer,         //!< モデル情報の転送を終了します。
    MessageType_Viewer_SetUserShaderPathV2E = 0x08 | MessageCategory_Viewer,        //!< カスタムシェーダフォルダパスを指定します。
    MessageType_Viewer_SetUserDataPathV2E = 0x09 | MessageCategory_Viewer,          //!< カスタムアクションフォルダパスを指定します。
    MessageType_Viewer_SetUserConfigPathV2E = 0x0A | MessageCategory_Viewer,        //!< ユーザー設定ファイルパスを指定します。
    MessageType_Viewer_CreateEmitterSetV2E = 0x0B | MessageCategory_Viewer,         //!< ビューアからエミッタセットを新規作成します。
    MessageType_Viewer_CloneEmitterSetV2E = 0x0C | MessageCategory_Viewer,          //!< 指定したエミッタセットの複製します。
    MessageType_Viewer_CloseConnection = 0x0D | MessageCategory_Viewer,             //!< コネクションを切断します。
    MessageType_Viewer_CameraSettings = 0x0E | MessageCategory_Viewer,              //!< カメラの設定を送信します。
    MessageType_Viewer_SendModelPlaybackParam = 0x0F | MessageCategory_Viewer,      //!< プレビュー設定のモデルの再生パラメータを送信します

    MessageType_FORCE32 = 0xFFFFFFFF                                               //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      ヘッダ部分
//---------------------------------------------------------------------------
struct Header
{
    static const int StructSize = 12;   //!< TBD

    MessageType type;                   //!< TBD
    int size;                           //!< TBD
    int arraySize;                      //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct FixedPathString
{
    static const int MaxStringLength = 260 * 2;  //!< TBD
    mutable char value[ MaxStringLength ];       //!< TBD

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetBufferSize() const NN_NOEXCEPT
    {
        return MaxStringLength;
    }

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @param[in] src     TBD
    //---------------------------------------------------------------------------
    void FromString( const char* src ) NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        nw::ut::strncpy( value, MaxStringLength, src, MaxStringLength );
#else
        util::Strlcpy( value, src, MaxStringLength );
#endif
    }

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @param[in] pOutCharBuffer     TBD
    //! @param[in] bufferSize     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetCharArray( char* pOutCharBuffer, size_t bufferSize ) const NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        return nw::ut::strncpy( pOutCharBuffer, bufferSize, value, MaxStringLength );
#else
        return util::Strlcpy( pOutCharBuffer, value, bufferSize );
#endif
    }
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct FixedString16
{
    static const int MaxStringLength = 16 * 2;//!< TBD
    mutable char value[ MaxStringLength ];    //!< TBD

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetBufferSize() const NN_NOEXCEPT
    {
        return MaxStringLength;
    }

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @param[in] src     TBD
    //---------------------------------------------------------------------------
    void FromString( const char* src ) NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        nw::ut::strncpy( value, MaxStringLength, src, MaxStringLength );
#else
        util::Strlcpy( value, src, MaxStringLength );
#endif
    }

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @param[in] pOutCharBuffer     TBD
    //! @param[in] bufferSize     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetCharArray( char* pOutCharBuffer, size_t bufferSize ) const NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        return nw::ut::strncpy( pOutCharBuffer, bufferSize, value, MaxStringLength );
#else
       return util::Strlcpy( pOutCharBuffer, value, bufferSize );
#endif
    }
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct FixedString32
{
    static const int MaxStringLength = 32 * 2;
    mutable char value[ MaxStringLength ];

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetBufferSize() const NN_NOEXCEPT
    {
        return MaxStringLength;
    }

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @param[in] src     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    void FromString( const char* src ) NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        nw::ut::strncpy( value, MaxStringLength, src, MaxStringLength );
#else
        util::Strlcpy( value, src, MaxStringLength );
#endif
    }

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //! @param[in] pOutCharBuffer     TBD
    //! @param[in] bufferSize     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetCharArray( char* pOutCharBuffer, size_t bufferSize ) const NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        return nw::ut::strncpy( pOutCharBuffer, bufferSize, value, MaxStringLength );
#else
        return util::Strlcpy( pOutCharBuffer, value, bufferSize );
#endif
    }

};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct NRgba
{
    float R;    //!< TBD
    float G;    //!< TBD
    float B;    //!< TBD
    float A;    //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct NVector2
{
    float X;    //!< TBD
    float Y;    //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct NVector3
{
    float X;    //!< TBD
    float Y;    //!< TBD
    float Z;    //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct NVector2i
{
    int X;      //!< TBD
    int Y;      //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct NVector3i
{
    int X;      //!< TBD
    int Y;      //!< TBD
    int Z;      //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct NVector4
{
    float X;    //!< TBD
    float Y;    //!< TBD
    float Z;    //!< TBD
    float W;    //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      TBD
//---------------------------------------------------------------------------
struct ByteArray
{
    int   Length;   //!< TBD
    char  Data;     //!< TBD
};

//---------------------------------------------------------------------------
//! @brief      最後のパケットであることを示すパケット
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_EndOfPacket
{
    static const MessageType MessagePacketType = MessageType_File_EndOfPacket;  //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;                                    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;                                                 //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;                                           //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      接続時に送信するパケット
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_Connect
{
    static const MessageType MessagePacketType = MessageType_File_Connect;  //!< メッセージタイプ
    static const int StructSize = 64 + 4 + 4;                               //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EffectMakerName = 1 << 0,    //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;                 //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;           //!< 編集されたプロパティ要素
    FixedString32    EffectMakerName;               //!< EffectMaker名（Viewer->EM4Fへの通信時に利用）
};

//---------------------------------------------------------------------------
//! @brief      すべてのファイルを閉じます。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_CloseAll
{
    static const MessageType MessagePacketType = MessageType_File_CloseAll;     //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;                                    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;                                                 //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;                                           //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      選択されたモデルを閉じます。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_Close
{
    static const MessageType MessagePacketType = MessageType_File_Close;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;                              //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_Type = 1 << 0,           //!< TBD
        PropertyFlag_EmitterSetName = 1 << 1, //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    BinaryDataType    Type;       //!< タイプです。
    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
};

//---------------------------------------------------------------------------
//! @brief      Viewer側からファイルを閉じます。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_CloseV2E
{
    static const MessageType MessagePacketType = MessageType_File_CloseV2E;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_Type = 1 << 0,           //!< TBD
        PropertyFlag_DocumentName = 1 << 1,   //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    BinaryDataType    Type;       //!< タイプです。
    FixedPathString    DocumentName;       //!< 対象のドキュメント名
};

//---------------------------------------------------------------------------
//! @brief      バイナリデータを読み込みます。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_LoadBinaryData
{
    static const MessageType MessagePacketType = MessageType_File_LoadBinaryData;    //!< メッセージタイプ
    static const int StructSize = 1056 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_Type = 1 << 0,               //!< TBD
        PropertyFlag_Id = 1 << 1,                 //!< TBD
        PropertyFlag_FileSize = 1 << 2,           //!< TBD
        PropertyFlag_FilePath = 1 << 3,           //!< TBD
        PropertyFlag_EmitterSetName = 1 << 4,     //!< TBD
        PropertyFlag_EmitterSetID = 1 << 5,       //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    BinaryDataType    Type;       //!< タイプです。
    uint32_t    Id;       //!< IDです。
    uint32_t    FileSize;       //!< ファイルサイズです。
    FixedPathString    FilePath;       //!< ファイルパスです。
    FixedPathString    EmitterSetName;       //!< エミッタセット名
    int    EmitterSetID;       //!< エミッタセット名を識別するＩＤ
};

//---------------------------------------------------------------------------
//! @brief      指定ファイルの読み込みをビューア→EffectMakerへリクエストします。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_LoadFileV2E
{
    static const MessageType MessagePacketType = MessageType_File_LoadFileV2E;    //!< メッセージタイプ
    static const int StructSize = 520 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_FilePath = 1 << 0,       //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    FilePath;       //!< ファイルパスです。
};

//---------------------------------------------------------------------------
//! @brief      読み込んだファイルを Byte[] としてEffectMaker→ビューアへ送ります。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_LoadFileE2V
{
    static const MessageType MessagePacketType = MessageType_File_LoadFileE2V;    //!< メッセージタイプ
    static const int StructSize = 528 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_FilePath = 1 << 0,       //!< TBD
        PropertyFlag_Data = 1 << 1,           //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    FilePath;       //!< 読み込んだファイルパスです。
    ByteArray    Data;       //!< データ（&Data からLength分データが続く）
};

//---------------------------------------------------------------------------
//! @brief      Viewer側で利用しているEmitterSetをEffectMaker側へ通知
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_RequireEmitterSetV2E
{
    static const MessageType MessagePacketType = MessageType_File_RequireEmitterSetV2E;    //!< メッセージタイプ
    static const int StructSize = 528 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0, //!< TBD
        PropertyFlag_StrType = 1 << 1,        //!< TBD
        PropertyFlag_BinType = 1 << 2,        //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< EffectMaker名／ファイルのフルパス
    StringType    StrType;       //!< EmitterSetNameの文字列タイプ（Name/Path）
    BinaryDataType    BinType;       //!< 要求するタイプ
};

//---------------------------------------------------------------------------
//! @brief      接続時にビューア情報をEffectMakerへ通知するパケット
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_ConnectInfoV2E
{
    static const MessageType MessagePacketType = MessageType_File_ConnectInfoV2E;    //!< メッセージタイプ
    static const int StructSize = 4 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EncodingType = 1 << 0,   //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    ViewEncodingType    EncodingType;       //!< 文字コードのエンコーディングタイプ
};

//---------------------------------------------------------------------------
//! @brief      Viewer側からエミッタセットのリンク開始を通知するパケット
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_LinkEmitterSetV2E
{
    static const MessageType MessagePacketType = MessageType_File_LinkEmitterSetV2E;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0, //!< TBD
        PropertyFlag_EmitterSetID = 1 << 1,   //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< エミッタセット名
    int    EmitterSetID;       //!< エミッタセット名を識別するＩＤ
};

//---------------------------------------------------------------------------
//! @brief      Viewer側からエミッタセットのリンク解除を通知するパケット
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_UnlinkEmitterSetV2E
{
    static const MessageType MessagePacketType = MessageType_File_UnlinkEmitterSetV2E;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0, //!< TBD
        PropertyFlag_EmitterSetID = 1 << 1,   //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< エミッタセット名
    int    EmitterSetID;       //!< エミッタセット名を識別するＩＤ
};

//---------------------------------------------------------------------------
//! @brief      選択しているエミッタセットのデータを送信します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_File_SendEmitterSet
{
    static const MessageType MessagePacketType = MessageType_File_SendEmitterSet;    //!< メッセージタイプ
    static const int StructSize = 540 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_Id = 1 << 0,             //!< TBD
        PropertyFlag_EmitterSetID = 1 << 1,   //!< TBD
        PropertyFlag_EmitterSetName = 1 << 2, //!< TBD
        PropertyFlag_DataSize = 1 << 3,       //!< TBD
        PropertyFlag_Data = 1 << 4,           //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    uint32_t    Id;       //!< IDです。
    int    EmitterSetID;       //!< エミッタセット名を識別するＩＤ
    FixedPathString    EmitterSetName;       //!< エミッタセット名
    uint32_t    DataSize;       //!< バイナリデータのサイズです。
    ByteArray    Data;       //!< エミッタセットのバイナリデータです。
};

//---------------------------------------------------------------------------
//! @brief      編集ターゲットを選択します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Object_SelectEM
{
    static const MessageType MessagePacketType = MessageType_Object_SelectEM;    //!< メッセージタイプ
    static const int StructSize = 532 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_ObjectType = 1 << 0,     //!< TBD
        PropertyFlag_TargetType = 1 << 1,     //!< TBD
        PropertyFlag_Id = 1 << 2,             //!< TBD
        PropertyFlag_TargetName = 1 << 3,     //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    BinaryDataType    ObjectType;       //!< オブジェクトタイプです。
    EditTargetType    TargetType;       //!< 編集ターゲットタイプです。
    uint32_t    Id;       //!< バイナリーのハッシュです。
    FixedPathString    TargetName;       //!< 編集ターゲット名です。
};

//---------------------------------------------------------------------------
//! @brief      選択しているエミッタのデータを送信します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Emitter_SendEmitter
{
    static const MessageType MessagePacketType = MessageType_Emitter_SendEmitter;    //!< メッセージタイプ
    static const int StructSize = 1052 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0,     //!< TBD
        PropertyFlag_EmitterName = 1 << 1,        //!< TBD
        PropertyFlag_EmitterIndex = 1 << 2,       //!< TBD
        PropertyFlag_Data = 1 << 3,               //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
    FixedPathString    EmitterName;       //!< 対象のエミッタ名
    uint32_t    EmitterIndex;       //!< 対象のエミッタインデックス
    ByteArray    Data;       //!< データ（&Data からLength分データが続く）
};

//---------------------------------------------------------------------------
//! @brief      編集しているエミッタのアニメバイナリを送信します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Emitter_SendAnimTable
{
    static const MessageType MessagePacketType = MessageType_Emitter_SendAnimTable;    //!< メッセージタイプ
    static const int StructSize = 1056 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0,     //!< TBD
        PropertyFlag_EmitterIndex = 1 << 1,       //!< TBD
        PropertyFlag_EmitterName = 1 << 2,        //!< TBD
        PropertyFlag_DataSize = 1 << 3,           //!< TBD
        PropertyFlag_Data = 1 << 4,               //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
    uint32_t    EmitterIndex;       //!< 対象のエミッタインデックス
    FixedPathString    EmitterName;       //!< 対象のエミッタ名
    uint32_t    DataSize;       //!< アニメバイナリのサイズです。
    ByteArray    Data;       //!< アニメバイナリデータ
};

//---------------------------------------------------------------------------
//! @brief      編集しているエミッタのカスタムシェーダパラメータを送信します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Emitter_SendShaderParam
{
    static const MessageType MessagePacketType = MessageType_Emitter_SendShaderParam;    //!< メッセージタイプ
    static const int StructSize = 1048 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0,     //!< TBD
        PropertyFlag_EmitterIndex = 1 << 1,       //!< TBD
        PropertyFlag_EmitterName = 1 << 2,        //!< TBD
        PropertyFlag_DataLength = 1 << 3,         //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
    uint32_t    EmitterIndex;       //!< 対象のエミッタインデックス
    FixedPathString    EmitterName;       //!< 対象のエミッタ名
    int    DataLength;       //!< バイナリ配列の長さです。
};

//---------------------------------------------------------------------------
//! @brief      エミッタセットごとのプレビュー設定
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_SetPreviewEmitterSet
{
    static const MessageType MessagePacketType = MessageType_Preview_SetPreviewEmitterSet;    //!< メッセージタイプ
    static const int StructSize = 1564 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0,     //!< TBD
        PropertyFlag_ModelName = 1 << 1,          //!< TBD
        PropertyFlag_BoneName = 1 << 2,           //!< TBD
        PropertyFlag_BoneCount = 1 << 3,            //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
    FixedPathString    ModelName;       //!< コンストレイン先のモデル名
    FixedPathString    BoneName;       //!< コンストレイン先のボーン名
    int    BoneCount;       //!< コンストレイン先のボーン番号
};

//---------------------------------------------------------------------------
//! @brief      Function送信
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_SendFunctionInfo
{
    static const MessageType MessagePacketType = MessageType_Preview_SendFunctionInfo;    //!< メッセージタイプ
    static const int StructSize = 536 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_TargetType = 1 << 0,     //!< TBD
        PropertyFlag_TargetName = 1 << 1,     //!< TBD
        PropertyFlag_TargetID = 1 << 2,       //!< TBD
        PropertyFlag_Data = 1 << 3,           //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    EditTargetType    TargetType;       //!< 対象のタイプ
    FixedPathString    TargetName;       //!< 対象の名前
    uint32_t    TargetID;       //!< 対象のＩＤ
    ByteArray    Data;       //!< データ（&Data からLength分データが続く）
};

//---------------------------------------------------------------------------
//! @brief      エフェクトのインスタンスを生成します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_AddEmitterSetInst
{
    static const MessageType MessagePacketType = MessageType_Preview_AddEmitterSetInst;    //!< メッセージタイプ
    static const int StructSize = 536 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0,         //!< TBD
        PropertyFlag_PreviewId = 1 << 1,              //!< TBD
        PropertyFlag_IsVisible = 1 << 2,              //!< TBD
        PropertyFlag_PlaybackStartFrame = 1 << 3,     //!< TBD
        PropertyFlag_ForceStartFrame = 1 << 4,        //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF           //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
    uint32_t    PreviewId;       //!< プレビューのIDです。
    uint32_t    IsVisible;       //!< 表示/非表示のフラグです。
    uint32_t    PlaybackStartFrame;       //!< 再生開始フレーム
    uint32_t    ForceStartFrame;       //!< 強制開始 フレーム
};

//---------------------------------------------------------------------------
//! @brief      エフェクトのインスタンスを削除します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_RemoveEmitterSetInst
{
    static const MessageType MessagePacketType = MessageType_Preview_RemoveEmitterSetInst;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0, //!< TBD
        PropertyFlag_PreviewId = 1 << 1,      //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 対象のエミッタセット名
    uint32_t    PreviewId;       //!< プレビューのIDです。
};

//---------------------------------------------------------------------------
//! @brief      開始フレームまでフレーム移動します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_RewindToFirst
{
    static const MessageType MessagePacketType = MessageType_Preview_RewindToFirst;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      全てのアニメーションの更新を開始/一時停止します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_Play
{
    static const MessageType MessagePacketType = MessageType_Preview_Play;    //!< メッセージタイプ
    static const int StructSize = 4 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_ForcePlay = 1 << 0,          //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    uint32_t    ForcePlay;       //!< 強制的に再生させるフラグです。
};

//---------------------------------------------------------------------------
//! @brief      全てのアニメーションを停止し、フレームを最初に戻します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_Stop
{
    static const MessageType MessagePacketType = MessageType_Preview_Stop;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      全てのアニメーションの１フレーム更新（前に進める）します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_StepFrame
{
    static const MessageType MessagePacketType = MessageType_Preview_StepFrame;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      再生を強制Fadeします。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_ForceFade
{
    static const MessageType MessagePacketType = MessageType_Preview_ForceFade;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      再生するフレーム範囲を指定します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_SetFrameRange
{
    static const MessageType MessagePacketType = MessageType_Preview_SetFrameRange;    //!< メッセージタイプ
    static const int StructSize = 12 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_StartFrame = 1 << 0,         //!< TBD
        PropertyFlag_EndFrame = 1 << 1,           //!< TBD
        PropertyFlag_DoLoop = 1 << 2,             //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    int    StartFrame;       //!< 開始フレームです。
    int    EndFrame;       //!< 終了フレームです。
    int    DoLoop;       //!< フラグ設定（=1..ループ /=0..ループしない）
};

//---------------------------------------------------------------------------
//! @brief      エミッタセット/エミッタ/プレビューノードの表示/非表示を切り替えます
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Preview_SetVisibility
{
    static const MessageType MessagePacketType = MessageType_Preview_SetVisibility;    //!< メッセージタイプ
    static const int StructSize = 532 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_TargetType = 1 << 0,         //!< TBD
        PropertyFlag_TargetName = 1 << 1,         //!< TBD
        PropertyFlag_TargetID = 1 << 2,           //!< TBD
        PropertyFlag_Visibility = 1 << 3,         //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    EditTargetType    TargetType;       //!< 対象のエミッタセット名
    FixedPathString    TargetName;       //!< 対象のエミッタ名
    uint32_t    TargetID;       //!< 対象のエミッタインデックス
    uint32_t    Visibility;       //!< Visibilityです。
};

//---------------------------------------------------------------------------
//! @brief      初期設定です。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_InitSettings
{
    static const MessageType MessagePacketType = MessageType_Viewer_InitSettings;    //!< メッセージタイプ
    static const int StructSize = 40 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_FrameRate = 1 << 0,          //!< TBD
        PropertyFlag_Resolution = 1 << 1,         //!< TBD
        PropertyFlag_ClearColor = 1 << 2,         //!< TBD
        PropertyFlag_WorldScaleRange = 1 << 3,    //!< TBD
        PropertyFlag_WorldScaleTime = 1 << 4,     //!< TBD
        PropertyFlag_GridScale = 1 << 5,          //!< TBD
        PropertyFlag_LinearEditMode = 1 << 6,     //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    int    FrameRate;       //!< フレームレートです。
    int    Resolution;       //!< 解像度です。
    NRgba    ClearColor;       //!< クリアカラーです。
    float    WorldScaleRange;       //!< ワールドスケール 位置・速度・サイズレンジ
    int    WorldScaleTime;       //!< ワールドスケール 時間
    float    GridScale;       //!< ワールドスケール グリッドと床の表示スケール
    int    LinearEditMode;       //!< リニア編集モードの設定状態です(0: Disabled, 1: Enabled)。
};

//---------------------------------------------------------------------------
//! @brief      背景画像の設定です。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SendBackgroundImage
{
    static const MessageType MessagePacketType = MessageType_Viewer_SendBackgroundImage;    //!< メッセージタイプ
    static const int StructSize = 12 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_Width = 1 << 0,              //!< TBD
        PropertyFlag_Height = 1 << 1,             //!< TBD
        PropertyFlag_DataLength = 1 << 2,         //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    int    Width;       //!< 画像の幅です。
    int    Height;       //!< 画像の高さです。
    int    DataLength;       //!< 画像のRGBA配列の長さです。
};

//---------------------------------------------------------------------------
//! @brief      プレビュー設定のモデル情報を送信します
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SendPreviewModel
{
    static const MessageType MessagePacketType = MessageType_Viewer_SendPreviewModel;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_ModelID = 1 << 0,            //!< TBD
        PropertyFlag_ModelPath = 1 << 1,          //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    int    ModelID;       //!< モデルノードＩＤ
    FixedPathString    ModelPath;       //!< モデルファイルのパス
};

//---------------------------------------------------------------------------
//! @brief      モデル情報を削除します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_ClosePreviewModel
{
    static const MessageType MessagePacketType = MessageType_Viewer_ClosePreviewModel;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_ModelID = 1 << 0,        //!< TBD
        PropertyFlag_ModelName = 1 << 1,      //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    int    ModelID;       //!< モデルノードＩＤ
    FixedPathString    ModelName;       //!< モデル名（SendModelInfoV2E で取得したモデル名）
};

//---------------------------------------------------------------------------
//! @brief      モデル情報を送信します（Viewer -> EM4C）
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SendModelInfoV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_SendModelInfoV2E;    //!< メッセージタイプ
    static const int StructSize = 536 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_ModelID = 1 << 0,        //!< TBD
        PropertyFlag_ModelName = 1 << 1,      //!< TBD
        PropertyFlag_BoneCount = 1 << 2,      //!< TBD
        PropertyFlag_Data = 1 << 3,           //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    int        ModelID;        //!< モデルノードＩＤ
    FixedPathString     ModelName;      //!< モデル名
    int        BoneCount;      //!< ボーン数
    ByteArray   Data;           //!< データ（&Data からLength分データが続く）
};

//---------------------------------------------------------------------------
//! @brief      モデル情報を初期化します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_ClearPreviewModelV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_ClearPreviewModelV2E;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      モデル情報の転送を開始します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_BeginSendModelInfoV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_BeginSendModelInfoV2E;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      モデル情報の転送を終了します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_EndSendModelInfoV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_EndSendModelInfoV2E;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      カスタムシェーダフォルダパスを指定します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SetUserShaderPathV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_SetUserShaderPathV2E;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_UserShaderPath = 1 << 0,     //!< TBD
        PropertyFlag_ForceAssign = 1 << 1,        //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    UserShaderPath;       //!< ユーザシェーダパス
    int    ForceAssign;       //!< 強制的にカスタムシェーダフォルダパスを指定します。
};

//---------------------------------------------------------------------------
//! @brief      カスタムアクションフォルダパスを指定します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SetUserDataPathV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_SetUserDataPathV2E;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_UserDataPath = 1 << 0,       //!< TBD
        PropertyFlag_ForceAssign = 1 << 1,        //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    UserDataPath;       //!< ユーザデータパス
    int    ForceAssign;       //!< 強制的にカスタムアクションフォルダパスを指定します。
};

//---------------------------------------------------------------------------
//! @brief      ユーザー設定ファイルパスを指定します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SetUserConfigPathV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_SetUserConfigPathV2E;    //!< メッセージタイプ
    static const int StructSize = 524 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_FilePath = 1 << 0,           //!< TBD
        PropertyFlag_ForceAssign = 1 << 1,        //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    FilePath;       //!< ユーザ設定ファイルパス
    int    ForceAssign;       //!< 強制的にユーザー設定ファイルパスを指定します。
};

//---------------------------------------------------------------------------
//! @brief      ビューアからエミッタセットを新規作成します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_CreateEmitterSetV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_CreateEmitterSetV2E;    //!< メッセージタイプ
    static const int StructSize = 520 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_EmitterSetName = 1 << 0,     //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    EmitterSetName;       //!< 作成したエミッタセット名
};

//---------------------------------------------------------------------------
//! @brief      指定したエミッタセットの複製します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_CloneEmitterSetV2E
{
    static const MessageType MessagePacketType = MessageType_Viewer_CloneEmitterSetV2E;    //!< メッセージタイプ
    static const int StructSize = 1040 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_SrcEmitterSetName = 1 << 0,  //!< TBD
        PropertyFlag_DstEmitterSetName = 1 << 1,  //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF       //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    FixedPathString    SrcEmitterSetName;       //!< 複製元のエミッタセット名
    FixedPathString    DstEmitterSetName;       //!< 複製先のエミッタセット名
};

//---------------------------------------------------------------------------
//! @brief      コネクションを切断します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_CloseConnection
{
    static const MessageType MessagePacketType = MessageType_Viewer_CloseConnection;    //!< メッセージタイプ
    static const int StructSize = 0 + 4 + 4;    //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

};

//---------------------------------------------------------------------------
//! @brief      カメラの設定を送信します。
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_CameraSettings
{
    static const MessageType MessagePacketType = MessageType_Viewer_CameraSettings;     //!< メッセージタイプ
    static const int StructSize = 48 + 4 + 4;                                           //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_CameraPos = 1 << 0,      //!< TBD
        PropertyFlag_LookAtPos = 1 << 1,      //!< TBD
        PropertyFlag_CameraRotation = 1 << 2, //!< TBD
        PropertyFlag_CameraProjType = 1 << 3, //!< TBD
        PropertyFlag_AspectRatio = 1 << 4,    //!< TBD
        PropertyFlag_FOV = 1 << 5,            //!< TBD
        PropertyFlag_FrustumNear = 1 << 6,    //!< TBD
        PropertyFlag_FrustumFar = 1 << 7,     //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF   //!< TBD
    };

    uint32_t   ModifiedProperties;              //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;        //!< 編集されたプロパティ要素

    NVector3    CameraPos;          //!< カメラの位置。
    NVector3    LookAtPos;          //!< カメラの注視点。
    float    CameraRotation;       //!< カメラの回転。
    int    CameraProjType;         //!< カメラタイプ。
    float    AspectRatio;          //!< アスペクト比。
    float    FOV;                  //!< 画角(パースペクティブ) / 画像の縦サイズ(オルソ)
    float    FrustumNear;          //!< ニア
    float    FrustumFar;           //!< ファー
};

//---------------------------------------------------------------------------
//! @brief      プレビュー設定のモデルの再生パラメータを送信します
//---------------------------------------------------------------------------
struct MESSAGEPACKET_Viewer_SendModelPlaybackParam
{
    static const MessageType MessagePacketType = MessageType_Viewer_SendModelPlaybackParam;     //!< メッセージタイプ
    static const int StructSize = 20 + 4 + 4;                                                   //!< 構造体にした場合のサイズ。  + 4 は ModifiedProperties と ModifiedPropertyElements

    //---------------------------------------------------------------------------
    //! @brief      TBD
    //---------------------------------------------------------------------------
    enum PropertyFlag
    {
        PropertyFlag_ModelID = 1 << 0,                //!< TBD
        PropertyFlag_AnimStartFrame = 1 << 1,         //!< TBD
        PropertyFlag_AnimEndFrame = 1 << 2,           //!< TBD
        PropertyFlag_PlaybackSpeed = 1 << 3,          //!< TBD
        PropertyFlag_PlaybackStartFrame = 1 << 4,     //!< TBD
        PropertyFlag_FORCE32 = 0xFFFFFFFF           //!< TBD
    };
    uint32_t   ModifiedProperties;                     //!< 編集されたプロパティ
    uint32_t   ModifiedPropertyElements;               //!< 編集されたプロパティ要素
    int    ModelID;                                //!< モデルノードＩＤ
    int    AnimStartFrame;                         //!< 再生範囲の開始フレーム
    int    AnimEndFrame;                           //!< 再生範囲の終了フレーム
    float    PlaybackSpeed;                        //!< 再生速度
    int    PlaybackStartFrame;                     //!< 再生開始フレーム
};

//---------------------------------------------------------------------------
//! @brief      編集対象となるオブジェクトの指定
//---------------------------------------------------------------------------
struct EditSelectedObject
{
    static const int StructSize = 8;    //!< 構造体にした場合のサイズ
    uint32_t    Id;                        //!< IDです。
    uint32_t    Index;                     //!< Indexです。
};

//---------------------------------------------------------------------------
//! @brief      メッセージのバイナリ配列要素
//---------------------------------------------------------------------------
struct ByteData
{
    static const int StructSize = 1;    //!< 構造体にした場合のサイズ
    uint8_t    Data;                      //!< Byteの配列要素です。
};

} // namespace ProtocolEM4F
} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn


