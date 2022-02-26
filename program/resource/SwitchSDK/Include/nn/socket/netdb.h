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

#if defined(NN_BUILD_CONFIG_OS_WIN)
#ifndef EAI_SYSTEM
#define EAI_SYSTEM 11
#endif
#else
/*-
 * Copyright (c) 1980, 1983, 1988, 1993
 * The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 * This product includes software developed by the University of
 * California, Berkeley and its contributors.
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
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 * -
 * --Copyright--
 */

/*
 *      @(#)netdb.h 8.1 (Berkeley) 6/2/93
 *      From: Id: netdb.h,v 8.9 1996/11/19 08:39:29 vixie Exp $
 * $FreeBSD: /repoman/r/ncvs/src/include/netdb.h,v 1.41 2006/04/15 16:20:26 ume Exp $
 */
#include <nn/socket/sys/cdefs.h>
#include <nn/socket/sys/types.h>
#include <nn/socket/sys/socket.h>

#ifndef _PATH_HEQUIV
# define _PATH_HEQUIV "/system/etc/hosts.equiv"
#endif
#define _PATH_HOSTS "/system/etc/hosts"
#define _PATH_NETWORKS "/system/etc/networks"
#define _PATH_PROTOCOLS "/system/etc/protocols"
#define _PATH_SERVICES "/system/etc/services"

/*
 * Structures returned by network data base library.  All addresses are
 * supplied in host order, and returned in network order (suitable for
 * use in system calls).
 */
struct hostent
{

    /**
     * @brief official name of host
     */
    char *h_name;

    /**
     * @briefalias list
     */
    char **h_aliases;

    /**
     * @brief host address type
     */
    int h_addrtype;

    /**
     * @brief length of address
     */
    int h_length;

    /**
     * @brief list of addresses from name server
     */
    char **h_addr_list;

    /**
     * @brief address, for backward compatibility
     */
#define h_addr h_addr_list[0]
};

struct netent
{
    /**
     * @brief official name of net
     */
    char  *n_name;

    /**
     * @brief alias list
     */
    char  **n_aliases;

    /**
     *  @brief net address type
     */
    int  n_addrtype;

    /**
     * @brief network #
     */
    uint32_t n_net;
};

struct servent
{
    /**
     * @brief official service name
     */
    char *s_name;
    /**
     * @brief alias list
     */
    char **s_aliases;

    /**
     * @brief port #
     */
    int s_port;

    /**
     * @brief protocol to use
     */
    char *s_proto;
};

struct protoent
{
    /**
     * @brief official protocol name
     */
    char *p_name;

    /**
     * @brief alias list
     */
    char **p_aliases;

    /**
     * @brief protocol #
     */
    int p_proto;
};

struct addrinfo
{
    /**
     * @brief AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
     */
    int ai_flags;

    /**
     * @brief PF_xxx
     */
    int ai_family;

    /**
     * @brief SOCK_xxx
     */
    int ai_socktype;

    /**
     * @brief 0 or IPPROTO_xxx for IPv4 and IPv6
     */
    int ai_protocol;

    /**
     * @brief length of ai_addr
     */
    socklen_t ai_addrlen;

    /**
     * @brief canonical name for hostname
     */
    char *ai_canonname;

    /**
     * @brief binary address
     */
    struct sockaddr *ai_addr;

    /**
     * @brief next structure in linked list
     */
    struct addrinfo *ai_next;
};

/**
 * @brief Error return codes from GetHostByName() and GetHostByAddr() (left in h_errno).
 */

    /**
     * @brief see GetLastErrno()
     */
#define	NETDB_INTERNAL	-1	/* see errno */
    /**
     * @brief no problem
     */
#define	NETDB_SUCCESS	0	/* no problem */
    /**
     * @brief Authoritative Answer Host not found
     */
#define	HOST_NOT_FOUND	1 /* Authoritative Answer Host not found */
    /**
     * @brief Non-Authoritative Host not found, or SERVERFAIL
     */
