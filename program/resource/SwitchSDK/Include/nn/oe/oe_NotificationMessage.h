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
 * @brief   通知メッセージに関する API の宣言
 *
 * @details
 *  このヘッダファイルは、nn/oe.h からインクルードされており、
 *  アプリケーション開発者が利用する API を定義しています。
 *
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_SystemEvent.h>

namespace nn { namespace oe {

//-----------------------------------------------------------------------------

/**
 * @brief   アプリケーションへの通知メッセージを表す型です。
 *
 * @details
 *  アプリケーションに対して通知されるメッセージを表す型です。
 *
 *  通知メッセージは nn::oe::PopNotificationMessage() もしくは、
 *  nn::oe::TryPopNotificationMessage() で取得できます。
 *
 */
typedef Bit32   Message;


/**
 * @brief   プログラムのフォーカス状態に変更があったことを示す通知メッセージです。
 *
 * @details
 *  アプリケーションのフォーカス状態に変更があったことを示します。
 *  現在のフォーカス状態は nn::oe::GetCurrentFocusState() で取得できます。
 *
 *  また、プログラムが認識しているフォーカス状態から変更がない場合でも、
 *  本メッセージが通知されてしまうことがあります。
 *  そのため、プログラムが認識しているフォーカス状態と、本メッセージ受理後に
 *  nn::oe::GetCurrentFocusState() で取得したフォーカス状態が一致する場合でも
 *  問題なくプログラムが動作するように実装して下さい。
 *
 *  なお、nn::oe::SetFocusHandlingMode() の設定によっては、
 *  本メッセージは通知されません。
 *
 */
const Message MessageFocusStateChanged = 15;


/**
 * @brief   一時中断状態から動作を再開したことを示す通知メッセージです。
 *
 * @details
 *  プログラムが何等かの理由により一時中断した状態から動作を再開したことを
 *  示します。例えば、nn::oe::FocusHandlingMode によっては、プログラムが
 *  BG フォーカス状態時に一時中断する設定の場合、
 *  その一時中断が解除されてプログラムが実行を再開した場合などに通知されます。
 *
 *  再開後のフォーカス状態は nn::oe::GetCurrentFocusState() で取得できます。
 *
 *  また、nn::oe::SetResumeNotificationEnabled() の設定によっては、
 *  本メッセージは通知されません。
 *
 */
const Message MessageResume = 16;


/**
 * @brief   アプリケーションに対して終了要求があることを示します。
 *
 * @details
 *  アプリケーションに対して終了要求があることを示します。
 *
 *  このメッセージは、HOME メニューなどからユーザ操作等によって
 *  アプリケーションを終了しようとした場合などに通知されます。
 *  アプリケーション終了前に、セーブデータの保存を行なったり、
 *  ネットワークセッションの終了手続きなどを行ないたい場合には、
 *  このメッセージをハンドリングすることで対応することができます。
 *
 *  ただし、事前に nn::oe::EnterExitRequestHandlingSection() を
 *  発行していない場合には、このメッセージが通知されることはなく、
 *  アプリケーションは即座に終了させられます。
 *
 *  また、本メッセージ受信後は速やかに終了処理を行ない、
 *  最後に nn::oe::LeaveExitRequestHandlingSection() を発行して下さい。
 *
 */
const Message MessageExitRequest = 4;


/**
 * @brief   アプリケーションのフォーカス状態の種別を表す列挙型です。
 *
 * @details
 *  アプリケーションのフォーカス状態の種別を表す列挙型です。
 *
 *  アプリケーションには以下のフォーカス状態種別があります。
 *  現在のフォーカス状態は nn::oe::GetCurrentFocusState() で取得できます。
 *
 *  | フォーカス状態種別   | 状態を示す列挙子              |
 *  | -------------------- | ----------------------------- |
 *  | インフォーカス状態   | nn::oe::FocusState_InFocus    |
 *  | アウトフォーカス状態 | nn::oe::FocusState_OutOfFocus |
 *  | BG フォーカス状態    | nn::oe::FocusState_Background |
 *
 *  また、実際に通知が行われるか否かは、
 *  nn::oe::SetFocusHandlingMode() の設定によって変わります。
 *
 *  各状態の意味は以下の通りです。
 *
 *  @b インフォーカス状態
 *
 *  この状態は、HOME メニューからのプログラム起動直後など、
 *  自プログラムが直接ユーザ操作の対象になっている状態を示します。
 *
 *  出画と描画、オーディオ再生、コントローラ入力などを処理すべき状態です。
 *
 *  @b アウトフォーカス状態
 *
 *  この状態は、インフォーカス状態からライブラリアプレットを起動した時の状態です。
 *
 *  プログラムはインフォーカス状態と同様の処理を行なうことができますが、
 *  画面にはライブラリアプレットの画面が表示され、
 *  コントローラ入力は無入力を受け取る状態になります。
 *  また、オーディオの再生は起動したライブラリアプレットによって
 *  ボリュームが絞られることがあります。
 *
 *  これらの制限はインフォーカス状態に戻ると自動的に解除されます。
 *
 *  また、nn::oe::SetFocusHandlingMode() の設定によっては、
 *  アウトフォーカス状態の時にプログラムの実行およびオーディオ再生が
 *  自動的に一時中断されます。
 *
 *  @b BG フォーカス状態
 *
 *  この状態は、HOME メニューが前面に出た時の状態です。
 *
 *  プログラムはインフォーカス状態とほぼ同様の処理を行なうことができますが、
 *  画面には HOME メニューの画面が表示され、
 *  コントローラ入力は無入力を受け取る状態になります。
 *  また、オーディオの再生はボリューム 0 相当になります。
 *
 *  これらの制限はインフォーカス状態に戻ると自動的に解除されます。
 *
 *  また、nn::oe::SetFocusHandlingMode() の設定によっては、
 *  BG フォーカス状態の時にプログラムの実行およびオーディオ再生が
 *  自動的に一時中断されます。
 *
 */
enum FocusState
{
    FocusState_InFocus    = 1,  //!< インフォーカス状態
    FocusState_OutOfFocus = 2,  //!< アウトフォーカス状態
    FocusState_Background = 3,  //!< BG フォーカス状態
};

/**
 * @brief   プログラム状態変更のハンドリングモードを示す列挙型です。
 *
 * @see     nn::oe::SetFocusHandlingMode()
 *
 */
enum FocusHandlingMode
{
    FocusHandlingMode_Suspend           = 0,    //!< BG フォーカス状態時にプログラムが中断するモードです。
    FocusHandlingMode_Notify            = 1,    //!< 状態遷移時にメッセージを通知するモードです。
    FocusHandlingMode_SuspendAndNotify  = 2,    //!< BG フォーカス状態時にプログラムが中断し、メッセージも通知するモードです。
    FocusHandlingMode_InFocusOnly       = 3,    //!< インフォーカス状態以外の時にプログラムが中断するモードです。
};


//! @name 通知メッセージの取得に関する API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   通知メッセージを取得します。
 *
 * @return  通知メッセージを示す Message 型の定数が返ります。
 *
 * @details
 *  自プログラムへの通知メッセージを取得します。
 *  本 API は通知メッセージが届くまでブロックします。
 *
 *  代表的な通知メッセージを以下に示します。
 *  各メッセージの意味は、リンク先を参照してください。
 *
 *  - nn::oe::MessageFocusStateChanged
 *  - nn::oe::MessageResume
 *  - nn::oe::MessageOperationModeChanged
 *  - nn::oe::MessagePerformanceModeChanged
 *  - nn::oe::MessageExitRequest
 *  - 上記以外の場合
 *    - 上記以外のメッセージについては、別途マニュアル等を参照して下さい。
 *    - 通常、未知のメッセージに対しては何もせずに次のメッセージを待機して下さい。
 *
 */
Message PopNotificationMessage() NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   通知メッセージを取得します（ポーリング）。
 *
 * @param[in]   pOutMessage     取得したメッセージの格納先
 *
 * @return  通知メッセージの有無を bool 型で返します。
 *
 * @details
 *  自プログラムへの通知メッセージを取得します。
 *  本 API は通知メッセージがある場合には true を返して pOutMessage に
 *  メッセージを返します。通知メッセージがない場合には false を返します。
 *
 *  その他の詳細は、 PopNotificationMessage() を参照して下さい。
 *
 */
bool TryPopNotificationMessage(Message* pOutMessage) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   通知メッセージの到着を示す SystemEventType を返します。
 *
 * @return  通知メッセージの到着を示す SystemEventType へのポインタを返します。
 *
 * @details
 *  通知メッセージの到着を示す nn::os::SystemEventType オブジェクトへの
 *  ポインタを返します。取得した SystemEventType オブジェクトは、
 *  自プログラムへの通知メッセージがあればシグナル状態、
 *  通知メッセージがなくなった時点で自動的に非シグナル状態となります。
 *
 *  取得した SystemEventType オブジェクトに対して、
 *  nn::os::ClearSystemEvent() や nn::os::DestroySystemEvent() は
 *  呼ばないで下さい。
 *
 */
os::SystemEventType* GetNotificationMessageEvent() NN_NOEXCEPT;


//! @}


//-----------------------------------------------------------------------------

//! @name   フォーカス状態遷移のハンドリング方法に関する API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   フォーカス状態遷移のハンドリング方法を指定します。
 *
 * @param[in]   mode   フォーカス状態遷移のハンドリング方法
 *
 * @details
 *  HOME メニューによる画面切替えや、ライブラリアプレットの呼出し等によって
 *  フォーカス状態が切り替わった際に、プログラムの実行をどのように
 *  ハンドリングするかを mode で指定します。
 *
 *  mode = nn::oe::FocusHandlingMode_InFocusOnly を指定すると、
 *  インフォーカス状態以外の時には自動的にプログラムが中断されます。
 *  プログラムのフォーカス状態に変更があった場合でも、
 *  nn::oe::MessageFocusStateChanged は通知されません。
 *
 *  mode = nn::oe::FocusHandlingMode_Suspend を指定すると、
 *  BG フォーカス状態時には自動的にプログラムが中断されます。
 *  プログラムのフォーカス状態に変更があった場合でも、
 *  nn::oe::MessageFocusStateChanged は通知されません。
 *
 *  mode = nn::oe::FocusHandlingMode_Notify を指定すると、
 *  プログラムのフォーカス状態に変更があった場合に、
 *  nn::oe::MessageFocusStateChanged が通知されます。
 *  BG フォーカス状態になってもプログラムは中断されません。
 *
 *  mode = nn::oe::FocusHandlingMode_SuspendAndNotify を指定すると、
 *  BG フォーカス状態時にはプログラムが中断し、フォーカス状態に変更があった
 *  場合には nn::oe::MessageFocusStateChanged が通知されます。
 *
 *  その他の nn::oe::FocusHandlingMode 含め、詳細については、
 *  @confluencelink{166065886, 「プログラムへの通知メッセージ」} も
 *  参照してください。
 *
 *  なお、プログラム実行中断中にプログラムへの終了要求が発生した場合、
 *  nn::oe::MessageExitRequest が通知される直前に自動中断が解除されます。
 *
 */
void SetFocusHandlingMode(FocusHandlingMode mode) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   現在のフォーカス状態種別を取得します。
 *
 * @return  現在のフォーカス状態種別を返します。
 *
 * @details
 *  現在のフォーカス状態種別を nn::oe::FocusState 型で取得します。
 *  本 API は nn::oe::SetFocusHandlingMode() の設定値によらず、
 *  いつでも現在のフォーカス状態を取得することができます。
 *
 */
FocusState GetCurrentFocusState() NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   動作の再開を示すメッセージ通知の有無を指定します。
 *
 * @param[in]   enabled 動作の再開を示すメッセージを通知するか否か
 *
 * @details
 *  プログラムが何等かの理由により一時中断した状態から動作を再開した時に、
 *  自プログラムに対して nn::oe::MessageResume を通知するか否かを
 *  enabled で指定します。デフォルトでは false となっています。
 *
 *  例えば、nn::oe::FocusHandlingMode_Suspend モードの場合には、
 *  プログラムが BG フォーカス状態の時に一時中断されますが、
 *  その一時中断が解除されてプログラムが実行を再開した時に通知されます。
 *
 *  プログラムが一時中断されずに単に BG フォーカス状態から復帰するだけの
 *  場合には enabled に true を指定しても nn::oe::MessageResume は
 *  通知されません。
 *
 */
void SetResumeNotificationEnabled(bool enabled) NN_NOEXCEPT;


//! @}


//-----------------------------------------------------------------------------

//! @name   終了要求をハンドリングするための API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   終了要求のハンドリング処理区間を開始します。
 *
 * @details
 *  自プログラムに対する終了要求通知をハンドリングする区間を開始します。
 *
 *  本 API 発行後、そのような事象に対して nn::oe::MessageExitRequest が
 *  通知されるようになります。
 *
 *  nn::oe::MessageExitRequest が通知された場合、
 *  プログラムは速やかに終了処理を行ない、最終的に
 *  nn::oe::LeaveExitRequestHandlingSection() を発行して下さい。
 *  それによって、自プログラムは終了されます。
 *
 *  ハンドリング処理区間中に終了要求通知が来ない状態で
 *  nn::oe::LeaveExitRequestHandlingSection() が発行された場合は、
 *  単にハンドリング処理区間を終了します。
 *
 *  ハンドリング処理区間外のときに、自プログラムに対する終了要求が
 *  発生した場合には、nn::oe::MessageExitRequest が通知されることはなく
 *  システムによってプログラムが即座に終了されます。
 *
 *  本 API は発行した回数と同じ回数だけ
 *  nn::oe::LeaveExitRequestHandlingSection() を発行することで、
 *  終了要求のハンドリング処理区間を終了できます。
 *
 *  常に終了要求をハンドリングしたい場合には、
 *  プログラムの最初の方で一度だけ本 API を発行して下さい。
 *
 */
void EnterExitRequestHandlingSection() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   終了要求のハンドリング処理区間を終了します。
 *
 * @details
 *  自プログラムに対する終了要求通知をハンドリングする区間を終了します。
 *  具体的には、nn::oe::EnterExitRequestHandlingSection() が発行された回数だけ
 *  本 API が発行された段階でハンドリング区間が終了します。
 *
 *  詳細は nn::oe::EnterExitRequestHandlingSection() を参照してください。
 *
 */
void LeaveExitRequestHandlingSection() NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::oe

