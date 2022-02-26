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

#include <nn/nn_Macro.h>
#include <nn/os/os_ConditionVariableCommon.h>
#include <nn/os/detail/os_InternalCriticalSection.h>

namespace nn { namespace os { namespace detail {

class TimeoutHelper;
class InternalConditionVariableImplByHorizon
{
public:
    InternalConditionVariableImplByHorizon() NN_NOEXCEPT;

    void Initialize() NN_NOEXCEPT;
    void Signal() NN_NOEXCEPT;
    void Broadcast() NN_NOEXCEPT;

    void Wait(InternalCriticalSection* p) NN_NOEXCEPT;
    ConditionVariableStatus TimedWait(InternalCriticalSection* p, const TimeoutHelper& timeout) NN_NOEXCEPT;

private:
    uint32_t m_Value;
};

typedef InternalConditionVariableImplByHorizon InternalConditionVariableImpl;

}}} // namespace nn::os::detail

