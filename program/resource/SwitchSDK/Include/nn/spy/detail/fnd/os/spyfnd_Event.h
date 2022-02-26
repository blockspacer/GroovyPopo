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

#include <nn/spy/spy_Config.h>

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/os/OSEvent.h>
#else // NN_BUILD_CONFIG_SPEC_GENERIC
#include <nn/os/os_Event.h>
#endif

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

class Event
{
public:
    Event(bool isManual, bool initialState) NN_NOEXCEPT;
    ~Event() NN_NOEXCEPT;

public:
    void Wait() NN_NOEXCEPT;
    bool TryWait() NN_NOEXCEPT;
    void Set() NN_NOEXCEPT;
    void Reset() NN_NOEXCEPT;

private:
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    OSEvent m_Event;
#else // NN_BUILD_CONFIG_SPEC_GENERIC
    nn::os::Event m_Event;
#endif
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <nn/spy/detail/fnd/os/spyfnd_Event-spec.Cafe.h>
#else
#include <nn/spy/detail/fnd/os/spyfnd_Event-spec.Generic.h>
#endif
