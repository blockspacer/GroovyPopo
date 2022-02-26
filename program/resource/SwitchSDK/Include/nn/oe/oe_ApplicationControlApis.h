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

#include <nn/nn_Common.h>
#include <nn/settings/settings_Language.h>
#include <nn/oe/oe_ApplicationControlTypes.h>

namespace nn { namespace oe {

/**
 * @brief   本体機能の言語設定に合わせてアプリケーションが対応する最も優先度の高い言語を取得します。
 *
 * @details
 *  アプリケーションはアプリケーション管理データを編集して複数の対応言語を設定することが出来ます。
 *  また、本体設定は言語毎に選択する言語の優先度を持っています。
 *  本 API はアプリケーションが対応している言語のうち、本体設定で設定される言語と照らし合わせて最も適切な言語を取得します。
 *  言語の優先度については、@confluencelink{166503043, 「nmeta ファイルの書き方 」}を参照してください。
 *
 */
settings::LanguageCode GetDesiredLanguage() NN_NOEXCEPT;

/**
 * @brief   アプリケーションのバージョン表記を取得します。
 *
 * @details
 *  アプリケーションはアプリケーション管理データを編集してバージョン表記の情報を設定することが出来ます。
 *  本 API はアプリケーションに設定されたバージョン表記を取得します。
 *  バージョン表記については、@confluencelink{166503043, 「nmeta ファイルの書き方 」}を参照してください。
 *
 */
void GetDisplayVersion(DisplayVersion* outValue) NN_NOEXCEPT;

/**
    @brief アプリケーション起動時のロゴ表示を手動で消します。

    @details
     アプリケーション起動時に表示されるロゴ表示を手動で消します。

     デフォルトでは、アプリケーションのロゴ表示は、
     アプリケーションプログラムに制御が移ったのちに自動的に消されます。
     一方で、初期化処理に時間がかかりユーザへの表示を行うまでに時間がかかるような場合には、
     自動的にロゴ表示が消されるのを抑制し、本関数を明示的に呼ぶことで、
     ロゴ表示を消すタイミングをシステムに通知することが可能です。

     自動的にロゴ表示が消されるのを抑制するためには、
     nmeta ファイルの Application/LogoHandling に Manual を設定する必要があります。
     nmeta ファイルの書き方については @confluencelink{166503043, 「nmeta ファイルの書き方 」}を参照してください。

     なお、nmeta ファイルの Application/LogoHandling に Manual を設定しない場合に本関数を呼んでもエラーにはなりませんが、
     効果はなく、警告ログメッセージが出力されます。
*/
void FinishStartupLogo() NN_NOEXCEPT;

}} // namespace nn::oe

