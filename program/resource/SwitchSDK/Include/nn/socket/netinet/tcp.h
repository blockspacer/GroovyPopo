/*-
 * Copyright (c) 1982, 1986, 1993
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
 *    @(#)tcp.h    8.1 (Berkeley) 6/10/93
 * $FreeBSD$
 */

#pragma once

#include <nn/socket/sys/cdefs.h>

typedef uint32_t tcp_seq;

#define tcp6_seq tcp_seq             /* for KAME src sync over BSD*'s */
#define tcp6hdr  tcphdr              /* for KAME src sync over BSD*'s */

/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
enum
{
    TH_FIN    = 0x01,
    TH_SYN    = 0x02,
    TH_RST    = 0x04,
    TH_PUSH   = 0x08,
    TH_ACK    = 0x10,
    TH_URG    = 0x20,
    TH_ECE    = 0x40,
    TH_CWR    = 0x80,
    TH_FLAGS  = (TH_FIN|TH_SYN|TH_RST|TH_PUSH|TH_ACK|TH_URG|TH_ECE|TH_CWR)
};

static const char PRINT_TH_FLAGS[] = "\20\1FIN\2SYN\3RST\4PUSH\5ACK\6URG\7ECE\10CWR";

struct tcphdr
{
    uint16_t    th_sport;               /* source port */
    uint16_t    th_dport;               /* destination port */
    tcp_seq     th_seq;                 /* sequence number */
    tcp_seq     th_ack;                 /* acknowledgement number */
#if BYTE_ORDER == LITTLE_ENDIAN
    uint8_t     th_x2:4,                /* (unused) */
                th_off:4;               /* data offset */
#endif
#if BYTE_ORDER == BIG_ENDIAN
    uint8_t     th_off:4,               /* data offset */
                th_x2:4;                /* (unused) */
#endif
    uint8_t    th_flags;
    uint16_t    th_win;                 /* window */
    uint16_t    th_sum;                 /* checksum */
    uint16_t    th_urp;                 /* urgent pointer */
};

enum
{
    TCPOPT_EOL              = 0,
    TCPOLEN_EOL             = 1,
    TCPOPT_PAD              = 0,        /* padding after EOL */
    TCPOLEN_PAD             = 1,
    TCPOPT_NOP              = 1,
    TCPOLEN_NOP             = 1,
    TCPOPT_MAXSEG           = 2,
    TCPOLEN_MAXSEG          = 4,
    TCPOPT_WINDOW           = 3,
    TCPOLEN_WINDOW          = 3,
    TCPOPT_SACK_PERMITTED   = 4,
    TCPOLEN_SACK_PERMITTED  = 2,
    TCPOPT_SACK             = 5,
    TCPOLEN_SACKHDR         = 2,
    TCPOLEN_SACK            = 8,        /* 2*sizeof(tcp_seq) */
    TCPOPT_TIMESTAMP        = 8,
    TCPOLEN_TIMESTAMP       = 10,
    TCPOLEN_TSTAMP_APPA     = (TCPOLEN_TIMESTAMP + 2), /* appendix A */
    TCPOPT_SIGNATURE        = 19,       /* Keyed MD5: RFC 2385 */
    TCPOLEN_SIGNATURE       = 18,
};

/* Miscellaneous constants */
enum
{
    MAX_SACK_BLKS           = 6,        /* Max # SACK blocks stored at receiver side */
    TCP_MAX_SACK            = 4         /* MAX # SACKs sent in any segment */
};

/*
 * The default maximum segment size (MSS) to be used for new TCP connections
 * when path MTU discovery is not enabled.
 *
 * RFC879 derives the default MSS from the largest datagram size hosts are
 * minimally required to handle directly or through IP reassembly minus the
 * size of the IP and TCP header.  With IPv6 the minimum MTU is specified
 * in RFC2460.
 *
 * For IPv4 the MSS is 576 - sizeof(struct tcpiphdr)
 * For IPv6 the MSS is IPV6_MMTU - sizeof(struct ip6_hdr) - sizeof(struct tcphdr)
 *
 * We use explicit numerical definition here to avoid header pollution.
 */
enum
{
    TCP_MSS                 = 536,
    TCP6_MSS                = 1220
};

/*
 * Limit the lowest MSS we accept for path MTU discovery and the TCP SYN MSS
 * option.  Allowing low values of MSS can consume significant resources and
 * be used to mount a resource exhaustion attack.
 * Connections requesting lower MSS values will be rounded up to this value
 * and the IP_DF flag will be cleared to allow fragmentation along the path.
 *
 * See tcp_subr.c tcp_minmss SYSCTL declaration for more comments.  Setting
 * it to "0" disables the minmss check.
 *
 * The default value is fine for TCP across the Internet's smallest official
 * link MTU (256 bytes for AX.25 packet radio).  However, a connection is very
 * unlikely to come across such low MTU interfaces these days (anno domini 2003).
 */
enum
{
    TCP_MINMSS              = 216,
    TCP_MAXWIN              = 49152,    /* largest value for (unscaled) window.
                                        This value bitshifted by the determined window shift will allow
                                        for a window value of 192k, which has been determined to
                                        be a good maximum window to minimize packet loss at the interface due
                                        to insufficent buffer space. */
    TTCP_CLIENT_SND_WND     = 4096,     /* dflt send window for T/TCP client */
    TCP_MAX_WINSHIFT        = 14,       /* maximum window shift */
    TCP_MAXBURST            = 4,        /* maximum segments in a burst */
    TCP_MAXHLEN             = (0xf<<2), /* max length of header in bytes */
    TCP_MAXOLEN             = (TCP_MAXHLEN - sizeof(struct tcphdr))  /* max space left for options */
};

