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
 * @brief   SystemEvent に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/os/os_NativeHandleTypes.h>
#include <nn/os/os_EventCommon.h>
#include <nn/os/os_SdkSystemEventApi.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

struct  SystemEventType;
struct  MultiWaitHolderType;

//! @name システムイベント関連 API
//! @{


//--------------------------------------------------------------------------
/**
 * @brief   SystemEventType オブジェクトを破棄します。
 *
 * @param[in] event     SystemEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event オブジェクトが初期化状態である
 *
 * @post
 *  - event オブジェクトが未初期化状態である
 *
 * @details
 *  SystemEventType オブジェクトを破棄します。
 *
 *  @cond SECTION_selectable_DeviceDriverKit
 *  破棄を行なう際、 CreateSystemEvent() 時の interProcess の指定により、
 *  以下のような追加の処理が行なわれます。
 *
 *  CreateSystemEvent() 時に interProcess == true で初期化されていた場合は、
 *  内部で保持しているハンドル管理フラグによって更に以下のように処理が
 *  分かれます。ハンドル管理フラグが true ならば、ReadableEvent ハンドルおよび
 *  WritableEvent ハンドルをシステムに返却します。ハンドル管理フラグが false の
 *  場合はハンドルは引き続き有効なままです。
 *
 *  CreateSystemEvent() 時に interProcess == false で初期化されていた場合は、
 *  特に追加の処理はありません。
 *  @endcond
 *
 *  本 API の動作中に対象 event オブジェクトに対する操作は行なわないで下さい。
 *
 */
void DestroySystemEvent(SystemEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   システムイベントがシグナル状態になるまで待機します。
 *
 * @param[in] event     SystemEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event オブジェクトが初期化状態である
 *
 * @details
 *  システムイベントがシグナル状態になるまで待機します。@n
 *  システムイベントが既にシグナル状態の場合は即座に返ります。
 *
 *  @cond SECTION_selectable_DeviceDriverKit
 *  clearMode による動作の違いについては、
 *  CreateSystemEvent() を参照して下さい。
 *  @endcond
 *
 */
void WaitSystemEvent(SystemEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   システムイベントがシグナル状態かをポーリングします。
 *
 * @param[in] event     SystemEventType オブジェクトへのポインタ
 *
 * @return  システムイベントオブジェクト待機の成否を示します。
 *
 * @pre
 *  - event オブジェクトが初期化状態である
 *
 * @details
 *  システムイベントがシグナル状態か否かをポーリングします。@n
 *  システムイベントが既にシグナル状態の場合は即座に true を返します。@n
 *  システムイベントが非シグナル状態の場合は待機せずに false を返します。
 *
 *  @cond SECTION_selectable_DeviceDriverKit
 *  clearMode による動作の違いについては、
 *  CreateSystemEvent() を参照して下さい。
 *  @endcond
 *
 */
bool TryWaitSystemEvent(SystemEventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   システムイベントがシグナル状態になるまで待機します。
 *
 * @param[in] event     SystemEventType オブジェクトへのポインタ
 * @param[in] timeout   最大待ち時間
 *
 * @return  システムイベントオブジェクト待機の成否を示します。
 *
 * @pre
 *  - event オブジェクトが初期化状態である
 *  - timeout >= 0
 *
 * @details
 *  システムイベントがシグナル状態になるまで待機します。@n
 *  システムイベントがシグナル状態になると true を返します。@n
 *  自スレッドが待ちに入ってから timeout 時間経過するとタイムアウトが成立し、
 *  自動的に待ちが解除されます。この場合、返値として false が返されます。
 *
 *  タイムアウトの動作に関しては @ref PageNotificationOsTimeout の
 *  注意事項も参照して下さい。
 *
 *  @cond SECTION_selectable_DeviceDriverKit
 *  clearMode による動作の違いについては、
 *  CreateSystemEvent() を参照して下さい。
 *  @endcond
 *
 */
bool TimedWaitSystemEvent(SystemEventType* event, TimeSpan timeout) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   システムイベントを非シグナル状態にクリアします。
 *
 * @param[in] event     SystemEventType オブジェクトへのポインタ
 *
 * @pre
 *  - event オブジェクトが初期化状態である
 *
 * @details
 *  システムイベントを 非シグナル状態 にクリアします。
 *
 */
void ClearSystemEvent(SystemEventType* event) NN_NOEXCEPT;


//! @}


//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder を初期化し SystemEventType オブジェクトと関連付けます。
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 * @param[in] event             関連付ける先の SystemEventType オブジェクトへのポインタ
 *
 * @pre
 *  - multiWaitHolder が未初期化状態である
 *  - event が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（event に関連付けられる）
 *  - @ref nn::os::GetMultiWaitHolderUserData "nn::os::GetMultiWaitHolderUserData(multiWaitHolder)" == 0
 *
 * @details
 *  multiWaitHolder を初期化し event オブジェクトと関連付けます。
 *
 *  関連付けられた multiWaitHolder は、対象のシステムイベントの状態に
 *  合わせてシグナル状態か否かが変化します。
 *
 *  本 API によって初期化された multiWaitHolder オブジェクトを使って多重待ちを
 *  行ない、実際に対象のシステムイベントがシグナル化されることで多重待ちが解除
 *  された場合でも、通常の WaitSystemEvent() とは異なり、システムイベントが
 *  自動的に非シグナル状態にクリアされることはありません。@n
 *  詳細は @ref PageNotificationOsMultiWait_AboutWakeup を参照して下さい。
 *
 *  本 API の動作中に対象 multiWaitHolder に対する操作は行なわないで下さい。
 *
 */
void InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, SystemEventType* event) NN_NOEXCEPT;

//! @}


}} // namespace nn::os

