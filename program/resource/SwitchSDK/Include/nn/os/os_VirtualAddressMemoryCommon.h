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
 * @brief   仮想アドレスメモリ管理機能に関する定数などの宣言
 */

#pragma once

#include <nn/os/os_Config.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   アドレス領域の先頭アドレスが満たすアライメント制約を表す定数です。
 */
const size_t    AddressRegionAlignment = 64 * 1024;


}} // namespace nn::os

