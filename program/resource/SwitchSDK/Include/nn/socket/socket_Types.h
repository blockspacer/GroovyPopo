/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

#include <nn/nn_Macro.h>  // NN_NOEXCEPT

#include <nn/socket/socket_Options.h>
#include <nnc/socket/socket_Types.h>

namespace nn {
namespace socket {

typedef uint16_t        InPortT;    ///< Port number.
typedef uint32_t        InAddrT;    ///< IP address.
typedef unsigned int    SockLenT;   ///< Used for length and size values for socket-related parameters.
typedef unsigned long   NfdsT;      ///< Used for the number of file descriptors.
typedef unsigned long   FdMask;     ///< File descriptor mask.

const unsigned int FdSetSize            = 1024; ///< Max size of FdSet

const InAddrT InAddr_Any                = 0x00000000;   ///< 0.0.0.0.
const InAddrT InAddr_Broadcast          = 0xffffffff;   ///< 255.255.255.255.
const InAddrT InAddr_None               = 0xffffffff;   ///< 255.255.255.255.
const InAddrT InAddr_Loopback           = 0x7f000001;   ///< 127.0.0.1.
const InAddrT InAddr_Unspec_Group       = 0xe0000000;   ///< 224.0.0.0.
const InAddrT InAddr_AllHosts_Group     = 0xe0000001;   ///< 224.0.0.1.
const InAddrT InAddr_AllRtrs_Group      = 0xe0000002;   ///< 224.0.0.2.
const InAddrT InAddr_Max_Local_Group    = 0xe00000ff;   ///< 224.0.0.255.

/**
    IP protocols.
*/
enum class Protocol : int32_t
{
    IpProto_Ip      = 0,    ///< Internet protocol.
    IpProto_Icmp    = 1,    ///< Control message protocol.
    IpProto_Tcp     = 6,    ///< Transmission control protocol.
    IpProto_Udp     = 17,   ///< User datagram protocol.
    IpProto_None    = 59,   ///< No protocol.
    IpProto_UdpLite = 136,  ///< User datagram protocol, lite mode.
    IpProto_Raw     = 255,  ///< Raw IP packets protocol.

    IpProto_Max     = 256
};

/**
    Socket type, which specifies the semantics of communication.
*/
enum class Type : uint32_t
{
    Sock_Default   = 0,     ///< Default socket type for the protocol, if any. This is used for the @ref Socket() call.
    Sock_Stream    = 1,     ///< Stream socket.
    Sock_Dgram     = 2,     ///< Datagram socket.
    Sock_Raw       = 3,     ///< Raw-protocol interface.
    Sock_SeqPacket = 5,     ///< Sequenced packet stream.

    Sock_NonBlock  = 0x20000000,   ///< Set non-blocking mode on the new socket.
};

/**
    Address and protocol families.
*/
enum class Family : uint8_t
{
    Af_Unspec = 0,          ///< Unspecified.
    Pf_Unspec = Af_Unspec,  ///< Unspecified.
    Af_Inet   = 2,          ///< Internet Version 4.
    Pf_Inet   = Af_Inet,    ///< Internet Version 4.
    Af_Route  = 17,         ///< Communication with kernel routing layer.
    Pf_Route  = Af_Route,   ///< Communication with kernel routing layer.
    Af_Link   = 18,         ///< Raw link-layer access.
    Pf_Link   = Af_Link,    ///< Raw link-layer access.
    Af_Inet6  = 28,         ///< Internet Version 6.
    Pf_Inet6  = Af_Inet6,   ///< Internet Version 6.

