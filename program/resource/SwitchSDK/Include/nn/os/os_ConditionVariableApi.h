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
 * @brief   条件変数に関する API の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_ConditionVariableApi-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_ConditionVariableApi-os.horizon.h>
#endif

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  MutexType;
struct  ConditionVariableType;

//! @name 条件変数関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   ConditionVariableType オブジェクトを初期化します。
 *
 * @param[in] conditionVariable 条件変数オブジェクトへのポインタ
 *
 * @pre
 *  - conditionVariable が未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - conditionVariable が非シグナル状態である
 *
 */
void    InitializeConditionVariable(ConditionVariableType* conditionVariable)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   ConditionVariableType オブジェクトを Finalize します。
 *
 * @param[in] conditionVariable 条件変数オブジェクトへのポインタ
 *
 * @pre
 *  - conditionVariable が初期化状態である
 *  - conditionVariable を待機しているスレッドがいない
 *
 * @post
 *  - conditionVariable が未初期化状態である
 *
 */
void    FinalizeConditionVariable(ConditionVariableType* conditionVariable) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   ConditionVariableType にシグナルを通知します。
 *
 * @param[in] conditionVariable 条件変数オブジェクトへのポインタ
 *
 * @pre
 *  - conditionVariable が非シグナル状態である
 *
 * @post
 *  - conditionVariable が非シグナル状態である
 *  - conditionVariable を待機しているスレッドが１つだけ起床する
 *
 */
void    SignalConditionVariable(ConditionVariableType* conditionVariable)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   ConditionVariableType にブロードキャスト通知を行ないます。
 *
 * @param[in] conditionVariable 条件変数オブジェクトへのポインタ
 *
 * @pre
 *  - conditionVariable が非シグナル状態である
 *
 * @post
 *  - conditionVariable が非シグナル状態である
 *  - conditionVariable を待機しているスレッドが全て起床する
 *
 */
void    BroadcastConditionVariable(ConditionVariableType* conditionVariable)    NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   MutexType のアンロックと条件変数の待機をアトミックに行ないます。
 *
 * @param[in] conditionVariable 条件変数オブジェクトへのポインタ
 * @param[in] mutex             対象となる MutexType オブジェクトへのポインタ
 *
 * @pre
 *  - conditionVariable が初期化状態である
 *  - mutex が一度だけロックされた状態である
 *  - mutex のロック所有者が、自スレッドである
 *
 * @post
 *  - 事前条件と同じ
 *
 */
void    WaitConditionVariable(ConditionVariableType* conditionVariable, MutexType* mutex)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   MutexType のアンロックと条件変数の待機をアトミックに行ないます。
 *
 * @param[in] conditionVariable 条件変数オブジェクトへのポインタ
 * @param[in] mutex             対象となる MutexType オブジェクトへのポインタ
 * @param[in] timeout           最大待ち時間
 *
 * @return  条件変数待ちが解除された要因を返します
 *
 * @pre
 *  - conditionVariable オブジェクトが初期化状態である
 *  - mutex が一度だけロックされた状態である
 *  - mutex のロック所有者が、自スレッドである
 *  - timeout >= 0
 *
 * @post
 *  - 事前条件と同じ
 *
 */
ConditionVariableStatus TimedWaitConditionVariable(ConditionVariableType* conditionVariable, MutexType* mutex, TimeSpan timeout)    NN_NOEXCEPT;


#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)
//--------------------------------------------------------------------------
/**
 * @brief   nn::os::ConditionVariableType オブジェクトを静的に初期化します。
 *
 * @details
 *  nn::os::ConditionVariableType オブジェクトを静的に初期化するための
 *  初期化リストに展開されるマクロです。
 *
 *  条件変数の詳細は nn::os::InitializeConditionVariable() を参照して下さい。
 *
 *  本マクロは下記例のように、静的な nn::os::ConditionVariableType オブジェクト
 *  の初期化にのみ使用できます。こうして初期化したオブジェクトは
 *  nn::os::FinalizeConditionVariable() 呼び出しによる明示的な破棄も不要です。
 *
 *  使用例：
 *      @code
 *      nn::os::ConditionVariableType g_StaticCondVar = NN_OS_CONDITION_VARIABLE_INITIALIZER();
 *      @endcode
 *
 */
#define NN_OS_CONDITION_VARIABLE_INITIALIZER()
#endif

//! @}


}} // namespace nn::os

