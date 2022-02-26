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
 * @brief   条件変数に関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>
#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalConditionVariable.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   条件変数オブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeConditionVariable()
 * - @ref FinalizeConditionVariable()
 * - @ref SignalConditionVariable()
 * - @ref BroadcastConditionVariable()
 * - @ref WaitConditionVariable()
 * - @ref TimedWaitConditionVariable()
 * - @ref NN_OS_CONDITION_VARIABLE_INITIALIZER()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::ConditionVariable ユーティリティクラスもあわせてご確認ください。
 */
struct  ConditionVariableType
{
    //----------------------------------------------------------------------
    //  条件変数の状態を表す列挙型です。
    enum State
    {
        State_NotInitialized      = 0,
        State_Initialized         = 1,
    };

    // メンバ変数
    uint8_t                 _state;
#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #if defined(NN_BUILD_CONFIG_ADDRESS_32)
        uint8_t             _needLazyInitialize;
    #elif defined(NN_BUILD_CONFIG_ADDRESS_64)
        uint32_t            _needLazyInitialize;
    #endif
#endif

    // 排他制御用
    union
    {
        // 静的初期化用イメージを定義するための配列を最初に配置しておく
        int32_t _conditionImage[sizeof(detail::InternalConditionVariableStorage) / sizeof(int32_t)];
        detail::InternalConditionVariableStorage _condition;
    };
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(ConditionVariableType);

//--------------------------------------------------------------------------

}} // namespace nn::os

