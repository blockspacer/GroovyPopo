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
* @brief GPU により読み書きされるバッファーのレイアウトのデータに関する API の宣言（NVN8 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>

#include <nn/gfx/detail/gfx_Declare.h>

namespace nn {
namespace gfx {

/**
* @brief TimestampBuffer を表す構造体です（NVN8 特殊版）
*/
template<>
struct TimestampBufferImplData< ApiVariationNvn8 >
{
    char reserved[ 8 ]; //!< 予約領域です。
    uint64_t value; //!< タイムスタンプの値です
};

/**
* @brief QueryBuffer を表す構造体です（NVN8 特殊版）
*/
template<>
struct QueryBufferImplData< ApiVariationNvn8 >
{
    uint64_t value; //!< クエリー結果の値です
    uint64_t timestamp; //!< クエリー結果が書き込まれたときのタイムスタンプの値です
};

}
}
