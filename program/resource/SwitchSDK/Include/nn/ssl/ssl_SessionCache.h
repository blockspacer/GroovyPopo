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

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace ssl {

    /**
     * @brief    The option for clearing session cache
     */
    enum FlushSessionCacheOptionType
    {
        FlushSessionCacheOptionType_SingleHost = 0, //!< Deletes session cache associated with the specified hostname.
        FlushSessionCacheOptionType_AllHosts        //!< Deletes all session cache, no matter the hostname.
    };

    /**
     * @brief      Deletes session cache either for a specific hostname or all session cache.
     *
     * @param[out] pOutEntriesDeletedCount    Number of session cache entries that were deleted.
     * @param[in]  pInHostName                Hostname associated with the session cache.
     *             - When FlushSessionCacheOptionType_SingleHost is passed, pInHostName must be
     *               non-null and be null-terminated.
     *             - When FlushSessionCacheOptionType_AllHosts is passed, pInHostName must be null.
     * @param[in]  hostNameBufLen             Length of the buffer specified by pInHostName.
     *             - When FlushSessionCacheOptionType_SingleHost is passed, hostNameBufLen must be
     *               greater than zero
     *             - When FlushSessionCacheOptionType_AllHosts is passed, hostNameBufLen must be zero.
     * @param[in]  optionValue                Option for which session cache entries will be deleted.
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultLibraryNotInitialized SSL library is not initialized yet
     * @retval     ResultResourceBusy          The system is too busy to process the request, try again later
     * @retval     ResultInvalidOptionType     The provided option is not valid
     * @retval     ResultInvalidPointer        The provided pointer is invalid
     * @retval     ResultInvalidArgument       An invalid argument was passed
     *
     * @pre
     *             - nn::ssl::Initialize is already called.
     *
     * @details    This API deletes session cache. The session cache it deletes depends on the
     *             parameter 'optionValue'.
     *             When 'FlushSessionCacheOptionType_SingleHost' is passed,
     *             only session cache associated to the hostname(specified by 'pInHostName') will be
     *             deleted. Port number and IP address of the session cache are not considered and the
     *             session cache will be deleted if the hostname matches.
     *             When 'FlushSessionCacheOptionType_AllHosts' is passed, all session cache will be
     *             deleted no matter what the hostname is.
     */
    nn::Result FlushSessionCache(uint32_t * pOutEntriesDeletedCount, const char * pInHostName, uint32_t hostNameBufLen, FlushSessionCacheOptionType optionValue) NN_NOEXCEPT;

} }    //  nn::ssl
