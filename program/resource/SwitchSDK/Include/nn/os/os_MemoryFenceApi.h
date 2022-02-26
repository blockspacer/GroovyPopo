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
 * @brief   メモリフェンス機能に関する API の宣言
 */

#pragma once

#include <nn/os/os_Config.h>

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>


#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_MemoryFenceApi-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_MemoryFenceApi-os.horizon.h>
#endif


namespace nn { namespace os {

//! @name メモリフェンス関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   Store-Store メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Store-Store メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryStoreStore() NN_NOEXCEPT
{
    detail::FenceMemoryStoreStore();
}

//--------------------------------------------------------------------------
/**
 * @brief   Store-Load メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Store-Load メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryStoreLoad() NN_NOEXCEPT
{
    detail::FenceMemoryStoreLoad();
}

//--------------------------------------------------------------------------
/**
 * @brief   Store-Any メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Store-Any メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryStoreAny() NN_NOEXCEPT
{
    detail::FenceMemoryStoreAny();
}

//--------------------------------------------------------------------------
/**
 * @brief   Load-Store メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Load-Store メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryLoadStore() NN_NOEXCEPT
{
    detail::FenceMemoryLoadStore();
}

//--------------------------------------------------------------------------
/**
 * @brief   Load-Load メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Load-Load メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryLoadLoad() NN_NOEXCEPT
{
    detail::FenceMemoryLoadLoad();
}

//--------------------------------------------------------------------------
/**
 * @brief   Load-Any メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Load-Any メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryLoadAny() NN_NOEXCEPT
{
    detail::FenceMemoryLoadAny();
}

//--------------------------------------------------------------------------
/**
 * @brief   Any-Store メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Any-Store メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryAnyStore() NN_NOEXCEPT
{
    detail::FenceMemoryAnyStore();
}

//--------------------------------------------------------------------------
/**
 * @brief   Any-Load メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Any-Load メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryAnyLoad() NN_NOEXCEPT
{
    detail::FenceMemoryAnyLoad();
}

//--------------------------------------------------------------------------
/**
 * @brief   Any-Any メモリフェンスを挿入します。
 *
 * @details
 *  本 API 呼び出しが配置された場所に Any-Any メモリフェンスを挿入します。
 *  詳細は @ref PageNotificationOsMemoryFence を参照して下さい。
 *
 */
NN_FORCEINLINE void FenceMemoryAnyAny() NN_NOEXCEPT
{
    detail::FenceMemoryAnyAny();
}


//! @}

}} // namespace nn::os

