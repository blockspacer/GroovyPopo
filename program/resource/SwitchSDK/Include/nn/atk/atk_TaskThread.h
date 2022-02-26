/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/atk/atk_ThreadStack.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn {
namespace atk {
namespace detail {

class TaskThread : public fnd::Thread::Handler
{
public:
    TaskThread() NN_NOEXCEPT;
    virtual ~TaskThread() NN_NOEXCEPT NN_OVERRIDE;

    bool Create( int priority, void* stackBase, size_t stackSize, int idealCoreNumber, uint32_t affinityMask, FsPriority fsPriority ) NN_NOEXCEPT;
    void Destroy() NN_NOEXCEPT;

    bool IsCreated() const NN_NOEXCEPT { return m_IsCreated != 0; }

    void Lock() NN_NOEXCEPT
    {
        m_CriticalSection.Lock();
    }
    void Unlock() NN_NOEXCEPT
    {
        m_CriticalSection.Unlock();
    }
    static NN_NOINLINE TaskThread& GetInstance() NN_NOEXCEPT;

#if defined(NN_ATK_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    int GetThreadCoreNumber() const NN_NOEXCEPT
    {
        return m_Thread.GetCoreNumber();
    }
#endif

protected:
    virtual uint32_t Run(void* param) NN_NOEXCEPT NN_OVERRIDE;

private:
    nn::atk::detail::fnd::Thread m_Thread;
    nn::atk::detail::fnd::CriticalSection m_CriticalSection;
    volatile bool m_IsFinished;
    bool m_IsCreated;
    FsPriority m_FsPriority;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

