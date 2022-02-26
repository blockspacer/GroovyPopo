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
 * @brief   スレッドに関する共通定義
 */

#pragma once

#include <nn/nn_Common.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   スレッド名の最大文字数を表す定数です。@n
 *          文字列終端の '\0' 文字を含んだ文字数です。
 */
const int   ThreadNameLengthMax = 32;

//--------------------------------------------------------------------------
//  スレッドの優先度の範囲
const int   ThreadPriorityRangeSize = 32;

/**
 * @brief   スレッドの最高優先度を表す定数です。0 です。
 */
const int   HighestThreadPriority = 0;

/**
 * @brief   スレッド優先度のデフォルト値を表す定数です。16 です。
 */
const int   DefaultThreadPriority = (ThreadPriorityRangeSize / 2);

/**
 * @brief   スレッドの最低優先度を表す定数です。31 です。
 */
const int   LowestThreadPriority  = (ThreadPriorityRangeSize - 1);

const int   InvalidThreadPriority = 127;


//--------------------------------------------------------------------------
/**
 * @brief   ガード付きスタック領域のアライメント制約を表す定数です。
 */
const size_t GuardedStackAlignment = 4096;


//--------------------------------------------------------------------------
/**
 * @brief   ガード付きスタック領域のアライメントを指定するマクロです。
 *
 * @details
 *  ガード付きのスタック領域としてのアライメント制約を満たすようにするための
 *  マクロです。スタック用のバッファ定義に対して、以下のように使用することで
 *  nn::os::GuardedStackAlignment の定数値に等しいアライメントに合わせることが
 *  できます。
 *
 *  使用例：
 *      @code
 *      NN_OS_ALIGNAS_GUARDED_STACK  uint8_t  threadStack[8192];
 *      @endcode
 *
 */
#define NN_OS_ALIGNAS_GUARDED_STACK     NN_ALIGNAS(4096)



//--------------------------------------------------------------------------
/**
 * @brief   スレッドのスタック領域のアライメント制約を表す定数です。
 *
 */
const size_t ThreadStackAlignment = 4096;


//--------------------------------------------------------------------------
/**
 * @brief   スタック領域のアライメントを指定するマクロです。
 *
 * @details
 *  スレッドのスタック領域としてのアライメント制約を満たすようにするための
 *  マクロです。スタック用のバッファ定義に対して、以下のように使用することで
 *  nn::os::ThreadStackAlignment の定数値に等しいアライメントに合わせることが
 *  できます。
 *
 *  使用例：
 *      @code
 *      NN_OS_ALIGNAS_THREAD_STACK    uint8_t  threadStack[8192];
 *      @endcode
 *
 */
#define NN_OS_ALIGNAS_THREAD_STACK  NN_ALIGNAS(4096)


//--------------------------------------------------------------------------
/**
 * @brief   スレッド関数のエントリを表す型です。
 */
typedef void    (*ThreadFunction)(void*);




//--------------------------------------------------------------------------
//  これ以降は deprecated なものを定義しています。
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
/**
 * @brief   スタック領域のアライメント制約を表す定数です。
 *
 * @deprecated
 *
 * @details
 *  この定数は削除予定です。 nn::os::ThreadStackAlignment を使用して下さい。
 */
const size_t StackRegionAlignment = ThreadStackAlignment;

}} // namespace nn::os

