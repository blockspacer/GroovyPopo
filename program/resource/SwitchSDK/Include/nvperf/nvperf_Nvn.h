#ifndef NVPERF_NVN_H
#define NVPERF_NVN_H

/*
 * Copyright 2014-2018  NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and
 * international Copyright laws.
 *
 * This software and the information contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and conditions
 * of a form of NVIDIA software license agreement.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS,  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION,  ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOURCE CODE.
 *
 * U.S. Government End Users.   This source code is a "commercial item" as
 * that term is defined at  48 C.F.R. 2.101 (OCT 1995), consisting  of
 * "commercial computer  software"  and "commercial computer software
 * documentation" as such terms are  used in 48 C.F.R. 12.212 (SEPT 1995)
 * and is provided to the U.S. Government only as a commercial end item.
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the
 * source code with only those rights set forth herein.
 *
 * Any use of this source code in individual and commercial software must
 * include, in the user documentation and internal comments to the code,
 * the above Disclaimer and U.S. Government End Users Notice.
 */

#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__) && defined(NVPA_SHARED_LIB)
    #pragma GCC visibility push(default)
    #if !defined(NVPW_LOCAL)
        #define NVPW_LOCAL __attribute__ ((visibility ("hidden")))
    #endif
#else
    #if !defined(NVPW_LOCAL)
        #define NVPW_LOCAL
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @file   nvperf_nvn.h
 */

/***************************************************************************//**
 *  @name   Common Types
 *  @{
 */

#ifndef NVPERF_NVPA_STATUS_DEFINED
#define NVPERF_NVPA_STATUS_DEFINED

    /// Error codes.
    typedef enum NVPA_Status
    {
        /// Success
        NVPA_STATUS_SUCCESS = 0,
        /// Generic error.
        NVPA_STATUS_ERROR = 1,
        /// Internal error.  Please file a bug!
        NVPA_STATUS_INTERNAL_ERROR = 2,
        /// NVPA_Init() has not been called yet.
        NVPA_STATUS_NOT_INITIALIZED = 3,
        /// The NvPerfAPI DLL/DSO could not be loaded during init.
        NVPA_STATUS_NOT_LOADED = 4,
        /// The function was not found in this version of the NvPerfAPI DLL/DSO.
        NVPA_STATUS_FUNCTION_NOT_FOUND = 5,
        /// The request is intentionally not supported by NvPerfAPI.
        NVPA_STATUS_NOT_SUPPORTED = 6,
        /// The request is not implemented by this version of NvPerfAPI.
        NVPA_STATUS_NOT_IMPLEMENTED = 7,
        /// Invalid argument.
        NVPA_STATUS_INVALID_ARGUMENT = 8,
        /// A MetricId argument does not belong to the specified NVPA_Activity or NVPA_Config.
        NVPA_STATUS_INVALID_METRIC_ID = 9,
        /// No driver has been loaded via NVPA_*_LoadDriver().
        NVPA_STATUS_DRIVER_NOT_LOADED = 10,
        /// Failed memory allocation.
        NVPA_STATUS_OUT_OF_MEMORY = 11,
        /// The request could not be fulfilled due to the state of the current thread.
        NVPA_STATUS_INVALID_THREAD_STATE = 12,
        /// Allocation of context object failed.
        NVPA_STATUS_FAILED_CONTEXT_ALLOC = 13,
        /// The specified GPU is not supported.
        NVPA_STATUS_UNSUPPORTED_GPU = 14,
        /// The installed NVIDIA driver is too old.
        NVPA_STATUS_INSUFFICIENT_DRIVER_VERSION = 15,
        /// Graphics object has not been registered via NVPA_Register*().
        NVPA_STATUS_OBJECT_NOT_REGISTERED = 16,
        /// The operation failed due to a security check.
        NVPA_STATUS_INSUFFICIENT_PRIVILEGE = 17,
        /// The request could not be fulfilled due to the state of the context.
        NVPA_STATUS_INVALID_CONTEXT_STATE = 18,
        /// The request could not be fulfilled due to the state of the object.
        NVPA_STATUS_INVALID_OBJECT_STATE = 19,
        /// The request could not be fulfilled because a system resource is already in use.
        NVPA_STATUS_RESOURCE_UNAVAILABLE = 20,
        /// The NVPA_*_LoadDriver() is called after the context, command queue or device is created.
        NVPA_STATUS_DRIVER_LOADED_TOO_LATE = 21,
        /// The provided buffer is not large enough.
        NVPA_STATUS_INSUFFICIENT_SPACE = 22,
        /// The API object passed to NVPA_[API]_BeginPass/NVPA_[API]_EndPass and
        /// NVPA_[API]_PushRange/NVPA_[API]_PopRange does not match with the NVPA_[API]_BeginSession.
        NVPA_STATUS_OBJECT_MISMATCH = 23,
        NVPA_STATUS__COUNT
    } NVPA_Status;


