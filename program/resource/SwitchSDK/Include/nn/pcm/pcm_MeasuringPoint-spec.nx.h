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

namespace nn {
namespace pcm {

/**
 * @brief 電力計測ポイント識別子を表す型です。
 *
 * @details PCM ライブラリで現在対応している電力計測ポイント識別子の一覧です。
 */
enum MeasuringPoint
{
    MeasuringPoint_Gpu          = 0, //!< メイン GPU における電力を計測するポイントです。
    MeasuringPoint_Cpu          = 1, //!< メイン CPU における電力を計測するポイントです。
    MeasuringPoint_Ddr          = 2, //!< メイン DDR における電力を計測するポイントです。

    // 以下は SDK 内部開発者用の定義です。
    // アプリケーションのチューニングにおいて参照する必要はありません。

    MeasuringPoint_Backlight    = 3, // 本体画面のバックライトにおける電力を計測するポイントです。
    MeasuringPoint_VsysAp       = 4, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_Vdd          = 5, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_VsysCore     = 6, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_Soc1V8       = 7, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_Lpddr1V8     = 8, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_Reg1V32      = 9, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_Vdd3V3Sys    = 10, // ボード内の一部分の電力を計測するポイントです。
    MeasuringPoint_VddDdr0V6    = 11, // ボード内の一部分の電力を計測するポイントです。

    MeasuringPoint_Number,
};

} //pcm
} //nn