    Af_Max    = 42,
    Pf_Max    = Af_Max
};

/**
    Message flags.
*/
enum class MsgFlag : int32_t
{
    Msg_None         = 0x00000,         ///< No flags set.
    Msg_Oob          = 0x00001,         ///< Process out-of-band data.
    Msg_Peek         = 0x00002,         ///< Peek at incoming message.
    Msg_DontRoute    = 0x00004,         ///< Send without using routing tables.
    Msg_Eor          = 0x00008,         ///< Data completes record.
    Msg_Trunc        = 0x00010,         ///< Data discarded before delivery.
    Msg_CTrunc       = 0x00020,         ///< Control data lost before delivery.
    Msg_WaitAll      = 0x00040,         ///< Wait for full request or error.
    Msg_DontWait     = 0x00080,         ///< This message should be nonblocking.
    Msg_Eof          = 0x00100,         ///< Data completes connection.
    Msg_Notification = 0x02000,         ///< SCTP notification.
    Msg_Nbio         = 0x04000,         ///< FIONBIO mode, used by fifofs.
    Msg_Compat       = 0x08000,         ///< Used in sendit().
    Msg_SoCallbck    = 0x10000,         ///< For use by socket callbacks.
    Msg_NoSignal     = 0x20000,         ///< Do not generate SIGPIPE on EOF.
    Msg_CMsg_CloExec = 0x40000          ///< Make received fds close-on-exec.
};

/**
    Poll events.
*/
enum class PollEvent : int16_t
{
    PollNone     = 0x0000,

    /** Data other than high priority data may be read without blocking. */
    PollIn       = 0x0001,

    /** High priority data may be read without blocking. */
    PollPri      = 0x0002,

    /** File descriptor is writeable. */
    PollOut      = 0x0004,

    /**
        An exceptional condition has occurred on the device or socket. This flag
        is always checked, even if not present in the events bitmask.
    */
    PollErr      = 0x0008,

    /**
        The device or socket has been disconnected. This flag is always checked,
        even if not present in the events bitmask. Note that PollHup and PollOut
        should never be present in the revents bitmask at the same time.
    */
    PollHup      = 0x0010,

    /**
        The file descriptor is not open. This flag is always checked, even if
        not present in the events bitmask.
    */
    PollNVal     = 0x0020,

    /** Normal data may be read without blocking. */
    PollRdNorm   = 0x0040,

    /** Data with a non-zero priority may be read without blocking. */
    PollRdBand   = 0x0080,

    /** Normal data may be written without blocking. */
    PollWrNorm   = 0x0100,

    /** Data with a non-zero priority may be written without blocking. */
    PollWrBand   = 0x0200,

    PollMsg      = 0x0400,
    PollRdHup    = 0x2000,
    PollInIgnEof = 0x3000
};

/**
   Flag values for @ref nn::socket::GetAddrInfo().
*/
enum class AddrInfoFlag : uint32_t
{
    /**
        No flags set.
    */
    Ai_None         = 0x00000000,

    /**
        If this bit is set, it indicates that the returned socket address
        structure is intended for use in a call to @ref Bind().
    */
    Ai_Passive      = 0x00000001,

    /**
        If this bit is set, a successful call to @ref GetAddrInfo() will return
        a NULL-terminated string containing the canonical name of the specified
        hostname in the @a ai_canonname element of the first @ref AddrInfo structure
        returned.
    */
    Ai_CanonName    = 0x00000002,

    /**
        If this bit is set, it indicates that hostname should be treated as a
        numeric string defining an IPv4 address and no name resolution should be
        attempted.
    */
    Ai_NumericHost  = 0x00000004,

    /**
        If this bit is set, then a non-null servname string supplied shall be a
        numeric port string.
    */
    Ai_NumericServ  = 0x00000008,

    /**
        If this bit is set, IPv4 addresses shall be returned only if an IPv4
        address is configured on the local system.
    */
    Ai_AddrConfig   = 0x00000400
};

/**
    Flag values for @ref nn::socket::GetNameInfo().
*/
enum class NameInfoFlag : uint32_t
{
    /**
        No flags set.
    */
    Ni_None         = 0x00000000,

    /**
        A fully qualified domain name is not required for local hosts. The local
        part of the fully qualified domain name is returned instead.
    */
    Ni_NoFqdn       = 0x00000001,

    /**
        Return the address in numeric form, as if calling @ref InetNtop(),
        instead of a host name.
    */
    Ni_NumericHost  = 0x00000002,

    /**
        A name is required. If the host name cannot be found in DNS and this
        flag is set, a non-zero error code is returned. If the host name is not
        found and the	flag is not set, the address is returned in numeric form.
    */
    Ni_NameReqd     = 0x00000004,

    /**
        The service name is returned as a digit string representing the port
        number.
    */
    Ni_NumericServ  = 0x00000008,

