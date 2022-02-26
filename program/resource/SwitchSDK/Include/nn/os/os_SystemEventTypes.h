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
 * @brief   SystemEvent に関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_Macro.h>
#include <nn/os/os_EventTypes.h>
#include <nn/os/os_NativeHandleTypes.h>
#include <nn/os/detail/os_MacroImpl.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

namespace detail {
//--------------------------------------------------------------------------
//  プロセス間同期イベントオブジェクトを定義する構造体です。
//  この構造体は SystemEventType の内部で使用されます。
//
struct  InterProcessEventType
{
    enum State
    {
        State_NotInitialized    = 0,
        State_Initialized       = 1,
    };

    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectList;

    bool                _autoClear;
    uint8_t             _state;
    bool                _isReadableHandleManaged;
    bool                _isWritableHandleManaged;

    NativeHandle        _readableHandle;
    NativeHandle        _writableHandle;
};

}   // namespace detail


//--------------------------------------------------------------------------
/**
 * @brief   システムイベントオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref DestroySystemEvent()
 * - @ref WaitSystemEvent()
 * - @ref TryWaitSystemEvent()
 * - @ref TimedWaitSystemEvent()
 * - @ref ClearSystemEvent()
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, SystemEventType* event)
 *
 * @cond SECTION_selectable_DeviceDriverKit
 * - @ref CreateSystemEvent()
 * - @ref AttachSystemEvent()
 * - @ref AttachReadableHandleToSystemEvent()
 * - @ref AttachWritableHandleToSystemEvent()
 * - @ref DetachReadableHandleOfSystemEvent()
 * - @ref DetachWritableHandleOfSystemEvent()
 * - @ref SignalSystemEvent()
 * - @ref GetReadableHandleOfSystemEvent()
 * - @ref GetWritableHandleOfSystemEvent()
 * @endcond
 *
 * 上記の関数を利用するには nn/os/os_SystemEvent.h をインクルードする必要があります。
 *
 * SystemEvent ユーティリティクラスもあわせてご確認ください。
 */
struct  SystemEventType
{
    union
    {
        EventType                       _event;
        detail::InterProcessEventType   _interProcessEvent;
    };

    //----------------------------------------------------------------------
    //  システムイベントの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized                    = 0,
        State_InitializedAsEvent                = 1,
        State_InitializedAsInterProcessEvent    = 2,
    };

    uint8_t     _state;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

