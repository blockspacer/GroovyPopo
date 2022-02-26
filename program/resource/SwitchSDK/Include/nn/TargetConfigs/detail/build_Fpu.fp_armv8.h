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

#if !defined(NN_BUILD_CONFIG_FPU_FP_ARMV8)
    #error   "FPU種別として VFP ARMv8 が指定されていません。"
#endif

#define NN_BUILD_CONFIG_FPU_VFP                     1
#define NN_BUILD_CONFIG_FPU_NEON                    1
#define NN_BUILD_CONFIG_FPU_NUM_DOUBLE_REGISTERS    32

