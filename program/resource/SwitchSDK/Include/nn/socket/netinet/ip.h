/*-
 * Copyright (c) 1982, 1986, 1993
 *    The Regents of the University of California.
 * All rights reserved.
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
 *    @(#)ip.h    8.2 (Berkeley) 6/1/94
 * $FreeBSD$
 */

#pragma once

/*
 * Definitions for internet protocol version 4.
 *
 * Per RFC 791, September 1981.
 */
#ifndef IPVERSION
enum
{
    IPVERSION = 4
};
#endif

/*
 * Structure of an internet header, naked of options.
 */
enum
{
    IP_RF        = 0x8000,                  /* reserved fragment flag */
    IP_DF        = 0x4000,                  /* dont fragment flag */
    IP_MF        = 0x2000,                  /* more fragments flag */
    IP_OFFMASK   = 0x1fff                   /* mask for fragmenting bits */
};

struct ip
{
#if BYTE_ORDER == LITTLE_ENDIAN
    uint8_t         ip_hl:4,                /* header length */
                    ip_v:4;                 /* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
    uint8_t         ip_v:4,                 /* version */
                    ip_hl:4;                /* header length */
#endif
    uint8_t         ip_tos;                 /* type of service */
    uint16_t        ip_len;                 /* total length */
    uint16_t        ip_id;                  /* identification */
    uint16_t        ip_off;                 /* fragment offset field */
    uint8_t         ip_ttl;                 /* time to live */
    uint8_t         ip_p;                   /* protocol */
    uint16_t        ip_sum;                 /* checksum */
    struct          in_addr ip_src,ip_dst;  /* source and dest address */
} __packed __aligned(4);

enum
{
    IP_MAXPACKET = 65535                    /* maximum packet size */
};

enum
{
/*
 * Definitions for IP type of service (ip_tos).
 */
    IPTOS_LOWDELAY              = 0x10,
    IPTOS_THROUGHPUT            = 0x08,
    IPTOS_RELIABILITY           = 0x04,
    IPTOS_MINCOST               = 0x02,

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused).
 */
    IPTOS_PREC_NETCONTROL       = 0xe0,
    IPTOS_PREC_INTERNETCONTROL  = 0xc0,
    IPTOS_PREC_CRITIC_ECP       = 0xa0,
    IPTOS_PREC_FLASHOVERRIDE    = 0x80,
    IPTOS_PREC_FLASH            = 0x60,
    IPTOS_PREC_IMMEDIATE        = 0x40,
    IPTOS_PREC_PRIORITY         = 0x20,
    IPTOS_PREC_ROUTINE          = 0x00,

/*
 * Definitions for DiffServ Codepoints as per RFC2474
 */
    IPTOS_DSCP_CS0              = 0x00,
    IPTOS_DSCP_CS1              = 0x20,
    IPTOS_DSCP_AF11             = 0x28,
    IPTOS_DSCP_AF12             = 0x30,
    IPTOS_DSCP_AF13             = 0x38,
    IPTOS_DSCP_CS2              = 0x40,
    IPTOS_DSCP_AF21             = 0x48,
    IPTOS_DSCP_AF22             = 0x50,
    IPTOS_DSCP_AF23             = 0x58,
    IPTOS_DSCP_CS3              = 0x60,
    IPTOS_DSCP_AF31             = 0x68,
    IPTOS_DSCP_AF32             = 0x70,
    IPTOS_DSCP_AF33             = 0x78,
    IPTOS_DSCP_CS4              = 0x80,
    IPTOS_DSCP_AF41             = 0x88,
    IPTOS_DSCP_AF42             = 0x90,
    IPTOS_DSCP_AF43             = 0x98,
    IPTOS_DSCP_CS5              = 0xa0,
    IPTOS_DSCP_EF               = 0xb8,
    IPTOS_DSCP_CS6              = 0xc0,
    IPTOS_DSCP_CS7              = 0xe0,

/*
 * ECN (Explicit Congestion Notification) codepoints in RFC3168 mapped to the
 * lower 2 bits of the TOS field.
 */
    IPTOS_ECN_NOTECT            = 0x00,     /* not-ECT */
    IPTOS_ECN_ECT1              = 0x01,     /* ECN-capable transport (1) */
    IPTOS_ECN_ECT0              = 0x02,     /* ECN-capable transport (0) */
    IPTOS_ECN_CE                = 0x03,     /* congestion experienced */
    IPTOS_ECN_MASK              = 0x03,     /* ECN field mask */
};

