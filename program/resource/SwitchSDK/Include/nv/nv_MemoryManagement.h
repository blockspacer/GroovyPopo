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

/**
* @file
* @brief グラフィックスのメモリ管理機能を提供します。
*/

#ifdef __cplusplus
#include <nn/nn_Common.h>


namespace nv {

    /**
     * @brief      A structure for retrieving memory allocation status.
     */
    struct MemoryInfo
    {
        /**
         * @brief  Number of free bytes currently in firmware memory.
         */
        size_t firmwareMemoryFreeSize;

        /**
         * @brief  Largest block that can be currently allocated from firmware memory.
         */
        size_t firmwareMemoryAllocatableSize;

        /**
         * @brief  Number of free bytes in firmware memory when firmware memory usage peaked.
         */
        size_t firmwareMemoryMinimumFreeSize;

        /**
         * @brief  Largest block that could be allocated from firmware memory when firmware
         *         memory usage peaked.
         */
        size_t firmwareMemoryMinimumAllocatableSize;

        /**
         * @brief  Reserved for future use.
         */
        size_t reserved[4];
    };

    /**
     * @brief      A function pointer for allocating memory.
     * @details    Can be passed to SetGraphicsAllocator().
     */
    typedef void* (*AllocateFunction)(size_t size, size_t alignment, void *userPtr);

    /**
     * @brief      A function pointer for freeing memory.
     * @details    Can be passed to SetGraphicsAllocator().
     */
    typedef void  (*FreeFunction)(void *addr, void *userPtr);

    /**
     * @brief      A function pointer for reallocating memory.
     * @details    Can be passed to SetGraphicsAllocator().
     */
    typedef void* (*ReallocateFunction)(void* addr, size_t newSize, void *userPtr);

//! @name Graphics memory management
//! @{

    /**
     * @brief Set memory allocator for graphics subsystem.
     *
     * @param[in]  allocate     A function pointer for allocating memory.
     * @param[in]  free         A function pointer for freeing memory.
     * @param[in]  reallocate   A function pointer for reallocating memory.
     * @param[in]  userPtr      User pointer that will be passed to callbacks.
     *
     * @details
     *   This function must be called before using any graphics APIs.
     *   The relative order of nv::InitializeGraphics, nv::SetGraphicsAllocator and
     *   nv::SetGraphicsDevtoolsAllocator calls does not matter, as long as the application
     *   makes all its calls before using any graphics API's.
     *   Failure to do so will result in the application being aborted.
     */
    void SetGraphicsAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void *userPtr) NN_NOEXCEPT;

    /**
     * @brief Set memory allocator for graphics developer tools and NVN debug layer.
     *
     * @param[in]  allocate     A function pointer for allocating memory.
     * @param[in]  free         A function pointer for freeing memory.
     * @param[in]  reallocate   A function pointer for reallocating memory.
     * @param[in]  userPtr      User pointer that will be passed to callbacks.
     *
     * @details
     *   This function must be called before using any graphics developer features
     *   which can be set by nvn::DeviceFlagBits::DEBUG_ENABLE. The relative order of nv::InitializeGraphics,
     *   nv::SetGraphicsAllocator and nv::SetGraphicsDevtoolsAllocator calls does not matter,
     *   as long as the application makes all its calls before using any graphics API's.
     *   Failure to do so will result in the application being aborted in the future.
     */
    void SetGraphicsDevtoolsAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void *userPtr) NN_NOEXCEPT;

    /**
     * @brief Specify the address and size of memory that is donated to graphics firmware.
     *
     * @details
     *   This function must be called before using any graphics APIs.
     *   The relative order of nv::InitializeGraphics, nv::SetGraphicsAllocator and
     *   nv::SetGraphicsDevtoolsAllocator calls does not matter, as long as the application
     *   makes all its calls before using any graphics API's.
     *   Failure to do so will result in the application being aborted.
     *   The donated memory block will be unmapped from process address space and cannot
     *   be used afterwards.
     *
     *   @param[in]  address    Address of memory being donated to graphics firmware.
     *   @param[in]  size       Size of memory being donated to graphics firmware.
     */
    void InitializeGraphics(void *address, size_t size) NN_NOEXCEPT;

    /**
     * @brief Release resources from graphics subsystem.
     *
     * @details
     *   This function must be called once the client is no longer making use
     *   of the graphics subsystem. It will release the resources in use from
     *   the memory donated in nv::InitializeGraphics(), and also free the
     *   memory received from the allocator previously set up with the
         nv::SetGraphicsAllocator() function.
     */
    void FinalizeGraphics() NN_NOEXCEPT;

    /**
     * @brief Retrieve information about current memory usage status of firmware.
     *
     * @details
     *   The function returns current memory usage status of the memory heap that was
     *   provided for firmware through nv::InitializeGraphics.
     *
     *   @param[out] memoryInfo  A pointer to structure that will receive the info.
     */
    void GetGraphicsMemoryInfo(MemoryInfo *memoryInfo) NN_NOEXCEPT;


    /**
     * @brief Dump detailed information about graphics memory status to log.
     *
     * @details
     *  This method will not print information for SDK users.
     *  To debug memory leaks, consider using nvn::Device::WalkDebugDatabase() instead.
     *  Please see the \ref nvn_guide_api_object_tracking "API Object Tracking" section
     *  of the NVN Programming Guide.
     */
    void DumpGraphicsMemoryInfo() NN_NOEXCEPT;

//! @}

}

#endif
