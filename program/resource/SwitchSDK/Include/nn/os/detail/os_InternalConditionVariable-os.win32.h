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
 * @brief   OS-API 内部実装用の条件変数の型宣言（Windows 版）
 */

#pragma once

#include <nn/os/os_Config.h>

#if !defined(NN_BUILD_CONFIG_OS_SUPPORTS_WIN32)
    #error  "OS 種別として Win32 が指定されていません。"
#endif

#include <nn/nn_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_Macro.h>
#include <nn/os/os_ConditionVariableCommon.h>
#include <nn/os/detail/os_MacroImpl.h>
#include <nn/os/detail/os_InternalCriticalSection.h>

//--------------------------------------------------------------------------

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------
//  InternalConditionVariableImplByWin32 の Windows 実装
//

//  前方宣言
class   TimeoutHelper;
struct  ConditionVariableWin32;

typedef nn::util::TypedStorage<ConditionVariableWin32,NN_OS_DETAIL_CALC_OBJECT_SIZE(0,2),NN_ALIGNOF(void*)> ConditionVariableWin32Storage;

class   InternalConditionVariableImplByWin32
{
public:
    InternalConditionVariableImplByWin32()  NN_NOEXCEPT;
    ~InternalConditionVariableImplByWin32() NN_NOEXCEPT;

    void Initialize() NN_NOEXCEPT;
    void    Signal()            NN_NOEXCEPT;
    void    Broadcast()         NN_NOEXCEPT;

    void    Wait(InternalCriticalSection* p) NN_NOEXCEPT;
    ConditionVariableStatus TimedWait(InternalCriticalSection* p, const TimeoutHelper& timeout)  NN_NOEXCEPT;

private:
    // Windows の条件変数
    ConditionVariableWin32Storage   m_conditionVariableWin32;
};

typedef InternalConditionVariableImplByWin32    InternalConditionVariableImpl;

}   // namespace detail
}}  // namespace nn::os

