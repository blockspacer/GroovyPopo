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
* @brief    ID type of nn::ssl::Context
*/
typedef uint64_t SslContextId;

/**
* @brief    ID type of nn::ssl::Connection
*/
typedef uint64_t SslConnectionId;

/**
* @brief    ID type of Certstore ID
*/
typedef uint64_t CertStoreId;

/**
* @brief    The format of certificate
*/
enum CertificateFormat
{
    CertificateFormat_Pem = 0x01, //!< PEM format (Base64 encoded ASCII starts with "-----BEGIN CERTIFICATE-----")
    CertificateFormat_Der = 0x02  //!< DER foramt (binary)
};

/**
 * @brief   Status types for built-in trusted certificates
 */
enum TrustedCertStatus
{
    TrustedCertStatus_Invalid           = 0xFFFFFFFF, //!< Invalid certificate
    TrustedCertStatus_Removed           = 0x00000000, //!< Certificate has been removed (not available anymore)
    TrustedCertStatus_EnabledTrusted    = 0x00000001, //!< Certificate is trusted
    TrustedCertStatus_EnabledNotTrusted = 0x00000002, //!< Certificate is not trusted
    TrustedCertStatus_Revoked           = 0x00000003, //!< Certificate is revoked
};

/**
* @brief    The maximum length of host name
*/
const uint32_t MaxHostNameLength  = 255;

/**
* @brief    The maximum number of nn::ssl::Context objects allowed to create
*/
const uint32_t MaxContextCount    = 8;

/**
* @brief    The maximum number of nn::ssl::Connection objects allowed to create
*/
const uint32_t MaxConnectionCount = 8;

/**
* @brief    The default concurrency limit
*/
const uint32_t DefaultConcurrencyLimit = 2;

/**
* @brief    The maximum number of server PKI entity allowed to import
*/
const uint32_t MaxServerPkiImportCount = 70;

/**
* @brief    The maximum length of policy OID string
*/
const uint32_t MaxPolicyOidStringLength = 255;

/**
* @brief    The maximum number of policy OID strings allowed to add per context
*/
const uint32_t MaxPolicyOidStringCount = 32;

/**
* @brief    The maximum number of CRLs which can be imported by an app (total for
*           all contexts.)
*/
const uint32_t MaxAppCrlImportCount = 16;

/**
* @brief    Set of the options to be used for debugging purposes
*/
enum DebugOption
{
    DebugOption_AllowDisableVerifyOption, //!<  Allows to unset the option Connection::VerifyOption::VerifyOption_Default in a non release build
    DebugOption_Max
};

}}
