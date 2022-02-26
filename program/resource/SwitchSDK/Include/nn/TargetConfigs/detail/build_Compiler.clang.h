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

#include <nn/TargetConfigs/build_Base.h>

#if !defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
    #error   "ツールチェイン種別として CLANG が指定されていません。"
#endif

#define NN_BUILD_CONFIG_COMPILER_CLANG            1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_CLANG    1

#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_STATIC_ASSERT    1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_STRONG_ENUMS     1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE        1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR          1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_UNICODE_LITERALS 1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_CHRONO       1
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_TYPE_TRAITS  1
