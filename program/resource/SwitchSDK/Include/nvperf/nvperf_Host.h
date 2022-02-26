#ifndef NVPERF_HOST_H
#define NVPERF_HOST_H

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
 *  @file   nvperf_host.h
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
 

// Guard against multiple definition of PerfWorks host types
#ifndef NVPERF_HOST_API_DEFINED
#define NVPERF_HOST_API_DEFINED


/***************************************************************************//**
 *  @name   Host Configuration
 *  @{
 */

    typedef struct NVPA_CounterDataImageCopyOptions
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
    } NVPA_CounterDataImageCopyOptions;
#define NVPA_COUNTER_DATA_IMAGE_COPY_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_CounterDataImageCopyOptions, maxRangeNameLength)

    NVPA_Status NVPA_InitializeHost(void);

    NVPA_Status NVPA_CounterData_CalculateCounterDataImageCopySize(
        const NVPA_CounterDataImageCopyOptions* pCounterDataImageCopyOptions,
        const uint8_t* pCounterDataSrc,
        size_t* pCopyDataImageCounterSize);

    NVPA_Status NVPA_CounterData_InitializeCounterDataImageCopy(
        const NVPA_CounterDataImageCopyOptions* pCounterDataImageCopyOptions,
        const uint8_t* pCounterDataSrc,
        uint8_t* pCounterDataDst);

    typedef struct NVPA_CounterDataCombiner NVPA_CounterDataCombiner;

    typedef struct NVPA_CounterDataCombinerOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// The destination counter data into which the source datas will be combined
        uint8_t* pCounterDataDst;
    } NVPA_CounterDataCombinerOptions;
#define NVPA_COUNTER_DATA_COMBINER_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_CounterDataCombinerOptions, pCounterDataDst)

    NVPA_Status NVPA_CounterDataCombiner_Create(
        const NVPA_CounterDataCombinerOptions* pCounterDataCombinerOptions,
        NVPA_CounterDataCombiner** ppCounterDataCombiner);

    NVPA_Status NVPA_CounterDataCombiner_Destroy(NVPA_CounterDataCombiner* pCounterDataCombiner);

    NVPA_Status NVPA_CounterDataCombiner_CreateRange(
        NVPA_CounterDataCombiner* pCounterDataCombiner,
        size_t numDescriptions,
        const char* const* ppDescriptions,
        size_t* pRangeIndexDst);

    NVPA_Status NVPA_CounterDataCombiner_AccumulateIntoRange(
        NVPA_CounterDataCombiner* pCounterDataCombiner,
        size_t rangeIndexDst,
        uint32_t dstMultiplier,
        const uint8_t* pCounterDataSrc,
        size_t rangeIndexSrc,
        uint32_t srcMultiplier);

/**
 *  @}
 ******************************************************************************/
 
/***************************************************************************//**
 *  @name   Metrics Configuration
 *  @{
 */

    typedef struct NVPA_SupportedChipNames
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out
        const char* const* ppChipNames;
        /// out
        size_t numChipNames;
    } NVPA_SupportedChipNames;
#define NVPA_SUPPORTED_CHIP_NAMES_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_SupportedChipNames, numChipNames)

    typedef struct NVPA_RawMetricsConfig NVPA_RawMetricsConfig;

    typedef struct NVPA_RawMetricsConfigOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        NVPA_ActivityKind activityKind;
        const char* pChipName;
    } NVPA_RawMetricsConfigOptions;
#define NVPA_RAW_METRICS_CONFIG_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_RawMetricsConfigOptions, pChipName)

    typedef struct NVPA_RawMetricsPassGroupOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        size_t maxPassCount;
    } NVPA_RawMetricsPassGroupOptions;
#define NVPA_RAW_METRICS_PASS_GROUP_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_RawMetricsPassGroupOptions, maxPassCount)

    typedef struct NVPA_RawMetricProperties
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out
        const char* pMetricName;
        /// out
        NVPA_Bool supportsPipelined;
        /// out
        NVPA_Bool supportsIsolated;
    } NVPA_RawMetricProperties;
