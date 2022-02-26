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

#include <nn/spy/detail/fnd/basis/spyfnd_Config.h>

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#else // NN_BUILD_CONFIG_SPEC_GENERIC
#include <nn/nn_TimeSpan.h>
#endif

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @brief  時間間隔を管理するクラスです。
//---------------------------------------------------------------------------
class TimeSpan
{
private:
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    typedef OSTime TimeSpanBaseType;
#else // NN_BUILD_CONFIG_SPEC_GENERIC
    typedef nn::TimeSpan TimeSpanBaseType;
#endif

public:
    TimeSpan() NN_NOEXCEPT : m_TimeSpan(0) { }

private:
    explicit TimeSpan(TimeSpanBaseType timeSpan) NN_NOEXCEPT : m_TimeSpan(timeSpan) { }

public:
    static TimeSpan FromNanoSeconds(int64_t value) NN_NOEXCEPT;
    static TimeSpan FromMicroSeconds(int64_t value) NN_NOEXCEPT;
    static TimeSpan FromMilliSeconds(int64_t value) NN_NOEXCEPT;

    int64_t ToNanoSeconds() const NN_NOEXCEPT;
    int64_t ToMicroSeconds() const NN_NOEXCEPT;
    int64_t ToMilliSeconds() const NN_NOEXCEPT;

    TimeSpan& operator =(const TimeSpan& value) NN_NOEXCEPT { m_TimeSpan = value.m_TimeSpan; return *this; }

    TimeSpan operator +(const TimeSpan& right) const NN_NOEXCEPT { return TimeSpan(m_TimeSpan + right.m_TimeSpan); }
    TimeSpan operator -(const TimeSpan& right) const NN_NOEXCEPT { return TimeSpan(m_TimeSpan - right.m_TimeSpan); }

    TimeSpan& operator +=(const TimeSpan& rhs) NN_NOEXCEPT { m_TimeSpan += rhs.m_TimeSpan; return *this; }
    TimeSpan& operator -=(const TimeSpan& rhs) NN_NOEXCEPT { m_TimeSpan -= rhs.m_TimeSpan; return *this; }

    bool operator ==(const TimeSpan& rhs) const NN_NOEXCEPT { return (rhs.m_TimeSpan == m_TimeSpan); }
    bool operator !=(const TimeSpan& rhs) const NN_NOEXCEPT { return !(*this == rhs); }

    bool operator < (const TimeSpan& right) const NN_NOEXCEPT { return m_TimeSpan < right.m_TimeSpan; }
    bool operator > (const TimeSpan& right) const NN_NOEXCEPT { return m_TimeSpan > right.m_TimeSpan; }
    bool operator <=(const TimeSpan& rhs)   const NN_NOEXCEPT { return m_TimeSpan <= rhs.m_TimeSpan; }
    bool operator >=(const TimeSpan& rhs)   const NN_NOEXCEPT { return m_TimeSpan >= rhs.m_TimeSpan; }

private:
    TimeSpanBaseType m_TimeSpan;

};

//---------------------------------------------------------------------------
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
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
