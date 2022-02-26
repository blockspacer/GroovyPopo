/*-
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *    @(#)if.h    8.1 (Berkeley) 6/10/93
 * $FreeBSD$
 */

#pragma once

#include <nn/socket/sys/socket.h>

/*
 * Length of interface external name, including terminating '\0'.
 * Note: this is the same size as a generic device's external name.
 */
enum
{
    IF_NAMESIZE = 16,
    IFNAMSIZ    = IF_NAMESIZE,
    IF_MAXUNIT  =  0x7fff /* historical value */
};

/*
 * Structure used to query names of interface cloners.
 */

struct if_clonereq
{
    int         ifcr_total;                 /* total cloners (out) */
    int         ifcr_count;                 /* room for this many in user buffer */
    char        *ifcr_buffer;               /* buffer for cloner names */
};

/*
 * Structure describing information about an interface
 * which may be of interest to management entities.
 */
struct if_data
{
    /* generic interface information */
    uint8_t   ifi_type;                     /* ethernet, tokenring, etc */
    uint8_t   ifi_physical;                 /* e.g., AUI, Thinnet, 10base-T, etc */
    uint8_t   ifi_addrlen;                  /* media address length */
    uint8_t   ifi_hdrlen;                   /* media header length */
    uint8_t   ifi_link_state;               /* current link state */
    uint8_t   ifi_vhid;                     /* carp vhid */
    uint8_t   ifi_baudrate_pf;              /* baudrate power factor */
    uint8_t   ifi_datalen;                  /* length of this data struct */
    uint32_t  ifi_mtu;                      /* maximum transmission unit */
    uint32_t  ifi_metric;                   /* routing metric (external only) */
    uint32_t  ifi_baudrate;                 /* linespeed */
    /* volatile statistics */
    uint32_t  ifi_ipackets;                 /* packets received on interface */
    uint32_t  ifi_ierrors;                  /* input errors on interface */
    uint32_t  ifi_opackets;                 /* packets sent on interface */
    uint32_t  ifi_oerrors;                  /* output errors on interface */
    uint32_t  ifi_collisions;               /* collisions on csma interfaces */
    uint32_t  ifi_ibytes;                   /* total number of octets received */
    uint32_t  ifi_obytes;                   /* total number of octets sent */
    uint32_t  ifi_imcasts;                  /* packets received via multicast */
    uint32_t  ifi_omcasts;                  /* packets sent via multicast */
    uint32_t  ifi_iqdrops;                  /* dropped on input, this interface */
    uint32_t  ifi_noproto;                  /* destined for unsupported protocol */
    uint64_t  ifi_hwassist;                 /* HW offload capabilities, see IFCAP */
    time_t    ifi_epoch;                    /* uptime at attach or stat reset */
    struct    timeval ifi_lastchange;       /* time of last administrative change */
#ifdef _IFI_OQDROPS
    uint32_t  ifi_oqdrops;                  /* dropped on output */
#endif
};

/*-
 * Interface flags are of two types: network stack owned flags, and driver
 * owned flags.  Historically, these values were stored in the same ifnet
 * flags field, but with the advent of fine-grained locking, they have been
 * broken out such that the network stack is responsible for synchronizing
 * the stack-owned fields, and the device driver the device-owned fields.
 * Both halves can perform lockless reads of the other half's field, subject
 * to accepting the involved races.
 *
 * Both sets of flags come from the same number space, and should not be
 * permitted to conflict, as they are exposed to user space via a single
 * field.
 *
 * The following symbols identify read and write requirements for fields:
 *
 * (i) if_flags field set by device driver before attach, read-only there
 *     after.
 * (n) if_flags field written only by the network stack, read by either the
 *     stack or driver.
 * (d) if_drv_flags field written only by the device driver, read by either
 *     the stack or driver.
 */
