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

#include <nn/spy/detail/fnd/os/spyfnd_Event.h>

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
inline Event::Event(bool isManual, bool initialState) NN_NOEXCEPT
    : m_Event(isManual ? nn::os::EventClearMode_ManualClear : nn::os::EventClearMode_AutoClear)
{
    if (initialState)
    {
        m_Event.Signal();
    }
}

//---------------------------------------------------------------------------
inline Event::~Event() NN_NOEXCEPT
{
}

//---------------------------------------------------------------------------
inline void
Event::Wait() NN_NOEXCEPT
{
    m_Event.Wait();
}

//---------------------------------------------------------------------------
inline bool
Event::TryWait() NN_NOEXCEPT
{
    return m_Event.TryWait();
}

//---------------------------------------------------------------------------
inline void
Event::Set() NN_NOEXCEPT
{
    m_Event.Signal();
}

//---------------------------------------------------------------------------
inline void
Event::Reset() NN_NOEXCEPT
{
    m_Event.Clear();
}

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
