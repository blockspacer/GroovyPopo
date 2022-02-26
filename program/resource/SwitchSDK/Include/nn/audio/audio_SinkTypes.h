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

#include <nn/audio/audio_SampleFormat.h>

namespace nn {
namespace audio {

// 前方宣言
struct SinkInfo;

/**
 * @brief       レンダリング結果をオーディオ出力デバイスに出力するための Sink です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AddDeviceSink()
 * - @ref RemoveDeviceSink()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct DeviceSinkType
{
    /**
     * @brief    アプリケーション独自のダウンミックス処理に用いるパラメータです。
     * @details
     * coeff 配列のプラットフォーム毎に設定すべき値と配列中の位置との関係は @confluencelink{89991492, ドキュメントの DeviceSinkType の説明} を参照してください。
     */
    struct DownMixParameter
    {
        static const int CoeffCount = 16;   //!< ダウンミックスの計算に用いる係数の個数
        float coeff[CoeffCount];            //!< ダウンミックスの計算に用いる係数列
    };

    SinkInfo* _handle;
};

/**
 * @brief       レンダリング結果をメモリに出力するための Sink です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AddCircularBufferSink()
 * - @ref RemoveCircularBufferSink()
 * - @ref GetRequiredBufferSizeForCircularBufferSink()
 * - @ref ReadCircularBufferSink()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct CircularBufferSinkType
{
    SinkInfo* _handle;
};

}  // namespace audio
}  // namespace nn