#define NVPA_RAW_METRIC_PROPERTIES_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_RawMetricProperties, supportsIsolated)

    typedef struct NVPA_RawMetricRequest
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// in
        const char* pMetricName;
        /// in
        NVPA_Bool isolated;
        /// in; ignored by AddMetric but observed by CounterData initialization
        NVPA_Bool keepInstances;
    } NVPA_RawMetricRequest;
#define NVPA_RAW_METRIC_REQUEST_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_RawMetricRequest, keepInstances)

    NVPA_Status NVPA_GetSupportedChipNames(NVPA_SupportedChipNames* pSupportedChipNames);

    NVPA_Status NVPA_RawMetricsConfig_Create(
        const NVPA_RawMetricsConfigOptions* pMetricsConfigOptions,
        NVPA_RawMetricsConfig** ppRawMetricsConfig);

    NVPA_Status NVPA_RawMetricsConfig_Destroy(NVPA_RawMetricsConfig* pRawMetricsConfig);

    NVPA_Status NVPA_RawMetricsConfig_BeginPassGroup(
        NVPA_RawMetricsConfig* pRawMetricsConfig,
        const NVPA_RawMetricsPassGroupOptions* pRawMetricsPassGroupOptions);

    NVPA_Status NVPA_RawMetricsConfig_EndPassGroup(NVPA_RawMetricsConfig* pRawMetricsConfig);

    NVPA_Status NVPA_RawMetricsConfig_GetNumMetrics(
        const NVPA_RawMetricsConfig* pRawMetricsConfig,
        size_t* pNumMetrics);

    NVPA_Status NVPA_RawMetricsConfig_GetMetricProperties(
        const NVPA_RawMetricsConfig* pRawMetricsConfig,
        size_t metricIndex,
        NVPA_RawMetricProperties* pRawMetricProperties);

    NVPA_Status NVPA_RawMetricsConfig_GetMetricNameFromCounterId(
        const NVPA_RawMetricsConfig* pRawMetricsConfig,
        uint64_t counterId,
        const char** pMetricName);

    NVPA_Status NVPA_RawMetricsConfig_AddMetrics(
        NVPA_RawMetricsConfig* pRawMetricsConfig,
        const NVPA_RawMetricRequest* pRawMetricRequests,
        size_t numMetricRequests);

    NVPA_Status NVPA_RawMetricsConfig_IsAddMetricsPossible(
        const NVPA_RawMetricsConfig* pRawMetricsConfig,
        const NVPA_RawMetricRequest* pRawMetricRequests,
        size_t numMetricRequests,
        NVPA_Bool* pIsPossible);

    NVPA_Status NVPA_RawMetricsConfig_GenerateConfigImage(NVPA_RawMetricsConfig* pRawMetricsConfig);

    NVPA_Status NVPA_RawMetricsConfig_GetConfigImage(
        const NVPA_RawMetricsConfig* pRawMetricsConfig,
        size_t bufferSize,
        uint8_t* pBuffer,
        size_t* pBufferSize);

    /// Total num passes = *pNumPipelinedPasses + *pNumIsolatedPasses * numNestingLevels
    NVPA_Status NVPA_RawMetricsConfig_GetNumPasses(
        const NVPA_RawMetricsConfig* pRawMetricsConfig,
        size_t* pNumPipelinedPasses,
        size_t* pNumIsolatedPasses);

/**
 *  @}
 ******************************************************************************/
 
/***************************************************************************//**
 *  @name   CounterData Creation
 *  @{
 */

    typedef struct NVPA_CounterDataBuilder NVPA_CounterDataBuilder;

    typedef struct NVPA_CounterDataBuilderOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        const char* pChipName;
    } NVPA_CounterDataBuilderOptions;
