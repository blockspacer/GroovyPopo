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

#if defined(NN_BUILD_CONFIG_CPU_X86)
    #include <nn/TargetConfigs/detail/build_Cpu.x86.h>
#elif defined(NN_BUILD_CONFIG_CPU_X64)
    #include <nn/TargetConfigs/detail/build_Cpu.x64.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A9)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a9.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A15)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a15.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A7)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a7.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A53_AARCH32)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a53_aarch32.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A53_AARCH64)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a53_aarch64.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A57_AARCH32)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a57_aarch32.h>
#elif defined(NN_BUILD_CONFIG_CPU_CORTEX_A57_AARCH64)
    #include <nn/TargetConfigs/detail/build_Cpu.cortex_a57_aarch64.h>
#elif defined(NN_BUILD_CONFIG_CPU_PPC)
    // 必要があれば何かインクルードする
#else
    #error   "未サポートのCPUが指定されています。"
#endif

