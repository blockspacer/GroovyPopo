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
#include <nn/ssl/ssl_Context.h>

namespace nn { namespace ssl {

/**
 * @brief   The class representing connection of SSL used to perform SSL communication.
 *
 * @details This is the class which represents connection of SSL. One instance of the class is in
 *          charge of managing one SSL connection based on TCP/IP socket.
 *          Each connection is tied to the specific SSL context (nn::ssl::Context) and necessary
 *          values to configure SSL connection are referred in the SSL context.
 *
 *          ### Create/Destoy
 *          Instantiating a class doesn't create an actual SSL connection. It needs to be
 *          created explicitly by nn::ssl::Connection::Create. It needs to be destroyed
 *          by nn::ssl::Connection::Destroy when it is not used anymore.
 *
 *          ### Working with a socket
 *          Prior to performing SSL handshake by DoHandshake, the caller is required to create a
 *          TCP socket and establish connection on the socket by use of Socket library (nn::socket).
 *          Then the socket needs to be imported into the SSL connection by SetSocketDescriptor.
 *
 *          Once the socket is imported, it is not allowed to perform any operation with socket
 *          APIs on the imported socket. Any following operation needs to be performed by the
 *          methods provided in this class. Once the corresponding socket is not used anymore
 *          (e.g. SSL connection is closed) then it is closed by the SSL library automatically.
 *          It is also closed automatically when the SSL connection is destroyed by
 *          nn::ssl::Connection::Destroy if it's not closed yet at the moment. Hence the caller
 *          is not required to deal with any socket operation including closing it once it is
 *          passed to the SSL connection.
 *
 *          ### I/O operation and Poll
 *          Methods for I/O operation are provided in this class too. Following methods are
 *          considered as I/O API in the SSL library.
 *          - nn::ssl::Connection::DoHandshake
 *          - nn::ssl::Connection::Read
 *          - nn::ssl::Connection::Peek
 *          - nn::ssl::Connection::Write
 *
 *          The behavior of I/O API is controlled by the IoMode which can be configured by
 *          nn::ssl::Connection::SetIoMode. By default I/O APIs stated above behave as blocking API
 *          so that they block until the appropriate event to process occur. When I/O modes is set
 *          to non-blokcing, I/O APIs return immediately possibly with ResultIoWouldBlock.
 *          nn::ssl::Connection::Poll is provided for non-blocking I/O APIs to wait for the
 *          appropriate event to process occur on the connection.
 *
 *          ### Session Cache
 *          When the SSL connection is created by nn::ssl::Connection::Create() and configured
 *          to be ready for performing the SSL handshake, session cache feature is enabled by
 *          default.
 *          When SSL handshake is performed by nn::ssl::Connection::DoHandshake(), it first
 *          searches if there is the available session cache which can be used for the specific
 *          host that the SSL client is attempting to establish a secure connection with.
 *          When the corresponding session cache is found, it will be used automatically for
 *          the SSL connection and the reference count in the session cache is incremented.
 *          When the corresponding one is not found, the SSL library creates the session cache
 *          after SSL handshake completes.
 *          It is possible to disable automatic socket close if the feature is explicitly disabled
 *          via nn::ssl::Connection::SetOption. It is recommended not to disable it unless it
 *          is really necessary.
 *
 *          #### Session ID and Session Ticket
 *          By default, session caches are created by the session ID feature. It is possible to
 *          get them created by the session ticket feature too. It can be configured by
 *          nn::ssl::Connection::SetSessionCacheMode().
 *          Please note that the session ID feature is always enabled even when the session ticket
 *          feature is enabled unless session cache is disabled explicitly by
 *          nn::ssl::Connection::SessionCacheMode::SessionCacheMode_None. The behavior on the
 *          client side depends on the server when the session ticket is enabled.
 *          - If the server sends session_ticket message, the session ticket feature will be used
 *          - If the server doesn't send session ticket message, but a valid session ID is found
 *            in server_hello, the session ID feature will be used
 *
 *          #### Session cache handling inside the SSL library
 *          Session caches are maintained by the SSL library until one of the condition
 *          described below is met.
 *          - nn::ssl::Connection::FlushSessionCache() is called when there is no other connection
 *            which refers the same session cache
 *          -- Calling nn::ssl::Connection::FlushSessionCache() is recommended if the application
 *             doesn't communicate with the same host anymore
 *          - When the timeout comes
 *          -- A session cache will be freed if it is not refered for AN HOUR
 *          - When the maximum number of session caches are created
 *          -- The SSL library can maintain 32 session caches which is the system wide limit
 *          -- If a session cache is created when there are the maximum number of session caches,
 *             the oldest session cache (which is not refered for the longest time) will be freed
 *          - When the application exits
 *          -- All session caches created for the application will be freed when the
 *             application exits
 *
 *          #### Configuring the session cache
 *          Session cache feature can be configured by nn::ssl::Connection::SetSessionCacheMode().
 *          Passing nn::ssl::Connection::SessionCacheMode_None disables session cache.
 *          Passing nn::ssl::Connection::SessionCacheMode_SessionId enables session ID caching.
 *
 *          ### Limitation
 *          An application can create maximum upto 8 SSL connections. Please not that the SSL
 *          library controls the number of SSL connections inside the system. There is system wide
 *          limit on the number of SSL connections too thus the caller may not able to create even
 *          8 connections at some point when the system is exhausting connection resources in the
 *          busy situation. Please consider to close unnecessary SSL connection or destroy later
 *          in such the case.
 */
class Connection
{
private:
    SslConnectionId m_ConnectionId; //<! Connection ID assigned when the connection is created
    SslContextId    m_ContextId;    //<! Context ID which is associated with the connection (set by nn::ssl::Connection::Create)
    int             m_SockToClose;  //<! Socket descriptor to be closed when object is destroyed (if >= 0)
    char*           m_CertBuf;      //<! Temporary storage of buffer to hold server certificate when provided via nn::ssl::SetServerCertBuffer() and used during nn::ssl::DoHandshake()
    uint32_t        m_CertBufLen;   //<! The length of the buffer pointed to by m_CertBuf
    nn::Result      m_IoLastError;  //<! Last error of I/O APIs

public:

    /**
     * @brief    Event type of interest and ready to process in I/O APIs. This enum event can be
     *           manipulated via bitwise OR or AND operation to handle multiple events.
     */
    enum class PollEvent : uint32_t
    {
        PollEvent_None   = 0x00, //!< No event
        PollEvent_Read   = 0x01, //!< Read event
        PollEvent_Write  = 0x02, //!< Write event
        PollEvent_Except = 0x04  //!< Exception event
    };

