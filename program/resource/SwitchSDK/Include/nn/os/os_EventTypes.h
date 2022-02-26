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
 * @brief   イベントに関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_Macro.h>
#include <nn/os/os_EventCommon.h>

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
 * @brief   イベントオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeEvent()
 * - @ref FinalizeEvent()
 * - @ref SignalEvent()
 * - @ref WaitEvent()
 * - @ref TryWaitEvent()
 * - @ref TimedWaitEvent()
 * - @ref ClearEvent()
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType *multiWaitHolder, EventType *event)
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::Event ユーティリティクラスもあわせてご確認ください。
 */
struct  EventType
{
    //----------------------------------------------------------------------
    //  イベントの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized      = 0,
        State_Initialized         = 1,
    };

    // メンバ変数
    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectList;

    bool                _signalState;
    bool                _initiallySignaled;
    uint8_t             _clearMode;
    uint8_t             _state;

    uint32_t            _broadcastCounterLower;
    uint32_t            _broadcastCounterUpper;

    // 排他制御用
    detail::InternalCriticalSectionStorage   _csEvent;
    detail::InternalConditionVariableStorage _cvSignaled;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(EventType);

}} // namespace nn::os