    /** Specifies that the service being looked up is a datagram service. */
    Ni_Dgram        = 0x00000010
};

/*
 * Constants for getnameinfo()
 */
const unsigned int	Ni_MaxHost = 1025;
const unsigned int	Ni_MaxServ = 32;

/**
    Shutdown method used for @ref nn::socket::Shutdown().
*/
enum class ShutdownMethod : uint32_t
{
    Shut_Rd   = 0,  ///< Shut down the reading side.
    Shut_Wr   = 1,  ///< Shut down the writing side.
    Shut_RdWr = 2   ///< Shut down both sides.
};

/**
    Commands used for @ref nn::socket::Ioctl().
*/
enum class IoctlCommand : uint32_t
{
    /**
        Get the number of bytes that are immediately available for reading.
    */
    FionRead        = 0x4004667f,

    /**
        Get the number of bytes in the descriptor's send queue. These
        bytes are data which has been written to the descriptor but which
        are being held by the kernel for further processing. The nature
        of the required processing depends on the underlying device. For
        TCP sockets, these bytes have not yet been acknowledged by the
        other side of the connection.
    */
    FionWrite       = 0x40046677,

    /**
        Get the free space in the descriptor's send queue. This value is
        the size of the send queue minus the number of bytes being held
        in the queue.  Note: while this value represents the number of
        bytes that may be added to the queue, other resource limitations
        may cause a write not larger than the send queue's space to be
        blocked.  One such limitation would be a lack of network buffers
        for a write to a network connection.
    */
    FionSpace       = 0x40046676
};

/**
    Commands used for @ref nn::socket::Fcntl().
*/
enum class FcntlCommand : uint32_t
{
    /**
        Get descriptor status flags.
    */
    F_GetFl  = 3,

    /**
        Set descriptor status flags.
    */
    F_SetFl  = 4
};

/**
    Commands used for @ref nn::socket::Fcntl().
*/
enum class FcntlFlag : uint32_t
{
    /**
        No flags set.
    */
    None        = 0x0000,

    /**
        Non-blocking mode.
    */
    O_NonBlock  = 0x0800
};

/**
    Flags used for @ref nn::socket::EventFd().
*/
enum class EventFdFlags : int
{
    None                 = 0x00,
    Semaphore            = 0x01,
    Nonblock             = 0x04,
    SemaphoreAndNonblock = 0x05
};

/**
    Type for EventFdRead and EventFdWrite
*/

typedef uint64_t    EventFdType;

/** Structure for storing Internet address in network format. */
struct InAddr
{
    InAddrT S_addr; ///< Internet address in network format
};

/**
    Structure used to store most addresses.
*/
struct SockAddr
{
    unsigned char   sa_len;         ///< Total length.
    Family          sa_family;      ///< Socket address family.
    char            sa_data[14];    ///< Actually longer; address value.
};

/**
    Structure used to specify a local or remote endpoint address to which to
    connect a socket.
*/
struct SockAddrIn
{
    uint8_t         sin_len;        ///< Total length.
    Family          sin_family;     ///< Socket address family.
    InPortT         sin_port;       ///< IP port.
    InAddr          sin_addr;       ///< IP address.
    uint8_t         sin_zero[8];    ///< Padding to make structure the same size as @ref SockAddr.
};

NN_DEPRECATED const unsigned int SSMaxSize = 128U;
const unsigned int SS_MaxSize = 128U;
NN_DEPRECATED const unsigned int SSAlignSize = sizeof(int64_t);
const unsigned int SS_AlignSize = sizeof(int64_t);
NN_DEPRECATED const unsigned int SSPad1Size = SS_AlignSize - sizeof(unsigned char) - sizeof(Family);
const unsigned int SS_Pad1Size = SS_AlignSize - sizeof(unsigned char) - sizeof(Family);
NN_DEPRECATED const unsigned int SSPad2Size = SS_MaxSize - sizeof(unsigned char) - sizeof(Family) - SS_Pad1Size - SS_AlignSize;
const unsigned int SS_Pad2Size = SS_MaxSize - sizeof(unsigned char) - sizeof(Family) - SS_Pad1Size - SS_AlignSize;

/**
    Structure for storing socket address information.
*/
struct SockAddrStorage
{
    unsigned char   ss_len;                     ///< Address length.
    Family          ss_family;                  ///< Socket address family.
    char            _ss_pad1[SS_Pad1Size];       ///<
    int64_t         _ss_align;                  ///<
    char            _ss_pad2[SS_Pad2Size];       ///<
};

/**
    Structure used by @ref nn::socket::GetAddrInfo() to hold host address information.
*/
struct AddrInfo
{
    AddrInfoFlag    ai_flags;       ///< Input flags,
    Family          ai_family;      ///< Protocol family for socket,
    Type            ai_socktype;    ///< Socket type.
    Protocol        ai_protocol;    ///< Protocol for socket.
    SockLenT        ai_addrlen;     ///< Length of socket-address.
    SockAddr*       ai_addr;        ///< Socket-address for socket.
    char*           ai_canonname;   ///< Canonical name for service location.
    AddrInfo*       ai_next;        ///< Pointer to next in list.
};

/**
    Structure used for manipulating linger option.
*/
struct Linger
{
    /**
        This field is interpreted as a boolean. If nonzero, close blocks until
        the data are transmitted or the timeout period has expired.
    */
    int l_onoff;