    /**
     * @brief   Option value to specify what is verified during SSL handshake. This enum option can
     *          be manipulated via bitwise OR or AND operation to handle multiple options.
     *          VerifyOption_Default will be used when nothing is configured by SetVerifyOption().
     *          NOTE: VerifyOption_PeerCa must be enabled for all other VerifyOption values
     *          to be performed.
     */
    enum class VerifyOption : uint32_t
    {
        VerifyOption_None              = 0x00, //!< No validation
        VerifyOption_PeerCa            = 0x01, //!< Validate the peer certificate
        VerifyOption_HostName          = 0x02, //!< In addition to peer certificate validation, also check if the host name matches the Common Name field or a Subject Alternate Name field in the peer certificate
        VerifyOption_DateCheck         = 0x04, //!< In addition to peer certificate validation, validate certificates ValidFrom and ValidTo fields
        VerifyOption_EvCertPartial     = 0x08, //!< This is the same as VerifyOption_EvPolicyOid
        VerifyOption_EvPolicyOid       = 0x08, //!< In addition to peer certificate validation, verifies the policy OID is trusted
        VerifyOption_EVCertFingerprint = 0x20, //!< In addition to VerifyOption_EvPolicyOid, validates EV CA fingerprints
        VerifyOption_Default       = (VerifyOption_PeerCa | VerifyOption_HostName),                         //!< Default verify option
        VerifyOption_All           = (VerifyOption_PeerCa | VerifyOption_HostName | VerifyOption_DateCheck) //!< All
    };

    /**
     * @brief    The value to specify blocking mode of I/O APIs (DoHandshake/Read/Write).
     */
    enum IoMode
    {
        IoMode_Blocking    = 1, //!< I/O APIs behave as blocking API (default)
        IoMode_NonBlocking = 2  //!< I/O APIs behave as non-blocking API
    };

    /**
     * @brief    The value to specify the session cache mode of the connection.
     */
    enum SessionCacheMode
    {
        SessionCacheMode_None          = 0, //!< No session cache will be used for this connection
        SessionCacheMode_SessionId     = 1, //!< Session ID will be used when reusing available SSL session (default)
        SessionCacheMode_SessionTicket = 2, //!< Session ticket will be enabled in addition to session ID
    };

    /**
     * @brief    The value to specify the renegotiation mode of the connection.
     */
    enum RenegotiationMode
    {
        RenegotiationMode_None   = 0, //!< No renegotiation will be performed
        RenegotiationMode_Secure = 1, //!< Secure renegotiation will be performed when requested by the server (default)
    };

    /**
     * @brief    Option values to be used in SetOption() and GetOption()
     */
    enum OptionType
    {
        OptionType_DoNotCloseSocket   = 0, //!< The imported socket by SetSocketDescriptor() will not be closed automatically when enabled (disabled by default)
        OptionType_GetServerCertChain = 1, //!< Return the entire server certificate chain after DoHandshake. The default setting just returns the server's certificate.
        OptionType_SkipDefaultVerify  = 2, //!< Allow the connection to skip default certificate validation (disabled by default). nn::ssl::SetDebugOption() shouold be used if verify option needs to be disabled for debugging purpose.
    };

    /**
     * @brief    Structure used to hold server DER certificate data.  Used to
     *           get the certificates in the server chain after DoHandshake().
     */
    struct ServerCertDetail
    {
        uint32_t                dataSize;     //!< The size of the DER data, in bytes
        const char*             pDerData;    //!< Pointer to the DER data
    };

    /**
     * @brief   Holds information about cipher spec.
     */
    struct CipherInfo
    {
        CipherInfo() NN_NOEXCEPT;

        static const uint32_t StructureVersion = 1;  //!< For internal use only.
        static const uint32_t NameBufLen       = 64; //!< Length of the buffer used to hold SSL cipher name.
        static const uint32_t VersionBufLen    = 8;  //!< Legnth of the buffer used to hold SSL version name.

        char cipherName[NameBufLen];     //!< Buffer to hold SSL cipher name.
        char versionName[VersionBufLen]; //!< Buffer to hold SSL version name.
    };

    /**
     * @brief    The constructor of nn::ssl::Connection.
     */
    Connection() NN_NOEXCEPT;

    /**
     * @brief    The destructor of nn::ssl::Connection.
     */
    ~Connection() NN_NOEXCEPT;

    /**
     * @brief      Create SSL connection.
     *
     * @param[in]  pInSslContext    A context to be used for the connection.
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess               Success
     * @retval     ResultResourceBusy          The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference      The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError    The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized The SSL library is not initialized yet
     * @retval     ResultInsufficientMemory    The system failed to allocate memory, try again later
     * @retval     ResultResourceMax           The maximum number of SSL connections are already created
     * @retval     ResultInvalidPointer        NULL pointer is passed in pInSslContext
     * @retval     ResultInvalidContext        Passed context is not created yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - nn::ssl::Context used to create this connection is already instantiated and created
     *
     * @post
     *             - m_ConnectionId != 0
     *
     * @details    This API creates the SSL connection to perform secure data transfer over TCP/IP
     *             connection. Instantiating a class doesn't create an actual SSL connection.
     *             It needs to be created explicitly by calling this API. An application can create
     *             maximum upto 8 SSL connections.
     */
    nn::Result Create(Context* pInSslContext) NN_NOEXCEPT;

    /**
     * @brief      Destroy SSL connection.
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *
     * @post
     *             - m_ConnectionId == 0
     *
     * @details    This API shuts down the underlying SSL connection and destroys the connection.
     *             Deleting an instance doesn't destroy SSL connection. It needs to be destroyed
     *             explicitly by calling this API. Note that the socket which is associated with
     *             the connection by nn::ssl::Connection::SetSocketDescriptor is closed
     *             automatically.
     *
     *             Note that deleting instance of SSL connection before detroying it will cause
     *             assertion in Debug/Develop build.
     */
    nn::Result Destroy() NN_NOEXCEPT;

