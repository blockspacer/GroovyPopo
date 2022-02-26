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

#include_next <fcntl.h>

#define FREAD       0x0001          // NOLINT(readability/define)
#define FWRITE      0x0002          // NOLINT(readability/define)

#else

#include_next <sys/fcntl.h>

#define O_CLOEXEC   0x00100000      // NOLINT(readability/define)
#define FNONBLOCK   O_NONBLOCK      // NOLINT(readability/define)

__BEGIN_DECLS
int fcntl(int, int, ...);
__END_DECLS

#endif

