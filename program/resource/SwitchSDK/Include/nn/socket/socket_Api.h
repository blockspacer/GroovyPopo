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
    @brief  Public header defining socket API.
            Based on FreeBSD man pages - https://www.freebsd.org/cgi/man.cgi
 */

#pragma once

#include <stdint.h>
#include <nn/socket/socket_Types.h>
#include <nn/socket/socket_Constants.h>
#include <nn/socket/socket_Config.h>
#include <nn/socket/socket_Result.h>
#include <nn/socket/socket_Errno.h>
#include <nn/socket/socket_ApiDeprecated.h>
#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>

/**
    @namespace nn::socket
*/
namespace nn     {
namespace socket {

/**
    @brief          Read the global variable @a errno.

    @return         Returns @a errno value.

    @details        When a network function call detects an error, it returns an integer value indicating
                    failure (-1) and sets the global variable @a errno accordingly. In Windows builds,
                    @a errno cannot be accessed directly. Instead, this function needs to be called
                    to retrieve that value. It can also be used in NX builds but it is not necessary.

    @see            SetLastError()
*/
Errno GetLastError() NN_NOEXCEPT;

/**
    @brief          Set the global variable @a errno.

    @param[in]      error   The error to which to set the global variable @a errno to.

    @details        The SetLastError() routine can be used to set the global variable @a errno.
                    Certain components of the network stack may choose to set or override
                    the global variable @a errno. Since @a errno cannot be accessed directly in
                    Windows builds, this function is necessary in those cases.
                    It can also be used in NX builds but it is not necessary.

    @see            GetLastError()
*/
void SetLastError(Errno error) NN_NOEXCEPT;

/**
    @brief          Convert address to network format.

    @param[in]      family              Type of the network address, only @ref Family::Af_Inet is supported
    @param[in]      pAddressString      Address in presentation format
    @param[out]     pOutAddressBuffer   Address in network format

    @return         Returns 1 if the address was valid for the specified
                    address family, or 0 if the address was not parseable in the specified
                    address family, or -1 if some system error occurred (in which case the global variable @a errno
                    will have been set). This function is presently valid only for @ref Family::Af_Inet.

    @details        The InetPton() function converts a presentation format address (that is,
                    printable form as held in a character string) to network format (usually
                    an @ref InAddr structure or some other internal binary representation, in network
                    byte order). This function can be used without calling @ref Initialize() first.

    @see            InetAton(), InetNtoa(), InetNtop(), GetLastError()
*/
int InetPton(Family family, const char* pAddressString, void* pOutAddressBuffer) NN_NOEXCEPT;

/**
    @brief          Convert string to network address.

    @param[in]      addressStringBuffer Address string in ASCII format
    @param[out]     pOutNetworkAddress  Address in network format

    @return         Returns 1 if the string was successfully interpreted, or 0 if the string
                    is invalid.

    @details        The InetAton() routine interprets the specified character string as an
                    Internet address, placing the address into the structure provided.
                    This function can be used without calling @ref Initialize() first.

    @see            InetPton(), InetNtoa(), InetNtop(), GetLastError()
*/
int InetAton(const char* addressStringBuffer, InAddr* pOutNetworkAddress) NN_NOEXCEPT;

/**
    @brief          Convert Internet address into ASCII string.

    @param[in]      networkAddress      Address in network format

    @return         Returns network address in ASCII representation.

    @details        The InetNtoa() routine takes an Internet address and returns an ASCII
                    string representing the address in '.' notation.
                    The return value is managed internally, so if you wish to maintain the value between calls, a copy must be made.
                    This function is not thread safe so there is a chance for the returned string to
                    not be what is expected if called from another thread at the same time.
                    This function can be used without calling @ref Initialize() first.

    @see            InetPton(), InetAton(), InetNtop(), GetLastError()
*/
char* InetNtoa(InAddr networkAddress) NN_NOEXCEPT;

/**
    @brief          Convert network address to presentation format.

    @param[in]      family                      Type of network address, only @ref Family::Af_Inet is supported
    @param[in]      pNetworkAddress             Address in network format
    @param[out]     addressStringBuffer         Address in presentation format
    @param[in]      addressStringBufferLength   Size of the buffer for address in presentation format

    @return         Returns NULL if a system error occurs (in which case, the global variable @a errno will have
                    been set), or it returns a pointer to the destination string (@a addressStringBuffer).

    @details        The InetNtop() routine converts an address stored at @a pNetworkAddress
                    from network format (usually an @ref InAddr structure or some other binary form, in network byte
                    order) to presentation format (suitable for external display purposes).
                    This function can be used without calling @ref Initialize() first.

    @see            InetPton(), InetAton(), InetNtoa(), GetLastError()
*/
const char* InetNtop(Family family, const void* pNetworkAddress, char* addressStringBuffer, SockLenT addressStringBufferLength) NN_NOEXCEPT;

/**
    @brief          Convert 16-bit integer from host to network byte order.

    @param[in]      hostValue           16-bit unsigned integer in host byte order

    @return         Returns 16-bit unsigned integer in network byte order

    @details        The InetHtons() routine converts 16-bit unsigned integer @a hostValue
                    from host byte order to network byte order.
                    This function can be used without calling @ref Initialize() first.

    @see            InetHtonl(), InetNtohs(), InetNtohl(), GetLastError()
 */
uint16_t InetHtons(uint16_t hostValue) NN_NOEXCEPT;

/**
    @brief          Convert 32-bit integer from host to network byte order.

    @param[in]      hostValue           32-bit unsigned integer in host byte order

    @return         Returns 32-bit unsigned integer in network byte order.

    @details        The InetHtonl() routine  converts 32-bit unsigned integer @a hostValue
                    from host byte order to network byte order.
                    This function can be used without calling @ref Initialize() first.

    @see            InetHtons(), InetNtohs(), InetNtohl(), GetLastError()
 */
uint32_t InetHtonl(uint32_t hostValue) NN_NOEXCEPT;

/**
    @brief          Convert 16-bit ineger from network to host byte order.

    @param[in]      networkValue        16-bit unsigned integer in network byte order

    @return         Returns 16-bit unsigned integer in host byte order

    @details        The InetNtohs() routine converts 16-bit unsigned integer @a networkValue
                    from network byte order to host byte order.
                    This function can be used without calling @ref Initialize() first.

    @see            InetHtons(), InetHtonl(), InetNtohl(), GetLastError()
 */
uint16_t InetNtohs(uint16_t networkValue) NN_NOEXCEPT;

/**
    @brief          Convert 32-bit integer from network to host byte order.

    @param[in]      networkValue        32-bit unsigned integer in network byte order

    @return         Returns 32-bit unsigned integer in host byte order.

    @details        The InetNtohl() routine converts 32-bit unsigned integer @a networkValue
                    from network byte order to host byte order.
                    This function can be used without calling @ref Initialize() first.

    @see            InetHtons(), InetHtonl(), InetNtohs(), GetLastError()
 */
uint32_t InetNtohl(uint32_t networkValue) NN_NOEXCEPT;

/**
    @brief          Receives a message from a connection-oriented or connectionless-oriented socket.
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

    Error                     | Description
    --------------------------|------------
    @ref Errno::EBadf         | The argument socket is an invalid descriptor
    @ref Errno::EConnReset    | The remote socket end is forcibly closed
    @ref Errno::ENetDown      | The network interface has been removed and the socket has been shut down.
    @ref Errno::ENotConn      | The socket is associated with a connection-oriented protocol and has not been connected (see @ref Connect() and @ref Accept()).
    @ref Errno::ENotSock      | The argument socket does not refer to a socket.
    @ref Errno::EAgain        | The socket is marked non-blocking, and the receive operation would block, or a receive timeout had been set, and the timeout expired before data was received.
    @ref Errno::EAgain        | Resources to complete the request are temporarily unavailable.
    @ref Errno::EInval        | A non-zero @a outBufferLength was provided with a null @a outBuffer.
    @ref Errno::EFault        | The address range referred to by @a outBuffer and @a outBufferLength does not refer to a valid part of the process address space.

    @details        The RecvFrom() routine is used to receive messages from a socket,
                    and may be used to receive data on a socket whether or not
                    it is connection-oriented.

                    If @a pOutAddress is not a null pointer and the socket is not connection-oriented,
                    the source address of the message is filled in. The @a pOutAddressLength argument is
                    a value-result argument, initialized to the size of the buffer associated
                    with @a pOutAddress, and modified on return to indicate the actual size of the
                    address stored there. If the actual length of the address is greater than the length
                    of the supplied @ref SockAddr structure, the stored address will be truncated.

                    If no messages are available at the socket, the receive call waits for a
                    message to arrive, unless the socket is non-blocking (see @ref Fcntl()) in
                    which case the value -1 is returned and the global variable @a errno is set
                    to @ref Errno::EAgain. The receive calls normally return any data available, up to
                    the requested amount, rather than waiting for receipt of the full amount
                    requested; this behavior is affected by the socket-level options
                    @ref Option::So_RcvLoWat and @ref Option::So_RcvTimeo described in @ref GetSockOpt().

                    The @ref Select() function call may be used to determine when more data
                    arrives.

                    The @a flags argument is formed by or'ing one or more
                    of the values:

                    Flag                       | Description
                    ---------------------------|-----------------------------------
                    @ref MsgFlag::Msg_Oob      | Process out-of-band data
                    @ref MsgFlag::Msg_Peek     | Peek at incoming message
                    @ref MsgFlag::Msg_WaitAll  | Wait for full request or error
                    @ref MsgFlag::Msg_DontWait | Do not block

                    The @ref MsgFlag::Msg_Oob flag requests receipt of out-of-band data that would not be
                    received in the normal data stream. Some protocols place expedited data
                    at the head of the normal data queue, and thus this flag cannot be used
                    with such protocols. The @ref MsgFlag::Msg_Peek flag causes the receive operation to
                    return data from the beginning of the receive queue without removing that
                    data from the queue. Thus, a subsequent receive call will return the
                    same data. The @ref MsgFlag::Msg_WaitAll flag requests that the operation block until
                    the full request is satisfied. However, the call may still return less
                    data than requested if an error or disconnect occurs,
                    or the next data to be received is of a different type than that
                    returned. The @ref MsgFlag::Msg_DontWait flag requests the call to return when it
                    would block otherwise. If no data is available, the global variable @a errno
                    is set to @ref Errno::EAgain.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Fcntl(), GetSockOpt(), Select(), Socket(), Connect(), Accept(), GetLastError()

 */
ssize_t RecvFrom(int socket, void* outBuffer, size_t outBufferLength, MsgFlag flags, SockAddr* pOutAddress, SockLenT* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Receives a message from a connection-mode or connectionless-mode socket.
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

    @see            Fcntl(), GetSockOpt(), Select(), Socket(), Connect(), Accept(), RecvFrom(), GetLastError()
*/
ssize_t Recv(int socket, void* outBuffer, size_t outBufferLength, MsgFlag flags) NN_NOEXCEPT;

/**
    @brief          Send a message from a socket.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      buffer              Buffer containing the message to send
    @param[in]      bufferLength        Size of the message
    @param[in]      flags               Flags modifying behavior of the function
    @param[in]      pAddress            Buffer containing address of the recipient
    @param[in]      addressLength       Size of the recipient address

    @return         Returns the number of characters sent, or -1 if an error occurred.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                     | Description
    --------------------------|------------
    @ref Errno::EBadf         | An invalid descriptor was specified.
    @ref Errno::EAcces        | The destination address is a broadcast address, and @ref Option::So_Broadcast has not been set on the socket.
    @ref Errno::ENotSock      | The argument @a socket is not a socket.
    @ref Errno::EMsgSize      | The socket requires that message be sent atomically, and the size of the message to be sent made this impossible.
    @ref Errno::EAgain        | The socket is marked non-blocking and the requested operation would block.
    @ref Errno::EAgain        | Resources to complete the request are temporarily unavailable.
    @ref Errno::ENoBufs       | The system was unable to allocate an internal buffer. The operation may succeed when buffers become available.
    @ref Errno::EHostUnreach  | The remote host was unreachable.
    @ref Errno::EIsConn       | A destination address was specified and the socket is already connected.
    @ref Errno::EConnRefused  | The socket received an ICMP destination unreachable message from the last message sent. This typically means that the receiver is not listening on the remote port.
    @ref Errno::EHostDown     | The remote host was down.
    @ref Errno::ENetDown      | The local network interface is down.
    @ref Errno::ENetUnreach   | The network is not reachable from this host.
    @ref Errno::EPipe         | The socket is unable to send any more data (SBS_CANTSENDMORE has been set on the socket). This typically means that the socket is not connected.
    @ref Errno::EInval        | A non-zero @a bufferLength was provided with a null @a buffer.
    @ref Errno::EFault        | The address range referred to by @a buffer and @a bufferLength does not refer to a valid part of the process address space.

    @details        The SendTo() routine is used to transmit a message to another socket.
                    The address of the target is given by @a pAddress with @a addressLength specifying its size.
                    The length of the message is given by @a bufferLength. If the message is too long to
                    pass atomically through the underlying protocol, the error @ref Errno::EMsgSize is
                    returned, and the message is not transmitted.

                    If no message space is available at the socket to hold the message to be
                    transmitted, then SendTo() normally blocks, unless the socket has been
                    placed in non-blocking I/O mode. The @ref Select() call may be used
                    to determine when it is possible to send more data.

                    The flags argument may include one or more of the following:

                    Flag                          |   Description
                    ------------------------------|--------------------------------------
                    @ref MsgFlag::Msg_Oob         |  Process out-of-band data
                    @ref MsgFlag::Msg_DontRoute   |  Bypass routing, use direct interface
                    @ref MsgFlag::Msg_Eor         |  Data completes record
                    @ref MsgFlag::Msg_Eof         |  Data completes transaction

                    The flag @ref MsgFlag::Msg_Oob is used to send "out-of-band" data on sockets that
                    support this notion (e.g. @ref Type::Sock_Stream); the underlying protocol must also
                    support "out-of-band" data. @ref MsgFlag::Msg_Eor is used to indicate a record mark
                    for protocols which support the concept. @ref MsgFlag::Msg_Eof requests that the
                    sender side of a socket be shut down, and that an appropriate indication
                    be sent at the end of the specified data; this flag is only implemented
                    for @ref Type::Sock_Stream sockets in the @ref Family::Af_Inet protocol family. @ref MsgFlag::Msg_DontRoute is
                    usually used only by diagnostic or routing programs.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Fcntl(), GetSockOpt(), Recv(), Select(), Socket(), Write(), GetLastError()
*/
ssize_t SendTo(int socket, const void* buffer, size_t bufferLength, MsgFlag flags, const SockAddr* pAddress, SockLenT addressLength) NN_NOEXCEPT;

/**
    @brief          Send a message from a socket.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      buffer              Buffer containing the message to send
    @param[in]      bufferLength        Size of the message
    @param[in]      flags               Flags modifying behavior of the function

    @return         Returns the number of characters sent, or -1 if an error occurred.

    @details        The Send() routine is similar in behavior to @ref SendTo(); however,
                    it is normally used only on a connected socket (see @ref Connect()).

                    This function requires calling @ref Initialize() before it can be used.

    @see            Fcntl(), GetSockOpt(), Recv(), Select(), Socket(), Write(), GetLastError()

*/
ssize_t Send(int socket, const void* buffer, size_t bufferLength, MsgFlag flags) NN_NOEXCEPT;

/**
    @brief          Accept connection on a socket.

    @param[in]      socket              Socket desciptor obtained with @ref Socket() or @ref Accept()
    @param[out]     pOutAddress         Address of the remote host
    @param[in,out]  pOutAddressLength   Size of the buffer containing remote host address

    @return         Returns -1 on error. If it succeeds, it returns a non-negative integer that is a
                    descriptor for the accepted socket.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                     | Description
    --------------------------|------------
    @ref Errno::EBadf         | The descriptor is invalid.
    @ref Errno::EIntr         | The accept operation was interrupted.
    @ref Errno::EMFile        | The per-process descriptor table is full.
    @ref Errno::ENotSock      | The descriptor references a file, not a socket.
    @ref Errno::EInval        | @ref Listen() has not been called on the socket descriptor.
    @ref Errno::EInval        | A non-zero @a pOutAddressLength was passed with a null @a pOutAddress.
    @ref Errno::EWouldBlock or @ref Errno::EAgain | The socket is marked non-blocking and no connections are present to be accepted.
    @ref Errno::EConnAborted  | A connection arrived, but it was closed while waiting on the listen queue.
    @ref Errno::EFault        | The address range referred to by @a pOutAddress and @a pOutAddressLength does not refer to a valid part of the process address space.
    @ref Errno::EAgain        | Resources to complete the request are temporarily unavailable.

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
                    socket types, currently with @ref Type::Sock_Stream.

                    It is possible to @ref Select() a socket for the purposes of doing an
                    Accept() by selecting it for read.

                    When using Accept(), portable programs should not rely on the O_NONBLOCK
                    property and the signal destination being inherited, but
                    should set them explicitly using @ref Fcntl().

                    This function requires calling @ref Initialize() before it can be used.

    @see            Bind(), Connect(), GetPeerName(), GetSockName(), Listen(), Select(), Socket(), GetLastError()

*/
int Accept(int socket, SockAddr* pOutAddress, SockLenT* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Assign a local protocol address to a socket.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      pAddress            Buffer containing address
    @param[in]      addressLength       Length of the address

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                     | Description
    --------------------------|------------
    @ref Errno::EAgain        | Resources to complete the request are temporarily unavailable.
    @ref Errno::EBadf         | The socket argument is not a valid descriptor.
    @ref Errno::EInval        | The socket is already bound to an address, and the protocol does not support binding to a new address; or the socket has been shut down.
    @ref Errno::EInval        | The @a addressLength argument is not a valid length for the address family.
    @ref Errno::EInval        | The @a pAddress argument is null.
    @ref Errno::ENotSock      | The @a socket argument is not a socket.
    @ref Errno::EAddrNotAvail | The specified address is not available from the local machine.
    @ref Errno::EAddrInUse    | The specified address is already in use.
    @ref Errno::EAfNoSupport  | Addresses in the specified address family cannot be used with this socket.
    @ref Errno::EAcces        | The requested address is protected, and the current user has inadequate permission to access it.
    @ref Errno::EFault        | The address range referred to by @a pAddress and @a addressLength does not refer to a valid part of the process address space.

    @details        The Bind() routine assigns the local protocol address to a socket.
                    When a socket is created with @ref Socket() it exists in an address family
                    space but has no protocol address assigned. The Bind() function call
                    requests that address stored at @a pAddress be assigned to the socket.

                    For maximum portability, you should always zero the socket address
                    structure before populating it and passing it to Bind().

                    This function requires calling @ref Initialize() before it can be used.

    @see            Connect(), GetSockName(), Listen(), Socket(), GetLastError()

*/
int Bind(int socket, const SockAddr* pAddress, SockLenT addressLength) NN_NOEXCEPT;

/**
    @brief          Initiate a connection on a socket.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      pAddress            Buffer containing address of the remote host
    @param[in]      addressLength       Size of the buffer containing address of the remote host

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                     | Description
    --------------------------|------------
    @ref Errno::EBadf         | The @a socket argument is not a valid descriptor.
    @ref Errno::EInval        | The @a addressLength argument is not a valid length for the address family.
    @ref Errno::EFault        | The address range referred to by @a pAddress and @a addressLength does not refer to a valid part of the process address space.
    @ref Errno::ENotSock      | The @a socket argument is a descriptor for a file, not a socket.
    @ref Errno::EAddrNotAvail | The specified address is not available on this machine.
    @ref Errno::EAfNoSupport  | Addresses in the specified address family cannot be used with this socket.
    @ref Errno::EIsConn       | The socket is already connected.
    @ref Errno::ETimedOut     | Connection establishment timed out without establishing a connection.
    @ref Errno::EConnRefused  | The attempt to connect was forcefully rejected.
    @ref Errno::EConnReset    | The connection was reset by the remote host.
    @ref Errno::ENetUnreach   | The network is not reachable from this host.
    @ref Errno::EHostUnreach  | The remote host is not reachable from this host.
    @ref Errno::EAddrInUse    | The address is already in use.
    @ref Errno::EInProgress   | The socket is non-blocking and the connection cannot be completed immediately. It is possible to @ref Select() for completion by selecting the socket for writing.
    @ref Errno::EAlready      | A previous connection attempt has not yet been completed.
    @ref Errno::EAcces        | An attempt is made to connect to a broadcast address (obtained through the @ref InAddr_Broadcast constant or the @ref InAddr_None return value) through a socket that does not provide broadcast functionality.
    @ref Errno::EAgain        | An auto-assigned port number was requested but no auto-assigned ports are available.
    @ref Errno::EAgain        | Resources to complete the request are temporarily unavailable.


    @details        The Connect() routine initiates connection on a socket.
                    If socket is of type @ref Type::Sock_Dgram, this call specifies
                    the peer with which the socket is to be associated; this address is
                    that to which datagrams are to be sent, and the only address from which
                    datagrams are to be received.

                    If the socket is of type @ref Type::Sock_Stream, this call attempts to make
                    a connection to another socket. The other socket
                    is specified by name, which is an address in the communications space of
                    the socket. Each communications space interprets the name argument in
                    its own way.

                    Generally, stream sockets may successfully connect only
                    once; datagram sockets may use connect multiple times to change their
                    association. Datagram sockets may dissolve the association by connecting
                    to an invalid address, such as a null address.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Accept(), GetPeerName(), GetSockName(), Select(), Socket(), GetLastError()
*/
int Connect(int socket, const SockAddr* pAddress, SockLenT addressLength) NN_NOEXCEPT;

/**
    @brief          Get name of connected peer.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     pOutAddress         Buffer for storing name of the peer
    @param[in,out]  pOutAddressLength   Size of the buffer

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                  | Description
    -----------------------|------------
    @ref Errno::EBadf      | The argument @a socket is not a valid descriptor.
    @ref Errno::EConnReset | The connection has been reset by the peer.
    @ref Errno::EInval     | The value of the @a pOutAddressLength argument is not valid.
    @ref Errno::EInval     | The @a pOutAddress argument or @a pOutAddressLength argument are null.
    @ref Errno::EFault     | The address range referred to by @a outAddress and @a pOutAddressLength does not refer to a valid part of the process address space.
    @ref Errno::ENotSock   | The argument @a socket is not a socket.
    @ref Errno::ENotConn   | The socket is not connected.
    @ref Errno::ENoBufs    | Insufficient resources were available in the system to perform the operation.
    @ref Errno::EAgain     | Resources to complete the request are temporarily unavailable.

    @details        The GetPeerName() routine returns the name of the peer connected to
                    socket. The @a pOutAddressLength argument should be initialized to indicate the
                    amount of space pointed to by @a pOutAddress. On return it contains the actual
                    size of the name returned (in bytes). The name is truncated if the
                    buffer provided is too small.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Accept(), Bind(), GetSockName(), Socket(), GetLastError()
*/
int GetPeerName(int socket, SockAddr* pOutAddress, SockLenT* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Get socket name.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     pOutAddress         Buffer for name
    @param[in,out]  pOutAddressLength   Size of the buffer for name

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                  | Description
    -----------------------|------------
    @ref Errno::EBadf      | The argument @a socket is not a valid descriptor.
    @ref Errno::EConnReset | The connection has been reset by the peer.
    @ref Errno::EInval     | The value of the @a pOutAddressLength argument is not valid.
    @ref Errno::EInval     | The @a pOutAddress argument or @a pOutAddressLength argument are null.
    @ref Errno::EFault     | The address range referred to by @a outAddress and @a pOutAddressLength does not refer to a valid part of the process address space.
    @ref Errno::ENotSock   | The argument @a socket is not a socket.
    @ref Errno::ENoBufs    | Insufficient resources were available in the system to perform the operation.
    @ref Errno::EAgain     | Resources to complete the request are temporarily unavailable.

    @details        The GetSockName() routine returns the current name for the specified
                    socket. The @a pOutAddressLength argument should be initialized to indicate the
                    amount of space pointed to by @a pOutAddress. On return it contains the actual
                    size of the name returned (in bytes).

                    This function requires calling @ref Initialize() before it can be used.

    @see            Bind(), GetPeerName(), Socket(), GetLastError()
*/
int GetSockName(int socket, SockAddr* pOutAddress, SockLenT* pOutAddressLength) NN_NOEXCEPT;

/**
    @brief          Set options on a socket.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      level               Level of the option
    @param[in]      optionName          Type of the option
    @param[in]      pOptionValue        Buffer containing option value
    @param[in]      optionLength        Size of the pOptionValue buffer

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                   | Description
    ------------------------|------------
    @ref Errno::EBadf       | The argument @a socket is not a valid descriptor.
    @ref Errno::ENotSock    | The argument @a socket is not a socket.
    @ref Errno::ENoProtoOpt | The @a optionName is unknown at the level indicated.
    @ref Errno::EInval      | The @a pOptionValue argument is null or the value of the @a optionLength argument is not valid.
    @ref Errno::EFault      | The address range referred to by @a pOptionValue and @a optionLength does not refer to a valid part of the process address space.
    @ref Errno::EAgain      | Resources to complete the request are temporarily unavailable.

    @details        The SetSockOpt() routine manipulates the options associated with a socket.
                    Options may exist at multiple protocol levels; they are always present at
                    the uppermost "socket" level.

                    When manipulating socket options the level at which the option resides
                    and the name of the option must be specified. To manipulate options at
                    the socket level, level is specified as @ref Level::Sol_Socket. To manipulate
                    options at any other level the protocol number of the appropriate protocol
                    controlling the option is supplied. For example, to indicate that an
                    option is to be interpreted by the TCP protocol, level should be set to
                    the protocol number of TCP.

                    The following are supported levels:

                    @ref Level::Sol_Ip, @ref Level::Sol_Icmp, @ref Level::Sol_Tcp, @ref Level::Sol_Udp, @ref Level::Sol_Socket.

                    The @a pOptionValue and @a optionLength arguments are used to access option values.
                    The @a optionName argument and any specified options are passed uninterpreted
                    to the appropriate protocol module for interpretation.

                    Most socket-level options utilize an int argument for option value. For
                    SetSockOpt(), the argument should be non-zero to enable a boolean option,
                    or zero if the option is to be disabled. @ref Option::So_Linger uses a @ref Linger
                    argument which specifies the desired state of the option and the linger
                    interval (see below). @ref Option::So_SndTimeo and @ref Option::So_RcvTimeo use a @ref TimeVal argument.

                    The following options are recognized at the socket level.

                    Option                        |    Short description
                    ------------------------------|---------------------
                    @ref Option::So_ReuseAddr     |    Enables local address reuse
                    @ref Option::So_ReusePort     |    Enables duplicate address and port bindings
                    @ref Option::So_KeepAlive     |    Enables keep connections alive
                    @ref Option::So_DontRoute     |    Enables routing bypass for outgoing messages
                    @ref Option::So_Linger        |    Linger on close if data present
                    @ref Option::So_Nn_Linger     |    Upon completion of @ref Close(), this version of linger always drops the connection entirely, freeing associated resources
                    @ref Option::So_Broadcast     |    Enables permission to transmit broadcast messages
                    @ref Option::So_OobInline     |    Enables reception of out-of-band data in band
                    @ref Option::So_SndBuf        |    Set buffer size for output
                    @ref Option::So_RcvBuf        |    Set buffer size for input
                    @ref Option::So_SndLoWat      |    Set minimum count for output (not supported on Windows)
                    @ref Option::So_RcvLoWat      |    Set minimum count for input (not supported on Windows)
                    @ref Option::So_SndTimeo      |    Set timeout value for output
                    @ref Option::So_RcvTimeo      |    Set timeout value for input
                    @ref Option::So_AcceptConn    |    Get listening status of the socket (get only, not supported on Windows)
                    @ref Option::So_Type          |    Get the type of the socket (get only, not supported on Windows)
                    @ref Option::So_Protocol      |    Get the protocol number for the socket (get only)
                    @ref Option::So_Error         |    Get and clear error on the socket (get only)
                    @ref Option::So_ListenQLimit  |    Get backlog limit of the socket (get only)
                    @ref Option::So_ListenQLen    |    Get complete queue length of the socket (get only)
                    @ref Option::So_ListenIncQLen |    Get incomplete queue length of the socket (get only)

                    @ref Option::So_ReuseAddr indicates that the rules used in validating addresses supplied
                    in a @ref Bind() system call should allow reuse of local addresses.

                    @ref Option::So_ReusePort allows completely duplicate bindings by multiple processes
                    if they all set @ref Option::So_ReusePort before binding the port. This option permits
                    multiple instances of a program to each receive UDP/IP multicast or
                    broadcast datagrams destined for the bound port.

                    @ref Option::So_KeepAlive enables the periodic transmission of messages on a connected
                    socket. Should the connected party fail to respond to these messages,
                    the connection is considered broken and processes using the socket are
                    notified via a SIGPIPE signal when attempting to send data.

                    @ref Option::So_DontRoute indicates that outgoing messages should bypass the standard
                    routing facilities. Instead, messages are directed to the appropriate
                    network interface according to the network portion of the destination
                    address.

                    @ref Option::So_Linger controls the action taken when unsent messages are queued on
                    socket and a @ref Close() is performed. If the socket promises reliable
                    delivery of data and @ref Option::So_Linger is set, the system will block the process
                    on the @ref Close() attempt until it is able to transmit the data or until it
                    decides it is unable to deliver the information (a timeout period, termed
                    the linger interval, is specified in seconds in the SetSockOpt() system
                    call when @ref Option::So_Linger is requested). If @ref Option::So_Linger is disabled and a
                    @ref Close() is issued, the system will process the close in a manner that
                    allows the process to continue as quickly as possible.

                    The option @ref Option::So_Broadcast requests permission to send broadcast datagrams
                    on the socket.

                    With protocols that support out-of-band data, the @ref Option::So_OobInline option
                    requests that out-of-band data be placed in the normal data input queue
                    as received; it will then be accessible with @ref Recv() or @ref Read() calls
                    without the @ref MsgFlag::Msg_Oob flag. Some protocols always behave as if this option
                    is set.

                    @ref Option::So_SndBuf and @ref Option::So_RcvBuf are options to adjust the normal buffer sizes
                    allocated for output and input buffers, respectively. The buffer size
                    may be increased for high-volume connections, or may be decreased to
                    limit the possible backlog of incoming data.

                    @ref Option::So_SndLoWat is an option to set the minimum count for output operations.
                    Most output operations process all of the data supplied by the call,
                    delivering data to the protocol for transmission and blocking as necessary
                    for flow control. Nonblocking output operations will process as
                    much data as permitted subject to flow control without blocking, but will
                    process no data if flow control does not allow the smaller of the low
                    water mark value or the entire request to be processed. A @ref Select()
                    operation testing the ability to write to a socket will return true only
                    if the low water mark amount could be processed. The default value for
                    @ref Option::So_SndLoWat is set to a convenient size for network efficiency, often
                    1024.

                    @ref Option::So_RcvLoWat is an option to set the minimum count for input operations.
                    In general, receive calls will block until any (non-zero) amount of data
                    is received, then return with the smaller of the amount available or the
                    amount requested. The default value for @ref Option::So_RcvLoWat is 1. If
                    @ref Option::So_RcvLoWat is set to a larger value, blocking receive calls normally
                    wait until they have received the smaller of the low water mark value or
                    the requested amount. Receive calls may still return less than the low
                    water mark if an error occurs, or the type of data
                    next in the receive queue is different from that which was returned.

                    @ref Option::So_SndTimeo is an option to set a timeout value for output operations.
                    It accepts a @ref TimeVal argument with the number of seconds and
                    microseconds used to limit waits for output operations to complete. If a
                    send operation has blocked for this much time, it returns with a partial
                    count or with the error @ref Errno::EWouldBlock if no data was sent. In the current
                    implementation, this timer is restarted each time additional data are
                    delivered to the protocol, implying that the limit applies to output portions
                    ranging in size from the low water mark to the high water mark for output.

                    @ref Option::So_RcvTimeo is an option to set a timeout value for input operations. It
                    accepts a @ref TimeVal argument with the number of seconds and
                    microseconds used to limit waits for input operations to complete. In
                    the current implementation, this timer is restarted each time additional
                    data are received by the protocol, and thus the limit is in effect an
                    inactivity timer. If a receive operation has been blocked for this much
                    time without receiving additional data, it returns with a short count or
                    with the error @ref Errno::EWouldBlock if no data was received.

                    @ref Option::So_AcceptConn, @ref Option::So_Type, @ref Option::So_Protocol and
                    @ref Option::So_Error are options used only with @ref GetSockOpt(). @ref Option::So_AcceptConn returns
                    whether the socket is currently accepting connections, that is, whether
                    or not the @ref Listen() system call was invoked on the socket. @ref Option::So_Type
                    returns the type of the socket, such as @ref Type::Sock_Stream; it is useful for
                    servers that inherit sockets on startup. @ref Option::So_Protocol returns the protocol
                    number for the socket, for @ref Family::Af_Inet family.
                    @ref Option::So_Error returns any pending error on the socket and clears the error
                    status. It may be used to check for asynchronous errors on connected
                    datagram sockets or for other asynchronous errors.

                    @ref Option::So_ListenQLimit returns the maximal number of queued connections, as set
                    by @ref Listen(). @ref Option::So_ListenQLen returns the number of unaccepted complete
                    connections. @ref Option::So_ListenIncQLen returns the number of unaccepted incomplete connections.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Listen(), Recv(), Socket(), GetLastError()
*/
int SetSockOpt(int socket, Level level, Option optionName, const void* pOptionValue, SockLenT optionLength) NN_NOEXCEPT;

/**
    @brief          Get socket options.

    @param[in]      socket              Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      level               Level of the option
    @param[in]      optionName          Type of the option
    @param[out]     pOutOptionValue     Buffer containing option value
    @param[in, out] pOutOptionLength    Size of the buffer with option value

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                   | Description
    ------------------------|------------
    @ref Errno::EBadf       | The argument @a socket is not a valid descriptor.
    @ref Errno::ENotSock    | The argument @a socket is not a socket.
    @ref Errno::ENoProtoOpt | The @a optionName is unknown at the level indicated.
    @ref Errno::EInval      | The @a pOutOptionValue argument or @a pOutOptionLength argument are null.
    @ref Errno::EInval      | The value of the @a pOutOptionLength argument is not valid.
    @ref Errno::EFault      | The address range referred to by @a pOutOptionValue and the value of @a pOutOptionLength does not refer to a valid part of the process address space.
    @ref Errno::EAgain      | Resources to complete the request are temporarily unavailable.

    @details        The GetSockOpt() routine obtains socket options, See @ref SetSockOpt() for
                    supported options and their description.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Listen(), Recv(), Socket(), SetSockOpt(), GetLastError()
*/
int GetSockOpt(int socket, Level level, Option optionName, void* pOutOptionValue, SockLenT* pOutOptionLength) NN_NOEXCEPT;

/**
    @brief          Listen for connections on a socket.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      backlog         Length of the connection queue. Negative values or values greater
                                    than @ref SoMaxConn will set queue length to @ref SoMaxConn.

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                    | Description
    -------------------------|------------
    @ref Errno::EBadf        | The argument @a socket is not a valid descriptor.
    @ref Errno::EDestAddrReq | The socket is not bound to a local address, and the protocol does not support listening on an unbound socket.
    @ref Errno::EInval       | The socket is already connected, or in the process of being connected.
    @ref Errno::ENotSock     | The argument @a socket is not a socket.
    @ref Errno::EOpNotSupp   | The socket is not of a type that supports the operation Listen.
    @ref Errno::EAgain       | Resources to complete the request are temporarily unavailable.

    @details        The Listen() routine can be used to listen for connection on a socket.
                    To accept connections, a socket is first created with @ref Socket(), a
                    willingness to accept incoming connections and a queue limit for incoming
                    connections are specified with Listen(), and then the connections are
                    accepted with @ref Accept(). The Listen() applies only to
                    sockets of type @ref Type::Sock_Stream or @ref Type::Sock_SeqPacket.

                    The @a backlog argument defines the maximum length the queue of pending
                    connections may grow to. The real maximum queue length will be 1.5 times
                    more than the value specified in the backlog argument. A subsequent
                    listen call on the listening socket allows the caller to change
                    the maximum queue length using a new backlog argument. If a connection
                    request arrives with the queue full the client may receive an error with
                    an indication of @ref Errno::EConnRefused, or, in the case of TCP, the connection
                    will be silently dropped.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Accept(), Connect(), Socket(), SetSockOpt(), GetLastError()
*/
int Listen(int socket, int backlog) NN_NOEXCEPT;

/**
    @brief          Determine whether the read pointer is at the OOB mark.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()

    @return         1               Read pointer points at mark.
    @return         0               Read pointer does not point at mark.
    @return        -1               Error occurred.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error              | Description
    -------------------|------------
    @ref Errno::EBadf  | The @a socket argument is not a valid descriptor.
    @ref Errno::ENotTy | The @a socket argument is not a socket descriptor
    @ref Errno::EAgain | Resources to complete the request are temporarily unavailable.

    @details        The SockAtMark() routine can be used to find out if the read pointer
                    is currently pointing at the mark in the data stream. If SockAtMark()
                    returns 1, the next read will return data after the mark. Otherwise
                    (assuming out of band data has arrived), the next read will provide data
                    sent by the client prior to transmission of the out of band signal.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), Accept(), Read(), GetLastError()
*/
int SockAtMark(int socket) NN_NOEXCEPT;

/**
    @brief          Disable sends and/or receives on a socket.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      how             Shutdown method

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                  | Description
    -----------------------|------------
    @ref Errno::EBadf      | The @a socket argument is not a valid file descriptor.
    @ref Errno::EInval     | The @a how argument is invalid.
    @ref Errno::EOpNotSupp | The socket associated with the file descriptor @a socket does not support this operation.
    @ref Errno::ENotConn   | The @a socket argument specifies a @ref Type::Sock_Stream socket which is not connected.
    @ref Errno::ENotSock   | The @a socket argument does not refer to a socket.
    @ref Errno::EAgain     | Resources to complete the request are temporarily unavailable.

    @details        The Shutdown() routine disables sends or receives on a socket. The
                    @a how argument specifies the type of shutdown.

                    Possible values are:

                    Value                          | Description
                    -------------------------------|------------
                    @ref ShutdownMethod::Shut_Rd   | Further receives will be disallowed.
                    @ref ShutdownMethod::Shut_Wr   | Further sends will be disallowed. This may cause actions specific to the protocol family of the socket to happen.
                    @ref ShutdownMethod::Shut_RdWr | Further sends and receives will be disallowed. Implies SHUT_WR.

                    If the file descriptor @a socket is associated with a @ref Type::Sock_Stream socket, all or
                    part of the full-duplex connection will be shut down.

                    IMPLEMENTATION NOTES:

                    The following protocol-specific actions apply to the use of SHUT_WR (and
                    potentially also SHUT_RDWR), based on the properties of the socket
                    associated with the file descriptor socket.

                    Domain               |Type                   |Protocol                      |Return value and action
                    ---------------------|-----------------------|------------------------------|-----------------------
                    @ref Family::Af_Inet |@ref Type::Sock_Dgram  |@ref Protocol::IpProto_Udp    |Return 0. ICMP messages will not be generated
                    @ref Family::Af_Inet |@ref Type::Sock_Stream |@ref Protocol::IpProto_Tcp    |Return 0. Send queued data, wait for ACK, then send FIN

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), Connect(), GetLastError()
*/
int Shutdown(int socket, ShutdownMethod how) NN_NOEXCEPT;

/**
    @brief          Create endpoint for communication.

    @param[in]      domain          Communication domain
    @param[in]      type            Socket type
    @param[in]      protocol        Protocol type

    @return         Returns -1 on error. If it succeeds, it returns a non-negative integer that is a
                    descriptor for the created socket.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                       | Description
    ----------------------------|------------
    @ref Errno::EAcces          | Permission to create a socket of the specified type and/or protocol is denied.
    @ref Errno::EAfNoSupport    | The address family (domain) is not supported or the specified domain is not supported by this protocol family.
    @ref Errno::EMFile          | The per-process descriptor table is full.
    @ref Errno::ENoBufs         | Insufficient buffer space is available. The socket cannot be created until sufficient resources are freed.
    @ref Errno::EPerm           | User has insufficient privileges to carry out the requested operation.
    @ref Errno::EProtoNoSupport | The protocol type or the specified protocol is not supported within this domain.
    @ref Errno::EPrototype      | The socket type is not supported by the protocol.
    @ref Errno::EAgain          | Resources to complete the request are temporarily unavailable.

    @details        The Socket() routine creates an endpoint for communication and returns a descriptor.
                    The @a domain argument specifies a communications domain within which communication will take place;
                    this selects the protocol family which should be used.

                    The currently understood formats are:

                    Format                 |      Description
                    -----------------------|------------------------------------
                    @ref Family::Af_Inet   |  Internet version 4 protocols
                    @ref Family::Af_Route  |  Internal Routing protocol
                    @ref Family::Af_Link   |  Link layer interface

                    The socket has the indicated @a type, which specifies the semantics of
                    communication.

                    Currently defined types are:

                    Type                      |  Description
                    --------------------------|-------------------------
                    @ref Type::Sock_Stream    |  Stream socket,
                    @ref Type::Sock_Dgram     |  Datagram socket,
                    @ref Type::Sock_Raw       |  Raw-protocol interface
                    @ref Type::Sock_SeqPacket |  Sequenced packet stream

                    A @ref Type::Sock_Stream type provides sequenced, reliable, two-way connection based
                    byte streams. An out-of-band data transmission mechanism may be supported.

                    A @ref Type::Sock_Dgram socket supports datagrams (connectionless, unreliable
                    messages of a fixed (typically small) maximum length).

                    A @ref Type::Sock_SeqPacket socket may provide a sequenced, reliable, two-way
                    connection-based data transmission path for datagrams of fixed maximum length;
                    a consumer may be required to read an entire packet with each read system
                    call. This facility is protocol specific, and presently unimplemented.

                    @ref Type::Sock_Raw sockets provide access to internal network protocols and interfaces.

                    The types @ref Type::Sock_Raw, which is available only to the super-user, is
                    not described here.

                    Additionally, the following flag is allowed in the @a type argument:

                    Flag                          | Description
                    ------------------------------|------------
                    @ref Option::Sock_NonBlock | Set non-blocking mode on the new socket

                    The @a protocol argument specifies a particular protocol to be used with the
                    socket. Normally only a single protocol exists to support a particular
                    socket type within a given protocol family. However, it is possible that
                    many protocols may exist, in which case a particular protocol must be
                    specified in this manner. The @a protocol number to use is particular to
                    the "communication domain" in which communication is to take place.

                    The @a protocol argument may be set to @ref Type::Sock_Default to request the default
                    implementation of a socket type for the protocol, if any.

                    Sockets of type @ref Type::Sock_Stream are full-duplex byte streams, similar to
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

                    The communications protocols used to implement a @ref Type::Sock_Stream ensure that
                    data is not lost or duplicated. If a piece of data for which the peer
                    protocol has buffer space cannot be successfully transmitted within a
                    reasonable length of time, then the connection is considered broken and
                    calls will indicate an error with -1 returns and with @ref Errno::ETimedOut as the
                    specific code in the global variable @a errno. The protocols optionally
                    keep sockets "warm" by forcing transmissions roughly every minute in
                    the absence of other activity. An error is then indicated if no response
                    can be elicited on an otherwise idle connection for an extended period
                    (e.g. 5 minutes).

                    @ref Type::Sock_SeqPacket sockets employ the same system calls as @ref Type::Sock_Stream
                    sockets. The only difference is that @ref Read() calls will return only the
                    amount of data requested, and any remaining in the arriving packet will
                    be discarded.

                    @ref Type::Sock_Dgram and @ref Type::Sock_Raw sockets allow sending of datagrams to
                    correspondents named in @ref Send() calls. Datagrams are generally received with
                    @ref RecvFrom(), which returns the next datagram with its return address.

                    The operation of sockets is controlled by socket level options with @ref SetSockOpt()
                    and @ref GetSockOpt().

                    This function requires calling @ref Initialize() before it can be used.

    @see            Accept(), Bind(), Connect(), GetPeerName(), GetSockName(), GetSockOpt(), Ioctl(),
                    Listen(), Read(), Recv(), Select(), Shutdown(), Write(), GetLastError()
*/
int Socket(Family domain, Type type, Protocol protocol) NN_NOEXCEPT;

/**
    @brief          Write output.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      buffer          Buffer to send
    @param[in]      bufferLength    Length of the buffer to send

    @return         Returns the number of characters written, or -1 if an error occurred.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                    | Description
    -------------------------|------------
    @ref Errno::EBadf        | An invalid descriptor was specified.
    @ref Errno::ENotSock     | The argument @a socket is not a socket.
    @ref Errno::EMsgSize     | The socket requires that message be sent atomically, and the size of the message to be sent made this impossible.
    @ref Errno::EAgain       | The socket is marked non-blocking and the requested operation would block.
    @ref Errno::EAgain       | Resources to complete the request are temporarily unavailable.
    @ref Errno::EAcces       | The destination address is a broadcast address, and @ref Option::So_Broadcast has not been set on the socket.
    @ref Errno::ENoBufs      | The system was unable to allocate an internal buffer. The operation may succeed when buffers become available.
    @ref Errno::EHostUnreach | The remote host was unreachable.
    @ref Errno::EIsConn      | A destination address was specified and the socket is already connected.
    @ref Errno::EConnRefused | The socket received an ICMP destination unreachable message from the last message sent. This typically means that the receiver is not listening on the remote port.
    @ref Errno::EHostDown    | The remote host was down.
    @ref Errno::ENetDown     | The local network interface is down.
    @ref Errno::ENetUnreach  | The network is not reachable from this host.
    @ref Errno::EPipe        | The socket is unable to send anymore data (SBS_CANTSENDMORE has been set on the socket). This typically means that the socket is not connected.
    @ref Errno::EInval       | The @a buffer argument is null with non-zero @a bufferLength argument.
    @ref Errno::EFault       | The address range referred to by @a buffer and @a bufferLength does not refer to a valid part of the process address space.

    @details        The Write() routine attempts to write @a bufferLength bytes of data to the socket
                    referenced by the descriptor @a socket from the buffer pointed to by @a buffer.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), Send(), GetLastError()
*/
int Write(int socket, const void* buffer, size_t bufferLength) NN_NOEXCEPT;

/**
    @brief          Read input.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[out]     outBuffer       Buffer for data
    @param[in]      outBufferLength Size of the buffer

    @return         If successful, the number of bytes actually read is returned. Upon reading end-of-file, zero is returned.
                    A -1 is returned if an error occurred.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                  | Description
    -----------------------|------------
    @ref Errno::EBadf      | The argument @a socket is an invalid descriptor.
    @ref Errno::EConnReset | The remote socket end is forcibly closed.
    @ref Errno::ENetDown   | The network interface has been removed and the socket has been shut down.
    @ref Errno::ENotConn   | The socket is associated with a connection-oriented protocol and has not been connected (see @ref Connect() and @ref Accept()).
    @ref Errno::ENotSock   | The argument @a socket does not refer to a socket.
    @ref Errno::EAgain     | The socket is marked non-blocking, and the receive operation would block.
    @ref Errno::EAgain     | A receive timeout had been set, and the timeout expired before data was received.
    @ref Errno::EAgain     | Resources to complete the request are temporarily unavailable.
    @ref Errno::EInval     | The @a outBuffer argument is null with non-zero @a outBufferLength argument.
    @ref Errno::EFault     | The address range referred to by @a outBuffer and @a outBufferLength does not refer to a valid part of the process address space.

    @details        The Read() routine attempts to read @a outBufferLength bytes of data from the object
                    referenced by the descriptor @a socket into the buffer pointed to by @a outBuffer.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), Read()
*/
int Read(int socket, void* outBuffer, size_t outBufferLength) NN_NOEXCEPT;

/**
    @brief          Delete a descriptor.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                  | Description
    -----------------------|------------
    @ref Errno::EBadf      | The @a socket argument is not an active descriptor.
    @ref Errno::EConnReset | The underlying object was a stream socket that was shut down by the peer before all pending data was delivered.
    @ref Errno::EAgain     | Resources to complete the request are temporarily unavailable.

    @details        The Close() routine deletes a descriptor from the per-process object
                    reference table. The underlying object is deleted on the first close call.
                    All subsequent calls to the same object from other processes/threads after
                    the first close will return an error. This behavior deviates from POSIX
                    standard.

                    When a process exists, all associated descriptors are freed and all objects
                    created by that process are destroyed.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), GetLastError()
*/
int Close(int socket) NN_NOEXCEPT;

/**
    @brief          Synchronous I/O multiplexing.

    @param[in,out]  pSocketDescriptors  Array of @ref PollFd structures
    @param[in]      numberOfDescriptors Number of @ref PollFd structures
    @param[in]      timeoutMilliseconds Timeout in milliseconds

    @return         Number of descriptors that are ready for I/O, or -1 if an error occurred.
                    If the time limit expires, Poll() returns 0. If Poll() returns with an error,
                    file descriptor array will be unmodified.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error              | Description
    -------------------|------------
    @ref Errno::EInval | The specified time limit is invalid. One of its components is negative or too large.
    @ref Errno::EInval | The @a pSocketDescriptors argument is null with non-zero @a outBufferLength argument.
    @ref Errno::EFault | The address range referred to by @a pSocketDescriptors and @a numberOfDescriptors * sizeof(@ref PollFd) does not refer to a valid part of the process address space.
    @ref Errno::EAgain | Resources to complete the request are temporarily unavailable.

    @details        The Poll() system call examines a set of file descriptors to see if some
                    of them are ready for I/O. The @a pSocketDescriptors argument is a pointer to an array of
                    @ref PollFd structures). The @a numberOfDescriptors argument
                    determines the size of the array.

                    The event bitmasks in events and revents have the following bits:

                    Event                      |       Description
                    ---------------------------|------------------
                    @ref PollEvent::PollIn     | Data other than high priority data may be read without blocking.
                    @ref PollEvent::PollRdNorm | Normal data may be read without blocking.
                    @ref PollEvent::PollRdBand | Data with a non-zero priority may be read without blocking.
                    @ref PollEvent::PollPri    | High priority data may be read without blocking.
                    @ref PollEvent::PollOut    | Normal data may be written without blocking.
                    @ref PollEvent::PollWrNorm | Normal data may be written without blocking.
                    @ref PollEvent::PollWrBand | Data with a non-zero priority may be written without blocking.
                    @ref PollEvent::PollErr    | An exceptional condition has occurred on the device or socket. This flag is always checked, even if not present in the events bitmask.
                    @ref PollEvent::PollHup    | The device or socket has been disconnected. This flag is always checked, even if not present in the events bitmask.
                    @ref PollEvent::PollNVal   | The file descriptor is not open. This flag is always checked, even if not present in the events bitmask.

                    Note that @ref PollEvent::PollHup and @ref PollEvent::PollOut should never be present in the revents bitmask at the same time.

                    If @a timeout is neither zero nor @ref nn::socket::InfTim (-1), it specifies a maximum interval
                    to wait for any file descriptor to become ready, in milliseconds. If
                    @a timeout is @ref nn::socket::InfTim (-1), the poll blocks indefinitely. If @a timeout is
                    zero, then Poll() will return without blocking.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), Accept(), GetLastError()
*/
int Poll(PollFd* pSocketDescriptors, NfdsT numberOfDescriptors, int timeoutMilliseconds) NN_NOEXCEPT;

/**
    @brief          File control.

    @param[in]      socket          Socket descriptor obtained with @ref Socket() or @ref Accept()
    @param[in]      command         Control command

    @return         Returns the value 0 if successful; otherwise the value -1 is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error                  | Description
    -----------------------|------------
    @ref Errno::EBadf      | The argument @a socket is not a valid descriptor.
    @ref Errno::ENotSock   | The argument @a socket does not refer to a socket.
    @ref Errno::EOpNotSupp | Command is not supported
    @ref Errno::EAgain     | Resources to complete the request are temporarily unavailable.

    @details        The Fcntl() routine provides control over descriptors. The argument
                    @a socket is a descriptor to be operated on by @a command as described below.
                    Depending on the value of command, Fcntl() can take additional arguments.

                    Supported commands:

                    @ref nn::socket::FcntlCommand::F_GetFl    Get descriptor status flags. Additional arguments are ignored.

                    The descriptor flags for the @ref nn::socket::FcntlCommand::F_GetFl command are returned from fcntl. Flags
                    that are supported to be modified are documented in the @ref nn::socket::FcntlCommand::F_SetFl command below.

                    @ref nn::socket::FcntlCommand::F_SetFl    Set descriptor status flags. Takes an additional argument - 'flags'

                    The flags for the @ref nn::socket::FcntlCommand::F_SetFl flags are as follows:

                    @ref nn::socket::FcntlFlag::O_NonBlock Non-blocking I/O; if no data is available to a @ref Read
                                                           system call, or if a @ref Write operation would block, the read
                                                           or write call returns -1 with the error @ref Errno::EAgain.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), GetLastError()
*/
int Fcntl(int socket, FcntlCommand command, ...) NN_NOEXCEPT;

/**
    @brief          Synchronous I/O multiplexing.

    @param[in]      numberOfDescriptors     Highest descriptor id plus 1
    @param[in,out]  pReadDescriptors        Read descriptor @ref FdSet
    @param[in,out]  pWriteDescriptors       Write descriptor @ref FdSet
    @param[in,out]  pExceptDescriptors      Exception descriptor @ref FdSet
    @param[in]      pTimeout                Timeout

    @return         Returns the number of ready descriptors that are
                    contained in the descriptor sets, or -1 if an error occurred. If the
                    time limit expires, Select() returns 0. If Select() returns with an
                    error, the descriptor sets will be unmodified.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error              | Description
    -------------------|------------
    @ref Errno::EBadf  | One of the descriptor sets specified an invalid descriptor.
    @ref Errno::EInval | The specified time limit is invalid. One of its components is negative or too large.
    @ref Errno::EInval | The @a numberOfDescriptors argument was invalid.
    @ref Errno::EFault | One of the arguments @a pReadDescriptors, @a pWriteDescriptors, @a pExceptDescriptors, or @a pTimeout points to an invalid address.
    @ref Errno::EAgain | Resources to complete the request are temporarily unavailable.

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
                    @ref FdSetZero() initializes a descriptor set fdset to the null set.
                    @ref FdSetSet() includes a particular descriptor @a fd in fdset.
                    @ref FdSetClr() removes @a fd from fdset. @ref FdSetIsSet() is non-
                    zero if @a fd is a member of fdset, zero otherwise. The behavior of these
                    functions is undefined if a descriptor value is less than zero or greater
                    than or equal to @ref FdSetSize, which is normally at least equal to the
                    maximum number of descriptors supported by the system.

                    If @a timeout is not a null pointer, it specifies the maximum interval to
                    wait for the selection to complete. System activity can lengthen the
                    interval by an indeterminate amount.

                    If @a timeout is a null pointer, the select blocks indefinitely.

                    To effect a poll, the timeout argument should not be a null pointer, but
                    it should point to a zero-valued @ref TimeVal structure.

                    Any of descriptor sets may be given as null pointers if
                    no descriptors are of interest.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Accept(), Connect(), Poll(), Read(), Recv(), Send(), Write(), GetLastError(), Fd_Zero(), Fd_Set(), Fd_Clr(), Fd_IsSet()
*/
int Select(int numberOfDescriptors, FdSet*  pReadDescriptors, FdSet*  pWriteDescriptors, FdSet*  pExceptDescriptors, TimeVal* pTimeout) NN_NOEXCEPT;

/**
    @brief          Initializes a descriptor set @a s to the null set.

    @param[in]      pFdset  Descriptor set to be initialized to the null set.

    @details        Initializes a descriptor set @a s to the null set.

    @see            Select()
*/
inline void FdSetZero(FdSet *pFdset)
NN_NOEXCEPT
{
    int i;
    unsigned long *b = pFdset->fds_bits;

    for( i = sizeof(FdSet) / sizeof(long); i; i-- )
    {
        *b++ = 0;
    }
}

/**
    @brief          Includes a particular descriptor @a fd in @a fdset.

    @param[in]      fd      Descriptor which should be included.
    @param[in,out]  pFdset  Set which descriptor should be included in.

    @details        Includes a particular descriptor @a fd in @a pFdset.
                    The behavior of this function is undefined if a descriptor
                    value is less than zero or greater than or equal to @ref FdSetSize,
                    which is normally at least equal to the maximum number of
                    descriptors supported by the system.

    @see            Select()
*/
inline long FdSetSet(int fd, FdSet *pFdset)
NN_NOEXCEPT
{
    return pFdset->fds_bits[fd / (8 * sizeof(long))] |= (1UL << (fd % (8 * sizeof(long))));
}

/**
    @brief          Removes	@a fd from @a fdset.

    @param[in]      fd      Descriptor which should be removed.
    @param[in,out]  pFdset  Set which descriptor should be removed from.

    @details        Removes	@a fd from @a pFdset.
                    The behavior of this function is undefined if a descriptor
                    value is less than zero or greater than or equal to @ref FdSetSize,
                    which is normally at least equal to the maximum number of
                    descriptors supported by the system.

    @see            Select()
*/
inline void FdSetClr(int fd, FdSet *pFdset)
NN_NOEXCEPT
{
    pFdset->fds_bits[fd / (8 * sizeof(long))] &= ~(1UL << (fd % (8 * sizeof(long))));
}

/**
    @brief          Returns non-zero if @a fd is a member of @a pFdset, zero otherwise.

    @param[in]      fd      Descriptor which should be included.
    @param[in]      pFdset  Set which descriptor should be checked for set state.

    @return         Non-zero if @a fd is a member of @a pFdset, zero otherwise.

    @details        Returns non-zero if @a fd is a member of @a pFdset, zero otherwise.
                    The behavior of this function is undefined if a descriptor
                    value is less than zero or greater than or equal to @ref FdSetSize,
                    which is normally at least equal to the maximum number of
                    descriptors supported by the system.

    @see            Select()
*/
inline bool FdSetIsSet(int fd, const FdSet* pFdset)
NN_NOEXCEPT
{
    return !!(pFdset->fds_bits[fd / (8 * sizeof(long))] & (1UL << (fd % (8 * sizeof(long)))));
}

/**
    @brief          Control I/O device.

    @param[in]      fd              File descriptor for socket or device.
    @param[in]      command         Device control command.
    @param[in]      pData           Buffer containing command data.
    @param[in]      dataLength      Size of the buffer.

    @return         Returns result of the ioctl operation. If an error has occurred, a value of -1
                    is returned.

    @post           In the case of an error, the global variable @a errno will be set to one of the following:

    Error              | Description
    -------------------|------------
    @ref Errno::EBadf  | The @a fd argument is not a valid descriptor.
    @ref Errno::ENotTy | The specified request does not apply to the kind of object that the descriptor fd references.
    @ref Errno::ENotTy | The specified request does not apply to the kind of object that the descriptor fd references.
    @ref Errno::EInval | The request or @a pData argument is not valid.
    @ref Errno::EAcces | Command is not allowed.
    @ref Errno::EAgain | Resources to complete the request are temporarily unavailable.

    @details        The Ioctl() routine manipulates the underlying device parameters of
                    special files. The argument @a fd must be an open file descriptor obtained with
                    either @ref Socket(), @ref Accept().

                    The following Ioctl commands are supported:

                    Command                                 | Description
                    ----------------------------------------|------------
                    @ref nn::socket::IoctlCommand::FionRead  | Get the number of bytes that are immediately available for reading. For this command, @a pData should be a pointer to an int.
                    @ref nn::socket::IoctlCommand::FionWrite | Get the number of bytes in the descriptor's send queue. These bytes are data which has been written to the descriptor but which are being held by the kernel for further processing. The nature of the required processing depends on the underlying device. For TCP sockets, these bytes have not yet been acknowledged by the other side of the connection. For this command, @a pData should be a pointer to an int. This command is not supported on Windows builds.
                    @ref nn::socket::IoctlCommand::FionSpace | Get the free space in the descriptor's send queue. This value is the size of the send queue minus the number of bytes being held in the queue. Note: while this value represents the number of bytes that may be added to the queue, other resource limitations may cause a write not larger than the send queue's space to be blocked. One such limitation would be a lack of network buffers for a write to a network connection. For this command, @a pData should be a pointer to an int. This command is not supported on Windows builds.

                    Please note that @ref nn::socket::IoctlCommand::FionWrite and @ref nn::socket::IoctlCommand::FionSpace
                    are not supported in Windows builds. In the case that these commands are used for Windows builds,
                    this function will return -1 and the global variable errno will be set to @ref Errno::EInval.

                    This function requires calling @ref Initialize() before it can be used.

    @see            Socket(), Accept(), GetLastError()
*/
int Ioctl(int fd, IoctlCommand command, void* pData, size_t dataLength) NN_NOEXCEPT;

/**
    @brief          Return pointer to host errno (@a h_errno) for @ref GetHostEntByName() and @ref GetHostEntByAddr().

    @return         Returns pointer to host errno (@a h_errno) for @ref GetHostEntByName() and @ref GetHostEntByAddr().

    @details        This function is used only to determine error conditions, it is also thread-localized.

    @see            GetHostEntByName(), GetHostEntByAddr()

*/
HErrno* GetHError() NN_NOEXCEPT;

/**
    @brief          Look up @ref Family::Af_Inet host IP address by hostname.

    @param[in]      pName           The name of the host to look up (e.g. www.nintendo.com).

    @details        The return value of this function is thread-localized, this deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to keep the value between calls, a deep copy must be performed.

    @see            GetHError(), HStrError()

*/
HostEnt* GetHostEntByName(const char* pName) NN_NOEXCEPT;

/**
    @brief          Look up the address value for a host for @ref Family::Af_Inet.

    @param[in]      pAddress        This value shall be pointer to an @ref InAddr structure that contains a binary address in network byte order.
    @param[in]      length          The size of the structure
    @param[in]      addressFamily   Currently only @ref Family::Af_Inet is supported

    @return         Returns a pointer to a @ref HostEnt structure or NULL on error with error detail in @ref GetHError().

    @details        The return value of this function is thread-localized, which deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to keep the value between calls, a deep copy must be performed.

    @see            GetHError(), HStrError()
*/
HostEnt* GetHostEntByAddr(const void* pAddress, SockLenT length, Family addressFamily) NN_NOEXCEPT;

/**
    @brief          Return a human-readable version of h_error / @ref GetHError().

    @param[in]      errorNumber     The h_errno.

    @return         Returns a string representing the error number or NULL if out-of-memory.

    @details        The return value of this function is thread-localized, this deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to maintain the value between calls, a copy must be made.

    @see            GetHError(), GetHostEntByName(), GetHostEntByAddr()
*/
const char* HStrError(HErrno errorNumber) NN_NOEXCEPT;

/**
    @brief          Get a list of IP addresses and port numbers.

    @param[in]      pNodeName   The name of the host or NULL.
    @param[in]      pServername The name of the service or NULL.
    @param[in]      pHints      An optional pointer to an @ref AddrInfo structure.
    @param[out]     pResult     An out parameter containing 0, 1, or many (via @a ai_next) @ref AddrInfo structures.

    @return         Returns @ref AiErrno::EAi_Success on success or one of the error codes listed in @ref GAIStrError() if an error occurs.

    @details        GetAddrInfo() routine can be used to obtain list of IP addresses and port numbers for
                    host @a pNodename and service @a pServername. It provides more flexibility than @ref GetHostEntByName().

                    Please note that @a pNodename and @a pServername cannot both be NULL at the same time.

                    The @ref AddrInfo result @a pResult returned by this function can be deallocated via @ref FreeAddrInfo().

                    The optional @a pHints can be used to provide hints concerning the type of socket
                    that the caller supports or wishes to use. The caller can supply the
                    following structure elements in @a pHints:

                    <table>
                        <tr>
                            <th>Element</th>
                            <th>Description</th>
                        </tr>
                        <tr>
                            <td>@a ai_family</td>
                            <td>The protocol family that should be used. When @a ai_family is set to @ref Family::Af_Unspec,
                                it means the caller will accept any protocol family supported by the operating system.
                            </td>
                        </tr>
                        <tr>
                            <td>@a ai_socktype</td>
                            <td>Denotes the type of socket that is wanted: @ref Type::Sock_Stream, @ref Type::Sock_Dgram,
                                or @ref Type::Sock_Raw. When @a ai_socktype is zero the caller will accept any socket type.
                            </td>
                        </tr>
                        <tr>
                            <td>@a ai_protocol</td>
                            <td>Indicates which transport protocol is desired, @ref Protocol::IpProto_Udp or @ref Protocol::IpProto_Tcp. If
                                @a ai_protocol is zero the caller will accept any protocol.</td>
                        </tr>
                        <tr>
                            <td>@a ai_flags</td>
                            <td>The @a ai_flags field to which the hints parameter points shall be set to zero or be the
                                bitwise-inclusive OR of one or more of the values:
                                <table>
                                    <tr>
                                        <th>Flag</th>
                                        <th>Description</th>
                                    </tr>
                                    <tr>
                                        <td>@ref AddrInfoFlag::Ai_AddrConfig</td>
                                        <td>
                                            If the @ref AddrInfoFlag::Ai_AddrConfig bit is set, IPv4 addresses shall be
                                            returned only if an IPv4 address is configured on the local system, and
                                            IPv6 addresses shall be returned only if an IPv6 address is
                                            configured on the local system.
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>@ref AddrInfoFlag::Ai_CanonName</td>
                                        <td>
                                            If the @ref AddrInfoFlag::Ai_CanonName bit is set, a successful call to
                                            @ref GetAddrInfo() will return a NULL-terminated string containing
                                            the canonical name of the specified hostname in the @a ai_canonname
                                            element of the first @ref AddrInfo structure returned.
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>@ref AddrInfoFlag::Ai_NumericHost</td>
                                        <td>
                                            If the @ref AddrInfoFlag::Ai_NumericHost bit is set, it indicates that hostname
                                            should be treated as a numeric string defining an IPv4 or IPv6 address and
                                            resolution should be attempted.
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>@ref AddrInfoFlag::Ai_NumericServ</td>
                                        <td>
                                            If the @ref AddrInfoFlag::Ai_NumericServ bit is set, then a non-NULL servname
                                            string supplied shall be a numeric port string. Otherwise, an
                                            @ref AiErrno::EAi_NoName error shall be returned. This bit shall prevent any
                                            type of name resolution service (for example, NIS+) from being invoked.
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>@ref AddrInfoFlag::Ai_Passive</td>
                                        <td>
                                            If the @ref AddrInfoFlag::Ai_Passive bit is set it indicates that the
                                            returned socket address structure is intended for use in a call to
                                            @ref Bind(). In this case, if the hostname argument is the null pointer,
                                            then the IP address portion of the socket address structure will be set
                                            to @ref InAddr_Any for an IPv4 address. If the @ref AddrInfoFlag::Ai_Passive
                                            bit is not set, the returned socket address structure will be ready
                                            for use in a call to @ref Connect() for a connection-oriented protocol
                                            or @ref Connect() or @ref SendTo() if a connectionless
                                            protocol was chosen. The IP address portion of the socket address
                                            structure will be set to the loopback address if hostname is the null
                                            pointer and @ref AddrInfoFlag::Ai_Passive is not set.
                                        </td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                    </table>

                    All other elements of the @ref AddrInfo structure passed via @a pHints must be
                    zero or the null pointer.

                    If @a pHints is the null pointer, GetAddrInfo() behaves as if the caller provided
                    an @ref AddrInfo structure with @a ai_family set to @ref Family::Af_Unspec and all other
                    elements set to zero or NULL.

    @see            FreeAddrInfo(), GetNameInfo(), GAIStrError()
*/
AiErrno GetAddrInfo(const char* pNodeName, const char* pServername, const AddrInfo* pHints, AddrInfo** pResult) NN_NOEXCEPT;

/**
    @brief          Free the address info structure.

    @param[in]      addrInfoStorage Pointer to an @ref AddrInfo structure that was returned by @ref GetAddrInfo() or @ref FreeAddrInfo()

    @details        The FreeAddrInfo() routine can be used to free structured returned by @ref GetAddrInfo() / @ref GetNameInfo().

    @see            GetNameInfo(), GetAddrInfo()
*/
void FreeAddrInfo(AddrInfo* addrInfoStorage) NN_NOEXCEPT;

/**
    @brief          Return a human-readable version of the return value from @ref GetAddrInfo() or @ref GetNameInfo().

    @param[in]      errorCode       Error code for translation to human readable format.

    @return         Returns a c-string human readable representation of the error code provided.

    @details        The GAIStrError() routine is used to obtain human readable version of the return value from @ref GetAddrInfo()
                    and @ref GetNameInfo(). The return value of this function is thread-localized, which deviates from some POSIX implementations.
                    The return value is managed internally, so if you wish to maintain the value between calls a copy must be made.

    @see            GetNameInfo(), GetAddrInfo()
*/
const char* GAIStrError(AiErrno errorCode) NN_NOEXCEPT;

/**
    @brief          Convert a @ref SockAddr structure to a pair of host name and service strings.

    @param[in]      socketAddress       The @ref SockAddr structure socketAddress should point to a @ref SockAddrIn that is socketAddressLength bytes long.
    @param[in]      socketAddressLength The size of the SockAddr structure.
    @param[out]     host                The hostname buffer.
    @param[in]      hostLength          The length of the hostname buffer.
    @param[out]     service             The service name buffer.
    @param[in]      serviceLength       The length of the service name buffer.
    @param[in]      flags               Flags.

    @return         Returns @ref AiErrno::EAi_Success on success or one of the error codes listed in @ref GAIStrError() if an error occurs.

    @details        The GetNameInfo() function is used to convert a @ref SockAddr structure to a pair of host name and service strings.
                    It provides more flexibility than @ref GetHostEntByAddr() and is the converse of the @ref GetAddrInfo() function.

                    The @a flags argument is formed by OR'ing the following values from socket_Types.h:

                    Flag                              | Description
                    ----------------------------------|------------
                    @ref NameInfoFlag::Ni_NoFqdn      | A fully qualified domain name is not required for local hosts. The local part of the fully qualified domain name is returned instead.
                    @ref NameInfoFlag::Ni_NumericHost | Return the address in numeric form, as if calling @ref InetNtop(), instead of a host name.
                    @ref NameInfoFlag::Ni_NameReqd    | A name is required. If the host name cannot be found in DNS and this flag is set, a non-zero error code is returned. If the host name is not found and the flag is not set, the address is returned in numeric form.
                    @ref NameInfoFlag::Ni_NumericServ | The service name is returned as a digit string representing the port number.
                    @ref NameInfoFlag::Ni_Dgram       | Specifies that the service being looked up is a datagram service.

    @see            FreeAddrInfo(), GetAddrInfo(), GAIStrError()
*/
AiErrno GetNameInfo(const SockAddr* socketAddress, SockLenT socketAddressLength, char* host, size_t hostLength, char* service, size_t serviceLength, NameInfoFlag flags) NN_NOEXCEPT;

/**
    @brief          Initialize socket library

    @param[in]      config              An instance of @ref Config, which supplies socket library configuration

    @return         Returns one of the following values:
    @return         nn::ResultSuccess()                  Call was successful.
    @return         ResultInsufficientProvidedMemory()   Insufficient Memory Provided. @ref nn::socket::MinSocketMemoryPoolSize is required.

    @pre
     - The socket library is not in the initialized state.
     - The socket library user will make no further adjustments to passed @a config object.

    @post
     - The socket library is in the initialized state.
     - Memory supplied via @a config object is used exclusively by socket library and should not be accessed or freed.

    @details        This method must be called first before attempting to call any other socket library API.
                    It is not valid to call this API multiple times in succession without first calling
                    @ref Finalize().

                    Upon call to @ref Initialize() memory supplied via @a config object will be used
                    exclusively by the socket library.

                    See socket library functional overview documentation for details about how to
                    instantiate @a config.

    @see            Finalize()
*/
Result Initialize(const Config& config) NN_NOEXCEPT;

/**
    @brief          Create an event fd

    @param[in]      initialValue        Initial, unsigned counter value associated with this event fd.  Must not be
                                        0xFFFFFFFFFFFFFFFF.  All other values are permitted.  Its interpretation
                                        depends on the flags value.
    @param[in]      flags               One of the following:
                        None                 If the counter is non-zero, then its value is returned with
                                             nn::socket::Read() and is reset to zero.  If the counter is
                                             zero at the time of the call, then the nn::socket::Read()
                                             call will block.  If a write causes a counter wrap around,
                                             it will block.
                        Semaphore            If the counter is non-zero, it is decremented by one and a
                                             value of 1 is read.  If the counter is zero, then the
                                             nn::socket::Read() call will block.  If a write causes a
                                             counter wrap around, it will block.
                        Nonblock             If the counter is non-zero, then the behavior is the same as
                                             if no flags were specified.  If the counter is zero,
                                             nn::socket::Read() returns -1 and nn::socket::GetLastError()
                                             needs to be used to retrieve the error code, EAGAIN.
                                             If a write causes a counter wrap around, it will return -1
                                             and nn::socket::GetLastError() will read EAGAIN.
                        SemaphoreAndNonblock Same behavior as Semaphore if the counter is non-zero, and
                                             nn::socket::Read() returns -1 and nn::socket::GetLastError()
                                             needs to be used to retrieve the error code, EAGAIN, if the
                                             counter is zero.  Similarly for write, if the counter wraps
                                             around an error of -1 will be returned with GetLastError() being EAGAIN.

    @return         Returns a valid file handle or -1 in case of an error.  In case of error, use nn::socket::GetLastError()
                    to determine details of the failure.
    @pre            The socket library must be initialized.
    @post           A file handle to an event fd synchronization object has been created.
    @details        The counter associated with the event fd is accessed using nn::socket::Read() and nn::socket::Write() by
                    reading and writing 8 bytes (sizeof(uint64_t)).  The value written is added to the counter value.  The
                    behavior of nn::socket::Read() and nn::socket::Write() depends on the flags.
                    An event fd is intended to be used in conjunction with nn::socket::Poll() or nn::socket::Select().
                    Whenever a call to nn::socket::Read() will succeed and not block, POLLIN will be set for an event fd.
                    Multiple fds can be given to nn::socket::Poll() or nn::socket::Select(), and an event fd can be used to
                    unblock a poll on one or more socket fds.
*/
int EventFd(uint64_t initialValue, EventFdFlags flags) NN_NOEXCEPT;

/**
    @brief          Read from an event fd.

    @param[in]      fd          File descriptor to read from.
    @param[out]     value       Pointer to value variable.
    @return         Success or failure in posix sense: 0 success, -1 failure.
    @pre            The socket library must be initialized.
    @post           The EventFd counter has been read.  The value read and counter post read depend on the flags.
    @details        No internal check is performed whether the fd is in fact an EventFd.  This function is a wrapper
                    for Read() supplying appropriately sized operands and that checks the return value from
                    the underlying function call only.
    @details        The blocking behavior of EventFdRead is determined by flags used when creating the fd.
    @details        @ref EventFd(uint64_t initialValue, EventFdFlags flags) documents the read semantics.
    @details        An error is returned when incorrect or invalid arguments are supplied.
*/
int EventFdRead(int fd, EventFdType* value) NN_NOEXCEPT;

/**
    @brief          Write to an event fd.

    @param[in]      fd          File descriptor to write to.
    @param[in]      value       Value variable.
    @return         Success or failure in posix sense: 0 success, -1 failure.
    @pre            The socket library must be initialized.
    @post           The EventFd counter has been incremented by value.
    @details        No internal check is performed whether the fd is in fact an EventFd.  This function is a wrapper
                    for Write() supplying appropriately sized operands and that check the return value from
                    the underlying function call only.
    @details        Depending on flags, EventFdWrite() can block if the internal counter would wrap around.
                    Counter wrap around will not occur in a realistic use case.
    @details        @ref EventFd(uint64_t initialValue, EventFdFlags flags) documents the write semantics.
    @details        An error is returned when incorrect or invalid arguments are supplied.
*/
int EventFdWrite(int fd, EventFdType value) NN_NOEXCEPT;

/**
    @brief          Initialize socket library

    @param[in]      memoryPool          Pointer to the start of the memory region; must be 4kB aligned.
    @param[in]      memoryPoolSize      Total size of the memory region; must be a multiple of 4kB.
    @param[in]      allocatorPoolSize   Portion of the memory region to be used for resolver; must be a multiple of 4kB.
    @param[in]      concurrencyLimit    nn::socket::DefaultConcurrencyLimit should always be passed.

    @return         This function currently cannot fail and always returns nn::ResultSuccess().
                    Failure would mean a fatal OS error has occurred.

    @pre
     - The socket library is not in the initialized state.

    @post
     - The socket library is in the initialized state.
     - Memory supplied via @a memoryPool is used exclusively by socket library and should not be accessed or freed.

    @details        Use of this API is discouraged. It is preferred to instead use
                    @ref Initialize(const Config& config).
                    It is not valid to call this API multiple times in succession without first calling
                    @ref Finalize().

                    Upon call to Initialize(), memory supplied via @a memoryPool will be used
                    exclusively by the socket library.

                    See socket library functional overview documentation for details about how to
                    appropriately size parameters @a memoryPoolSize and @a allocatorPoolSize.

    @see            Finalize()
*/
Result Initialize(
    void* memoryPool,
    size_t memoryPoolSize,
    size_t allocatorPoolSize,
    int concurrencyLimit) NN_NOEXCEPT;

/**
    @brief          Finalize socket library.

    @return         This function currently cannot fail and always returns nn::ResultSuccess().
                    Failure would mean a fatal OS error has occurred.

    @pre
     - The socket library is in the initialized state.
     - The socket library user has synchronized its threads to prevent subsequent socket library API calls.

    @post
     - Remaining allocated socket descriptors will have been automatically closed.
     - The socket library is no longer in the initialized state.
     - Memory supplied via @a config upon prior call to @ref Initialize() may now be freed or repurposed.
     - Subsequent calls to socket library API will fail.

    @details        This method sets the library to an uninitialized state.
                    Memory supplied upon prior call to @ref Initialize() will no longer be used by
                    the socket library.

    @see            Initialize()
*/
Result Finalize() NN_NOEXCEPT;

}} /* nn::socket */
