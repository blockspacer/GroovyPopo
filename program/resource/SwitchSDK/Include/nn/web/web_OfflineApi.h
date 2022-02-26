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
 * @brief オフライン HTML を開くのに利用する API 宣言
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/web/web_OfflineShowArg.h>

namespace nn { namespace web {

//------------------------------------------------------------------------
/**
 * @brief アプリケーションに付加されたオフライン HTML (ドキュメント)を開きます。
 *
 * @param[out]  pOutputOfflineHtmlPageReturnValue  オフライン HTML を開いた結果の情報を受け取る @ref OfflineHtmlPageReturnValue のインスタンスへのポインタ
 * @param[in]   arg                                オフライン HTML を開くための情報を保持する @ref ShowOfflineHtmlPageArg のインスタンス
 *
 * @return 処理の結果を表す Result 値
 * @retval nn::ResultSuccess            オフライン HTML の表示に成功し、操作によって表示が終了されました。
 * @retval nn::web::ResultTerminated    オフライン HTML の表示中に強制的にキャンセルが行われて表示が終了されました。
 *
 * @pre
 *  - pOutputOfflineHtmlPageReturnValue != nullptr
 *
 * @details
 * 本関数は、arg で指定されたインスタンスが保持する
 * パス上に存在するオフライン HTML を表示します。
 *
 * 本関数は、オフライン HTML の表示を終了するまで呼び出し元をブロックします。
 * 返り値が nn::ResultSuccess ではない場合、pOutputOfflineHtmlPageReturnValue の値は不定値となりますので参照しないでください。
 *
 * オフライン HTML (ドキュメント)が開かれていない状態で RequestExitOfflineHtmlPage を呼んだ場合、nn::web::ResultTerminated を返します。
 *
 * @platformbegin{NX}
 * 本関数は「オフライン Web アプレット」を利用してオフライン HTML の表示を行います。
 * オフライン Web アプレットおよびオフライン HTML の詳細については
 * 「本体機能ガイド」の「オフライン Web アプレット」を参照してください。
 * @platformend
 */
nn::Result ShowOfflineHtmlPage(OfflineHtmlPageReturnValue* pOutputOfflineHtmlPageReturnValue,
    const ShowOfflineHtmlPageArg& arg) NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
 * @brief 開かれているページに対して終了要求を送ります。
 *
 * @pre
 *  - ShowOfflineHtmlPage 済み
 *
 * @details
 * BG フォーカス状態で本関数を呼び出した場合、インフォーカス状態への遷移時に、開かれていたページが一瞬表示されます。
 * これを回避するには BG フォーカス状態では本関数を呼び出さないようにする必要があります。
 */
void RequestExitOfflineHtmlPage() NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
 * @brief (廃止予定) アプリケーションに付加されたオフライン HTML (ドキュメント)を開きます。
 * @deprecated 本関数は廃止予定です。代わりに @ref ShowOfflineHtmlPage を使用してください。
 *
 * @param[out]  pOutputOfflineHtmlPageReturnValue  オフライン HTML を開いた結果の情報を受け取る @ref OfflineHtmlPageReturnValue のインスタンスへのポインタ
 * @param[in]   arg                                オフライン HTML を開くための情報を保持する @ref ShowOfflineHtmlPageArg のインスタンス
 *
 * @return 処理の結果を表す Result 値
 * @retval nn::ResultSuccess            オフライン HTML の表示に成功し、操作によって表示が終了されました。
 * @retval nn::web::ResultTerminated    オフライン HTML の表示中に強制的にキャンセルが行われて表示が終了されました。
 *
 * @pre
 *  - pOutputOfflineHtmlPageReturnValue != nullptr
 */
NN_DEPRECATED inline nn::Result ShowApplicationHtmlDocumentPage(OfflineHtmlPageReturnValue* pOutputOfflineHtmlPageReturnValue,
    const ShowOfflineHtmlPageArg& arg) NN_NOEXCEPT
{
    return ShowOfflineHtmlPage(pOutputOfflineHtmlPageReturnValue, arg);
}

}} // namespace nn::web