enum
{
    IFF_UP          = 0x1,                 /* (n) interface is up */
    IFF_BROADCAST   = 0x2,                 /* (i) broadcast address valid */
    IFF_DEBUG       = 0x4,                 /* (n) turn on debugging */
    IFF_LOOPBACK    = 0x8,                 /* (i) is a loopback net */
    IFF_POINTOPOINT = 0x10,                /* (i) is a point-to-point link */
    IFF_SMART       = 0x20,                /* (i) interface manages own routes */
    IFF_DRV_RUNNING = 0x40,                /* (d) resources allocated */
    IFF_NOARP       = 0x80,                /* (n) no address resolution protocol */
    IFF_PROMISC     = 0x100,               /* (n) receive all packets */
    IFF_ALLMULTI    = 0x200,               /* (n) receive all multicast packets */
    IFF_DRV_OACTIVE = 0x400,               /* (d) tx hardware queue is full */
    IFF_SIMPLEX     = 0x800,               /* (i) can't hear own transmissions */
    IFF_LINK0       = 0x1000,              /* per link layer defined bit */
    IFF_LINK1       = 0x2000,              /* per link layer defined bit */
    IFF_LINK2       = 0x4000,              /* per link layer defined bit */
    IFF_ALTPHYS     = IFF_LINK2,           /* use alternate physical connection */
    IFF_MULTICAST   = 0x8000,              /* (i) supports multicast */
    IFF_CANTCONFIG  = 0x10000,             /* (i) unconfigurable using ioctl(2) */
    IFF_PPROMISC    = 0x20000,             /* (n) user-requested promisc mode */
    IFF_MONITOR     = 0x40000,             /* (n) user-requested monitor mode */
    IFF_STATICARP   = 0x80000,             /* (n) static ARP */
    IFF_DYING       = 0x200000,            /* (n) interface is winding down */
    IFF_RENAMING    = 0x400000,            /* (n) interface is being renamed */
    IFF_RUNNING     = IFF_DRV_RUNNING,
    IFF_OACTIVE     = IFF_DRV_OACTIVE
};

/*
 * Values for if_link_state.
 */
enum
{
    LINK_STATE_UNKNOWN  = 0,    /* link invalid/unknown */
    LINK_STATE_DOWN     = 1,    /* link is down */
    LINK_STATE_UP       = 2     /* link is up */
};

/*
 * Some convenience macros used for setting ifi_baudrate.
 * XXX 1000 vs. 1024? --thorpej@netbsd.org
 */
#define IF_Kbps(x)      ((uintmax_t)(x) * 1000)     /* kilobits/sec. */
#define IF_Mbps(x)      (IF_Kbps((x) * 1000))       /* megabits/sec. */
#define IF_Gbps(x)      (IF_Mbps((x) * 1000))       /* gigabits/sec. */

/*
 * Capabilities that interfaces can advertise.
 *
 * struct ifnet.if_capabilities
 *   contains the optional features & capabilities a particular interface
 *   supports (not only the driver but also the detected hw revision).
 *   Capabilities are defined by IFCAP_* below.
 * struct ifnet.if_capenable
 *   contains the enabled (either by default or through ifconfig) optional
 *   features & capabilities on this interface.
 *   Capabilities are defined by IFCAP_* below.
 * struct if_data.ifi_hwassist in mbuf CSUM_ flag form, controlled by above
 *   contains the enabled optional feature & capabilites that can be used
 *   individually per packet and are specified in the mbuf pkthdr.csum_flags
 *   field.  IFCAP_* and CSUM_* do not match one to one and CSUM_* may be
 *   more detailed or differenciated than IFCAP_*.
 *   Hwassist features are defined CSUM_* in sys/mbuf.h
 *
 * Capabilities that cannot be arbitrarily changed with ifconfig/ioctl
 * are listed in IFCAP_CANTCHANGE, similar to IFF_CANTCHANGE.
 * This is not strictly necessary because the common code never
 * changes capabilities, and it is left to the individual driver
 * to do the right thing. However, having the filter here
 * avoids replication of the same code in all individual drivers.
 */
