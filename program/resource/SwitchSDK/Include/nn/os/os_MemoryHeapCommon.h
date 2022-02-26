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
 * @brief   メモリヒープ機能に関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_Config.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   メモリヒープ領域の最小単位を格納した定数です。
 */
const size_t    MemoryHeapUnitSize = 2 * 1024 * 1024;

//--------------------------------------------------------------------------
/**
 * @brief   メモリブロックの最小単位を格納した定数です。
 */
const size_t    MemoryBlockUnitSize = 2 * 1024 * 1024;

//--------------------------------------------------------------------------
/**
 * @brief   メモリ操作の最小単位を格納した定数です。
 */
const size_t    MemoryPageSize = 4 * 1024;


}} // namespace nn::os

