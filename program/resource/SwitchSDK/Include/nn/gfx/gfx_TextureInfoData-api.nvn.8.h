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
* @brief テクスチャー初期化用の情報のデータに関する API の宣言（NVN8 特殊版）
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
* @brief テクスチャーを初期化するための情報を表す構造体です（NVN8 特殊版）。
*/
template<>
struct TextureInfoImplData< ApiVariationNvn8 >
{
    NVNtextureBuilder nvnTextureBuilder; //!< nvn のテクスチャビルダーです
    Bit8 imageStorageDimension; //!< nn::gfx::ImageStorageDimension で表される、イメージデータの次元です
    char reserved; //!< 予約領域です
    uint16_t multisampleCount; //!< マルチサンプルの数です
    Bit16 tileMode; //!< nn::gfx::TileMode で表される、タイルモードです
    char reserved2[ 2 ]; //!< 予約領域です
    uint32_t arrayLength; //!< 配列の長さです
    Bit32 gpuAccessFlags; //!< nn::gfx::GpuAccess の組み合わせで表される、GPU がアクセスする方法です
    Bit32 imageFormat; //!< nn::gfx::ImageFormat で表される、イメージフォーマットです
    uint32_t width; //!< 幅です
    uint32_t height; //!< 高さです
    uint32_t depth; //!< 深さです
};

}
}
