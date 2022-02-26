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

#include <cstdint>
#include <nn/nn_Macro.h>
#include <nn/atk/fnd/basis/atkfnd_Config.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  時間間隔を管理するクラスです。
//---------------------------------------------------------------------------
class TimeSpan
{
public:
    typedef int64_t TickType;

public:
    TimeSpan() NN_NOEXCEPT : m_TickSpan(0) { }
    explicit TimeSpan(TickType tickSpan) NN_NOEXCEPT : m_TickSpan(tickSpan) { }

public:
    static TimeSpan FromNanoSeconds(int64_t value) NN_NOEXCEPT;
    static TimeSpan FromMicroSeconds(int64_t value) NN_NOEXCEPT;
    static TimeSpan FromMilliSeconds(int64_t value) NN_NOEXCEPT;

    TickType GetTick() const NN_NOEXCEPT { return m_TickSpan; }

    int64_t ToNanoSeconds() const NN_NOEXCEPT;
    int64_t ToMicroSeconds() const NN_NOEXCEPT;
    int64_t ToMilliSeconds() const NN_NOEXCEPT;

    TimeSpan& operator =(const TimeSpan& value) NN_NOEXCEPT { m_TickSpan = value.m_TickSpan; return *this; }
    TimeSpan& operator =(TickType value) NN_NOEXCEPT { m_TickSpan = value; return *this; }

    NN_IMPLICIT operator TickType() const NN_NOEXCEPT { return m_TickSpan; }

    TimeSpan operator +(const TimeSpan& right) const NN_NOEXCEPT { return TimeSpan(m_TickSpan + right.m_TickSpan); }
    TimeSpan operator -(const TimeSpan& right) const NN_NOEXCEPT { return TimeSpan(m_TickSpan - right.m_TickSpan); }
    TimeSpan operator *(const TimeSpan& right) const NN_NOEXCEPT { return TimeSpan(m_TickSpan * right.m_TickSpan); }
    TimeSpan operator /(const TimeSpan& right) const NN_NOEXCEPT { return TimeSpan(m_TickSpan / right.m_TickSpan); }

    TimeSpan& operator +=(const TimeSpan& rhs) NN_NOEXCEPT { m_TickSpan += rhs.m_TickSpan; return *this; }
    TimeSpan& operator -=(const TimeSpan& rhs) NN_NOEXCEPT { m_TickSpan -= rhs.m_TickSpan; return *this; }
    TimeSpan& operator *=(const TimeSpan& rhs) NN_NOEXCEPT { m_TickSpan *= rhs.m_TickSpan; return *this; }
    TimeSpan& operator /=(const TimeSpan& rhs) NN_NOEXCEPT { m_TickSpan /= rhs.m_TickSpan; return *this; }

    bool operator ==(const TimeSpan& rhs) const NN_NOEXCEPT { return (rhs.m_TickSpan == m_TickSpan); }
    bool operator !=(const TimeSpan& rhs) const NN_NOEXCEPT { return !(*this == rhs); }

    bool operator < (const TimeSpan& right) const NN_NOEXCEPT { return m_TickSpan < right.m_TickSpan; }
    bool operator > (const TimeSpan& right) const NN_NOEXCEPT { return m_TickSpan > right.m_TickSpan; }
    bool operator <=(const TimeSpan& rhs)   const NN_NOEXCEPT { return m_TickSpan <= rhs.m_TickSpan; }
    bool operator >=(const TimeSpan& rhs)   const NN_NOEXCEPT { return m_TickSpan >= rhs.m_TickSpan; }

private:
    TickType m_TickSpan;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  時間を管理するクラスです。
//---------------------------------------------------------------------------
class Time
{
private:
    Time() NN_NOEXCEPT { }

public:
    static TimeSpan Current() NN_NOEXCEPT;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  経過時間を計測するストップウォッチクラスです。
//---------------------------------------------------------------------------
class StopWatch
{
public:
    StopWatch() NN_NOEXCEPT : m_IsStarted(false) { }

public:
    void Start() NN_NOEXCEPT
    {
        if(!m_IsStarted)
        {
            m_IsStarted = true;
            Reset();
        }
    }

    void Stop() NN_NOEXCEPT
    {
        if(m_IsStarted)
        {
            m_IsStarted = false;
            m_StopTime = Time::Current();
        }
    }

    bool IsStarted() const NN_NOEXCEPT
    {
        return m_IsStarted;
    }

    TimeSpan GetElapsedTime() const NN_NOEXCEPT
    {
        return m_IsStarted ? Time::Current() - m_StartTime : m_StopTime - m_StartTime;
    }

    void Reset() NN_NOEXCEPT
    {
        m_StartTime = Time::Current();
        m_StopTime  = m_StartTime;
    }

private:
    TimeSpan m_StartTime;
    TimeSpan m_StopTime;
    bool     m_IsStarted;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t  m_Padding[7];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
