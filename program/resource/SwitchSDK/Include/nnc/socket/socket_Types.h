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

#include <nn/os/os_Config.h>
#include <nn/socket/sys/errno.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)

/* Unused function argument in ws2tcpip.h... */
#pragma warning (disable : 4100)
#pragma warning (disable : 4101)

#if defined(POSIX_FUNCTION_DECLARATIONS)
/* WIN32 network headers:
 * Force winsock header not to define
 * posix functions
 */
#define INCL_WINSOCK_API_PROTOTYPES 0   // NOLINT
#define WSASetLastError(a)
#define WSAIoctl(a0, a1, a2, a3, a4, a5, a6, a7, a8) 0
#endif  /* POSIX_FUNCTION_DECLARATIONS */

#include <ws2tcpip.h>

#include <nn/socket/sys/types.h>

/* this message should emulate nonblocking semantics */
#if defined(NN_BUILD_CONFIG_OS_WIN) & !defined(MSG_DONTWAIT)
#define MSG_DONTWAIT 0x80   // NOLINT(preprocessor/const)
#endif // WINDOWS

#ifndef USE_WINSOCK_FD_SET
/* POSIX style FD_* definitions to replace winsock's non-compliant ones */
#ifdef  FD_ZERO
#undef  FD_ZERO
#endif
#ifdef  FD_SET
#undef  FD_SET
#endif
#ifdef  FD_CLR
#undef  FD_CLR
#endif
#ifdef  FD_ISSET
#undef  FD_ISSET
#endif
#define FD_ZERO(s)      POSIX_FD_ZERO(s)
#define FD_SET(d, s)    POSIX_FD_SET(d, s)
#define FD_CLR(d, s)    POSIX_FD_CLR(d, s)
#define FD_ISSET(d, s)  POSIX_FD_ISSET(d, s)
#endif  // USE_WINSOCK_FD_SET

#else

#include <stdint.h>
#include <sys/types.h>
#if defined(NN_BUILD_CONFIG_COMPILER_GCC)
#include <sys/param.h>
#endif
#include <sys/time.h>
#include <nn/socket/sys/cdefs.h>
#include <nn/socket/sys/poll.h>
#include <nn/socket/sys/fcntl.h>
#include <nn/socket/sys/filio.h>
#include <nn/socket/sys/select.h>
#include <nn/socket/sys/socket.h>
#include <nn/socket/sys/sockio.h>
#include <nn/socket/netinet/in.h>
#include <nn/socket/net/if.h>
#include <nn/socket/net/route.h>
#include <nn/socket/netinet/ip.h>
#include <nn/socket/netinet/tcp.h>
#include <nn/socket/netinet/ip_icmp.h>

#endif

/* netdb.h needs socklen_t from types.h */

#include <nn/socket/netdb.h>