    /**
     * @brief      Set socket descriptor to be used for SSL/TLS connection.
     *
     * @param[in]  socketDescriptor    Socket descriptor created by the socket library
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketAlreadyRegistered  Socket descriptor is already registered
     * @retval     ResultNoTcpConnection          Passed socket doesn't have a TCP connection on it
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - nn::ssl::Connection is already created by nn::ssl::Connection::Create
     *             - The socket is not registered by this API yet
     *             - TCP connection is alredy established upon the socket passed to this API
     *
     * @details    This API imports a socket descriptor to the SSL connection. The caller is
     *             required to create the socket using nn::socket::Socket, connect it to the server
     *             using nn::socket::Connect before importing it by this API.
     *             Note that the socket imported by this API should not be used through nn::socket
     *             APIs once it gets imported. Following I/O operation needs to be performed by
     *             APIs in nn::ssl library.
     */
    nn::Result SetSocketDescriptor(int socketDescriptor) NN_NOEXCEPT;

    /**
     * @brief      Set host name of the server used for hostname validation.
     *
     * @param[in]  pInHostName       hostname of the server
     * @param[in]  hostNameLength    Length of hostname buffer
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pInHostName
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultHostNameTooLong          Passed host name is too long
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *
     * @details    This API sets host name of the server to the connection. The host name set by
     *             this API will be used to verify CommonName or dNSName in SubjectAltName field.
     *             nn::ssl::Connection::DoHandshake will return an error when validation fails but
     *             only when host name validation is enbled by nn::ssl::Connection::SetVerifyOption.
     *             The maximum length of host name can be set is nn::ssl::MaxHostNameLength
     *             including NULL terminated string.
     *             Please set the host name explicitly with this API to use
     *             Server Name Indication (SNI).
     */
    nn::Result SetHostName(const char* pInHostName, uint32_t hostNameLength) NN_NOEXCEPT;

    /**
     * @brief      Set option value regarding server verification.
     *
     * @param[in]  optionValue    Option value to set
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInvalidOptionType        An invalid combination of VerifyOption was requested.
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultOperationNotAllowed      A specified verify option is not allowed. This error will only occur in Release build.
     *                                            For non-release builds, the application will assert. You may override this behavior by
     *                                            setting nn::ssl::Connection::OptionType_SkipDefaultVerify with nn::ssl::Connection::SetOption()
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *
     * @details    This API sets option value to be used for server verification. Option value
     *             will be selected from @ref nn::ssl::Connection::VerifyOption which can be
     *             handled as bit mask too to specify multiple options.
     */
    nn::Result SetVerifyOption(VerifyOption optionValue) NN_NOEXCEPT;

    /**
     * @brief      Set a buffer used to store server certificate.
     *
     * @param[out] pOutBuffer      Pointer to the buffer in which server certificate should be stored
     * @param[in]  bufferLength    Length of the server cert buffer
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutBuffer
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultBufferAlreadyRegistered  The buffer is already registered
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - The buffer is not set by this API yet
     *
     * @details    This API sets a buffer to the SSL connection which is used to store server
     *             certificate obtained during SSL handshake. Setting buffer by this API doesn't
     *             store anything in the given buffer. Actual server certificate will be stored
     *             when nn::ssl::Connection::DoHandshake is performed. Certificate data will be
     *             stored in DER-encoded format.
     *             If the given buffer is not large enough, nn::ssl::Connection::DoHandshake
     *             returns nn::ssl::ResultInsufficientServerCertBuffer. In this case, the caller
     *             will still get server certificate in the buffer, but data which can't fit in
     *             the buffer gets cut off hence such the data may be handled as invalid data.
     */
    nn::Result SetServerCertBuffer(char* pOutBuffer, uint32_t bufferLength) NN_NOEXCEPT;

    /**
     * @brief      Set blocking mode of I/O APIs of the connection.
     *
     * @param[out] mode    Blocking mode to set
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultInvalidIoMode            Passed IoMode in mode is invalid
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - A socket is already imported by nn::ssl::Connection::SetSocketDescriptor
     *
     * @details    This API configures blocking mode of I/O APIs of the connection.
     *             Blocking mode will be selected from @ref nn::ssl::Connection::IoMode.
     *             By default, I/O APIs behave as blocking API
     *             (nn::ssl::Connection::IoMode_Blocking).
     *
     *             Note that even blocking API will be unblocked after 5 minitutes if there is
     *             no event to process.
     *             This API must be called after the connection is created and the socket
     *             descriptor is set by @ref nn::ssl::Connection::SetSocketDescriptor.
     */
    nn::Result SetIoMode(IoMode mode) NN_NOEXCEPT;

    /**
     * @brief      Set session cache mode.
     *
     * @param[out] mode    Session cache mode to set
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultInvalidSessionCacheMode  Passed IoMode in mode is invalid
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - A socket is already imported by nn::ssl::Connection::SetSocketDescriptor
     *
     * @details    This API configures the session cache mode of the SSL connection.
     *             The session cache mode will be selected from
     *             @ref nn::ssl::Connection::SessionCacheMode. By default, the session cache mode
     *             is set to @ref nn::ssl::Connection::SessionCacheMode_SessionId. This means that
     *             the session cache is enabled by default when the connection is created.
     *
     *             This API must be called after the connection is created and the socket
     *             descriptor is set by @ref nn::ssl::Connection::SetSocketDescriptor.
     */
    nn::Result SetSessionCacheMode(SessionCacheMode mode) NN_NOEXCEPT;

    /**
     * @brief      Set renegotiation mode.
     *
     * @param[out] mode    The renegotiation mode to set
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultInvalidRenegotiationMode Passed IoMode in mode is invalid
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - A socket is already imported by nn::ssl::Connection::SetSocketDescriptor
     *
     * @details    This API configures the renegotiation mode of the SSL connection.
     *             The renegotiation mode will be selected from
     *             @ref nn::ssl::Connection::RenegotiationMode. By default, the renegotiation mode
     *             is set to @ref nn::ssl::Connection::RenegotiationMode_Secure. This means that
     *             secure renegotiation is enabled hence renegotiation will be performed securely
     *             when it is requested by the server.
     *
     *             This API must be called after the connection is created and the socket
     *             descriptor is set by @ref nn::ssl::Connection::SetSocketDescriptor.
     */
    nn::Result SetRenegotiationMode(RenegotiationMode mode) NN_NOEXCEPT;

