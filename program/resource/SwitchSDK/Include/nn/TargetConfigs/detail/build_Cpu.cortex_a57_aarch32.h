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

#if !defined(NN_BUILD_CONFIG_CPU_CORTEX_A57_AARCH32)
    #error   "CPU種別として Cortex-A57(AArch32) が指定されていません。"
#endif

#define NN_BUILD_CONFIG_CPU_ARM               1
#define NN_BUILD_CONFIG_CPU_ARM_V8A           1
#define NN_BUILD_CONFIG_CPU_ARM_VE            1
#define NN_BUILD_CONFIG_CPU_CORTEX_A57        1
#define NN_BUILD_CONFIG_CPU_SVC_32            1

