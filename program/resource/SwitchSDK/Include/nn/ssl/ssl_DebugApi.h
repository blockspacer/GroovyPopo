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
 * @brief      Set the value for debug options
 *
 * @param[in]  pInDataBuffer    A pointer to the memory containing a value specific to the debug option
 * @param[in]  dataBufferSize   The size of the pInDataBuffer
 * @param[in]  option           The option for which the value to be set
 *
 * @return     Result of the process is returned
 *
 * @retval     ResultSuccess                Success
 * @retval     ResultLibraryNotInitialized  The SSL library is not initialized yet
 * @retval     ResultInvalidPointer         NULL pointer is passed in pInDataBuffer
 * @retval     ResultInvalidOptionType      The option passed is out of range of the DebugOption
 * @retval     ResultBufferTooShort         The buffer pInDataBuffer is pointed to is too short
 *                                          to keep option value
 *
 * @pre        nn::ssl::Initialize is already called
 *
 * @details    This API is supposed to deal with various debug options. It allows to specify
 *             a debug option and a parameter specific to that option. This API can only be
 *             called for a debugging purpose. Calling it in the Release build is prohibited.
 *
 *             The debug options below are supported.
 *
 *             - DebugOption_AllowDisableVerifyOption
 *             This option expected to receive boolean parameter in pInDataBuffer. It enables
 *             nn::ssl::Connection::SetVerifyOption() function to unset the options
 *             nn::ssl::Connection::VerifyOption_PeerCa and nn::ssl::Connection::VerifyOption_HostName
 *             if the boolean parameter is set to "true". Calling of nn::ssl::Connection::SetVerifyOption()
 *             will generate SDK Assertion Failure if parameter is set to "false". The parameter
 *             is set to "false" upon initialization.
 */
nn::Result SetDebugOption(
    const void* pInDataBuffer,
    uint32_t dataBufferSize,
    DebugOption option) NN_NOEXCEPT;

/**
 * @brief      Return option value for debug options
 *
 * @param[out] pOutDataBuffer   A pointer to the memory to receive value specific to the debug option
 * @param[in]  dataBufferSize   The size of the pOutDataBuffer
 * @param[in]  option           The option for which the value to be returned
 *
 * @return     Result of the process is returned
 *
 * @retval     ResultSuccess                Success
 * @retval     ResultLibraryNotInitialized  The SSL library is not initialized yet
 * @retval     ResultInvalidPointer         NULL pointer is passed in pOutDataBuffer
 * @retval     ResultInvalidOptionType      The option passed is out of range of the DebugOption
 * @retval     ResultBufferTooShort         The buffer pOutDataBuffer is pointed to is not sufficient
 *                                          to receive the output
 *
 * @pre        nn::ssl::Initialize is already called
 *
 * @details    This API returns a parameter value for the option passed in the "option" argument.
 *             It can only be called for a debugging purpose. Calling it in the Release build is
 *             prohibited.
 *
 *             The debug options below are supported.
 *
 *             - DebugOption_AllowDisableVerifyOption
 *             This option returns boolean parameter in pOutDataBuffer. Thus size of pOutDataBuffer
 *             need to be large enough to store boolean variable. It returns the parameter value
 *             that was previously set by calling nn::ssl::SetDebugOption() function or "false" if
 *             nn::ssl::SetDebugOption() was not called yet.
 */
nn::Result GetDebugOption(
    void* pOutDataBuffer,
    uint32_t dataBufferSize,
    DebugOption option) NN_NOEXCEPT;

}}