    /**
     * @brief      Get socket descriptor set by @ref nn::ssl::Connection::SetSocketDescriptor.
     *
     * @param[out] pOutValue    Pointer to the buffer in which obtained socket descriptor should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - The socket descriptor is already registered by @ref nn::ssl::Connection::SetSocketDescriptor
     *
     * @details    This API obtains the socket descriptor which is registered by
     *             @ref nn::ssl::Connection::SetSocketDescriptor.
     */
    nn::Result GetSocketDescriptor(int* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Get host name of the server set by @ref nn::ssl::Connection::SetHostName.
     *
     * @param[out] pOutHostNameBuffer      Pointer to the buffer in which obtained host name should be stored
     * @param[out] pOutHostNameLength      The length of host name stored in the buffer
     * @param[in]  hostNameBufferLength    Length of host name buffer
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutHostNameBuffer or pOutHostNameLength
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultBufferTooShort           Passed buffer is too short to store the host name
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - Host name is already set by @ref nn::ssl::Connection::SetHostName
     *
     * @post
     *             - *pOutHostNameLength == The length of host name stored in pOutHostName
     *
     * @details    This API obtains host name string which is set by
     *             @ref nn::ssl::Connection::SetHostName.
     */
    nn::Result GetHostName(char* pOutHostNameBuffer, uint32_t* pOutHostNameLength, uint32_t hostNameBufferLength) NN_NOEXCEPT;

    /**
     * @brief      Get verify option value set by @ref nn::ssl::Connection::SetVerifyOption.
     *
     * @param[out] pOutValue    Pointer to the buffer in which obtained verify option value should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - Verify option value is already set by @ref nn::ssl::Connection::SetVerifyOption
     *
     * @details    This API obtains verify option value which is set by
     *             @ref nn::ssl::Connection::SetVerifyOption.
     */
    nn::Result GetVerifyOption(VerifyOption* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Get blocking mode of I/O APIs set by @ref nn::ssl::Connection::SetIoMode.
     *
     * @param[out] pOutValue    Pointer to the buffer in which obtained blocking mode should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - Blocking mode is already set by @ref nn::ssl::Connection::SetIoMode
     *
     * @details    This API obtains blocking mode of I/O APIs which is set by
     *             @ref nn::ssl::Connection::SetIoMode.
     */
    nn::Result GetIoMode(IoMode* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Get the session cache mode of the connection set by @ref nn::ssl::Connection::SetSessionCacheMode.
     *
     * @param[out] pOutValue    Pointer to the buffer in which obtained session cache mode should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - A socket is already imported by nn::ssl::Connection::SetSocketDescriptor
     *
     * @details    This API obtains the session cache mode of the connection which is set by
     *             @ref nn::ssl::Connection::SetSessionCacheMode.
     */
    nn::Result GetSessionCacheMode(SessionCacheMode* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Get the renegotiation mode of the connection.
     *
     * @param[out] pOutValue    Pointer to the buffer in which obtained renegotiation mode should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - A socket is already imported by nn::ssl::Connection::SetSocketDescriptor
     *
     * @details    This API obtains the renegotiation mode of the connection which can be set by
     *             @ref nn::ssl::Connection::SetRenegotiationMode.
     */
    nn::Result GetRenegotiationMode(RenegotiationMode* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Flush the session cache tight to the SSL connection context.
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultSocketNotRegistered      No socket has been registered with this connection yet
     * @retval     ResultNoSslConnection          No SSL connection is established yet
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - A socket is already imported by nn::ssl::Connection::SetSocketDescriptor
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     *
     * @details    This API flushes the session cache which is created for the connection context.
     *
     *             Note that what this API actually does is decrementing the reference count of
     *             the session cache. The session cache will not be freed when there are other
     *             connections which are refering the same session cache. Such the session cache
     *             will be freed when this API is caleld by the last connection context.
     *
     *             This API returns ResultSuccess() when there's no session cache associated
     *             to this connection too.
     */
    nn::Result FlushSessionCache() NN_NOEXCEPT;

    /**
     * @brief      Perform SSL handshake with the peer.
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultHostNameNotRegistered    Host name is not registered yet
     * @retval     ResultIoWouldBlock             No event to process, try again later (returns when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                There was no event to process (returns when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower               An error occurred in low level
     * @retval     ResultVerifyCertFailed         Failed to verify the server certificate
     * @retval     ResultConnectionClosed         The underlying connection was closed by the peer
     * @retval     ResultConnectionReset          Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted        Underlying connection was aborted
     * @retval     ResultSocketShutdown           Socket shutdown occurred
     * @retval     ResultNetworkDown              The local network interface was disabled
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - The socket is already registered by nn::ssl::Connection::RegisterSocketDescriptor.
     *             - Verify option is alredy set by nn::ssl::Connection::GetVerifyOption (optional)
     *
     * @details    This API performs SSL handshake with the peer which connection is established
     *             by the socket registered by @ref nn::ssl::Connection::SetSocketDescriptor.
     *             This API returns ResultVerifyCertFailed when certificate validation fails.
     *             The reason of validation failure can be obtained by
     *             nn::ssl::Connection::GetVerifyCertError.
     */
    nn::Result DoHandshake() NN_NOEXCEPT;

    /**
     * @brief      Perform SSL handshake with the peer and store server certificate or the certificate
     *             chain in the buffer set by @ref nn::ssl::Connection::SetServerCertBuffer
     *
     * @param[out] pOutServerCertSize  Pointer to the buffer in which the size of certificate data is stored
     * @param[out] pOutServerCertCount Pointer to the buffer in which the count of the number of certificates in the buffer is returned
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                      Success
     * @retval     ResultResourceBusy                 The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference             The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError           The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized        The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext     The SSL connection is not created yet
     * @retval     ResultInvalidContext               Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered          The socket descriptor is not registered yet
     * @retval     ResultHostNameNotRegistered        Host name is not registered yet (when host name validation is required)
     * @retval     ResultInsufficientServerCertBuffer Failed to store whole certificate data because passed buffer is not large enough (this is just a
     *                                                notification, the caller can continue following process).  Call nn::ssl::Connection::GetNeededServerCertBufferSize()
     *                                                to get the minimum buffer size needed to receive the server certificate.
     * @retval     ResultIoWouldBlock                 No event to process, try again later (returns when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                    There was no event to process (returns when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower                   An error occurred in low level
     * @retval     ResultVerifyCertFailed             Failed to verify the server certificate
     * @retval     ResultConnectionClosed             The underlying connection was closed by the peer
     * @retval     ResultConnectionReset              Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted            Underlying connection was aborted
     * @retval     ResultSocketShutdown               Socket shutdown occurred
     * @retval     ResultNetworkDown                  The local network interface was disabled
     *
     * @details    This API performs SSL handshake with the peer which connection is established
     *             by the socket registered by @ref nn::ssl::Connection::SetSocketDescriptor.
     *             During SSL handshake, the server certificate is stored in the buffer set by
     *             @ref nn::ssl::Connection::SetServerCertBuffer. If OptionType_GetServerCertChain has
     *             been enabled by nn::ssl::Connection::SetOption() for the nn::ssl::Connection, then the
     *             buffer will contain the certificate chain of trust.
     *             In this case @ref nn::ssl::Connection::GetServerCertDetail must
     *             be called to retrieve each certificate individually as the buffer contents are
     *             opaque to the caller. The certificate or certificate chain data is only copied if the
     *             buffer provided is large enough.
     *             This API returns ResultVerifyCertFailed when certificate validation fails.
     *             The reason of validation failure can be obtained by nn::ssl::Connection::GetVerifyCertError.
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - The socket is already registered by nn::ssl::Connection::RegisterSocketDescriptor.
     *             - Server certificate buffer is already set by nn::ssl::Connection::SetServerCertBuffer
     *             - Verify option is alredy set by nn::ssl::Connection::GetVerifyOption (optional)
     *
     * @post       - pOutServerCertSize == The size of certificate data (when nn::ResultSuccess is returned)
     *             - pOutServerCertCount == The number of certificates copied into the buffer (when nn::Result::Success is returned)
     */
    nn::Result DoHandshake(uint32_t* pOutServerCertSize, uint32_t* pOutServerCertCount) NN_NOEXCEPT;

    /**
     * @brief      Perform SSL handshake with the peer and store server certificate or the certificate
     *             chain in the buffer provided.  This may be called instead of calling nn::ssl::Connection::SetServerCertBuffer
     *             before calling nn::ssl::Connection::DoHandshake.
     *
     * @param[out] pOutServerCertCount Pointer to the buffer where the count of certificates in the chain is stored.
     * @param[out] pOutBuffer          Pointer to the buffer where the server certificate chain is stored.
     * @param[out] pOutCertSize        Pointer to the buffer where the total size of certificate data is written.
     * @param[in]  bufferLen           The size of the caller provided buffer, in bytes.
     *
     * @return     Result of the operation is returned.
     * @retval     ResultSuccess                      Success
     * @retval     ResultResourceBusy                 The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference             The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError           The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized        The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext     The SSL connection is not created yet
     * @retval     ResultInvalidContext               Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered          The socket descriptor is not registered yet
     * @retval     ResultHostNameNotRegistered        Host name is not registered yet (when host name validation is required)
     * @retval     ResultInsufficientServerCertBuffer Failed to store whole certificate data because the provided buffer is not large enough (this is just a
     *                                                notification, the caller can continue following process).  Call nn::ssl::Connection::GetNeededServerCertBufferSize()
     *                                                to get the minimum buffer size needed to receive the server certificate.
     * @retval     ResultIoWouldBlock                 No event to process, try again later (returns when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                    There was no event to process (returns when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower                   An error occurred in low level
     * @retval     ResultVerifyCertFailed             Failed to verify the server certificate
     * @retval     ResultConnectionClosed             The underlying connection was closed by the peer
     * @retval     ResultConnectionReset              Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted            Underlying connection was aborted
     * @retval     ResultSocketShutdown               Socket shutdown occurred
     * @retval     ResultNetworkDown                  The local network interface was disabled
     *
     * @details    This API performs SSL handshake with the peer which connection is established
     *             by the socket registered by @ref nn::ssl::Connection::SetSocketDescriptor.
     *             During SSL handshake, the server certificate or the certificate chain is stored in
     *             the provided buffer. This API returns ResultVerifyCertFailed when certificate
     *             validation fails. The reason of validation failure can be obtained by nn::ssl::Connection::GetVerifyCertError.
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - The SSL connection is already created by nn::ssl::Connection::Create
     *             - The socket is already registered by nn::ssl::Connection::RegisterSocketDescriptor.
     *             - Verify option is alredy set by nn::ssl::Connection::GetVerifyOption (optional)
     *
     * @post       - pOutServerCertCount == The number of certificates copied into the provided buffer
     *               pOutBuffer          == The caller provided buffer is filled with certificate data, if nn::ResultSuccess is returned
     *               pOutCertSize        == The size of the data copied into the provided buffer, in bytes
     */
    nn::Result DoHandshake(uint32_t* pOutCertSize, uint32_t* pOutServerCertCount, char* pOutBuffer, uint32_t bufferLen);

    /**
     * @brief      Retrieve the certificate details (size and pointer to DER data) from the input buffer.
     *
     * @param[out] pOutCertDetail  Pointer to a buffer where the server certificate detail is copied.
     * @param[in]  pInBuffer       Pointer to the buffer containing the server certificate chain acquired during handshake.
     * @param[in]  index           The index for the specific server certificate in the chain.  Index values start at 0
     *                             (the connected server) and go up through the trusted signing CA certificate.
     *
     * @return     The status of the operation.
     *
     * @retval     ResultSuccess                      Success
     * @retval     ResultNoServerChain                The buffer does not contain the server certificate chain.
     * @retval     ResultInvalidReference             The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError           The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized        The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext     The SSL connection is not created yet
     * @retval     ResultInvalidContext               Appropriate SSL context is not set yet
     * @retval     ResultInvalidIndex                 An invalid index was specified.
     */
    nn::Result GetServerCertDetail(ServerCertDetail* pOutCertDetail, const char* pInBuffer, uint32_t index);

    /**
     * @brief      Read from the SSL connection.
     *
     * @param[out] pOutBuffer      Pointer to the buffer in which the data read from SSL connection is placed
     * @param[in]  bufferLength    Number of bytes to read (must be bigger than the size of buffer)
     *
     * @return
     *             - return > 0  : Count of bytes read
     *             - return == 0 : All data in the buffer is read and the server closed SSL connection
     *             - return < 0  : Error occured, error reason will be obtained by nn::ssl::Connection::GetLastError,
     *                             errors which could be obtained are listed below
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutBuffer
     * @retval     ResultIoWouldBlock             No event to process, try again later (set when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                There was no event to process (set when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower               An error occurred in low level
     * @retval     ResultConnectionReset          Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted        Underlying connection was aborted
     * @retval     ResultSocketShutdown           Socket shutdown occurred
     * @retval     ResultNetworkDown              The local network interface was disabled
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     */
    int Read(char* pOutBuffer, uint32_t bufferLength) NN_NOEXCEPT;

    /**
     * @brief      Read from the SSL connection.
     *
     * @param[out] pOutBuffer          Pointer to the buffer in which the data read from SSL connection is placed
     * @param[out] pOutReadSizeCourier Pointer to the buffer in which number of bytes read should be stored
     *             - return > 0  : Count of bytes read
     *             - return == 0 : All data in the buffer is read and the server closed SSL connection
     *             - return < 0  : Error occured and appropriate error code is returned in return value
     * @param[in]  bufferLength        Number of bytes to read (must be bigger than the size of buffer)
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutBuffer or pOutReadSizeCourier
     * @retval     ResultIoWouldBlock             No event to process, try again later (set when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                There was no event to process (set when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower               An error occurred in low level
     * @retval     ResultConnectionReset          Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted        Underlying connection was aborted
     * @retval     ResultSocketShutdown           Socket shutdown occurred
     * @retval     ResultNetworkDown              The local network interface was disabled
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     */
    nn::Result Read(char* pOutBuffer, int* pOutReadSizeCourier, uint32_t bufferLength) NN_NOEXCEPT;

    /**
     * @brief      Write to the SSL connection.
     *
     * @param[out] pInBuffer       Pointer to the buffer in which the data sent through SSL connection is placed
     * @param[in]  bufferLength    Number of bytes to write (must be bigger than the size of buffer)
     *
     * @return
     *             - return > 0  : Count of bytes written
     *             - return < 0  : Error occured, error reason will be obtained by nn::ssl::Connection::GetLastError,
     *                             errors which could be obtained are listed below
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pInBuffer
     * @retval     ResultIoWouldBlock             No event to process, try again later (set when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                There was no event to process (set when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower               An error occurred in low level
     * @retval     ResultConnectionReset          Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted        Underlying connection was aborted
     * @retval     ResultSocketShutdown           Socket shutdown occurred
     * @retval     ResultNetworkDown              The local network interface was disabled
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     */
    int Write(const char* pInBuffer, uint32_t bufferLength) NN_NOEXCEPT;

    /**
     * @brief      Write to the SSL connection.
     *
     * @param[out] pInBuffer              Pointer to the buffer in which the data sent through SSL connection is placed
     * @param[out] pOutWrittenSizeCourier Pointer to the buffer in which number of bytes written should be stored
     *             - return > 0  : Count of bytes written
     *             - return < 0  : Error occured, error reason will be obtained by nn::ssl::Connection::GetLastError,
     *                             errors which could be obtained are listed below
     * @param[in]  bufferLength           Number of bytes to write (must be bigger than the size of buffer)
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pInBuffer or pOutWrittenSizeCourier
     * @retval     ResultIoWouldBlock             No event to process, try again later (set when blocking mode is set to nn::ssl::Connection::IoMode_NonBlocking)
     * @retval     ResultIoTimeout                There was no event to process (set when blocking mode is set to nn::ssl::Connection::IoMode_Blocking)
     * @retval     ResultErrorLower               An error occurred in low level
     * @retval     ResultConnectionReset          Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted        Underlying connection was aborted
     * @retval     ResultSocketShutdown           Socket shutdown occurred
     * @retval     ResultNetworkDown              The local network interface was disabled
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     */
    nn::Result Write(const char* pInBuffer, int* pOutWrittenSizeCourier, uint32_t bufferLength) NN_NOEXCEPT;

    /**
     * @brief      Provides the count of bytes available in the internal SSL buffer
     *
     * @return
     *             - return > 0  : Count of bytes available to be read
     *             - return < 0  : Error occured, error reason will be obtained by nn::ssl::Connection::GetLastError,
     *                             errors which could be obtained are listed below
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultErrorLower               An error occurred in low level
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     *
     * @details    This API provides the count of bytes in the internal SSL sockets' receive buffer
     *             that has been received and decrypted. Note that the count of bytes obtained by
     *             this API is not the amount of data which has been received but not decrypted yet
     *             hence getting zero as the pending bytes doesn't mean that a subsequent
     *             nn::ssl::Connection::Read() would block.
     */
    int Pending() NN_NOEXCEPT;

    /**
     * @brief      Provides the count of bytes available in the internal SSL buffer
     *
     * @param[out] pOutValue Pointer to the buffer in which number of bytes pending should be stored
     *             - return > 0  : Count of bytes pending
     *             - return < 0  : Error occured, error reason will be obtained by nn::ssl::Connection::GetLastError,
     *                             errors which could be obtained are listed below
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultErrorLower               An error occurred in low level
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     *
     * @details    This API provides the count of bytes in the internal SSL sockets' receive buffer
     *             that has been received and decrypted. Note that the count of bytes obtained by
     *             this API is not the amount of data which has been received but not decrypted yet
     *             hence getting zero as the pending bytes doesn't mean that a subsequent
     *             nn::ssl::Connection::Read() would block.
     */
    nn::Result Pending(int* pOutValue) NN_NOEXCEPT;


    /**
     * @brief      Read from the SSL connection without removing data from socket buffer.
     *
     * @param[out] pOutBuffer          Pointer to the buffer in which the data read from SSL connection is placed
     * @param[out] pOutReadSizeCourier Pointer to the buffer in which number of bytes read should be stored
     *             - return > 0  : Count of bytes read
     *             - return == 0 : All data in the buffer is read and the server closed SSL connection
     *             - return < 0  : Error occured and appropriate error code is returned in return value
     * @param[in]  bufferLength        Number of bytes to read (must be bigger than the size of buffer)
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultSocketNotRegistered      The socket descriptor is not registered yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutBuffer or pOutReadSizeCourier
     * @retval     ResultErrorLower               An error occurred in low level
     * @retval     ResultConnectionReset          Underlying connection got reset by the peer
     * @retval     ResultConnectionAborted        Underlying connection was aborted
     * @retval     ResultSocketShutdown           Socket shutdown occurred
     * @retval     ResultNetworkDown              The local network interface was disabled
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *             - SSL handshake is already completed by nn::ssl::Connection::DoHandshake
     *
     * @details    This API reads data from the SSL connection without removing data from socket buffer.
     */
    nn::Result Peek(char* pOutBuffer, int* pOutReadSizeCourier, uint32_t bufferLength) NN_NOEXCEPT;

    /**
     * @brief      Wait for the I/O event is ready to process
     *
     * @param[out] pOutEvent      Pointer to the buffer in which the type of event ready to process is stored
     * @param[in]  pInEvent       Pointer to the buffer in which the event type that should be detected is stored
     * @param[in]  msecTimeout    Amount of time to block for I/O event becomes ready in milli second
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidContext           Appropriate SSL context is not set yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutEvent or pInEvent
     * @retval     ResultSocketNotRegistered      Socket descriptor is not registered yet
     * @retval     ResultIoTimeout                No I/O event became ready before timeout
     * @retval     ResultErrorLower               An error occurred in low level
     *
     * @pre
     *             - nn::ssl::Initialize is already called
     *             - SSL connection is already created by nn::ssl::Connection::Create
     *
     * @post
     *             - pOutEvent has I/O event which is ready to process (when ResultSuccess returns)
     *
     * @details    This API returns when I/O event is ready to process on the underlying SSL
     *             connection. It is requried to set I/O event that the caller is interested in
     *             to pInEvent when this API is called. It will be selected from
     *             nn::ssl::connection::PollEvent. It is possible to handle it as bitmask to set
     *             multiple event types.
     *             When ResultSuccess is returned, there is an I/O event ready to process. Caller
     *             is required to check the type of event stored in pOutEvent.
     *             If no I/O event becomes ready during specified amount of time by timeout, this
     *             API returns ResultIoTimeout.
     *             This API can be called regardless of blocking mode of the connection which can
     *             be set by nn::ssl::Connection::SetIoMode.
     */
    nn::Result Poll(PollEvent* pOutEvent, PollEvent* pInEvent, uint32_t msecTimeout) NN_NOEXCEPT;

    /**
     * @brief      Obtains last error occurred in Read/Write APIs.
     *
     * @param[out] pOutValue    Pointer to the buffer in which last error should be stored
     *
     * @return     Result of the process is returned (note that last error is stored in pOutLastError)
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext SSL connection is not created yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     *
     * @details    This API obtains last error occurred in Read/write APIs. Note that last error
     *             maintained inside the system is cleared (nn::ResultSuccess is set) once it is
     *             obtained by this API.
     */
    nn::Result GetLastError(nn::Result* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Obtains error reason of certificate validation failure.
     *
     * @param[out] pOutValue Pointer to the buffer in which verify error should be stored
     *
     * @return     Result of the process is returned (note that verify error is stored in pOutVerifyError)
     *
     * @retval     ResultSuccess                   Success
     * @retval     ResultResourceBusy              The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference          The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError        The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized     SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext  SSL connection is not created yet
     * @retval     ResultInvalidPointer            NULL pointer is passed in pOutValue
     *
     * @details    This API obtains error reason of certificate validation failure occured during
     *             SSL handshake. Note that cerfificate validation failure reason is reset when
     *             nn::ssl::Connection::DoHandshake is called.
     *
     *             This API could return any failure reason which occurred during the SSL handshake
     *             (not specific to only the server certificate verification).
     *             Please refer ResultSslError defined in ssl_Result.public.h for possible failure
     *             reasons.
     */
    nn::Result GetVerifyCertError(nn::Result* pOutValue) NN_NOEXCEPT;

    /**
     * @brief      Obtains multiple error reasons of certificate validation failure.
     *
     * @param[out] pOutResultArray         Pointer to the array of nn::Result in which verify error results will be stored
     * @param[out] pOutResultCountWritten  Pointer to the memory in which the number of verify error results written in pOutResultArray should be stored
     * @param[out] pOutTotalResultCount    Pointer to the memory in which the total number of error results occurred should be stored. This parameter is optional.
     * @param[in]  resultArrayMaxCount     The max number of nn::Result objects that can fit in the array passed in pOutResultArray
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    The SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext The SSL connection is not created yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in to either pOutResultArray or pOutResultCountWritten
     * @retval     ResultBufferTooShort           pOutResultArray is not large enough to hold all the errors that occurred.
     *                                            Note that pOutResultArray is still filled with as many error results that can fit.
     *                                            This api should be called again with an array the length of at least pOutTotalResultCount to
     *                                            obtain all the errors that occurred.
     *
     * @details    This API obtains multiple error results of certificate validation failure that occured during SSL handshake.
     *             If ResultBufferTooShort is returned, pOutResultArray is not large enough to hold all the errors that occurred.
     *             Note that pOutResultArray is still filled with as many error results that can fit. This api should be called
     *             again with an array the length of at least pOutTotalResultCount to obtain all the errors that occurred.
     *
     */
    nn::Result GetVerifyCertErrors(nn::Result* pOutResultArray, uint32_t* pOutResultCountWritten, uint32_t* pOutTotalResultCount, uint32_t resultArrayMaxCount) NN_NOEXCEPT;

    /**
     * @brief      Obtains the minimum size buffer needed to return the server certificate(s)
     *             when calling nn::ssl::Connection::DoHandshake.
     *
     * @param[out] pOutValue Pointer to the buffer in which the size will be copied
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                   Success
     * @retval     ResultResourceBusy              The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference          The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized     SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext  SSL connection is not created yet
     * @retval     ResultInvalidPointer            NULL pointer is passed in pOutValue
     *
     * @details    This API obtains the minimum buffer size needed to retrieve the server certificate(s)
     *             during SSL handshake and ResultInsufficientServerCertBuffer is returned.
     *             Note that the size is reset when nn::ssl::Connection::DoHandshake is called.
     */
    nn::Result GetNeededServerCertBufferSize(uint32_t *pOutValue);

    /**
     * @brief      Get context ID registered to the connection.
     *
     * @param[in]  pOutValue    Pointer to the buffer in which obtained context ID should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                   Success
     * @retval     ResultLibraryNotInitialized     SSL library is not initialized yet
     * @retval     ResultInvalidPointer            NULL pointer is passed in pOutValue
     */
    nn::Result GetContextId(SslContextId* pOutValue) NN_NOEXCEPT;

    /**
     * @brief    Get connection ID of this connection.
     *
     * @param[in]  pOutValue    Pointer to the buffer in which obtained connection ID should be stored
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                   Success
     * @retval     ResultLibraryNotInitialized     SSL library is not initialized yet
     * @retval     ResultInvalidPointer            NULL pointer is passed in pOutValue
     */
    nn::Result GetConnectionId(SslConnectionId* pOutValue) NN_NOEXCEPT;

    /**
     * @brief    Enable/Disable the option of this connection.
     *
     * @param[in]  optionType    The option type to enable/disable
     * @param[in]  enable        The value for the option, it needs to be true when enabling the option
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext SSL connection is not created yet
     * @retval     ResultInvalidOptionType        The specified option type is invalid
     * @retval     ResultSocketAlreadyRegistered  Socket descriptor is already registered
     *
     * @details    This API enables/disables the option for the connection. Supported option types
     *             are defined in nn::ssl::Connection::OptionType.
     *             ### OptionType_DoNotCloseSocket
     *             When this option is enabled (disabled by default), The SSL library will not
     *             close the imported socket by nn::ssl::ConnectionSetSocketDescriptor()
     *             automatically when nn::ssl::Connection::Destroy() is called. Closing such the
     *             socket is caller's responsibility once this option is enabled.
     *             Note that this option needs to be enabled before importing the socket.
     */
    nn::Result SetOption(OptionType optionType, bool enable) NN_NOEXCEPT;

    /**
     * @brief    Get option value of this connection. Please refer nn::ssl::Connection::SetOption
     *           for supported option types.
     *
     * @param[out]  pOutIsEnabled    Pointer to the buffer in which obtained option value is stored
     * @param[in]   optionType       The option type
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext SSL connection is not created yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultInvalidOptionType        The specified option type is invalid
     */
    nn::Result GetOption(bool* pOutIsEnabled, OptionType optionType) NN_NOEXCEPT;

    /**
     * @brief    Gets ssl cipher information
     *
     * @param[out]  pOutValue    Pointer to the cipher info sturcture where info will be stored.
     *
     * @return     Result of the process is returned
     *
     * @retval     ResultSuccess                  Success
     * @retval     ResultResourceBusy             The system is too busy to process the request, try again later
     * @retval     ResultInvalidReference         The system encountered invalid reference, SSL library should be finalized
     * @retval     ResultLibraryNotInitialized    SSL library is not initialized yet
     * @retval     ResultInvalidConnectionContext SSL connection is not created yet
     * @retval     ResultNoSslConnection          No SSL connection is established yet
     * @retval     ResultInvalidPointer           NULL pointer is passed in pOutValue
     * @retval     ResultInternalLogicError       The system failed to process the request due to internal logic error, SSL library should be finalized
     * @retval     ResultErrorLower               An error occurred in low level
     */
    nn::Result GetCipherInfo(CipherInfo* pOutValue) NN_NOEXCEPT;
};

/**
 * @brief    Overloading & operator for nn::ssl::Connection::PollEvent to provide bitmask style operation
 */
inline Connection::PollEvent operator & (Connection::PollEvent lEvent, Connection::PollEvent rEvent)
{
    return static_cast<Connection::PollEvent>((static_cast<uint32_t>(lEvent) & static_cast<uint32_t>(rEvent)));
}

/**
 * @brief    Overloading | operator for nn::ssl::Connection::PollEvent to provide bitmask style operation
 */
inline Connection::PollEvent operator | (Connection::PollEvent lEvent, Connection::PollEvent rEvent)
{
    return static_cast<Connection::PollEvent>((static_cast<uint32_t>(lEvent) | static_cast<uint32_t>(rEvent)));
}

/**
 * @brief    Overloading ^ operator for nn::ssl::Connection::PollEvent to provide bitmask style operation
 */
inline Connection::PollEvent operator ^ (Connection::PollEvent lEvent, Connection::PollEvent rEvent)
{
    return static_cast<Connection::PollEvent>((static_cast<uint32_t>(lEvent) ^ static_cast<uint32_t>(rEvent)));
}

/**
 * @brief    Overloading &= operator for nn::ssl::Connection::PollEvent to provide bitmask style operation
 */
inline Connection::PollEvent& operator &= (Connection::PollEvent &lEvent, Connection::PollEvent rEvent)
{
    lEvent = static_cast<Connection::PollEvent>((static_cast<uint32_t>(lEvent) & static_cast<uint32_t>(rEvent)));
    return lEvent;
}

/**
 * @brief    Overloading |= operator for nn::ssl::Connection::PollEvent to provide bitmask style operation
 */
inline Connection::PollEvent& operator |= (Connection::PollEvent &lEvent, Connection::PollEvent rEvent)
{
    lEvent = static_cast<Connection::PollEvent>((static_cast<uint32_t>(lEvent) | static_cast<uint32_t>(rEvent)));
    return lEvent;
}

/**
 * @brief    Overloading ^= operator for nn::ssl::Connection::PollEvent to provide bitmask style operation
 */
inline Connection::PollEvent& operator ^= (Connection::PollEvent &lEvent, Connection::PollEvent rEvent)
{
    lEvent = static_cast<Connection::PollEvent>((static_cast<uint32_t>(lEvent) ^ static_cast<uint32_t>(rEvent)));
    return lEvent;
}

/**
 * @brief    Overloading & operator for nn::ssl::Connection::VerifyOption to provide bitmask style operation
 */
inline Connection::VerifyOption operator & (Connection::VerifyOption lOption, Connection::VerifyOption rOption)
{
    return static_cast<Connection::VerifyOption>((static_cast<uint32_t>(lOption) & static_cast<uint32_t>(rOption)));
}

/**
 * @brief    Overloading | operator for nn::ssl::Connection::VerifyOption to provide bitmask style operation
 */
inline Connection::VerifyOption operator | (Connection::VerifyOption lOption, Connection::VerifyOption rOption)
{
    return static_cast<Connection::VerifyOption>((static_cast<uint32_t>(lOption) | static_cast<uint32_t>(rOption)));
}

/**
 * @brief    Overloading ^ operator for nn::ssl::Connection::VerifyOption to provide bitmask style operation
 */
inline Connection::VerifyOption operator ^ (Connection::VerifyOption lOption, Connection::VerifyOption rOption)
{
    return static_cast<Connection::VerifyOption>((static_cast<uint32_t>(lOption) ^ static_cast<uint32_t>(rOption)));
}

/**
 * @brief    Overloading &= operator for nn::ssl::Connection::VerifyOption to provide bitmask style operation
 */
inline Connection::VerifyOption& operator &= (Connection::VerifyOption &lOption, Connection::VerifyOption rOption)
{
    lOption = static_cast<Connection::VerifyOption>((static_cast<uint32_t>(lOption) & static_cast<uint32_t>(rOption)));
    return lOption;
}

/**
 * @brief    Overloading |= operator for nn::ssl::Connection::VerifyOption to provide bitmask style operation
 */
inline Connection::VerifyOption& operator |= (Connection::VerifyOption &lOption, Connection::VerifyOption rOption)
{
    lOption = static_cast<Connection::VerifyOption>((static_cast<uint32_t>(lOption) | static_cast<uint32_t>(rOption)));
    return lOption;
}

/**
 * @brief    Overloading ^= operator for nn::ssl::Connection::VerifyOption to provide bitmask style operation
 */
inline Connection::VerifyOption& operator ^= (Connection::VerifyOption &lOption, Connection::VerifyOption rOption)
{
    lOption = static_cast<Connection::VerifyOption>((static_cast<uint32_t>(lOption) ^ static_cast<uint32_t>(rOption)));
    return lOption;
}

}}
