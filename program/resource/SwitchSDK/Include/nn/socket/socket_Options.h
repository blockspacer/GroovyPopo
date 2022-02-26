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

#include <nn/nn_Macro.h>  // for NN_NOEXCEPT

namespace nn {
namespace socket {

/**
    Values used for SetSockOpt() and GetSockOpt() @a level argument.
*/
enum class Level : int32_t
{
    /*
        Level number for SetSockOpt() and GetSockOpt() to apply to socket itself.
    */

    Sol_Socket  = 0xffff, ///< Socket level.

    Sol_Ip      =   0,    ///< IP level.
    Sol_Icmp    =   1,    ///< ICMP level.
    Sol_Tcp     =   6,    ///< TCP level.
    Sol_Udp     =  17,    ///< UDP level.
    Sol_UdpLite = 136     ///< UDP-Lite level.
};

/**
    Values used for SetSockOpt() and GetSockOpt() @a optionName argument.
*/
enum class Option : uint32_t
{
    /*
        Option flags per-socket.
    */
    So_Debug        = 0x0001,   ///< Enables recording of debugging information.
    So_AcceptConn   = 0x0002,   ///< Get listening status of the socket (get only).
    So_ReuseAddr    = 0x0004,   ///< Enables local address reuse.
    So_KeepAlive    = 0x0008,   ///< Keep connections alive.
    So_DontRoute    = 0x0010,   ///< Enables routing bypass for outgoing messages.
    So_Broadcast    = 0x0020,   ///< Enables permission to transmit broadcast messages.
    So_UseLoopback  = 0x0040,   ///< Bypass hardware when possible.
    So_Linger       = 0x0080,   ///< Linger on close if data present.
    So_OobInline    = 0x0100,   ///< Enables reception of out-of-band data in band.
    So_ReusePort    = 0x0200,   ///< Enables duplicate address and port bindings.

    /*
        Additional options, not kept in so_options.
    */
    So_SndBuf        = 0x1001,  ///< Send buffer size.
    So_RcvBuf        = 0x1002,  ///< Receive buffer size.
    So_SndLoWat      = 0x1003,  ///< Send low-water mark.
    So_RcvLoWat      = 0x1004,  ///< Receive low-water mark.
    So_SndTimeo      = 0x1005,  ///< Send timeout.
    So_RcvTimeo      = 0x1006,  ///< Receive timeout.
    So_Error         = 0x1007,  ///< Get error status and clear.
    So_Type          = 0x1008,  ///< Get socket type.
    So_Label         = 0x1009,  ///< Socket's MAC label.
    So_PeerLabel     = 0x1010,  ///< Socket's peer's MAC label.
    So_ListenQLimit  = 0x1011,  ///< Socket's backlog limit.
    So_ListenQLen    = 0x1012,  ///< Socket's complete queue length.
    So_ListenIncQLen = 0x1013,  ///< Socket's incomplete queue length.
    So_SetFib        = 0x1014,  ///< Use this FIB to route.
    So_User_Cookie   = 0x1015,  ///< User cookie (dummynet etc.).
    So_Protocol      = 0x1016,  ///< Get socket protocol (Linux name).

    /*
        Space reserved for new socket options added by third-party vendors.
        This range applies to all socket option levels.  New socket options
        should always use an option value less than So_Vendor.
    */
    So_Vendor       = 0x80000000,

    // Siglo added socket options

    /**
        Upon completion of @ref Close(), this version of linger always drops the connection entirely,
        freeing associated resources.
    */
    So_Nn_Linger    = 0x80000001,

    /*
        Exempts a socket from being shutdown after a call to @ref ShutdownAllSockets
    */
    So_Nn_Shutdown_Exempt   = 0x00010000,

    /*
        Options for use with @ref GetSockOpt() and @ref SetSockOpt() at the IP level.
        First word of comment is data type; bool is stored in int.
    */
    Ip_Options                =   1,    ///< Set/get IP options.
    Ip_HdrIncl                =   2,    ///< Header is included with data.
    Ip_Tos                    =   3,    ///< IP type of service and preced..
    Ip_Ttl                    =   4,    ///< IP time to live.
    Ip_RecvOpts               =   5,    ///< Receive all IP opts w/dgram.
    Ip_Multicast_If           =   9,    ///< Set/get IP multicast i/f.
    Ip_Multicast_Ttl          =  10,    ///< Set/get IP multicast ttl.
    Ip_Multicast_Loop         =  11,    ///< Set/get IP multicast loopback.
    Ip_Add_Membership         =  12,    ///< Add an IP group membership.
    Ip_Drop_Membership        =  13,    ///< Drop an IP group membership.
    Ip_Multicast_Vif          =  14,    ///< Set/get IP mcast virt. iface.
    Ip_Rsvp_On                =  15,    ///< Enable RSVP in kernel.
    Ip_Rsvp_Off               =  16,    ///< Disable RSVP in kernel.
    Ip_Rsvp_Vif_On            =  17,    ///< Set RSVP per-vif socket.
    Ip_Rsvp_Vif_Off           =  18,    ///< Unset RSVP per-vif socket.
    Ip_PortRange              =  19,    ///< Range to choose for unspec port.
    Ip_Faith                  =  22,    ///< Accept FAITH'ed connections.
    Ip_OnesBcast              =  23,    ///< Send all-ones broadcast.
    Ip_BindAny                =  24,    ///< Allow bind to any address.

