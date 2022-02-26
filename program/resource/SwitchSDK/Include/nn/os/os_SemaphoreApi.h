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
 * @brief   セマフォに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

struct  SemaphoreType;
struct  MultiWaitHolderType;

//! @name セマフォ関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType オブジェクトを初期化します。
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 * @param[in] initialCount  セマフォのカウンタ初期値
 * @param[in] maxCount      セマフォのカウンタ最大値
 *
 * @pre
 *  - semaphore が未初期化状態もしくは初期化されていない状態である
 *  - maxCount >= 1
 *  - 0 <= initialCount <= maxCount
 *
 * @post
 *  - semaphore が初期化状態である
 *    - initialCount == 0 の場合は 非シグナル状態 である
 *    - initialCount >= 1 の場合は シグナル状態 である
 *
 */
void    InitializeSemaphore(SemaphoreType* semaphore, int initialCount, int maxCount)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType オブジェクトをファイナライズします。
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 *
 * @pre
 *  - semaphore が初期化状態である
 *  - semaphore を待機しているスレッドがいない
 *
 * @post
 *  - semaphore が未初期化状態である
 *
 */
void    FinalizeSemaphore(SemaphoreType* semaphore) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType を獲得します（Ｐ操作）
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 *
 * @pre
 *  - semaphore が初期化状態である
 *
 * @post
 *  - semaphore が初期化状態である
 *  - セマフォカウンタがデクリメントされる
 *
 */
void    AcquireSemaphore(SemaphoreType* semaphore)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType の獲得を試みます（Ｐ操作）
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 *
 * @return  セマフォ獲得の成否を示します。
 *
 * @pre
 *  - semaphore が初期化状態である
 *
 * @post
 *  - 返値が true の場合
 *    - semaphore が初期化状態である
 *    - セマフォカウンタがデクリメントされる
 *  - 返値が false の場合
 *    - semaphore が非シグナル状態である
 *
 */
bool    TryAcquireSemaphore(SemaphoreType* semaphore)   NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType を時限付きで獲得を試みます（Ｐ操作）
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 * @param[in] timeout       セマフォ待ちのタイムアウト時間
 *
 * @return  セマフォ獲得の成否を示します。
 *
 * @pre
 *  - semaphore が初期化状態である
 *  - timeout >= 0
 *
 * @post
 *  - 返値が true の場合
 *    - semaphore が初期化状態である
 *    - セマフォカウンタがデクリメントされる
 *  - 返値が false の場合
 *    - semaphore が非シグナル状態である
 *
 */
bool    TimedAcquireSemaphore(SemaphoreType* semaphore, TimeSpan timeout)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType を１カウントだけ返却します（Ｖ操作）
 *
 * @overloadlist{nn_os_ReleaseSemaphore}
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 *
 * @pre
 *  - semaphore が初期化状態である
 *  - semaphore のセマフォカウンタが maxCount 未満である
 *
 * @post
 *  - semaphore のセマフォカウンタがインクリメントされる
 *  - 待機スレッドがある場合、待機スレッドを１つだけ待ち解除する
 *
 */
void    ReleaseSemaphore(SemaphoreType* semaphore)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType を複数カウント返却します（Ｖ操作）
 *
 * @overloadlist{nn_os_ReleaseSemaphore}
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 * @param[in] count         返却したいセマフォカウント数
 *
 * @pre
 *  - count >= 1
 *  - semaphore が初期化状態である
 *  - semaphore のセマフォカウンタが maxCount - count 以下である
 *
 * @post
 *  - semaphore のセマフォカウンタが count 個だけ増加される
 *  - 待機スレッドがある場合、待機スレッドを全て待ち解除する
 *
 */
void    ReleaseSemaphore(SemaphoreType* semaphore, int count)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   SemaphoreType の現在のカウンタ値を返します
 *
 * @param[in] semaphore     SemaphoreType オブジェクトへのポインタ
 *
 * @return  対象セマフォの現在のカウンタ値を返します。
 *
 * @pre
 *  - semaphore が初期化状態である
 *
 * @post
 *  - semaphore が初期化状態である
 *  - semaphore の現在のカウンタ値を返す
 *
 */
int     GetCurrentSemaphoreCount(const SemaphoreType* semaphore)  NN_NOEXCEPT;

//! @}

//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder を初期化し、semaphore と関連付けます。
 *
 * @overloadlist{nn_os_InitializeMultiWaitHolder}
 *
 * @param[in]  multiWaitHolder 多重待ちオブジェクトへのポインタ
 * @param[in]  semaphore       紐付ける先のセマフォへのポインタ
 *
 * @pre
 *  - multiWaitHolder が未初期化状態である
 *  - semaphore が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（semaphore に関連付けられる）
 *  - @ref nn::os::GetMultiWaitHolderUserData "nn::os::GetMultiWaitHolderUserData(multiWaitHolder)" == 0
 *
 */
void    InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, SemaphoreType* semaphore) NN_NOEXCEPT;

//! @}


}} // namespace nn::os

