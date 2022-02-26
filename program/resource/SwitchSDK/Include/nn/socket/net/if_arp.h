/*-
 * Copyright (c) 1986, 1993
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
 *    @(#)if_arp.h    8.1 (Berkeley) 6/10/93
 * $FreeBSD$
 */

#pragma once

/*
 * Address Resolution Protocol.
 *
 * See RFC 826 for protocol description.  ARP packets are variable
 * in size; the arphdr structure defines the fixed-length portion.
 * Protocol type values are the same as those for 10 Mb/s Ethernet.
 * It is followed by the variable-sized fields ar_sha, arp_spa,
 * arp_tha and arp_tpa in that order, according to the lengths
 * specified.  Field names used correspond to RFC 826.
 */
enum
{
    ARPHRD_ETHER        = 1,        /* ethernet hardware format */
    ARPHRD_IEEE802      = 6,        /* token-ring hardware format */
    ARPHRD_ARCNET       = 7,        /* arcnet hardware format */
    ARPHRD_FRELAY       = 15,       /* frame relay hardware format */
    ARPHRD_IEEE1394     = 24,       /* firewire hardware format */
    ARPHRD_INFINIBAND   = 32,       /* infiniband hardware format */
};
enum
{
    ARPOP_REQUEST       = 1,        /* request to resolve address */
    ARPOP_REPLY         = 2,        /* response to previous request */
    ARPOP_REVREQUEST    = 3,        /* request protocol address given hardware */
    ARPOP_REVREPLY      = 4,        /* response giving protocol address */
    ARPOP_INVREQUEST    = 8,        /* request to identify peer */
    ARPOP_INVREPLY      = 9,        /* response identifying peer */
};

struct arphdr
{
    uint16_t    ar_hrd;             /* format of hardware address */
    uint16_t    ar_pro;             /* format of protocol address */
    uint8_t     ar_hln;             /* length of hardware address */
    uint8_t     ar_pln;             /* length of protocol address */
    uint16_t    ar_op;              /* one of: */
/*
 * The remaining fields are variable in size,
 * according to the sizes above.
 */
#ifdef COMMENT_ONLY
    uint8_t     ar_sha[];           /* sender hardware address */
    uint8_t     ar_spa[];           /* sender protocol address */
    uint8_t     ar_tha[];           /* target hardware address */
    uint8_t     ar_tpa[];           /* target protocol address */
#endif
};

#define ar_sha(ap)      (((caddr_t)((ap) + 1)) +   0)
#define ar_spa(ap)      (((caddr_t)((ap) + 1)) +   (ap)->ar_hln)
#define ar_tha(ap)      (((caddr_t)((ap) + 1)) +   (ap)->ar_hln + (ap)->ar_pln)
#define ar_tpa(ap)      (((caddr_t)((ap) + 1)) + 2 * (ap)->ar_hln + (ap)->ar_pln)

#define arphdr_len2(ar_hln, ar_pln)                    \
                        (sizeof(struct arphdr) + 2 * (ar_hln) + 2 * (ar_pln))
#define arphdr_len(ap)  (arphdr_len2((ap)->ar_hln, (ap)->ar_pln))

/*
 * ARP ioctl request
 */
struct arpreq
{
    struct      sockaddr arp_pa;    /* protocol address */
    struct      sockaddr arp_ha;    /* hardware address */
    int         arp_flags;          /* flags */
};

/*  arp_flags and at_flags field values */
enum
{
    ATF_INUSE           = 0x01,     /* entry in use */
    ATF_COM             = 0x02,     /* completed entry (enaddr valid) */
    ATF_PERM            = 0x04,     /* permanent entry */
    ATF_PUBL            = 0x08,     /* publish entry (respond for other host) */
    ATF_USETRAILERS     = 0x10      /* has requested trailers */
};

struct arpstat
{
    /* Normal things that happen: */
    uint64_t    txrequests;         /* # of ARP requests sent by this host. */
    uint64_t    txreplies;          /* # of ARP replies sent by this host. */
    uint64_t    rxrequests;         /* # of ARP requests received by this host. */
    uint64_t    rxreplies;          /* # of ARP replies received by this host. */
    uint64_t    received;           /* # of ARP packets received by this host. */

    uint64_t    arp_spares[4];      /* For either the upper or lower half. */
    /* Abnormal event and error  counting: */
    uint64_t    dropped;            /* # of packets dropped waiting for a reply. */
    uint64_t    timeouts;           /* # of times with entries removed */
                                    /* due to timeout. */
    uint64_t    dupips;             /* # of duplicate IPs detected. */
};

