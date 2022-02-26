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
 * @brief   ファイバに関する共通定義
 */

#pragma once

#include <nn/nn_Common.h>


namespace nn { namespace os {

struct FiberType;

//-----------------------------------------------------------------------------
/**
 * @brief   ファイバスタック領域のアライメント制約を表す定数です。
 *
 * @details
 *  この定数は、スタックガードを設置しないファイバのスタック領域に対する
 *  アライメント制約を示します。@n
 *  スタックガードを設置するファイバのスタック領域に対するアライメントは
 *  nn::os::GuardedStackAlignment に従って下さい。
 */
const size_t FiberStackAlignment = 16;


//-----------------------------------------------------------------------------
/**
 * @brief   ガードなしファイバスタック領域のアライメントを指定するマクロです。
 *
 * @details
 *  スタックガードを設置しないファイバのスタック領域としてのアライメント制約を
 *  満たすようにするためのマクロです。スタック用のバッファ定義に対して、
 *  以下のように使用することで nn::os::FiberStackAlignment の定数値に等しい
 *  アライメントに合わせることができます。
 *
 *  スタックガードを設置するファイバのスタック領域に対するアライメント調整には
 *  #NN_OS_ALIGNAS_GUARDED_STACK マクロを使用して下さい。
 *
 *  使用例：
 *      @code
 *      NN_OS_ALIGNAS_FIBER_STACK    uint8_t  fiberStack[8192];
 *      @endcode
 *
 */
#define NN_OS_ALIGNAS_FIBER_STACK  NN_ALIGNAS(16)


//-----------------------------------------------------------------------------
/**
 * @brief   ファイバ関数のエントリを表す型です。
 *
 * @param[in] argument  ファイバ関数が受け取る引数
 *
 * @return  遷移先のファイバを示す FiberType オブジェクトへのポインタ
 *
 * @details
 *  ファイバ関数のエントリを表す型です。
 *
 *  ファイバ関数からリターンで終了する際には、遷移先のファイバを示す
 *  FiberType ポインタを返値として渡して下さい。@n
 *  返値に NULL を渡した場合は、元々のスレッドが SwitchToFiber() を呼び出した
 *  ポイントに戻ります。詳細は InitializeFiber() も合わせて確認して下さい。
 *
 */
typedef FiberType* (*FiberFunction)(void* argument);


}} // namespace nn::os

