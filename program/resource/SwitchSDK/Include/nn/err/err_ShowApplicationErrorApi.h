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
 * @brief エラー表示用APIの定義
 */

#pragma once

#include <nn/err/err_ApplicationErrorArg.h>

namespace nn { namespace err {

//! @name エラー表示用API
//! @{

/**
 * @brief アプリケーション固有のエラーを表示します。
 * @param[in] arg アプリケーション固有のエラーの情報を保持するクラス。
 * @pre
 *  - arg のダイアログ表示用のメッセージと全画面表示用のメッセージのどちらか一方、もしくは両方にメッセージが設定されている。
 *  - arg の言語が設定されている。
 * @details
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 *     - 汎用的なメッセージ表示のための関数ではありません。警告やお知らせなど、エラー以外のメッセージ表示には使用しないでください。
 * - 本関数の使用には nmeta ファイルの Application/ApplicationErrorCodeCategory を設定する必要があります。nmeta ファイルの書き方については @confluencelink{166503043, 「nmeta ファイルの書き方 」}を参照してください。
 */
void ShowApplicationError(const ApplicationErrorArg& arg) NN_NOEXCEPT;

//! @}
//-----------------------------------------------------------------------------

}} // namespace nn::err
