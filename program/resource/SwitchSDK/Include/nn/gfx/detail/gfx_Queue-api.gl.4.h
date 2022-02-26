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

#include <nn/gfx/gfx_Common.h>

#if defined( NN_BUILD_CONFIG_OS_SUPPORTS_WIN32 )
    #include <nn/gfx/detail/gfx_Queue-api.gl.4-os.win32.h>
#elif defined( NN_BUILD_CONFIG_OS_SUPPORTS_HORIZON )
    #include <nn/gfx/detail/gfx_Queue-api.gl.4-os.horizon.h>
#else
    #error
#endif
