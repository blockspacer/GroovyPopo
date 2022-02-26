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
 * @brief   メッセージキューに関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_Macro.h>

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
 * @brief   メッセージキューオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeMessageQueue()
 * - @ref FinalizeMessageQueue()
 * - @ref SendMessageQueue()
 * - @ref TrySendMessageQueue()
 * - @ref TimedSendMessageQueue()
 * - @ref JamMessageQueue()
 * - @ref TryJamMessageQueue()
 * - @ref TimedJamMessageQueue()
 * - @ref ReceiveMessageQueue()
 * - @ref TryReceiveMessageQueue()
 * - @ref TimedReceiveMessageQueue()
 * - @ref PeekMessageQueue()
 * - @ref TryPeekMessageQueue()
 * - @ref TimedPeekMessageQueue()
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, MessageQueueType* messageQueue, MessageQueueWaitType waitType);
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::MessageQueue ユーティリティクラスもあわせてご確認ください。
 */
struct  MessageQueueType
{
    //----------------------------------------------------------------------
    //  メッセージキューの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized      = 0,
        State_Initialized         = 1,
    };

    // メンバ変数
    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectListNotFull;
    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectListNotEmpty;

    uintptr_t*          _buffer;
    int                 _maxCount;

    // メッセージキュー制御用変数
    int                 _count;
    int                 _offset;
    uint8_t             _state;

    // 排他制御用
    mutable detail::InternalCriticalSectionStorage      _csQueue;
    mutable detail::InternalConditionVariableStorage    _cvNotFull;
    mutable detail::InternalConditionVariableStorage    _cvNotEmpty;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(MessageQueueType);

}} // namespace nn::os

