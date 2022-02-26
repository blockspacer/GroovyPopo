/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief Nintendo NX CPU Profiler Types used in interacting with the profiler API
 * @details Nintendo NX CPU Profiler API
 */

#pragma once

#include <nn/nn_Common.h>


namespace nn { namespace profiler {

/**
 *  @brief An enumeration of the different states the profiler can be in.
 */
enum ProfilerStatus
{
    /** @brief The profiler has not yet been initialized, or has been finalized. */
    ProfilerStatus_Offline = 0,

    /** @brief The profiler is ready to take a profile. */
    ProfilerStatus_Active = 1,

    /** @brief The profiler is currently taking a profile. */
    ProfilerStatus_Profiling = 2,

    /** @brief The profiler has finished profiling but is still transferring data to the PC. */
    ProfilerStatus_Transferring = 3,
    ProfilerStatus_Max,
};



/**
 *  @brief Sets the name of a heartbeat when calling RecordHeartbeat().
 *
 *  @details
 *  @note The Main heartbeat should only be used in a single location.
 *  If it is used in multiple locations, the Sample Graph view in the GUI may not draw correctly.
 *  For tracking more than one heartbeat (such as graphics and audio),
 *  please make make use of the User heartbeats.
 *
 */
enum Heartbeats
{
    Heartbeats_Main = 0x0,  /**< Main is used to determine framerate. */
    Heartbeats_Vsync = 0x1, /**< Used internally, do not use directly. */
    Heartbeats_User1 = 0x8, /**< &nbsp; */
    Heartbeats_User2 = 0x9, /**< &nbsp; */
    Heartbeats_User3 = 0xA, /**< &nbsp; */
    Heartbeats_User4 = 0xB, /**< &nbsp; */
    Heartbeats_User5 = 0xC, /**< &nbsp; */
    Heartbeats_User6 = 0xD, /**< &nbsp; */
    Heartbeats_User7 = 0xE, /**< &nbsp; */
    Heartbeats_User8 = 0xF, /**< &nbsp; */

    Heartbeats_MAX = 0xF
};


/**
 *  @brief A helper class for using constant string literals.
 *
 *  @details
 *  This class is used by the code blocks and user logs in order to allow for
 *  using a string to identify the block or log.
 */
class StringLiteral
{
public:
    template <size_t N>
    NN_IMPLICIT StringLiteral(const char (&str)[N]) NN_NOEXCEPT
        : m_String(str),
          m_Length(N - 1)
    {
    }

    const char* operator&() const NN_NOEXCEPT
    {
        return m_String;
    }

    const char* GetString() const NN_NOEXCEPT
    {
        return m_String;
    }

    size_t GetLength() const NN_NOEXCEPT
    {
        return m_Length;
    }

    NN_IMPLICIT operator const char*() const NN_NOEXCEPT
    {
        return m_String;
    }

private:
    const char* m_String;
    size_t m_Length;

    template <size_t N>
    NN_IMPLICIT StringLiteral(char(&str)[N]) NN_NOEXCEPT
    {
        NN_UNUSED(str);
    }
};


}} // namespace nn::profiler
