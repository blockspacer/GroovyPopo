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

#include <nn/audio/audio_EffectTypes.h>

namespace nn { namespace audio {

/**
 * @brief       nn::audio::AuxType に適用する I3dl2Reverb エフェクトのユーティリティです。
 *
 * @details
 * nn::audio::AuxType によって得られたサンプルデータを入力として I3dl2Reverb エフェクト処理を行うユーティリティです。@n
 * nn::audio::ReadAuxSendBuffer() によって得られたサンプルデータを、このエフェクトへの入力に指定することで、
 * Interactive 3D Audio Level 2 (I3DL2) 仕様に部分的に準拠した環境リバーブエフェクトを出力します。
 * このエフェクトによって施される I3dl2Reverb エフェクトは nn::audio::I3dl2ReverbType による処理と同一の内容です。@n
 * ただしこのエフェクト処理は nn::audio::ProcessAuxI3dl2Reverb() を呼び出した CPU スレッド上で処理されます。@n
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (ワークバッファサイズ取得)
 * - @ref GetRequiredBufferSizeForAuxI3dl2Reverb()
 *
 * (初期化関連)
 * - @ref InitializeAuxI3dl2Reverb()
 * - @ref ResetAuxI3dl2Reverb()
 *
 * (接続先設定)
 * - @ref SetAuxI3dl2ReverbInputOutput()
 * - @ref GetAuxI3dl2ReverbInputOutput()
 *
 * (パラメータ)
 * - @ref SetAuxI3dl2ReverbParameters()
 * - @ref GetAuxI3dl2ReverbParameters()
 *
 * (エフェクト処理)
 * - @ref ProcessAuxI3dl2Reverb()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AuxI3dl2ReverbType
{
    void* _workBuffer;     // AuxI3dl2ReverbType が利用するワークバッファ
    size_t _bufferSize;    // workBuffer に指定されたバッファのサイズ
};

/**
 * @brief       nn::audio::AuxType に適用する Reverb エフェクトのユーティリティです。
 *
 * @details
 * nn::audio::AuxType によって得られたサンプルデータを入力として Reverb エフェクト処理を行うユーティリティです。@n
 * nn::audio::ReadAuxSendBuffer() によって得られたサンプルデータを、このエフェクトへの入力に指定することで、
 * 閉じた空間内の音の響きをエミュレートする Reverb エフェクトを出力します。
 * このエフェクトによって施される Reverb エフェクトは nn::audio::ReverbType による処理と同一の内容です。@n
 * ただしこのエフェクト処理は nn::audio::ProcessAuxReverb() を呼び出した CPU スレッド上で処理されます。@n
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (ワークバッファサイズ取得)
 * - @ref GetRequiredBufferSizeForAuxReverb()
 *
 * (初期化関連)
 * - @ref InitializeAuxReverb()
 * - @ref ResetAuxReverb()
 *
 * (接続先設定)
 * - @ref SetAuxReverbInputOutput()
 * - @ref GetAuxReverbInputOutput()
 *
 * (パラメータ)
 * - @ref SetAuxReverbParameters()
 * - @ref GetAuxReverbParameters()
 *
 * (エフェクト処理)
 * - @ref ProcessAuxReverb()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AuxReverbType
{
    void* _workBuffer;     // AuxReverbType が利用するワークバッファ
    size_t _bufferSize;    // workBuffer に指定されたバッファのサイズ
};

/**
 * @brief       nn::audio::AuxType に適用する Delay エフェクトのユーティリティです。
 *
 * @details
 * nn::audio::AuxType によって得られたサンプルデータを入力として Delay エフェクト処理を行うユーティリティです。@n
 * nn::audio::ReadAuxSendBuffer() によって得られたサンプルデータを、このエフェクトへの入力に指定することで、
 * 閉じた空間内の音の響きをエミュレートする Delay エフェクトを出力します。
 * このエフェクトによって施される Delay エフェクトは nn::audio::DelayType による処理と同一の内容です。@n
 * ただしこのエフェクト処理は nn::audio::ProcessAuxDelay() を呼び出した CPU スレッド上で処理されます。@n
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (ワークバッファサイズ取得)
 * - @ref GetRequiredBufferSizeForAuxDelay()
 *
 * (初期化関連)
 * - @ref InitializeAuxDelay()
 * - @ref ResetAuxDelay()
 *
 * (接続先設定)
 * - @ref SetAuxDelayInputOutput()
 * - @ref GetAuxDelayInputOutput()
 *
 * (パラメータ)
 * - @ref GetAuxDelayChannelCountMax()
 * - @ref SetAuxDelayParameters()
 * - @ref GetAuxDelayParameters()
 * - @ref GetAuxDelayTimeMax()
 *
 * (エフェクト処理)
 * - @ref ProcessAuxDelay()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AuxDelayType
{
    void* _workBuffer;     // AuxDelayType が利用するワークバッファ
    size_t _bufferSize;    // workBuffer に指定されたバッファのサイズ
};

/**
 * @brief       nn::audio::AuxType に適用する Limiter エフェクトのユーティリティです。
 *
 * @details
 * nn::audio::AuxType によって得られたサンプルデータを入力として Limiter エフェクト処理を行うユーティリティです。@n
 * nn::audio::ReadAuxSendBuffer() によって得られたサンプルデータを、このエフェクトへの入力に指定することで、
 * サンプル値を一定の値域に制限する Limiter エフェクトを出力します。
 * このエフェクト処理は nn::audio::ProcessAuxLimiter() を呼び出した CPU スレッド上で処理されます。@n
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (ワークバッファサイズ取得)
 * - @ref GetRequiredBufferSizeForAuxLimiter()
 *
 * (初期化関連)
 * - @ref InitializeAuxLimiter()
 * - @ref ResetAuxLimiter()
 *
 * (接続先設定)
 * - @ref SetAuxLimiterInputOutput()
 * - @ref GetAuxLimiterInputOutput()
 *
 * (パラメータ)
 * - @ref GetAuxLimiterChannelCountMax()
 * - @ref SetAuxLimiterParameters()
 * - @ref GetAuxLimiterParameters()
 * - @ref GetAuxLimiterAttackTimeMax()
 * - @ref GetAuxLimiterReleaseTimeMax()
 *
 * (エフェクト処理)
 * - @ref ProcessAuxLimiter()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AuxLimiterType
{
    void* _workBuffer;     // AuxLimiterType が利用するワークバッファ
    size_t _bufferSize;    // workBuffer に指定されたバッファのサイズ
};

}} // namespace nn::audio
