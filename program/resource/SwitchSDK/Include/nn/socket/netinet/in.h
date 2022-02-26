/*-
 * Copyright (c) 1982, 1986, 1990, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *    @(#)in.h    8.3 (Berkeley) 1/3/94
 * $FreeBSD$
 */

#pragma once

#include <nn/socket/sys/socket.h>

/* Protocols common to RFC 1700, POSIX, and X/Open. */
enum
{
    IPPROTO_IP              = 0,         /* dummy for IP */
    IPPROTO_ICMP            = 1,         /* control message protocol */
    IPPROTO_TCP             = 6,         /* tcp */
    IPPROTO_UDP             = 17,        /* user datagram protocol */
};

enum
{
    INADDR_ANY              = (uint32_t)0x00000000,
    INADDR_BROADCAST        = (uint32_t)0xffffffff    /* must be masked */
};

typedef uint32_t            in_addr_t;
typedef uint16_t            in_port_t;

/* Internet address (a structure for historical reasons). */
struct in_addr
{
    in_addr_t s_addr;
};

/* Socket address, internet style. */
struct sockaddr_in
{
    uint8_t     sin_len;
    sa_family_t sin_family;
    in_port_t   sin_port;
    struct      in_addr sin_addr;
    char        sin_zero[8];
};

#ifdef  _KERNEL
#ifndef _BYTEORDER_PROTOTYPED
#define _BYTEORDER_PROTOTYPED
__BEGIN_DECLS
uint32_t    htonl(uint32_t);
uint16_t    htons(uint16_t);
uint32_t    ntohl(uint32_t);
uint16_t    ntohs(uint16_t);
__END_DECLS
#endif

#ifndef _BYTEORDER_FUNC_DEFINED
#define _BYTEORDER_FUNC_DEFINED
#if BYTE_ORDER == BIG_ENDIAN
enum
{
    _QUAD_HIGHWORD = 0,
    _QUAD_LOWWORD  = 1
};
#define __ntohl(x)          ((uint32_t)(x))
#define __ntohs(x)          ((uint16_t)(x))
#define __htonl(x)          ((uint32_t)(x))
#define __htons(x)          ((uint16_t)(x))
#else
enum
{
    _QUAD_HIGHWORD = 1,
    _QUAD_LOWWORD  = 0
};
#define __ntohl(x)          (__bswap32(x))
#define __ntohs(x)          (__bswap16(x))
#define __htonl(x)          (__bswap32(x))
#define __htons(x)          (__bswap16(x))
#endif

#define __bswap64_var(x)    __builtin_bswap64(x)
#define __bswap32_var(x)    __builtin_bswap32(x)
#define __bswap16_var(x)    __builtin_bswap16(x)

#define __bswap32_constant(x)                   \
    ((((x) & 0xff000000U) >> 24) |              \
     (((x) & 0x00ff0000U) >>  8) |              \
     (((x) & 0x0000ff00U) <<  8) |              \
     (((x) & 0x000000ffU) << 24))

#define __bswap16_constant(x)                   \
    ((((x) & 0xff00) >> 8) |                    \
     (((x) & 0x00ff) << 8))

#define __bswap16(x)                            \
    ((uint16_t)(__builtin_constant_p(x) ?       \
     __bswap16_constant(x) :                    \
     __bswap16_var(x)))

#define __bswap32(x)                            \
    ((uint32_t)(__builtin_constant_p(x) ?       \
     __bswap32_constant(x) :                    \
     __bswap32_var(x)))

#define htonl(x)    __htonl(x)
#define htons(x)    __htons(x)
#define ntohl(x)    __ntohl(x)
#define ntohs(x)    __ntohs(x)
#endif
#endif

enum
{
    INET_ADDRSTRLEN = 16
};

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981, and numerous additions.
 */

/*
 * Protocols (RFC 1700)
 */
