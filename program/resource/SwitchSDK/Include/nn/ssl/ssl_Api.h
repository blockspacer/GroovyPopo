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
 * @brief    Public header file for SSL library.
 */

#pragma once

#include "ssl_Types.h"

namespace nn { namespace ssl {

/**
 * @brief      Initialize SSL library with the default concurrency limit.
 *
 * @return     Result of the process is returned
 *
 * @retval     ResultSuccess                   Success
 * @retval     ResultResourceBusy              The system is too busy to process the request, try again later
 * @retval     ResultInvalidReference          The system encountered invalid reference, SSL library should be finalized
 * @retval     ResultInternalLogicError        The system failed to process the request due to internal logic error, SSL library should be finalized
 * @retval     ResultLibraryAlreadyInitialized The SSL library is already initialized
 * @retval     ResultInsufficientMemory        The system failed to allocate memory, try again later
 *
 * @pre        nn::ssl::Initialize is not called yet
 *
 * @details    This API initializes nn::ssl library. This API needs to be called prior to any
 *             other nn::ssl APIs including methods in classes like nn::ssl::Context and
 *             nn::ssl::Connection.
 *
 *             Calling this API after initialization has been done is harmless because the SSL
 *             library maintains the reference count inside which is incremented every time when
 *             called. Please make sure to call nn::ssl::Finalize() for the same number of times
 *             as this API call.
 */
nn::Result Initialize() NN_NOEXCEPT;

/**
 * @brief      Initialize SSL library and specify a concurrency limit.
 *
 * @param[in]  concurLimit                     The maximum number of synchronously called simultaneous operations.  Currently limited to no more than 4.
 *
 * @return     Result of the process is returned
 *
 * @retval     ResultSuccess                   Success
 * @retval     ResultResourceBusy              The system is too busy to process the request, try again later
 * @retval     ResultInvalidReference          The system encountered invalid reference, SSL library should be finalized
 * @retval     ResultInternalLogicError        The system failed to process the request due to internal logic error, SSL library should be finalized
 * @retval     ResultLibraryAlreadyInitialized The SSL library is already initialized
 * @retval     ResultInsufficientMemory        The system failed to allocate memory, try again later
 * @retval     ResultInvalidConcurrencyLimit   Invalid parameter is passed in concurLimit
 *
 * @pre        nn::ssl::Initialize is not called yet
 *
 * @details    This API initializes nn::ssl library. This API needs to be called prior to any
 *             other nn::ssl APIs including methods in classes like nn::ssl::Context and
 *             nn::ssl::Connection.
 *
 *             Calling this API after initialization has been done is harmless because the SSL
 *             library maintains the reference count inside which is incremented every time when
 *             called. Please make sure to call nn::ssl::Finalize() for the same number of times
 *             as this API call.
 */
nn::Result Initialize(uint32_t concurLimit) NN_NOEXCEPT;

/**
 * @brief      Finalize SSL library.
 *
 * @return     Result of the process is returned
 *
 * @retval     ResultSuccess               Success
 * @retval     ResultResourceBusy          The system is too busy to process the request, try again later
 * @retval     ResultInvalidReference      The system encountered invalid reference, SSL library should be finalized
 * @retval     ResultInternalLogicError    The system failed to process the request due to internal logic error, SSL library should be finalized
 * @retval     ResultLibraryNotInitialized The SSL library is not initialized yet
 *
 * @details    This API finalizes nn::ssl library. Before finalizing the library, all the existing
 *             class instances like nn::ssl::Context and nn::ssl::Connection created by its
 *             Create API need to be destroyed by its Destroy API.
 *             Calling this API before detroying SSL contexts/connections will cause assertion in
 *             Debug/Develop build.
 *
 *             Note that this API returns ResultSuccess even when the reference count maintained
 *             inside the library is not zero. In such the case, this API just decrements the
 *             reference counter except for when ResultLibraryNotInitialized is returned.
 *             The SSL library is actually finalized when this API is called after the reference
 *             counter becomes zero. Please make sure to call this API for the same number of times
 *             as nn::ssl::Initialize() call.
 *
 * @pre
 *             - nn::ssl::Initialize is already called
 *             - All the nn::ssl::Context and nn::ssl::Connection are already destroyed
 */
nn::Result Finalize() NN_NOEXCEPT;

/**
 * @brief      Get the SSL error result from value
 *
 * @param[out] pOutResult  A pointer to the memory which converted SSL result should be stored
 * @param[in]  pInValue    A pointer to the memory which the value needs conversion is stored
 * @param[in]  inValueSize The size of pInValue buffer
 *
 * @return     Result of the process is returned
 *
 * @retval     ResultSuccess           Success
 * @retval     ResultInvalidPointer    NULL pointer is passed in pOutResult or in pInValue
 * @retval     ResultBufferTooShort    inValueSize is too short. It needs to be bigger than size of uint32_t
 *
 * @details    This API converts value passed in pInValue to the SSL result defined in
 *             ssl_Result.public.h. Converted result will be stored in pOutResult. Please refer
 *             ssl_Result.public.h for details about SSL errors.
 */
nn::Result GetSslResultFromValue(
    nn::Result* pOutResult,
    const char* pInValue,
    uint32_t inValueSize) NN_NOEXCEPT;

}}
