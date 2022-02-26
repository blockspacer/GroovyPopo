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

#define __packed     __attribute__((__packed__))
#define __aligned(x) __attribute__((__aligned__(x)))

#elif defined(NN_BUILD_CONFIG_COMPILER_GCC)

#include_next <sys/cdefs.h>

#endif

#if defined(__cplusplus)
#define __BEGIN_DECLS   extern "C" {
#define __END_DECLS     }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