enum
{
    IPPROTO_IPV6            = 41,          /* IP6 header */
    IPPROTO_RAW             = 255,         /* raw IP packet */
    IPPROTO_HOPOPTS         = 0,           /* IP6 hop-by-hop options */
    IPPROTO_IGMP            = 2,           /* group mgmt protocol */
    IPPROTO_GGP             = 3,           /* gateway^2 (deprecated) */
    IPPROTO_IPV4            = 4,           /* IPv4 encapsulation */
    IPPROTO_IPIP            = IPPROTO_IPV4,    /* for compatibility */
    IPPROTO_ST              = 7,           /* Stream protocol II */
    IPPROTO_EGP             = 8,           /* exterior gateway protocol */
    IPPROTO_PIGP            = 9,           /* private interior gateway */
    IPPROTO_RCCMON          = 10,          /* BBN RCC Monitoring */
    IPPROTO_NVPII           = 11,          /* network voice protocol*/
    IPPROTO_PUP             = 12,          /* pup */
    IPPROTO_ARGUS           = 13,          /* Argus */
    IPPROTO_EMCON           = 14,          /* EMCON */
    IPPROTO_XNET            = 15,          /* Cross Net Debugger */
    IPPROTO_CHAOS           = 16,          /* Chaos*/
    IPPROTO_MUX             = 18,          /* Multiplexing */
    IPPROTO_MEAS            = 19,          /* DCN Measurement Subsystems */
    IPPROTO_HMP             = 20,          /* Host Monitoring */
    IPPROTO_PRM             = 21,          /* Packet Radio Measurement */
    IPPROTO_IDP             = 22,          /* xns idp */
    IPPROTO_TRUNK1          = 23,          /* Trunk-1 */
    IPPROTO_TRUNK2          = 24,          /* Trunk-2 */
    IPPROTO_LEAF1           = 25,          /* Leaf-1 */
    IPPROTO_LEAF2           = 26,          /* Leaf-2 */
    IPPROTO_RDP             = 27,          /* Reliable Data */
    IPPROTO_IRTP            = 28,          /* Reliable Transaction */
    IPPROTO_TP              = 29,          /* tp-4 w/ class negotiation */
    IPPROTO_BLT             = 30,          /* Bulk Data Transfer */
    IPPROTO_NSP             = 31,          /* Network Services */
    IPPROTO_INP             = 32,          /* Merit Internodal */
    IPPROTO_SEP             = 33,          /* Sequential Exchange */
    IPPROTO_3PC             = 34,          /* Third Party Connect */
    IPPROTO_IDPR            = 35,          /* InterDomain Policy Routing */
    IPPROTO_XTP             = 36,          /* XTP */
    IPPROTO_DDP             = 37,          /* Datagram Delivery */
    IPPROTO_CMTP            = 38,          /* Control Message Transport */
    IPPROTO_TPXX            = 39,          /* TP++ Transport */
    IPPROTO_IL              = 40,          /* IL transport protocol */
    IPPROTO_SDRP            = 42,          /* Source Demand Routing */
    IPPROTO_ROUTING         = 43,          /* IP6 routing header */
    IPPROTO_FRAGMENT        = 44,          /* IP6 fragmentation header */
    IPPROTO_IDRP            = 45,          /* InterDomain Routing*/
    IPPROTO_RSVP            = 46,          /* resource reservation */
    IPPROTO_GRE             = 47,          /* General Routing Encap. */
    IPPROTO_MHRP            = 48,          /* Mobile Host Routing */
    IPPROTO_BHA             = 49,          /* BHA */
    IPPROTO_ESP             = 50,          /* IP6 Encap Sec. Payload */
    IPPROTO_AH              = 51,          /* IP6 Auth Header */
    IPPROTO_INLSP           = 52,          /* Integ. Net Layer Security */
    IPPROTO_SWIPE           = 53,          /* IP with encryption */
    IPPROTO_NHRP            = 54,          /* Next Hop Resolution */
    IPPROTO_MOBILE          = 55,          /* IP Mobility */
    IPPROTO_TLSP            = 56,          /* Transport Layer Security */
    IPPROTO_SKIP            = 57,          /* SKIP */
    IPPROTO_ICMPV6          = 58,          /* ICMP6 */
    IPPROTO_NONE            = 59,          /* IP6 no next header */
    IPPROTO_DSTOPTS         = 60,          /* IP6 destination option */
    IPPROTO_AHIP            = 61,          /* any host internal protocol */
    IPPROTO_CFTP            = 62,          /* CFTP */
    IPPROTO_HELLO           = 63,          /* "hello" routing protocol */
    IPPROTO_SATEXPAK        = 64,          /* SATNET/Backroom EXPAK */
    IPPROTO_KRYPTOLAN       = 65,          /* Kryptolan */
    IPPROTO_RVD             = 66,          /* Remote Virtual Disk */
    IPPROTO_IPPC            = 67,          /* Pluribus Packet Core */
    IPPROTO_ADFS            = 68,          /* Any distributed FS */
    IPPROTO_SATMON          = 69,          /* Satnet Monitoring */
    IPPROTO_VISA            = 70,          /* VISA Protocol */
    IPPROTO_IPCV            = 71,          /* Packet Core Utility */
    IPPROTO_CPNX            = 72,          /* Comp. Prot. Net. Executive */
    IPPROTO_CPHB            = 73,          /* Comp. Prot. HeartBeat */
    IPPROTO_WSN             = 74,          /* Wang Span Network */
    IPPROTO_PVP             = 75,          /* Packet Video Protocol */
    IPPROTO_BRSATMON        = 76,          /* BackRoom SATNET Monitoring */
    IPPROTO_ND              = 77,          /* Sun net disk proto (temp.) */
    IPPROTO_WBMON           = 78,          /* WIDEBAND Monitoring */
    IPPROTO_WBEXPAK         = 79,          /* WIDEBAND EXPAK */
    IPPROTO_EON             = 80,          /* ISO cnlp */
    IPPROTO_VMTP            = 81,          /* VMTP */
    IPPROTO_SVMTP           = 82,          /* Secure VMTP */
    IPPROTO_VINES           = 83,          /* Banyon VINES */
    IPPROTO_TTP             = 84,          /* TTP */
    IPPROTO_IGP             = 85,          /* NSFNET-IGP */
    IPPROTO_DGP             = 86,          /* dissimilar gateway prot. */
    IPPROTO_TCF             = 87,          /* TCF */
    IPPROTO_IGRP            = 88,          /* Cisco/GXS IGRP */
    IPPROTO_OSPFIGP         = 89,          /* OSPFIGP */
    IPPROTO_SRPC            = 90,          /* Strite RPC protocol */
    IPPROTO_LARP            = 91,          /* Locus Address Resoloution */
    IPPROTO_MTP             = 92,          /* Multicast Transport */
    IPPROTO_AX25            = 93,          /* AX.25 Frames */
    IPPROTO_IPEIP           = 94,          /* IP encapsulated in IP */
    IPPROTO_MICP            = 95,          /* Mobile Int.ing control */
    IPPROTO_SCCSP           = 96,          /* Semaphore Comm. security */
    IPPROTO_ETHERIP         = 97,          /* Ethernet IP encapsulation */
    IPPROTO_ENCAP           = 98,          /* encapsulation header */
    IPPROTO_APES            = 99,          /* any private encr. scheme */
    IPPROTO_GMTP            = 100,         /* GMTP*/
    IPPROTO_IPCOMP          = 108,         /* payload compression (IPComp) */
    IPPROTO_SCTP            = 132,         /* SCTP */
    IPPROTO_MH              = 135,         /* IPv6 Mobility Header */
    IPPROTO_UDPLITE         = 136,         /* UDP-Lite */
    IPPROTO_HIP             = 139,         /* IP6 Host Identity Protocol */
    IPPROTO_SHIM6           = 140,         /* IP6 Shim6 Protocol */
/* 101-254: Partly Unassigned */
    IPPROTO_PIM             = 103,         /* Protocol Independent Mcast */
    IPPROTO_CARP            = 112,         /* CARP */
    IPPROTO_PGM             = 113,         /* PGM */
    IPPROTO_MPLS            = 137,         /* MPLS-in-IP */
    IPPROTO_PFSYNC          = 240,         /* PFSYNC */
    IPPROTO_RESERVED_253    = 253,         /* Reserved */
    IPPROTO_RESERVED_254    = 254,         /* Reserved */
/* 255: Reserved */
/* BSD Private, local use, namespace incursion, no longer used */
    IPPROTO_OLD_DIVERT      = 254,         /* OLD divert pseudo-proto */
    IPPROTO_MAX             = 256,

/* last return value of *_input(), meaning "all job for this pkt is done".  */
    IPPROTO_DONE            = 257,

/* Only used internally, so can be outside the range of valid IP protocols. */
    IPPROTO_DIVERT          = 258,         /* divert pseudo-protocol */
    IPPROTO_SEND            = 259,         /* SeND pseudo-protocol */

/*
 * Defined to avoid confusion.  The master value is defined by
 * PROTO_SPACER in sys/protosw.h.
 */
    IPPROTO_SPACER          = 32767,       /* spacer for loadable protos */
};

