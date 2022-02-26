/*-
 * Copyright (c) 1980, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)route.h	8.4 (Berkeley) 1/9/95
 * $FreeBSD$
 */

#pragma once

enum
{
    RT_CACHING_CONTEXT      = 0x1,      /* XXX: not used anywhere */
    RT_NORTREF              = 0x2       /* doesn't hold reference on ro_rt */
};

struct rt_metrics
{
    uint32_t    rmx_locks;              /* Kernel must leave these values alone */
    uint32_t    rmx_mtu;                /* MTU for this path */
    uint32_t    rmx_hopcount;           /* max hops expected */
    uint32_t    rmx_expire;             /* lifetime for route, e.g. redirect */
    uint32_t    rmx_recvpipe;           /* inbound delay-bandwidth product */
    uint32_t    rmx_sendpipe;           /* outbound delay-bandwidth product */
    uint32_t    rmx_ssthresh;           /* outbound gateway buffer limit */
    uint32_t    rmx_rtt;                /* estimated round trip time */
    uint32_t    rmx_rttvar;             /* estimated rtt variance */
    uint32_t    rmx_pksent;             /* packets sent using this route */
    uint32_t    rmx_weight;             /* route weight */
    uint32_t    rmx_filler[3];          /* will be used for T/TCP later */
};

/*
 * rmx_rtt and rmx_rttvar are stored as microseconds;
 * RTTTOPRHZ(rtt) converts to a value suitable for use
 * by a protocol slowtimo counter.
 */
enum
{
    RTM_RTTUNIT  = 1000000,             /* units for rtt, rttvar, as units per sec */
};

#define RTTTOPRHZ(r)    ((r) / (RTM_RTTUNIT / PR_SLOWHZ))

enum
{
    RT_DEFAULT_FIB      = 0,            /* Explicitly mark fib=0 restricted cases */
    RT_ALL_FIBS         = (-1)          /* Announce event for every fib */
};

enum
{
    RTF_UP              = 0x1,          /* route usable */
    RTF_GATEWAY         = 0x2,          /* destination is a gateway */
    RTF_HOST            = 0x4,          /* host entry (net otherwise) */
    RTF_REJECT          = 0x8,          /* host or net unreachable */
    RTF_DYNAMIC         = 0x10,         /* created dynamically (by redirect) */
    RTF_MODIFIED        = 0x20,         /* modified dynamically (by redirect) */
    RTF_DONE            = 0x40,         /* message confirmed */
/*                      = 0x80,            unused, was RTF_DELCLONE */
/*                      = 0x100,           unused, was RTF_CLONING */
    RTF_XRESOLVE        = 0x200,        /* external daemon resolves name */
    RTF_LLINFO          = 0x400,        /* DEPRECATED - exists ONLY for backward
                                           compatibility */
    RTF_LLDATA          = 0x400,        /* used by apps to add/del L2 entries */
    RTF_STATIC          = 0x800,        /* manually added */
    RTF_BLACKHOLE       = 0x1000,       /* just discard pkts (during updates) */
    RTF_PROTO2          = 0x4000,       /* protocol specific routing flag */
    RTF_PROTO1          = 0x8000,       /* protocol specific routing flag */
/*                      = 0x10000,         unused, was RTF_PRCLONING */
/*                      = 0x20000,         unused, was RTF_WASCLONED */
    RTF_PROTO3          = 0x40000,      /* protocol specific routing flag */
/*                      = 0x80000,         unused */
    RTF_PINNED          = 0x100000,     /* route is immutable */
    RTF_LOCAL           = 0x200000,     /* route represents a local address */
    RTF_BROADCAST       = 0x400000,     /* route represents a bcast address */
    RTF_MULTICAST       = 0x800000,     /* route represents a mcast address */
/*                      = 0x8000000,       and up unassigned */
    RTF_STICKY          = 0x10000000,   /* always route dst->src */

    RTF_RNH_LOCKED      = 0x40000000,   /* radix node head is locked */

    RTF_GWFLAG_COMPAT   = 0x80000000,   /* a compatibility bit for interacting
                                           with existing routing apps */

/* Mask of RTF flags that are allowed to be modified by RTM_CHANGE. */
    RTF_FMASK           = (RTF_PROTO1 | RTF_PROTO2 | RTF_PROTO3 | RTF_BLACKHOLE | \
                           RTF_REJECT | RTF_STATIC | RTF_STICKY)
};

/*
 * Routing statistics.
 */
struct rtstat
{
    int16_t    rts_badredirect;         /* bogus redirect calls */
    int16_t    rts_dynamic;             /* routes created by redirects */
    int16_t    rts_newgateway;          /* routes modified by redirects */
    int16_t    rts_unreach;             /* lookups which failed */
    int16_t    rts_wildcard;            /* lookups satisfied by a wildcard */
};
/*
 * Structures for routing messages.
 */
