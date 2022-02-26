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
 * @brief オフラインページを開くのに利用される型や定数の宣言
 */

#pragma once

namespace nn { namespace web {

//--------------------------------------------------------------------------
/**
 * @brief ウェブページの背景として用いる種類を表す定数です。
 */
enum OfflineBackgroundKind
{
    OfflineBackgroundKind_Normal,                  //!< 通常
    OfflineBackgroundKind_ApplicationCapture,      //!< 呼び出し元アプリケーション画面のキャプチャー画像
    OfflineBackgroundKind_ApplicationCaptureBlur,  //!< 呼び出し元アプリケーション画面のキャプチャー画像(ぼかし)
    OfflineBackgroundKind_Max,                     //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief 左スティックのモードを表す定数です。
 */
enum OfflineLeftStickMode
{
    OfflineLeftStickMode_StickPointer, //!< スティックポインター
    OfflineLeftStickMode_SpatialNavi,  //!< 空間ナビ
    OfflineLeftStickMode_Max           //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief 起動時に表示する画面の種類を表す定数です。
 */
enum OfflineBootDisplayKind
{
    OfflineBootDisplayKind_Default,           //!< 既定の動作
    OfflineBootDisplayKind_White,             //!< 白画面
    OfflineBootDisplayKind_Black,             //!< 黒画面
    OfflineBootDisplayKind_CallerCapture,     //!< 呼び出し元アプリケーション画面のキャプチャー画像
    OfflineBootDisplayKind_CallerCaptureBlur, //!< 呼び出し元アプリケーション画面のキャプチャー画像(ぼかし)
    OfflineBootDisplayKind_Max,               //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief フッター表示の制御方法の種類を表す定数です。
 */
enum OfflineFooterFixedKind
{
    OfflineFooterFixedKind_Auto,              //!< 自動で表示と非表示を切替
    OfflineFooterFixedKind_Shown,             //!< 常に表示
    OfflineFooterFixedKind_Max,               //!< (列挙子の総数です。使用しないでください。)
};

//--------------------------------------------------------------------------
/**
 * @brief ドキュメント表示の終了理由を表す定数です。
 */
enum OfflineExitReason
{
    OfflineExitReason_EndButtonPressed = 0,                //!< 「おわる」ボタンが押された。
    OfflineExitReason_BackButtonPressed = 1,               //!< 「戻る」ボタンが押された。
    OfflineExitReason_ExitMessage = 2,                     //!< システムからの終了メッセージを受け取りました。
    OfflineExitReason_CallbackUrlReached = 3,              //!< コールバック URL に到達した。
    OfflineExitReason_LastWindowDeleted = 4,               //!< 最後のウインドウが削除された。
    OfflineExitReason_LocalNetworkDisconnectionDenied = 5, //!< ローカル通信の切断確認が許可されなかった
    OfflineExitReason_MediaPlayerClosed = 6,               //!< メディアプレイヤーが閉じられた。
    OfflineExitReason_CausedByWebPage = 7,                 //!< ウェブページから終了要求があり終了された。
    OfflineExitReason_Unexpected = 20                      //!< 想定外の要因で終了した。
};

}} // namespace nn::web