/*
 * Local port number conventions:
 *
 * When a user does a bind(2) or connect(2) with a port number of zero,
 * a non-conflicting local port address is chosen.
 * The default range is IPPORT_HIFIRSTAUTO through
 * IPPORT_HILASTAUTO, although that is settable by sysctl.
 *
 * A user may set the IPPROTO_IP option IP_PORTRANGE to change this
 * default assignment range.
 *
 * The value IP_PORTRANGE_DEFAULT causes the default behavior.
 *
 * The value IP_PORTRANGE_HIGH changes the range of candidate port numbers
 * into the "high" range.  These are reserved for client outbound connections
 * which do not want to be filtered by any firewalls.
 *
 * The value IP_PORTRANGE_LOW changes the range to the "low" are
 * that is (by convention) restricted to privileged processes.  This
 * convention is based on "vouchsafe" principles only.  It is only secure
 * if you trust the remote host to restrict these ports.
 *
 * The default range of ports and the high range can be changed by
 * sysctl(3).  (net.inet.ip.port{hi,low}{first,last}_auto)
 *
 * Changing those values has bad security implications if you are
 * using a stateless firewall that is allowing packets outside of that
 * range in order to allow transparent outgoing connections.
 *
 * Such a firewall configuration will generally depend on the use of these
 * default values.  If you change them, you may find your Security
 * Administrator looking for you with a heavy object.
 *
 * For a slightly more orthodox text view on this:
 *
 *            ftp://ftp.isi.edu/in-notes/iana/assignments/port-numbers
 *
 *    port numbers are divided into three ranges:
 *
 *                0 -  1023 Well Known Ports
 *             1024 - 49151 Registered Ports
 *            49152 - 65535 Dynamic and/or Private Ports
 *
 */

