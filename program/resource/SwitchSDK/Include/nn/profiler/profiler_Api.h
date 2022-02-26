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
 * @brief Nintendo NX CPU Profiler General functions in the API
 * @details Nintendo NX CPU Profiler API
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/nn_Version.h>

#include <nn/profiler/profiler_Types.h>
#include <nn/profiler/profiler_Result.h>

struct NVNqueue;

namespace nn { namespace profiler {

/**
 *  @brief The minimum buffer size needed by the profiler to function.
 *
 *  @details
 *  Of this space, 1MB is reserved for internal usage while the remaining
 *  space is used for the profile data storage.
 *  If nn::profiler::InstallGraphicsHooks was called, the internal usage
 *  requirement bumps up to 5MB, with the remaining space use for profile
 *  data storage.
 *
 *  @sa Initialize
 */
const size_t MinimumBufferSize = 64 * 1024 * 1024;

namespace detail {

    nn::Result Initialize(void* pBuffer, size_t bufferSize, uint32_t sdkVersion) NN_NOEXCEPT;
}


/**
 *  @brief Initializes the profiler.
 *
 *  @param [in] pBuffer A memory buffer for the profiler to use.
 *  @param [in] bufferSize The size of the memory buffer passed in.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The process completed as expected.}
 *      @handleresult{nn::profiler::ResultNullArgument, One of the arguments was NULL.}
 *      @handleresult{nn::profiler::ResultInvalidArgument, One of the arguments was invalid.}
 *      @handleresult{nn::profiler::ResultMemoryAllocationFailure, There was a problem either
 *          setting up the memory to use or allocating from the memory space.}
 *      @handleresult{nn::profiler::ResultAlreadyDone, The profiler is already initialized.}
 *  @endretresult
 *
 *  @details
 *  This initializes the profiler, setting up and running multiple threads.
 *  There is one thread that control main profiler thread.
 *  Two threads, Send and Receive, for communicating with the PC.
 *  One thread per core that your application runs on.
 *
 *  If it is planned to capture NVN graphics information, be sure to call
 *  nn::profiler::InstallGraphicsHooks before calling this function.
 *
 *  @note
 *  The profiler needs HTCS in order to function correctly.
 *  If HTCS is not already initialized, it will internally call nn::htcs::Initialize().
 *  If your application also uses HTCS, be sure to initialize it before calling the profiler.
 *
 *  @note
 *  When using Graphics Tracing, this function should be called before any NVN queues are created.
 *  If this is not possible, it will be necessary to call
 *  nn::profiler::SetQueuePerFrameOperationEstimates for each queue.
 *
 *  @sa Finalize
 */
inline nn::Result Initialize(void* pBuffer, size_t bufferSize) NN_NOEXCEPT
{
    return detail::Initialize(pBuffer, bufferSize, NN_SDK_CURRENT_VERSION_NUMBER);
}



/**
 *  @brief Shuts down the profiler.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The process completed as expected.}
 *  @endretresult
 *
 *  @details
 *  Shuts down all of the profiler threads and releases the resources allocated.
 *
 *  @note
 *  If the profiler initialized HTCS communication, this will finalize it.
 */
nn::Result Finalize() NN_NOEXCEPT;



/**
 *  @brief Install graphics hooks.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The process completed as expected.}
 *      @handleresult{nn::profiler::ResultInvalidProfilerStatus,
 *          Attempted to install the hooks after nn::profiler::Initialize was called.}
 *  @endretresult
 *
 *  @details
 *  Installs the hooks for adding graphics performance data to a profile session.
 *
 *  This function must be called before the NVN function pointers are loaded
 *  using nvnLoadCProcs or nvnLoadCPPProcs.
 *  Additionally, this function must be called before calling nn::profiler::Initialize.
 *
 *  @note
 *  Make sure nn::profiler::Initialize is called before any queues are created.
 *
 */
nn::Result InstallGraphicsHooks() NN_NOEXCEPT;



/**
 *  @brief Set the estimated max number of draws, debug pushes, and compute operations per frame for this queue.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The process completed as expected.}
 *      @handleresult{nn::profiler::ResultNullArgument, The queue argument was set to null.
 *      @handleresult{nn::profiler::ResultInvalidArgument, One of the arguments was set to an invalid value.}
 *  @endretresult
 *
 *  @param [in] queue The queue for which to set the estimates.
 *  @param [in] numDraws The maximum number draw calls per frame.
 *  @param [in] numDebugPushes The maximum number of debug group pushes per frame.
 *  @param [in] numComputeDispatches The maximum number of compute operations per frame.
 *
 *  @details
 *  Sets up sizes of internal data structure for recording of graphics performance data.
 *
 *  The following are the default values used for each queue:
 *    - @c numDraws : 8000
 *    - @c numDebugPushes : 50
 *    - @c numComputeDispatches : 128
 *
 *  @note
 *  This function is meant to be called once per program run, not once per frame.
 */
nn::Result SetQueuePerFrameOperationEstimates(NVNqueue* queue, int numDraws, int numDebugPushes, int numComputeDispatches) NN_NOEXCEPT;



/**
 *  @brief Gets the current status of the profiler.
 *
 *  @return The profiler's current status.
 *
 *  @details Gets the current status of the profiler.
 */
ProfilerStatus GetProfilerStatus() NN_NOEXCEPT;


//! @name Logging API
//! @{


/**
 *  @brief Records the specified heartbeat into the current core's buffer.
 *
 *  @param [in] heartbeat The heartbeat to record.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The heartbeat was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultInvalidArgument, The heartbeat passed in was invalid.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The heartbeat was not recorded into the buffer.}
 *  @endretresult
 *
 *  @details
 *  This function records a heartbeat in a game or application.
 *  The most common form of heartbeat is the main update loop and has
 *  been provided a preexisting name.
 *  Other heartbeat types are provided as User heartbeat numbers.
 *  The heartbeat will be recorded as long as profiling is active.
 *
 *  The default heartbeat that the profiler GUI looks for is @ref Heartbeats_Main.
 *  This is used to determine frames in the profiler - which will result in a framerate
 *  graph and visual markings showing the frames.
 *  The call to set this up should look like:
 *  @code
 *  nn::profiler::RecordHeartbeat(nn::profiler::Heartbeats_Main);
 *  @endcode
 */
nn::Result RecordHeartbeat(Heartbeats heartbeat) NN_NOEXCEPT;



/**
 *  @brief Records the specified data into the current core's buffer.
 *
 *  @param [in] id A numeric ID used to identify the data.
 *  @param [in] data The 64-bit integer to record.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The data was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *  @endretresult
 *
 *  @details
 *  This function records a single 64-bit integer value into the profile data.
 *  The data is identified on the PC using the provided integer.
 *  The data will only be recorded if profiling is active.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::RecordData(10, 12345uL);
 *  @endcode
 */
nn::Result RecordData(uint64_t id, uint64_t data) NN_NOEXCEPT;



/**
 *  @brief Records the specified data into the current core's buffer.
 *
 *  @param [in] id A string used to identify the data.
 *  @param [in] data The 64-bit integer to record.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The data was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultNullArgument, One of the arguments was NULL.}
 *  @endretresult
 *
 *  @details
 *  This function records a single 64-bit integer value into the profile data.
 *  The data is identified on the PC using the provided string.
 *  The data will only be recorded if profiling is active.
 *
 *  Strings up to 254 bytes may be used; longer strings will be truncated.
 *  The string used must be null-terminated.
 *  An internal buffer stores the strings provided, so dynamically created strings may be used.
 *  If that internal buffer becomes full, the GUI will use the value UINT64_MAX as the ID.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::RecordData("Example Data", 12345uL);
 *  @endcode
 */
nn::Result RecordData(const char* id, uint64_t data) NN_NOEXCEPT;



/**
 *  @brief Records the specified data into the current core's buffer.
 *
 *  @param [in] id A numeric ID used to identify the data.
 *  @param [in] data The 64-bit floating-point to record.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The data was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *  @endretresult
 *
 *  @details
 *  This function records a single 64-bit floating-point value into the profile data.
 *  The data is identified on the PC using the provided integer.
 *  The data will only be recorded if profiling is active.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::RecordData(10, 123.45);
 *  @endcode
 */
nn::Result RecordData(uint64_t id, double data) NN_NOEXCEPT;



/**
 *  @brief Records the specified data into the current core's buffer.
 *
 *  @param [in] id A string used to identify the data.
 *  @param [in] data The 64-bit floating-point to record.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The data was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultNullArgument, One of the arguments was NULL.}
 *  @endretresult
 *
 *  @details
 *  This function records a single 64-bit floating-point value into the profile data.
 *  The data is identified on the PC using the provided string.
 *  The data will only be recorded if profiling is active.
 *
 *  Strings up to 254 bytes may be used; longer strings will be truncated.
 *  The string used must be null-terminated.
 *  An internal buffer stores the strings provided, so dynamically created strings may be used.
 *  If that internal buffer becomes full, the GUI will use the value UINT64_MAX as the ID.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::RecordData("Example Data", 123.45);
 *  @endcode
 */
nn::Result RecordData(const char* id, double data) NN_NOEXCEPT;


//! @}

}} // namespace nn::profiler
