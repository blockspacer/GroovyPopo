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
 * @brief   ファイバに関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_Result.h>
#include <nn/os/os_FiberCommon.h>

namespace nn { namespace os {

struct  FiberType;

//! @name ファイバ関連 API
//! @{

//-----------------------------------------------------------------------------
/**
 * @brief   ファイバオブジェクトを初期化します。
 *
 * @param[in] fiber         FiberType オブジェクトへのポインタ
 * @param[in] function      ファイバの関数エントリ
 * @param[in] argument      ファイバに渡される引数
 * @param[in] stack         ファイバのスタック領域の先頭アドレス
 * @param[in] stackSize     ファイバのスタックサイズ
 * @param[in] fiberFlag     ファイバに付随するフラグ
 *
 * @pre
 *  - fiber が NotInitialized 状態もしくは初期化されていない状態である
 *  - fiberFlag に nn::os::FiberFlag_NoStackGuard が含まれる場合
 *    - stack が nn::os::FiberStackAlignment でアライメントされている
 *    - stackSize % nn::os::FiberStackAlignment == 0
 *    - stackSize > 0
 *  - fiberFlag に nn::os::FiberFlag_NoStackGuard が含まれない場合
 *    - stack が nn::os::GuardedStackAlignment でアライメントされている
 *    - stackSize % nn::os::GuardedStackAlignment == 0
 *    - stackSize > 0
 *
 * @post
 *  - fiber が Ready 状態である
 *
 * @details
 *  fiber で指定されるファイバオブジェクトを初期化します。
 *
 *  ファイバは、利用者が手動でスケジューリングを行なう必要がある実行単位です。
 *  ファイバは既存のスレッドコンテキスト上で動作し、
 *  SwitchToFiber() を明示的に呼び出すことで切り替えることができます。
 *  ファイバが他のファイバによって自動的にプリエンプトされることはありません。
 *
 *  function はファイバ関数のエントリで、argument はファイバ関数に渡す引数、
 *  stack と stackSize でファイバが使用するスタック領域を指定します。
 *
 *  fiberFlag には nn::os::FiberFlag で定義されている列挙子のパラメータを
 *  論理和したものを指定します。
 *
 *  fiberFlag に nn::os::FiberFlag_NoStackGuard が含まれない場合は
 *  スタックガードを用意します。つまり、stack で指定された領域を他の空間に
 *  ミラーリングし、その前後にアクセス不可なガードページを設定した上で、
 *  そこをファイバのスタック領域として使用します。
 *
 *  fiberFlag に nn::os::FiberFlag_NoStackGuard が含まれる場合はスタックガードを
 *  用意しません。つまり、stack で指定された領域をそのままスタック領域として
 *  使用します。スタック領域の前後にガードページは設定されないため、
 *  スタックオーバーフロー等に注意して下さい。
 *
 *  カレントスレッドから初めてファイバへ遷移するには SwitchToFiber() を
 *  呼び出します。他のファイバから元々のスレッドへ戻るには、
 *  SwitchToFiber( NULL ) のようにファイバオブジェクトに NULL を指定します。
 *
 *  全てのファイバは、その動作優先度や nn::os::ThreadLocalStorage などの
 *  スレッドに関連付けられるものについては、カレントスレッドのコンテキストと
 *  共有しています。ただし、スタック領域だけはそれぞれのファイバで
 *  専用のものを持ちます。
 *
 *  ファイバは他のスレッドで生成されたものであっても SwitchToFiber() で
 *  指定することができます。ただし、他のスレッドで実行中のファイバを
 *  指定することは出来ません。
 *
 *  ファイバからリターンする際には、切り替え先のファイバを返値に指定する
 *  必要があります。元々のスレッドに戻る場合には返値に NULL を渡して下さい。
 *  また、一度リターンしたファイバに対して SwitchToFiber() は出来ません。
 *
 * @platformbegin{Windows}
 *  - ファイバが使用するスタック領域は、指定された stack の場所ではなく、
 *    Windows が独自に確保した領域が使用されます。
 *  - fiberFlag の nn::os::FiberFlag_NoStackGuard 指定は無視されます。
 *
 * @platformend
 *
 */
void InitializeFiber(FiberType* fiber, FiberFunction function, void* argument, void* stack, size_t stackSize, int fiberFlag) NN_NOEXCEPT;


//-----------------------------------------------------------------------------
/**
 * @brief   ファイバオブジェクトを破棄します。
 *
 * @param[in] fiber     FiberType オブジェクトへのポインタ
 *
 * @pre
 *  - fiber が Ready, Completed のいずれかの状態である
 *
 * @post
 *  - fiber が NotInitialized 状態である
 *
 * @details
 *  ファイバオブジェクトを破棄します。@n
 *  対象ファイバが Running 状態の場合には内部のアサートに失敗します。
 *
 */
void FinalizeFiber(FiberType* fiber) NN_NOEXCEPT;


//-----------------------------------------------------------------------------
/**
 * @brief   ファイバの実行を開始もしくは再開します。
 *
 * @param[in] fiber    FiberType オブジェクトへのポインタ
 *
 * @pre
 *  - fiber が Ready 状態である
 *  - fiber != GetCurrentFiber()
 *
 * @post
 *  - 呼び出し前のカレントファイバが Ready 状態である
 *  - fiber が Running 状態である
 *  - fiber == GetCurrentFiber()
 *
 * @details
 *  現在動作中のスレッドもしくはファイバを停止し、
 *  指定された fiber が指すファイバの実行を開始もしくは再開します。
 *
 *  fiber に NULL を指定すると、元々のスレッドから SwitchToFiber() を
 *  呼び出した場所へ復帰します。@n
 *  また、カレントファイバを fiber に指定することはできません。
 *
 */
void SwitchToFiber(FiberType* fiber) NN_NOEXCEPT;


//-----------------------------------------------------------------------------
/**
 * @brief   カレントファイバの FiberType オブジェクトを取得します。
 *
 * @return  FiberType オブジェクトへのポインタです。
 *
 * @details
 *  現在実行中のファイバの FiberType オブジェクトへのポインタを返します。@n
 *  現在実行中のファイバがない場合（単にスレッドを実行中の場合）には、
 *  NULL を返します。
 *
 */
FiberType* GetCurrentFiber()  NN_NOEXCEPT;


//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::os