struct rt_msghdr
{
    uint16_t    rtm_msglen;             /* to skip over non-understood messages */
    uint8_t     rtm_version;            /* future binary compatibility */
    uint8_t     rtm_type;               /* message type */
    uint16_t    rtm_index;              /* index for associated ifp */
    int         rtm_flags;              /* flags, incl. kern & message, e.g. DONE */
    int         rtm_addrs;              /* bitmask identifying sockaddrs in msg */
    pid_t       rtm_pid;                /* identify sender */
    int         rtm_seq;                /* for sender to identify action */
    int         rtm_errno;              /* why failed */
    int         rtm_fmask;              /* bitmask used in RTM_CHANGE message */
    uint32_t    rtm_inits;              /* which metrics we are initializing */
    struct      rt_metrics rtm_rmx;     /* metrics themselves */
};

enum
{
    RTM_VERSION = 5                     /* Up the ante and ignore older versions */
};

/*
 * Message types.
 */
enum
{
    RTM_ADD             = 0x1,          /* Add Route */
    RTM_DELETE          = 0x2,          /* Delete Route */
    RTM_CHANGE          = 0x3,          /* Change Metrics or flags */
    RTM_GET             = 0x4,          /* Report Metrics */
    RTM_LOSING          = 0x5,          /* Kernel Suspects Partitioning */
    RTM_REDIRECT        = 0x6,          /* Told to use different route */
    RTM_MISS            = 0x7,          /* Lookup failed on this address */
    RTM_LOCK            = 0x8,          /* fix specified metrics */
/*                      = 0x9,          */
/*                      = 0xa,          */
    RTM_RESOLVE         = 0xb,          /* req to resolve dst to LL addr */
    RTM_NEWADDR         = 0xc,          /* address being added to iface */
    RTM_DELADDR         = 0xd,          /* address being removed from iface */
    RTM_IFINFO          = 0xe,          /* iface going up/down etc. */
    RTM_NEWMADDR        = 0xf,          /* mcast group membership being added to if */
    RTM_DELMADDR        = 0x10,         /* mcast group membership being deleted */
    RTM_IFANNOUNCE      = 0x11,         /* iface arrival/departure */
    RTM_IEEE80211       = 0x12,         /* IEEE80211 wireless event */
};

/*
 * Bitmask values for rtm_inits and rmx_locks.
 */
enum
{
    RTV_MTU             = 0x1,          /* init or lock _mtu */
    RTV_HOPCOUNT        = 0x2,          /* init or lock _hopcount */
    RTV_EXPIRE          = 0x4,          /* init or lock _expire */
    RTV_RPIPE           = 0x8,          /* init or lock _recvpipe */
    RTV_SPIPE           = 0x10,         /* init or lock _sendpipe */
    RTV_SSTHRESH        = 0x20,         /* init or lock _ssthresh */
    RTV_RTT             = 0x40,         /* init or lock _rtt */
    RTV_RTTVAR          = 0x80,         /* init or lock _rttvar */
    RTV_WEIGHT          = 0x100,        /* init or lock _weight */
};

/*
 * Bitmask values for rtm_addrs.
 */
enum
{
    RTA_DST             = 0x1,          /* destination sockaddr present */
    RTA_GATEWAY         = 0x2,          /* gateway sockaddr present */
    RTA_NETMASK         = 0x4,          /* netmask sockaddr present */
    RTA_GENMASK         = 0x8,          /* cloning mask sockaddr present */
    RTA_IFP             = 0x10,         /* interface name sockaddr present */
    RTA_IFA             = 0x20,         /* interface addr sockaddr present */
    RTA_AUTHOR          = 0x40,         /* sockaddr for author of redirect */
    RTA_BRD             = 0x80,         /* for NEWADDR, broadcast or p-p dest addr */
};

/*
 * Index offsets for sockaddr array for alternate internal encoding.
 */
enum
{
    RTAX_DST            = 0,            /* destination sockaddr present */
    RTAX_GATEWAY        = 1,            /* gateway sockaddr present */
    RTAX_NETMASK        = 2,            /* netmask sockaddr present */
    RTAX_GENMASK        = 3,            /* cloning mask sockaddr present */
    RTAX_IFP            = 4,            /* interface name sockaddr present */
    RTAX_IFA            = 5,            /* interface addr sockaddr present */
    RTAX_AUTHOR         = 6,            /* sockaddr for author of redirect */
    RTAX_BRD            = 7,            /* for NEWADDR, broadcast or p-p dest addr */
    RTAX_MAX            = 8,            /* size of array to allocate */
};

struct rt_addrinfo
{
    int         rti_addrs;
    struct      sockaddr *rti_info[RTAX_MAX];
    int         rti_flags;
    struct      ifaddr *rti_ifa;
    struct      ifnet *rti_ifp;
};

/*
 * This macro returns the size of a struct sockaddr when passed
 * through a routing socket. Basically we round up sa_len to
 * a multiple of sizeof(long), with a minimum of sizeof(long).
 * The check for a NULL pointer is just a convenience, probably never used.
 * The case sa_len == 0 should only apply to empty structures.
 */
#define SA_SIZE(sa)                                                         \
    (  (!(sa) || ((struct sockaddr *)(sa))->sa_len == 0) ?                  \
    sizeof(long)        :                                                   \
    1 + ( (((struct sockaddr *)(sa))->sa_len - 1) | (sizeof(long) - 1) ) )

