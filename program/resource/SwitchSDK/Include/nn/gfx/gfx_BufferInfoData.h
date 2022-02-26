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
* @brief バッファー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief バッファーを初期化するための情報を表す構造体です。
*/
struct BufferInfoData
{
    uint32_t size; //!< バイトでのバッファーサイズです
    Bit32 gpuAccessFlag; //!< nn::gfx::GpuAccess の組み合わせによって表される、GPU がアクセスする方法です

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief バッファーテクスチャービューを初期化するための情報を表す構造体です。
*/
struct BufferTextureViewInfoData
{
    Bit32 format; //!< nn::gfx::ImageFormat によって表される、バッファーの内容を解釈するイメージフォーマットです。
    uint32_t offset; //!< バッファーの先頭からのバイトでのオフセットです。
    uint32_t size; //!< 参照するバッファーのバイトでのサイズです。
    char reserved2[ 4 ]; //!< 予約領域です。
    detail::Ptr< const void > pBuffer; //!< 参照するバッファーへのポインターです。

    char reserved[ 8 ];
};

}
}
