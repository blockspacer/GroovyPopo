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

#include <nn/os/os_Event.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {

class TaskProfileLogger;

namespace detail {

/* ========================================================================
        Task class
   ======================================================================== */
class TaskManager;

class Task
{
    friend class TaskManager;

public:
    enum Status {
        Status_Free,
        Status_Append,
        Status_Execute,
        Status_Done,
        Status_Cancel
    };

    Task() NN_NOEXCEPT;
    virtual ~Task() NN_NOEXCEPT;

    void SetId( uint32_t id ) NN_NOEXCEPT { m_Id = id; }
    Status GetStatus() const NN_NOEXCEPT { return m_Status; }

    void Wait() NN_NOEXCEPT { m_Event.Wait(); }
    bool TryWait() NN_NOEXCEPT { return m_Event.TryWait(); }

protected:
    // タスク処理を実装する
    // NOTE: Execute関数内で、Taskインスタンスを解放しないこと
    virtual void Execute(TaskProfileLogger& logger) NN_NOEXCEPT = 0;
    void InitializeStatus() NN_NOEXCEPT { m_Status = Status_Free; }

private:
    NN_DISALLOW_COPY( Task );

    util::IntrusiveListNode m_TaskLink;
    nn::os::Event m_Event;
    Status m_Status;
    uint32_t m_Id;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

