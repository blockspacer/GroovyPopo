/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   OS-API ライブラリ共通の config 入力
 */

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/TargetConfigs/build_Cpu.h>
#include <nn/TargetConfigs/build_Fpu.h>
#include <nn/TargetConfigs/build_Os.h>

//  ARM 系の場合は組合せ判別用のマクロを独自に追加定義
#if defined(NN_BUILD_CONFIG_CPU_ARM_V7A)
        // ARMv7A 環境でユーザランドが AArch32 動作（ILP32 のみ）
        #define NN_OS_CPU_ARM_AARCH32
        #define NN_OS_CPU_ARM_AARCH32_ARMV7A
#elif defined(NN_BUILD_CONFIG_CPU_ARM_V8A)
    #if defined(NN_BUILD_CONFIG_CPU_ARM)
        // ARMv8A 環境でユーザランドが AArch32 動作（ILP32 のみ）
        #define NN_OS_CPU_ARM_AARCH32
        #define NN_OS_CPU_ARM_AARCH32_ARMV8A
    #elif defined(NN_BUILD_CONFIG_CPU_ARM64)
        // ARMv8A 環境でユーザランドが AArch64 動作（ILP32/LP64 両方）
        #define NN_OS_CPU_ARM_AARCH64
        #define NN_OS_CPU_ARM_AARCH64_ARMV8A
    #endif
#endif