/*
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).         (IP_PORTRANGE_LOW)
 */
enum
{
    IPPORT_RESERVED         = 1024,

/*
 * Default local port range, used by IP_PORTRANGE_DEFAULT
 */
    IPPORT_EPHEMERALFIRST   = 10000,
    IPPORT_EPHEMERALLAST    = 65535,

/*
 * Dynamic port range, used by IP_PORTRANGE_HIGH.
 */
    IPPORT_HIFIRSTAUTO      = 49152,
    IPPORT_HILASTAUTO       = 65535,

/*
 * Scanning for a free reserved port return a value below IPPORT_RESERVED,
 * but higher than IPPORT_RESERVEDSTART.  Traditionally the start value was
 * 512, but that conflicts with some well-known-services that firewalls may
 * have a fit if we use.
 */
    IPPORT_RESERVEDSTART    = 600,

    IPPORT_MAX              = 65535,
};

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define IN_CLASSA(i) (((uint32_t)(i) & 0x80000000) == 0)
enum
{
    IN_CLASSA_NET         =  0xff000000,
    IN_CLASSA_NSHIFT      =  24,
    IN_CLASSA_HOST        =  0x00ffffff,
    IN_CLASSA_MAX         =  128
};

#define IN_CLASSB(i) (((uint32_t)(i) & 0xc0000000) == 0x80000000)
enum
{
    IN_CLASSB_NET         =  0xffff0000,
    IN_CLASSB_NSHIFT      =  16,
    IN_CLASSB_HOST        =  0x0000ffff,
    IN_CLASSB_MAX         =  65536
};

