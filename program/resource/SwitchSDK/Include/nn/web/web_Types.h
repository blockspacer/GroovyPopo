/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief ウェブページを開くのに利用される型や定数の宣言
 */

#pragma once

#include <nn/nn_Result.h>

namespace nn { namespace web {

//--------------------------------------------------------------------------
/**
 * @brief ウェブページの背景として用いる種類を表す定数です。
 */
enum WebBackgroundKind
{
    WebBackgroundKind_Normal,                 //!< 通常背景
    WebBackgroundKind_ApplicationCapture,     //!< 呼び出し元アプリケーション画面のキャプチャー画像
    WebBackgroundKind_ApplicationCaptureBlur, //!< 呼び出し元アプリケーション画面のキャプチャー画像(ぼかし)
    WebBackgroundKind_Max,                    //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief 左スティックのモードを表す定数です。
 */
enum WebLeftStickMode
{
    WebLeftStickMode_StickPointer, //!< スティックポインター
    WebLeftStickMode_SpatialNavi,  //!< 空間ナビ
    WebLeftStickMode_Max           //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief 起動時に表示する画面の種類を表す定数です。
 */
enum WebBootDisplayKind
{
    WebBootDisplayKind_Default,           //!< 既定の動作
    WebBootDisplayKind_White,             //!< 白画面
    WebBootDisplayKind_Black,             //!< 黒画面
    WebBootDisplayKind_CallerCapture,     //!< 呼び出し元アプリケーション画面のキャプチャー画像
    WebBootDisplayKind_CallerCaptureBlur, //!< 呼び出し元アプリケーション画面のキャプチャー画像(ぼかし)
    WebBootDisplayKind_Max,               //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief フッター表示の制御方法の種類を表す定数です。
 */
enum WebFooterFixedKind
{
    WebFooterFixedKind_Auto,  //!< 自動で表示と非表示を切替
    WebFooterFixedKind_Shown, //!< 常に表示
    WebFooterFixedKind_Max,   //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief URL 表記の形式を表す定数です。
 */
enum WebDisplayUrlKind
{
    WebDisplayUrlKind_FullPath, //!< フルパス
    WebDisplayUrlKind_Domain,   //!< ドメインのみ
};

//--------------------------------------------------------------------------
/**
 * @brief ウェブページ表示の終了理由を表す定数です。
 */
enum WebExitReason
{
    WebExitReason_EndButtonPressed = 0,                //!< 「おわる」ボタンが押された。
    WebExitReason_BackButtonPressed = 1,               //!< 「戻る」ボタンが押された。
    WebExitReason_ExitMessage = 2,                     //!< システムからの終了メッセージを受け取りました。
    WebExitReason_CallbackUrlReached = 3,              //!< コールバック URL に到達した。
    WebExitReason_LastWindowDeleted = 4,               //!< 最後のウインドウが削除された。
    WebExitReason_LocalNetworkDisconnectionDenied = 5, //!< ローカル通信の切断確認が許可されなかった
    WebExitReason_MediaPlayerClosed = 6,               //!< メディアプレイヤーが閉じられた。
    WebExitReason_CausedByWebPage = 7,                 //!< ウェブページから終了要求があり終了された。
    WebExitReason_UserSelectionCanceled = 8,           //!< ユーザー選択がキャンセル操作で閉じられた。
    WebExitReason_NetworkConnectionFailed = 9,         //!< ネットワーク接続に失敗した。
    WebExitReason_SystemUpdateRequired = 10,           //!< 本体更新が必要とされた。
    WebExitReason_Unexpected = 20                      //!< 想定外の要因で終了した。
};

}} // namespace nn::web
