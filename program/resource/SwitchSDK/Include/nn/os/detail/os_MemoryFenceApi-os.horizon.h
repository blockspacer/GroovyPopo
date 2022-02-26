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
 * @brief   OS-API 内部実装用のメモリフェンスの定義（Horizon 版）
 */

#pragma once

#include <nn/os/os_Config.h>

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#if defined(NN_BUILD_CONFIG_CPU_ARM_V7A)
    #define NN_OS_DMB_ISHST()   asm volatile ("dmb ishst" : : : "memory")
    #define NN_OS_DMB_ISHLD()   asm volatile ("dmb ish"   : : : "memory")
    #define NN_OS_DMB_ISH()     asm volatile ("dmb ish"   : : : "memory")
#elif defined(NN_BUILD_CONFIG_CPU_ARM_V8A)
    #define NN_OS_DMB_ISHST()   asm volatile ("dmb ishst" : : : "memory")
    #define NN_OS_DMB_ISHLD()   asm volatile ("dmb ishld" : : : "memory")
    #define NN_OS_DMB_ISH()     asm volatile ("dmb ish"   : : : "memory")
#elif defined(NN_BUILD_CONFIG_CPU_ARM)
    #define NN_OS_DMB_ISHST()   asm volatile ("dmb" : : : "memory")
    #define NN_OS_DMB_ISHLD()   asm volatile ("dmb" : : : "memory")
    #define NN_OS_DMB_ISH()     asm volatile ("dmb" : : : "memory")
#else
    #error "指定されたビルド対象の CPU が ARM 系ではありません。"
#endif

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------

NN_FORCEINLINE void FenceMemoryStoreStore() NN_NOEXCEPT
{
    NN_OS_DMB_ISHST();
}

NN_FORCEINLINE void FenceMemoryStoreLoad() NN_NOEXCEPT
{
    NN_OS_DMB_ISH();
}

NN_FORCEINLINE void FenceMemoryStoreAny() NN_NOEXCEPT
{
    NN_OS_DMB_ISH();
}

NN_FORCEINLINE void FenceMemoryLoadStore() NN_NOEXCEPT
{
    NN_OS_DMB_ISHLD();
}

NN_FORCEINLINE void FenceMemoryLoadLoad() NN_NOEXCEPT
{
    NN_OS_DMB_ISHLD();
}

NN_FORCEINLINE void FenceMemoryLoadAny() NN_NOEXCEPT
{
    NN_OS_DMB_ISHLD();
}

NN_FORCEINLINE void FenceMemoryAnyStore() NN_NOEXCEPT
{
    NN_OS_DMB_ISH();
}

NN_FORCEINLINE void FenceMemoryAnyLoad() NN_NOEXCEPT
{
    NN_OS_DMB_ISH();
}

NN_FORCEINLINE void FenceMemoryAnyAny() NN_NOEXCEPT
{
    NN_OS_DMB_ISH();
}

//--------------------------------------------------------------------------

}   // namespace detail
}}  // namespace nn::os

