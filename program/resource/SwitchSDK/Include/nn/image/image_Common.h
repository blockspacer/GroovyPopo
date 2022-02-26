/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/nn_Common.h>

/**
    @file
    @brief      画像の取り扱いに関するデータ型の宣言
    @details    nn::image 名前空間共通の種々のデータ型を定義します。
*/

/**
    @namespace  nn::image
    @brief      画像データを取り扱う名前空間です。
*/

namespace nn { namespace image {

/**
    @brief      画像の汎用的な寸法を表す構造体
*/
struct Dimension
{
    std::int32_t width;   //!< 画像の幅
    std::int32_t height;  //!< 画像の高さ
};

/**
    @brief      画像データのピクセルフォーマットを表す列挙子
*/
enum PixelFormat
{
    PixelFormat_Rgba32, //!< RGBA各8bitで表現された計32bitのピクセルデータ
    PixelFormat_Rgb24,  //!< RGB各8bitで表現された計24bitのピクセルデータ
};

}}
