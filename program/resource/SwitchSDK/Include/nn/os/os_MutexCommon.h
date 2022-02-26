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
 * @brief   ミューテックスに関する共通定義
 */

#pragma once

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//----------------------------------------------------------------------------
/**
 * @brief   ミューテックスのロックレベルの最小値を表す定数です。
 */
const int   MutexLockLevelMin = 1;

//----------------------------------------------------------------------------
/**
 * @brief   ミューテックスのロックレベルの最大値を表す定数です。
 */
const int   MutexLockLevelMax = 31;

//----------------------------------------------------------------------------
/**
 * @brief   カレントスレッドのミューテックスロックレベルの初期値を表す定数です。
 */
const int   MutexLockLevelInitial = 0;

//----------------------------------------------------------------------------
/**
 * @brief   ミューテックスの再帰ロック回数の最大値を表す定数です。
 */
const int   MutexRecursiveLockCountMax = 65535;


}} // namespace nn::os