    /** This specifies the timeout period, in seconds. */
    int l_linger;
};

/**
    Structure to be passed to @ref nn::socket::SetSockOpt() when setting the
    @ref Option::Tcp_Info option.
*/
struct TcpInfo
{
    uint8_t         tcpi_state;             ///< TCP FSM state.
    uint8_t         _tcpi_ca_state;
    uint8_t         _tcpi_retransmits;
    uint8_t         _tcpi_probes;
    uint8_t         _tcpi_backoff;
    TcpInfoOption   tcpi_options;           ///< Options enabled on connection.
    uint8_t         tcpi_snd_wscale:4,      ///< RFC1323 send shift value.
                    tcpi_rcv_wscale:4;      ///< RFC1323 recv shift value.

    uint32_t        tcpi_rto;               ///< Retransmission timeout (usec).
    uint32_t        _tcpi_ato;              ///<
    uint32_t        tcpi_snd_mss;           ///< Max segment size for send.
    uint32_t        tcpi_rcv_mss;           ///< Max segment size for receive.

    uint32_t        _tcpi_unacked;
    uint32_t        _tcpi_sacked;
    uint32_t        _tcpi_lost;
    uint32_t        _tcpi_retrans;
    uint32_t        _tcpi_fackets;

    /* Times; measurements in usecs. */
    uint32_t        _tcpi_last_data_sent;
    uint32_t        _tcpi_last_ack_sent;
    uint32_t        tcpi_last_data_recv;    ///< Time since last recv data.
    uint32_t        _tcpi_last_ack_recv;

    /* Metrics; variable units. */
    uint32_t        _tcpi_pmtu;
    uint32_t        _tcpi_rcv_ssthresh;
    uint32_t        tcpi_rtt;               ///< Smoothed RTT in usecs.
    uint32_t        tcpi_rttvar;            ///< RTT variance in usecs.
    uint32_t        tcpi_snd_ssthresh;      ///< Slow start threshold.
    uint32_t        tcpi_snd_cwnd;          ///< Send congestion window.
    uint32_t        _tcpi_advmss;
    uint32_t        _tcpi_reordering;

    uint32_t        _tcpi_rcv_rtt;
    uint32_t        tcpi_rcv_space;         ///< Advertised recv window.

    /* FreeBSD extensions to tcp_info (TcpInfo). */
    uint32_t        tcpi_snd_wnd;           ///< Advertised send window.
    uint32_t        tcpi_snd_bwnd;          ///< No longer used.
    uint32_t        tcpi_snd_nxt;           ///< Next egress seqno.
    uint32_t        tcpi_rcv_nxt;           ///< Next ingress seqno.
    uint32_t        tcpi_toe_tid;           ///< HWTID for TOE endpoints.
    uint32_t        tcpi_snd_rexmitpack;    ///< Retransmitted packets.
    uint32_t        tcpi_rcv_ooopack;       ///< Out-of-order packets.
    uint32_t        tcpi_snd_zerowin;       ///< Zero-sized windows sent.

