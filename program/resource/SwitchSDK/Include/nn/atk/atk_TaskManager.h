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

#include <nn/os/os_MessageQueue.h>

#include <nn/atk/atk_Task.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_TaskProfileReader.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn {
namespace atk {
namespace detail {

class TaskManager
{
public:
    enum TaskPriority
    {
        TaskPriority_Low,
        TaskPriority_Middle,
        TaskPriority_High
    };

    static NN_NOINLINE TaskManager& GetInstance() NN_NOEXCEPT;

    void Initialize(bool isEnableProfiling) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void ExecuteTask() NN_NOEXCEPT;
    void AppendTask( Task* task, TaskPriority priority ) NN_NOEXCEPT;
    void CancelTask( Task* task ) NN_NOEXCEPT;
    void CancelTaskById( uint32_t id ) NN_NOEXCEPT;
    void CancelAllTask() NN_NOEXCEPT;

    void WaitTask() NN_NOEXCEPT;
    void CancelWaitTask() NN_NOEXCEPT;
    bool TryRemoveTask(Task* task) NN_NOEXCEPT;

    void RegisterTaskProfileReader(nn::atk::TaskProfileReader& reader) NN_NOEXCEPT
    {
        m_TaskProfileLogger.RegisterReader( reader );
    }
    void UnregisterTaskProfileReader(const nn::atk::TaskProfileReader& reader) NN_NOEXCEPT
    {
        m_TaskProfileLogger.UnregisterReader( reader );
    }

private:
    static const int PriorityCount = TaskPriority_High - TaskPriority_Low + 1;
    static const int ThreadMessageBufferSize = 32;

    enum Message
    {
        Message_Append
    };

    typedef util::IntrusiveList< Task, util::IntrusiveListMemberNodeTraits<Task,&Task::m_TaskLink>> TaskList;

    TaskManager() NN_NOEXCEPT;
    Task* PopTask() NN_NOEXCEPT;
    Task* PeekTask() NN_NOEXCEPT;
    void RemoveTaskById(uint32_t id) NN_NOEXCEPT;

    Task* GetNextTask( TaskPriority priority, bool doRemove ) NN_NOEXCEPT;

    TaskList m_TaskList[ PriorityCount ];

    volatile bool m_IsWaitTaskCancel;

    fnd::CriticalSection m_CriticalSection;
    nn::os::MessageQueue m_BlockingQueue;
    uintptr_t m_MsgBuffer[ ThreadMessageBufferSize ];

    nn::atk::TaskProfileLogger m_TaskProfileLogger;
};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

