/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/*
    @file
    @brief  Public header with all the socket definitions and function
            declarations that will soon be deprecated.
 */

#pragma once


/**
    @namespace nn::socket
*/
namespace nn     {
namespace socket {

/**
    @brief          Read the global variable @a errno - This API will be deprecated in an upcoming SDK release.

    @return         Returns errno value.

    @details        When a network function call detects an error, it returns an integer value indicating
                    failure (-1) and sets the global variable @a errno accordingly. In Windows builds,
                    @a errno cannot be accessed directly. Instead, this function needs to be called
                    to retrieve that value. It can also be used in NX builds but it is not necessary.

    @note           This function will soon be deprecated. Please use @ref SetLastError().

    @see            SetLastError()
*/
int GetLastErrno() NN_NOEXCEPT;

/**
    @brief          Set the global variable @a errno - This API will be deprecated in an upcoming SDK release.

    @details        The SetLastErrno() routine can be used to set the global variable @a errno.
                    Certain components of the network stack may choose to set or override
                    the global variable @a errno. Since @a errno cannot be accessed directly in
                    Windows builds, this function is necessary in those cases.
                    It can also be used in NX builds but it is not necessary.

    @note           This function will soon be deprecated. Please use @ref GetLastError().

    @see            GetLastError()
*/
void SetLastErrno(int error) NN_NOEXCEPT;

/**
    @brief          Convert address to network format - This API will be deprecated in an upcoming SDK release.

    @param[in]      family              Type of the network address, only AF_INET is supported
    @param[in]      pAddressString      Address in presentation format
    @param[out]     pOutAddressBuffer   Address in network format

    @return         Returns 1 if the address was valid for the specified
                    address family, or 0 if the address was not parseable in the specified
                    address family, or -1 if some system error occurred (in which case the global variable @a errno
                    will have been set).  This function is presently valid for AF_INET.

    @details        The InetPton() function converts a presentation format address (that is,
                    printable form as held in a character string) to network format (usually
                    a struct in_addr or some other internal binary representation, in network
                    byte order). This function can be used without calling @ref Initialize() first.

    @note           This function will soon be deprecated. Please use
                    @ref InetPton(nn::socket::Family, const char*, void*).

    @see            InetPton(nn::socket::Family, const char*, void*)
*/
int InetPton(int family, const char* pAddressString, void* pOutAddressBuffer) NN_NOEXCEPT;

/**
    @brief          Convert string to network address - This API will be deprecated in an upcoming SDK release.

    @param[in]      addressStringBuffer Address string in ASCII format
    @param[out]     pOutNetworkAddress  Address in network format

    @return         Returns 1 if the string was successfully interpreted, or 0 if the string
                    is invalid.

    @details        The InetAton() routine interprets the specified character string as an
                    Internet address, placing the address into the structure provided.
                    This function can be used without calling @ref Initialize() first.

    @note           This function will soon be deprecated. Please use
                    @ref InetAton(const char*, nn::socket::InAddr*).

    @see            InetAton(const char*, nn::socket::InAddr*)
*/
int InetAton(const char* addressStringBuffer, in_addr* pOutNetworkAddress) NN_NOEXCEPT;

/**
    @brief          Convert Internet address into ASCII string - This API will be deprecated in an upcoming SDK release.

    @param[in]      networkAddress      Address in network format

    @return         Returns network address in ASCII representation.

    @details        The InetNtoa() routine takes an Internet address and returns an ASCII
                    string representing the address in '.' notation.
                    The return value is managed internally, so if you wish to maintain the value between calls, a copy must be made.
                    This function is not thread safe so there is a chance for the returned string to
                    not be what is expected if called from another thread at the same time.
                    This function can be used without calling @ref Initialize() first.

    @note           This function will soon be deprecated. Please use
                    @ref InetNtoa(nn::socket::InAddr).

    @see            InetNtoa(nn::socket::InAddr)
*/
char* InetNtoa(in_addr networkAddress) NN_NOEXCEPT;

/**
    @brief          Convert network address to presentation format - This API will be deprecated in an upcoming SDK release.

    @param[in]      family                      Type of network address, only AF_INET is supported
    @param[in]      pNetworkAddress             Address in network format
    @param[out]     addressStringBuffer         Address in presentation format
    @param[in]      addressStringBufferLength   Size of the buffer for address in presentation format

    @return         Returns NULL if a system error occurs (in which case, the global variable @a errno will have
                    been set), or it returns a pointer to the destination string (@a addressStringBuffer).

    @details        The InetNtop() routine converts an address stored at @a pNetworkAddress
                    from network format (usually a struct in_addr or some other binary form, in network byte
                    order) to presentation format (suitable for external display purposes).
                    This function can be used without calling @ref Initialize() first.

    @note           This function will soon be deprecated. Please use
                    @ref InetNtop(nn::socket::Family, const void*, char*, nn::socket::SockLenT).

    @see            InetNtop(nn::socket::Family, const void*, char*, nn::socket::SockLenT)
*/
const char* InetNtop(int family, const void* pNetworkAddress, char* addressStringBuffer, socklen_t addressStringBufferLength) NN_NOEXCEPT;

/**
    @brief          Receives a message from a connection-oriented or connectionless-oriented socket - This API will be deprecated in an upcoming SDK release.
                    It is normally used with connectionless-oriented sockets because it permits
                    the application to retrieve the source address of received data.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     outBuffer           Buffer for message
    @param[in]      outBufferLength     Size of the message to read
    @param[in]      flags               Flags modifying behavior of the function
    @param[out]     pOutAddress         Buffer for message source address
    @param[in,out]  pOutAddressLength   Size of the message source address

    @return         Returns number of bytes received, or -1 if an error occurred.
                    If no messages are available to be received and the peer has performed an orderly shutdown,
                    RecvFrom() shall return 0.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The argument socket is an invalid descriptor
    @post           [ECONNRESET]        The remote socket end is forcibly closed
    @post           [ENETDOWN]          The network interface has been removed and the socket has been shut down.
    @post           [ENOTCONN]          The socket is associated with a connection-oriented
                                        protocol and has not been connected (see @ref Connect()
                                        and @ref Accept()).
    @post           [ENOTSOCK]          The argument socket does not refer to a socket.
    @post           [EAGAIN]            The socket is marked non-blocking, and the receive
                                        operation would block, or a receive timeout had been
                                        set, and the timeout expired before data were
                                        received.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.
    @post           [EINVAL]            A non-zero @a outBufferLength was provided with a null @a outBuffer.
    @post           [EFAULT]            The address range referred to by @a outBuffer and @a outBufferLength does not refer to a valid
                                        part of the process address space.

    @details        The RecvFrom() routine is used to receive messages from a socket,
                    and may be used to receive data on a socket whether or not
                    it is connection-oriented.

                    If @a pOutAddress is not a null pointer and the socket is not connection-oriented,
                    the source address of the message is filled in. The @a pOutAddressLength argument is
                    a value-result argument, initialized to the size of the buffer associated
                    with @a pOutAddress, and modified on return to indicate the actual size of the
                    address stored there. If the actual length of the address is greater than the length
                    of the supplied sockaddr structure, the stored address will be truncated.

                    If no messages are available at the socket, the receive call waits for a
                    message to arrive, unless the socket is non-blocking (see @ref Fcntl()) in
                    which case the value -1 is returned and the global variable @a errno is set
                    to EAGAIN.  The receive calls normally return any data available, up to
                    the requested amount, rather than waiting for receipt of the full amount
                    requested; this behavior is affected by the socket-level options
                    SO_RCVLOWAT and SO_RCVTIMEO described in @ref GetSockOpt().

                    The @ref Select() function call may be used to determine when more data
                    arrives.

                    The flags argument is formed by or'ing one or more
                    of the values:

                    Flag                       | Description
                    ---------------|-----------------------------------
                    MSG_OOB        |  Process out-of-band data
                    MSG_PEEK       |  Peek at incoming message
                    MSG_WAITALL    |  Wait for full request or error
                    MSG_DONTWAIT   |  Do not block

                    The MSG_OOB flag requests receipt of out-of-band data that would not be
                    received in the normal data stream. Some protocols place expedited data
                    at the head of the normal data queue, and thus this flag cannot be used
                    with such protocols.  The MSG_PEEK flag causes the receive operation to
                    return data from the beginning of the receive queue without removing that
                    data from the queue. Thus, a subsequent receive call will return the
                    same data.  The MSG_WAITALL flag requests that the operation block until
                    the full request is satisfied. However, the call may still return less
                    data than requested if an error or disconnect occurs,
                    or the next data to be received is of a different type than that
                    returned.  The MSG_DONTWAIT flag requests the call to return when it
                    would block otherwise.  If no data is available, the global variable @a errno
                    is set to EAGAIN.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref RecvFrom(int, void*, size_t, nn::socket::MsgFlag, nn::socket::SockAddr*, nn::socket::SockLenT*).

    @see            RecvFrom(int, void*, size_t, nn::socket::MsgFlag, nn::socket::SockAddr*, nn::socket::SockLenT*)

 */
ssize_t RecvFrom(int socket, void* outBuffer, size_t outBufferLength, int flags, sockaddr* pOutAddress, socklen_t* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Receives a message from a connection-mode or connectionless-mode socket - This API will be deprecated in an upcoming SDK release.
                    It is normally used with connected sockets because it does not permit
                    the application to retrieve the source address of received data.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     outBuffer           Buffer for message
    @param[in]      outBufferLength     Size of the message to read
    @param[in]      flags               Flags modifying behavior of the function

    @return         Returns number of bytes received, or -1 if an error occurred.
                    If no messages are available to be received and the peer has performed an orderly shutdown,
                    Recv() shall return 0.

    @details        The Recv() routine is similar in behavior to @ref RecvFrom(); however,
                    it is normally used only on a connected socket (see @ref Connect()).

    @note           This function will soon be deprecated. Please use
                    @ref Recv(int, void*, size_t, nn::socket::MsgFlag).

    @see            Recv(int, void*, size_t, nn::socket::MsgFlag)
*/
ssize_t Recv(int socket, void* outBuffer, size_t outBufferLength, int flags) NN_NOEXCEPT;

/**
    @brief          Send a message from a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      buffer              Buffer containing the message to send
    @param[in]      bufferLength        Size of the message
    @param[in]      flags               Flags modifying behavior of the function
    @param[in]      pAddress            Buffer containing address of the recipient
    @param[in]      addressLength       Size of the recipient address

    @return         Returns the number of characters sent, or -1 if an error occurred.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             An invalid descriptor was specified.
    @post           [EACCES]            The destination address is a broadcast address, and
                                        SO_BROADCAST has not been set on the socket.
    @post           [ENOTSOCK]          The argument socket is not a socket.
    @post           [EMSGSIZE]          The socket requires that message be sent atomically,
                                        and the size of the message to be sent made this
                                        impossible.
    @post           [EAGAIN]            The socket is marked non-blocking and the requested
                                        operation would block.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.
    @post           [ENOBUFS]           The system was unable to allocate an internal buffer.
                                        The operation may succeed when buffers become available.
    @post           [EHOSTUNREACH]      The remote host was unreachable.
    @post           [EISCONN]           A destination address was specified and the socket is already connected.
    @post           [ECONNREFUSED]      The socket received an ICMP destination unreachable
                                        message from the last message sent.  This typically
                                        means that the receiver is not listening on the remote port.
    @post           [EHOSTDOWN]         The remote host was down.
    @post           [ENETDOWN]          The local network interface is down.
    @post           [ENETUNREACH]       The network is not reachable from this host.
    @post           [EPIPE]             The socket is unable to send anymore data
                                        (SBS_CANTSENDMORE has been set on the socket).  This
                                        typically means that the socket is not connected.
    @post           [EINVAL]            A non-zero @a bufferLength was provided with a null @a buffer.
    @post           [EFAULT]            The address range referred to by @a buffer and @a bufferLength does not refer to a valid
                                        part of the process address space.

    @details        The SendTo() routine is used to transmit a message to another socket.
                    The address of the target is given by @a pAddress with @a addressLength specifying its size.
                    The length of the message is given by @a bufferLength. If the message is too long to
                    pass atomically through the underlying protocol, the error EMSGSIZE is
                    returned, and the message is not transmitted.

                    If no message space is available at the socket to hold the message to be
                    transmitted, then SendTo() normally blocks, unless the socket has been
                    placed in non-blocking I/O mode. The @ref Select() call may be used
                    to determine when it is possible to send more data.

                    The flags argument may include one or more of the following:

                    Flag                          |   Description
                    ----------------|-------------------------------------------
                    MSG_OOB         |  Process out-of-band data
                    MSG_DONTROUTE   |  Bypass routing, use direct interface
                    MSG_EOR         |  Data completes record
                    MSG_EOF         |  Data completes transaction

                    The flag MSG_OOB is used to send "out-of-band" data on sockets that
                    support this notion (e.g. SOCK_STREAM); the underlying protocol must also
                    support "out-of-band" data.  MSG_EOR is used to indicate a record mark
                    for protocols which support the concept.  MSG_EOF requests that the
                    sender side of a socket be shut down, and that an appropriate indication
                    be sent at the end of the specified data; this flag is only implemented
                    for SOCK_STREAM sockets in the PF_INET protocol family.  MSG_DONTROUTE is
                    usually used only by diagnostic or routing programs.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref SendTo(int, const void*, size_t, nn::socket::MsgFlag, const nn::socket::SockAddr*, nn::socket::SockLenT).

    @see            SendTo(int, const void*, size_t, nn::socket::MsgFlag, const nn::socket::SockAddr*, nn::socket::SockLenT)
*/
ssize_t SendTo(int socket, const void* buffer, size_t bufferLength, int flags, const sockaddr* pAddress, socklen_t addressLength) NN_NOEXCEPT;

/**
    @brief          Send a message from a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      buffer              Buffer containing the message to send
    @param[in]      bufferLength        Size of the message
    @param[in]      flags               Flags modifying behavior of the function

    @return         Returns the number of characters sent, or -1 if an error occurred.

    @details        The Send() routine is similar in behavior to @ref SendTo(); however,
                    it is normally used only on a connected socket (see @ref Connect()).

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Send(int, const void*, size_t, nn::socket::MsgFlag).

    @see            Send(int, const void*, size_t, nn::socket::MsgFlag)

*/
ssize_t Send(int socket, const void* buffer, size_t bufferLength, int flags) NN_NOEXCEPT;

/**
    @brief          Receive a message from a socket - This API will be deprecated in an upcoming SDK release.

    @return         Returns -1, the global variable @a errno is set to [EOPNOTSUPP].

    @details        The RecvMsg() routine is used to receive a message from a socket.
                    It uses msghdr structure to minimize the number of directly supplied arguments.

                    In the current implementation this function is disabled.

    @note           This function will soon be deprecated with no replacement.

    @see            Recv(int, void*, size_t, nn::socket::MsgFlag),
                    RecvFrom(int, void*, size_t, nn::socket::MsgFlag, nn::socket::SockAddr*, nn::socket::SockLenT*)
*/
ssize_t RecvMsg(int socket, msghdr* pOutMessage, int flags) NN_NOEXCEPT;

/**
    @brief          Send a message from a socket - This API will be deprecated in an upcoming SDK release.

    @return         Returns -1, the global variable @a errno is set to [EOPNOTSUPP].

    @details        The SendMsg() routine is used to send a message from a socket.
                    It uses msghdr structure to minimize the number of directly supplied arguments.

                    In the current implementation this function is disabled.

    @note           This function will soon be deprecated with no replacement.

    @see            Send(int, const void*, size_t, nn::socket::MsgFlag),
                    SendTo(int, const void*, size_t, nn::socket::MsgFlag, const nn::socket::SockAddr*, nn::socket::SockLenT)
*/
ssize_t SendMsg(int socket, const msghdr* pMessage, int flags) NN_NOEXCEPT;

/**
    @brief          Accept connection on a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket desciptor obtained with @ref Socket() or @ref Accept()
    @param[out]     pOutAddress         Address of the remote host
    @param[in,out]  pOutAddressLength   Size of the buffer containing remote host address

    @return         Returns -1 on error. If it succeeds, it returns a non-negative integer that is a
                    descriptor for the accepted socket.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The descriptor is invalid.
    @post           [EINTR]             The accept operation was interrupted.
    @post           [EMFILE]            The per-process descriptor table is full.
    @post           [ENOTSOCK]          The descriptor references a file, not a socket.
    @post           [EINVAL]            @ref Listen() has not been called on the socket descriptor.
    @post           [EINVAL]            A non-zero @a pOutAddressLength was passed with a null @a pOutAddress.
    @post           [EWOULDBLOCK] or [EAGAIN] The socket is marked non-blocking and no connections are present to be accepted.
    @post           [ECONNABORTED]      A connection arrived, but it was closed while waiting on the listen queue.
    @post           [EFAULT]            The address range referred to by @a pOutAddress and @a pOutAddressLength
                                        does not refer to a valid part of the process address space.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.

    @details        The Accept() routine can be used to accept connection on a socket.
                    The argument @a socket is a socket that has been created with @ref Socket(), bound to
                    an address with @ref Bind(), and is listening for connections after a
                    @ref Listen(). The Accept() function call extracts the first connection
                    request on the queue of pending connections, creates a new socket, and
                    allocates a new file descriptor for the socket which inherits the state
                    of the O_NONBLOCK property from the original socket.

                    If no pending connections are present on the queue, and the original
                    socket is not marked as non-blocking, Accept() blocks the caller until a
                    connection is present. If the original socket is marked non-blocking and
                    no pending connections are present on the queue, Accept() returns an
                    error as described below. The accepted socket may not be used to accept
                    more connections. The original socket remains open.

                    The argument @a pOutAddress is a result argument that is filled-in with the address
                    of the connecting entity, as known to the communications layer. The
                    exact format of the @a pOutAddress argument is determined by the domain in which
                    the communication is occurring. A null pointer may be specified for @a pOutAddress
                    if the address information is not desired; in this case, @a pOutAddressLength is not
                    used and should also be null. Otherwise, the @a pOutAddressLength argument is a
                    value-result argument; it should initially contain the amount of space
                    pointed to by @a pOutAddress; on return it will contain the actual length (in
                    bytes) of the address returned. This call is used with connection-based
                    socket types, currently with SOCK_STREAM.

                    It is possible to @ref Select() a socket for the purposes of doing an
                    Accept() by selecting it for read.

                    When using Accept(), portable programs should not rely on the O_NONBLOCK
                    property and the signal destination being inherited, but
                    should set them explicitly using @ref Fcntl().

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Accept(int, nn::socket::SockAddr*, nn::socket::SockLenT*).

    @see            Accept(int, nn::socket::SockAddr*, nn::socket::SockLenT*)

*/
int Accept(int socket, sockaddr* pOutAddress, socklen_t* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Assign a local protocol address to a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      pAddress            Buffer containing address
    @param[in]      addressLength       Length of the address

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.
    @post           [EBADF]             The socket argument is not a valid descriptor.
    @post           [EINVAL]            The socket is already bound to an address, and the
                                        protocol does not support binding to a new address; or
                                        the socket has been shut down.
    @post           [EINVAL]            The @a addressLength argument is not a valid length for the
                                        address family.
    @post           [EINVAL]            The @a pAddress argument is null.
    @post           [ENOTSOCK]          The @a socket argument is not a socket.
    @post           [EADDRNOTAVAIL]     The specified address is not available from the local machine.
    @post           [EADDRINUSE]        The specified address is already in use.
    @post           [EAFNOSUPPORT]      Addresses in the specified address family cannot be
                                        used with this socket.
    @post           [EACCES]            The requested address is protected, and the current
                                        user has inadequate permission to access it.
    @post           [EFAULT]            The address range referred to by @a pAddress and @a addressLength
                                        does not refer to a valid part of the process address space.

    @details        The Bind() routine assigns the local protocol address to a socket.
                    When a socket is created with @ref Socket() it exists in an address family
                    space but has no protocol address assigned. The Bind() function call
                    requests that address stored at @a pAddress be assigned to the socket.

                    For maximum portability, you should always zero the socket address
                    structure before populating it and passing it to Bind().

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Bind(int, const nn::socket::SockAddr*, nn::socket::SockLenT).

    @see            Bind(int, const nn::socket::SockAddr*, nn::socket::SockLenT)

*/
int Bind(int socket, const sockaddr* pAddress, socklen_t addressLength) NN_NOEXCEPT;

/**
    @brief          Initiate a connection on a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      pAddress            Buffer containing address of the remote host
    @param[in]      addressLength       Size of the buffer containing address of the remote host

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The @a socket argument is not a valid descriptor.
    @post           [EINVAL]            The @a addressLength argument is not a valid length for the address family.
    @post           [EINVAL]            The @a pAddress argument is null.
    @post           [EFAULT]            The address range referred to by @a pAddress and @a addressLength does not refer to a valid
                                        part of the process address space.
    @post           [ENOTSOCK]          The @a socket argument is a descriptor for a file, not a socket.
    @post           [EADDRNOTAVAIL]     The specified address is not available on this machine.
    @post           [EAFNOSUPPORT]      Addresses in the specified address family cannot be used with this socket.
    @post           [EISCONN]           The socket is already connected.
    @post           [ETIMEDOUT]         Connection establishment timed out without establishing a connection.
    @post           [ECONNREFUSED]      The attempt to connect was forcefully rejected.
    @post           [ECONNRESET]        The connection was reset by the remote host.
    @post           [ENETUNREACH]       The network is not reachable from this host.
    @post           [EHOSTUNREACH]      The remote host is not reachable from this host.
    @post           [EADDRINUSE]        The address is already in use.
    @post           [EINPROGRESS]       The socket is non-blocking and the connection cannot
                                        be completed immediately.  It is possible to @ref Select()
                                        for completion by selecting the socket for writing.
    @post           [EALREADY]          A previous connection attempt has not yet been completed.
    @post           [EACCES]            An attempt is made to connect to a broadcast address
                                        (obtained through the INADDR_BROADCAST constant or the
                                        INADDR_NONE return value) through a socket that does
                                        not provide broadcast functionality.
    @post           [EAGAIN]            An auto-assigned port number was requested but no
                                        auto-assigned ports are available.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.


    @details        The Connect() routine initiates connection on a socket.
                    If socket is of type SOCK_DGRAM, this call specifies
                    the peer with which the socket is to be associated; this address is
                    that to which datagrams are to be sent, and the only address from which
                    datagrams are to be received.

                    If the socket is of type SOCK_STREAM, this call attempts to make
                    a connection to another socket. The other socket
                    is specified by name, which is an address in the communications space of
                    the socket. Each communications space interprets the name argument in
                    its own way.

                    Generally, stream sockets may successfully connect only
                    once; datagram sockets may use connect multiple times to change their
                    association. Datagram sockets may dissolve the association by connecting
                    to an invalid address, such as a null address.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Connect(int, const nn::socket::SockAddr*, nn::socket::SockLenT).

    @see            Connect(int, const nn::socket::SockAddr*, nn::socket::SockLenT)
*/
int Connect(int socket, const sockaddr* pAddress, socklen_t addressLength) NN_NOEXCEPT;

/**
    @brief          Get name of connected peer - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     pOutAddress         Buffer for storing name of the peer
    @param[in,out]  pOutAddressLength   Size of the buffer

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The argument @a socket is not a valid descriptor.
    @post           [ECONNRESET]        The connection has been reset by the peer.
    @post           [EINVAL]            The value of the @a pOutAddressLength argument is not valid.
    @post           [EINVAL]            The @a pOutAddress argument or @a pOutAddressLength argument are null.
    @post           [EFAULT]            The address range referred to by @a outAddress and @a pOutAddressLength
                                        does not refer to a valid part of the process address space.
    @post           [ENOTSOCK]          The argument @a socket is not a socket.
    @post           [ENOTCONN]          The socket is not connected.
    @post           [ENOBUFS]           Insufficient resources were available in the system to
                                        perform the operation.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.

    @details        The GetPeerName() routine returns the name of the peer connected to
                    socket. The @a pOutAddressLength argument should be initialized to indicate the
                    amount of space pointed to by @a pOutAddress. On return it contains the actual
                    size of the name returned (in bytes). The name is truncated if the
                    buffer provided is too small.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref GetPeerName(int, nn::socket::SockAddr*, nn::socket::SockLenT*).

    @see            GetPeerName(int, nn::socket::SockAddr*, nn::socket::SockLenT*)
*/
int GetPeerName(int socket, sockaddr* pOutAddress, socklen_t* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Get socket name - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     pOutAddress         Buffer for name
    @param[in,out]  pOutAddressLength   Size of the buffer for name

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The argument @a socket is not a valid descriptor.
    @post           [ECONNRESET]        The connection has been reset by the peer.
    @post           [EINVAL]            The value of the @a pOutAddressLength argument is not valid.
    @post           [EINVAL]            The @a pOutAddress argument or @a pOutAddressLength argument are null.
    @post           [EFAULT]            The address range referred to by @a outAddress and @a pOutAddressLength
                                        does not refer to a valid part of the process address space.
    @post           [ENOTSOCK]          The argument @a socket is not a socket.
    @post           [ENOBUFS]           Insufficient resources were available in the system to
                                        perform the operation.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.

    @details        The GetSockName() routine returns the current name for the specified
                    socket. The @a pOutAddressLength argument should be initialized to indicate the
                    amount of space pointed to by @a pOutAddress. On return it contains the actual
                    size of the name returned (in bytes).

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref GetSockName(int, nn::socket::SockAddr*, nn::socket::SockLenT*).

    @see            GetSockName(int, nn::socket::SockAddr*, nn::socket::SockLenT*)
*/
int GetSockName(int socket, sockaddr* pOutAddress, socklen_t* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Set options on a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      level               Level of the option
    @param[in]      optionName          Type of the option
    @param[in]      pOptionValue        Buffer containing option value
    @param[in]      optionLength        Size of the buffer with option value

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The argument @a socket is not a valid descriptor.
    @post           [ENOTSOCK]          The argument @a socket is not a socket.
    @post           [ENOPROTOOPT]       The @a optionName is unknown at the level indicated.
    @post           [EINVAL]            The @a pOptionValue argument is null or the value of the @a optionLength argument is not valid.
    @post           [EFAULT]            The address range referred to by @a pOptionValue and @a optionLength
                                        does not refer to a valid part of the process address space.
    @post           [EPERM]             User has insufficient privileges to carry out the
                                        requested operation.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.

    @details        The SetSockOpt() routine manipulates the options associated with a socket.
                    Options may exist at multiple protocol levels; they are always present at
                    the uppermost "socket" level.

                    When manipulating socket options the level at which the option resides
                    and the name of the option must be specified. To manipulate options at
                    the socket level, level is specified as SOL_SOCKET.  To manipulate
                    options at any other level the protocol number of the appropriate protocol
                    controlling the option is supplied. For example, to indicate that an
                    option is to be interpreted by the TCP protocol, level should be set to
                    the protocol number of TCP.

                    The following are supported levels:

                    SOL_IP, SOL_ICMP, SOL_TCP, SOL_UDP, SOL_SOCKET.

                    The @a pOptionValue and @a optionLength arguments are used to access option values.
                    The @a optionName argument and any specified options are passed uninterpreted
                    to the appropriate protocol module for interpretation.

                    Most socket-level options utilize an int argument for option value. For
                    SetSockOpt(), the argument should be non-zero to enable a boolean option,
                    or zero if the option is to be disabled.  SO_LINGER uses a struct linger
                    argument which specifies the desired state of the option and the linger
                    interval (see below). SO_SNDTIMEO and SO_RCVTIMEO use a struct timeval argument.

                    @code
                    struct linger
                    {
                        int         l_onoff;    // option on/off
                        int         l_linger;   // linger time
                    }

                    struct timeval
                    {
                        time_t      tv_sec;     // seconds
                        suseconds_t tv_usec;    // microseconds
                    }
                    @endcode

                    The following options are recognized at the socket level.

                    Option                        |    Short description
                    ----------------|--------------------------------------------------
                    SO_REUSEADDR    |    Enables local address reuse
                    SO_REUSEPORT    |    Enables duplicate address and port bindings
                    SO_KEEPALIVE    |    Enables keep connections alive
                    SO_DONTROUTE    |    Enables routing bypass for outgoing messages
                    SO_LINGER       |    Linger on close if data present
                    SO_BROADCAST    |    Enables permission to transmit broadcast messages
                    SO_OOBINLINE    |    Enables reception of out-of-band data in band
                    SO_SNDBUF       |    Set buffer size for output
                    SO_RCVBUF       |    Set buffer size for input
                    SO_SNDLOWAT     |    Set minimum count for output (not supported on Windows)
                    SO_RCVLOWAT     |    Set minimum count for input (not supported on Windows)
                    SO_SNDTIMEO     |    Set timeout value for output
                    SO_RCVTIMEO     |    Set timeout value for input
                    SO_ACCEPTFILTER |    Set accept filter on listening socket
                    SO_ACCEPTCONN   |    Get listening status of the socket (get only, not supported on Windows)
                    SO_TYPE         |    Get the type of the socket (get only, not supported on Windows)
                    SO_PROTOCOL     |    Get the protocol number for the socket (get only)
                    SO_ERROR        |    Get and clear error on the socket (get only)
                    SO_LISTENQLIMIT |    Get backlog limit of the socket (get only)
                    SO_LISTENQLEN   |    Get complete queue length of the socket (get only)
                    SO_LISTENINCQLEN|    Get incomplete queue length of the socket (get only)

                    SO_REUSEADDR indicates that the rules used in validating addresses supplied
                    in a @ref Bind() system call should allow reuse of local addresses.

                    SO_REUSEPORT allows completely duplicate bindings by multiple processes
                    if they all set SO_REUSEPORT before binding the port.  This option permits
                    multiple instances of a program to each receive UDP/IP multicast or
                    broadcast datagrams destined for the bound port.

                    SO_KEEPALIVE enables the periodic transmission of messages on a connected
                    socket. Should the connected party fail to respond to these messages,
                    the connection is considered broken and processes using the socket are
                    notified via a SIGPIPE signal when attempting to send data.

                    SO_DONTROUTE indicates that outgoing messages should bypass the standard
                    routing facilities. Instead, messages are directed to the appropriate
                    network interface according to the network portion of the destination
                    address.

                    SO_LINGER controls the action taken when unsent messages are queued on
                    socket and a @ref Close() is performed. If the socket promises reliable
                    delivery of data and SO_LINGER is set, the system will block the process
                    on the @ref Close() attempt until it is able to transmit the data or until it
                    decides it is unable to deliver the information (a timeout period, termed
                    the linger interval, is specified in seconds in the SetSockOpt() system
                    call when SO_LINGER is requested).  If SO_LINGER is disabled and a
                    @ref Close() is issued, the system will process the close in a manner that
                    allows the process to continue as quickly as possible.

                    The option SO_BROADCAST requests permission to send broadcast datagrams
                    on the socket.

                    With protocols that support out-of-band data, the SO_OOBINLINE option
                    requests that out-of-band data be placed in the normal data input queue
                    as received; it will then be accessible with @ref Recv() or @ref Read() calls
                    without the MSG_OOB flag.  Some protocols always behave as if this option
                    is set.

                    SO_SNDBUF and SO_RCVBUF are options to adjust the normal buffer sizes
                    allocated for output and input buffers, respectively. The buffer size
                    may be increased for high-volume connections, or may be decreased to
                    limit the possible backlog of incoming data.

                    SO_SNDLOWAT is an option to set the minimum count for output operations.
                    Most output operations process all of the data supplied by the call,
                    delivering data to the protocol for transmission and blocking as necessary
                    for flow control. Nonblocking output operations will process as
                    much data as permitted subject to flow control without blocking, but will
                    process no data if flow control does not allow the smaller of the low
                    water mark value or the entire request to be processed. A @ref Select()
                    operation testing the ability to write to a socket will return true only
                    if the low water mark amount could be processed. The default value for
                    SO_SNDLOWAT is set to a convenient size for network efficiency, often
                    1024.

                    SO_RCVLOWAT is an option to set the minimum count for input operations.
                    In general, receive calls will block until any (non-zero) amount of data
                    is received, then return with the smaller of the amount available or the
                    amount requested.  The default value for SO_RCVLOWAT is 1.  If
                    SO_RCVLOWAT is set to a larger value, blocking receive calls normally
                    wait until they have received the smaller of the low water mark value or
                    the requested amount. Receive calls may still return less than the low
                    water mark if an error occurs, or the type of data
                    next in the receive queue is different from that which was returned.

                    SO_SNDTIMEO is an option to set a timeout value for output operations.
                    It accepts a struct timeval argument with the number of seconds and
                    microseconds used to limit waits for output operations to complete. If a
                    send operation has blocked for this much time, it returns with a partial
                    count or with the error EWOULDBLOCK if no data were sent.  In the current
                    implementation, this timer is restarted each time additional data are
                    delivered to the protocol, implying that the limit applies to output portions
                    ranging in size from the low water mark to the high water mark for output.

                    SO_RCVTIMEO is an option to set a timeout value for input operations.  It
                    accepts a struct timeval argument with the number of seconds and
                    microseconds used to limit waits for input operations to complete. In
                    the current implementation, this timer is restarted each time additional
                    data are received by the protocol, and thus the limit is in effect an
                    inactivity timer. If a receive operation has been blocked for this much
                    time without receiving additional data, it returns with a short count or
                    with the error EWOULDBLOCK if no data were received.

                    SO_ACCEPTCONN, SO_TYPE, SO_PROTOCOL (and its alias SO_PROTOTYPE) and
                    SO_ERROR are options used only with @ref GetSockOpt().  SO_ACCEPTCONN returns
                    whether the socket is currently accepting connections, that is, whether
                    or not the @ref Listen() system call was invoked on the socket.  SO_TYPE
                    returns the type of the socket, such as SOCK_STREAM; it is useful for
                    servers that inherit sockets on startup.  SO_PROTOCOL returns the protocol
                    number for the socket, for AF_INET and AF_INET6 address families.
                    SO_ERROR returns any pending error on the socket and clears the error
                    status. It may be used to check for asynchronous errors on connected
                    datagram sockets or for other asynchronous errors.

                    SO_LISTENQLIMIT returns the maximal number of queued connections, as set
                    by @ref Listen().  SO_LISTENQLEN returns the number of unaccepted complete
                    connections.  SO_LISTENINCQLEN returns the number of unaccepted incomplete connections.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref SetSockOpt(int, nn::socket::Level, nn::socket::Option, const void*, nn::socket::SockLenT).

    @see            SetSockOpt(int, nn::socket::Level, nn::socket::Option, const void*, nn::socket::SockLenT)
*/
int SetSockOpt(int socket, int level, int optionName, const void* pOptionValue, socklen_t optionLength) NN_NOEXCEPT;

/**
    @brief          Get socket options - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      level               Level of the option
    @param[in]      optionName          Type of the option
    @param[out]     pOutOptionValue     Buffer containing option value
    @param[in, out] pOutOptionLength    Size of the buffer with option value

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]             The argument @a socket is not a valid descriptor.
    @post           [ENOTSOCK]          The argument @a socket is not a socket.
    @post           [ENOPROTOOPT]       The @a optionName is unknown at the level indicated.
    @post           [EINVAL]            The @a pOutOptionValue argument or @a pOutOptionLength argument are null.
    @post           [EINVAL]            The value of the @a pOutOptionLength argument is not valid.
    @post           [EFAULT]            The address range referred to by @a pOutOptionValue and the value of @a pOutOptionLength
                                        does not refer to a valid part of the process address space.
    @post           [EAGAIN]            Resources to complete the request are temporarily unavailable.

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @details        The GetSockOpt() routine obtains socket options, See @ref SetSockOpt() for
                    supported options and their description.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref GetSockOpt(int, nn::socket::Level, nn::socket::Option, void*, nn::socket::SockLenT*).

    @see            GetSockOpt(int, nn::socket::Level, nn::socket::Option, void*, nn::socket::SockLenT*)
*/
int GetSockOpt(int socket, int level, int optionName, void* pOutOptionValue, socklen_t* pOutOptionLength) NN_NOEXCEPT;

/**
    @brief          Disable sends and/or receives on a socket - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      how             Shutdown method

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]         The @a socket argument is not a valid file descriptor.
    @post           [EINVAL]        The @a how argument is invalid.
    @post           [EOPNOTSUPP]    The socket associated with the file descriptor @a socket does
                                    not support this operation.
    @post           [ENOTCONN]      The @a socket argument specifies a SOCK_STREAM socket which is not connected.
    @post           [ENOTSOCK]      The @a socket argument does not refer to a socket.
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The shutdown() routine disables sends or receives on a socket. The
                    how argument specifies the type of shutdown.

                    Possible values are:

                    SHUT_RD         Further receives will be disallowed.

                    SHUT_WR         Further sends will be disallowed.  This may cause actions specific to the protocol family of the socket to happen.

                    SHUT_RDWR       Further sends and receives will be disallowed.  Implies SHUT_WR.

                    If the file descriptor @a socket is associated with a SOCK_STREAM socket, all or
                    part of the full-duplex connection will be shut down.

                    IMPLEMENTATION NOTES

                    The following protocol specific actions apply to the use of SHUT_WR (and
                    potentially also SHUT_RDWR), based on the properties of the socket
                    associated with the file descriptor socket.

                    Domain               |Type                   |Protocol                      |Return value and action
                    ------------|------------|---------------|--------------------------------------------------------------
                    PF_INET     |SOCK_DGRAM  |IPPROTO_SCTP   |Return -1. The global variable @a errno will be set to EOPNOTSUPP
                    PF_INET     |SOCK_DGRAM  |IPPROTO_UDP    |Return 0. ICMP messages will not be generated
                    PF_INET     |SOCK_STREAM |IPPROTO_SCTP   |Return 0. Send queued data and tear down association
                    PF_INET     |SOCK_STREAM |IPPROTO_TCP    |Return 0. Send queued data, wait for ACK, then send FIN
                    PF_INET6    |SOCK_DGRAM  |IPPROTO_SCTP   |Return -1. The global variable @a errno will be set to EOPNOTSUPP
                    PF_INET6    |SOCK_DGRAM  |IPPROTO_UDP    |Return 0. ICMP messages will not be generated
                    PF_INET6    |SOCK_STREAM |IPPROTO_SCTP   |Return 0. Send queued data and tear down association
                    PF_INET6    |SOCK_STREAM |IPPROTO_TCP    |Return 0. Send queued data, wait for ACK, then send FIN

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Shutdown(int, nn::socket::ShutdownMethod).

    @see            Shutdown(int, nn::socket::ShutdownMethod)
*/
int Shutdown(int socket, int how) NN_NOEXCEPT;

/**
    @brief          Create endpoint for communication - This API will be deprecated in an upcoming SDK release.

    @param[in]      domain          Communication domain
    @param[in]      type            Socket type
    @param[in]      protocol        Protocol type

    @return         Returns -1 on error. If it succeeds, it returns a non-negative integer that is a
                    descriptor for the created socket.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EACCES]        Permission to create a socket of the specified type
                                    and/or protocol is denied.
    @post           [EAFNOSUPPORT]  The address family (domain) is not supported or the
                                    specified domain is not supported by this protocol family.
    @post           [EMFILE]        The per-process descriptor table is full.
    @post           [ENOBUFS]       Insufficient buffer space is available.  The socket
                                    cannot be created until sufficient resources are
                                    freed.
    @post           [EPERM]         User has insufficient privileges to carry out the
                                    requested operation.
    @post           [EPROTONOSUPPORT]  The protocol type or the specified protocol is not
                                       supported within this domain.
    @post           [EPROTOTYPE]    The socket type is not supported by the protocol.
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The Socket() routine creates an endpoint for communication and returns a descriptor.
                    The @a domain argument specifies a communications domain within which communication will take place;
                    this selects the protocol family which should be used.

                    The currently understood formats are:

                    Format                 |      Description
                    ---------------|------------------------------------
                    PF_INET        |  Internet version 4 protocols
                    PF_ROUTE       |  Internal Routing protocol
                    PF_LINK        |  Link layer interface

                    The socket has the indicated @a type, which specifies the semantics of
                    communication.

                    Currently defined types are:

                    Type                      |  Description
                    ---------------|-----------------------------------
                    SOCK_STREAM    |  Stream socket,
                    SOCK_DGRAM     |  Datagram socket,
                    SOCK_RAW       |  Raw-protocol interface
                    SOCK_RDM       |  Reliably-delivered packet
                    SOCK_SEQPACKET |  Sequenced packet stream

                    A SOCK_STREAM type provides sequenced, reliable, two-way connection based
                    byte streams. An out-of-band data transmission mechanism may be supported.

                    A SOCK_DGRAM socket supports datagrams (connectionless, unreliable
                    messages of a fixed (typically small) maximum length).

                    A SOCK_SEQPACKET socket may provide a sequenced, reliable, two-way
                    connection-based data transmission path for datagrams of fixed maximum length;
                    a consumer may be required to read an entire packet with each read system
                    call. This facility is protocol specific, and presently unimplemented.

                    SOCK_RAW sockets provide access to internal network protocols and interfaces.

                    The types SOCK_RAW, which is available only to the super-user,
                    and SOCK_RDM, which is planned, but not yet implemented, are not
                    described here.

                    Additionally, the following flags are allowed in the type argument:

                    SOCK_NONBLOCK   Set non-blocking mode on the new socket

                    The protocol argument specifies a particular protocol to be used with the
                    socket. Normally only a single protocol exists to support a particular
                    socket type within a given protocol family. However, it is possible that
                    many protocols may exist, in which case a particular protocol must be
                    specified in this manner. The @a protocol number to use is particular to
                    the "communication domain" in which communication is to take place.

                    The @a protocol argument may be set to zero (0) to request the default
                    implementation of a socket type for the protocol, if any.

                    Sockets of type SOCK_STREAM are full-duplex byte streams, similar to
                    pipes. A stream socket must be in a connected state before any data may
                    be sent or received on it. A connection to another socket is created
                    with a @ref Connect() system call. Once connected, data may be transferred
                    using @ref Read() and @ref Write() calls or some variant of the @ref Send() and
                    @ref Recv() functions. (Some protocol families, such as the Internet family,
                    support the notion of an "implied connect", which permits data to be
                    sent piggybacked onto a connect operation by using the @ref SendTo() system
                    call.)  When a session has been completed a @ref Close() may be performed.
                    Out-of-band data may also be transmitted as described in @ref Send() and
                    received as described in @ref Recv().

                    The communications protocols used to implement a SOCK_STREAM ensure that
                    data is not lost or duplicated. If a piece of data for which the peer
                    protocol has buffer space cannot be successfully transmitted within a
                    reasonable length of time, then the connection is considered broken and
                    calls will indicate an error with -1 returns and with ETIMEDOUT as the
                    specific code in the global variable @a errno.  The protocols optionally
                    keep sockets "warm" by forcing transmissions roughly every minute in
                    the absence of other activity. An error is then indicated if no response
                    can be elicited on an otherwise idle connection for an extended period
                    (e.g. 5 minutes).

                    SOCK_SEQPACKET sockets employ the same system calls as SOCK_STREAM
                    sockets. The only difference is that @ref Read() calls will return only the
                    amount of data requested, and any remaining in the arriving packet will
                    be discarded.

                    SOCK_DGRAM and SOCK_RAW sockets allow sending of datagrams to
                    correspondents named in @ref Send() calls. Datagrams are generally received with
                    @ref RecvFrom(), which returns the next datagram with its return address.

                    The operation of sockets is controlled by socket level options with @ref SetSockOpt()
                    and @ref GetSockOpt().

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Socket(nn::socket::Family, nn::socket::Type, nn::socket::Protocol).

    @see            Socket(nn::socket::Family, nn::socket::Type, nn::socket::Protocol)
*/
int Socket(int domain, int type, int protocol) NN_NOEXCEPT;

/**
    @brief          Synchronous I/O multiplexing - This API will be deprecated in an upcoming SDK release.

    @param[in,out]  pSocketDescriptors  Array of pollfd structures
    @param[in]      numberOfDescriptors Number of pollfd structures
    @param[in]      timeoutMilliseconds Timeout in milliseconds

    @return         Number of descriptors that are ready for I/O, or -1 if an error occurred.
                    If the time limit expires, Poll() returns 0. If Poll() returns with an error,
                    file descriptor array will be unmodified.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:
    @post           [EINVAL]        The specified time limit is invalid. One of its components is negative or too large.
    @post           [EINVAL]        The @a pSocketDescriptors argument is null with non-zero @a outBufferLength argument.
    @post           [EFAULT]        The address range referred to by @a pSocketDescriptors and @a numberOfDescriptors * sizeof(pollfd)
                                    does not refer to a valid part of the process address space.
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The Poll() system call examines a set of file descriptors to see if some
                    of them are ready for I/O. The @a pSocketDescriptors argument is a pointer to an array of
                    pollfd structures defined as shown below).  The @a numberOfDescriptors argument
                    determines the size of the array.

                    @code
                    struct pollfd
                    {
                        int    fd;       // file descriptor
                        short  events;   // events to look for
                        short  revents;  // events returned
                    };
                    @endcode

                    The event bitmasks in events and revents have the following bits:

                    Event                      |       Description
                    ----------------|----------------------------------------------------------------
                    POLLIN          | Data other than high priority data may be read without blocking.
                    POLLRDNORM      | Normal data may be read without blocking.
                    POLLRDBAND      | Data with a non-zero priority may be read without blocking.
                    POLLPRI         | High priority data may be read without blocking.
                    POLLOUT         | Normal data may be written without blocking.
                    POLLWRNORM      | Normal data may be written without blocking.
                    POLLWRBAND      | Data with a non-zero priority may be written without blocking.
                    POLLERR         | An exceptional condition has occurred on the device or socket.  This flag is always checked, even if not present in the events bitmask.
                    POLLHUP         | The device or socket has been disconnected.  This flag is always checked, even if not present in the events bitmask.
                    POLLNVAL        | The file descriptor is not open.  This flag is always checked, even if not present in the events bitmask.

                    Note that POLLHUP and POLLOUT should never be present in the revents bitmask at the same time.

                    If @a timeout is neither zero nor INFTIM (-1), it specifies a maximum interval
                    to wait for any file descriptor to become ready, in milliseconds. If
                    @a timeout is INFTIM (-1), the poll blocks indefinitely.  If @a timeout is
                    zero, then Poll() will return without blocking.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Poll(nn::socket::PollFd*, nn::socket::NfdsT, int).

    @see            Poll(nn::socket::PollFd*, nn::socket::NfdsT, int)
*/
int Poll(pollfd* pSocketDescriptors, nfds_t numberOfDescriptors, int timeoutMilliseconds) NN_NOEXCEPT;

/**
    @brief          File control - This API will be deprecated in an upcoming SDK release.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      command         Control command

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]         The argument @a socket is not a valid descriptor.
    @post           [ENOTSOCK]      The argument @a socket does not refer to a socket.
    @post           [EOPNOTSUPP]    Command is not supported
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The Fcntl() routine provides control over descriptors. The argument
                    @a socket is a descriptor to be operated on by @a command as described below.
                    Depending on the value of command, Fcntl() can take additional arguments.

                    Supported commands:

                    F_GETFL         Get descriptor status flags. Additional arguments are ignored.

                    The descriptor flags for the F_GETFL command are returned from fcntl. Flags
                    that are supported to be modified are documented in the F_SETFL command below.

                    F_SETFL         Set descriptor status flags. Takes an additional argument - 'flags'

                    The flags for the F_SETFL flags are as follows:

                    O_NONBLOCK      Non-blocking I/O; if no data is available to a @ref Read
                                                           system call, or if a @ref Write operation would block, the read
                                    or write call returns -1 with the error EAGAIN.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Fcntl(int, nn::socket::FcntlCommand, ...).

    @see            Fcntl(int, nn::socket::FcntlCommand, ...)
*/
int Fcntl(int socket, int command, ...) NN_NOEXCEPT;


/**
    @brief          Synchronous I/O multiplexing - This API will be deprecated in an upcoming SDK release.

    @param[in]      numberOfDescriptors     Highest descriptor id plus 1
    @param[in,out]  pReadDescriptors        Read descriptor fdset
    @param[in,out]  pWriteDescriptors       Write descriptor fdset
    @param[in,out]  pExceptDescriptors      Exception descriptor fdset
    @param[in]      pTimeout                Timeout

    @return         Returns the number of ready descriptors that are
                    contained in the descriptor sets, or -1 if an error occurred. If the
                    time limit expires, select() returns 0.  If select() returns with an
                    error, the descriptor sets will be unmodified.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]         One of the descriptor sets specified an invalid descriptor.
    @post           [EINVAL]        The specified time limit is invalid.  One of its components is negative or too large.
    @post           [EINVAL]        The @a numberOfDescriptors argument was invalid.
    @post           [EFAULT]        One of the arguments @a pReadDescriptors, @a pWriteDescriptors,
                                    @a pExceptDescriptors, or @a pTimeout points to an invalid address.
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The Select() routine examines the I/O descriptor sets whose addresses
                    are passed in @a pReadDescriptors, @a pWriteDescriptors, and @a pExceptDescriptors
                    to see if some of their descriptors are ready for reading,
                    are ready for writing, or have an exceptional condition pending, respectively.
                    The only exceptional condition detectable is out-of-band data received on a socket.
                    The first @a numberOfDescriptors descriptors are checked in each set;
                    i.e., the descriptors from 0 through @a numberOfDescriptors-1 in the descriptor
                    sets are examined. On return, Select() replaces
                    the given descriptor sets with subsets consisting of those descriptors
                    that are ready for the requested operation. The Select() system call
                    returns the total number of ready descriptors in all the sets.

                    The descriptor sets are stored as bit fields in arrays of integers. The
                    following macros are provided for manipulating such descriptor sets:
                    FD_ZERO(_fdset) initializes a descriptor set fdset to the null set.
                    FD_SET(fd, _fdset) includes a particular descriptor fd in fdset.
                    FD_CLR(fd, _fdset) removes fd from fdset.  FD_ISSET(fd, _fdset) is non-
                    zero if fd is a member of fdset, zero otherwise.  The behavior of these
                    macros is undefined if a descriptor value is less than zero or greater
                    than or equal to FD_SETSIZE, which is normally at least equal to the
                    maximum number of descriptors supported by the system.

                    If @a timeout is not a null pointer, it specifies the maximum interval to
                    wait for the selection to complete. System activity can lengthen the
                    interval by an indeterminate amount.

                    If @a timeout is a null pointer, the select blocks indefinitely.

                    To effect a poll, the timeout argument should not be a null pointer, but
                    it should point to a zero-valued timeval structure.

                    Any of descriptor sets may be given as null pointers if
                    no descriptors are of interest.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated. Please use
                    @ref Select(int, nn::socket::FdSet*, nn::socket::FdSet*, nn::socket::FdSet*, nn::socket::TimeVal*).

    @see            Select(int, nn::socket::FdSet*, nn::socket::FdSet*, nn::socket::FdSet*, nn::socket::TimeVal*)
*/
int Select(int numberOfDescriptors, fd_set* pReadDescriptors, fd_set* pWriteDescriptors, fd_set* pExceptDescriptors, timeval* pTimeout) NN_NOEXCEPT;

/**
    @brief          Control I/O device - This API will be deprecated in an upcoming SDK release.

    @param[in]      fd              File descriptor for socket or device
    @param[in]      command         Device control command
    @param[in]      pData           Buffer containing command data
    @param[in]      dataLength      Size of the buffer

    @return         Returns result of the ioctl operation. If an error has occurred, a value of -1
                    is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EBADF]         The @a socket argument is not a valid descriptor.
    @post           [ENOTTY]        The specified request does not apply to the kind of
                                    object that the descriptor socket references.
    @post           [EINVAL]        The request or @a pData argument is not valid.
    @post           [EACCES]        Command is not allowed.
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The Ioctl() routine manipulates the underlying device parameters of
                    special files. The argument @a fd must be an open file descriptor obtained with
                    either @ref Socket(), @ref Accept().

                    The following ioctl commands are supported:

                    FIONREAD        Get the number of bytes that are immediately available for reading.
                                    For this command, @a pData should be a pointer to an int.

                    FIONWRITE       Get the number of bytes in the descriptor's send queue.  These
                                    bytes are data which has been written to the descriptor but which
                                    are being held by the kernel for further processing.  The nature
                                    of the required processing depends on the underlying device.  For
                                    TCP sockets, these bytes have not yet been acknowledged by the
                                    other side of the connection.
                                    For this command, @a pData should be a pointer to an int.

                    FIONSPACE       Get the free space in the descriptor's send queue.  This value is
                                    the size of the send queue minus the number of bytes being held
                                    in the queue.  Note: while this value represents the number of
                                    bytes that may be added to the queue, other resource limitations
                                    may cause a write not larger than the send queue's space to be
                                    blocked.  One such limitation would be a lack of network buffers
                                    for a write to a network connection.
                                    For this command, @a pData should be a pointer to an int.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated with no replacement.

    @see            Socket(nn::socket::Family, nn::socket::Type, nn::socket::Protocol),
                    Accept(int, nn::socket::SockAddr*, nn::socket::SockLenT*),
                    GetLastError()
*/
int Ioctl(int fd, uint32_t command, void* pData, size_t dataLength) NN_NOEXCEPT;

/**
    @brief          Return host @a errno (@a h_errno) for @ref GetHostByName() / @ref GetHostByAddr() - This API will be deprecated in an upcoming SDK release.

    @return         This function returns on of the following:
    @return         NETDB_INTERNAL
    @return         NETDB_SUCCESS
    @return         HOST_NOT_FOUND
    @return         TRY_AGAIN
    @return         NO_RECOVERY
    @return         NO_DATA
    @return         NO_ADDRESS

    @details        This function is used only to determine error conditions, it is also thread-localized.

    @note           This function will soon be deprecated. Please use @ref GetHError().

    @see            GetHError()

*/
int* GetHErrno() NN_NOEXCEPT;

/**
    @brief          Look up AF_INET host IP address by hostname - This API will be deprecated in an upcoming SDK release.

    @param[in]      pName           The name of the host to look up (e.g. www.nintendo.com)

    @return         Returns a struct hostent or NULL on error with error detail in @ref GetHErrno().

    @details        The return value of this function is thread-localized, this deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to keep the value between calls, a deep copy must be performed.

    @note           This function will soon be deprecated. Please use
                    @ref GetHostEntByName(const char*).

    @see            GetHostEntByName(const char*)

*/
hostent* GetHostByName(const char* pName) NN_NOEXCEPT;

/**
    @brief          Look up the address value for a host for AF_INET - This API will be deprecated in an upcoming SDK release.

    @param[in]      pAddress        This value shall be an in_addr structure that contains a binary address in network byte order.
    @param[in]      length          The size of the structure
    @param[in]      addressFamily   Currently only AF_INET is supported

    @return         Returns a struct hostent or NULL on error with error detail in @ref GetHErrno().

    @details        The return value of this function is thread-localized, which deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to keep the value between calls, a deep copy must be performed.

    @note           This function will soon be deprecated. Please use
                    @ref GetHostEntByAddr(const void*, nn::socket::SockLenT, nn::socket::Family).

    @see            GetHostEntByAddr(const void*, nn::socket::SockLenT, nn::socket::Family)
*/
hostent* GetHostByAddr(const void* pAddress, socklen_t length, int addressFamily) NN_NOEXCEPT;

/**
    @brief          Return a human-readable version of h_error / GetHError() - This API will be deprecated in an upcoming SDK release.

    @param[in]      errorNumber     The @a h_errno.

    @return         Returns a string representing the error number or NULL if out-of-memory.

    @details        The return value of this function is thread-localized, this deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to maintain the value between calls, a copy must be made.

    @note           This function will soon be deprecated. Please use
                    @ref HStrError(nn::socket::HErrno).

    @see            HStrError(nn::socket::HErrno)
*/
const char* HStrError(int errorNumber) NN_NOEXCEPT;

/**
    @brief          Get a list of IP addresses and port numbers - This API will be deprecated in an upcoming SDK release.

    @param[in]      pNodeName the name of the host or NULL.
    @param[in]      pServername the name of the service or NULL.
    @param[in]      pHints is an optional pointer to a struct addrinfo as defined by netdb.h.
    @param[out]     pResult an out parameter containing 0, 1, or many (via ai_next) addrinfo structures.

    @return         Returns one of the following values:
    @return         NETDB_INTERNAL see @ref GetLastErrno()
    @return         NETDB_SUCCESS no problem
    @return         HOST_NOT_FOUND Authoritative Answer Host not found
    @return         TRY_AGAIN Non-Authoritative Host not found, or SERVERFAIL
    @return         NO_RECOVERY Non recoverable errors, FORMERR, REFUSED, NOTIMP
    @return         NO_DATA Valid name, no data record of requested type
    @return         NO_ADDRESS no address, look for MX record

    @details        GetAddrInfo() routine can be used to obtain list of IP addresses and port numbers for
                    host @a pNodename and service @a pServername. It provides more flexibility than @ref GetHostByName().

                    Please note that @a pNodename and @a pServername cannot both be NULL at the same time.

                    The addrinfo result @a pResult returned by this function can be deallocated via @ref FreeAddrInfo().

                    The optional pHint can be used to provide hints concerning the type of socket
                    that the caller supports or wishes to use. The caller can supply the
                    following structure elements in hints:

                    ai_family       The protocol family that should be used.  When ai_family
                                    is set to PF_UNSPEC, it means the caller will accept any
                                    protocol family supported by the operating system.

                    ai_socktype     Denotes the type of socket that is wanted: SOCK_STREAM,
                                    SOCK_DGRAM, or SOCK_RAW.  When ai_socktype is zero the
                                    caller will accept any socket type.

                    ai_protocol     Indicates which transport protocol is desired, IPPROTO_UDP
                                    or IPPROTO_TCP.  If ai_protocol is zero the caller will
                                    accept any protocol.

                    ai_flags        The ai_flags field to which the hints parameter points
                                    shall be set to zero or be the bitwise-inclusive OR of one
                                    or more of the values AI_ADDRCONFIG, AI_CANONNAME,
                                    AI_NUMERICHOST, AI_NUMERICSERV and AI_PASSIVE.

                    AI_ADDRCONFIG   If the AI_ADDRCONFIG bit is set, IPv4
                                    addresses shall be returned only if an
                                    IPv4 address is configured on the local
                                    system, and IPv6 addresses shall be
                                    returned only if an IPv6 address is configured on the local system.

                    AI_CANONNAME    If the AI_CANONNAME bit is set, a successful
                                    call to @ref GetAddrInfo() will return a
                                    NUL-terminated string containing the
                                    canonical name of the specified hostname
                                    in the ai_canonname element of the first
                                    addrinfo structure returned.

                    AI_NUMERICHOST  If the AI_NUMERICHOST bit is set, it indicates
                                    that hostname should be treated as a
                                    numeric string defining an IPv4 or IPv6
                                    address and resolution should be
                                    attempted.

                    AI_NUMERICSERV  If the AI_NUMERICSERV bit is set, then a
                                    non-null servname string supplied shall be
                                    a numeric port string.  Otherwise, an
                                    EAI_NONAME error shall be returned. This
                                    bit shall prevent any type of name resolution
                                    service (for example, NIS+) from
                                    being invoked.

                    AI_PASSIVE      If the AI_PASSIVE bit is set it indicates
                                    that the returned socket address structure
                                    is intended for use in a call to bind(2).
                                    In this case, if the hostname argument is
                                    the null pointer, then the IP address portion
                                    of the socket address structure will
                                    be set to INADDR_ANY for an IPv4 address
                                    or IN6ADDR_ANY_INIT for an IPv6 address.

                                    If the AI_PASSIVE bit is not set, the
                                    returned socket address structure will be
                                    ready for use in a call to Connect() for
                                    a connection-oriented protocol or
                                    Connect(), SendTo(), or SendMsg() if a
                                    connectionless protocol was chosen.  The
                                    IP address portion of the socket address
                                    structure will be set to the loopback
                                    address if hostname is the null pointer
                                    and AI_PASSIVE is not set.

                    All other elements of the addrinfo structure passed via hints must be
                    zero or the null pointer.

                    If hints is the null pointer, getaddrinfo() behaves as if the caller provided
                    a struct addrinfo with ai_family set to PF_UNSPEC and all other
                    elements set to zero or NULL.

    @note           This function will soon be deprecated. Please use
                    @ref GetAddrInfo(const char*, const char*, const nn::socket::AddrInfo*, nn::socket::AddrInfo**).

    @see            GetAddrInfo(const char*, const char*, const nn::socket::AddrInfo*, nn::socket::AddrInfo**)
*/
int GetAddrInfo(const char* pNodeName, const char* pServername, const addrinfo* pHints, addrinfo** pResult) NN_NOEXCEPT;

/**
    @brief          Free the address info structure - This API will be deprecated in an upcoming SDK release.

    @param[in]      addrInfoStorage pointer to an  addrinfo structure that was returned by GetAddrInfo() / FreeAddrInfo()

    @details        The FreeAddrInfo() routine can be used to free structured returned by @ref GetAddrInfo() / @ref GetNameInfo().

    @note           This function will soon be deprecated. Please use
                    @ref FreeAddrInfo(nn::socket::AddrInfo*).

    @see            FreeAddrInfo(nn::socket::AddrInfo*)
*/
void FreeAddrInfo(addrinfo* addrInfoStorage) NN_NOEXCEPT;

/**
    @brief          Return a human-readable version of the return value from GetAddrInfo() / GetNameInfo() - This API will be deprecated in an upcoming SDK release.

    @param[in]      errorCode       Error code for translation to human readable format.

    @return         Returns a c-string human readable representation of the error code provided

    @details        The GAIStrError routine() is used to obtain human readable version of the return value from @ref GetAddrInfo()
                    and @ref GetNameInfo(). The return value of this function is thread-localized, this deviates from some POSIX implementations.
                    The return value is managed internally, if you wish to maintain the value between calls a copy must be made.

    @note           This function will soon be deprecated. Please use
                    @ref GAIStrError(nn::socket::AiErrno).

    @see            GAIStrError(nn::socket::AiErrno)
*/
const char* GAIStrError(int errorCode) NN_NOEXCEPT;

/**
    @brief          Convert a sockaddr structure to a pair of host name and service strings - This API will be deprecated in an upcoming SDK release.

    @param[in]      socketAddress       The sockaddr structure socketAddress should point to a sockaddr_in that is socketAddressLength bytes long.
                                        Note: sockaddr_in6 / sockaddr_dl are not currently supported.
    @param[in]      socketAddressLength The size of the socckaddr structure.
    @param[out]     host                The hostname buffer.
    @param[in]      hostLength          The length of the hostname buffer.
    @param[out]     service             The service name buffer.
    @param[in]      serviceLength       The length of the service name buffer.
    @param[in]      flags               Flags.

    @return         Returns one of the following values:
    @return         NETDB_INTERNAL      see @ref GetLastErrno().
    @return         NETDB_SUCCESS       No problem.
    @return         HOST_NOT_FOUND      Authoritative Answer Host not found.
    @return         TRY_AGAIN           Non-Authoritative Host not found, or SERVERFAIL.
    @return         NO_RECOVERY         Non recoverable errors, FORMERR, REFUSED, NOTIMP.
    @return         NO_DATA             Valid name, no data record of requested type.
    @return         NO_ADDRESS          No address, look for MX record.

    @details        The GetNameInfo() function is used to convert a sockaddr structure to a pair of host name and service strings.
                    It provides more flexibility than the @ref GetHostByAddr(). The addrinfo result @a pResult
                    returned by this function can be deallocated via @ref FreeAddrInfo().

                    @a flags argument is formed by OR'ing the following values from netdb.h:

                    NI_NOFQDN           A fully qualified domain name is not required for local
                                        hosts.  The local part of the fully qualified domain
                                        name is returned instead.

                    NI_NUMERICHOST      Return the address in numeric form, as if calling
                                        @ref InetNtop(), instead of a host name.

                    NI_NAMEREQD         A name is required.  If the host name cannot be found
                                        in DNS and this flag is set, a non-zero error code is
                                        returned.  If the host name is not found and the flag
                                        is not set, the address is returned in numeric form.

                    NI_NUMERICSERV      The service name is returned as a digit string repre-
                                        senting the port number.

                    NI_DGRAM            Specifies that the service being looked up is a datagram
                                        service.

    @note           This function will soon be deprecated. Please use
                    @ref GetNameInfo(const nn::socket::SockAddr*, nn::socket::SockLenT, char*, size_t, char*, size_t, nn::socket::NameInfoFlag).

    @see            GetNameInfo(const nn::socket::SockAddr*, nn::socket::SockLenT, char*, size_t, char*, size_t, nn::socket::NameInfoFlag)
*/
int GetNameInfo(const sockaddr* socketAddress, socklen_t socketAddressLength, char* host, socklen_t hostLength, char* service, socklen_t serviceLength, int flags) NN_NOEXCEPT;


/**
    @brief          Open network device for reading/writing - This API will be deprecated in an upcoming SDK release.

    @param[in]      path            Path to device
    @param[in]      flags           Ignored

    @return         Returns non-negative file descriptor, or -1 if an error occurred.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    @post           [EOPNOTSUPP]    Invalid path or device does not support open call.
    @post           [EINVAL]        The @a path argument is null.
    @post           [EPERM]         User has insufficient privileges to carry out the
                                    requested operation.
    @post           [EFAULT]        The @a path argument points outside the process's allocated address space.
    @post           [EAGAIN]        Resources to complete the request are temporarily unavailable.

    @details        The file name specified by @a path is opened for reading
                    and/or writing as specified by the argument @a flags, and the file descriptor
                    returned to the calling process. When operations on fd are completed,
                    file descriptor needs to be released by calling @ref Close().

                    In the current implementation only '/dev/bpf' device
                    can be accessed via Open(). 'flags' argument is not used.

                    This function requires calling @ref Initialize() before it can be used.

    @note           This function will soon be deprecated with no replacement.

    @see            Open(const char* path, nn::socket::OpenFlag)
*/
int Open(const char* path, int flags) NN_NOEXCEPT;

enum /* IpProtocol */
{
    IpProtocol_Ip      = 0,
    IpProtocol_Icmp    = 1,
    IpProtocol_Tcp     = 6,
    IpProtocol_Udp     = 17,
    IpProtocol_None    = 59,
    IpProtocol_UdpLite = 136,
    IpProtocol_Raw     = 255,
};

enum /* Type */
{
    Type_Sock_Stream    = 1,
    Type_Sock_Dgram     = 2,
    Type_Sock_Raw       = 3,
    Type_Sock_SeqPacket = 5
};

enum /* Family */
{
    Family_Unspec = 0,
    Family_Inet   = 2,
    Family_Route  = 17,
    Family_Link   = 18,
    Family_Inet6  = 28,

    Family_Max    = 42
};

enum /* PollEvent */
{
    PollEvent_PollNone     = 0x0000,
    PollEvent_PollIn       = 0x0001,
    PollEvent_PollPri      = 0x0002,
    PollEvent_PollOut      = 0x0004,
    PollEvent_PollErr      = 0x0008,
    PollEvent_PollHup      = 0x0010,
    PollEvent_PollNVal     = 0x0020,
    PollEvent_PollRdNorm   = 0x0040,
    PollEvent_PollRdBand   = 0x0080,
    PollEvent_PollWrNorm   = 0x0100,
    PollEvent_PollWrBand   = 0x0200,
    PollEvent_PollMsg      = 0x0400,
    PollEvent_PollRdHup    = 0x2000,
    PollEvent_PollInIgnEof = 0x3000,
    PollEvent_InfTim       = -1
};

enum /* AddrInfoFlag */
{
    AddrInfoFlag_None         = 0x00000000,
    AddrInfoFlag_Passive      = 0x00000001,
    AddrInfoFlag_CanonName    = 0x00000002,
    AddrInfoFlag_NumericHost  = 0x00000004,
    AddrInfoFlag_NumericServ  = 0x00000008,
    AddrInfoFlag_AddrConfig   = 0x00000400
};

enum /* NameInfoFlag */
{
    NameInfoFlag_None         = 0x00000000,
    NameInfoFlag_NoFqdn       = 0x00000001,
    NameInfoFlag_NumericHost  = 0x00000002,
    NameInfoFlag_NameReqd     = 0x00000004,
    NameInfoFlag_NumericServ  = 0x00000008,
    NameInfoFlag_Dgram        = 0x00000010
};

enum /* Level */
{
    Level_Socket  = 0xffff,
    Level_Ip      =  0,
    Level_Icmp    =  1,
    Level_Tcp     =  6,
    Level_Udp     = 17,
    Level_Config  = 0xfffe
};

enum /* OptionName */
{
    OptionName_Sock_NonBlock             = 0x20000000,
    OptionName_So_Debug                  = 0x0001,
    OptionName_So_AcceptConn             = 0x0002,
    OptionName_So_ReuseAddr              = 0x0004,
    OptionName_So_KeepAlive              = 0x0008,
    OptionName_So_DontRoute              = 0x0010,
    OptionName_So_Broadcast              = 0x0020,
    OptionName_So_UseLoopback            = 0x0040,
    OptionName_So_Linger                 = 0x0080,
    OptionName_So_OobInline              = 0x0100,
    OptionName_So_ReusePort              = 0x0200,
    OptionName_So_AcceptFilter           = 0x1000,
    OptionName_So_SndBuf                 = 0x1001,
    OptionName_So_RcvBuf                 = 0x1002,
    OptionName_So_SndLoWat               = 0x1003,
    OptionName_So_RcvLoWat               = 0x1004,
    OptionName_So_SndTimeo               = 0x1005,
    OptionName_So_RcvTimeo               = 0x1006,
    OptionName_So_Error                  = 0x1007,
    OptionName_So_Type                   = 0x1008,
    OptionName_So_Label                  = 0x1009,
    OptionName_So_PeerLabel              = 0x1010,
    OptionName_So_ListenQLimit           = 0x1011,
    OptionName_So_ListenQLen             = 0x1012,
    OptionName_So_ListenIncQLen          = 0x1013,
    OptionName_So_SetFib                 = 0x1014,
    OptionName_So_User_Cookie            = 0x1015,
    OptionName_So_Protocol               = 0x1016,
    OptionName_So_Vendor                 = 0x80000000,
    OptionName_So_Nn_Linger              = 0x80000001,
    OptionName_So_Nn_Shutdown_Exempt     = 0x00010000,
    OptionName_Ip_Options                =   1,
    OptionName_Ip_HdrIncl                =   2,
    OptionName_Ip_Tos                    =   3,
    OptionName_Ip_Ttl                    =   4,
    OptionName_Ip_RecvOpts               =   5,
    OptionName_Ip_Multicast_If           =   9,
    OptionName_Ip_Multicast_Ttl          =  10,
    OptionName_Ip_Multicast_Loop         =  11,
    OptionName_Ip_Add_Membership         =  12,
    OptionName_Ip_Drop_Membership        =  13,
    OptionName_Ip_Multicast_Vif          =  14,
    OptionName_Ip_Rsvp_On                =  15,
    OptionName_Ip_Rsvp_Off               =  16,
    OptionName_Ip_Rsvp_Vif_On            =  17,
    OptionName_Ip_Rsvp_Vif_Off           =  18,
    OptionName_Ip_PortRange              =  19,
    OptionName_Ip_Faith                  =  22,
    OptionName_Ip_OnesBcast              =  23,
    OptionName_Ip_BindAny                =  24,

    OptionName_Ip_RecvTtl                =  65,
    OptionName_Ip_MinTtl                 =  66,
    OptionName_Ip_DontFrag               =  67,
    OptionName_Ip_RecvTos                =  68,

    OptionName_Ip_Add_Source_Membership  =  70,
    OptionName_Ip_Drop_Source_Membership =  71,
    OptionName_Ip_Block_Source           =  72,
    OptionName_Ip_Unblock_Source         =  73,
    OptionName_Tcp_NoDelay               =    1,
    OptionName_Tcp_MaxSeg                =    2,
    OptionName_Tcp_NoPush                =    4,
    OptionName_Tcp_NoOpt                 =    8,
    OptionName_Tcp_Md5Sig                =   16,
    OptionName_Tcp_Info                  =   32,
    OptionName_Tcp_Congestion            =   64,
    OptionName_Tcp_KeepInit              =  128,
    OptionName_Tcp_KeepIdle              =  256,
    OptionName_Tcp_KeepIntvl             =  512,
    OptionName_Tcp_KeepCnt               = 1024,
    OptionName_Tcp_Vendor                = OptionName_So_Vendor,
    OptionName_Tcp_Ca_Name_Max           = 16
};

enum /* TcpInfoOption */
{
    TcpInfoOption_None       = 0x00,
    TcpInfoOption_Timestamps = 0x01,
    TcpInfoOption_Sack       = 0x02,
    TcpInfoOption_WScale     = 0x04,
    TcpInfoOption_Ecn        = 0x08,
    TcpInfoOption_Toe        = 0x10
};

enum /* Errno */
{
    Errno_Perm           = 1,
    Errno_NoEnt          = 2,
    Errno_Srch           = 3,
    Errno_Intr           = 4,
    Errno_Io             = 5,
    Errno_NxIo           = 6,
    Errno_2Big           = 7,
    Errno_NoExec         = 8,
    Errno_Badf           = 9,
    Errno_Child          = 10,
    Errno_Again          = 11,
    Errno_NoMem          = 12,
    Errno_Acces          = 13,
    Errno_Fault          = 14,
    Errno_NotBlk         = 15,
    Errno_Busy           = 16,
    Errno_Exist          = 17,
    Errno_XDev           = 18,
    Errno_NoDev          = 19,
    Errno_NotDir         = 20,
    Errno_IsDir          = 21,
    Errno_Inval          = 22,
    Errno_NFile          = 23,
    Errno_MFile          = 24,
    Errno_NotTy          = 25,
    Errno_TxtBsy         = 26,
    Errno_FBig           = 27,
    Errno_NoSpc          = 28,
    Errno_SPipe          = 29,
    Errno_Rofs           = 30,
    Errno_MLink          = 31,
    Errno_Pipe           = 32,
    Errno_Dom            = 33,
    Errno_Range          = 34,
    Errno_DeadLk         = 35,
    Errno_NameTooLong    = 36,
    Errno_NoLck          = 37,
    Errno_NoSys          = 38,
    Errno_NotEmpty       = 39,
    Errno_Loop           = 40,
    Errno_WouldBlock     = Errno_Again,
    Errno_NoMsg          = 42,
    Errno_Idrm           = 43,
    Errno_Chrng          = 44,
    Errno_L2NSync        = 45,
    Errno_L3Hlt          = 46,
    Errno_L3Rst          = 47,
    Errno_Lnrng          = 48,
    Errno_Unatch         = 49,
    Errno_NoCsi          = 50,
    Errno_L2Hlt          = 51,
    Errno_Bade           = 52,
    Errno_Badr           = 53,
    Errno_XFull          = 54,
    Errno_NoAno          = 55,
    Errno_BadRqc         = 56,
    Errno_BadSsl         = 57,
    Errno_DeadLock       = Errno_DeadLk,
    Errno_BFont          = 59,
    Errno_NoStr          = 60,
    Errno_NoData         = 61,
    Errno_Time           = 62,
    Errno_NoSr           = 63,
    Errno_NoNet          = 64,
    Errno_NoPkg          = 65,
    Errno_Remote         = 66,
    Errno_NoLink         = 67,
    Errno_Adv            = 68,
    Errno_Srmnt          = 69,
    Errno_Comm           = 70,
    Errno_Proto          = 71,
    Errno_MultiHop       = 72,
    Errno_DotDot         = 73,
    Errno_BadMsg         = 74,
    Errno_Overflow       = 75,
    Errno_NotUnuq        = 76,
    Errno_BadFd          = 77,
    Errno_RemChg         = 78,
    Errno_LibAcc         = 79,
    Errno_LibBad         = 80,
    Errno_LibScn         = 81,
    Errno_LibMax         = 82,
    Errno_LibExec        = 83,
    Errno_IlSeq          = 84,
    Errno_Restart        = 85,
    Errno_StrPipe        = 86,
    Errno_Users          = 87,
    Errno_NotSock        = 88,
    Errno_DestAddrReq    = 89,
    Errno_MsgSize        = 90,
    Errno_Prototype      = 91,
    Errno_NoProtoOpt     = 92,
    Errno_ProtoNoSupport = 93,
    Errno_SocktNoSupport = 94,
    Errno_OpNotSupp      = 95,
    Errno_NotSup         = Errno_OpNotSupp,
    Errno_PfNoSupport    = 96,
    Errno_AfNoSupport    = 97,
    Errno_AddrInUse      = 98,
    Errno_AddrNotAvail   = 99,
    Errno_NetDown        = 100,
    Errno_NetUnreach     = 101,
    Errno_NetReset       = 102,
    Errno_ConnAborted    = 103,
    Errno_ConnReset      = 104,
    Errno_NoBufs         = 105,
    Errno_IsConn         = 106,
    Errno_NotConn        = 107,
    Errno_ShutDown       = 108,
    Errno_TooManyRefs    = 109,
    Errno_TimedOut       = 110,
    Errno_ConnRefused    = 111,
    Errno_HostDown       = 112,
    Errno_HostUnreach    = 113,
    Errno_Already        = 114,
    Errno_InProgress     = 115,
    Errno_Stale          = 116,
    Errno_UClean         = 117,
    Errno_NotNam         = 118,
    Errno_NAvail         = 119,
    Errno_IsNam          = 120,
    Errno_RemoteIo       = 121,
    Errno_DQuot          = 122,
    Errno_NoMedium       = 123,
    Errno_MediumType     = 124,
    Errno_Canceled       = 125,
    Errno_NoKey          = 126,
    Errno_KeyExpired     = 127,
    Errno_KeyRevoked     = 128,
    Errno_KeyRejected    = 129,
    Errno_OwnerDead      = 130,
    Errno_NotRecoverable = 131,
    Errno_RfKill         = 132,
    Errno_HwPoison       = 133,
    Errno_ProcLim        = 156
};

enum /* HErrno */
{
    HErrno_Netdb_Internal  = -1,
    HErrno_Netdb_Success   = 0,
    HErrno_Host_Not_Found  = 1,
    HErrno_Try_Again       = 2,
    HErrno_No_Recovery     = 3,
    HErrno_No_Data         = 4,
    HErrno_No_Address      = HErrno_No_Data
};

enum /* AiErrno */
{
    AiErrno_AddrFamily  = 1,
    AiErrno_Again       = 2,
    AiErrno_BadFlags    = 3,
    AiErrno_Fail        = 4,
    AiErrno_Family      = 5,
    AiErrno_Memory      = 6,
    AiErrno_NoData      = 7,
    AiErrno_NoName      = 8,
    AiErrno_Service     = 9,
    AiErrno_SockType    = 10,
    AiErrno_System      = 11,
    AiErrno_BadHints    = 12,
    AiErrno_Protocol    = 13,
    AiErrno_Overflow    = 14,
    AiErrno_Max         = 15
};

}} /* nn::socket */