#endif // NVPERF_NVPA_STATUS_DEFINED


#ifndef NVPERF_NVPA_ACTIVITY_KIND_DEFINED
#define NVPERF_NVPA_ACTIVITY_KIND_DEFINED

    /// The configuration's activity-kind dictates which types of data may be collected.
    typedef enum NVPA_ActivityKind
    {
        /// Invalid value.
        NVPA_ACTIVITY_KIND_INVALID = 0,
        /// A workload-centric activity for serialized and pipelined collection.
        /// 
        /// Profiler is capable of collecting both serialized and pipelined metrics.  The library introduces any
        /// synchronization required to collect serialized metrics.
        NVPA_ACTIVITY_KIND_PROFILER,
        /// A realtime activity for sampling counters from the CPU or GPU.
        NVPA_ACTIVITY_KIND_REALTIME_SAMPLED,
        NVPA_ACTIVITY_KIND__COUNT
    } NVPA_ActivityKind;


#endif // NVPERF_NVPA_ACTIVITY_KIND_DEFINED


#ifndef NVPERF_NVPA_BOOL_DEFINED
#define NVPERF_NVPA_BOOL_DEFINED
    /// The type used for boolean values.
    typedef uint8_t NVPA_Bool;
#endif // NVPERF_NVPA_BOOL_DEFINED

#ifndef NVPA_STRUCT_SIZE
#define NVPA_STRUCT_SIZE(type_, lastfield_)                     (offsetof(type_, lastfield_) + sizeof(((type_*)0)->lastfield_))
#endif // NVPA_STRUCT_SIZE


#ifndef NVPERF_NVPA_GETPROCADDRESS_DEFINED
#define NVPERF_NVPA_GETPROCADDRESS_DEFINED

typedef NVPA_Status (*NVPA_GenericFn)(void);


    /// 
    /// Gets the address of a PerfWorks API function.
    /// 
    /// \return A function pointer to the function, or NULL if the function is not available.
    /// 
    /// \param pFunctionName [in] Name of the function to retrieve.
    NVPA_GenericFn NVPA_GetProcAddress(const char* pFunctionName);

#endif

#ifndef NVPERF_NVPW_SETLIBRARYLOADPATHS_DEFINED
#define NVPERF_NVPW_SETLIBRARYLOADPATHS_DEFINED


#endif

/**
 *  @}
 ******************************************************************************/
 
/***************************************************************************//**
 *  @name   External Types
 *  @{
 */


    struct NVNqueue;
    struct NVNmemoryPool;


/**
 *  @}
 ******************************************************************************/
 
    NVPA_Status NVPA_InitializeTarget(void);


    // Device enumeration functions must be preceded by NVPA_<API>_LoadDriver(); any API is fine.


    NVPA_Status NVPA_GetDeviceCount(size_t* pNumDevices);

    NVPA_Status NVPA_Device_GetNames(
        size_t deviceIndex,
        const char** ppDeviceName,
        const char** ppChipName);

    NVPA_Status NVPA_CounterData_GetNumRanges(
        const uint8_t* pCounterDataImage,
        size_t* pNumRanges);

    NVPA_Status NVPA_CounterData_GetRangeDescriptions(
        const uint8_t* pCounterDataImage,
        size_t rangeIndex,
        size_t numDescriptions,
        const char** ppDescriptions,
        size_t* pNumDescriptions);


#define NVPA_NVN_MIN_NUM_TRACE_BUFFERS                       2
#define NVPA_NVN_TRACE_BUFFER_PAD_SIZE              0x00010000
#define NVPA_NVN_TRACE_RECORD_SIZE                  0x00000020
#define NVPA_NVN_PERFMON_RECORD_SIZE                0x00000020
#define NVPA_NVN_COMPUTE_BUFFER_PAD_SIZE            0x00000020
#define NVPA_NVN_COMPUTE_RECORD_SIZE                0x00000010

