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
#include <nn/os/os_Config.h>
#include <nn/os/os_Macro.h>
#include <nn/os/os_EventCommon.h>

#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/os/detail/os_InternalConditionVariable.h>

#include <atomic>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   軽量イベントオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeLightEvent()
 * - @ref FinalizeLightEvent()
 * - @ref SignalLightEvent()
 * - @ref WaitLightEvent()
 * - @ref TryWaitLightEvent()
 * - @ref TimedWaitLightEvent()
 * - @ref ClearLightEvent()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::LightEvent ユーティリティクラスもあわせてご確認ください。
 *
 */
struct  LightEventType
{
#if defined(NN_BUILD_CONFIG_OS_HORIZON)
    static const int32_t NotSignaledAndNoWaiter = 0;
    static const int32_t NotSignaledAndWaiter   = 1;
    static const int32_t Signaled               = 2;

    std::atomic<int32_t> _signalState;
    bool                 _isAutoClear;
    bool                 _isInitialized;
    uint8_t              _padding1[2];
    uint32_t             _padding2;
#else
    static const int8_t SignaledManual      = 2;
    static const int8_t SignaledAuto        = 1;
    static const int8_t NotInitialized      = 0;
    static const int8_t NotSignaledAuto     = -1;
    static const int8_t NotSignaledManual   = -2;

    uint16_t            _broadcastCounterLower;
    uint8_t             _broadcastCounterUpper;
    std::atomic<int8_t> _signalState;

    detail::InternalCriticalSectionStorage   _mutex;
    detail::InternalConditionVariableStorage _cond;
#endif
};

//NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(LightEventType);

}} // namespace nn::os