#define IN_CLASSC(i) (((uint32_t)(i) & 0xe0000000) == 0xc0000000)
enum
{
    IN_CLASSC_NET         =  0xffffff00,
    IN_CLASSC_NSHIFT      =  8,
    IN_CLASSC_HOST        =  0x000000ff
};

#define IN_CLASSD(i) (((uint32_t)(i) & 0xf0000000) == 0xe0000000)
enum
{
    IN_CLASSD_NET         =  0xf0000000,  /* These ones aren't really */
    IN_CLASSD_NSHIFT      =  28,          /* net and host fields, but */
    IN_CLASSD_HOST        =  0x0fffffff   /* routing needn't know.    */
};
#define IN_MULTICAST(i)         IN_CLASSD(i)

#define IN_EXPERIMENTAL(i)      (((uint32_t)(i) & 0xf0000000) == 0xf0000000)
#define IN_BADCLASS(i)          (((uint32_t)(i) & 0xf0000000) == 0xf0000000)

#define IN_LINKLOCAL(i)         (((uint32_t)(i) & 0xffff0000) == 0xa9fe0000)
#define IN_LOOPBACK(i)          (((uint32_t)(i) & 0xff000000) == 0x7f000000)
#define IN_ZERONET(i)           (((uint32_t)(i) & 0xff000000) == 0)

#define IN_PRIVATE(i)           ((((uint32_t)(i) & 0xff000000) == 0x0a000000) || \
                                (((uint32_t)(i) & 0xfff00000) == 0xac100000) || \
                                (((uint32_t)(i) & 0xffff0000) == 0xc0a80000))

#define IN_LOCAL_GROUP(i)       (((uint32_t)(i) & 0xffffff00) == 0xe0000000)

#define IN_ANY_LOCAL(i)         (IN_LINKLOCAL(i) || IN_LOCAL_GROUP(i))

enum
{
    INADDR_LOOPBACK         = (uint32_t)0x7f000001,
    INADDR_NONE             = 0xffffffff,                  /* -1 return */
    INADDR_UNSPEC_GROUP     = (uint32_t)0xe0000000,        /* 224.0.0.0 */
    INADDR_ALLHOSTS_GROUP   = (uint32_t)0xe0000001,        /* 224.0.0.1 */
    INADDR_ALLRTRS_GROUP    = (uint32_t)0xe0000002,        /* 224.0.0.2 */
    INADDR_ALLRPTS_GROUP    = (uint32_t)0xe0000016,        /* 224.0.0.22, IGMPv3 */
    INADDR_CARP_GROUP       = (uint32_t)0xe0000012,        /* 224.0.0.18 */
    INADDR_PFSYNC_GROUP     = (uint32_t)0xe00000f0,        /* 224.0.0.240 */
    INADDR_ALLMDNS_GROUP    = (uint32_t)0xe00000fb,        /* 224.0.0.251 */
    INADDR_MAX_LOCAL_GROUP  = (uint32_t)0xe00000ff,        /* 224.0.0.255 */
};

enum
{
    IN_LOOPBACKNET  = 127,                                 /* official! */
    IN_RFC3021_MASK = (uint32_t)0xfffffffe
};


