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
 * @brief   Readers-Writer ロックに関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/os/detail/os_InternalConditionVariable.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  ThreadType;

//--------------------------------------------------------------------------
/**
 * @brief   Readers-Writer ロックオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeReaderWriterLock()
 * - @ref FinalizeReaderWriterLock()
 * - @ref AcquireReadLock()
 * - @ref TryAcquireReadLock()
 * - @ref ReleaseReadLock()
 * - @ref AcquireWriteLock()
 * - @ref TryAcquireWriteLock()
 * - @ref ReleaseWriteLock()
 * - @ref IsReadLockHeld()
 * - @ref IsWriteLockHeldByCurrentThread()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * nn::os::ReaderWriterLock ユーティリティクラスもあわせてご確認ください。
 *
 */
struct  ReaderWriterLockType
{
    //----------------------------------------------------------------------
    //  Readers-Writer ロックの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized  = 0,
        State_Initialized     = 1,
    };

    uint8_t             _state;
    int                 _ReadLockCount;
    int                 _ReadLockWaiterCount;
    int                 _WriteLockCount;
    int                 _WriteLockWaiterCount;
    nn::os::ThreadType* _LockOwner;

    detail::InternalCriticalSectionStorage      _cs;
    detail::InternalConditionVariableStorage    _cvReadLockWaiter;
    detail::InternalConditionVariableStorage    _cvWriteLockWaiter;
};


NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(ReaderWriterLockType);

//--------------------------------------------------------------------------

}} // namespace nn::os

