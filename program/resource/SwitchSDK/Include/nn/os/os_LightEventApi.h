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
 * @brief   軽量イベントに関する API の宣言
 *
 * @details
 *  軽量イベントはスレッド間でフラグの通知を行なう同期機能です。
 *
 *  軽量イベントは nn::os::EventType を扱うイベント機能に比べて
 *  多重待ちができない代わりに高速に動作する特徴を持っています。
 *
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_EventCommon.h>


//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class TimeSpan;

namespace os {

struct LightEventType;

//! @name 軽量イベント関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   LightEventType オブジェクトを初期化します。
 *
 * @param[in] event             LightEventType オブジェクトへのポインタ
 * @param[in] initiallySignaled 軽量イベントの初期状態
 * @param[in] clearMode         軽量イベントの自動クリアモード
 *
 * @pre
 *  - event が未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - initiallySignaled == true  の場合、event はシグナル状態である。
 *  - initiallySignaled == false の場合、event は非シグナル状態である。
 *
 * @details
 *  軽量イベントオブジェクトを初期化します。
 *
 *  初期化された軽量イベントオブジェクトは、シグナル状態 と 非シグナル状態 の
 *  いずれか２つの状態を持ちます。初期状態は initiallySignaled で指定します。
 *
 *  軽量イベントオブジェクトのクリア方法として clearMode に以下のいずれかを
 *  指定して下さい。
 *
 *  - nn::os::EventClearMode_AutoClear   ・・・ 自動クリア
 *  - nn::os::EventClearMode_ManualClear ・・・ 手動クリア
 *
 *  自動クリアでは、シグナル状態にセットされることによっていずれかのスレッドが
 *  起床した場合、自動的に非シグナル状態にクリアされます。
 *  このため、複数のスレッドが待機していても一回のシグナル状態へのセットでは
 *  一つのスレッドだけが起床します。
 *
 *  手動クリアではシグナル状態が自動的にクリアされることはありません。
 *  そのため、複数のスレッドが待機しているなら一回のフラグのセットで
 *  全てのスレッドが起床します。
 *
 *  本 API はスレッドセーフではありません。@n
 *  本 API の動作中に対象 event に対する操作は行なわないで下さい。
 *
 */
void InitializeLightEvent(LightEventType* event, bool initiallySignaled, EventClearMode clearMode) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   LightEventType オブジェクトをファイナライズします。
 *
 * @param[in] event     LightEventType オブジェクトへのポインタ
 *
 * @post
 *  - event が未初期化状態である
 *
 * @details
 *  軽量イベントオブジェクトをファイナライズします。
 *
 *  本 API はスレッドセーフではありません。@n
 *  本 API の動作中に対象 event に対する操作は行なわないで下さい。
 *
 */
void FinalizeLightEvent(LightEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   LightEventType をシグナル状態にセットします。
 *
 * @param[in] event     LightEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @details
 *  軽量イベントオブジェクトをシグナル状態にセットします。
 *  対象イベントを待機しているスレッドがいる場合、そのスレッドを起床させます。
 *  待機スレッドが存在しなかった場合、シグナル状態はセットされたままとなります。
 *
 *  自動クリアの場合は、複数のスレッドが待機していても
 *  起床するスレッドは一つだけです。また、待機スレッドを起床させると同時に
 *  非シグナル状態にクリアします。
 *
 *  手動クリアの場合、待機している全スレッドを起床させます。
 *  こちらの場合は、シグナル状態はセットされたままとなります。
 *
 */
void SignalLightEvent(LightEventType* event)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   LightEventType がシグナル状態になるまで待機します。
 *
 * @param[in] event     LightEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @details
 *  軽量イベントオブジェクトがシグナル状態にセットされるまで待機します。
 *  軽量イベントが既にシグナル状態の場合は本 API は即座にリターンします。
 *
 *  自動クリアの場合は、シグナル状態を検知すると同時に
 *  軽量イベントを非シグナル状態にクリアします。
 *  手動クリアの場合はシグナル状態は自動的にはクリアされません。
 *
 */
void WaitLightEvent(LightEventType* event) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   LightEventType のシグナル状態をポーリングします。
 *
 * @param[in] event     LightEventType オブジェクトへのポインタ
 *
 * @return  軽量イベントポーリングの成否を示します。
 *
 * @pre
 *  - event が初期化状態である
 *
 * @details
 *  軽量イベントオブジェクトがシグナル状態かどうかをポーリングします。
 *  軽量イベントがシグナル状態の場合には true を、
 *  そうでなければ false を返します。
 *  本 API が待ちに入ることはありません。
 *
 *  自動クリアの場合は、シグナル状態を検知すると同時に
 *  軽量イベントを非シグナル状態にクリアします。
 *  手動クリアの場合はシグナル状態は自動的にはクリアされません。
 *
 */
bool TryWaitLightEvent(LightEventType* event)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   LightEventType がシグナル状態になるまで時限付きで待機します。
 *
 * @param[in] event     LightEventType オブジェクトへのポインタ
 * @param[in] timeout   最大待ち時間
 *
 * @return  イベント待機の成否を示します。
 *
 * @pre
 *  - event が初期化状態である
 *  - timeout >= 0
 *
 * @details
 *  軽量イベントオブジェクトがシグナル状態にセットされるまで時限付きで
 *  待機します。シグナル状態を検知した場合は true を返し、シグナル状態を
 *  検知できずに timeout 時間が経過した場合は false を返します。
 *
 *  シグナル状態を検知した場合、軽量イベントの自動クリアモードが
 *  自動クリアの場合は、軽量イベントを非シグナル状態にクリアします。
 *  手動クリアの場合はシグナル状態は自動的にはクリアされません。
 *
 *  タイムアウトの動作に関しては @ref PageNotificationOsTimeout の
 *  注意事項も参照して下さい。
 *
 */
bool TimedWaitLightEvent(LightEventType* event, TimeSpan timeout) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   LightEventType をクリア状態にします。
 *
 * @param[in] event     LightEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @details
 *  軽量イベントオブジェクトを非シグナル状態にクリアします。
 *
 */
void ClearLightEvent(LightEventType* event)    NN_NOEXCEPT;

//! @}

}} // namespace nn::os

