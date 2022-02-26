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
 * @brief   OS-API 内部実装用のクリティカルセクションの型宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_TypedStorage.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_InternalCriticalSection-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_InternalCriticalSection-os.horizon.h>
#else
    #error "未サポートの OS 種別が指定されています。"
#endif

//--------------------------------------------------------------------------

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------
//  InternalCriticalSection の定義
//

class   InternalCriticalSection
{
public:
    // メンバ関数
    void Initialize() NN_NOEXCEPT
    {
        m_Impl.Initialize();
    }
    void Finalize() NN_NOEXCEPT
    {
        m_Impl.Finalize();
    }
    void    Enter()         NN_NOEXCEPT
    {
        m_Impl.Enter();
    }
    bool    TryEnter()      NN_NOEXCEPT
    {
        return m_Impl.TryEnter();
    }
    void    Leave()         NN_NOEXCEPT
    {
        m_Impl.Leave();
    }
    bool IsLockedByCurrentThread() const NN_NOEXCEPT
    {
        return m_Impl.IsLockedByCurrentThread();
    }

    // std::lock_guard 用の定義
    void    lock()          NN_NOEXCEPT
    {
        m_Impl.Enter();
    }
    void    unlock()        NN_NOEXCEPT
    {
        m_Impl.Leave();
    }

    InternalCriticalSectionImpl* Get() NN_NOEXCEPT
    {
        return &m_Impl;
    }

private:
    InternalCriticalSectionImpl m_Impl;
};

typedef util::TypedStorage<InternalCriticalSection,sizeof(InternalCriticalSection),NN_ALIGNOF(InternalCriticalSection)> InternalCriticalSectionStorage;

}   // namespace detail
}}  // namespace nn::os