#define	TRY_AGAIN	2 /* Non-Authoritive Host not found, or SERVERFAIL */
    /**
     * @brief Non recoverable errors, FORMERR, REFUSED, NOTIMP
     */
#define	NO_RECOVERY	3 /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
    /**
     * @brief Valid name, no data record of requested type
     */
#define	NO_DATA		4 /* Valid name, no data record of requested type */
    /**
     * @brief no address, look for MX record
     */
#define	NO_ADDRESS	NO_DATA		/* no address, look for MX record */



/*
 * Error return codes from getaddrinfo()
 */
    /**
     * @brief address family for hostname not supported
     */
#define	EAI_ADDRFAMILY 1
    /**
     * @brief temporary failure in name resolution
     */
#define	EAI_AGAIN      2
    /**
     * @brief invalid value for ai_flags
     */
#define	EAI_BADFLAGS   3
    /**
     * @brief non-recoverable failure in name resolution
     */
#define	EAI_FAIL       4
    /**
     * @brief ai_family not supported
     */
#define	EAI_FAMILY     5
    /**
     * @brief memory allocation failure
     */
#define	EAI_MEMORY     6
    /**
     * @brief no adqdress associated with hostname
     */
#define	EAI_NODATA     7
    /**
     * @brief hostname nor servname provided, or not known
     */

#define	EAI_NONAME     8
    /**
     * @brief servname not supported for ai_socktype
     */
#define	EAI_SERVICE    9
    /**
     * @brief ai_socktype not supported
     */
#define	EAI_SOCKTYPE   10
    /**
     * @brief system error returned in errno
     */
#define	EAI_SYSTEM     11
    /**
     * @brief invalid value for hints
     */
#define	EAI_BADHINTS   12
    /**
     * @brief resolved protocol is unknown
     */
#define	EAI_PROTOCOL   13
    /**
     * @brief argument buffer overflow
     */
#define	EAI_OVERFLOW   14
    /**
     * @brief used for bounds checking
     */
#define	EAI_MAX   15
/*
 * Flag values for getaddrinfo()
 */
    /**
     * @brief get address to use Bind()
     */
#define	AI_PASSIVE      0x00000001

    /**
     * @brief fill ai_canonname
     */
#define	AI_CANONNAME    0x00000002

    /**
     * @brief prevent host name resolution
     */
#define	AI_NUMERICHOST  0x00000004

    /**
     * @brief prevent service name resolution
     */
#define	AI_NUMERICSERV  0x00000008

    /* valid flags for addrinfo (not a standard def, apps should not use it) */

    /**
     * @brief IPv6 and IPv4-mapped (with AI_V4MAPPED)
     */
#define	AI_ALL          0x00000100

    /**
     * @brief accept IPv4-mapped if kernel supports
     */
#define	AI_V4MAPPED_CFG 0x00000200

    /**
     * @brief only if any address is assigned
     */
#define	AI_ADDRCONFIG   0x00000400

    /**
     * @brief accept IPv4-mapped IPv6 address
     */
#define	AI_V4MAPPED     0x00000800


#define AI_MASK (AI_PASSIVE | AI_CANONNAME | AI_NUMERICHOST | AI_NUMERICSERV | AI_ADDRCONFIG)
#define AI_DEFAULT (AI_V4MAPPED_CFG | AI_ADDRCONFIG)

/*
 * Constants for getnameinfo()
 */
#define	NI_MAXHOST 1025
#define	NI_MAXSERV 32

/**
 * @brief Flag values for getnameinfo()
 */
#define	NI_NOFQDN      0x00000001
#define	NI_NUMERICHOST 0x00000002
#define	NI_NAMEREQD    0x00000004
#define	NI_NUMERICSERV 0x00000008
#define	NI_DGRAM       0x00000010

/*
 * Scope delimit character
 */
#define SCOPE_DELIMITER '%'
#endif // !WINDOWS