/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
enum
{
    IP_OPTIONS                =  1,     /* buf/ip_opts; set/get IP options */
    IP_HDRINCL                =  2,     /* int; header is included with data */
    IP_TOS                    =  3,     /* int; IP type of service and preced. */
    IP_TTL                    =  4,     /* int; IP time to live */
    IP_RECVOPTS               =  5,     /* bool; receive all IP opts w/dgram */
    IP_RECVRETOPTS            =  6,     /* bool; receive IP opts for response */
    IP_RECVDSTADDR            =  7,     /* bool; receive IP dst addr w/dgram */
    IP_SENDSRCADDR            =  IP_RECVDSTADDR, /* cmsg_type to set src addr */
    IP_RETOPTS                =  8,     /* ip_opts; set/get IP options */
    IP_MULTICAST_IF           =  9,     /* struct in_addr *or* struct ip_mreqn;
                                         * set / get IP multicast i / f  */
    IP_MULTICAST_TTL          =  10,    /* u_char; set/get IP multicast ttl */
    IP_MULTICAST_LOOP         =  11,    /* u_char; set/get IP multicast loopback */
    IP_ADD_MEMBERSHIP         =  12,    /* ip_mreq; add an IP group membership */
    IP_DROP_MEMBERSHIP        =  13,    /* ip_mreq; drop an IP group membership */
    IP_MULTICAST_VIF          =  14,    /* set/get IP mcast virt. iface */
    IP_RSVP_ON                =  15,    /* enable RSVP in kernel */
    IP_RSVP_OFF               =  16,    /* disable RSVP in kernel */
    IP_RSVP_VIF_ON            =  17,    /* set RSVP per-vif socket */
    IP_RSVP_VIF_OFF           =  18,    /* unset RSVP per-vif socket */
    IP_PORTRANGE              =  19,    /* int; range to choose for unspec port */
    IP_RECVIF                 =  20,    /* bool; receive reception if w/dgram */
/* for IPSEC */
    IP_IPSEC_POLICY           =  21,    /* int; set/get security policy */
    IP_FAITH                  =  22,    /* bool; accept FAITH'ed connections */

    IP_ONESBCAST              =  23,    /* bool: send all-ones broadcast */
    IP_BINDANY                =  24,    /* bool: allow bind to any address */

/*
 * Options for controlling the firewall and dummynet.
 * Historical options (from 40 to 64) will eventually be
 * replaced by only two options, IP_FW3 and IP_DUMMYNET3.
 */
    IP_FW_TABLE_ADD           =  40,    /* add entry */
    IP_FW_TABLE_DEL           =  41,    /* delete entry */
    IP_FW_TABLE_FLUSH         =  42,    /* flush table */
    IP_FW_TABLE_GETSIZE       =  43,    /* get table size */
    IP_FW_TABLE_LIST          =  44,    /* list table contents */

    IP_FW3                    =  48,    /* generic ipfw v.3 sockopts */
    IP_DUMMYNET3              =  49,    /* generic dummynet v.3 sockopts */

    IP_FW_ADD                 =  50,    /* add a firewall rule to chain */
    IP_FW_DEL                 =  51,    /* delete a firewall rule from chain */
    IP_FW_FLUSH               =  52,    /* flush firewall rule chain */
    IP_FW_ZERO                =  53,    /* clear single/all firewall counter(s) */
    IP_FW_GET                 =  54,    /* get entire firewall rule chain */
    IP_FW_RESETLOG            =  55,    /* reset logging counters */

    IP_FW_NAT_CFG             =  56,    /* add/config a nat rule */
    IP_FW_NAT_DEL             =  57,    /* delete a nat rule */
    IP_FW_NAT_GET_CONFIG      =  58,    /* get configuration of a nat rule */
    IP_FW_NAT_GET_LOG         =  59,    /* get log of a nat rule */

    IP_DUMMYNET_CONFIGURE     =  60,    /* add/configure a dummynet pipe */
    IP_DUMMYNET_DEL           =  61,    /* delete a dummynet pipe from chain */
    IP_DUMMYNET_FLUSH         =  62,    /* flush dummynet */
    IP_DUMMYNET_GET           =  64,    /* get entire dummynet pipes */

