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

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
    #include <nn/TargetConfigs/detail/build_Compiler.vc.h>
#elif defined(NN_BUILD_CONFIG_TOOLCHAIN_GCC)
    #include <nn/TargetConfigs/detail/build_Compiler.gcc.h>
#elif defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
    #include <nn/TargetConfigs/detail/build_Compiler.clang.h>
#elif defined(NN_BUILD_CONFIG_TOOLCHAIN_GHS)
    #include <nn/TargetConfigs/detail/build_Compiler.ghs.h>
#else
    #error   "未サポートのツールチェインが指定されています。"
#endif

