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

#include <nn/os/os_SystemEvent.h>
#include <nn/audio/audio_Result.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ
 * @{
 */

/**
 * @brief       オーディオレンダラパラメータを初期化します。
 * @param[out]  pOutParameter           パラメータ
 * @pre
 * - pOutParameter != nullptr
 * @post
 * - pOutParameter->sampleRate == 48000
 * - pOutParameter->sampleCount == 240
 * - pOutParameter->mixBufferCount == 1
 * - pOutParameter->subMixCount == 0
 * - pOutParameter->voiceCount == 1
 * - pOutParameter->sinkCount == 1
 * - pOutParameter->effectCount == 0
 * - pOutParameter->performanceFrameCount == 0
 * - pOutParameter->isVoiceDropEnabled == false
 * - pOutParameter->executionMode == nn::audio::AudioRendererExecutionMode_AutoExecution
 */
void InitializeAudioRendererParameter(AudioRendererParameter* pOutParameter) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラパラメータが有効なものかどうかを確認します。
 * @param[in]   parameter               パラメータ
 * @return      parameter が許容される場合は true を、そうでなければ false を返します。
 */
bool IsValidAudioRendererParameter(const AudioRendererParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       AudioRendererConfig の初期化に必要なワークバッファのサイズを取得します。
 * @param[in]   parameter               パラメータ
 * @return      ワークバッファのサイズを返します。
 * @pre
 * - @ref nn::audio::IsValidAudioRendererParameter "nn::audio::IsValidAudioRendererParameter(parameter)" == true
 */
size_t GetAudioRendererConfigWorkBufferSize(const AudioRendererParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       AudioRendererConfig を初期化します。
 * @param[out]  pOutConfig              AudioRendererConfig
 * @param[in]   parameter               パラメータ
 * @param[in]   buffer                  ワークバッファ
 * @param[in]   size                    ワークバッファサイズ
 * @return      処理の結果を返します。
 * @pre
 * - pOutConfig != nullptr
 * - @ref nn::audio::IsValidAudioRendererParameter "nn::audio::IsValidAudioRendererParameter(parameter)" == true
 * - buffer != nullptr
 * - buffer は nn::os::MemoryPageSize にアラインされている
 * - size >= @ref nn::audio::GetAudioRendererConfigWorkBufferSize "nn::audio::GetAudioRendererConfigWorkBufferSize(parameter)"
 */
void InitializeAudioRendererConfig(AudioRendererConfig* pOutConfig, const AudioRendererParameter& parameter, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラ利用時の排他漏れ検知機能の有効・無効を設定します。
 * @param[in]   enabled                  有効か無効か
 * @pre         開かれたオーディオレンダラが存在しない
 * @post        @ref nn::audio::IsAudioRendererExclusiveControlLeakageCheckEnabled "nn::audio::IsAudioRendererExclusiveControlLeakageCheckEnabled()" == enabled
 */
void SetAudioRendererExclusiveControlLeakageCheckEnabled(bool enabled) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラ利用時の排他漏れ検知機能の有効・無効を取得します。
 * @return      有効な場合は true を、そうでなければ false を返します。
 */
bool IsAudioRendererExclusiveControlLeakageCheckEnabled() NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラの経過フレーム数を取得します。
 * @param[in]   pConfig                 AudioRendererConfig
 * @return      経過フレーム数を返します。
 * @pre
 * - pConfig != nullptr
 */
int64_t GetAudioRendererElapsedFrameCount(const AudioRendererConfig* pConfig) NN_NOEXCEPT;

/**
 * @}
 */
}  // namespace audio
}  // namespace nn
