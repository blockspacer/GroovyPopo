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
 * @brief   バリアに関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/os/detail/os_InternalConditionVariable.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   バリアオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeBarrier()
 * - @ref FinalizeBarrier()
 * - @ref AwaitBarrier()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::Barrier ユーティリティクラスもあわせてご確認ください。
 */
struct  BarrierType
{
    uint16_t _maxThreads;
    uint16_t _waitingThreads;
    uint32_t _baseCounterLower;
    uint32_t _baseCounterUpper;

    // 排他制御用
    detail::InternalCriticalSectionStorage   _csBarrier;
    detail::InternalConditionVariableStorage _cvGathered;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(BarrierType);


}} // namespace nn::os