    IP_RECVTTL                =  65,    /* bool; receive IP TTL w/dgram */
    IP_MINTTL                 =  66,    /* minimum TTL for packet or drop */
    IP_DONTFRAG               =  67,    /* don't fragment packet */
    IP_RECVTOS                =  68,    /* bool; receive IP TOS w/dgram */

/* IPv4 Source Filter Multicast API [RFC3678] */
    IP_ADD_SOURCE_MEMBERSHIP  =  70,    /* join a source-specific group */
    IP_DROP_SOURCE_MEMBERSHIP =  71,    /* drop a single source */
    IP_BLOCK_SOURCE           =  72,    /* block a source */
    IP_UNBLOCK_SOURCE         =  73,    /* unblock a source */

/* The following option is private; do not use it from user applications. */
    IP_MSFILTER               =  74,    /* set/get filter list */
};

/* Protocol Independent Multicast API [RFC3678] */
enum
{
    MCAST_JOIN_GROUP          =  80,    /* join an any-source group */
    MCAST_LEAVE_GROUP         =  81,    /* leave all sources for group */
    MCAST_JOIN_SOURCE_GROUP   =  82,    /* join a source-specific group */
    MCAST_LEAVE_SOURCE_GROUP  =  83,    /* leave a single source */
    MCAST_BLOCK_SOURCE        =  84,    /* block a source */
    MCAST_UNBLOCK_SOURCE      =  85,    /* unblock a source */
};

/*
 * Defaults and limits for options
 */
enum
{
    IP_DEFAULT_MULTICAST_TTL   = 1,     /* normally limit m'casts to 1 hop  */
    IP_DEFAULT_MULTICAST_LOOP  = 1,     /* normally hear sends if a member  */

/*
 * The imo_membership vector for each socket is now dynamically allocated at
 * run-time, bounded by USHRT_MAX, and is reallocated when needed, sized
 * according to a power-of-two increment.
 */
    IP_MIN_MEMBERSHIPS         = 31,
    IP_MAX_MEMBERSHIPS         = 4095,
    IP_MAX_SOURCE_FILTER       = 1024,  /* XXX to be unused */

/*
 * Default resource limits for IPv4 multicast source filtering.
 * These may be modified by sysctl.
 */
    IP_MAX_GROUP_SRC_FILTER    = 512,   /* sources per group */
    IP_MAX_SOCK_SRC_FILTER     = 128,   /* sources per socket/group */
    IP_MAX_SOCK_MUTE_FILTER    = 128,   /* XXX no longer used */
};

/*
 * Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP.
 */
struct ip_mreq
{
    struct in_addr imr_multiaddr;       /* IP multicast address of group */
    struct in_addr imr_interface;       /* local IP address of interface */
};

/*
 * Modified argument structure for IP_MULTICAST_IF, obtained from Linux.
 * This is used to specify an interface index for multicast sends, as
 * the IPv4 legacy APIs do not support this (unless IP_SENDIF is available).
 */
struct ip_mreqn
{
    struct in_addr imr_multiaddr;       /* IP multicast address of group */
    struct in_addr imr_address;         /* local IP address of interface */
    int    imr_ifindex;                 /* Interface index; cast to uint32_t */
};

/*
 * Argument structure for IPv4 Multicast Source Filter APIs. [RFC3678]
 */
struct ip_mreq_source
{
    struct in_addr imr_multiaddr;       /* IP multicast address of group */
    struct in_addr imr_sourceaddr;      /* IP address of source */
    struct in_addr imr_interface;       /* local IP address of interface */
};

/*
 * Argument structures for Protocol-Independent Multicast Source
 * Filter APIs. [RFC3678]
 */
struct group_req
{
    uint32_t                   gr_interface;    /* interface index */
    struct sockaddr_storage    gr_group;        /* group address */
};

struct group_source_req
{
    uint32_t                   gsr_interface;   /* interface index */
    struct sockaddr_storage    gsr_group;       /* group address */
    struct sockaddr_storage    gsr_source;      /* source address */
};

/*
 * Argument for IP_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
enum
{
    IP_PORTRANGE_DEFAULT   = 0,         /* default range */
    IP_PORTRANGE_HIGH      = 1,         /* "high" - request firewall bypass */
    IP_PORTRANGE_LOW       = 2,         /* "low" - vouchsafe security */
};

#include <nn/socket/netinet6/in6.h>
