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

#include <nn/ssl/ssl_Types.h>


namespace nn { namespace ssl {

/**
 * @brief   The class representing a context of SSL which holds values used when creating SSL connections.
 *
 * @details This is the class which represents context of SSL. The context is used to hold
 *          configuration values which are used when creating the SSL connection under the context.
 *          For example, SSL version used in nn::ssl::Connection is determined based on the
 *          configuration in the SSL context.
 *
 *          ### Create/Destoy
 *          Instantiating a class doesn't create an actual SSL context. It needs to be
 *          created explicitly by nn::ssl::Context::Create. It needs to be destroyed
 *          by nn::ssl::Context::Destroy when it is not used anymore. It is requried to close all
 *          SSL connections created under the SSL context before destroying it.
 *          Please note that deleting instance of the SSL context before detroying SSL connections
 *          associated with the SSL context will cause assertion in Debug/Develop build.
 *
 *          #### Certitiface
 *          Though the SSL library provides a build-in certificate store with common commercial CA
 *          certificates, it provides a way to import CA certificates and client certificates/keys
 *          too. These entities are imported through the SSL context and each context is
 *          responsible to maintain imported entities to be used for SSL communication.
 *          Certificates/keys associated with the SSL context will be used for all SSL connections
 *          created under the SSL context.
 *
 *          It is recommended to remove imported certificates once the application doesn't need it
 *          anymore. It can be removed by @ref nn::ssl::Context::RemovePki with the CertStoreId
 *          provided by the API used when importing certificate/key. It is automatically removed
 *          when the SSL context is destroyed or the instance of the SSL context gets freed.
 *
 *          Note that the imported certificates/keys are volatile. As previously stated,
 *          certificates/keys imported into the SSL context are removed once the SSL context is
 *          destroyed.
 *          The SSL library does not maintain the certificates/keys in the permanent
 *          certificate/key storage. The application must import any needed certificates/keys every
 *          time the application requires them.
 *
 *          ### Policy OID string for EV certificate verification
 *          Similar to built-in certificate store, the SSL library comes with built-in policy OID
 *          strings which are used during EV certificate verification. EV certificate verification
 *          needs to be enabled by nn::ssl::Connection::SetVerifyOption().
 *          In addition to built-in policy OID strings, nn::ssl::Context provides the way to
 *          register user defined policy OID strings via nn::ssl::Context::AddPolicyOid().
 *          Registered policy OID strings are used in addition to built-in policy OID strings
 *          during EV certificate verification. It is available in all nn::ssl::Connection objects
 *          created under the SSL context.
 *          Note that registered policy OID strings are volatile. These are removed once the SSL
 *          context is destroyed. The application must register it every time when the SSL context
 *          is created.
 *
 *          ### Limitation
 *          An application can create maximum upto 8 SSL contexts. Please not that the SSL
 *          library controls the number of SSL contexts inside the system. There is system wide
 *          limit on the number of SSL contexts too thus the caller may not able to create even
 *          8 contexts at some point when the system is exhausting context resources in the
 *          busy situation. Please consider to destroy unnecessary SSL contexts or retry later
 *          in such the case.
 */
class Context
{
private:
    SslContextId                m_ContextId; //!< Context ID assigned when the context is created

public:
    /**
     * @brief    The option can be get or set by nn::ssl::Context::GetOption or nn::ssl::Context::SetOption
     */
    enum ContextOption
    {
        ContextOption_None                     = 0x00, //!< No option
        ContextOption_CrlImportDateCheckEnable = 0x01, //!< Enable date check during CRL import (0 = false | 1 = true). Default is true.
    };

    /**
     * @brief    SSL version to be used
     */
    enum SslVersion
    {
        SslVersion_Auto   = 0x01, //!< Automatically select the available highest version supported
        SslVersion_TlsV10 = 0x08, //!< TLS 1.0
        SslVersion_TlsV11 = 0x10, //!< TLS 1.1
        SslVersion_TlsV12 = 0x20, //!< TLS 1.2
    };

    /**
     * @brief    The type of PKI stored in the device
     */
    enum InternalPki
    {
        InternalPki_None = 0x00,                   //!< None
        InternalPki_DeviceClientCertDefault = 0x01 //!< The default client certificate stored in the device
    };

    /**
     * @brief    The constructor of nn::ssl::Context.
     */
    Context() NN_NOEXCEPT;

    /**
     * @brief    The destructor of nn::ssl::Context.
     */
    ~Context() NN_NOEXCEPT;

