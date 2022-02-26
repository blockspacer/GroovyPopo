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
 * @brief   タイマーイベントに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_EventCommon.h>


//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

struct  TimerEventType;
struct  MultiWaitHolderType;

//! @name タイマーイベント関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトを初期化します。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 * @param[in] clearMode タイマーイベントの自動クリアモード
 *
 * @pre
 *  - event が未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
void InitializeTimerEvent(TimerEventType* event, EventClearMode clearMode) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトを破棄します。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *  - event を待機しているスレッドがいない
 *
 * @post
 *  - event が未初期化状態である
 *
 */
void FinalizeTimerEvent(TimerEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトのワンショットタイマー動作を開始します。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 * @param[in] first     最初のタイマー通知までの時間
 *
 * @pre
 *  - event が初期化状態である
 *  - first >= 0
 *
 * @post
 *  - event が初期化状態である
 *
 */
void StartOneShotTimerEvent(TimerEventType* event, nn::TimeSpan first) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトの周期タイマー動作を開始します。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 * @param[in] first     最初のタイマー通知までの時間
 * @param[in] interval  2 度目以降のタイマー通知の間隔
 *
 * @pre
 *  - event が初期化状態である
 *  - first >= 0
 *  - interval > 0
 *
 * @post
 *  - event が初期化状態である
 *
 */
void StartPeriodicTimerEvent(TimerEventType* event, nn::TimeSpan first, nn::TimeSpan interval) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトのタイマー動作を停止します。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
void StopTimerEvent(TimerEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトがシグナル状態になるまで待機します。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
void WaitTimerEvent(TimerEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトがシグナル状態かをポーリングします。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 *
 * @return  タイマーイベント待機の成否を示します。
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
bool TryWaitTimerEvent(TimerEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトを即座にシグナル状態にします。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
void SignalTimerEvent(TimerEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TimerEventType オブジェクトを非シグナル状態にクリアします。
 *
 * @param[in] event     TimerEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が非シグナル状態である
 *
 */
void ClearTimerEvent(TimerEventType* event) NN_NOEXCEPT;

//! @}


//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder を初期化し、タイマーイベントと関連付けます。
 *
 * @overloadlist{nn_os_InitializeMultiWaitHolder}
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 * @param[in] timerEvent        関連付ける TimerEventType オブジェクトへのポインタ
 *
 * @pre
 *  - multiWaitHolder が未初期化状態である
 *  - timerEvent が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（timerEvent に関連付けられる）
 *  - @ref nn::os::GetMultiWaitHolderUserData "nn::os::GetMultiWaitHolderUserData(multiWaitHolder)" == 0
 *
 */
void InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, TimerEventType* timerEvent) NN_NOEXCEPT;

//! @}


}} // namespace nn::os