enum
{
    IFCAP_RXCSUM            = 0x00001,     /* can offload checksum on RX */
    IFCAP_TXCSUM            = 0x00002,     /* can offload checksum on TX */
    IFCAP_NETCONS           = 0x00004,     /* can be a network console */
    IFCAP_VLAN_MTU          = 0x00008,     /* VLAN-compatible MTU */
    IFCAP_VLAN_HWTAGGING    = 0x00010,     /* hardware VLAN tag support */
    IFCAP_JUMBO_MTU         = 0x00020,     /* 9000 byte MTU supported */
    IFCAP_POLLING           = 0x00040,     /* driver supports polling */
    IFCAP_VLAN_HWCSUM       = 0x00080,     /* can do IFCAP_HWCSUM on VLANs */
    IFCAP_TSO4              = 0x00100,     /* can do TCP Segmentation Offload */
    IFCAP_TSO6              = 0x00200,     /* can do TCP6 Segmentation Offload */
    IFCAP_LRO               = 0x00400,     /* can do Large Receive Offload */
    IFCAP_WOL_UCAST         = 0x00800,     /* wake on any unicast frame */
    IFCAP_WOL_MCAST         = 0x01000,     /* wake on any multicast frame */
    IFCAP_WOL_MAGIC         = 0x02000,     /* wake on any Magic Packet */
    IFCAP_TOE4              = 0x04000,     /* interface can offload TCP */
    IFCAP_TOE6              = 0x08000,     /* interface can offload TCP6 */
    IFCAP_VLAN_HWFILTER     = 0x10000,     /* interface hw can filter vlan tag */
    IFCAP_POLLING_NOCOUNT   = 0x20000,     /* polling ticks cannot be fragmented */
    IFCAP_VLAN_HWTSO        = 0x40000,     /* can do IFCAP_TSO on VLANs */
    IFCAP_LINKSTATE         = 0x80000,     /* the runtime link state is dynamic */
    IFCAP_NETMAP            = 0x100000,    /* netmap mode supported/enabled */
    IFCAP_RXCSUM_IPV6       = 0x200000,    /* can offload checksum on IPv6 RX */
    IFCAP_TXCSUM_IPV6       = 0x400000,    /* can offload checksum on IPv6 TX */
    IFCAP_HWSTATS           = 0x800000,    /* manages counters internally */

    IFCAP_HWCSUM_IPV6       = (IFCAP_RXCSUM_IPV6 | IFCAP_TXCSUM_IPV6),

    IFCAP_HWCSUM            = (IFCAP_RXCSUM | IFCAP_TXCSUM),
    IFCAP_TSO               = (IFCAP_TSO4 | IFCAP_TSO6),
    IFCAP_WOL               = (IFCAP_WOL_UCAST | IFCAP_WOL_MCAST | IFCAP_WOL_MAGIC),
    IFCAP_TOE               = (IFCAP_TOE4 | IFCAP_TOE6),

    IFCAP_CANTCHANGE        = (IFCAP_NETMAP)
};

enum
{
    IFQ_MAXLEN     = 50,
    IFNET_SLOWHZ   = 1             /* granularity is 1 second */
};

/*
 * Message format for use in obtaining information about interfaces
 * from getkerninfo and the routing socket
 * For the new, extensible interface see struct if_msghdrl below.
 */
struct if_msghdr
{
    uint16_t    ifm_msglen;                 /* to skip over non-understood messages */
    uint8_t     ifm_version;                /* future binary compatibility */
    uint8_t     ifm_type;                   /* message type */
    int         ifm_addrs;                  /* like rtm_addrs */
    int         ifm_flags;                  /* value of if_flags */
    uint16_t    ifm_index;                  /* index for associated ifp */
    struct      if_data ifm_data;           /* statistics and other data about if */
};