    /**
     * @brief     Create SSL context.
     *
     * @param[in] version    Version of SSL/TLS to be used
     *
     * @return    Result of the process is returned
     *
     * @retval    ResultSuccess               Success
     * @retval    ResultResourceBusy          The system is too busy to process the request, try again later
     * @retval    ResultInvalidReference      The system encountered invalid reference, SSL library should be finalized
     * @retval    ResultInternalLogicError    The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval    ResultLibraryNotInitialized The SSL library is not initialized yet
     * @retval    ResultInsufficientMemory    The system failed to allocate memory, try again later
     * @retval    ResultResourceMax           The maximum number of SSL contexts are already created
     *
     * @pre
     *            - nn::ssl::Initialize is already called
     *
     * @post
      *           - m_ContextID != 0 (can be obtained by nn::ssl::Context::GetContextId)
     *
     * @details   This API creates SSL context. Instantiating a class doesn't create an actual
     *            SSL context. It needs to be created explicitly by calling this API.
     *            An application can create maximum upto 8 contexts.
     */
    nn::Result Create(SslVersion version) NN_NOEXCEPT;

    /**
     * @brief     Destroy SSL context.
     *
     * @return    Result of the process is returned
     *
     * @retval    ResultSuccess               Success
     * @retval    ResultResourceBusy          The system is too busy to process the request, try again later
     * @retval    ResultInvalidReference      The system encountered invalid reference, SSL library should be finalized
     * @retval    ResultInternalLogicError    The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval    ResultLibraryNotInitialized The SSL library is not initialized yet
     * @retval    ResultInvalidContext        The SSL context is not created yet
     * @retval    ResultConnectionRemaining   Instance of nn::ssl:Connection associated with the context still remains
     *
     * @pre
     *            - nn::ssl::Initialize is already called
     *            - SSL context is already created by nn::ssl::Context::Create
     *            - All SSL connections (nn::ssl::Connection) associated with the context have been destroyed
     *
     * @post
     *            - m_ContextID == 0 (can be obtained by nn::ssl::Context::GetContextId)
     *
     * @details   This API destroys SSL context. Deleting an instance of the nn::ssl::Context
     *            doesn't destroy the SSL context. It needs to be destroyed explicitly by calling
     *            this API.
     *            Before destroying the SSL context, all SSL connections (nn::ssl::Connection)
     *            associated with the context need to be destroyed explicitly.
     *            Note that deleting instance of SSL context before detroying it will cause
     *            assertion in Debug/Develop build.
     */
    nn::Result Destroy() NN_NOEXCEPT;

    /**
     * @brief      Import a trusted CA certificate to be used to verify the peer certificate
     *
     * @param[out] pOutCertId     A pointer to the memory which CertStoreId tight to imported certificate is set
     * @param[in]  pInCertData    A pointer to the memory which certificate to be imported is stored
     * @param[in]  certDataSize   The size of certificate buffer (pInCertData)
     * @param[in]  certFormat     The format of certificate stored in pInCertData
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultResourceBusy          The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference      The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError    The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower            An error occurred in low level
     * @retval     ResultLibraryNotInitialized SSL library is not initialized yet
     * @retval     ResultInsufficientMemory    The system failed to allocate memory, try again later
     * @retval     ResultResourceMax           The maximum number of SSL contexts are already created
     * @retval     ResultInvalidContext        The SSL context is not created yet
     * @retval     ResultInvalidCertFormat     Passed certFormat is invalid
     * @retval     ResultInvalidCertificate    The format of passed server certificate in pInCertData is invalid
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL context is already created by nn::ssl::Context::Create
     *
     * @details    This API imports a trusted CA certificate to be used to verify the peer
     *             certificate. The certificate is used for all SSL connections created under
     *             the SSL context (nn::ssl::Context). This API can be called multiple times to
     *             import multiple trusted CA certificates.
     *             Note that this API takes only one certificate even when multiple certificates
     *             are contained in pInCertData. If there are multiple certificates in the buffer,
     *             only the first one will be imported. Please call this API multiple times with
     *             the buffer which contains only one certificate.
     *
     *             The format of certificate passed in pIncertData needs to be either in DER binary
     *             or PEM Base64 encoded ASCII format.
     *
     *             Note that imported certificates are not persistent. The application is reuqired
     *             to provide them every time when the application starts.
     *
     *             It is recommended to remove imported certificates once the application doesn't
     *             need it anymore. It can be removed by @ref nn::ssl::Context::RemovePki with
     *             the CertStoreId provided when the certificate is imported by this API.
     */
    nn::Result ImportServerPki(
        CertStoreId* pOutCertId,
        const char* pInCertData,
        uint32_t certDataSize,
        CertificateFormat certFormat) NN_NOEXCEPT;

