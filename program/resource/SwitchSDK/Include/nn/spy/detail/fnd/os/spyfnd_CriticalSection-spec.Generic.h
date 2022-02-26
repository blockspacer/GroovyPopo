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

#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection.h>

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

inline CriticalSection::CriticalSection() NN_NOEXCEPT
: m_Mutex(true)
{
}

inline CriticalSection::~CriticalSection() NN_NOEXCEPT
{
}

inline void CriticalSection::Enter() NN_NOEXCEPT
{
    m_Mutex.Lock();
}

inline bool CriticalSection::TryEnter() NN_NOEXCEPT
{
    return m_Mutex.TryLock();
}

inline void CriticalSection::Leave() NN_NOEXCEPT
{
    m_Mutex.Unlock();
}

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