/*
 * The 'l' version shall be used by new interfaces, like NET_RT_IFLISTL.  It is
 * extensible after ifm_data_off or within ifm_data.  Both the if_msghdr and
 * if_data now have a member field detailing the struct length in addition to
 * the routing message length.  Macros are provided to find the start of
 * ifm_data and the start of the socket address strucutres immediately following
 * struct if_msghdrl given a pointer to struct if_msghdrl.
 */
#define    IF_MSGHDRL_IFM_DATA(_l) \
    (struct if_data *)((char *)(_l) + (_l)->ifm_data_off)
#define    IF_MSGHDRL_RTA(_l) \
    (void *)((uintptr_t)(_l) + (_l)->ifm_len)
struct if_msghdrl
{
    uint16_t    ifm_msglen;                 /* to skip over non-understood messages */
    uint8_t     ifm_version;                /* future binary compatibility */
    uint8_t     ifm_type;                   /* message type */
    int         ifm_addrs;                  /* like rtm_addrs */
    int         ifm_flags;                  /* value of if_flags */
    uint16_t    ifm_index;                  /* index for associated ifp */
    uint16_t   _ifm_spare1;                 /* spare space to grow if_index, see if_var.h */
    uint16_t    ifm_len;                    /* length of if_msghdrl incl. if_data */
    uint16_t    ifm_data_off;               /* offset of if_data from beginning */
    struct      if_data ifm_data;           /* statistics and other data about if */
#ifdef _IN_NET_RTSOCK_C
    uint32_t    ifi_oqdrops;
#endif
};

/*
 * Message format for use in obtaining information about interface addresses
 * from getkerninfo and the routing socket
 * For the new, extensible interface see struct ifa_msghdrl below.
 */
struct ifa_msghdr
{
    uint16_t    ifam_msglen;                /* to skip over non-understood messages */
    uint8_t     ifam_version;               /* future binary compatibility */
    uint8_t     ifam_type;                  /* message type */
    int         ifam_addrs;                 /* like rtm_addrs */
    int         ifam_flags;                 /* value of ifa_flags */
    uint16_t    ifam_index;                 /* index for associated ifp */
    int         ifam_metric;                /* value of ifa_metric */
};

/*
 * The 'l' version shall be used by new interfaces, like NET_RT_IFLISTL.  It is
 * extensible after ifam_metric or within ifam_data.  Both the ifa_msghdrl and
 * if_data now have a member field detailing the struct length in addition to
 * the routing message length.  Macros are provided to find the start of
 * ifm_data and the start of the socket address strucutres immediately following
 * struct ifa_msghdrl given a pointer to struct ifa_msghdrl.
 */
#define IFA_MSGHDRL_IFAM_DATA(_l) \
    (struct if_data *)((char *)(_l) + (_l)->ifam_data_off)
#define IFA_MSGHDRL_RTA(_l) \
    (void *)((uintptr_t)(_l) + (_l)->ifam_len)
struct ifa_msghdrl
{
    uint16_t    ifam_msglen;                /* to skip over non-understood messages */
    uint8_t     ifam_version;               /* future binary compatibility */
    uint8_t     ifam_type;                  /* message type */
    int         ifam_addrs;                 /* like rtm_addrs */
    int         ifam_flags;                 /* value of ifa_flags */
    uint16_t    ifam_index;                 /* index for associated ifp */
    uint16_t   _ifam_spare1;                /* spare space to grow if_index, see if_var.h */
    uint16_t    ifam_len;                   /* length of ifa_msghdrl incl. if_data */
    uint16_t    ifam_data_off;              /* offset of if_data from beginning */
    int         ifam_metric;                /* value of ifa_metric */
    struct      if_data ifam_data;          /* statistics and other data about if or address */
};

/*
 * Message format for use in obtaining information about multicast addresses
 * from the routing socket
 */