struct NVNqueue;
struct NVNcommandBuffer;


    typedef struct NVPA_NVNC_DecodeCountersOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// in : Config created by e.g.    nvperf2 configure   or   NVPA_RawMetricsConfig_GetConfigImage().  Must be
        /// align(8).
        const uint8_t* pConfig;
        size_t configSize;
        /// in : CounterDataImage, initialized by NVPA_NVNC_InitializeCounterDataImage()
        uint8_t* pCounterDataImage;
        size_t counterDataImageSize;
        /// in :
        uint8_t* pCounterDataScratchBuffer;
        /// in : as returned by NVPA_NVNC_CalculateCounterDataScratchBufferSize()
        size_t counterDataScratchBufferSize;
    } NVPA_NVNC_DecodeCountersOptions;
#define NVPA_NVNC_DECODE_COUNTERS_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_DecodeCountersOptions, counterDataScratchBufferSize)

    typedef struct NVPA_NVNC_DecodeCountersStats
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out: number of bytes consumed from perfmon buffer
        size_t numPerfmonBytesConsumed;
        /// out: number of bytes not written to trace buffer due to buffer full
        size_t numTraceBytesDropped;
        /// out: number of bytes not written to compute buffer due to buffer full
        size_t numComputeBytesDropped;
        /// out: true if the current call to DecodeCounters was able to decode a pass (Note: DecodeCounters() can return
        /// SUCCESS when no passes are decoded)
        NVPA_Bool onePassDecoded;
        /// out: true if all passes for iteration have been decoded and ready to be evaluated
        NVPA_Bool allPassesDecoded;
    } NVPA_NVNC_DecodeCountersStats;
#define NVPA_NVNC_DECODE_COUNTERS_STATS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_DecodeCountersStats, allPassesDecoded)

    typedef struct NVPA_NVNC_QueueDebugStats
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out: the traceBufferIndex that will be targeted by the next BeginPass()
        size_t traceBufferIndexWrite;
        /// out: the traceBufferIndex that will be consumed by the next DecodeCounters()
        size_t traceBufferIndexRead;
        /// out: number of unread trace buffers (unread or in-flight profiler passes)
        size_t numTraceBuffersUnread;
        /// out: number of bytes read back by DecodeCounters; will be returned to hardware FIFO in BeginPass
        size_t numPerfmonBytesUnacknowledged;
    } NVPA_NVNC_QueueDebugStats;
#define NVPA_NVNC_QUEUE_DEBUG_STATS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_QueueDebugStats, numPerfmonBytesUnacknowledged)

    typedef struct NVPA_NVNC_TraceBufferDebugStats
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out: boolean; zero if pipelined counters collected, 1 if isolated counters collected
        uint32_t isIsolatedPass;
        /// out: passIndex in the configuration
        uint32_t passIndex;
        /// out: targetNestingLevel collected
        uint32_t targetNestingLevel;
        /// out: the hardware "PUT" offset at the end of the pass
        uint32_t perfmonBufferPutOffset;
    } NVPA_NVNC_TraceBufferDebugStats;
