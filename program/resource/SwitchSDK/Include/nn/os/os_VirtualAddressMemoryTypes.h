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
 * @brief   仮想アドレスメモリ管理機能に関する型の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>
#include <nn/os/detail/os_MacroImpl.h>

namespace nn { namespace os {

//! @name 仮想アドレスメモリ管理関連 API
//! @{

//-----------------------------------------------------------------------------
/**
 * @brief   仮想アドレスメモリ管理用のリソース消費量を格納する構造体です。
 *
 * @details
 *  詳細は nn::os::GetVirtualAddressMemoryResourceUsage を参照してください。
 */
struct VirtualAddressMemoryResourceUsage
{
    size_t  assignedSize;   //!< 使用できるリソース量が格納されます。
    size_t  usedSize;       //!< 使用中のリソース量が格納されます。
};

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::os

