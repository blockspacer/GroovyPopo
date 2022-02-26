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
 * @brief ウェブページを開くのに利用する API 宣言
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/web/web_ShowArg.h>

namespace nn { namespace web {

//------------------------------------------------------------------------
/**
 * @brief ウェブページを開きます。
 *
 * @param[out]  pOutputWebPageReturnValue   ウェブページを表示した結果の情報を受け取る @ref WebPageReturnValue のインスタンスへのポインタ
 * @param[in]   arg                         ウェブページを開くための情報を保持する @ref ShowWebPageArg のインスタンス
 *
 * @return 処理の結果を表す Result 値
 * @retval nn::ResultSuccess            ウェブページの表示に成功し、操作によって表示が終了されました。
 * @retval nn::web::ResultTerminated    ウェブページの表示中に強制的にキャンセルが行われて表示が終了されました。
 *
 * @pre
 *  - pOutputWebPageReturnValue != nullptr
 *
 * @details
 * 本関数は、arg で指定されたインスタンスが保持する
 * URL に基づくウェブページを表示します。
 *
 * 本関数は、ウェブページの表示を終了するまで呼び出し元をブロックします。
 * 返り値が nn::ResultSuccess ではない場合、pOutputWebPageReturnValue の値は不定値となりますので参照しないでください。
 *
 * ウェブページが開かれていない状態で RequestExitWebPage を呼んだ場合、nn::web::ResultTerminated を返します。
 *
 * @platformbegin{NX}
 * 本関数は「Web アプレット」を利用してウェブページの表示を行います。
 * Web アプレットおよび表示可能なウェブページの詳細については
 * 「本体機能ガイド」の「Web アプレット」を参照してください。
 * @platformend
 */
nn::Result ShowWebPage(WebPageReturnValue* pOutputWebPageReturnValue,
    const ShowWebPageArg& arg) NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
 * @brief 開かれているページに対して終了要求を送ります。
 *
 * @pre
 *  - ShowWebPage済み
 *
 * @details
 * BG フォーカス状態で本関数を呼び出した場合、インフォーカス状態への遷移時に、開かれていたページが一瞬表示されます。
 * これを回避するには BG フォーカス状態では本関数を呼び出さないようにする必要があります。
 */
void RequestExitWebPage() NN_NOEXCEPT;

}} // namespace nn::web
