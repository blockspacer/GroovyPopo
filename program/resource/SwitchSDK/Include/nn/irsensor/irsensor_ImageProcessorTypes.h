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
 * @brief イメージプロセッサに関する型の宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

namespace nn { namespace irsensor {

const int IrCameraIntensityMax = 255; //!< IR カメラの輝度の最大値です。

/**
 * @brief 矩形です。ピクセル単位の範囲を表す場合に使用します。
 */
struct Rect
{
    int16_t x;      //!< X 座標
    int16_t y;      //!< Y 座標
    int16_t width;  //!< 幅
    int16_t height; //!< 高
};

/**
 * @brief イメージプロセッサの状態です。
 */
enum ImageProcessorStatus
{
    ImageProcessorStatus_Stopped, //!< イメージプロセッサは停止状態です。
    ImageProcessorStatus_Running, //!< イメージプロセッサは稼動状態です。
};


}} // namespace nn::irsensor