    /**
     * @brief      Import a client certificate and a private key in PKCS12 format, and its password
     *
     * @param[out] pOutCertId     A pointer to the memory which CertStoreId tight to imported client PKI is set
     * @param[in]  pInP12Data     A pointer to the memory which client certificate and private key in PKCS12 format to be imported is stored
     * @param[in]  pInPwData      A pointer to the memory which password for PKCS12 data stored in pInP12Data gets decrypted is stored
     * @param[in]  p12DataSize    The length of PKCS12 data in pInP12Data
     * @param[in]  pwDataSize     The length of password in pInPwData
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                    Success
     * @retval     ResultResourceBusy               The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference           The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError         The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower                 An error occurred in low level
     * @retval     ResultLibraryNotInitialized      The SSL library is not initialized yet
     * @retval     ResultInsufficientMemory         The system failed to allocate memory, try again later
     * @retval     ResultResourceMax                The maximum number of SSL contexts are already created
     * @retval     ResultInvalidContext             The SSL context is not created yet
     * @retval     ResultClientPkiAlreadyRegistered Client PKI is already registered under the context
     * @retval     ResultInvalidPasswordSize        Invalid size (zero) was passed in pwDataSize for the size of passed password in pinPwData
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL context is already created by nn::ssl::Context::Create
     *             - No client certificate and private key are imported yet
     *
     * @details    This API imports a client certificate and a private key to be used in case the
     *             server requires client authentication. The certificate and the private
     *             key need to be passed in PKCS12 format along with its password to allow SSL
     *             library decrypting PKCS12 data.
     *             The certificate and the private key will be used for all the connections created
     *             under the context (nn::ssl::Context).
     *
     *             Note that only one pair of the client certificate and the private key can be
     *             imported into the SSL context.
     *
     *             Note that imported client PKI is not persistent. The caller is reuqired to
     *             provide it every time when the application starts.
     *
     *             It is recommended to remove imported client PKI once the application doesn't
     *             need it anymore. It can be removed by @ref nn::ssl::Context::RemovePki with
     *             the CertStoreId provided when the certificate is imported by this API.
     */
    nn::Result ImportClientPki(
        CertStoreId* pOutCertId,
        const char* pInP12Data,
        const char* pInPwData,
        uint32_t  p12DataSize,
        uint32_t  pwDataSize) NN_NOEXCEPT;

    /**
     * @brief      Resiger an internal PKI information stored in the system to the context
     *
     * @param[out] pOutCertId    A pointer to the memory which CertStoreId tight to registered PKI information is set
     * @param[in]  pkiType       The type of PKI information to register
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                    Success
     * @retval     ResultResourceBusy               The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference           The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError         The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower                 An error occurred in low level
     * @retval     ResultLibraryNotInitialized      The SSL library is not initialized yet
     * @retval     ResultInsufficientMemory         The system failed to allocate memory, try again later
     * @retval     ResultResourceMax                The maximum number of SSL contexts are already created
     * @retval     ResultInvalidContext             The SSL context is not created yet
     * @retval     ResultClientPkiAlreadyRegistered Client PKI is already registered under the context
     * @retval     ResultInvalidInternalPkiType     Invalid PKI type was passed
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL context is already created by nn::ssl::Context::Create
     *             - No client certificate and private key are imported yet
     *
     * @details    This API registers PKI information which is stored in the system to the context.
     *             Unlike ImportServerPki or ImportClientPki, this API obtains PKI information from
     *             the system internally and allows the context to used it. The imported PKI
     *             information will be used for all the connections created under the context
     *             (nn::ssl::Context).
     *
     *             Note that InternalPki_DeviceClientCertDefault cannot be registered when the other
     *             cilent certificate and the private key are already imported in the context by
     *             ImportClientPki.
     *
     *             It is recommended to remove registered PKI information once the application
     *             doesn't need it anymore. It can be removed by @ref nn::ssl::Context::RemovePki
     *             with the CertStoreId provided when the certificate is registered by this API.
     */
    nn::Result RegisterInternalPki(CertStoreId* pOutCertId, InternalPki pkiType) NN_NOEXCEPT;

    /**
     * @brief      Remove imported certstore object
     *
     * @param[in]  certId    ID value which is obtained by ImportServerPki or ImportClientPki when certstore object is imported
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultResourceBusy          The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference      The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError    The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower            An error occurred in low level
     * @retval     ResultLibraryNotInitialized SSL library is not initialized yet
     * @retval     ResultResourceMax           The maximum number of SSL contexts are already created
     * @retval     ResultInvalidContext        SSL context is not created yet
     * @retval     ResultInvalidCertStoreId    There is no certstore entity tight to passed ID
     *
     */
    nn::Result RemovePki(CertStoreId certId) NN_NOEXCEPT;

