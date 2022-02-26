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
 * @brief   多重待ちに関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_Config.h>
#include <nn/os/os_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/detail/os_MacroImpl.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

namespace detail {
    class   MultiWaitImpl;
    struct  MultiWaitHolderImpl;
}

//--------------------------------------------------------------------------
/**
 * @brief   多重待ちオブジェクトのヘッダを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeMultiWait()
 * - @ref FinalizeMultiWait()
 * - @ref WaitAny()
 * - @ref TryWaitAny()
 * - @ref TimedWaitAny()
 * - @ref LinkMultiWaitHolder()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::MultipleWait ユーティリティクラスもあわせてご確認ください。
 */
struct MultiWaitType
{
    enum State
    {
        State_NotInitialized      = 0,  // 未初期化状態
        State_Initialized         = 1,  // 初期化状態
    };

    uint8_t     _state;
    bool        _isWaiting;

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    util::TypedStorage<detail::MultiWaitImpl,NN_OS_DETAIL_CALC_OBJECT_SIZE(32,6),NN_ALIGNOF(uint64_t)> _multiWaitImpl;
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    util::TypedStorage<detail::MultiWaitImpl,NN_OS_DETAIL_CALC_OBJECT_SIZE(24,2),NN_ALIGNOF(uint64_t)> _multiWaitImpl;
#else
    #error   "未サポートの OS 種別が指定されています。"
#endif
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(MultiWaitType);

//--------------------------------------------------------------------------
/**
 * @brief   多重待ちオブジェクトホルダーを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, EventType *event)
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, TimerEventType *timerEvent)
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, SystemEventType *event)
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, MessageQueueType *messageQueue, MessageQueueWaitType waitType)
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, SemaphoreType *semaphore)
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, ThreadType *thread)
 * - @ref FinalizeMultiWaitHolder()
 * - @ref LinkMultiWaitHolder()
 * - @ref UnlinkMultiWaitHolder()
 * - @ref UnlinkAllMultiWaitHolder()
 * - @ref MoveAllMultiWaitHolder()
 * - @ref SetMultiWaitHolderUserData()
 * - @ref GetMultiWaitHolderUserData()
 *
 * また、各種 WaitAny() 関数が、本オブジェクトを返します。
 *
 * - @ref WaitAny()
 * - @ref TryWaitAny()
 * - @ref TimedWaitAny()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 */
struct MultiWaitHolderType
{
    util::TypedStorage<detail::MultiWaitHolderImpl,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,7),NN_ALIGNOF(void*)>    _holderImpl;
    uintptr_t   userData;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(MultiWaitHolderType);

}} // namespace nn::os