#define NVPA_NVNC_TRACE_BUFFER_DEBUG_STATS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_TraceBufferDebugStats, perfmonBufferPutOffset)

    typedef struct NVPA_NVNC_SessionOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        NVPA_ActivityKind activityKind;
        /// in : Config created by e.g.    nvperf2 configure   or   NVPA_RawMetricsConfig_GetConfigImage().  Must be
        /// align(8).
        const uint8_t* pConfig;
        size_t configSize;
        /// in : number of TraceBuffers; must be > 0
        size_t numTraceBuffers;
        /// in : size of TraceBuffer for one pass
        size_t traceBufferSize;
        /// in : traceArenaSize = numTraceBuffers * traceBufferSize;
        uint8_t* pTraceArena;
        /// in : NVNbufferAddress of the TraceArena
        uint64_t traceArenaGpuAddress;
        struct NVNmemoryPool* pTraceArenaMemoryPool;
        /// in : size of ComputeBuffer for one pass
        size_t computeBufferSize;
        /// in : computeArenaSize = numTraceBuffers * computeBufferSize;
        uint8_t* pComputeArena;
        /// in : NVNbufferAddress of the ComputeBuffer
        uint64_t computeArenaGpuAddress;
        struct NVNmemoryPool* pComputeArenaMemoryPool;
        /// in : size of PerfmonBuffer
        size_t perfmonBufferSize;
        /// (NX) in: pointer to page-aligned PerfmonBuffer
        /// (Windows) out: pointer to OS-allocated PerfmonBuffer (freed at EndSession)
        uint8_t* pPerfmonBuffer;
        /// in : if true, EndPass calls nvnQueueFinish automatically
        NVPA_Bool finishOnEndPass;
        /// in : the lowest nesting level to be profiled; must be >= 1
        uint16_t minNestingLevel;
        /// in : the number of nesting levels to profile; must be >= 1
        uint16_t numNestingLevels;
        /// in : if false, nvnCommandBufferPushDebugGroup* & nvnCommandBufferPopDebugGroup* denote profiling boundaries;
        /// if true, they are ignored
        NVPA_Bool disableDebugGroups;
        /// in : if false, NVPA_NVNC_PushRange* and NVPA_NVNC_PopRange denote profiling boundaries; if true, they are
        /// ignored
        NVPA_Bool disableRangeGroups;
    } NVPA_NVNC_SessionOptions;
#define NVPA_NVNC_SESSION_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_SessionOptions, disableRangeGroups)

    typedef struct NVPA_NVNC_CounterDataImageOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// The CounterDataPrefix generated from e.g.    nvperf2 initdata   or
        /// NVPA_CounterDataBuilder_GetCounterDataPrefix().  Must be align(8).
        const uint8_t* pCounterDataPrefix;
        size_t counterDataPrefixSize;
        /// max number of ranges that can be profiled
        uint32_t maxNumRanges;
        /// max number of RangeTree nodes; must be >= maxNumRanges
        uint32_t maxNumRangeTreeNodes;
        /// max string length of each RangeName, including the trailing NUL character
        uint32_t maxRangeNameLength;
    } NVPA_NVNC_CounterDataImageOptions;
#define NVPA_NVNC_COUNTER_DATA_IMAGE_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_CounterDataImageOptions, maxRangeNameLength)

    typedef struct NVPA_NVNC_UnpackRawMetricsOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// in
        const uint8_t* pCounterDataImage;
        /// in
        size_t rangeIndex;
        /// in : if true, query isolated metric values
        NVPA_Bool isolated;
        /// in
        size_t numRawMetrics;
        /// in : sorted array of rawMetricId
        const uint64_t* pRawMetricIds;
        /// out
        double* pRawMetricValues;
        /// out
        uint16_t* pHwUnitCounts;
    } NVPA_NVNC_UnpackRawMetricsOptions;
