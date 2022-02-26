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

#if defined(NN_BUILD_CONFIG_OS_WIN32) && !defined(POSIX_ERRNO_DEFINED)

#include <errno.h>              // NOLINT

/* 140 is the highest errno VC defines in errno.h,
 * start local errno range just above...
 */
#define EREMOTE         150     // NOLINT(readability/define)
#define ESOCKTNOSUPPORT 151     // NOLINT(readability/define)
#define EPFNOSUPPORT    152     // NOLINT(readability/define)
#define ESHUTDOWN       153     // NOLINT(readability/define)
#define EHOSTDOWN       154     // NOLINT(readability/define)
#define ETOOMANYREFS    155     // NOLINT(readability/define)
#define EPROCLIM        156     // NOLINT(readability/define)
#define EUSERS          157     // NOLINT(readability/define)
#define EDQUOT          158     // NOLINT(readability/define)
#define ESTALE          159     // NOLINT(readability/define)

#elif defined(NN_BUILD_CONFIG_COMPILER_CLANG)

#include_next <errno.h>         // NOLINT

#define ENOIOCTL        (-3)    // NOLINT(readability/define)
#define EUNKNOWN        255     // NOLINT(readability/define)

#else

#include_next <sys/errno.h>     // NOLINT

#define ERESTART        (-1)    // NOLINT(readability/define)
#define EJUSTRETURN     (-2)    // NOLINT(readability/define)
#define ENOIOCTL        (-3)    // NOLINT(readability/define)
#define EDIRIOCTL       (-4)    // NOLINT(readability/define)
#define EDOOFUS         88      // NOLINT(readability/define)
#define EUNKNOWN        255     // NOLINT(readability/define)

#endif