#define NVPA_COUNTER_DATA_BUILDER_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_CounterDataBuilderOptions, pChipName)

    NVPA_Status NVPA_CounterDataBuilder_Create(
        const NVPA_CounterDataBuilderOptions* pOptions,
        NVPA_CounterDataBuilder** ppCounterDataBuilder);

    NVPA_Status NVPA_CounterDataBuilder_Destroy(NVPA_CounterDataBuilder* pCounterDataBuilder);

    NVPA_Status NVPA_CounterDataBuilder_AddMetrics(
        NVPA_CounterDataBuilder* pCounterDataBuilder,
        const NVPA_RawMetricRequest* pRawMetricRequests,
        size_t numMetricRequests);

    NVPA_Status NVPA_CounterDataBuilder_GetCounterDataPrefix(
        NVPA_CounterDataBuilder* pCounterDataBuilder,
        size_t bufferSize,
        uint8_t* pBuffer,
        size_t* pBufferSize);

/**
 *  @}
 ******************************************************************************/
 
/***************************************************************************//**
 *  @name   MetricsContext - metric configuration and evaluation
 *  @{
 */

    typedef struct NVPA_MetricsContext NVPA_MetricsContext;

    typedef struct NVPA_MetricsContextOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        const char* pChipName;
    } NVPA_MetricsContextOptions;
#define NVPA_METRICS_CONTEXT_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_MetricsContextOptions, pChipName)

    typedef struct NVPA_MetricsScriptOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// in : if true, upon error, calls PyErr_Print() which causes exceptions to be logged to stderr
        NVPA_Bool printErrors;
        /// in : the script source code
        const char* pSource;
        /// in : the filename reported in stack traces; if NULL, uses an auto-generated name
        const char* pFileName;
    } NVPA_MetricsScriptOptions;
#define NVPA_METRICS_SCRIPT_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_MetricsScriptOptions, pFileName)

    typedef struct NVPA_MetricsExecOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// in : if true, treats pSource as a statement to be eval'd; otherwise, calls exec.
        NVPA_Bool isStatement;
        /// in : if true, upon error, calls PyErr_Print() which causes exceptions to be logged to stderr
        NVPA_Bool printErrors;
        /// in : the script source code
        const char* pSource;
        /// in : the filename reported in stack traces; if NULL, uses an auto-generated name
        const char* pFileName;
        /// out: if isStatement, points at a string form of the evaluation; if !isStatement, points at
        /// str(locals()['result'])
        const char* pResultStr;
    } NVPA_MetricsExecOptions;
#define NVPA_METRICS_EXEC_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_MetricsExecOptions, pResultStr)

    typedef struct NVPA_MetricsEnumerationOptions
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out: number of elements in array ppMetricNames
        size_t numMetrics;
        /// out: pointer to array of 'const char* pMetricName'
        const char* const* ppMetricNames;
        /// in : if true, doesn't enumerate \<metric\>.peak_{burst, sustained}
        NVPA_Bool hidePeakSubMetrics;
        /// in : if true, doesn't enumerate \<metric\>.per_{active,elapsed,region,frame}_cycle
        NVPA_Bool hidePerCycleSubMetrics;
        /// in : if true, doesn't enumerate \<metric\>.pct_of_peak_{burst,sustained}_{active,elapsed,region,frame}
        NVPA_Bool hidePctOfPeakSubMetrics;
    } NVPA_MetricsEnumerationOptions;
#define NVPA_METRICS_ENUMERATION_OPTIONS_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_MetricsEnumerationOptions, hidePctOfPeakSubMetrics)

    typedef struct NVPA_MetricProperties
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// out
        const char* pDescription;
        /// out
        const char* pDimUnits;
        /// out: a NULL-terminated array of pointers to RawMetric names that can be passed to
        /// NVPA_RawMetricsConfig_AddMetrics()
        const char** ppRawMetricDependencies;
        /// out: metric.peak_burst.value.gpu
        double gpuBurstRate;
        /// out: metric.peak_sustained.value.gpu
        double gpuSustainedRate;
    } NVPA_MetricProperties;
