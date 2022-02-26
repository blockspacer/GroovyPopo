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

//#include <nn/TargetConfigs/build_Base.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/TargetConfigs/detail/build_Os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/TargetConfigs/detail/build_Os.horizon.h>
#elif defined(NN_BUILD_CONFIG_OS_COS)
    #include <nn/TargetConfigs/detail/build_Os.cos.h>
#else
    #error   "未サポートの OS 種別が指定されています。"
#endif