struct ifma_msghdr
{
    uint16_t    ifmam_msglen;               /* to skip over non-understood messages */
    uint8_t     ifmam_version;              /* future binary compatibility */
    uint8_t     ifmam_type;                 /* message type */
    int         ifmam_addrs;                /* like rtm_addrs */
    int         ifmam_flags;                /* value of ifa_flags */
    uint16_t    ifmam_index;                /* index for associated ifp */
};

/*
 * Message format announcing the arrival or departure of a network interface.
 */
struct if_announcemsghdr
{
    uint16_t    ifan_msglen;                /* to skip over non-understood messages */
    uint8_t     ifan_version;               /* future binary compatibility */
    uint8_t     ifan_type;                  /* message type */
    uint16_t    ifan_index;                 /* index for associated ifp */
    char        ifan_name[IFNAMSIZ];        /* if name, e.g. "en0" */
    uint16_t    ifan_what;                  /* what type of announcement */
};

enum
{
    IFAN_ARRIVAL       =  0,               /* interface arrival */
    IFAN_DEPARTURE     =  1               /* interface departure */
};

/*
 * Buffer with length to be used in SIOCGIFDESCR/SIOCSIFDESCR requests
 */
struct ifreq_buffer
{
    size_t      length;
    void       *buffer;
};

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct ifreq
{
    char            ifr_name[IFNAMSIZ];     /* if name, e.g. "en0" */
    union
    {
        struct      sockaddr ifru_addr;
        struct      sockaddr ifru_dstaddr;
        struct      sockaddr ifru_broadaddr;
        struct      ifreq_buffer ifru_buffer;
        short       ifru_flags[2];
        short       ifru_index;
        int         ifru_jid;
        int         ifru_metric;
        int         ifru_mtu;
        int         ifru_phys;
        int         ifru_media;
        char*       ifru_data;
        int         ifru_cap[2];
        uint32_t    ifru_fib;
    } ifr_ifru;
#define ifr_addr        ifr_ifru.ifru_addr      /* address */
#define ifr_dstaddr     ifr_ifru.ifru_dstaddr   /* other end of p-to-p link */
#define ifr_broadaddr   ifr_ifru.ifru_broadaddr /* broadcast address */
#define ifr_buffer      ifr_ifru.ifru_buffer    /* user supplied buffer with its length */
#define ifr_flags       ifr_ifru.ifru_flags[0]  /* flags (low 16 bits) */
#define ifr_flagshigh   ifr_ifru.ifru_flags[1]  /* flags (high 16 bits) */
#define ifr_jid         ifr_ifru.ifru_jid       /* jail/vnet */
#define ifr_metric      ifr_ifru.ifru_metric    /* metric */
#define ifr_mtu         ifr_ifru.ifru_mtu       /* mtu */
#define ifr_phys        ifr_ifru.ifru_phys      /* physical wire */
#define ifr_media       ifr_ifru.ifru_media     /* physical media */
#define ifr_data        ifr_ifru.ifru_data      /* for use by interface */
#define ifr_reqcap      ifr_ifru.ifru_cap[0]    /* requested capabilities */
#define ifr_curcap      ifr_ifru.ifru_cap[1]    /* current capabilities */
#define ifr_index       ifr_ifru.ifru_index     /* interface index */
#define ifr_fib         ifr_ifru.ifru_fib       /* interface fib */
};

#define _SIZEOF_ADDR_IFREQ(ifr) \
    ((ifr).ifr_addr.sa_len > sizeof(struct sockaddr) ? \
     (sizeof(struct ifreq) - sizeof(struct sockaddr) + \
      (ifr).ifr_addr.sa_len) : sizeof(struct ifreq))

struct ifaliasreq
{
    char        ifra_name[IFNAMSIZ];            /* if name, e.g. "en0" */
    struct      sockaddr ifra_addr;
    struct      sockaddr ifra_broadaddr;
    struct      sockaddr ifra_mask;
    int         ifra_vhid;
};

/* Compat with pre-10.x */
struct oifaliasreq
{
    char        ifra_name[IFNAMSIZ];
    struct      sockaddr ifra_addr;
    struct      sockaddr ifra_broadaddr;
    struct      sockaddr ifra_mask;
};