    /**
     * @brief      Add trusted policy OID string for EV certificate verification
     *
     * @param[in]  pInPolicyOIdString    A pointer to the memory which policy OID string is set
     * @param[in]  stringBufferSize      The size of the policy OID string buffer (pInPolicyOIdString)
     *
     * @return     Result of the process is returned
     *
     * @details    This API registers the policy OID string which will be used when the EV
     *             certificate validation is enabled in the nn::ssl::Connection which belongs
     *             to this SSL context. Registerd policy OID string is used in all connections
     *             tied to the SSL context in addition to built-in policy OID strings. Please refer
     *             the overview of nn::ssl for the list of policy OID strings included in
     *             the built-in policy OID string database.
     *
     *             It is allowed to register policy OID strings up to
     *             nn::ssl::MaxPolicyOidStringCount per the SSL context. The length of the policy
     *             OID needs to be shorter than nn::ssl::MaxPolicyOidStringLength including null
     *             terminator.
     *
     * @retval     ResultSuccess                            Success
     * @retval     ResultResourceBusy                       The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference                   The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError                 The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower                         An error occurred in low level
     * @retval     ResultLibraryNotInitialized              SSL library is not initialized yet
     * @retval     ResultInvalidContext                     SSL context is not created yet
     * @retval     ResultPolicyOidStringTooLong             Passed policy OID string is too long
     * @retval     ResultMaxPolicyOidRegistered             The maximum number of policy OIDs are already set
     * @retval     ResultInvalidPolicyOidStringBufferLength Passed buffer size is invalid, it is required to pass the buffer including null terminator
     *
     */
    nn::Result AddPolicyOid(const char* pInPolicyOIdString, uint32_t stringBufferSize) NN_NOEXCEPT;

    /**
     * @brief      Import a CRL in DER format to be used during peer certificate verification
     *
     * @param[out] pOutCrlId      A pointer to the memory where the CertStoreId tied to the imported CRL is set
     * @param[in]  pInCrlDerData  A pointer to the memory where the data to be imported is stored
     * @param[in]  crlDataSize    The size of certificate buffer (pInCrlDerData), bytes
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                Success
     * @retval     ResultResourceBusy           The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference       The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError     The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower             An error occurred in low level
     * @retval     ResultLibraryNotInitialized  SSL library is not initialized yet
     * @retval     ResultInsufficientMemory     The system failed to allocate memory, try again later
     * @retval     ResultInvalidContext         The SSL context is not created yet
     * @retval     ResultUnknownCrlIssuer       The issuer of the CRL is not in the default or imported trust
     * @retval     ResultCrlSignatureVerifyFail The CRL signature verification failed
     * @retval     ResultResourceMax            The maximum number of CRLs for the app process have been imported
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL context is already created by nn::ssl::Context::Create
     *
     * @details    This API imports a CRL to be used during verification of the peer
     *             certificate. The CRL is used for all SSL connections created under
     *             the SSL context (nn::ssl::Context). This API can be called multiple times to
     *             import multiple CRL and they can be from different issuers.
     *
     *             The format of certificate passed in pInCrlDerData must be in DER format.
     *
     *             Note that imported CRLs are not persistent. The application is required
     *             to provide them every time when the application starts.
     *
     *             It is recommended to remove imported CRLs once the application doesn't
     *             need it anymore. It can be removed by @ref nn::ssl::Context::RemovePki with
     *             the CertStoreId provided when the certificate is imported by this API.
     */
    nn::Result ImportCrl(
        CertStoreId* pOutCrlId,
        const char*  pInCrlDerData,
        uint32_t     crlDataSize) NN_NOEXCEPT;

    /**
     * @brief      Get context ID of this context.
     *
     * @param[out] pOutValue    Pointer to the buffer in which obtained context ID should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultLibraryNotInitialized SSL library is not initialized yet
     * @retval     ResultInvalidPointer        NULL pointer is passed in pOutValue
     */
    nn::Result GetContextId(SslContextId* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Set an option on the Context.
     *
     * @param[in]  optionName    The name of the option to set
     * @param[in]  optionValue   The value to set for the option
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultLibraryNotInitialized SSL library is not initialized yet
     * @retval     ResultInvalidOptionType     The provided option is not valid
     * @retval     ResultInvalidArgument       The provided option value is not
     *                                         valid for the specified option
     */
    nn::Result SetOption(ContextOption optionName, int optionValue) NN_NOEXCEPT;

    /**
     * @brief      Get an option on the Context.
     *
     * @param[out] pOutValue     The current value of the option
     * @param[in]  optionName    The name of the option to get
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultLibraryNotInitialized SSL library is not initialized yet
     * @retval     ResultInvalidOptionType     The provided option is not valid
     */
    nn::Result GetOption(int* pOutValue, ContextOption optionName) NN_NOEXCEPT;
};

}}
