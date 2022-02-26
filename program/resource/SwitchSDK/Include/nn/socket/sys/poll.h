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

#if defined(NN_BUILD_CONFIG_COMPILER_CLANG)

#include_next <poll.h>

#define POLLINIGNEOF 0x3000 // NOLINT(readability/define)
#define INFTIM       (-1)   // NOLINT(readability/define)

#else

#define POLLIN       0x001  // NOLINT(readability/define)
#define POLLPRI      0x002  // NOLINT(readability/define)
#define POLLOUT      0x004  // NOLINT(readability/define)
#define POLLERR      0x008  // NOLINT(readability/define)
#define POLLHUP      0x010  // NOLINT(readability/define)
#define POLLNVAL     0x020  // NOLINT(readability/define)
#define POLLRDNORM   0x040  // NOLINT(readability/define)
#define POLLRDBAND   0x080  // NOLINT(readability/define)
#define POLLWRNORM   0x100  // NOLINT(readability/define)
#define POLLWRBAND   0x200  // NOLINT(readability/define)
#define POLLMSG      0x400  // NOLINT(readability/define)
#define POLLRDHUP    0x2000 // NOLINT(readability/define)
#define POLLINIGNEOF 0x3000 // NOLINT(readability/define)

#define INFTIM       (-1)   // NOLINT(readability/define)

typedef unsigned long nfds_t;

struct pollfd
{
    int   fd;
    short events;
    short revents;
};

__BEGIN_DECLS
int poll (struct pollfd *, nfds_t, int);
__END_DECLS

#endif