/*
 * Definitions for options.
 */
#define IPOPT_COPIED(o)         ((o)&0x80)
#define IPOPT_CLASS(o)          ((o)&0x60)
#define IPOPT_NUMBER(o)         ((o)&0x1f)
enum
{
    IPOPT_CONTROL               = 0x00,
    IPOPT_RESERVED1             = 0x20,
    IPOPT_DEBMEAS               = 0x40,
    IPOPT_RESERVED2             = 0x60,

    IPOPT_EOL                   = 0,        /* end of option list */
    IPOPT_NOP                   = 1,        /* no operation */

    IPOPT_RR                    = 7,        /* record packet route */
    IPOPT_TS                    = 68,       /* timestamp */
    IPOPT_SECURITY              = 130,      /* provide s,c,h,tcc */
    IPOPT_LSRR                  = 131,      /* loose source route */
    IPOPT_ESO                   = 133,      /* extended security */
    IPOPT_CIPSO                 = 134,      /* commerical security */
    IPOPT_SATID                 = 136,      /* satnet id */
    IPOPT_SSRR                  = 137,      /* strict source route */
    IPOPT_RA                    = 148,      /* router alert */
/*
 * Offsets to fields in options other than EOL and NOP.
 */
    IPOPT_OPTVAL                = 0,        /* option ID */
    IPOPT_OLEN                  = 1,        /* option length */
    IPOPT_OFFSET                = 2,        /* offset within option */
    IPOPT_MINOFF                = 4,        /* min value of above */
};

/*
 * Time stamp option structure.
 */
struct ip_timestamp
{
    uint8_t     ipt_code;                   /* IPOPT_TS */
    uint8_t     ipt_len;                    /* size of structure (variable) */
    uint8_t     ipt_ptr;                    /* index of current entry */
#if BYTE_ORDER == LITTLE_ENDIAN
    uint8_t     ipt_flg:4,                  /* flags, see below */
                ipt_oflw:4;                 /* overflow counter */
#endif
#if BYTE_ORDER == BIG_ENDIAN
    uint8_t     ipt_oflw:4,                 /* overflow counter */
                ipt_flg:4;                  /* flags, see below */
#endif
    union ipt_timestamp
    {
        uint32_t    ipt_time[1];            /* network format */
        struct      ipt_ta
        {
            struct in_addr ipt_addr;
            uint32_t ipt_time;              /* network format */
        } ipt_ta[1];
    } ipt_timestamp;
};

/* Flag bits for ipt_flg. */
enum
{
    IPOPT_TS_TSONLY             = 0,       /* timestamps only */
    IPOPT_TS_TSANDADDR          = 1,       /* timestamps and addresses */
    IPOPT_TS_PRESPEC            = 3,       /* specified modules only */

/* Bits for security (not byte swapped). */
    IPOPT_SECUR_UNCLASS         = 0x0000,
    IPOPT_SECUR_CONFID          = 0xf135,
    IPOPT_SECUR_EFTO            = 0x789a,
    IPOPT_SECUR_MMMM            = 0xbc4d,
    IPOPT_SECUR_RESTR           = 0xaf13,
    IPOPT_SECUR_SECRET          = 0xd788,
    IPOPT_SECUR_TOPSECRET       = 0x6bc5,
};

/*
 * Internet implementation parameters.
 */
enum
{
    MAXTTL    = 255,                        /* maximum time to live (seconds) */
    IPDEFTTL  = 64,                         /* default ttl, from RFC 1340 */
    IPFRAGTTL = 60,                         /* time to live for frags, slowhz */
    IPTTLDEC  = 1,                          /* subtracted when forwarding */
    IP_MSS    = 576                         /* default maximum segment size */
};

/*
 * This is the real IPv4 pseudo header, used for computing the TCP and UDP
 * checksums. For the Internet checksum, struct ipovly can be used instead.
 * For stronger checksums, the real thing must be used.
 */
struct ippseudo
{
    struct    in_addr   ippseudo_src;       /* source internet address */
    struct    in_addr   ippseudo_dst;       /* destination internet address */
    uint8_t             ippseudo_pad;       /* pad, must be zero */
    uint8_t             ippseudo_p;         /* protocol */
    uint16_t            ippseudo_len;       /* protocol length */
};
