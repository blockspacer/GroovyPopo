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

#include <nn/nn_Macro.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)

typedef SSIZE_T ssize_t;
typedef int     nfds_t;

typedef struct msghdr
{
    void                   *msg_name;       /* optional address */
    socklen_t               msg_namelen;    /* size of address */
    struct iovec           *msg_iov;        /* scatter/gather array */
    int                     msg_iovlen;     /* # elements in msg_iov */
    void                   *msg_control;    /* ancillary data, see below */
    socklen_t               msg_controllen; /* ancillary data buffer len */
    int                     msg_flags;      /* flags on received message */
} msghdr;

typedef struct mmsghdr
{
    struct msghdr   msg_hdr;
    socklen_t       msg_len;
} mmsghdr;

enum
{
    F_GETFL    = 3,
    F_SETFL    = 4,
    O_NONBLOCK = F_SETFL,
};

enum
{
    SHUT_RD    = SD_RECEIVE,
    SHUT_WR    = SD_SEND,
    SHUT_RDWR  = SD_BOTH
};

/* No special linger functionality needed for Windows platforms.
   So just map NN version to standard SO_LINGER. */
#define SO_NN_LINGER SO_LINGER

#define POSIX_FD_ZERO(s)                                                                            \
    do                                                                                              \
    {                                                                                               \
        int nnsocketBitIndex;                                                                       \
        for(nnsocketBitIndex = 0;                                                                   \
            nnsocketBitIndex < sizeof((s)->fd_array) / sizeof((s)->fd_array[0]);                    \
            nnsocketBitIndex++)                                                                     \
        {                                                                                           \
            (s)->fd_array[nnsocketBitIndex] = 0;                                                    \
        }                                                                                           \
    } while(NN_STATIC_CONDITION(0))

#define POSIX_FD_SET(d, s)   ((s)->fd_array[(d) / (8 * sizeof((s)->fd_array[0]))] |=  (((SOCKET)1)<<((d) % (8 * sizeof((s)->fd_array[0])))))
#define POSIX_FD_CLR(d, s)   ((s)->fd_array[(d) / (8 * sizeof((s)->fd_array[0]))] &= ~(((SOCKET)1)<<((d) % (8 * sizeof((s)->fd_array[0])))))
#define POSIX_FD_ISSET(d, s) !!((s)->fd_array[(d) / (8 * sizeof((s)->fd_array[0]))] & (((SOCKET)1)<<((d) % (8 * sizeof((s)->fd_array[0])))))

#if defined(InetPton)
#undef InetPton
#endif // InetPton

#if defined(InetNtop)
#undef InetNtop
#endif // InetNtop

#if defined(GetAddrInfo)
#undef GetAddrInfo
#endif // GetAddrInfo

#if defined(FreeAddrInfo)
#undef FreeAddrInfo
#endif // FreeAddrInfo

#if defined(GetNameInfo)
#undef GetNameInfo
#endif // GetNameInfo

#else

#include_next <sys/types.h>

#endif