/*
 * User-settable options (used with setsockopt).  These are discrete
 * values and are not masked together.  Some values appear to be
 * bitmasks for historical reasons.
 */
enum
{
    TCP_NODELAY             = 1,        /* don't delay send to coalesce packets */
    TCP_MAXSEG              = 2,        /* set maximum segment size */
    TCP_NOPUSH              = 4,        /* don't push last block of write */
    TCP_NOOPT               = 8,        /* don't use TCP options */
    TCP_MD5SIG              = 16,       /* use MD5 digests (RFC2385) */
    TCP_INFO                = 32,       /* retrieve tcp_info structure */
    TCP_CONGESTION          = 64,       /* get/set congestion control algorithm */
    TCP_KEEPINIT            = 128,      /* N, time to establish connection */
    TCP_KEEPIDLE            = 256,      /* L,N,X start keeplives after this period */
    TCP_KEEPINTVL           = 512,      /* L,N interval between keepalives */
    TCP_KEEPCNT             = 1024,     /* L,N number of keepalives before close */

/* Start of reserved space for third-party user-settable options. */
    TCP_VENDOR              = SO_VENDOR,

    TCP_CA_NAME_MAX         = 16,       /* max congestion control name length */

    TCPI_OPT_TIMESTAMPS     = 0x01,
    TCPI_OPT_SACK           = 0x02,
    TCPI_OPT_WSCALE         = 0x04,
    TCPI_OPT_ECN            = 0x08,
    TCPI_OPT_TOE            = 0x10,
};

/*
 * The TCP_INFO socket option comes from the Linux 2.6 TCP API, and permits
 * the caller to query certain information about the state of a TCP
 * connection.  We provide an overlapping set of fields with the Linux
 * implementation, but since this is a fixed size structure, room has been
 * left for growth.  In order to maximize potential future compatibility with
 * the Linux API, the same variable names and order have been adopted, and
 * padding left to make room for omitted fields in case they are added later.
 *
 * XXX: This is currently an unstable ABI/API, in that it is expected to
 * change.
 */
struct tcp_info
{
    uint8_t     tcpi_state;             /* TCP FSM state. */
    uint8_t     __tcpi_ca_state;
    uint8_t     __tcpi_retransmits;
    uint8_t     __tcpi_probes;
    uint8_t     __tcpi_backoff;
    uint8_t     tcpi_options;           /* Options enabled on conn. */
    uint8_t     tcpi_snd_wscale:4,      /* RFC1323 send shift value. */
                tcpi_rcv_wscale:4;      /* RFC1323 recv shift value. */

    uint32_t    tcpi_rto;               /* Retransmission timeout (usec). */
    uint32_t    __tcpi_ato;
    uint32_t    tcpi_snd_mss;           /* Max segment size for send. */
    uint32_t    tcpi_rcv_mss;           /* Max segment size for receive. */

    uint32_t    __tcpi_unacked;
    uint32_t    __tcpi_sacked;
    uint32_t    __tcpi_lost;
    uint32_t    __tcpi_retrans;
    uint32_t    __tcpi_fackets;

    /* Times; measurements in usecs. */
    uint32_t    __tcpi_last_data_sent;
    uint32_t    __tcpi_last_ack_sent;   /* Also unimpl. on Linux? */
    uint32_t    tcpi_last_data_recv;    /* Time since last recv data. */
    uint32_t    __tcpi_last_ack_recv;

    /* Metrics; variable units. */
    uint32_t    __tcpi_pmtu;
    uint32_t    __tcpi_rcv_ssthresh;
    uint32_t    tcpi_rtt;               /* Smoothed RTT in usecs. */
    uint32_t    tcpi_rttvar;            /* RTT variance in usecs. */
    uint32_t    tcpi_snd_ssthresh;      /* Slow start threshold. */
    uint32_t    tcpi_snd_cwnd;          /* Send congestion window. */
    uint32_t    __tcpi_advmss;
    uint32_t    __tcpi_reordering;

    uint32_t    __tcpi_rcv_rtt;
    uint32_t    tcpi_rcv_space;         /* Advertised recv window. */

    /* FreeBSD extensions to tcp_info. */
    uint32_t    tcpi_snd_wnd;           /* Advertised send window. */
    uint32_t    tcpi_snd_bwnd;          /* No longer used. */
    uint32_t    tcpi_snd_nxt;           /* Next egress seqno */
    uint32_t    tcpi_rcv_nxt;           /* Next ingress seqno */
    uint32_t    tcpi_toe_tid;           /* HWTID for TOE endpoints */
    uint32_t    tcpi_snd_rexmitpack;    /* Retransmitted packets */
    uint32_t    tcpi_rcv_ooopack;       /* Out-of-order packets */
    uint32_t    tcpi_snd_zerowin;       /* Zero-sized windows sent */

    /* Padding to grow without breaking ABI. */
    uint32_t    __tcpi_pad[26];         /* Padding. */
};