#define NVPA_METRIC_PROPERTIES_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_MetricProperties, gpuSustainedRate)

    typedef struct NVPA_MetricUserData
    {
        /// [in]
        size_t structSize;
        /// [in] assign to NULL
        void* pPriv;
        /// in: duration in ns of user defined frame
        double frame_duration;
        /// in: duration in ns of user defined region
        double region_duration;
    } NVPA_MetricUserData;
#define NVPA_METRIC_USER_DATA_STRUCT_SIZE NVPA_STRUCT_SIZE(NVPA_MetricUserData, region_duration)

    typedef enum NVPA_MetricDetailLevel
    {
        NVPA_METRIC_DETAIL_LEVEL_INVALID,
        NVPA_METRIC_DETAIL_LEVEL_GPU,
        NVPA_METRIC_DETAIL_LEVEL_ALL,
        NVPA_METRIC_DETAIL_LEVEL_GPU_AND_LEAF_INSTANCES,
        NVPA_METRIC_DETAIL_LEVEL__COUNT
    } NVPA_MetricDetailLevel;

    NVPA_Status NVPA_MetricsContext_Create(
        const NVPA_MetricsContextOptions* pMetricsContextOptions,
        NVPA_MetricsContext** ppMetricsContext);

    NVPA_Status NVPA_MetricsContext_Destroy(NVPA_MetricsContext* pMetricsContext);

    /// Runs code in the metrics module.  Additional metrics can be added through this interface.
    /// If printErrors is true, calls PyErr_Print() which causes exceptions to be logged to stderr.
    /// Equivalent to:
    ///      exec(source, metrics.__dict__, metrics.__dict__)
    NVPA_Status NVPA_MetricsContext_RunScript(
        NVPA_MetricsContext* pMetricsContext,
        const NVPA_MetricsScriptOptions* pOptions);

    /// Executes a script in the metrics module, but does not modify its contents (for ordinary queries).
    /// Equivalent to one of:
    ///      eval(source, metrics.__dict__, {})            # isStatement true
    ///      exec(source, metrics.__dict__, {})            # isStatement false
    NVPA_Status NVPA_MetricsContext_ExecScript_Begin(
        NVPA_MetricsContext* pMetricsContext,
        NVPA_MetricsExecOptions* pOptions);

    /// Cleans up memory internally allocated by NVPA_MetricsContext_ExecScript_Begin.
    NVPA_Status NVPA_MetricsContext_ExecScript_End(NVPA_MetricsContext* pMetricsContext);

    /// Outputs (size, pointer) to an array of "const char* pCounterName".  The lifetime of the array is tied to
    /// MetricsContext.  The names are sorted.
    /// Impl: lazily creates list
    NVPA_Status NVPA_MetricsContext_GetCounterNames_Begin(
        NVPA_MetricsContext* pMetricsContext,
        size_t* pNumCounters,
        const char* const** pppCounterNames);

    /// Cleans up memory internally allocated by NVPA_MetricsContext_GetCounterNames_Begin.
    NVPA_Status NVPA_MetricsContext_GetCounterNames_End(NVPA_MetricsContext* pMetricsContext);

    /// Outputs (size, pointer) to an array of "const char* pThroughputName".  The lifetime of the array is tied to
    /// MetricsContext.  The names are sorted.
    /// Impl: lazily creates list
    NVPA_Status NVPA_MetricsContext_GetThroughputNames_Begin(
        NVPA_MetricsContext* pMetricsContext,
        size_t* pNumThroughputs,
        const char* const** pppThroughputName);

    /// Cleans up memory internally allocated by NVPA_MetricsContext_GetThroughputNames_Begin.
    NVPA_Status NVPA_MetricsContext_GetThroughputNames_End(NVPA_MetricsContext* pMetricsContext);

    /// Outputs (size, pointer) to an array of "const char* pMetricName".  The lifetime of the array is tied to
    /// MetricsContext.  The names are sorted.
    /// Enumerates all metrics at all levels.  Includes:
    ///  *   counter.{sum,avg,min,max}
    ///  *   throughput.{avg,min,max}
    ///  *   \<metric\>.peak_{burst, sustained}
    ///  *   \<metric\>.per_{active,elapsed,region,frame}_cycle
    ///  *   \<metric\>.pct_of_peak_{burst,sustained}_{active,elapsed,region,frame}
    ///  *   \<metric\>.per.{other, other_pct}
    NVPA_Status NVPA_MetricsContext_GetMetricNames_Begin(
        NVPA_MetricsContext* pMetricsContext,
        NVPA_MetricsEnumerationOptions* pOptions);

    /// Cleans up memory internally allocated by NVPA_MetricsContext_GetMetricNames_Begin.
    NVPA_Status NVPA_MetricsContext_GetMetricNames_End(NVPA_MetricsContext* pMetricsContext);

    /// After this function returns, the lifetimes of strings pointed to by {ppCounterNames, ppSubThroughputNames,
    /// ppSubMetricNames} are guaranteed until NVPA_MetricsContext_GetThroughputBreakdown_End, or until pMetricsContext
    /// is destroyed
    NVPA_Status NVPA_MetricsContext_GetThroughputBreakdown_Begin(
        NVPA_MetricsContext* pMetricsContext,
        const char* pThroughputName,
        const char* const** pppCounterNames,
        const char* const** pppSubThroughputNames);

    /// Cleans up memory internally allocated by NVPA_MetricsContext_GetThroughputBreakdown_Begin.
    NVPA_Status NVPA_MetricsContext_GetThroughputBreakdown_End(NVPA_MetricsContext* pMetricsContext);

    /// After this function returns, the lifetimes of strings pointed to by pMetricProperties are guaranteed until
    /// NVPA_MetricsContext_GetMetricProperties_End, or until pMetricsContext is destroyed.
    NVPA_Status NVPA_MetricsContext_GetMetricProperties_Begin(
        NVPA_MetricsContext* pMetricsContext,
        const char* pMetricName,
        NVPA_MetricProperties* pMetricProperties);

    /// Cleans up memory internally allocated by NVPA_MetricsContext_GetMetricProperties_Begin.
    NVPA_Status NVPA_MetricsContext_GetMetricProperties_End(NVPA_MetricsContext* pMetricsContext);

    /// Sets data for subsequent evaluation calls.
    /// Only one (CounterData, range, isolated) set of counters can be active at a time; subsequent calls will overwrite
    /// previous calls' data.
    NVPA_Status NVPA_MetricsContext_SetCounterData(
        NVPA_MetricsContext* pMetricsContext,
        const uint8_t* pCounterDataImage,
        size_t rangeIndex,
        NVPA_Bool isolated);

    /// Sets user data for subsequent evaluation calls.
    NVPA_Status NVPA_MetricsContext_SetUserData(
        NVPA_MetricsContext* pMetricsContext,
        const NVPA_MetricUserData* pMetricUserData);

    /// Evaluate multiple metrics to retrieve their GPU values.
    NVPA_Status NVPA_MetricsContext_EvaluateToGpuValues(
        NVPA_MetricsContext* pMetricsContext,
        size_t numMetrics,
        const char* const* ppMetricNames,
        double* pMetricValues);

    /// Evaluate a single metric, retrieving instance values from the requested unit level.
    /// If a metric was scheduled with keepInstances=true, then instance values can be queried at that metric's unit
    /// level and all of its parents.
    NVPA_Status NVPA_MetricsContext_EvaluateToInstanceValues(
        NVPA_MetricsContext* pMetricsContext,
        const char* pMetricName,
        const char* pUnitName,
        size_t numInstances,
        double* pInstanceValues,
        size_t* pNumInstances);

/**
 *  @}
 ******************************************************************************/
 

#endif // NVPERF_HOST_API_DEFINED




#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__GNUC__) && defined(NVPA_SHARED_LIB)
    #pragma GCC visibility pop
#endif

#endif // NVPERF_HOST_H
