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
 * @brief   イベントに関する API の宣言
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

struct  EventType;
struct  MultiWaitHolderType;

//! @name イベント関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   EventType オブジェクトを初期化します。
 *
 * @param[in] event             EventType オブジェクトへのポインタ
 * @param[in] initiallySignaled イベントの初期状態
 * @param[in] clearMode         イベントの自動クリアモード
 *
 * @pre
 *  - event が未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - initiallySignaled == true  の場合、event はシグナル状態である。
 *  - initiallySignaled == false の場合、event は非シグナル状態である。
 *
 */
void    InitializeEvent(EventType* event, bool initiallySignaled, EventClearMode clearMode) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   EventType オブジェクトを Finalize します。
 *
 * @param[in] event     EventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *  - event を待機しているスレッドがいない
 *
 * @post
 *  - event が未初期化状態である
 *
 */
void    FinalizeEvent(EventType* event) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   EventType をシグナル状態にします。
 *
 * @param[in] event     EventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
void    SignalEvent(EventType* event)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   EventType がシグナル状態になるまで待機します。
 *
 * @param[in] event     EventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
void    WaitEvent(EventType* event) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   EventType のシグナル状態をポーリングします。
 *
 * @param[in] event     EventType オブジェクトへのポインタ
 *
 * @return  イベント待機の成否を示します。
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が初期化状態である
 *
 */
bool    TryWaitEvent(EventType* event)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   EventType がシグナル状態になるまで待機します。
 *
 * @param[in] event     EventType オブジェクトへのポインタ
 * @param[in] timeout   最大待ち時間
 *
 * @return  イベント待機の成否を示します。
 *
 * @pre
 *  - event が初期化状態である
 *  - timeout >= 0
 *
 * @post
 *  - event が初期化状態である
 *
 */
bool    TimedWaitEvent(EventType* event, TimeSpan timeout)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   EventType をクリア状態にします。
 *
 * @param[in] event     EventType オブジェクトへのポインタ
 *
 * @pre
 *  - event が初期化状態である
 *
 * @post
 *  - event が非シグナル状態である
 *
 */
void    ClearEvent(EventType* event)    NN_NOEXCEPT;

//! @}

//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder を初期化し、event と関連付けます。
 *
 * @overloadlist{nn_os_InitializeMultiWaitHolder}
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 * @param[in] event             紐付ける先のイベントへのポインタ
 *
 * @pre
 *  - multiWaitHolder が未初期化状態である
 *  - event が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（event に関連付けられる）
 *  - @ref nn::os::GetMultiWaitHolderUserData "nn::os::GetMultiWaitHolderUserData(multiWaitHolder)" == 0
 *
 */
void    InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, EventType* event) NN_NOEXCEPT;

//! @}


}} // namespace nn::os

