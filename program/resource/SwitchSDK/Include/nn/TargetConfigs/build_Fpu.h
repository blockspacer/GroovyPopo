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

#if defined(NN_BUILD_CONFIG_FPU_FP_ARM64)
    #include <nn/TargetConfigs/detail/build_Fpu.fp_arm64.h>
#elif defined(NN_BUILD_CONFIG_FPU_FP_ARMV8)
    #include <nn/TargetConfigs/detail/build_Fpu.fp_armv8.h>
#elif defined(NN_BUILD_CONFIG_FPU_VFPV4_D32)
    #include <nn/TargetConfigs/detail/build_Fpu.vfpv4_d32.h>
#elif defined(NN_BUILD_CONFIG_FPU_VFPV3_D32)
    #include <nn/TargetConfigs/detail/build_Fpu.vfpv3_d32.h>
#elif defined(NN_BUILD_CONFIG_FPU_X87)
    #include <nn/TargetConfigs/detail/build_Fpu.x87.h>
#elif defined(NN_BUILD_CONFIG_CPU_PPC)
    // 必要があれば何かインクルードする
#else
    #error   "未サポートの CPU もしくは FPU が指定されています。"
#endif