    Ip_RecvTtl                =  65,    ///< Receive IP TTL w/dgram.
    Ip_MinTtl                 =  66,    ///< Minimum TTL for packet or drop.
    Ip_DontFrag               =  67,    ///< Don't fragment packet.
    Ip_RecvTos                =  68,    ///< Receive IP TOS w/dgram.

    Ip_Add_Source_Membership  =  70,    ///< Join a source-specific group.
    Ip_Drop_Source_Membership =  71,    ///< Drop a single source.
    Ip_Block_Source           =  72,    ///< Block a source.
    Ip_Unblock_Source         =  73,    ///< Unblock a source.

    /*
         User-settable options (used with @ref SetSockOpt() and @ref GetSockOpt()). These are
         discrete values and are not masked together. Some values appear to be bitmasks
         for historical reasons.
    */
    Tcp_NoDelay               =    1,   ///< Don't delay send to coalesce packets.
    Tcp_MaxSeg                =    2,   ///< Set maximum segment size.
    Tcp_NoPush                =    4,   ///< Don't push last block of write.
    Tcp_NoOpt                 =    8,   ///< Don't use TCP options.
    Tcp_Md5Sig                =   16,   ///< Use MD5 digests (RFC2385).
    Tcp_Info                  =   32,   ///< Retrieve @ref TcpInfo structure.
    Tcp_Congestion            =   64,   ///< Get/set congestion control algorithm.
    Tcp_KeepInit              =  128,   ///< Time to establish connection.
    Tcp_KeepIdle              =  256,   ///< Start keeplives after this period.
    Tcp_KeepIntvl             =  512,   ///< Interval between keepalives.
    Tcp_KeepCnt               = 1024,   ///< Number of keepalives before close.

    /*
        Start of reserved space for third-party user-settable options.
    */
    Tcp_Vendor                = So_Vendor,

    /*
        Options for use with @ref GetSockOpt() and @ref SetSockOpt() at the UDPLite level.
    */
    UdpLite_Send_CsCov        = 2,      ///< Sender checksum coverage
    UdpLite_Recv_CsCov        = 4       ///< Receiver checksum coverage
};

/**
    Values used for Option::Ip_Tos socket option.
*/
enum class IpTos : uint8_t
{
    /*
    * Definitions for IP type of service (ip_tos).
    */
    IpTos_LowDelay              = 0x10, ///< Minimize delay.
    IpTos_Throughput            = 0x08, ///< Maximize throughput.
    IpTos_Reliability           = 0x04, ///< Maximize reliability
    IpTos_MinCost               = 0x02  ///< Minimize monetary cost
};

/**
    Values used for Option::Tcp_Info socket option.
*/
enum class TcpInfoOption : uint8_t
{
    Tcpi_Opt_None       = 0x00,         ///< No flags set.
    Tcpi_Opt_Timestamps = 0x01,         ///< Timestamps.
    Tcpi_Opt_Sack       = 0x02,         ///< Selective acknowledgements.
    Tcpi_Opt_WScale     = 0x04,         ///< Window scale.
    Tcpi_Opt_Ecn        = 0x08,         ///< Explicit congestion notification.
    Tcpi_Opt_Toe        = 0x10          ///< TCP offload engine.
};

inline TcpInfoOption operator | (TcpInfoOption a, TcpInfoOption b) NN_NOEXCEPT
{
    return static_cast<TcpInfoOption>(static_cast<int>(a) | static_cast<int>(b));
}

inline TcpInfoOption operator |= (TcpInfoOption &a, TcpInfoOption b) NN_NOEXCEPT
{
    return a = a | b;
}

inline TcpInfoOption operator & (TcpInfoOption a, TcpInfoOption b) NN_NOEXCEPT
{
    return static_cast<TcpInfoOption>(static_cast<int>(a) & static_cast<int>(b));
}

inline TcpInfoOption operator &= (TcpInfoOption &a, TcpInfoOption b) NN_NOEXCEPT
{
    return a = a & b;
}

inline TcpInfoOption operator ^ (TcpInfoOption a, TcpInfoOption b) NN_NOEXCEPT
{
    return static_cast<TcpInfoOption>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline TcpInfoOption operator ^= (TcpInfoOption &a, TcpInfoOption b) NN_NOEXCEPT
{
    return a = a ^ b;
}

inline TcpInfoOption operator ~ (TcpInfoOption a) NN_NOEXCEPT
{
    return static_cast<TcpInfoOption>(~static_cast<int>(a));
}

inline bool operator ! (TcpInfoOption a) NN_NOEXCEPT
{
    return (TcpInfoOption::Tcpi_Opt_None == a);
}

}} /* nn::socket */
