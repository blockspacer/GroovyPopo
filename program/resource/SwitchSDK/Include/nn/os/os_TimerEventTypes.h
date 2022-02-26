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
 * @brief   タイマーイベントに関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_TimeSpan.h>
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
 * @brief   タイマーイベントオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeTimerEvent()
 * - @ref FinalizeTimerEvent()
 * - @ref StartOneShotTimerEvent()
 * - @ref StartPeriodicTimerEvent()
 * - @ref StopTimerEvent()
 * - @ref WaitTimerEvent()
 * - @ref TryWaitTimerEvent()
 * - @ref SignalTimerEvent()
 * - @ref ClearTimerEvent()
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, TimerEventType* timerEvent)
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::TimerEvent ユーティリティクラスもあわせてご確認ください。
 */
struct  TimerEventType
{
    typedef util::TypedStorage<nn::TimeSpan,sizeof(nn::TimeSpan),NN_ALIGNOF(nn::TimeSpan)>  TimeSpanStorage;

    //----------------------------------------------------------------------
    //  タイマーイベントの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized      = 0,
        State_Initialized         = 1,
    };

    //----------------------------------------------------------------------
    //  タイマーイベントのタイマー動作状態を表す列挙型です。
    enum TimerState
    {
        TimerState_Stop         = 0,
        TimerState_OneShot      = 1,
        TimerState_Periodic     = 2,
    };

    // メンバ変数
    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectList;

    uint8_t             _state;
    uint8_t             _clearMode;
    bool                _signalState;
    uint8_t             _timerState;

    uint32_t            _broadcastCounterLower;
    uint32_t            _broadcastCounterUpper;

    TimeSpanStorage     _nextTimeToWakeup;
    TimeSpanStorage     _first;
    TimeSpanStorage     _interval;

    // 排他制御用
    detail::InternalCriticalSectionStorage   _csTimerEvent;
    detail::InternalConditionVariableStorage _cvSignaled;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(TimerEventType);

}} // namespace nn::os