    /* Padding to grow without breaking ABI. */
    uint32_t        _tcpi_pad[26];
};

/**
    Structures returned by network data base library. All addresses are
    supplied in host order, and returned in network order.
*/
struct HostEnt
{
    char*   h_name;         ///< Official name	of the host.
    char**  h_aliases;      ///< A NULL-terminated array of alternate names for the host.
    Family  h_addrtype;     ///< The type of address being returned; usually @ref Family::Af_Inet.
    int     h_length;       ///< The length, in bytes, of the address.
    char**  h_addr_list;    ///< A NULL-terminated array of network addresses for the host.
                            ///< Host addresses are returned in network byte order.
#define h_addr h_addr_list[0]
};

/**
    Structure used by @ref nn::socket::Select() and for setting timeout options with
    @ref nn::socket::SetSockOpt().
*/
struct TimeVal
{
    long  tv_sec;     ///< Seconds.
    long  tv_usec;    ///< Microseconds.
};

/**
    Structure used for @ref Poll().
*/
struct PollFd
{
    /**
        File descriptor to poll. If fd is equal to -1 then revents
        is cleared (set to PollEvent::PollNone), and that PollFd is not checked.
    */
    int   fd;

    /** Events to poll for. */
    PollEvent events;

    /** Events which may occur. */
    PollEvent revents;
};

/**
    Structure used for @ref Select().
*/
struct FdSet
{
    FdMask fds_bits[FdSetSize / 8 / sizeof(long)];  ///< fds_bits.
};

/**
    Argument structure for Option::Ip_Add_Membership and Option::Ip_Drop_Membership.
*/
struct IpMreq
{
    InAddr imr_multiaddr;       ///< IP multicast address of group
    InAddr imr_interface;       ///< local IP address of interface
};

inline Type operator | (Type a, Type b) NN_NOEXCEPT
{
    return static_cast<Type>(static_cast<int>(a) | static_cast<int>(b));
}

inline Type operator |= (Type &a, Type b) NN_NOEXCEPT
{
    return a = a | b;
}

inline Type operator & (Type a, Type b) NN_NOEXCEPT
{
    return static_cast<Type>(static_cast<int>(a) & static_cast<int>(b));
}

inline Type operator &= (Type &a, Type b) NN_NOEXCEPT
{
    return a = a & b;
}

inline Type operator ^ (Type a, Type b) NN_NOEXCEPT
{
    return static_cast<Type>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline Type operator ^= (Type &a, Type b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline Type operator ~ (Type a) NN_NOEXCEPT
{
    return static_cast<Type>(~static_cast<int>(a));
}

inline MsgFlag operator | (MsgFlag a, MsgFlag b) NN_NOEXCEPT
{
    return static_cast<MsgFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline MsgFlag operator |= (MsgFlag &a, MsgFlag b) NN_NOEXCEPT
{
    return a = a | b;
}

inline MsgFlag operator & (MsgFlag a, MsgFlag b) NN_NOEXCEPT
{
    return static_cast<MsgFlag>(static_cast<int>(a) & static_cast<int>(b));
}

inline MsgFlag operator &= (MsgFlag &a, MsgFlag b) NN_NOEXCEPT
{
    return a = a & b;
}

inline MsgFlag operator ^ (MsgFlag a, MsgFlag b) NN_NOEXCEPT
{
    return static_cast<MsgFlag>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline MsgFlag operator ^= (MsgFlag &a, MsgFlag b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline MsgFlag operator ~ (MsgFlag a) NN_NOEXCEPT
{
    return static_cast<MsgFlag>(~static_cast<int>(a));
}

inline bool operator ! (MsgFlag a) NN_NOEXCEPT
{
    return (MsgFlag::Msg_None == a);
}

inline PollEvent operator | (PollEvent a, PollEvent b) NN_NOEXCEPT
{
    return static_cast<PollEvent>(static_cast<int>(a) | static_cast<int>(b));
}

inline PollEvent operator |= (PollEvent &a, PollEvent b) NN_NOEXCEPT
{
    return a = a | b;
}

inline PollEvent operator & (PollEvent a, PollEvent b) NN_NOEXCEPT
{
    return static_cast<PollEvent>(static_cast<int>(a) & static_cast<int>(b));
}

inline PollEvent operator &= (PollEvent &a, PollEvent b) NN_NOEXCEPT
{
    return a = a & b;
}

inline PollEvent operator ^ (PollEvent a, PollEvent b) NN_NOEXCEPT
{
    return static_cast<PollEvent>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline PollEvent operator ^= (PollEvent &a, PollEvent b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline PollEvent operator ~ (PollEvent a) NN_NOEXCEPT
{
    return static_cast<PollEvent>(~static_cast<int>(a));
}

inline bool operator ! (PollEvent a) NN_NOEXCEPT
{
    return (PollEvent::PollNone == a);
}

inline AddrInfoFlag operator | (AddrInfoFlag a, AddrInfoFlag b) NN_NOEXCEPT
{
    return static_cast<AddrInfoFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline AddrInfoFlag operator |= (AddrInfoFlag &a, AddrInfoFlag b) NN_NOEXCEPT
{
    return a = a | b;
}

inline AddrInfoFlag operator & (AddrInfoFlag a, AddrInfoFlag b) NN_NOEXCEPT
{
    return static_cast<AddrInfoFlag>(static_cast<int>(a) & static_cast<int>(b));
}

inline AddrInfoFlag operator &= (AddrInfoFlag &a, AddrInfoFlag b) NN_NOEXCEPT
{
    return a = a & b;
}

inline AddrInfoFlag operator ^ (AddrInfoFlag a, AddrInfoFlag b) NN_NOEXCEPT
{
    return static_cast<AddrInfoFlag>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline AddrInfoFlag operator ^= (AddrInfoFlag &a, AddrInfoFlag b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline AddrInfoFlag operator ~ (AddrInfoFlag a) NN_NOEXCEPT
{
    return static_cast<AddrInfoFlag>(~static_cast<int>(a));
}

inline bool operator ! (AddrInfoFlag a) NN_NOEXCEPT
{
    return (AddrInfoFlag::Ai_None == a);
}

inline NameInfoFlag operator | (NameInfoFlag a, NameInfoFlag b) NN_NOEXCEPT
{
    return static_cast<NameInfoFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline NameInfoFlag operator |= (NameInfoFlag &a, NameInfoFlag b) NN_NOEXCEPT
{
    return a = a | b;
}

inline NameInfoFlag operator & (NameInfoFlag a, NameInfoFlag b) NN_NOEXCEPT
{
    return static_cast<NameInfoFlag>(static_cast<int>(a) & static_cast<int>(b));
}

inline NameInfoFlag operator &= (NameInfoFlag &a, NameInfoFlag b) NN_NOEXCEPT
{
    return a = a & b;
}

inline NameInfoFlag operator ^ (NameInfoFlag a, NameInfoFlag b) NN_NOEXCEPT
{
    return static_cast<NameInfoFlag>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline NameInfoFlag operator ^= (NameInfoFlag &a, NameInfoFlag b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline NameInfoFlag operator ~ (NameInfoFlag a) NN_NOEXCEPT
{
    return static_cast<NameInfoFlag>(~static_cast<int>(a));
}

inline bool operator ! (NameInfoFlag a) NN_NOEXCEPT
{
    return (NameInfoFlag::Ni_None == a);
}

inline FcntlFlag operator | (FcntlFlag a, FcntlFlag b) NN_NOEXCEPT
{
    return static_cast<FcntlFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline FcntlFlag operator |= (FcntlFlag &a, FcntlFlag b) NN_NOEXCEPT
{
    return a = a | b;
}

inline FcntlFlag operator & (FcntlFlag a, FcntlFlag b) NN_NOEXCEPT
{
    return static_cast<FcntlFlag>(static_cast<int>(a) & static_cast<int>(b));
}

inline FcntlFlag operator &= (FcntlFlag &a, FcntlFlag b) NN_NOEXCEPT
{
    return a = a & b;
}

inline FcntlFlag operator ^ (FcntlFlag a, FcntlFlag b) NN_NOEXCEPT
{
    return static_cast<FcntlFlag>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline FcntlFlag operator ^= (FcntlFlag &a, FcntlFlag b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline FcntlFlag operator ~ (FcntlFlag a) NN_NOEXCEPT
{
    return static_cast<FcntlFlag>(~static_cast<int>(a));
}

inline bool operator ! (FcntlFlag a) NN_NOEXCEPT
{
    return (FcntlFlag::None == a);
}

}} /* nn::socket */
