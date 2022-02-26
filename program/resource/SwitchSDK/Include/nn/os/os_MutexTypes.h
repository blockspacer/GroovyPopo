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
 * @brief   ミューテックスに関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  ThreadType;

//--------------------------------------------------------------------------
/**
 * @brief   ミューテックスオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeMutex()
 * - @ref FinalizeMutex()
 * - @ref LockMutex()
 * - @ref TryLockMutex()
 * - @ref UnlockMutex()
 * - @ref IsMutexLockedByCurrentThread()
 * - @ref NN_OS_MUTEX_INITIALIZER()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::Mutex ユーティリティクラスもあわせてご確認ください。
 */
struct  MutexType
{
    //----------------------------------------------------------------------
    //  ミューテックスの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized  = 0,
        State_Initialized     = 1,
    };

    // メンバ変数
    uint8_t             _state;
    bool                _isRecursive;
    int                 _lockLevel;

    // 以下はミューテックス制御用
    int                 _nestCount;
    ThreadType*         _ownerThread;

    // 排他制御用
    union
    {
        // 静的初期化用イメージを定義するための配列を最初に配置しておく
        int32_t _mutexImage[sizeof(detail::InternalCriticalSectionStorage) / sizeof(int32_t)];
        detail::InternalCriticalSectionStorage _mutex;
    };
};


NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(MutexType);

//--------------------------------------------------------------------------

}} // namespace nn::os

