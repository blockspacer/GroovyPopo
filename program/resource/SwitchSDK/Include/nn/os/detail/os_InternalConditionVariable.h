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
 * @brief   OS-API 内部実装用の条件変数の型宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_ConditionVariableCommon.h>
#include <nn/os/detail/os_InternalCriticalSection.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_InternalConditionVariable-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_InternalConditionVariable-os.horizon.h>
#else
    #error "未サポートの OS 種別が指定されています。"
#endif

//--------------------------------------------------------------------------

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------
//  InternalConditionVariable の定義
//

class   InternalConditionVariable
{
public:
    void Initialize() NN_NOEXCEPT
    {
        m_Impl.Initialize();
    }
    void    Signal()    NN_NOEXCEPT
    {
        m_Impl.Signal();
    }
    void    Broadcast() NN_NOEXCEPT
    {
        m_Impl.Broadcast();
    }

    void    Wait(InternalCriticalSection* p) NN_NOEXCEPT
    {
        m_Impl.Wait(p);
    }

    ConditionVariableStatus TimedWait(InternalCriticalSection* p, const TimeoutHelper& timeout)  NN_NOEXCEPT
    {
        return m_Impl.TimedWait(p, timeout);
    }

private:
    InternalConditionVariableImpl   m_Impl;
};


typedef util::TypedStorage<InternalConditionVariable,sizeof(InternalConditionVariable),NN_ALIGNOF(InternalConditionVariable)> InternalConditionVariableStorage;


}   // namespace detail
}}  // namespace nn::os


