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

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/TargetConfigs/build_Cpu.h>
#include <nn/TargetConfigs/build_Fpu.h>
#if !defined(NN_BUILD_CONFIG_COMPILER_GHS)
    #include <cstdint>
#endif

namespace nn
{

#if defined(NN_BUILD_CONFIG_COMPILER_GHS)
    typedef uint8_t     Bit8;
    typedef uint16_t    Bit16;
    typedef uint32_t    Bit32;
    typedef uint64_t    Bit64;
#else
    typedef std::uint8_t     Bit8;
    typedef std::uint16_t    Bit16;
    typedef std::uint32_t    Bit32;
    typedef std::uint64_t    Bit64;
#endif

#if defined(NN_BUILD_CONFIG_COMPILER_GCC) || defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    #if     defined(NN_BUILD_CONFIG_CPU_ARM64)
        typedef unsigned __int128   Bit128;
    #else
        typedef uint64_t Bit128 __attribute__((__vector_size__ (16)));
    #endif
#endif



}   // namespace nn

