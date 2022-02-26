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
 * @brief   OS-API 内部実装用のメモリフェンスの定義（Windows 版）
 */

#pragma once

#include <intrin.h>
#include <nn/os/os_Config.h>

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#define NN_OS_DETAIL_SFENCE()   do { _WriteBarrier(); _mm_sfence(); } while (NN_STATIC_CONDITION(0))
#define NN_OS_DETAIL_LFENCE()   do { _WriteBarrier(); _mm_lfence(); } while (NN_STATIC_CONDITION(0))
#define NN_OS_DETAIL_MFENCE()   do { _WriteBarrier(); _mm_mfence(); } while (NN_STATIC_CONDITION(0))

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------

NN_FORCEINLINE void FenceMemoryStoreStore() NN_NOEXCEPT
{
    NN_OS_DETAIL_SFENCE();
}

NN_FORCEINLINE void FenceMemoryStoreLoad() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

NN_FORCEINLINE void FenceMemoryStoreAny() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

NN_FORCEINLINE void FenceMemoryLoadStore() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

NN_FORCEINLINE void FenceMemoryLoadLoad() NN_NOEXCEPT
{
    NN_OS_DETAIL_LFENCE();
}

NN_FORCEINLINE void FenceMemoryLoadAny() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

NN_FORCEINLINE void FenceMemoryAnyStore() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

NN_FORCEINLINE void FenceMemoryAnyLoad() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

NN_FORCEINLINE void FenceMemoryAnyAny() NN_NOEXCEPT
{
    NN_OS_DETAIL_MFENCE();
}

//--------------------------------------------------------------------------

}   // namespace detail
}}  // namespace nn::os

