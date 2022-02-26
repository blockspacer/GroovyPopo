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
 * @brief   スレッドに関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/os/os_Macro.h>

#include <type_traits>
#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_ThreadCommon.h>
#include <nn/os/os_ThreadLocalStorage.h>

#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/os/detail/os_InternalConditionVariable.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_ThreadTypes-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_ThreadTypes-os.horizon.h>
#else
    #error   "未サポートの OS 種別が選択されています"
#endif

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace util {
    class  IntrusiveListNode;
}}

namespace nn { namespace os {
    struct FiberType;

namespace detail {
    class  MultiWaitObjectList;
}

//--------------------------------------------------------------------------
/**
 * @brief   スレッドオブジェクトを定義する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref CreateThread(ThreadType* thread, ThreadFunction function, void* argument, void* stack, size_t stackSize, int priority, int coreNumber)
 * - @ref CreateThread(ThreadType* thread, ThreadFunction function, void* argument, void* stack, size_t stackSize, int priority)
 * - @ref DestroyThread()
 * - @ref StartThread()
 * - @ref GetCurrentThread()
 * - @ref WaitThread()
 * - @ref YieldThread()
 * - @ref SleepThread()
 * - @ref ChangeThreadPriority()
 * - @ref GetThreadPriority()
 * - @ref GetThreadCurrentPriority()
 * - @ref SetThreadName()
 * - @ref SetThreadNamePointer()
 * - @ref GetThreadNamePointer()
 * - @ref GetCurrentCoreNumber()
 * - @ref SetThreadCoreMask()
 * - @ref GetThreadCoreMask()
 * - @ref GetThreadAvailableCoreMask()
 * - @ref InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, ThreadType* thread)
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 */
struct  ThreadType
{
    //----------------------------------------------------------------------
    //  スレッドの状態を表す列挙型です。
    enum State
    {
        State_NotInitialized            = 0,
        State_Initialized               = 1,
        State_DestroyedBeforeStarted    = 2,
        State_Started                   = 3,
        State_Exited                    = 4,
    };

    // メンバ変数
    util::TypedStorage<util::IntrusiveListNode,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _allThreadsListNode;

    util::TypedStorage<detail::MultiWaitObjectList,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> _multiWaitObjectList;

    NN_OS_DETAIL_PADDING_POINTER;
    NN_OS_DETAIL_PADDING_POINTER;
    NN_OS_DETAIL_PADDING_POINTER;
    NN_OS_DETAIL_PADDING_POINTER;

    uint8_t             _state;
    bool                _stackIsAliased;
    bool                _autoRegistered;
    uint8_t             _suspendCount;

    int16_t             _basePriority;
    uint16_t            _version;
    void*               _originalStack;
    void*               _stack;
    size_t              _stackSize;
    void*               _argument;
    ThreadFunction      _threadFunction;
    FiberType*          _currentFiber;
    FiberType*          _initialFiber;
    mutable uintptr_t   _tlsValueArray[TlsSlotCountMax + SdkTlsSlotCountMax];
    char                _threadNameBuffer[ThreadNameLengthMax];
    const char*         _namePointer;

    // 排他制御用
    mutable detail::InternalCriticalSectionStorage   _csThread;
    mutable detail::InternalConditionVariableStorage _cvThread;

    // 環境依存
    detail::InternalThreadHandle    _handle;

    uint32_t            _lockHistory;
#if defined(NN_BUILD_CONFIG_OS_HORIZON) && defined(NN_BUILD_CONFIG_ABI_LP64)
    Bit64               _threadId;
#elif defined(NN_BUILD_CONFIG_OS_HORIZON) && defined(NN_BUILD_CONFIG_ABI_ILP32)
    Bit32               _threadIdLower;
    Bit32               _threadIdUpper;
#endif
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(ThreadType);

const int IdealCoreDontCare             = -1;  //!< 優先コア番号なしに設定します
const int IdealCoreUseDefaultValue      = -2;  //!< 自プログラムに設定された優先コアでのみ動作します。
const int IdealCoreNoUpdate             = -3;  //!< 優先コア番号の更新を行いません

}} // namespace nn::os

