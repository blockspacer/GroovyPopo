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
 * @brief   OS-API 内部実装用のファイバー型の宣言（Horizon 版）
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>

namespace nn { namespace os { namespace detail {

//-----------------------------------------------------------------------------

struct FiberContextForHorizon
{
#if defined(NN_BUILD_CONFIG_CPU_ARM64)
    union
    {
        struct
        {
            Bit64   reg_x0;     // 1st argument
            Bit64   reg_x19;
            Bit64   reg_x20;
            Bit64   reg_x21;
            Bit64   reg_x22;
            Bit64   reg_x23;
            Bit64   reg_x24;
            Bit64   reg_x25;
            Bit64   reg_x26;
            Bit64   reg_x27;
            Bit64   reg_x28;
            Bit64   reg_x29;
            Bit64   reg_x30;    // reg_lr = entry point
            Bit64   reg_padding;

            Bit128  reg_q8;
            Bit128  reg_q9;
            Bit128  reg_q10;
            Bit128  reg_q11;
            Bit128  reg_q12;
            Bit128  reg_q13;
            Bit128  reg_q14;
            Bit128  reg_q15;
        };
        Bit64       regs[30];
    };
#elif defined(NN_BUILD_CONFIG_CPU_ARM)
    union
    {
        struct
        {
            Bit32   reg_r0;     // 1st argument
            Bit32   reg_r4;
            Bit32   reg_r5;
            Bit32   reg_r6;
            Bit32   reg_r7;
            Bit32   reg_r8;
            Bit32   reg_r9;
            Bit32   reg_r10;
            Bit32   reg_r11;
            Bit32   reg_lr;     // entry point

            Bit64   reg_d8;
            Bit64   reg_d9;
            Bit64   reg_d10;
            Bit64   reg_d11;
            Bit64   reg_d12;
            Bit64   reg_d13;
            Bit64   reg_d14;
            Bit64   reg_d15;
        };
        Bit64       regs[13];
    };
#else
    #error "*** Invalid NN_BUILD_CONFIG_CPU_"
#endif
};

//-----------------------------------------------------------------------------

struct FiberTypeImplByHorizon
{
    FiberContextForHorizon* _sp;
};

typedef FiberTypeImplByHorizon  FiberTypeImpl;

//-----------------------------------------------------------------------------

}}}  // namespace nn::os::detail