#define NVPA_NVNC_UNPACK_RAW_METRICS_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_NVNC_UnpackRawMetricsOptions, pHwUnitCounts)

    NVPA_Status NVPA_NVNC_LoadDriver(void);

    NVPA_Status NVPA_NVNC_BeginSession(
        struct NVNqueue* queue,
        NVPA_NVNC_SessionOptions* pSessionOptions);

    NVPA_Status NVPA_NVNC_EndSession(struct NVNqueue* queue);

    NVPA_Status NVPA_NVNC_BeginPass(struct NVNqueue* queue);

    NVPA_Status NVPA_NVNC_EndPass(
        struct NVNqueue* queue,
        NVPA_Bool* pAllPassesSubmitted);

    NVPA_Status NVPA_NVNC_DecodeCounters(
        const NVPA_NVNC_DecodeCountersOptions* pDecodeOptions,
        NVPA_NVNC_DecodeCountersStats* pDecodeStats);

    NVPA_Status NVPA_NVNC_CalculateCounterDataImageSize(
        const NVPA_NVNC_CounterDataImageOptions* pCounterDataImageOptions,
        size_t* pCounterDataImageSize);

    NVPA_Status NVPA_NVNC_InitializeCounterDataImage(
        const NVPA_NVNC_CounterDataImageOptions* pCounterDataImageOptions,
        size_t counterDataImageSize,
        uint8_t* pCounterDataImage);

    NVPA_Status NVPA_NVNC_CalculateCounterDataScratchBufferSize(
        const uint8_t* pCounterDataImage,
        size_t* pCounterDataScratchBufferSize);

    NVPA_Status NVPA_NVNC_InitializeCounterDataScratchBuffer(
        const uint8_t* pCounterDataImage,
        size_t counterDataScratchBufferSize,
        uint8_t* pCounterDataScratchBuffer);

    NVPA_Status NVPA_NVNC_CounterData_UnpackRawMetrics(const NVPA_NVNC_UnpackRawMetricsOptions* pUnpackOptions);

    /// Total num passes = *pNumPipelinedPasses + *pNumIsolatedPasses * numNestingLevels
    NVPA_Status NVPA_NVNC_Config_GetNumPasses(
        const uint8_t* pConfig,
        size_t* pNumPipelinedPasses,
        size_t* pNumIsolatedPasses);

    /// Fills in pDebugStats with the current Queue state.
    NVPA_Status NVPA_NVNC_QueueGetDebugStats(
        struct NVNqueue* queue,
        NVPA_NVNC_QueueDebugStats* pDebugStats);

    /// Fills in pDebugStats for the requested traceBufferIndex.
    NVPA_Status NVPA_NVNC_GetTraceBufferDebugStats(
        size_t traceBufferIndex,
        NVPA_NVNC_TraceBufferDebugStats* pDebugStats);

    /// This function controls whether GPU registers are written from the CPU or GPU in BeginPass and EndPass.
    ///  -   GPU writes are asynchronously performed as GPU commands.
    ///  -   CPU writes cause BeginPass and EndPass to first call nvnQueueFinish().
    /// By default, GPU writes are used (cpuRegisterAccesses = false).
    /// To take effect, this function must be called before NVPA_NVNC_BeginSession().
    NVPA_Status NVPA_NVNC_EnablePerfmonRegisterAccessesFromCpu(NVPA_Bool cpuRegisterAccesses);

    /// Equivalent functionality to nvnCommandBufferPushDebugGroupStatic, but controlled by
    /// NVPA_NVNC_SessionOptions::disableRangeGroups.
    NVPA_Status NVPA_NVNC_CommandBufferPushRangeStatic(
        struct NVNcommandBuffer* cmdBuf,
        uint32_t domainId,
        const char* description);

    /// Equivalent functionality to nvnCommandBufferPushDebugGroupDynamic, but controlled by
    /// NVPA_NVNC_SessionOptions::disableRangeGroups.
    NVPA_Status NVPA_NVNC_CommandBufferPushRangeDynamic(
        struct NVNcommandBuffer* cmdBuf,
        uint32_t domainId,
        const char* description);

    /// Equivalent functionality to nvnCommandBufferPopDebugGroupId, but controlled by
    /// NVPA_NVNC_SessionOptions::disableRangeGroups.
    NVPA_Status NVPA_NVNC_CommandBufferPopRange(
        struct NVNcommandBuffer* cmdBuf,
        uint32_t domainId);

    /// Must be called on an NVNqueue before any Range command is submitted to that queue.
    /// Submitting a Range command to a queue before this function is called will result in undefined behavior.
    /// Range commands are generated by NVPA_NVNC_CommandBuffer*Range*().
    /// This function may not be called between NVPA_NVNC_BeginPass and NVPA_NVNC_EndPass.
    NVPA_Status NVPA_NVNC_QueueInitializeRangeCommands(struct NVNqueue* queue);

    /// Equivalent to NVPA_NVNC_CommandBufferPushRangeStatic, but on a Queue.
    NVPA_Status NVPA_NVNC_QueuePushRangeStatic(
        struct NVNqueue* queue,
        uint32_t domainId,
        const char* description);

    /// Equivalent to NVPA_NVNC_CommandBufferPushRangeDynamic, but on a Queue.
    NVPA_Status NVPA_NVNC_QueuePushRangeDynamic(
        struct NVNqueue* queue,
        uint32_t domainId,
        const char* description);

    /// Equivalent to NVPA_NVNC_CommandBufferPopRange, but on a Queue.
    NVPA_Status NVPA_NVNC_QueuePopRange(
        struct NVNqueue* queue,
        uint32_t domainId);



#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__GNUC__) && defined(NVPA_SHARED_LIB)
    #pragma GCC visibility pop
#endif

#endif // NVPERF_NVN_H
