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

#if !defined(NN_BUILD_CONFIG_OS_WIN32)
    #error   "OS 種別として WIN32 が指定されていません。"
#endif

// 必要に応じて、NN_BUILD_CONFIG_OS_XXX         もしくは、
//               NN_BUILD_CONFIG_OS_SUPPORTS_XXX のマクロ定義を並べる。


