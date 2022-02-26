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
 * @brief    Public header file for libcurl library.
 */

#pragma once

#include <curl/curl.h>

/**
* @brief    Set of the options to be used for debugging purposes
*/
typedef enum
{
    CURLDEBUG_OPT_SSL_ALLOW_DISABLE_VERIFY, //!< Allows to disable CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST for a non release build
    CURLDEBUG_OPT_MAX
} CURLDEBUGssloption;

#ifdef __cplusplus
extern "C"
{
#endif
/**
* @brief Set value for debug options.
*
* @param[in]  pInDataBuffer     A pointer to the memory containing a value specific to the debug option
* @param[in]  dataBufferSize    The size of the pInDataBuffer
* @param[in]  option            The option for which the value to be set
*
* @return     Result of the process is returned
*
* @retval     CURLE_OK                      Success
* @retval     CURLE_FAILED_INIT             The SSL library is not initialized yet
* @retval     CURLE_BAD_FUNCTION_ARGUMENT   NULL pointer is passed in pInDataBuffer or the buffer is too short
* @retval     CURLE_UNKNOWN_OPTION          The option passed is out of range of the DebugOption
* @retval     CURLE_SSL_ERROR_LOWER         SSL library returned an unknown error
*
* @pre        libcurl initialization is completed.
*
* @details    This API is supposed to deal with various debug options. It allows to specify
*             a debug option and a parameter specific to that option. This API can only be
*             called for a debugging purpose.
*/
CURLcode curl_debug_ssl_setopt(const void* pInDataBuffer, uint32_t dataBufferSize, CURLDEBUGssloption option);

/**
* @brief      Return option value for debug options
*
* @param[out] pOutDataBuffer   A pointer to the memory to receive value specific to the debug option
* @param[in]  dataBufferSize   The size of the pOutDataBuffer
* @param[in]  option           The option for which the value to be returned
*
* @return     Result of the process is returned
*
* @retval     CURLE_OK                      Success
* @retval     CURLE_FAILED_INIT             The SSL library is not initialized yet
* @retval     CURLE_BAD_FUNCTION_ARGUMENT   NULL pointer is passed in pInDataBuffer or the buffer is too short
* @retval     CURLE_UNKNOWN_OPTION          The option passed is out of range of the DebugOption
* @retval     CURLE_SSL_ERROR_LOWER         SSL library returned an unknown error
*
* @pre        libcurl initialization is completed.
*
* @details    This API returns a parameter value for the option passed in the "option" argument.
*             It can only be called for a debugging purpose.
*/
CURLcode curl_debug_ssl_getopt(void* pOutDataBuffer, uint32_t dataBufferSize, CURLDEBUGssloption option);

#ifdef __cplusplus
}
#endif
