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
 * @brief   ミューテックスに関する API の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Macro.h>
#include <nn/os/os_MutexCommon.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_MutexApi-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_MutexApi-os.horizon.h>
#endif

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  MutexType;

//! @name ミューテックス関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   MutexType オブジェクトを初期化します。
 *
 * @param[in] mutex     MutexType オブジェクトへのポインタ
 * @param[in] recursive ミューテックスの再帰ロックの可否指定
 * @param[in] lockLevel ロックレベル
 *
 * @pre
 *  - mutex が未初期化状態もしくは初期化されていない状態である
 *  - lockLevel == 0 || ( nn::os::MutexLockLevelMin <= lockLevel <= nn::os::MutexLockLevelMax )
 *
 * @post
 *  - mutex がアンロック状態である
 *
 */
void    InitializeMutex(MutexType* mutex, bool recursive, int lockLevel)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   MutexType オブジェクトを Finalize します。
 *
 * @param[in] mutex     MutexType オブジェクトへのポインタ
 *
 * @pre
 *  - mutex がアンロック状態である
 *
 * @post
 *  - mutex が未初期化状態である
 *
 */
void    FinalizeMutex(MutexType* mutex) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   MutexType をロックします。
 *
 * @param[in] mutex     MutexType オブジェクトへのポインタ
 *
 * @pre
 *  - mutex が以下のいずれかの状態である
 *    - recursive == false で、mutex が アンロック状態 または 他スレッドロック状態 である
 *    - recursive == true で、 mutex が初期化状態である
 *  - lockLevel > 0 の場合、以下の事前条件も加わる
 *    - lockLevel が自スレッドのカレントロックレベルより大きい値である
 *
 * @post
 *  - mutex が 自スレッドロック状態 である
 *  - カレントロックカウンタをインクリメントする
 *  - mutex が lockLevel > 0 の場合、以下の事後条件も加わる
 *    - 自スレッドのカレントロックレベルが lockLevel になる
 *
 */
void    LockMutex(MutexType* mutex) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   MutexType がロック操作を試みます。
 *
 * @param[in] mutex     MutexType オブジェクトへのポインタ
 *
 * @return  ミューテックスの獲得の成否を示します
 *
 * @pre
 *  - mutex が以下のいずれかの状態である
 *    - recursive == false で、mutex が アンロック状態 または 他スレッドロック状態 である
 *    - recursive == true で、 mutex が初期化状態である
 *  - lockLevel > 0 の場合、以下の事前条件も加わる
 *    - lockLevel が自スレッドのカレントロックレベルより大きい値である
 *
 * @post
 *  - 返値が true の場合
 *    - mutex が 自スレッドロック状態 である
 *    - カレントロックカウンタをインクリメントする
 *    - mutex が lockLevel > 0 の場合、以下の事後条件も加わる
 *      - 自スレッドのカレントロックレベルが lockLevel になる
 *  - 返値が false の場合
 *    - mutex の状態は、事前条件の状態から変化しない
 *
 */
bool    TryLockMutex(MutexType* mutex)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   MutexType をアンロックします。
 *
 * @param[in] mutex     MutexType オブジェクトへのポインタ
 *
 * @pre
 *  - mutex が以下のいずれかの状態である
 *    - recursive == false の場合、mutex が ロック状態 である
 *    - recursive == true  の場合、mutex が ロック状態 もしくは 再帰ロック状態 である
 *  - mutex のロック所有者が自スレッドである
 *  - mutex が lockLevel > 0 の場合、以下の事前条件が加わる
 *    - lockLevel が自スレッドのカレントロックレベルに等しい
 *
 * @post
 *  - ロックカウンタがデクリメントされる
 *  - ロックカウンタが 0 になった場合、mutex がアンロック状態である
 *  - mutex が lockLevel > 0 の場合、以下の事前条件が加わる
 *    - 自スレッドのカレントロックレベルが、mutex をロックする前のロックレベル値になる
 *
 */
void    UnlockMutex(MutexType* mutex)   NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   MutexType が自スレッドによってロックされているかを返します。
 *
 * @param[in] mutex     MutexType オブジェクトへのポインタ
 *
 * @return  自スレッドによってミューテックスが獲得されているか否かを返します
 *
 * @pre
 *  - mutex がロック状態もしくはアンロック状態である。
 *
 * @details
 *  mutex が自スレッドによってロックされた状態であれば true を、
 *  そうでなければ false を返します。本 API によって mutex の状態が
 *  変化することはありません。
 *
 *  本 API は主に mutex が自スレッドによって確実にロックされていることを
 *  アサートする場合などに使用します。
 *
 */
bool    IsMutexLockedByCurrentThread(const MutexType* mutex)  NN_NOEXCEPT;


#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)
//--------------------------------------------------------------------------
/**
 * @brief   nn::os::MutexType オブジェクトを静的に初期化します。
 *
 * @param[in] recursive ミューテックスの再帰ロックの可否指定
 *
 * @details
 *  nn::os::MutexType オブジェクトを静的に初期化するための初期化リストに
 *  展開されるマクロです。recursive には再帰ロック可否を bool 型で指定します。
 *  ロックレベル値は 0 となります。
 *
 *  ミューテックスの詳細は nn::os::InitializeMutex() を参照して下さい。
 *
 *  本マクロは下記例のように、静的な nn::os::MutexType オブジェクトの初期化
 *  にのみ使用できます。こうして初期化したオブジェクトは nn::os::FinalizeMutex
 *  呼び出しによる明示的な破棄も不要です。
 *
 *  使用例：
 *      @code
 *      nn::os::MutexType g_StaticMutex = NN_OS_MUTEX_INITIALIZER(false);
 *      @endcode
 *
 */
#define NN_OS_MUTEX_INITIALIZER(recursive)
#endif


//! @}

}} // namespace nn::os