struct ifmediareq
{
    char        ifm_name[IFNAMSIZ];                 /* if name, e.g. "en0" */
    int         ifm_current;                        /* current media options */
    int         ifm_mask;                           /* don't care mask */
    int         ifm_status;                         /* media status */
    int         ifm_active;                         /* active options */
    int         ifm_count;                          /* # entries in ifm_ulist array */
    int        *ifm_ulist;                          /* media words */
};

enum
{
    IF_MACLEN = 6
};

struct ifduplicateipinfo
{
    char        ifdup_name[IFNAMSIZ];               /* if name, e.g. "en0" */
    uint32_t    ifdup_addr;                         /* last duplicate ip address. 0.0.0.0 if no duplicate detected */
    uint8_t     ifdup_mac[IF_MACLEN];               /* mac address of last duplicator */
    uint32_t    ifdup_maclen;
};

struct ifdrv
{
    char        ifd_name[IFNAMSIZ];                 /* if name, e.g. "en0" */
    unsigned long   ifd_cmd;
    size_t      ifd_len;
    void       *ifd_data;
};

/*
 * Structure used to retrieve aux status data from interfaces.
 * Kernel suppliers to this interface should respect the formatting
 * needed by ifconfig(8): each line starts with a TAB and ends with
 * a newline.  The canonical example to copy and paste is in if_tun.c.
 */

enum
{
    IFSTATMAX   =  800                              /* 10 lines of text */
};

struct ifstat
{
    char        ifs_name[IFNAMSIZ];                 /* if name, e.g. "en0" */
    char        ascii[IFSTATMAX + 1];
};

/*
 * Structure used in SIOCGIFCONF request.
 * Used to retrieve interface configuration
 * for machine (useful for programs which
 * must know all networks accessible).
 */
struct ifconf
{
    int         ifc_len;                            /* size of associated buffer */
    union
    {
        char*   ifcu_buf;
        struct  ifreq *ifcu_req;
    } ifc_ifcu;
#define ifc_buf ifc_ifcu.ifcu_buf                   /* buffer address */
#define ifc_req ifc_ifcu.ifcu_req                   /* array of structures returned */
};

/*
 * interface groups
 */

static const char IFG_ALL[]    = "all";             /* group contains all interfaces */
static const char IFG_EGRESS[] = "egress";          /* if(s) default route(s) point to */

struct ifg_req
{
    union
    {
        char    ifgrqu_group[IFNAMSIZ];
        char    ifgrqu_member[IFNAMSIZ];
    } ifgrq_ifgrqu;
#define ifgrq_group     ifgrq_ifgrqu.ifgrqu_group
#define ifgrq_member    ifgrq_ifgrqu.ifgrqu_member
};

/*
 * Used to lookup groups for an interface
 */
struct ifgroupreq
{
    char        ifgr_name[IFNAMSIZ];
    uint32_t    ifgr_len;
    union
    {
        char    ifgru_group[IFNAMSIZ];
        struct  ifg_req *ifgru_groups;
    } ifgr_ifgru;
#define ifgr_group      ifgr_ifgru.ifgru_group
#define ifgr_groups     ifgr_ifgru.ifgru_groups
};

/*
 * Structure for SIOC[AGD]LIFADDR
 */
enum
{
    IFLR_PREFIX   = 0x8000                          /* in: prefix given  out: kernel fills id */
};

struct if_laddrreq
{
    char        iflr_name[IFNAMSIZ];
    uint32_t    flags;
    uint32_t    prefixlen;                          /* in/out */
    struct      sockaddr_storage addr;              /* in/out */
    struct      sockaddr_storage dstaddr;           /* out */
};

struct if_nameindex
{
    unsigned int    if_index;                       /* 1, 2, ... */
    char           *if_name;                        /* null terminated name: "le0", ... */
};

