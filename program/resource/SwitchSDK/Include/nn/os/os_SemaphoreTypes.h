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
 * @brief   セマフォに関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/os/os_Macro.h>
#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>

#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/os/detail/os_InternalConditionVariable.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

namespace detail {
    class  MultiWaitObjectList;
}

//--------------------------------------------------------------------------
/**
 * @brief   セマフォオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeSemaphore()
 * - @ref FinalizeSemaphore()
 * - @ref AcquireSemaphore()
 * - @ref TryAcquireSemaphore()
 * - @ref TimedAcquireSemaphore()
 * - @ref ReleaseSemaphore(SemaphoreType* semaphore)
 * - @ref ReleaseSemaphore(SemaphoreType* semaphore, int count)
 * - @ref GetCurrentSemaphoreCount()
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, SemaphoreType* semaphore)
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::Semaphore ユーティリティクラスもあわせてご確認ください。
 */
struct SemaphoreType
{
    //--------------------------------------------------------------------------
    //  セマフォの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized      = 0,
        State_Initialized         = 1,
    };

    // メンバ変数
    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectList;

    uint8_t             _state;
    int                 _count;
    int                 _maxCount;

    // 排他制御用
    detail::InternalCriticalSectionStorage   _csSemaphore;
    detail::InternalConditionVariableStorage _cvNotZero;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(SemaphoreType);

}} // namespace nn::os

