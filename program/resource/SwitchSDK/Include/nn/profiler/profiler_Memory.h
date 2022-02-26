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
 * @brief Nintendo NX CPU Profiler Memory tracking API functions
 * @details Nintendo NX CPU Profiler API
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace profiler {

/**
 *  @brief Type definition of a tracking id.
 */
typedef uintptr_t TrackingId;


//! @name Memory Tracking API
//! @{


/**
 *  @brief Records a heap name to be used for a given range of addresses.
 *
 *  @param[in] startAddress A pointer to the memory location to associate as the start of the heap.
 *  @param[in] endAddress A pointer to the memory location to associate as the end of the heap.
 *  @param[in] heapName The name to associate with the heap memory range.
 *
 *  @details
 *  This function informs the memory tracking system of the profiler of a name to associate with
 *  addresses that live within a given address range.
 *  When one of the Track functions is called, it will search through the list of registered
 *  heaps in reverse order to find the associated name of the heap.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is not thread-safe.
 *  Care should be used when calling this function to ensure that multiple threads do not attempt
 *  to record heap information at the same time.
 *
 *  @note
 *  With the current implementation, only 5 heaps can be registered.
 *
 *  Example:
 *  @code
 *  nn::lmem::HeapHandle exampleHeap =
 *      nn::lmem::CreateExpHeap(startAddress, size, nn::lmem::CreationOption_NoOption);
 *  void* endAddress = reinterpret_cast<char*>(startAddress) + size;
 *  nn::profiler::RecordHeapRange(startAddress, endAddress, "Example");
 *  @endcode
 */
void RecordHeapRange(
    void const* startAddress,
    void const* endAddress,
    char const* heapName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to malloc has occurred.
 *
 *  @param [in] address The address that was returned by the call to malloc.
 *  @param [in] size The size/amount of memory that was requested to be allocated.
 *
 *  @details
 *  See nn::profiler::TrackMalloc(void const*, size_t, char const*, int).
 */
void TrackMalloc(void const* address, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to malloc has occurred.
 *
 *  @param [in] address The address that was returned by the call to malloc.
 *  @param [in] size The size/amount of memory that was requested to be allocated.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackMalloc(void const*, size_t, char const*, int).
 */
void TrackMalloc(void const* address, size_t size, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to malloc has occurred.
 *
 *  @param [in] address The address that was returned by the call to malloc.
 *  @param [in] size The size/amount of memory that was requested to be allocated.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  This function informs the memory tracking system that a call to malloc has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* MallocMemory(size_t size)
 *  {
 *      void* memory = malloc(size);
 *      nn::profiler::TrackMalloc(memory, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackMalloc(
    void const* address,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to calloc has occurred.
 *
 *  @param [in] address The address that was returned by the call to calloc.
 *  @param [in] num The number of elements that were requested to be allocated by the call to calloc.
 *  @param [in] size The size of each element.
 *
 *  @details
 *  See TrackCalloc(void const*, size_t, size_t, char const*, int).
 */
void TrackCalloc(void const* address, size_t num, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to calloc has occurred.
 *
 *  @param [in] address The address that was returned by the call to calloc.
 *  @param [in] num The number of elements that were requested to be allocated by the call to calloc.
 *  @param [in] size The size of each element.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See TrackCalloc(void const*, size_t, size_t, char const*, int).
 */
void TrackCalloc(void const* address, size_t num, size_t size, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to calloc has occurred.
 *
 *  @param [in] address The address that was returned by the call to calloc.
 *  @param [in] num The number of elements that were requested to be allocated by the call to calloc.
 *  @param [in] size The size of each element.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  This function informs the memory tracking system that a call to calloc has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* CallocMemory(size_t num, size_t size)
 *  {
 *      void* memory = calloc(num, size);
 *      nn::profiler::TrackCalloc(memory, num, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackCalloc(
    void const* address,
    size_t num,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to realloc has occurred.
 *
 *  @param [in] oldAddress The original address of the allocation that was passed to realloc.
 *  @param [in] address The address that was returned by the call to realloc.
 *  @param [in] size The size, in bytes, of the memory requested.
 *
 *  @details
 *  See nn::profiler::TrackRealloc(void const*, void const*, size_t, char const*, int).
 */
void TrackRealloc(void const* oldAddress, void const* address, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to realloc has occurred.
 *
 *  @param [in] oldAddress The original address of the allocation that was passed to realloc.
 *  @param [in] address The address that was returned by the call to realloc.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackRealloc(void const*, void const*, size_t, char const*, int).
 */
void TrackRealloc(
    void const* oldAddress,
    void const* address,
    size_t size,
    char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to realloc has occurred.
 *
 *  @param [in] oldAddress The original address of the allocation that was passed to realloc.
 *  @param [in] address The address that was returned by the call to realloc.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  This function informs the memory tracking system that a call to realloc has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* ReallocMemory(void* address, size_t size)
 *  {
 *      void* memory = realloc(address, size);
 *      nn::profiler::TrackRealloc(address, memory, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackRealloc(
    void const* oldAddress,
    void const* address,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to free has occurred.
 *
 *  @param [in] address The address that was passed into the call to free.
 *
 *  @details
 *  See nn::profiler::TrackFree(void const*, char const*).
 */
void TrackFree(void const* address) NN_NOEXCEPT;


/**
 *  @brief Record that a call to free has occurred.
 *
 *  @param [in] address The address that was passed into the call to free.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  This function informs the memory tracking system that a call to free has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void FreeMemory(void const* address)
 *  {
 *      free(address);
 *      nn::profiler::TrackFree(address, "Example");
 *  }
 *  @endcode
 */
void TrackFree(void const* address, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *
 *  @details
 *  See nn::profiler::TrackNew(void const*, size_t, char const*, int, bool).
 */
void TrackNew(void const* address, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackNew(void const*, size_t, char const*, int, bool).
 */
void TrackNew(void const* address, size_t size, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  See nn::profiler::TrackNew(void const*, size_t, char const*, int, bool).
 */
void TrackNew(void const* address, size_t size, char const* systemName, int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] isVector Specifies which variant of new was called. If false, new; otherwise, new[].
 *
 *  @details
 *  See nn::profiler::TrackNew(void const*, size_t, char const*, int, bool).
 */
void TrackNew(void const* address, size_t size, char const* systemName, bool isVector) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *  @param [in] isVector Specifies which variant of new was called. If false, new; otherwise, new[].
 *
 *  @details
 *  This function informs the memory tracking system that a call to new has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* NewMemory(size_t size)
 *  {
 *      void* memory = new char[size]
 *      nn::profiler::TrackNew(memory, size, "Example", 0, true);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackNew(
    void const* address,
    size_t size,
    char const* systemName,
    int alignment,
    bool isVector) NN_NOEXCEPT;


/**
 *  @brief Record that a call to delete has occurred.
 *
 *  @param [in] address The address that was passed int the call to delete.
 *
 *  @details
 *  See nn::profiler::TrackDelete(void const*, char const*, bool).
 */
void TrackDelete(void const* address) NN_NOEXCEPT;


/**
 *  @brief Record that a call to delete has occurred.
 *
 *  @param [in] address The address that was passed int the call to delete.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackDelete(void const*, char const*, bool).
 */
void TrackDelete(void const* address, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to delete has occurred.
 *
 *  @param [in] address The address that was passed int the call to delete.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] isVector Specifies which variant of delete was called. If false, delete; otherwise, delete[].
 *
 *  @details
 *  This function informs the memory tracking system that a call to delete has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void DeleteMemory(const* address)
 *  {
 *      delete [] reinterpret_cast<char*>(address);
 *      nn::profiler::TrackDelete(address, "Example", true);
 *  }
 *  @endcode
 */
void TrackDelete(void const* address, char const* systemName, bool isVector) NN_NOEXCEPT;


/**
 *  @brief Generates tracking information for a specific call to Track* functions.
 *
 *  @return A one-time use object used to denote a specific heap operation.
 *
 *  @details
 *  Tracking ids provide increased accuracy for timing and ordering of memory function calls.
 *
 *  The TrackingId that is obtained from this function is a single-use object.
 *  The intention is that the object be passed into the next Track* function that is called.
 *
 *  This function @em must be paired with a call to a Track* function which takes a
 *  TrackingId as its first parameter.
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* MallocMemory(size_t size)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      void* memory = malloc(size);
 *      nn::profiler::TrackMalloc(id, memory, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
TrackingId GenerateNextTrackingId() NN_NOEXCEPT;


/**
 *  @brief Record that a call to malloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to malloc.
 *  @param [in] size The size/amount of memory that was requested to be allocated.
 *
 *  @details
 *  See nn::profiler::TrackMalloc(TrackingId const&, void const*, size_t, char const*, int).
 */
void TrackMalloc(TrackingId const& id, void const* address, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to malloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to malloc.
 *  @param [in] size The size/amount of memory that was requested to be allocated.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackMalloc(TrackingId const&, void const*, size_t, char const*, int).
 */
void TrackMalloc(
    TrackingId const& id,
    void const* address,
    size_t size,
    char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to malloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to malloc.
 *  @param [in] size The size/amount of memory that was requested to be allocated.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  This function informs the memory tracking system that a call to malloc has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function call must be paired with a call to GenerateNextTrackingId().
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* MallocMemory(size_t size)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      void* memory = malloc(size);
 *      nn::profiler::TrackMalloc(id, memory, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackMalloc(
    TrackingId const& id,
    void const* address,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to calloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to calloc.
 *  @param [in] num The number of elements that were requested to be allocated by the call to calloc.
 *  @param [in] size The size of each element.
 *
 *  @details
 *  See nn::profiler::TrackCalloc(TrackingId const&, void const*, size_t, size_t, char const*, int).
 */
void TrackCalloc(TrackingId const& id, void const* address, size_t num, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to calloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to calloc.
 *  @param [in] num The number of elements that were requested to be allocated by the call to calloc.
 *  @param [in] size The size of each element.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackCalloc(TrackingId const&, void const*, size_t, size_t, char const*, int).
 */
void TrackCalloc(
    TrackingId const& id,
    void const* address,
    size_t num,
    size_t size,
    char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to calloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to calloc.
 *  @param [in] num The number of elements that were requested to be allocated by the call to calloc.
 *  @param [in] size The size of each element.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  This function informs the memory tracking system that a call to calloc has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function call must be paired with a call to GenerateNextTrackingId().
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* CallocMemory(size_t num, size_t size)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      void* memory = calloc(num, size);
 *      nn::profiler::TrackCalloc(id, memory, num, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackCalloc(
    TrackingId const& id,
    void const* address,
    size_t num,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to realloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] oldAddress The original address of the allocation that was passed to realloc.
 *  @param [in] address The address that was returned by the call to realloc.
 *  @param [in] size The size, in bytes, of the memory requested.
 *
 *  @details
 *  See nn::profiler::TrackRealloc(TrackingId const&, void const*, void const*, size_t, char const*, int).
 */
void TrackRealloc(
    TrackingId const& id,
    void const* oldAddress,
    void const* address,
    size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to realloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] oldAddress The original address of the allocation that was passed to realloc.
 *  @param [in] address The address that was returned by the call to realloc.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackRealloc(TrackingId const&, void const*, void const*, size_t, char const*, int).
 */
void TrackRealloc(
    TrackingId const& id,
    void const* oldAddress,
    void const* address,
    size_t size,
    char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to realloc has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] oldAddress The original address of the allocation that was passed to realloc.
 *  @param [in] address The address that was returned by the call to realloc.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  This function informs the memory tracking system that a call to realloc has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function call must be paired with a call to GenerateNextTrackingId().
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* ReallocMemory(void* address, size_t size)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      void* memory = realloc(address, size);
 *      nn::profiler::TrackRealloc(id, address, memory, size, "Example", 0);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackRealloc(
    TrackingId const& id,
    void const* oldAddress,
    void const* address,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to free has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was passed into the call to free.
 *
 *  @details
 *  See nn::profiler::TrackFree(TrackingId const&, void const*, char const*).
 */
void TrackFree(TrackingId const& id, void const* address) NN_NOEXCEPT;


/**
 *  @brief Record that a call to free has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was passed into the call to free.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  This function informs the memory tracking system that a call to free has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function call must be paired with a call to GenerateNextTrackingId().
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void FreeMemory(void* address)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      free(address);
 *      nn::profiler::TrackMalloc(id, address, "Example");
 *  }
 *  @endcode
 */
void TrackFree(TrackingId const& id, void const* address, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *
 *  @details
 *  See nn::profiler::TrackNew(TrackingId const&, void const*, size_t, char const*, int, bool).
 */
void TrackNew(TrackingId const& id, void const* address, size_t size) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackNew(TrackingId const&, void const*, size_t, char const*, int, bool).
 */
void TrackNew(
    TrackingId const& id,
    void const* address,
    size_t size,
    char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *
 *  @details
 *  See nn::profiler::TrackNew(TrackingId const&, void const*, size_t, char const*, int, bool).
 */
void TrackNew(
    TrackingId const& id,
    void const* address,
    size_t size,
    char const* systemName,
    int alignment) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] isVector Specifies which variant of new was called. If false, new; otherwise, new[].
 *
 *  @details
 *  See nn::profiler::TrackNew(TrackingId const&, void const*, size_t, char const*, int, bool).
 */
void TrackNew(
    TrackingId const& id,
    void const* address,
    size_t size,
    char const* systemName,
    bool isVector) NN_NOEXCEPT;


/**
 *  @brief Record that a call to new has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was returned by the call to new.
 *  @param [in] size The size, in bytes, of the memory requested.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] alignment The alignment requirement of the allocation.
 *  @param [in] isVector Specifies which variant of new was called. If false, new; otherwise, new[].
 *
 *  @details
 *  This function informs the memory tracking system that a call to new has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function call must be paired with a call to GenerateNextTrackingId().
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void* NewMemory(size_t size)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      void* memory = new char[size];
 *      nn::profiler::TrackNew(id, memory, size, "Example", 0, true);
 *      return memory;
 *  }
 *  @endcode
 */
void TrackNew(
    TrackingId const& id,
    void const* address,
    size_t size,
    char const* systemName,
    int alignment,
    bool isVector) NN_NOEXCEPT;


/**
 *  @brief Record that a call to delete has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was passed int the call to delete.
 *
 *  @details
 *  See nn::profiler::TrackDelete(TrackingId const&, void const*, char const*, bool).
 */
void TrackDelete(TrackingId const& id, void const* address) NN_NOEXCEPT;


/**
 *  @brief Record that a call to delete has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was passed int the call to delete.
 *  @param [in] systemName The name of a system to attribute to the call.
 *
 *  @details
 *  See nn::profiler::TrackDelete(TrackingId const&, void const*, char const*, bool).
 */
void TrackDelete(TrackingId const& id, void const* address, char const* systemName) NN_NOEXCEPT;


/**
 *  @brief Record that a call to delete has occurred.
 *
 *  @param [in] id The id to associate with this tracking information.
 *  @param [in] address The address that was passed int the call to delete.
 *  @param [in] systemName The name of a system to attribute to the call.
 *  @param [in] isVector Specifies which variant of delete was called. If false, delete; otherwise, delete[].
 *
 *  @details
 *  This function informs the memory tracking system that a call to delete has occurred.
 *  When the PC memory tracking tool is connected, this will result in information on
 *  memory allocation being sent to the PC.
 *  As such, calls to this function may be slow.
 *
 *  This function call must be paired with a call to GenerateNextTrackingId().
 *  Failure to do so may lead to improper operation of the memory tracking API.
 *  This may include a deadlock and/or application crash.
 *
 *  This function may be called before the profiler is initialized.
 *
 *  This function is thread-safe.
 *
 *  Example:
 *  @code
 *  void DeleteMemory(void* address)
 *  {
 *      nn::profiler::TrackingId const id = nn::profiler::GenerateNextTrackingId();
 *      delete [] reinterpret_cast<char*>(address);
 *      nn::profiler::TrackDelete(id, address, "Example", true);
 *  }
 *  @endcode
 */
void TrackDelete(
    TrackingId const& id,
    void const* address,
    char const* systemName,
    bool isVector) NN_NOEXCEPT;

//! @}

}} // namespace nn::profiler
