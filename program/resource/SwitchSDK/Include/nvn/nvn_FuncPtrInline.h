
/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvn_FuncPtrInline.h
///      Implementation of the NVN C interface entry points as inline
///      functions that call through global function pointer variables.

#ifndef __nvn_funcptrinline_h_
#define __nvn_funcptrinline_h_

#include "nvn.h"
#include "nvn_FuncPtr.h"

#if defined(__cplusplus) || defined(__STDC__)
#define NVNINLINE    inline
#else
#define NVNINLINE
#endif

#ifdef __cplusplus
extern "C" {
#endif

static NVNINLINE void nvnDeviceBuilderSetDefaults(NVNdeviceBuilder *builder)
{
    pfnc_nvnDeviceBuilderSetDefaults(builder);
}

static NVNINLINE void nvnDeviceBuilderSetFlags(NVNdeviceBuilder *builder, int flags)
{
    pfnc_nvnDeviceBuilderSetFlags(builder, flags);
}

static NVNINLINE NVNdeviceFlagBits nvnDeviceBuilderGetFlags(const NVNdeviceBuilder *builder)
{
    return pfnc_nvnDeviceBuilderGetFlags(builder);
}

static NVNINLINE NVNboolean nvnDeviceInitialize(NVNdevice *device, const NVNdeviceBuilder *builder)
{
    return pfnc_nvnDeviceInitialize(device, builder);
}

static NVNINLINE void nvnDeviceFinalize(NVNdevice *device)
{
    pfnc_nvnDeviceFinalize(device);
}

static NVNINLINE void nvnDeviceSetDebugLabel(NVNdevice *device, const char * label)
{
    pfnc_nvnDeviceSetDebugLabel(device, label);
}

static NVNINLINE PFNNVNGENERICFUNCPTRPROC nvnDeviceGetProcAddress(const NVNdevice *device, const char * name)
{
    return pfnc_nvnDeviceGetProcAddress(device, name);
}

static NVNINLINE void nvnDeviceGetInteger(const NVNdevice *device, NVNdeviceInfo pname, int *v)
{
    pfnc_nvnDeviceGetInteger(device, pname, v);
}

static NVNINLINE uint64_t nvnDeviceGetCurrentTimestampInNanoseconds(const NVNdevice *device)
{
    return pfnc_nvnDeviceGetCurrentTimestampInNanoseconds(device);
}

static NVNINLINE void nvnDeviceSetIntermediateShaderCache(NVNdevice *device, int numMaxEntries)
{
    pfnc_nvnDeviceSetIntermediateShaderCache(device, numMaxEntries);
}

static NVNINLINE NVNtextureHandle nvnDeviceGetTextureHandle(const NVNdevice *device, int textureID, int samplerID)
{
    return pfnc_nvnDeviceGetTextureHandle(device, textureID, samplerID);
}

static NVNINLINE NVNtextureHandle nvnDeviceGetTexelFetchHandle(const NVNdevice *device, int textureID)
{
    return pfnc_nvnDeviceGetTexelFetchHandle(device, textureID);
}

static NVNINLINE NVNimageHandle nvnDeviceGetImageHandle(const NVNdevice *device, int imageID)
{
    return pfnc_nvnDeviceGetImageHandle(device, imageID);
}

static NVNINLINE void nvnDeviceInstallDebugCallback(NVNdevice *device, const PFNNVNDEBUGCALLBACKPROC callback, void *callbackData, NVNboolean enable)
{
    pfnc_nvnDeviceInstallDebugCallback(device, callback, callbackData, enable);
}

static NVNINLINE NVNdebugDomainId nvnDeviceGenerateDebugDomainId(const NVNdevice *device, const char * name)
{
    return pfnc_nvnDeviceGenerateDebugDomainId(device, name);
}

static NVNINLINE void nvnDeviceSetWindowOriginMode(NVNdevice *device, NVNwindowOriginMode windowOriginMode)
{
    pfnc_nvnDeviceSetWindowOriginMode(device, windowOriginMode);
}

static NVNINLINE void nvnDeviceSetDepthMode(NVNdevice *device, NVNdepthMode depthMode)
{
    pfnc_nvnDeviceSetDepthMode(device, depthMode);
}

static NVNINLINE NVNboolean nvnDeviceRegisterFastClearColor(NVNdevice *device, const float *color, NVNformat format)
{
    return pfnc_nvnDeviceRegisterFastClearColor(device, color, format);
}

static NVNINLINE NVNboolean nvnDeviceRegisterFastClearColori(NVNdevice *device, const int *color, NVNformat format)
{
    return pfnc_nvnDeviceRegisterFastClearColori(device, color, format);
}

static NVNINLINE NVNboolean nvnDeviceRegisterFastClearColorui(NVNdevice *device, const uint32_t *color, NVNformat format)
{
    return pfnc_nvnDeviceRegisterFastClearColorui(device, color, format);
}

static NVNINLINE NVNboolean nvnDeviceRegisterFastClearDepth(NVNdevice *device, float depth)
{
    return pfnc_nvnDeviceRegisterFastClearDepth(device, depth);
}

static NVNINLINE NVNwindowOriginMode nvnDeviceGetWindowOriginMode(const NVNdevice *device)
{
    return pfnc_nvnDeviceGetWindowOriginMode(device);
}

static NVNINLINE NVNdepthMode nvnDeviceGetDepthMode(const NVNdevice *device)
{
    return pfnc_nvnDeviceGetDepthMode(device);
}

static NVNINLINE uint64_t nvnDeviceGetTimestampInNanoseconds(const NVNdevice *device, const NVNcounterData *counterData)
{
    return pfnc_nvnDeviceGetTimestampInNanoseconds(device, counterData);
}

static NVNINLINE void nvnDeviceApplyDeferredFinalizes(NVNdevice *device, int age)
{
    pfnc_nvnDeviceApplyDeferredFinalizes(device, age);
}

static NVNINLINE void nvnDeviceFinalizeCommandHandle(NVNdevice *device, NVNcommandHandle handles)
{
    pfnc_nvnDeviceFinalizeCommandHandle(device, handles);
}

static NVNINLINE void nvnDeviceWalkDebugDatabase(const NVNdevice *device, NVNdebugObjectType type, PFNNVNWALKDEBUGDATABASECALLBACKPROC callback, void *userParam)
{
    pfnc_nvnDeviceWalkDebugDatabase(device, type, callback, userParam);
}

static NVNINLINE NVNseparateTextureHandle nvnDeviceGetSeparateTextureHandle(const NVNdevice *device, int textureID)
{
    return pfnc_nvnDeviceGetSeparateTextureHandle(device, textureID);
}

static NVNINLINE NVNseparateSamplerHandle nvnDeviceGetSeparateSamplerHandle(const NVNdevice *device, int samplerID)
{
    return pfnc_nvnDeviceGetSeparateSamplerHandle(device, samplerID);
}

static NVNINLINE NVNboolean nvnDeviceIsExternalDebuggerAttached(const NVNdevice *device)
{
    return pfnc_nvnDeviceIsExternalDebuggerAttached(device);
}

static NVNINLINE NVNqueueGetErrorResult nvnQueueGetError(NVNqueue *queue, NVNqueueErrorInfo *errorInfo)
{
    return pfnc_nvnQueueGetError(queue, errorInfo);
}

static NVNINLINE size_t nvnQueueGetTotalCommandMemoryUsed(NVNqueue *queue)
{
    return pfnc_nvnQueueGetTotalCommandMemoryUsed(queue);
}

static NVNINLINE size_t nvnQueueGetTotalControlMemoryUsed(NVNqueue *queue)
{
    return pfnc_nvnQueueGetTotalControlMemoryUsed(queue);
}

static NVNINLINE size_t nvnQueueGetTotalComputeMemoryUsed(NVNqueue *queue)
{
    return pfnc_nvnQueueGetTotalComputeMemoryUsed(queue);
}

static NVNINLINE void nvnQueueResetMemoryUsageCounts(NVNqueue *queue)
{
    pfnc_nvnQueueResetMemoryUsageCounts(queue);
}

static NVNINLINE void nvnQueueBuilderSetDevice(NVNqueueBuilder *builder, NVNdevice *device)
{
    pfnc_nvnQueueBuilderSetDevice(builder, device);
}

static NVNINLINE void nvnQueueBuilderSetDefaults(NVNqueueBuilder *builder)
{
    pfnc_nvnQueueBuilderSetDefaults(builder);
}

static NVNINLINE void nvnQueueBuilderSetFlags(NVNqueueBuilder *builder, int flags)
{
    pfnc_nvnQueueBuilderSetFlags(builder, flags);
}

static NVNINLINE void nvnQueueBuilderSetCommandMemorySize(NVNqueueBuilder *builder, size_t size)
{
    pfnc_nvnQueueBuilderSetCommandMemorySize(builder, size);
}

static NVNINLINE void nvnQueueBuilderSetComputeMemorySize(NVNqueueBuilder *builder, size_t size)
{
    pfnc_nvnQueueBuilderSetComputeMemorySize(builder, size);
}

static NVNINLINE void nvnQueueBuilderSetControlMemorySize(NVNqueueBuilder *builder, size_t size)
{
    pfnc_nvnQueueBuilderSetControlMemorySize(builder, size);
}

static NVNINLINE size_t nvnQueueBuilderGetQueueMemorySize(const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueBuilderGetQueueMemorySize(builder);
}

static NVNINLINE void nvnQueueBuilderSetQueueMemory(NVNqueueBuilder *builder, void *memory, size_t size)
{
    pfnc_nvnQueueBuilderSetQueueMemory(builder, memory, size);
}

static NVNINLINE void nvnQueueBuilderSetCommandFlushThreshold(NVNqueueBuilder *builder, size_t threshold)
{
    pfnc_nvnQueueBuilderSetCommandFlushThreshold(builder, threshold);
}

static NVNINLINE void nvnQueueBuilderSetQueuePriority(NVNqueueBuilder *builder, const NVNqueuePriority priority)
{
    pfnc_nvnQueueBuilderSetQueuePriority(builder, priority);
}

static NVNINLINE NVNqueuePriority nvnQueueBuilderGetQueuePriority(const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueBuilderGetQueuePriority(builder);
}

static NVNINLINE const NVNdevice * nvnQueueBuilderGetDevice(const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueBuilderGetDevice(builder);
}

static NVNINLINE int nvnQueueBuilderGetFlags(const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueBuilderGetFlags(builder);
}

static NVNINLINE NVNboolean nvnQueueBuilderGetCommandMemorySize(const NVNqueueBuilder *builder, int *size)
{
    return pfnc_nvnQueueBuilderGetCommandMemorySize(builder, size);
}

static NVNINLINE NVNboolean nvnQueueBuilderGetComputeMemorySize(const NVNqueueBuilder *builder, int *size)
{
    return pfnc_nvnQueueBuilderGetComputeMemorySize(builder, size);
}

static NVNINLINE NVNboolean nvnQueueBuilderGetControlMemorySize(const NVNqueueBuilder *builder, int *size)
{
    return pfnc_nvnQueueBuilderGetControlMemorySize(builder, size);
}

static NVNINLINE NVNboolean nvnQueueBuilderGetCommandFlushThreshold(const NVNqueueBuilder *builder, int *flushThreshold)
{
    return pfnc_nvnQueueBuilderGetCommandFlushThreshold(builder, flushThreshold);
}

static NVNINLINE int nvnQueueBuilderGetMemorySize(const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueBuilderGetMemorySize(builder);
}

static NVNINLINE void* nvnQueueBuilderGetMemory(const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueBuilderGetMemory(builder);
}

static NVNINLINE NVNboolean nvnQueueInitialize(NVNqueue *queue, const NVNqueueBuilder *builder)
{
    return pfnc_nvnQueueInitialize(queue, builder);
}

static NVNINLINE void nvnQueueFinalize(NVNqueue *queue)
{
    pfnc_nvnQueueFinalize(queue);
}

static NVNINLINE void nvnQueueSetDebugLabel(NVNqueue *queue, const char * label)
{
    pfnc_nvnQueueSetDebugLabel(queue, label);
}

static NVNINLINE void nvnQueueSubmitCommands(NVNqueue *queue, int numCommands, const NVNcommandHandle *handles)
{
    pfnc_nvnQueueSubmitCommands(queue, numCommands, handles);
}

static NVNINLINE void nvnQueueFlush(NVNqueue *queue)
{
    pfnc_nvnQueueFlush(queue);
}

static NVNINLINE void nvnQueueFinish(NVNqueue *queue)
{
    pfnc_nvnQueueFinish(queue);
}

static NVNINLINE void nvnQueuePresentTexture(NVNqueue *queue, NVNwindow *window, int textureIndex)
{
    pfnc_nvnQueuePresentTexture(queue, window, textureIndex);
}

static NVNINLINE NVNqueueAcquireTextureResult nvnQueueAcquireTexture(NVNqueue *queue, NVNwindow *window, int *textureIndex)
{
    return pfnc_nvnQueueAcquireTexture(queue, window, textureIndex);
}

static NVNINLINE void nvnWindowBuilderSetDevice(NVNwindowBuilder *builder, NVNdevice *device)
{
    pfnc_nvnWindowBuilderSetDevice(builder, device);
}

static NVNINLINE void nvnWindowBuilderSetDefaults(NVNwindowBuilder *builder)
{
    pfnc_nvnWindowBuilderSetDefaults(builder);
}

static NVNINLINE void nvnWindowBuilderSetNativeWindow(NVNwindowBuilder *builder, NVNnativeWindow nativeWindow)
{
    pfnc_nvnWindowBuilderSetNativeWindow(builder, nativeWindow);
}

static NVNINLINE void nvnWindowBuilderSetTextures(NVNwindowBuilder *builder, int numTextures, NVNtexture * const *textures)
{
    pfnc_nvnWindowBuilderSetTextures(builder, numTextures, textures);
}

static NVNINLINE void nvnWindowBuilderSetPresentInterval(NVNwindowBuilder *builder, int presentInterval)
{
    pfnc_nvnWindowBuilderSetPresentInterval(builder, presentInterval);
}

static NVNINLINE void nvnWindowBuilderSetNumActiveTextures(NVNwindowBuilder *builder, int numActiveTextures)
{
    pfnc_nvnWindowBuilderSetNumActiveTextures(builder, numActiveTextures);
}

static NVNINLINE const NVNdevice * nvnWindowBuilderGetDevice(const NVNwindowBuilder *builder)
{
    return pfnc_nvnWindowBuilderGetDevice(builder);
}

static NVNINLINE int nvnWindowBuilderGetNumTextures(const NVNwindowBuilder *builder)
{
    return pfnc_nvnWindowBuilderGetNumTextures(builder);
}

static NVNINLINE const NVNtexture * nvnWindowBuilderGetTexture(const NVNwindowBuilder *builder, int textureId)
{
    return pfnc_nvnWindowBuilderGetTexture(builder, textureId);
}

static NVNINLINE NVNnativeWindow nvnWindowBuilderGetNativeWindow(const NVNwindowBuilder *builder)
{
    return pfnc_nvnWindowBuilderGetNativeWindow(builder);
}

static NVNINLINE int nvnWindowBuilderGetPresentInterval(const NVNwindowBuilder *builder)
{
    return pfnc_nvnWindowBuilderGetPresentInterval(builder);
}

static NVNINLINE int nvnWindowBuilderGetNumActiveTextures(const NVNwindowBuilder *builder)
{
    return pfnc_nvnWindowBuilderGetNumActiveTextures(builder);
}

static NVNINLINE NVNboolean nvnWindowInitialize(NVNwindow *window, const NVNwindowBuilder *builder)
{
    return pfnc_nvnWindowInitialize(window, builder);
}

static NVNINLINE void nvnWindowFinalize(NVNwindow *window)
{
    pfnc_nvnWindowFinalize(window);
}

static NVNINLINE void nvnWindowSetDebugLabel(NVNwindow *window, const char * label)
{
    pfnc_nvnWindowSetDebugLabel(window, label);
}

static NVNINLINE NVNwindowAcquireTextureResult nvnWindowAcquireTexture(NVNwindow *window, NVNsync *textureAvailableSync, int *textureIndex)
{
    return pfnc_nvnWindowAcquireTexture(window, textureAvailableSync, textureIndex);
}

static NVNINLINE NVNnativeWindow nvnWindowGetNativeWindow(const NVNwindow *window)
{
    return pfnc_nvnWindowGetNativeWindow(window);
}

static NVNINLINE int nvnWindowGetPresentInterval(const NVNwindow *window)
{
    return pfnc_nvnWindowGetPresentInterval(window);
}

static NVNINLINE void nvnWindowSetPresentInterval(NVNwindow *window, int presentInterval)
{
    pfnc_nvnWindowSetPresentInterval(window, presentInterval);
}

static NVNINLINE void nvnWindowSetCrop(NVNwindow *window, int x, int y, int w, int h)
{
    pfnc_nvnWindowSetCrop(window, x, y, w, h);
}

static NVNINLINE void nvnWindowGetCrop(const NVNwindow *window, NVNrectangle *crop)
{
    pfnc_nvnWindowGetCrop(window, crop);
}

static NVNINLINE void nvnWindowSetNumActiveTextures(NVNwindow *window, int numActiveTextures)
{
    pfnc_nvnWindowSetNumActiveTextures(window, numActiveTextures);
}

static NVNINLINE int nvnWindowGetNumActiveTextures(const NVNwindow *window)
{
    return pfnc_nvnWindowGetNumActiveTextures(window);
}

static NVNINLINE int nvnWindowGetNumTextures(const NVNwindow *window)
{
    return pfnc_nvnWindowGetNumTextures(window);
}

static NVNINLINE NVNboolean nvnProgramInitialize(NVNprogram *program, NVNdevice *device)
{
    return pfnc_nvnProgramInitialize(program, device);
}

static NVNINLINE void nvnProgramFinalize(NVNprogram *program)
{
    pfnc_nvnProgramFinalize(program);
}

static NVNINLINE void nvnProgramSetDebugLabel(NVNprogram *program, const char * label)
{
    pfnc_nvnProgramSetDebugLabel(program, label);
}

static NVNINLINE NVNboolean nvnProgramSetShaders(NVNprogram *program, int count, const NVNshaderData *stageData)
{
    return pfnc_nvnProgramSetShaders(program, count, stageData);
}

static NVNINLINE NVNboolean nvnProgramSetShadersExt(NVNprogram *program, int count, const NVNshaderDataExt *stageDataExt)
{
    return pfnc_nvnProgramSetShadersExt(program, count, stageDataExt);
}

static NVNINLINE void nvnProgramSetSampleShading(NVNprogram *program, NVNsampleShadingMode shadingMode)
{
    pfnc_nvnProgramSetSampleShading(program, shadingMode);
}

static NVNINLINE void nvnMemoryPoolBuilderSetDevice(NVNmemoryPoolBuilder *builder, NVNdevice *device)
{
    pfnc_nvnMemoryPoolBuilderSetDevice(builder, device);
}

static NVNINLINE void nvnMemoryPoolBuilderSetDefaults(NVNmemoryPoolBuilder *builder)
{
    pfnc_nvnMemoryPoolBuilderSetDefaults(builder);
}

static NVNINLINE void nvnMemoryPoolBuilderSetStorage(NVNmemoryPoolBuilder *builder, void *memory, size_t size)
{
    pfnc_nvnMemoryPoolBuilderSetStorage(builder, memory, size);
}

static NVNINLINE void nvnMemoryPoolBuilderSetFlags(NVNmemoryPoolBuilder *builder, int flags)
{
    pfnc_nvnMemoryPoolBuilderSetFlags(builder, flags);
}

static NVNINLINE const NVNdevice * nvnMemoryPoolBuilderGetDevice(const NVNmemoryPoolBuilder *builder)
{
    return pfnc_nvnMemoryPoolBuilderGetDevice(builder);
}

static NVNINLINE void* nvnMemoryPoolBuilderGetMemory(const NVNmemoryPoolBuilder *builder)
{
    return pfnc_nvnMemoryPoolBuilderGetMemory(builder);
}

static NVNINLINE size_t nvnMemoryPoolBuilderGetSize(const NVNmemoryPoolBuilder *builder)
{
    return pfnc_nvnMemoryPoolBuilderGetSize(builder);
}

static NVNINLINE NVNmemoryPoolFlags nvnMemoryPoolBuilderGetFlags(const NVNmemoryPoolBuilder *builder)
{
    return pfnc_nvnMemoryPoolBuilderGetFlags(builder);
}

static NVNINLINE NVNboolean nvnMemoryPoolInitialize(NVNmemoryPool *pool, const NVNmemoryPoolBuilder *builder)
{
    return pfnc_nvnMemoryPoolInitialize(pool, builder);
}

static NVNINLINE void nvnMemoryPoolSetDebugLabel(NVNmemoryPool *pool, const char * label)
{
    pfnc_nvnMemoryPoolSetDebugLabel(pool, label);
}

static NVNINLINE void nvnMemoryPoolFinalize(NVNmemoryPool *pool)
{
    pfnc_nvnMemoryPoolFinalize(pool);
}

static NVNINLINE void* nvnMemoryPoolMap(const NVNmemoryPool *pool)
{
    return pfnc_nvnMemoryPoolMap(pool);
}

static NVNINLINE void nvnMemoryPoolFlushMappedRange(const NVNmemoryPool *pool, ptrdiff_t offset, size_t size)
{
    pfnc_nvnMemoryPoolFlushMappedRange(pool, offset, size);
}

static NVNINLINE void nvnMemoryPoolInvalidateMappedRange(const NVNmemoryPool *pool, ptrdiff_t offset, size_t size)
{
    pfnc_nvnMemoryPoolInvalidateMappedRange(pool, offset, size);
}

static NVNINLINE NVNbufferAddress nvnMemoryPoolGetBufferAddress(const NVNmemoryPool *pool)
{
    return pfnc_nvnMemoryPoolGetBufferAddress(pool);
}

static NVNINLINE NVNboolean nvnMemoryPoolMapVirtual(NVNmemoryPool *virtualPool, int numRequests, const NVNmappingRequest *requests)
{
    return pfnc_nvnMemoryPoolMapVirtual(virtualPool, numRequests, requests);
}

static NVNINLINE size_t nvnMemoryPoolGetSize(const NVNmemoryPool *pool)
{
    return pfnc_nvnMemoryPoolGetSize(pool);
}

static NVNINLINE NVNmemoryPoolFlags nvnMemoryPoolGetFlags(const NVNmemoryPool *pool)
{
    return pfnc_nvnMemoryPoolGetFlags(pool);
}

static NVNINLINE NVNboolean nvnTexturePoolInitialize(NVNtexturePool *texturePool, const NVNmemoryPool *memoryPool, ptrdiff_t offset, int numDescriptors)
{
    return pfnc_nvnTexturePoolInitialize(texturePool, memoryPool, offset, numDescriptors);
}

static NVNINLINE void nvnTexturePoolSetDebugLabel(NVNtexturePool *pool, const char * label)
{
    pfnc_nvnTexturePoolSetDebugLabel(pool, label);
}

static NVNINLINE void nvnTexturePoolFinalize(NVNtexturePool *pool)
{
    pfnc_nvnTexturePoolFinalize(pool);
}

static NVNINLINE void nvnTexturePoolRegisterTexture(const NVNtexturePool *texturePool, int id, const NVNtexture *texture, const NVNtextureView *view)
{
    pfnc_nvnTexturePoolRegisterTexture(texturePool, id, texture, view);
}

static NVNINLINE void nvnTexturePoolRegisterImage(const NVNtexturePool *texturePool, int id, const NVNtexture *texture, const NVNtextureView *view)
{
    pfnc_nvnTexturePoolRegisterImage(texturePool, id, texture, view);
}

static NVNINLINE const NVNmemoryPool * nvnTexturePoolGetMemoryPool(const NVNtexturePool *pool)
{
    return pfnc_nvnTexturePoolGetMemoryPool(pool);
}

static NVNINLINE ptrdiff_t nvnTexturePoolGetMemoryOffset(const NVNtexturePool *pool)
{
    return pfnc_nvnTexturePoolGetMemoryOffset(pool);
}

static NVNINLINE int nvnTexturePoolGetSize(const NVNtexturePool *pool)
{
    return pfnc_nvnTexturePoolGetSize(pool);
}

static NVNINLINE NVNboolean nvnSamplerPoolInitialize(NVNsamplerPool *samplerPool, const NVNmemoryPool *memoryPool, ptrdiff_t offset, int numDescriptors)
{
    return pfnc_nvnSamplerPoolInitialize(samplerPool, memoryPool, offset, numDescriptors);
}

static NVNINLINE void nvnSamplerPoolSetDebugLabel(NVNsamplerPool *pool, const char * label)
{
    pfnc_nvnSamplerPoolSetDebugLabel(pool, label);
}

static NVNINLINE void nvnSamplerPoolFinalize(NVNsamplerPool *pool)
{
    pfnc_nvnSamplerPoolFinalize(pool);
}

static NVNINLINE void nvnSamplerPoolRegisterSampler(const NVNsamplerPool *samplerPool, int id, const NVNsampler *sampler)
{
    pfnc_nvnSamplerPoolRegisterSampler(samplerPool, id, sampler);
}

static NVNINLINE void nvnSamplerPoolRegisterSamplerBuilder(const NVNsamplerPool *samplerPool, int id, const NVNsamplerBuilder *builder)
{
    pfnc_nvnSamplerPoolRegisterSamplerBuilder(samplerPool, id, builder);
}

static NVNINLINE const NVNmemoryPool * nvnSamplerPoolGetMemoryPool(const NVNsamplerPool *pool)
{
    return pfnc_nvnSamplerPoolGetMemoryPool(pool);
}

static NVNINLINE ptrdiff_t nvnSamplerPoolGetMemoryOffset(const NVNsamplerPool *pool)
{
    return pfnc_nvnSamplerPoolGetMemoryOffset(pool);
}

static NVNINLINE int nvnSamplerPoolGetSize(const NVNsamplerPool *pool)
{
    return pfnc_nvnSamplerPoolGetSize(pool);
}

static NVNINLINE void nvnBufferBuilderSetDevice(NVNbufferBuilder *builder, NVNdevice *device)
{
    pfnc_nvnBufferBuilderSetDevice(builder, device);
}

static NVNINLINE void nvnBufferBuilderSetDefaults(NVNbufferBuilder *builder)
{
    pfnc_nvnBufferBuilderSetDefaults(builder);
}

static NVNINLINE void nvnBufferBuilderSetStorage(NVNbufferBuilder *builder, NVNmemoryPool *pool, ptrdiff_t offset, size_t size)
{
    pfnc_nvnBufferBuilderSetStorage(builder, pool, offset, size);
}

static NVNINLINE const NVNdevice * nvnBufferBuilderGetDevice(const NVNbufferBuilder *builder)
{
    return pfnc_nvnBufferBuilderGetDevice(builder);
}

static NVNINLINE NVNmemoryPool * nvnBufferBuilderGetMemoryPool(const NVNbufferBuilder *builder)
{
    return pfnc_nvnBufferBuilderGetMemoryPool(builder);
}

static NVNINLINE ptrdiff_t nvnBufferBuilderGetMemoryOffset(const NVNbufferBuilder *builder)
{
    return pfnc_nvnBufferBuilderGetMemoryOffset(builder);
}

static NVNINLINE size_t nvnBufferBuilderGetSize(const NVNbufferBuilder *builder)
{
    return pfnc_nvnBufferBuilderGetSize(builder);
}

static NVNINLINE NVNboolean nvnBufferInitialize(NVNbuffer *buffer, const NVNbufferBuilder *builder)
{
    return pfnc_nvnBufferInitialize(buffer, builder);
}

static NVNINLINE void nvnBufferSetDebugLabel(NVNbuffer *buffer, const char * label)
{
    pfnc_nvnBufferSetDebugLabel(buffer, label);
}

static NVNINLINE void nvnBufferFinalize(NVNbuffer *buffer)
{
    pfnc_nvnBufferFinalize(buffer);
}

static NVNINLINE void* nvnBufferMap(const NVNbuffer *buffer)
{
    return pfnc_nvnBufferMap(buffer);
}

static NVNINLINE NVNbufferAddress nvnBufferGetAddress(const NVNbuffer *buffer)
{
    return pfnc_nvnBufferGetAddress(buffer);
}

static NVNINLINE void nvnBufferFlushMappedRange(const NVNbuffer *buffer, ptrdiff_t offset, size_t size)
{
    pfnc_nvnBufferFlushMappedRange(buffer, offset, size);
}

static NVNINLINE void nvnBufferInvalidateMappedRange(const NVNbuffer *buffer, ptrdiff_t offset, size_t size)
{
    pfnc_nvnBufferInvalidateMappedRange(buffer, offset, size);
}

static NVNINLINE NVNmemoryPool * nvnBufferGetMemoryPool(const NVNbuffer *buffer)
{
    return pfnc_nvnBufferGetMemoryPool(buffer);
}

static NVNINLINE ptrdiff_t nvnBufferGetMemoryOffset(const NVNbuffer *buffer)
{
    return pfnc_nvnBufferGetMemoryOffset(buffer);
}

static NVNINLINE size_t nvnBufferGetSize(const NVNbuffer *buffer)
{
    return pfnc_nvnBufferGetSize(buffer);
}

static NVNINLINE uint64_t nvnBufferGetDebugID(const NVNbuffer *buffer)
{
    return pfnc_nvnBufferGetDebugID(buffer);
}

static NVNINLINE void nvnTextureBuilderSetDevice(NVNtextureBuilder *builder, NVNdevice *device)
{
    pfnc_nvnTextureBuilderSetDevice(builder, device);
}

static NVNINLINE void nvnTextureBuilderSetDefaults(NVNtextureBuilder *builder)
{
    pfnc_nvnTextureBuilderSetDefaults(builder);
}

static NVNINLINE void nvnTextureBuilderSetFlags(NVNtextureBuilder *builder, int flags)
{
    pfnc_nvnTextureBuilderSetFlags(builder, flags);
}

static NVNINLINE void nvnTextureBuilderSetTarget(NVNtextureBuilder *builder, NVNtextureTarget target)
{
    pfnc_nvnTextureBuilderSetTarget(builder, target);
}

static NVNINLINE void nvnTextureBuilderSetWidth(NVNtextureBuilder *builder, int width)
{
    pfnc_nvnTextureBuilderSetWidth(builder, width);
}

static NVNINLINE void nvnTextureBuilderSetHeight(NVNtextureBuilder *builder, int height)
{
    pfnc_nvnTextureBuilderSetHeight(builder, height);
}

static NVNINLINE void nvnTextureBuilderSetDepth(NVNtextureBuilder *builder, int depth)
{
    pfnc_nvnTextureBuilderSetDepth(builder, depth);
}

static NVNINLINE void nvnTextureBuilderSetSize1D(NVNtextureBuilder *builder, int width)
{
    pfnc_nvnTextureBuilderSetSize1D(builder, width);
}

static NVNINLINE void nvnTextureBuilderSetSize2D(NVNtextureBuilder *builder, int width, int height)
{
    pfnc_nvnTextureBuilderSetSize2D(builder, width, height);
}

static NVNINLINE void nvnTextureBuilderSetSize3D(NVNtextureBuilder *builder, int width, int height, int depth)
{
    pfnc_nvnTextureBuilderSetSize3D(builder, width, height, depth);
}

static NVNINLINE void nvnTextureBuilderSetLevels(NVNtextureBuilder *builder, int numLevels)
{
    pfnc_nvnTextureBuilderSetLevels(builder, numLevels);
}

static NVNINLINE void nvnTextureBuilderSetFormat(NVNtextureBuilder *builder, NVNformat format)
{
    pfnc_nvnTextureBuilderSetFormat(builder, format);
}

static NVNINLINE void nvnTextureBuilderSetSamples(NVNtextureBuilder *builder, int samples)
{
    pfnc_nvnTextureBuilderSetSamples(builder, samples);
}

static NVNINLINE void nvnTextureBuilderSetSwizzle(NVNtextureBuilder *builder, NVNtextureSwizzle r, NVNtextureSwizzle g, NVNtextureSwizzle b, NVNtextureSwizzle a)
{
    pfnc_nvnTextureBuilderSetSwizzle(builder, r, g, b, a);
}

static NVNINLINE void nvnTextureBuilderSetDepthStencilMode(NVNtextureBuilder *builder, NVNtextureDepthStencilMode mode)
{
    pfnc_nvnTextureBuilderSetDepthStencilMode(builder, mode);
}

static NVNINLINE size_t nvnTextureBuilderGetStorageSize(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetStorageSize(builder);
}

static NVNINLINE size_t nvnTextureBuilderGetStorageAlignment(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetStorageAlignment(builder);
}

static NVNINLINE void nvnTextureBuilderSetStorage(NVNtextureBuilder *builder, NVNmemoryPool *pool, ptrdiff_t offset)
{
    pfnc_nvnTextureBuilderSetStorage(builder, pool, offset);
}

static NVNINLINE void nvnTextureBuilderSetPackagedTextureData(NVNtextureBuilder *builder, const void *data)
{
    pfnc_nvnTextureBuilderSetPackagedTextureData(builder, data);
}

static NVNINLINE void nvnTextureBuilderSetPackagedTextureLayout(NVNtextureBuilder *builder, const NVNpackagedTextureLayout *layout)
{
    pfnc_nvnTextureBuilderSetPackagedTextureLayout(builder, layout);
}

static NVNINLINE void nvnTextureBuilderSetStride(NVNtextureBuilder *builder, ptrdiff_t stride)
{
    pfnc_nvnTextureBuilderSetStride(builder, stride);
}

static NVNINLINE void nvnTextureBuilderSetGLTextureName(NVNtextureBuilder *builder, uint32_t name)
{
    pfnc_nvnTextureBuilderSetGLTextureName(builder, name);
}

static NVNINLINE NVNstorageClass nvnTextureBuilderGetStorageClass(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetStorageClass(builder);
}

static NVNINLINE const NVNdevice * nvnTextureBuilderGetDevice(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetDevice(builder);
}

static NVNINLINE NVNtextureFlags nvnTextureBuilderGetFlags(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetFlags(builder);
}

static NVNINLINE NVNtextureTarget nvnTextureBuilderGetTarget(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetTarget(builder);
}

static NVNINLINE int nvnTextureBuilderGetWidth(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetWidth(builder);
}

static NVNINLINE int nvnTextureBuilderGetHeight(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetHeight(builder);
}

static NVNINLINE int nvnTextureBuilderGetDepth(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetDepth(builder);
}

static NVNINLINE int nvnTextureBuilderGetLevels(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetLevels(builder);
}

static NVNINLINE NVNformat nvnTextureBuilderGetFormat(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetFormat(builder);
}

static NVNINLINE int nvnTextureBuilderGetSamples(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetSamples(builder);
}

static NVNINLINE void nvnTextureBuilderGetSwizzle(const NVNtextureBuilder *builder, NVNtextureSwizzle *r, NVNtextureSwizzle *g, NVNtextureSwizzle *b, NVNtextureSwizzle *a)
{
    pfnc_nvnTextureBuilderGetSwizzle(builder, r, g, b, a);
}

static NVNINLINE NVNtextureDepthStencilMode nvnTextureBuilderGetDepthStencilMode(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetDepthStencilMode(builder);
}

static NVNINLINE const void * nvnTextureBuilderGetPackagedTextureData(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetPackagedTextureData(builder);
}

static NVNINLINE NVNboolean nvnTextureBuilderGetPackagedTextureLayout(const NVNtextureBuilder *builder, NVNpackagedTextureLayout *data)
{
    return pfnc_nvnTextureBuilderGetPackagedTextureLayout(builder, data);
}

static NVNINLINE ptrdiff_t nvnTextureBuilderGetStride(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetStride(builder);
}

static NVNINLINE void nvnTextureBuilderGetSparseTileLayout(const NVNtextureBuilder *builder, NVNtextureSparseTileLayout *layout)
{
    pfnc_nvnTextureBuilderGetSparseTileLayout(builder, layout);
}

static NVNINLINE uint32_t nvnTextureBuilderGetGLTextureName(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetGLTextureName(builder);
}

static NVNINLINE size_t nvnTextureBuilderGetZCullStorageSize(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetZCullStorageSize(builder);
}

static NVNINLINE NVNmemoryPool * nvnTextureBuilderGetMemoryPool(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetMemoryPool(builder);
}

static NVNINLINE ptrdiff_t nvnTextureBuilderGetMemoryOffset(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetMemoryOffset(builder);
}

static NVNINLINE NVNrawStorageClass nvnTextureBuilderGetRawStorageClass(const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureBuilderGetRawStorageClass(builder);
}

static NVNINLINE void nvnTextureViewSetDefaults(NVNtextureView *view)
{
    pfnc_nvnTextureViewSetDefaults(view);
}

static NVNINLINE void nvnTextureViewSetLevels(NVNtextureView *view, int baseLevel, int numLevels)
{
    pfnc_nvnTextureViewSetLevels(view, baseLevel, numLevels);
}

static NVNINLINE void nvnTextureViewSetLayers(NVNtextureView *view, int minLayer, int numLayers)
{
    pfnc_nvnTextureViewSetLayers(view, minLayer, numLayers);
}

static NVNINLINE void nvnTextureViewSetFormat(NVNtextureView *view, NVNformat format)
{
    pfnc_nvnTextureViewSetFormat(view, format);
}

static NVNINLINE void nvnTextureViewSetSwizzle(NVNtextureView *view, NVNtextureSwizzle r, NVNtextureSwizzle g, NVNtextureSwizzle b, NVNtextureSwizzle a)
{
    pfnc_nvnTextureViewSetSwizzle(view, r, g, b, a);
}

static NVNINLINE void nvnTextureViewSetDepthStencilMode(NVNtextureView *view, NVNtextureDepthStencilMode mode)
{
    pfnc_nvnTextureViewSetDepthStencilMode(view, mode);
}

static NVNINLINE void nvnTextureViewSetTarget(NVNtextureView *view, NVNtextureTarget target)
{
    pfnc_nvnTextureViewSetTarget(view, target);
}

static NVNINLINE NVNboolean nvnTextureViewGetLevels(const NVNtextureView *view, int *baseLevel, int *numLevels)
{
    return pfnc_nvnTextureViewGetLevels(view, baseLevel, numLevels);
}

static NVNINLINE NVNboolean nvnTextureViewGetLayers(const NVNtextureView *view, int *minLayer, int *numLayers)
{
    return pfnc_nvnTextureViewGetLayers(view, minLayer, numLayers);
}

static NVNINLINE NVNboolean nvnTextureViewGetFormat(const NVNtextureView *view, NVNformat *format)
{
    return pfnc_nvnTextureViewGetFormat(view, format);
}

static NVNINLINE NVNboolean nvnTextureViewGetSwizzle(const NVNtextureView *view, NVNtextureSwizzle *r, NVNtextureSwizzle *g, NVNtextureSwizzle *b, NVNtextureSwizzle *a)
{
    return pfnc_nvnTextureViewGetSwizzle(view, r, g, b, a);
}

static NVNINLINE NVNboolean nvnTextureViewGetDepthStencilMode(const NVNtextureView *view, NVNtextureDepthStencilMode *mode)
{
    return pfnc_nvnTextureViewGetDepthStencilMode(view, mode);
}

static NVNINLINE NVNboolean nvnTextureViewGetTarget(const NVNtextureView *view, NVNtextureTarget *target)
{
    return pfnc_nvnTextureViewGetTarget(view, target);
}

static NVNINLINE NVNboolean nvnTextureViewCompare(const NVNtextureView *view, const NVNtextureView *otherView)
{
    return pfnc_nvnTextureViewCompare(view, otherView);
}

static NVNINLINE NVNboolean nvnTextureInitialize(NVNtexture *texture, const NVNtextureBuilder *builder)
{
    return pfnc_nvnTextureInitialize(texture, builder);
}

static NVNINLINE size_t nvnTextureGetZCullStorageSize(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetZCullStorageSize(texture);
}

static NVNINLINE void nvnTextureFinalize(NVNtexture *texture)
{
    pfnc_nvnTextureFinalize(texture);
}

static NVNINLINE void nvnTextureSetDebugLabel(NVNtexture *texture, const char * label)
{
    pfnc_nvnTextureSetDebugLabel(texture, label);
}

static NVNINLINE NVNstorageClass nvnTextureGetStorageClass(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetStorageClass(texture);
}

static NVNINLINE ptrdiff_t nvnTextureGetViewOffset(const NVNtexture *texture, const NVNtextureView *textureView)
{
    return pfnc_nvnTextureGetViewOffset(texture, textureView);
}

static NVNINLINE NVNtextureFlags nvnTextureGetFlags(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetFlags(texture);
}

static NVNINLINE NVNtextureTarget nvnTextureGetTarget(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetTarget(texture);
}

static NVNINLINE int nvnTextureGetWidth(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetWidth(texture);
}

static NVNINLINE int nvnTextureGetHeight(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetHeight(texture);
}

static NVNINLINE int nvnTextureGetDepth(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetDepth(texture);
}

static NVNINLINE int nvnTextureGetLevels(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetLevels(texture);
}

static NVNINLINE NVNformat nvnTextureGetFormat(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetFormat(texture);
}

static NVNINLINE int nvnTextureGetSamples(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetSamples(texture);
}

static NVNINLINE void nvnTextureGetSwizzle(const NVNtexture *texture, NVNtextureSwizzle *r, NVNtextureSwizzle *g, NVNtextureSwizzle *b, NVNtextureSwizzle *a)
{
    pfnc_nvnTextureGetSwizzle(texture, r, g, b, a);
}

static NVNINLINE NVNtextureDepthStencilMode nvnTextureGetDepthStencilMode(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetDepthStencilMode(texture);
}

static NVNINLINE ptrdiff_t nvnTextureGetStride(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetStride(texture);
}

static NVNINLINE NVNtextureAddress nvnTextureGetTextureAddress(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetTextureAddress(texture);
}

static NVNINLINE void nvnTextureGetSparseTileLayout(const NVNtexture *texture, NVNtextureSparseTileLayout *layout)
{
    pfnc_nvnTextureGetSparseTileLayout(texture, layout);
}

static NVNINLINE void nvnTextureWriteTexels(const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, const void *data)
{
    pfnc_nvnTextureWriteTexels(texture, textureView, region, data);
}

static NVNINLINE void nvnTextureWriteTexelsStrided(const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, const void *data, ptrdiff_t rowStride, ptrdiff_t imageStride)
{
    pfnc_nvnTextureWriteTexelsStrided(texture, textureView, region, data, rowStride, imageStride);
}

static NVNINLINE void nvnTextureReadTexels(const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, void *data)
{
    pfnc_nvnTextureReadTexels(texture, textureView, region, data);
}

static NVNINLINE void nvnTextureReadTexelsStrided(const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, void *data, ptrdiff_t rowStride, ptrdiff_t imageStride)
{
    pfnc_nvnTextureReadTexelsStrided(texture, textureView, region, data, rowStride, imageStride);
}

static NVNINLINE void nvnTextureFlushTexels(const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region)
{
    pfnc_nvnTextureFlushTexels(texture, textureView, region);
}

static NVNINLINE void nvnTextureInvalidateTexels(const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region)
{
    pfnc_nvnTextureInvalidateTexels(texture, textureView, region);
}

static NVNINLINE NVNmemoryPool * nvnTextureGetMemoryPool(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetMemoryPool(texture);
}

static NVNINLINE ptrdiff_t nvnTextureGetMemoryOffset(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetMemoryOffset(texture);
}

static NVNINLINE int nvnTextureGetStorageSize(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetStorageSize(texture);
}

static NVNINLINE NVNboolean nvnTextureCompare(const NVNtexture *texture, const NVNtexture *otherTexture)
{
    return pfnc_nvnTextureCompare(texture, otherTexture);
}

static NVNINLINE uint64_t nvnTextureGetDebugID(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetDebugID(texture);
}

static NVNINLINE NVNrawStorageClass nvnTextureGetRawStorageClass(const NVNtexture *texture)
{
    return pfnc_nvnTextureGetRawStorageClass(texture);
}

static NVNINLINE void nvnSamplerBuilderSetDevice(NVNsamplerBuilder *builder, NVNdevice *device)
{
    pfnc_nvnSamplerBuilderSetDevice(builder, device);
}

static NVNINLINE void nvnSamplerBuilderSetDefaults(NVNsamplerBuilder *builder)
{
    pfnc_nvnSamplerBuilderSetDefaults(builder);
}

static NVNINLINE void nvnSamplerBuilderSetMinMagFilter(NVNsamplerBuilder *builder, NVNminFilter min, NVNmagFilter mag)
{
    pfnc_nvnSamplerBuilderSetMinMagFilter(builder, min, mag);
}

static NVNINLINE void nvnSamplerBuilderSetWrapMode(NVNsamplerBuilder *builder, NVNwrapMode s, NVNwrapMode t, NVNwrapMode r)
{
    pfnc_nvnSamplerBuilderSetWrapMode(builder, s, t, r);
}

static NVNINLINE void nvnSamplerBuilderSetLodClamp(NVNsamplerBuilder *builder, float min, float max)
{
    pfnc_nvnSamplerBuilderSetLodClamp(builder, min, max);
}

static NVNINLINE void nvnSamplerBuilderSetLodBias(NVNsamplerBuilder *builder, float bias)
{
    pfnc_nvnSamplerBuilderSetLodBias(builder, bias);
}

static NVNINLINE void nvnSamplerBuilderSetCompare(NVNsamplerBuilder *builder, NVNcompareMode mode, NVNcompareFunc func)
{
    pfnc_nvnSamplerBuilderSetCompare(builder, mode, func);
}

static NVNINLINE void nvnSamplerBuilderSetBorderColor(NVNsamplerBuilder *builder, const float *borderColor)
{
    pfnc_nvnSamplerBuilderSetBorderColor(builder, borderColor);
}

static NVNINLINE void nvnSamplerBuilderSetBorderColori(NVNsamplerBuilder *builder, const int *borderColor)
{
    pfnc_nvnSamplerBuilderSetBorderColori(builder, borderColor);
}

static NVNINLINE void nvnSamplerBuilderSetBorderColorui(NVNsamplerBuilder *builder, const uint32_t *borderColor)
{
    pfnc_nvnSamplerBuilderSetBorderColorui(builder, borderColor);
}

static NVNINLINE void nvnSamplerBuilderSetMaxAnisotropy(NVNsamplerBuilder *builder, float maxAniso)
{
    pfnc_nvnSamplerBuilderSetMaxAnisotropy(builder, maxAniso);
}

static NVNINLINE void nvnSamplerBuilderSetReductionFilter(NVNsamplerBuilder *builder, NVNsamplerReduction filter)
{
    pfnc_nvnSamplerBuilderSetReductionFilter(builder, filter);
}

static NVNINLINE void nvnSamplerBuilderSetLodSnap(NVNsamplerBuilder *builder, float lodSnap)
{
    pfnc_nvnSamplerBuilderSetLodSnap(builder, lodSnap);
}

static NVNINLINE const NVNdevice * nvnSamplerBuilderGetDevice(const NVNsamplerBuilder *builder)
{
    return pfnc_nvnSamplerBuilderGetDevice(builder);
}

static NVNINLINE void nvnSamplerBuilderGetMinMagFilter(const NVNsamplerBuilder *builder, NVNminFilter *min, NVNmagFilter *mag)
{
    pfnc_nvnSamplerBuilderGetMinMagFilter(builder, min, mag);
}

static NVNINLINE void nvnSamplerBuilderGetWrapMode(const NVNsamplerBuilder *builder, NVNwrapMode *s, NVNwrapMode *t, NVNwrapMode *r)
{
    pfnc_nvnSamplerBuilderGetWrapMode(builder, s, t, r);
}

static NVNINLINE void nvnSamplerBuilderGetLodClamp(const NVNsamplerBuilder *builder, float *min, float *max)
{
    pfnc_nvnSamplerBuilderGetLodClamp(builder, min, max);
}

static NVNINLINE float nvnSamplerBuilderGetLodBias(const NVNsamplerBuilder *builder)
{
    return pfnc_nvnSamplerBuilderGetLodBias(builder);
}

static NVNINLINE void nvnSamplerBuilderGetCompare(const NVNsamplerBuilder *builder, NVNcompareMode *mode, NVNcompareFunc *func)
{
    pfnc_nvnSamplerBuilderGetCompare(builder, mode, func);
}

static NVNINLINE void nvnSamplerBuilderGetBorderColor(const NVNsamplerBuilder *builder, float *borderColor)
{
    pfnc_nvnSamplerBuilderGetBorderColor(builder, borderColor);
}

static NVNINLINE void nvnSamplerBuilderGetBorderColori(const NVNsamplerBuilder *builder, int *borderColor)
{
    pfnc_nvnSamplerBuilderGetBorderColori(builder, borderColor);
}

static NVNINLINE void nvnSamplerBuilderGetBorderColorui(const NVNsamplerBuilder *builder, uint32_t *borderColor)
{
    pfnc_nvnSamplerBuilderGetBorderColorui(builder, borderColor);
}

static NVNINLINE float nvnSamplerBuilderGetMaxAnisotropy(const NVNsamplerBuilder *builder)
{
    return pfnc_nvnSamplerBuilderGetMaxAnisotropy(builder);
}

static NVNINLINE NVNsamplerReduction nvnSamplerBuilderGetReductionFilter(const NVNsamplerBuilder *builder)
{
    return pfnc_nvnSamplerBuilderGetReductionFilter(builder);
}

static NVNINLINE float nvnSamplerBuilderGetLodSnap(const NVNsamplerBuilder *builder)
{
    return pfnc_nvnSamplerBuilderGetLodSnap(builder);
}

static NVNINLINE NVNboolean nvnSamplerInitialize(NVNsampler *sampler, const NVNsamplerBuilder *builder)
{
    return pfnc_nvnSamplerInitialize(sampler, builder);
}

static NVNINLINE void nvnSamplerFinalize(NVNsampler *sampler)
{
    pfnc_nvnSamplerFinalize(sampler);
}

static NVNINLINE void nvnSamplerSetDebugLabel(NVNsampler *sampler, const char * label)
{
    pfnc_nvnSamplerSetDebugLabel(sampler, label);
}

static NVNINLINE void nvnSamplerGetMinMagFilter(const NVNsampler *sampler, NVNminFilter *min, NVNmagFilter *mag)
{
    pfnc_nvnSamplerGetMinMagFilter(sampler, min, mag);
}

static NVNINLINE void nvnSamplerGetWrapMode(const NVNsampler *sampler, NVNwrapMode *s, NVNwrapMode *t, NVNwrapMode *r)
{
    pfnc_nvnSamplerGetWrapMode(sampler, s, t, r);
}

static NVNINLINE void nvnSamplerGetLodClamp(const NVNsampler *sampler, float *min, float *max)
{
    pfnc_nvnSamplerGetLodClamp(sampler, min, max);
}

static NVNINLINE float nvnSamplerGetLodBias(const NVNsampler *sampler)
{
    return pfnc_nvnSamplerGetLodBias(sampler);
}

static NVNINLINE void nvnSamplerGetCompare(const NVNsampler *sampler, NVNcompareMode *mode, NVNcompareFunc *func)
{
    pfnc_nvnSamplerGetCompare(sampler, mode, func);
}

static NVNINLINE void nvnSamplerGetBorderColor(const NVNsampler *sampler, float *borderColor)
{
    pfnc_nvnSamplerGetBorderColor(sampler, borderColor);
}

static NVNINLINE void nvnSamplerGetBorderColori(const NVNsampler *sampler, int *borderColor)
{
    pfnc_nvnSamplerGetBorderColori(sampler, borderColor);
}

static NVNINLINE void nvnSamplerGetBorderColorui(const NVNsampler *sampler, uint32_t *borderColor)
{
    pfnc_nvnSamplerGetBorderColorui(sampler, borderColor);
}

static NVNINLINE float nvnSamplerGetMaxAnisotropy(const NVNsampler *sampler)
{
    return pfnc_nvnSamplerGetMaxAnisotropy(sampler);
}

static NVNINLINE NVNsamplerReduction nvnSamplerGetReductionFilter(const NVNsampler *sampler)
{
    return pfnc_nvnSamplerGetReductionFilter(sampler);
}

static NVNINLINE NVNboolean nvnSamplerCompare(const NVNsampler *sampler, const NVNsampler *otherSampler)
{
    return pfnc_nvnSamplerCompare(sampler, otherSampler);
}

static NVNINLINE uint64_t nvnSamplerGetDebugID(const NVNsampler *sampler)
{
    return pfnc_nvnSamplerGetDebugID(sampler);
}

static NVNINLINE void nvnBlendStateSetDefaults(NVNblendState *blend)
{
    pfnc_nvnBlendStateSetDefaults(blend);
}

static NVNINLINE void nvnBlendStateSetBlendTarget(NVNblendState *blend, int target)
{
    pfnc_nvnBlendStateSetBlendTarget(blend, target);
}

static NVNINLINE void nvnBlendStateSetBlendFunc(NVNblendState *blend, NVNblendFunc srcFunc, NVNblendFunc dstFunc, NVNblendFunc srcFuncAlpha, NVNblendFunc dstFuncAlpha)
{
    pfnc_nvnBlendStateSetBlendFunc(blend, srcFunc, dstFunc, srcFuncAlpha, dstFuncAlpha);
}

static NVNINLINE void nvnBlendStateSetBlendEquation(NVNblendState *blend, NVNblendEquation modeRGB, NVNblendEquation modeAlpha)
{
    pfnc_nvnBlendStateSetBlendEquation(blend, modeRGB, modeAlpha);
}

static NVNINLINE void nvnBlendStateSetAdvancedMode(NVNblendState *blend, NVNblendAdvancedMode overlap)
{
    pfnc_nvnBlendStateSetAdvancedMode(blend, overlap);
}

static NVNINLINE void nvnBlendStateSetAdvancedOverlap(NVNblendState *blend, NVNblendAdvancedOverlap overlap)
{
    pfnc_nvnBlendStateSetAdvancedOverlap(blend, overlap);
}

static NVNINLINE void nvnBlendStateSetAdvancedPremultipliedSrc(NVNblendState *blend, NVNboolean premultiplied)
{
    pfnc_nvnBlendStateSetAdvancedPremultipliedSrc(blend, premultiplied);
}

static NVNINLINE void nvnBlendStateSetAdvancedNormalizedDst(NVNblendState *blend, NVNboolean normalized)
{
    pfnc_nvnBlendStateSetAdvancedNormalizedDst(blend, normalized);
}

static NVNINLINE int nvnBlendStateGetBlendTarget(const NVNblendState *blend)
{
    return pfnc_nvnBlendStateGetBlendTarget(blend);
}

static NVNINLINE void nvnBlendStateGetBlendFunc(const NVNblendState *blend, NVNblendFunc *srcFunc, NVNblendFunc *dstFunc, NVNblendFunc *srcFuncAlpha, NVNblendFunc *dstFuncAlpha)
{
    pfnc_nvnBlendStateGetBlendFunc(blend, srcFunc, dstFunc, srcFuncAlpha, dstFuncAlpha);
}

static NVNINLINE void nvnBlendStateGetBlendEquation(const NVNblendState *blend, NVNblendEquation *modeRGB, NVNblendEquation *modeAlpha)
{
    pfnc_nvnBlendStateGetBlendEquation(blend, modeRGB, modeAlpha);
}

static NVNINLINE NVNblendAdvancedMode nvnBlendStateGetAdvancedMode(const NVNblendState *blend)
{
    return pfnc_nvnBlendStateGetAdvancedMode(blend);
}

static NVNINLINE NVNblendAdvancedOverlap nvnBlendStateGetAdvancedOverlap(const NVNblendState *blend)
{
    return pfnc_nvnBlendStateGetAdvancedOverlap(blend);
}

static NVNINLINE NVNboolean nvnBlendStateGetAdvancedPremultipliedSrc(const NVNblendState *blend)
{
    return pfnc_nvnBlendStateGetAdvancedPremultipliedSrc(blend);
}

static NVNINLINE NVNboolean nvnBlendStateGetAdvancedNormalizedDst(const NVNblendState *blend)
{
    return pfnc_nvnBlendStateGetAdvancedNormalizedDst(blend);
}

static NVNINLINE void nvnColorStateSetDefaults(NVNcolorState *color)
{
    pfnc_nvnColorStateSetDefaults(color);
}

static NVNINLINE void nvnColorStateSetBlendEnable(NVNcolorState *color, int index, NVNboolean enable)
{
    pfnc_nvnColorStateSetBlendEnable(color, index, enable);
}

static NVNINLINE void nvnColorStateSetLogicOp(NVNcolorState *color, NVNlogicOp logicOp)
{
    pfnc_nvnColorStateSetLogicOp(color, logicOp);
}

static NVNINLINE void nvnColorStateSetAlphaTest(NVNcolorState *color, NVNalphaFunc alphaTest)
{
    pfnc_nvnColorStateSetAlphaTest(color, alphaTest);
}

static NVNINLINE NVNboolean nvnColorStateGetBlendEnable(const NVNcolorState *color, int index)
{
    return pfnc_nvnColorStateGetBlendEnable(color, index);
}

static NVNINLINE NVNlogicOp nvnColorStateGetLogicOp(const NVNcolorState *color)
{
    return pfnc_nvnColorStateGetLogicOp(color);
}

static NVNINLINE NVNalphaFunc nvnColorStateGetAlphaTest(const NVNcolorState *color)
{
    return pfnc_nvnColorStateGetAlphaTest(color);
}

static NVNINLINE void nvnChannelMaskStateSetDefaults(NVNchannelMaskState *channelMask)
{
    pfnc_nvnChannelMaskStateSetDefaults(channelMask);
}

static NVNINLINE void nvnChannelMaskStateSetChannelMask(NVNchannelMaskState *channelMask, int index, NVNboolean r, NVNboolean g, NVNboolean b, NVNboolean a)
{
    pfnc_nvnChannelMaskStateSetChannelMask(channelMask, index, r, g, b, a);
}

static NVNINLINE void nvnChannelMaskStateGetChannelMask(const NVNchannelMaskState *channelMask, int index, NVNboolean *r, NVNboolean *g, NVNboolean *b, NVNboolean *a)
{
    pfnc_nvnChannelMaskStateGetChannelMask(channelMask, index, r, g, b, a);
}

static NVNINLINE void nvnMultisampleStateSetDefaults(NVNmultisampleState *multisample)
{
    pfnc_nvnMultisampleStateSetDefaults(multisample);
}

static NVNINLINE void nvnMultisampleStateSetMultisampleEnable(NVNmultisampleState *multisample, NVNboolean enable)
{
    pfnc_nvnMultisampleStateSetMultisampleEnable(multisample, enable);
}

static NVNINLINE void nvnMultisampleStateSetSamples(NVNmultisampleState *multisample, int samples)
{
    pfnc_nvnMultisampleStateSetSamples(multisample, samples);
}

static NVNINLINE void nvnMultisampleStateSetAlphaToCoverageEnable(NVNmultisampleState *multisample, NVNboolean enable)
{
    pfnc_nvnMultisampleStateSetAlphaToCoverageEnable(multisample, enable);
}

static NVNINLINE void nvnMultisampleStateSetAlphaToCoverageDither(NVNmultisampleState *multisample, NVNboolean dither)
{
    pfnc_nvnMultisampleStateSetAlphaToCoverageDither(multisample, dither);
}

static NVNINLINE NVNboolean nvnMultisampleStateGetMultisampleEnable(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetMultisampleEnable(multisample);
}

static NVNINLINE int nvnMultisampleStateGetSamples(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetSamples(multisample);
}

static NVNINLINE NVNboolean nvnMultisampleStateGetAlphaToCoverageEnable(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetAlphaToCoverageEnable(multisample);
}

static NVNINLINE NVNboolean nvnMultisampleStateGetAlphaToCoverageDither(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetAlphaToCoverageDither(multisample);
}

static NVNINLINE void nvnMultisampleStateSetRasterSamples(NVNmultisampleState *multisample, int rasterSamples)
{
    pfnc_nvnMultisampleStateSetRasterSamples(multisample, rasterSamples);
}

static NVNINLINE int nvnMultisampleStateGetRasterSamples(NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetRasterSamples(multisample);
}

static NVNINLINE void nvnMultisampleStateSetCoverageModulationMode(NVNmultisampleState *multisample, NVNcoverageModulationMode mode)
{
    pfnc_nvnMultisampleStateSetCoverageModulationMode(multisample, mode);
}

static NVNINLINE NVNcoverageModulationMode nvnMultisampleStateGetCoverageModulationMode(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetCoverageModulationMode(multisample);
}

static NVNINLINE void nvnMultisampleStateSetCoverageToColorEnable(NVNmultisampleState *multisample, NVNboolean enable)
{
    pfnc_nvnMultisampleStateSetCoverageToColorEnable(multisample, enable);
}

static NVNINLINE NVNboolean nvnMultisampleStateGetCoverageToColorEnable(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetCoverageToColorEnable(multisample);
}

static NVNINLINE void nvnMultisampleStateSetCoverageToColorOutput(NVNmultisampleState *multisample, int color)
{
    pfnc_nvnMultisampleStateSetCoverageToColorOutput(multisample, color);
}

static NVNINLINE int nvnMultisampleStateGetCoverageToColorOutput(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetCoverageToColorOutput(multisample);
}

static NVNINLINE void nvnMultisampleStateSetSampleLocationsEnable(NVNmultisampleState *multisample, NVNboolean enable)
{
    pfnc_nvnMultisampleStateSetSampleLocationsEnable(multisample, enable);
}

static NVNINLINE NVNboolean nvnMultisampleStateGetSampleLocationsEnable(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetSampleLocationsEnable(multisample);
}

static NVNINLINE void nvnMultisampleStateGetSampleLocationsGrid(NVNmultisampleState *multisample, int *width, int *height)
{
    pfnc_nvnMultisampleStateGetSampleLocationsGrid(multisample, width, height);
}

static NVNINLINE void nvnMultisampleStateSetSampleLocationsGridEnable(NVNmultisampleState *multisample, NVNboolean enable)
{
    pfnc_nvnMultisampleStateSetSampleLocationsGridEnable(multisample, enable);
}

static NVNINLINE NVNboolean nvnMultisampleStateGetSampleLocationsGridEnable(const NVNmultisampleState *multisample)
{
    return pfnc_nvnMultisampleStateGetSampleLocationsGridEnable(multisample);
}

static NVNINLINE void nvnMultisampleStateSetSampleLocations(NVNmultisampleState *multisample, int start, int count, const float *values)
{
    pfnc_nvnMultisampleStateSetSampleLocations(multisample, start, count, values);
}

static NVNINLINE void nvnPolygonStateSetDefaults(NVNpolygonState *polygon)
{
    pfnc_nvnPolygonStateSetDefaults(polygon);
}

static NVNINLINE void nvnPolygonStateSetCullFace(NVNpolygonState *polygon, NVNface face)
{
    pfnc_nvnPolygonStateSetCullFace(polygon, face);
}

static NVNINLINE void nvnPolygonStateSetFrontFace(NVNpolygonState *polygon, NVNfrontFace face)
{
    pfnc_nvnPolygonStateSetFrontFace(polygon, face);
}

static NVNINLINE void nvnPolygonStateSetPolygonMode(NVNpolygonState *polygon, NVNpolygonMode polygonMode)
{
    pfnc_nvnPolygonStateSetPolygonMode(polygon, polygonMode);
}

static NVNINLINE void nvnPolygonStateSetPolygonOffsetEnables(NVNpolygonState *polygon, int enables)
{
    pfnc_nvnPolygonStateSetPolygonOffsetEnables(polygon, enables);
}

static NVNINLINE NVNface nvnPolygonStateGetCullFace(const NVNpolygonState *polygon)
{
    return pfnc_nvnPolygonStateGetCullFace(polygon);
}

static NVNINLINE NVNfrontFace nvnPolygonStateGetFrontFace(const NVNpolygonState *polygon)
{
    return pfnc_nvnPolygonStateGetFrontFace(polygon);
}

static NVNINLINE NVNpolygonMode nvnPolygonStateGetPolygonMode(const NVNpolygonState *polygon)
{
    return pfnc_nvnPolygonStateGetPolygonMode(polygon);
}

static NVNINLINE NVNpolygonOffsetEnable nvnPolygonStateGetPolygonOffsetEnables(const NVNpolygonState *polygon)
{
    return pfnc_nvnPolygonStateGetPolygonOffsetEnables(polygon);
}

static NVNINLINE void nvnDepthStencilStateSetDefaults(NVNdepthStencilState *depthStencil)
{
    pfnc_nvnDepthStencilStateSetDefaults(depthStencil);
}

static NVNINLINE void nvnDepthStencilStateSetDepthTestEnable(NVNdepthStencilState *depthStencil, NVNboolean enable)
{
    pfnc_nvnDepthStencilStateSetDepthTestEnable(depthStencil, enable);
}

static NVNINLINE void nvnDepthStencilStateSetDepthWriteEnable(NVNdepthStencilState *depthStencil, NVNboolean enable)
{
    pfnc_nvnDepthStencilStateSetDepthWriteEnable(depthStencil, enable);
}

static NVNINLINE void nvnDepthStencilStateSetDepthFunc(NVNdepthStencilState *depthStencil, NVNdepthFunc func)
{
    pfnc_nvnDepthStencilStateSetDepthFunc(depthStencil, func);
}

static NVNINLINE void nvnDepthStencilStateSetStencilTestEnable(NVNdepthStencilState *depthStencil, NVNboolean enable)
{
    pfnc_nvnDepthStencilStateSetStencilTestEnable(depthStencil, enable);
}

static NVNINLINE void nvnDepthStencilStateSetStencilFunc(NVNdepthStencilState *depthStencil, NVNface faces, NVNstencilFunc func)
{
    pfnc_nvnDepthStencilStateSetStencilFunc(depthStencil, faces, func);
}

static NVNINLINE void nvnDepthStencilStateSetStencilOp(NVNdepthStencilState *depthStencil, NVNface faces, NVNstencilOp fail, NVNstencilOp depthFail, NVNstencilOp depthPass)
{
    pfnc_nvnDepthStencilStateSetStencilOp(depthStencil, faces, fail, depthFail, depthPass);
}

static NVNINLINE NVNboolean nvnDepthStencilStateGetDepthTestEnable(const NVNdepthStencilState *depthStencil)
{
    return pfnc_nvnDepthStencilStateGetDepthTestEnable(depthStencil);
}

static NVNINLINE NVNboolean nvnDepthStencilStateGetDepthWriteEnable(const NVNdepthStencilState *depthStencil)
{
    return pfnc_nvnDepthStencilStateGetDepthWriteEnable(depthStencil);
}

static NVNINLINE NVNdepthFunc nvnDepthStencilStateGetDepthFunc(const NVNdepthStencilState *depthStencil)
{
    return pfnc_nvnDepthStencilStateGetDepthFunc(depthStencil);
}

static NVNINLINE NVNboolean nvnDepthStencilStateGetStencilTestEnable(const NVNdepthStencilState *depthStencil)
{
    return pfnc_nvnDepthStencilStateGetStencilTestEnable(depthStencil);
}

static NVNINLINE NVNstencilFunc nvnDepthStencilStateGetStencilFunc(const NVNdepthStencilState *depthStencil, NVNface face)
{
    return pfnc_nvnDepthStencilStateGetStencilFunc(depthStencil, face);
}

static NVNINLINE void nvnDepthStencilStateGetStencilOp(const NVNdepthStencilState *depthStencil, NVNface face, NVNstencilOp *fail, NVNstencilOp *depthFail, NVNstencilOp *depthPass)
{
    pfnc_nvnDepthStencilStateGetStencilOp(depthStencil, face, fail, depthFail, depthPass);
}

static NVNINLINE void nvnVertexAttribStateSetDefaults(NVNvertexAttribState *attrib)
{
    pfnc_nvnVertexAttribStateSetDefaults(attrib);
}

static NVNINLINE void nvnVertexAttribStateSetFormat(NVNvertexAttribState *attrib, NVNformat format, ptrdiff_t relativeOffset)
{
    pfnc_nvnVertexAttribStateSetFormat(attrib, format, relativeOffset);
}

static NVNINLINE void nvnVertexAttribStateSetStreamIndex(NVNvertexAttribState *attrib, int streamIndex)
{
    pfnc_nvnVertexAttribStateSetStreamIndex(attrib, streamIndex);
}

static NVNINLINE void nvnVertexAttribStateGetFormat(const NVNvertexAttribState *attrib, NVNformat *format, ptrdiff_t *relativeOffset)
{
    pfnc_nvnVertexAttribStateGetFormat(attrib, format, relativeOffset);
}

static NVNINLINE int nvnVertexAttribStateGetStreamIndex(const NVNvertexAttribState *attrib)
{
    return pfnc_nvnVertexAttribStateGetStreamIndex(attrib);
}

static NVNINLINE void nvnVertexStreamStateSetDefaults(NVNvertexStreamState *stream)
{
    pfnc_nvnVertexStreamStateSetDefaults(stream);
}

static NVNINLINE void nvnVertexStreamStateSetStride(NVNvertexStreamState *stream, ptrdiff_t stride)
{
    pfnc_nvnVertexStreamStateSetStride(stream, stride);
}

static NVNINLINE void nvnVertexStreamStateSetDivisor(NVNvertexStreamState *stream, int divisor)
{
    pfnc_nvnVertexStreamStateSetDivisor(stream, divisor);
}

static NVNINLINE ptrdiff_t nvnVertexStreamStateGetStride(const NVNvertexStreamState *stream)
{
    return pfnc_nvnVertexStreamStateGetStride(stream);
}

static NVNINLINE int nvnVertexStreamStateGetDivisor(const NVNvertexStreamState *stream)
{
    return pfnc_nvnVertexStreamStateGetDivisor(stream);
}

static NVNINLINE NVNboolean nvnCommandBufferInitialize(NVNcommandBuffer *cmdBuf, NVNdevice *device)
{
    return pfnc_nvnCommandBufferInitialize(cmdBuf, device);
}

static NVNINLINE void nvnCommandBufferFinalize(NVNcommandBuffer *cmdBuf)
{
    pfnc_nvnCommandBufferFinalize(cmdBuf);
}

static NVNINLINE void nvnCommandBufferSetDebugLabel(NVNcommandBuffer *cmdBuf, const char * label)
{
    pfnc_nvnCommandBufferSetDebugLabel(cmdBuf, label);
}

static NVNINLINE void nvnCommandBufferSetMemoryCallback(NVNcommandBuffer *cmdBuf, PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC callback)
{
    pfnc_nvnCommandBufferSetMemoryCallback(cmdBuf, callback);
}

static NVNINLINE void nvnCommandBufferSetMemoryCallbackData(NVNcommandBuffer *cmdBuf, void *callbackData)
{
    pfnc_nvnCommandBufferSetMemoryCallbackData(cmdBuf, callbackData);
}

static NVNINLINE void nvnCommandBufferSetCommandMemoryCallbackEnabled(NVNcommandBuffer *cmdBuf, NVNboolean enabled)
{
    pfnc_nvnCommandBufferSetCommandMemoryCallbackEnabled(cmdBuf, enabled);
}

static NVNINLINE void nvnCommandBufferAddCommandMemory(NVNcommandBuffer *cmdBuf, const NVNmemoryPool *pool, ptrdiff_t offset, size_t size)
{
    pfnc_nvnCommandBufferAddCommandMemory(cmdBuf, pool, offset, size);
}

static NVNINLINE void nvnCommandBufferAddControlMemory(NVNcommandBuffer *cmdBuf, void *memory, size_t size)
{
    pfnc_nvnCommandBufferAddControlMemory(cmdBuf, memory, size);
}

static NVNINLINE size_t nvnCommandBufferGetCommandMemorySize(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetCommandMemorySize(cmdBuf);
}

static NVNINLINE size_t nvnCommandBufferGetCommandMemoryUsed(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetCommandMemoryUsed(cmdBuf);
}

static NVNINLINE size_t nvnCommandBufferGetCommandMemoryFree(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetCommandMemoryFree(cmdBuf);
}

static NVNINLINE size_t nvnCommandBufferGetControlMemorySize(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetControlMemorySize(cmdBuf);
}

static NVNINLINE size_t nvnCommandBufferGetControlMemoryUsed(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetControlMemoryUsed(cmdBuf);
}

static NVNINLINE size_t nvnCommandBufferGetControlMemoryFree(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetControlMemoryFree(cmdBuf);
}

static NVNINLINE void nvnCommandBufferBeginRecording(NVNcommandBuffer *cmdBuf)
{
    pfnc_nvnCommandBufferBeginRecording(cmdBuf);
}

static NVNINLINE NVNcommandHandle nvnCommandBufferEndRecording(NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferEndRecording(cmdBuf);
}

static NVNINLINE void nvnCommandBufferCallCommands(NVNcommandBuffer *cmdBuf, int numCommands, const NVNcommandHandle *handles)
{
    pfnc_nvnCommandBufferCallCommands(cmdBuf, numCommands, handles);
}

static NVNINLINE void nvnCommandBufferCopyCommands(NVNcommandBuffer *cmdBuf, int numCommands, const NVNcommandHandle *handles)
{
    pfnc_nvnCommandBufferCopyCommands(cmdBuf, numCommands, handles);
}

static NVNINLINE void nvnCommandBufferBindBlendState(NVNcommandBuffer *cmdBuf, const NVNblendState *blend)
{
    pfnc_nvnCommandBufferBindBlendState(cmdBuf, blend);
}

static NVNINLINE void nvnCommandBufferBindChannelMaskState(NVNcommandBuffer *cmdBuf, const NVNchannelMaskState *channelMask)
{
    pfnc_nvnCommandBufferBindChannelMaskState(cmdBuf, channelMask);
}

static NVNINLINE void nvnCommandBufferBindColorState(NVNcommandBuffer *cmdBuf, const NVNcolorState *color)
{
    pfnc_nvnCommandBufferBindColorState(cmdBuf, color);
}

static NVNINLINE void nvnCommandBufferBindMultisampleState(NVNcommandBuffer *cmdBuf, const NVNmultisampleState *multisample)
{
    pfnc_nvnCommandBufferBindMultisampleState(cmdBuf, multisample);
}

static NVNINLINE void nvnCommandBufferBindPolygonState(NVNcommandBuffer *cmdBuf, const NVNpolygonState *polygon)
{
    pfnc_nvnCommandBufferBindPolygonState(cmdBuf, polygon);
}

static NVNINLINE void nvnCommandBufferBindDepthStencilState(NVNcommandBuffer *cmdBuf, const NVNdepthStencilState *depthStencil)
{
    pfnc_nvnCommandBufferBindDepthStencilState(cmdBuf, depthStencil);
}

static NVNINLINE void nvnCommandBufferBindVertexAttribState(NVNcommandBuffer *cmdBuf, int numAttribs, const NVNvertexAttribState *attribs)
{
    pfnc_nvnCommandBufferBindVertexAttribState(cmdBuf, numAttribs, attribs);
}

static NVNINLINE void nvnCommandBufferBindVertexStreamState(NVNcommandBuffer *cmdBuf, int numStreams, const NVNvertexStreamState *streams)
{
    pfnc_nvnCommandBufferBindVertexStreamState(cmdBuf, numStreams, streams);
}

static NVNINLINE void nvnCommandBufferBindProgram(NVNcommandBuffer *cmdBuf, const NVNprogram *program, int stages)
{
    pfnc_nvnCommandBufferBindProgram(cmdBuf, program, stages);
}

static NVNINLINE void nvnCommandBufferBindVertexBuffer(NVNcommandBuffer *cmdBuf, int index, NVNbufferAddress buffer, size_t size)
{
    pfnc_nvnCommandBufferBindVertexBuffer(cmdBuf, index, buffer, size);
}

static NVNINLINE void nvnCommandBufferBindVertexBuffers(NVNcommandBuffer *cmdBuf, int first, int count, const NVNbufferRange *buffers)
{
    pfnc_nvnCommandBufferBindVertexBuffers(cmdBuf, first, count, buffers);
}

static NVNINLINE void nvnCommandBufferBindUniformBuffer(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNbufferAddress buffer, size_t size)
{
    pfnc_nvnCommandBufferBindUniformBuffer(cmdBuf, stage, index, buffer, size);
}

static NVNINLINE void nvnCommandBufferBindUniformBuffers(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNbufferRange *buffers)
{
    pfnc_nvnCommandBufferBindUniformBuffers(cmdBuf, stage, first, count, buffers);
}

static NVNINLINE void nvnCommandBufferBindTransformFeedbackBuffer(NVNcommandBuffer *cmdBuf, int index, NVNbufferAddress buffer, size_t size)
{
    pfnc_nvnCommandBufferBindTransformFeedbackBuffer(cmdBuf, index, buffer, size);
}

static NVNINLINE void nvnCommandBufferBindTransformFeedbackBuffers(NVNcommandBuffer *cmdBuf, int first, int count, const NVNbufferRange *buffers)
{
    pfnc_nvnCommandBufferBindTransformFeedbackBuffers(cmdBuf, first, count, buffers);
}

static NVNINLINE void nvnCommandBufferBindStorageBuffer(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNbufferAddress buffer, size_t size)
{
    pfnc_nvnCommandBufferBindStorageBuffer(cmdBuf, stage, index, buffer, size);
}

static NVNINLINE void nvnCommandBufferBindStorageBuffers(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNbufferRange *buffers)
{
    pfnc_nvnCommandBufferBindStorageBuffers(cmdBuf, stage, first, count, buffers);
}

static NVNINLINE void nvnCommandBufferBindTexture(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNtextureHandle texture)
{
    pfnc_nvnCommandBufferBindTexture(cmdBuf, stage, index, texture);
}

static NVNINLINE void nvnCommandBufferBindTextures(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNtextureHandle *textures)
{
    pfnc_nvnCommandBufferBindTextures(cmdBuf, stage, first, count, textures);
}

static NVNINLINE void nvnCommandBufferBindImage(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNimageHandle image)
{
    pfnc_nvnCommandBufferBindImage(cmdBuf, stage, index, image);
}

static NVNINLINE void nvnCommandBufferBindImages(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNimageHandle *images)
{
    pfnc_nvnCommandBufferBindImages(cmdBuf, stage, first, count, images);
}

static NVNINLINE void nvnCommandBufferSetPatchSize(NVNcommandBuffer *cmdBuf, int size)
{
    pfnc_nvnCommandBufferSetPatchSize(cmdBuf, size);
}

static NVNINLINE void nvnCommandBufferSetInnerTessellationLevels(NVNcommandBuffer *cmdBuf, const float *levels)
{
    pfnc_nvnCommandBufferSetInnerTessellationLevels(cmdBuf, levels);
}

static NVNINLINE void nvnCommandBufferSetOuterTessellationLevels(NVNcommandBuffer *cmdBuf, const float *levels)
{
    pfnc_nvnCommandBufferSetOuterTessellationLevels(cmdBuf, levels);
}

static NVNINLINE void nvnCommandBufferSetPrimitiveRestart(NVNcommandBuffer *cmdBuf, NVNboolean enable, int index)
{
    pfnc_nvnCommandBufferSetPrimitiveRestart(cmdBuf, enable, index);
}

static NVNINLINE void nvnCommandBufferBeginTransformFeedback(NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferBeginTransformFeedback(cmdBuf, buffer);
}

static NVNINLINE void nvnCommandBufferEndTransformFeedback(NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferEndTransformFeedback(cmdBuf, buffer);
}

static NVNINLINE void nvnCommandBufferPauseTransformFeedback(NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferPauseTransformFeedback(cmdBuf, buffer);
}

static NVNINLINE void nvnCommandBufferResumeTransformFeedback(NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferResumeTransformFeedback(cmdBuf, buffer);
}

static NVNINLINE void nvnCommandBufferDrawTransformFeedback(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferDrawTransformFeedback(cmdBuf, mode, buffer);
}

static NVNINLINE void nvnCommandBufferDrawArrays(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, int first, int count)
{
    pfnc_nvnCommandBufferDrawArrays(cmdBuf, mode, first, count);
}

static NVNINLINE void nvnCommandBufferDrawElements(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, int count, NVNbufferAddress indexBuffer)
{
    pfnc_nvnCommandBufferDrawElements(cmdBuf, mode, type, count, indexBuffer);
}

static NVNINLINE void nvnCommandBufferDrawElementsBaseVertex(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, int count, NVNbufferAddress indexBuffer, int baseVertex)
{
    pfnc_nvnCommandBufferDrawElementsBaseVertex(cmdBuf, mode, type, count, indexBuffer, baseVertex);
}

static NVNINLINE void nvnCommandBufferDrawArraysInstanced(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, int first, int count, int baseInstance, int instanceCount)
{
    pfnc_nvnCommandBufferDrawArraysInstanced(cmdBuf, mode, first, count, baseInstance, instanceCount);
}

static NVNINLINE void nvnCommandBufferDrawElementsInstanced(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, int count, NVNbufferAddress indexBuffer, int baseVertex, int baseInstance, int instanceCount)
{
    pfnc_nvnCommandBufferDrawElementsInstanced(cmdBuf, mode, type, count, indexBuffer, baseVertex, baseInstance, instanceCount);
}

static NVNINLINE void nvnCommandBufferDrawArraysIndirect(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNbufferAddress indirectBuffer)
{
    pfnc_nvnCommandBufferDrawArraysIndirect(cmdBuf, mode, indirectBuffer);
}

static NVNINLINE void nvnCommandBufferDrawElementsIndirect(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, NVNbufferAddress indexBuffer, NVNbufferAddress indirectBuffer)
{
    pfnc_nvnCommandBufferDrawElementsIndirect(cmdBuf, mode, type, indexBuffer, indirectBuffer);
}

static NVNINLINE void nvnCommandBufferMultiDrawArraysIndirectCount(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNbufferAddress indirectBuffer, NVNbufferAddress parameterBuffer, int maxDrawCount, ptrdiff_t stride)
{
    pfnc_nvnCommandBufferMultiDrawArraysIndirectCount(cmdBuf, mode, indirectBuffer, parameterBuffer, maxDrawCount, stride);
}

static NVNINLINE void nvnCommandBufferMultiDrawElementsIndirectCount(NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, NVNbufferAddress indexBuffer, NVNbufferAddress indirectBuffer, NVNbufferAddress parameterBuffer, int maxDrawCount, ptrdiff_t stride)
{
    pfnc_nvnCommandBufferMultiDrawElementsIndirectCount(cmdBuf, mode, type, indexBuffer, indirectBuffer, parameterBuffer, maxDrawCount, stride);
}

static NVNINLINE void nvnCommandBufferClearColor(NVNcommandBuffer *cmdBuf, int index, const float *color, int mask)
{
    pfnc_nvnCommandBufferClearColor(cmdBuf, index, color, mask);
}

static NVNINLINE void nvnCommandBufferClearColori(NVNcommandBuffer *cmdBuf, int index, const int *color, int mask)
{
    pfnc_nvnCommandBufferClearColori(cmdBuf, index, color, mask);
}

static NVNINLINE void nvnCommandBufferClearColorui(NVNcommandBuffer *cmdBuf, int index, const uint32_t *color, int mask)
{
    pfnc_nvnCommandBufferClearColorui(cmdBuf, index, color, mask);
}

static NVNINLINE void nvnCommandBufferClearDepthStencil(NVNcommandBuffer *cmdBuf, float depthValue, NVNboolean depthMask, int stencilValue, int stencilMask)
{
    pfnc_nvnCommandBufferClearDepthStencil(cmdBuf, depthValue, depthMask, stencilValue, stencilMask);
}

static NVNINLINE void nvnCommandBufferDispatchCompute(NVNcommandBuffer *cmdBuf, int groupsX, int groupsY, int groupsZ)
{
    pfnc_nvnCommandBufferDispatchCompute(cmdBuf, groupsX, groupsY, groupsZ);
}

static NVNINLINE void nvnCommandBufferDispatchComputeIndirect(NVNcommandBuffer *cmdBuf, NVNbufferAddress indirectBuffer)
{
    pfnc_nvnCommandBufferDispatchComputeIndirect(cmdBuf, indirectBuffer);
}

static NVNINLINE void nvnCommandBufferSetViewport(NVNcommandBuffer *cmdBuf, int x, int y, int w, int h)
{
    pfnc_nvnCommandBufferSetViewport(cmdBuf, x, y, w, h);
}

static NVNINLINE void nvnCommandBufferSetViewports(NVNcommandBuffer *cmdBuf, int first, int count, const float *ranges)
{
    pfnc_nvnCommandBufferSetViewports(cmdBuf, first, count, ranges);
}

static NVNINLINE void nvnCommandBufferSetViewportSwizzles(NVNcommandBuffer *cmdBuf, int first, int count, const NVNviewportSwizzle *swizzles)
{
    pfnc_nvnCommandBufferSetViewportSwizzles(cmdBuf, first, count, swizzles);
}

static NVNINLINE void nvnCommandBufferSetScissor(NVNcommandBuffer *cmdBuf, int x, int y, int w, int h)
{
    pfnc_nvnCommandBufferSetScissor(cmdBuf, x, y, w, h);
}

static NVNINLINE void nvnCommandBufferSetScissors(NVNcommandBuffer *cmdBuf, int first, int count, const int *rects)
{
    pfnc_nvnCommandBufferSetScissors(cmdBuf, first, count, rects);
}

static NVNINLINE void nvnCommandBufferSetDepthRange(NVNcommandBuffer *cmdBuf, float n, float f)
{
    pfnc_nvnCommandBufferSetDepthRange(cmdBuf, n, f);
}

static NVNINLINE void nvnCommandBufferSetDepthBounds(NVNcommandBuffer *cmdBuf, NVNboolean enable, float n, float f)
{
    pfnc_nvnCommandBufferSetDepthBounds(cmdBuf, enable, n, f);
}

static NVNINLINE void nvnCommandBufferSetDepthRanges(NVNcommandBuffer *cmdBuf, int first, int count, const float *ranges)
{
    pfnc_nvnCommandBufferSetDepthRanges(cmdBuf, first, count, ranges);
}

static NVNINLINE void nvnCommandBufferSetTiledCacheAction(NVNcommandBuffer *cmdBuf, NVNtiledCacheAction action)
{
    pfnc_nvnCommandBufferSetTiledCacheAction(cmdBuf, action);
}

static NVNINLINE void nvnCommandBufferSetTiledCacheTileSize(NVNcommandBuffer *cmdBuf, int tileWidth, int tileHeight)
{
    pfnc_nvnCommandBufferSetTiledCacheTileSize(cmdBuf, tileWidth, tileHeight);
}

static NVNINLINE void nvnCommandBufferBindSeparateTexture(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNseparateTextureHandle textureHandle)
{
    pfnc_nvnCommandBufferBindSeparateTexture(cmdBuf, stage, index, textureHandle);
}

static NVNINLINE void nvnCommandBufferBindSeparateSampler(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNseparateSamplerHandle samplerHandle)
{
    pfnc_nvnCommandBufferBindSeparateSampler(cmdBuf, stage, index, samplerHandle);
}

static NVNINLINE void nvnCommandBufferBindSeparateTextures(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNseparateTextureHandle *textureHandles)
{
    pfnc_nvnCommandBufferBindSeparateTextures(cmdBuf, stage, first, count, textureHandles);
}

static NVNINLINE void nvnCommandBufferBindSeparateSamplers(NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNseparateSamplerHandle *samplerHandles)
{
    pfnc_nvnCommandBufferBindSeparateSamplers(cmdBuf, stage, first, count, samplerHandles);
}

static NVNINLINE void nvnCommandBufferSetStencilValueMask(NVNcommandBuffer *cmdBuf, NVNface faces, int mask)
{
    pfnc_nvnCommandBufferSetStencilValueMask(cmdBuf, faces, mask);
}

static NVNINLINE void nvnCommandBufferSetStencilMask(NVNcommandBuffer *cmdBuf, NVNface faces, int mask)
{
    pfnc_nvnCommandBufferSetStencilMask(cmdBuf, faces, mask);
}

static NVNINLINE void nvnCommandBufferSetStencilRef(NVNcommandBuffer *cmdBuf, NVNface faces, int ref)
{
    pfnc_nvnCommandBufferSetStencilRef(cmdBuf, faces, ref);
}

static NVNINLINE void nvnCommandBufferSetBlendColor(NVNcommandBuffer *cmdBuf, const float *blendColor)
{
    pfnc_nvnCommandBufferSetBlendColor(cmdBuf, blendColor);
}

static NVNINLINE void nvnCommandBufferSetPointSize(NVNcommandBuffer *cmdBuf, float pointSize)
{
    pfnc_nvnCommandBufferSetPointSize(cmdBuf, pointSize);
}

static NVNINLINE void nvnCommandBufferSetLineWidth(NVNcommandBuffer *cmdBuf, float lineWidth)
{
    pfnc_nvnCommandBufferSetLineWidth(cmdBuf, lineWidth);
}

static NVNINLINE void nvnCommandBufferSetPolygonOffsetClamp(NVNcommandBuffer *cmdBuf, float factor, float units, float clamp)
{
    pfnc_nvnCommandBufferSetPolygonOffsetClamp(cmdBuf, factor, units, clamp);
}

static NVNINLINE void nvnCommandBufferSetAlphaRef(NVNcommandBuffer *cmdBuf, float ref)
{
    pfnc_nvnCommandBufferSetAlphaRef(cmdBuf, ref);
}

static NVNINLINE void nvnCommandBufferSetSampleMask(NVNcommandBuffer *cmdBuf, int mask)
{
    pfnc_nvnCommandBufferSetSampleMask(cmdBuf, mask);
}

static NVNINLINE void nvnCommandBufferSetRasterizerDiscard(NVNcommandBuffer *cmdBuf, NVNboolean discard)
{
    pfnc_nvnCommandBufferSetRasterizerDiscard(cmdBuf, discard);
}

static NVNINLINE void nvnCommandBufferSetDepthClamp(NVNcommandBuffer *cmdBuf, NVNboolean clamp)
{
    pfnc_nvnCommandBufferSetDepthClamp(cmdBuf, clamp);
}

static NVNINLINE void nvnCommandBufferSetConservativeRasterEnable(NVNcommandBuffer *cmdBuf, NVNboolean enable)
{
    pfnc_nvnCommandBufferSetConservativeRasterEnable(cmdBuf, enable);
}

static NVNINLINE void nvnCommandBufferSetConservativeRasterDilate(NVNcommandBuffer *cmdBuf, float dilate)
{
    pfnc_nvnCommandBufferSetConservativeRasterDilate(cmdBuf, dilate);
}

static NVNINLINE void nvnCommandBufferSetSubpixelPrecisionBias(NVNcommandBuffer *cmdBuf, int xBits, int yBits)
{
    pfnc_nvnCommandBufferSetSubpixelPrecisionBias(cmdBuf, xBits, yBits);
}

static NVNINLINE void nvnCommandBufferCopyBufferToTexture(NVNcommandBuffer *cmdBuf, NVNbufferAddress src, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, int flags)
{
    pfnc_nvnCommandBufferCopyBufferToTexture(cmdBuf, src, dstTexture, dstView, dstRegion, flags);
}

static NVNINLINE void nvnCommandBufferCopyTextureToBuffer(NVNcommandBuffer *cmdBuf, const NVNtexture *srcTexture, const NVNtextureView *srcView, const NVNcopyRegion *srcRegion, NVNbufferAddress dst, int flags)
{
    pfnc_nvnCommandBufferCopyTextureToBuffer(cmdBuf, srcTexture, srcView, srcRegion, dst, flags);
}

static NVNINLINE void nvnCommandBufferCopyTextureToTexture(NVNcommandBuffer *cmdBuf, const NVNtexture *srcTexture, const NVNtextureView *srcView, const NVNcopyRegion *srcRegion, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, int flags)
{
    pfnc_nvnCommandBufferCopyTextureToTexture(cmdBuf, srcTexture, srcView, srcRegion, dstTexture, dstView, dstRegion, flags);
}

static NVNINLINE void nvnCommandBufferCopyBufferToBuffer(NVNcommandBuffer *cmdBuf, NVNbufferAddress src, NVNbufferAddress dst, size_t size, int flags)
{
    pfnc_nvnCommandBufferCopyBufferToBuffer(cmdBuf, src, dst, size, flags);
}

static NVNINLINE void nvnCommandBufferClearBuffer(NVNcommandBuffer *cmdBuf, NVNbufferAddress dst, size_t size, uint32_t value)
{
    pfnc_nvnCommandBufferClearBuffer(cmdBuf, dst, size, value);
}

static NVNINLINE void nvnCommandBufferClearTexture(NVNcommandBuffer *cmdBuf, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, const float *color, int mask)
{
    pfnc_nvnCommandBufferClearTexture(cmdBuf, dstTexture, dstView, dstRegion, color, mask);
}

static NVNINLINE void nvnCommandBufferClearTexturei(NVNcommandBuffer *cmdBuf, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, const int *color, int mask)
{
    pfnc_nvnCommandBufferClearTexturei(cmdBuf, dstTexture, dstView, dstRegion, color, mask);
}

static NVNINLINE void nvnCommandBufferClearTextureui(NVNcommandBuffer *cmdBuf, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, const uint32_t *color, int mask)
{
    pfnc_nvnCommandBufferClearTextureui(cmdBuf, dstTexture, dstView, dstRegion, color, mask);
}

static NVNINLINE void nvnCommandBufferUpdateUniformBuffer(NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer, size_t bufferSize, ptrdiff_t updateOffset, size_t updateSize, const void *data)
{
    pfnc_nvnCommandBufferUpdateUniformBuffer(cmdBuf, buffer, bufferSize, updateOffset, updateSize, data);
}

static NVNINLINE void nvnCommandBufferReportCounter(NVNcommandBuffer *cmdBuf, NVNcounterType counter, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferReportCounter(cmdBuf, counter, buffer);
}

static NVNINLINE void nvnCommandBufferResetCounter(NVNcommandBuffer *cmdBuf, NVNcounterType counter)
{
    pfnc_nvnCommandBufferResetCounter(cmdBuf, counter);
}

static NVNINLINE void nvnCommandBufferReportValue(NVNcommandBuffer *cmdBuf, uint32_t value, NVNbufferAddress buffer)
{
    pfnc_nvnCommandBufferReportValue(cmdBuf, value, buffer);
}

static NVNINLINE void nvnCommandBufferSetRenderEnable(NVNcommandBuffer *cmdBuf, NVNboolean enable)
{
    pfnc_nvnCommandBufferSetRenderEnable(cmdBuf, enable);
}

static NVNINLINE void nvnCommandBufferSetRenderEnableConditional(NVNcommandBuffer *cmdBuf, NVNconditionalRenderMode mode, NVNbufferAddress counters)
{
    pfnc_nvnCommandBufferSetRenderEnableConditional(cmdBuf, mode, counters);
}

static NVNINLINE void nvnCommandBufferSetRenderTargets(NVNcommandBuffer *cmdBuf, int numColors, const NVNtexture * const *colors, const NVNtextureView * const *colorViews, const NVNtexture *depthStencil, const NVNtextureView *depthStencilView)
{
    pfnc_nvnCommandBufferSetRenderTargets(cmdBuf, numColors, colors, colorViews, depthStencil, depthStencilView);
}

static NVNINLINE void nvnCommandBufferDiscardColor(NVNcommandBuffer *cmdBuf, int index)
{
    pfnc_nvnCommandBufferDiscardColor(cmdBuf, index);
}

static NVNINLINE void nvnCommandBufferDiscardDepthStencil(NVNcommandBuffer *cmdBuf)
{
    pfnc_nvnCommandBufferDiscardDepthStencil(cmdBuf);
}

static NVNINLINE void nvnCommandBufferDownsample(NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtexture *dst)
{
    pfnc_nvnCommandBufferDownsample(cmdBuf, src, dst);
}

static NVNINLINE void nvnCommandBufferTiledDownsample(NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtexture *dst)
{
    pfnc_nvnCommandBufferTiledDownsample(cmdBuf, src, dst);
}

static NVNINLINE void nvnCommandBufferDownsampleTextureView(NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtextureView *srcView, const NVNtexture *dst, const NVNtextureView *dstView)
{
    pfnc_nvnCommandBufferDownsampleTextureView(cmdBuf, src, srcView, dst, dstView);
}

static NVNINLINE void nvnCommandBufferTiledDownsampleTextureView(NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtextureView *srcView, const NVNtexture *dst, const NVNtextureView *dstView)
{
    pfnc_nvnCommandBufferTiledDownsampleTextureView(cmdBuf, src, srcView, dst, dstView);
}

static NVNINLINE void nvnCommandBufferBarrier(NVNcommandBuffer *cmdBuf, int barrier)
{
    pfnc_nvnCommandBufferBarrier(cmdBuf, barrier);
}

static NVNINLINE void nvnCommandBufferWaitSync(NVNcommandBuffer *cmdBuf, const NVNsync *sync)
{
    pfnc_nvnCommandBufferWaitSync(cmdBuf, sync);
}

static NVNINLINE void nvnCommandBufferFenceSync(NVNcommandBuffer *cmdBuf, NVNsync *sync, NVNsyncCondition condition, int flags)
{
    pfnc_nvnCommandBufferFenceSync(cmdBuf, sync, condition, flags);
}

static NVNINLINE void nvnCommandBufferSetTexturePool(NVNcommandBuffer *cmdBuf, const NVNtexturePool *texturePool)
{
    pfnc_nvnCommandBufferSetTexturePool(cmdBuf, texturePool);
}

static NVNINLINE void nvnCommandBufferSetSamplerPool(NVNcommandBuffer *cmdBuf, const NVNsamplerPool *samplerPool)
{
    pfnc_nvnCommandBufferSetSamplerPool(cmdBuf, samplerPool);
}

static NVNINLINE void nvnCommandBufferSetShaderScratchMemory(NVNcommandBuffer *cmdBuf, const NVNmemoryPool *pool, ptrdiff_t offset, size_t size)
{
    pfnc_nvnCommandBufferSetShaderScratchMemory(cmdBuf, pool, offset, size);
}

static NVNINLINE void nvnCommandBufferSaveZCullData(NVNcommandBuffer *cmdBuf, NVNbufferAddress dst, size_t size)
{
    pfnc_nvnCommandBufferSaveZCullData(cmdBuf, dst, size);
}

static NVNINLINE void nvnCommandBufferRestoreZCullData(NVNcommandBuffer *cmdBuf, NVNbufferAddress src, size_t size)
{
    pfnc_nvnCommandBufferRestoreZCullData(cmdBuf, src, size);
}

static NVNINLINE void nvnCommandBufferSetCopyRowStride(NVNcommandBuffer *cmdBuf, ptrdiff_t stride)
{
    pfnc_nvnCommandBufferSetCopyRowStride(cmdBuf, stride);
}

static NVNINLINE void nvnCommandBufferSetCopyImageStride(NVNcommandBuffer *cmdBuf, ptrdiff_t stride)
{
    pfnc_nvnCommandBufferSetCopyImageStride(cmdBuf, stride);
}

static NVNINLINE ptrdiff_t nvnCommandBufferGetCopyRowStride(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetCopyRowStride(cmdBuf);
}

static NVNINLINE ptrdiff_t nvnCommandBufferGetCopyImageStride(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetCopyImageStride(cmdBuf);
}

static NVNINLINE void nvnCommandBufferDrawTexture(NVNcommandBuffer *cmdBuf, NVNtextureHandle texture, const NVNdrawTextureRegion *dstRegion, const NVNdrawTextureRegion *srcRegion)
{
    pfnc_nvnCommandBufferDrawTexture(cmdBuf, texture, dstRegion, srcRegion);
}

static NVNINLINE NVNboolean nvnProgramSetSubroutineLinkage(NVNprogram *program, int count, const NVNsubroutineLinkageMapPtr *linkageMapPtrs)
{
    return pfnc_nvnProgramSetSubroutineLinkage(program, count, linkageMapPtrs);
}

static NVNINLINE void nvnCommandBufferSetProgramSubroutines(NVNcommandBuffer *cmdBuf, NVNprogram *program, NVNshaderStage stage, const int first, const int count, const int *values)
{
    pfnc_nvnCommandBufferSetProgramSubroutines(cmdBuf, program, stage, first, count, values);
}

static NVNINLINE void nvnCommandBufferBindCoverageModulationTable(NVNcommandBuffer *cmdBuf, const float *entries)
{
    pfnc_nvnCommandBufferBindCoverageModulationTable(cmdBuf, entries);
}

static NVNINLINE void nvnCommandBufferResolveDepthBuffer(NVNcommandBuffer *cmdBuf)
{
    pfnc_nvnCommandBufferResolveDepthBuffer(cmdBuf);
}

static NVNINLINE void nvnCommandBufferSetColorReductionEnable(NVNcommandBuffer *cmdBuf, NVNboolean enable)
{
    pfnc_nvnCommandBufferSetColorReductionEnable(cmdBuf, enable);
}

static NVNINLINE void nvnCommandBufferSetColorReductionThresholds(NVNcommandBuffer *cmdBuf, NVNformatClass formatClass, int thresholdConservative, int thresholdAggressive)
{
    pfnc_nvnCommandBufferSetColorReductionThresholds(cmdBuf, formatClass, thresholdConservative, thresholdAggressive);
}

static NVNINLINE void nvnCommandBufferPushDebugGroupStatic(NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description)
{
    pfnc_nvnCommandBufferPushDebugGroupStatic(cmdBuf, domainId, description);
}

static NVNINLINE void nvnCommandBufferPushDebugGroupDynamic(NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description)
{
    pfnc_nvnCommandBufferPushDebugGroupDynamic(cmdBuf, domainId, description);
}

static NVNINLINE void nvnCommandBufferPushDebugGroup(NVNcommandBuffer *cmdBuf, const char * description)
{
    pfnc_nvnCommandBufferPushDebugGroup(cmdBuf, description);
}

static NVNINLINE void nvnCommandBufferPopDebugGroup(NVNcommandBuffer *cmdBuf)
{
    pfnc_nvnCommandBufferPopDebugGroup(cmdBuf);
}

static NVNINLINE void nvnCommandBufferPopDebugGroupId(NVNcommandBuffer *cmdBuf, uint32_t domainId)
{
    pfnc_nvnCommandBufferPopDebugGroupId(cmdBuf, domainId);
}

static NVNINLINE void nvnCommandBufferInsertDebugMarkerStatic(NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description)
{
    pfnc_nvnCommandBufferInsertDebugMarkerStatic(cmdBuf, domainId, description);
}

static NVNINLINE void nvnCommandBufferInsertDebugMarkerDynamic(NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description)
{
    pfnc_nvnCommandBufferInsertDebugMarkerDynamic(cmdBuf, domainId, description);
}

static NVNINLINE void nvnCommandBufferInsertDebugMarker(NVNcommandBuffer *cmdBuf, const char * description)
{
    pfnc_nvnCommandBufferInsertDebugMarker(cmdBuf, description);
}

static NVNINLINE PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC nvnCommandBufferGetMemoryCallback(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetMemoryCallback(cmdBuf);
}

static NVNINLINE void* nvnCommandBufferGetMemoryCallbackData(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferGetMemoryCallbackData(cmdBuf);
}

static NVNINLINE NVNboolean nvnCommandBufferIsRecording(const NVNcommandBuffer *cmdBuf)
{
    return pfnc_nvnCommandBufferIsRecording(cmdBuf);
}

static NVNINLINE NVNboolean nvnSyncInitialize(NVNsync *sync, NVNdevice *device)
{
    return pfnc_nvnSyncInitialize(sync, device);
}

static NVNINLINE void nvnSyncFinalize(NVNsync *sync)
{
    pfnc_nvnSyncFinalize(sync);
}

static NVNINLINE void nvnSyncSetDebugLabel(NVNsync *sync, const char * label)
{
    pfnc_nvnSyncSetDebugLabel(sync, label);
}

static NVNINLINE void nvnQueueFenceSync(NVNqueue *queue, NVNsync *sync, NVNsyncCondition condition, int flags)
{
    pfnc_nvnQueueFenceSync(queue, sync, condition, flags);
}

static NVNINLINE NVNsyncWaitResult nvnSyncWait(const NVNsync *sync, uint64_t timeoutNs)
{
    return pfnc_nvnSyncWait(sync, timeoutNs);
}

static NVNINLINE NVNboolean nvnQueueWaitSync(NVNqueue *queue, const NVNsync *sync)
{
    return pfnc_nvnQueueWaitSync(queue, sync);
}

static NVNINLINE NVNboolean nvnSyncInitializeFromFencedGLSync(NVNsync *sync, NVNdevice *device, const void *glSync, NVNsyncInteropStatus *status)
{
    return pfnc_nvnSyncInitializeFromFencedGLSync(sync, device, glSync, status);
}

static NVNINLINE NVNsyncInteropStatus nvnSyncCreateGLSync(const NVNsync *sync, uint64_t *glSync)
{
    return pfnc_nvnSyncCreateGLSync(sync, glSync);
}

static NVNINLINE void nvnEventBuilderSetDefaults(NVNeventBuilder *builder)
{
    pfnc_nvnEventBuilderSetDefaults(builder);
}

static NVNINLINE void nvnEventBuilderSetStorage(NVNeventBuilder *builder, const NVNmemoryPool *pool, int64_t offset)
{
    pfnc_nvnEventBuilderSetStorage(builder, pool, offset);
}

static NVNINLINE const NVNmemoryPool * nvnEventBuilderGetStorage(const NVNeventBuilder *builder, int64_t *offset)
{
    return pfnc_nvnEventBuilderGetStorage(builder, offset);
}

static NVNINLINE NVNboolean nvnEventInitialize(NVNevent *event, const NVNeventBuilder *builder)
{
    return pfnc_nvnEventInitialize(event, builder);
}

static NVNINLINE void nvnEventFinalize(NVNevent *event)
{
    pfnc_nvnEventFinalize(event);
}

static NVNINLINE uint32_t nvnEventGetValue(const NVNevent *event)
{
    return pfnc_nvnEventGetValue(event);
}

static NVNINLINE void nvnEventSignal(NVNevent *event, NVNeventSignalMode mode, uint32_t value)
{
    pfnc_nvnEventSignal(event, mode, value);
}

static NVNINLINE void nvnCommandBufferWaitEvent(NVNcommandBuffer *cmdBuf, const NVNevent *event, NVNeventWaitMode waitMode, uint32_t value)
{
    pfnc_nvnCommandBufferWaitEvent(cmdBuf, event, waitMode, value);
}

static NVNINLINE void nvnCommandBufferSignalEvent(NVNcommandBuffer *cmdBuf, const NVNevent *event, NVNeventSignalMode mode, NVNeventSignalLocation location, int flags, uint32_t value)
{
    pfnc_nvnCommandBufferSignalEvent(cmdBuf, event, mode, location, flags, value);
}

static NVNINLINE void nvnCommandBufferSetStencilCullCriteria(NVNcommandBuffer *cmdBuf, NVNstencilFunc func, int ref, int mask)
{
    pfnc_nvnCommandBufferSetStencilCullCriteria(cmdBuf, func, ref, mask);
}


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __nvn_funcptrinline_h_
