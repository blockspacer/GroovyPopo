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

#include <nn/nn_Macro.h>

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/os/OSMutex.h>
#else // NN_BUILD_CONFIG_SPEC_GENERIC
#include <nn/os/os_Mutex.h>
#endif

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

class CriticalSection
{
public:
    CriticalSection() NN_NOEXCEPT;
    ~CriticalSection() NN_NOEXCEPT;

public:
    void Enter() NN_NOEXCEPT;
    bool TryEnter() NN_NOEXCEPT;
    void Leave() NN_NOEXCEPT;

private:
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    OSMutex m_Mutex;
#else // NN_BUILD_CONFIG_SPEC_GENERIC
    nn::os::Mutex m_Mutex;
#endif
};

class ScopedCriticalSection
{
private:
    NN_DISALLOW_COPY(ScopedCriticalSection);

public:
    explicit ScopedCriticalSection(CriticalSection& lock) NN_NOEXCEPT : m_Lock(lock)
    {
        m_Lock.Enter();
    }

    ~ScopedCriticalSection() NN_NOEXCEPT
    {
        m_Lock.Leave();
    }

private:
    CriticalSection& m_Lock;
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection-spec.Cafe.h>
#else // NN_BUILD_CONFIG_SPEC_GENERIC
#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection-spec.Generic.h>
#endif
