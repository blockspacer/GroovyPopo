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
 * @brief メモリープール初期化用の情報のデータに関する API の宣言（NVN8 特殊版）
 */

#pragma once

#include <nn/nn_Common.h>

#include <nn/util/util_BitPack.h>

#include <nvn/nvn.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>

#include <nn/gfx/detail/gfx_Common-api.nvn.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief メモリープールを初期化するための情報を表す構造体です（NVN8 特殊版）。
*/
template<>
struct MemoryPoolInfoImplData< ApiVariationNvn8 >
{
    NVNmemoryPoolBuilder nvnMemoryPoolBuilder; //!< nvn のメモリープールです
};

}
}
