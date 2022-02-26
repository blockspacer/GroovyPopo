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

#include <nn/audio.h>
#include <nn/os.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ
 * @{
 */

/**
 * @brief       オーディオレンダラを取得します。
 * @param[out]  outHandle               オーディオレンダラハンドル
 * @param[in]   parameter               パラメータ
 * @param[in]   workBuffer              ワークバッファ
 * @param[in]   workBufferSize          ワークバッファサイズ
 * @retresult
 *   @handleresult{nn::audio::ResultNoAudioDeviceFound}
 *   @handleresult{nn::audio::ResultOutOfResource}
 *   @handleresult{nn::audio::ResultInsufficientBuffer}
 * @endretresult
 * @pre
 * - outHandle != nullptr
 * - @ref nn::audio::IsValidAudioRendererParameter "nn::audio::IsValidAudioRendererParameter(parameter)" == true
 * - workBuffer != nullptr
 * - workBuffer が nn::os::MemoryPageSize にアラインしている
 * - workBufferSize が nn::os::MemoryPageSize の整数倍である
 * - workBufferSize >= @ref nn::audio::GetAudioRendererWorkBufferSize "nn::audio::GetAudioRendererWorkBufferSize(parameter)"
 * @post
 * - *outHandle に有効なハンドルが格納される
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Stopped
 */
nn::Result OpenAudioRenderer(AudioRendererHandle* outHandle, const AudioRendererParameter& parameter, void* workBuffer, size_t workBufferSize) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラを取得し、通知用同期オブジェクトを初期化します。
 * @param[out]  outHandle               オーディオレンダラハンドル
 * @param[out]  pOutSystemEvent         同期用 SystemEvent
 * @param[in]   parameter               パラメータ
 * @param[in]   workBuffer              ワークバッファ
 * @param[in]   workBufferSize          ワークバッファサイズ
 * @retresult
 *   @handleresult{nn::audio::ResultNoAudioDeviceFound}
 *   @handleresult{nn::audio::ResultOutOfResource}
 *   @handleresult{nn::audio::ResultInsufficientBuffer}
 * @endretresult
 * @pre
 * - outHandle != nullptr
 * - pOutSystemEvent != nullptr
 * - pOutSystemEvent は未初期化
 * - @ref nn::audio::IsValidAudioRendererParameter "nn::audio::IsValidAudioRendererParameter(parameter)" == true
 * - workBuffer != nullptr
 * - workBuffer が nn::os::MemoryPageSize にアラインしている
 * - workBufferSize が nn::os::MemoryPageSize の整数倍である
 * - workBufferSize >= @ref nn::audio::GetAudioRendererWorkBufferSize "nn::audio::GetAudioRendererWorkBufferSize(parameter)"
 * @post
 * - *outHandle に有効なハンドルが格納される
 * - *pOutSystemEvent が初期化される
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Stopped
 */
nn::Result OpenAudioRenderer(AudioRendererHandle* outHandle, nn::os::SystemEvent* pOutSystemEvent, const AudioRendererParameter& parameter, void* workBuffer, size_t workBufferSize) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラを破棄します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @pre
 * - handle は有効なもの
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Stopped
 * @post
 * - handle は無効になる
 */
void CloseAudioRenderer(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラによるレンダリングを開始します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @retresult
 * @endretresult
 * @pre
 * - handle は有効なもの
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Stopped
 * @post
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Started
 */
nn::Result StartAudioRenderer(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラによるレンダリングを停止します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @pre
 * - handle は有効なもの
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Started
 * @post
 * - @ref nn::audio::GetAudioRendererState "nn::audio::GetAudioRendererState(handle)" == nn::audio::AudioRendererState_Stopped
 */
void StopAudioRenderer(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラの処理サンプルレートを取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      処理サンプルレートを返します。
 * @pre
 * - handle は有効なもの
 */
int GetAudioRendererSampleRate(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラのミックスバッファのサンプル数を取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      フレームサンプル数を返します。
 * @pre
 * - handle は有効なもの
 */
int GetAudioRendererSampleCount(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラのミックスバッファ数を取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      内部バッファ数を返します。
 * @pre
 * - handle は有効なもの
 */
int GetAudioRendererMixBufferCount(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラの状態を取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      状態を返します。
 * @pre
 * - handle は有効なもの
 */
AudioRendererState GetAudioRendererState(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラに紐づく Voice などのパラメータを更新します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @param[in]   pConfig                 AudioRendererConfig
 * @retresult
 *   @handleresult{nn::audio::ResultInvalidUpdateInfo}
 *   @handleresult{nn::audio::ResultNoMemoryPoolEntry}
 * @endretresult
 * @pre
 * - handle は有効なもの
 * - pOutConfig != nullptr
 */
nn::Result RequestUpdateAudioRenderer(AudioRendererHandle handle, const AudioRendererConfig* pConfig) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラを開くのに必要なワークバッファのサイズを取得します。
 * @param[in]   parameter               パラメータ
 * @return      ワークバッファのサイズを返します。
 * @pre
 * - @ref nn::audio::IsValidAudioRendererParameter "nn::audio::IsValidAudioRendererParameter(parameter)" == true
 */
size_t GetAudioRendererWorkBufferSize(const AudioRendererParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラに割り当てるレンダリング時間の上限を指定します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @param[in]   limitPercent            オーディオレンダラに割り当てるレンダリング時間の上限を指定します。単位はパーセントです。
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 * @endretresult
 * @pre
 * - handle は有効なもの
 * - limitPercent >= 0 && limitPercent <= 100
 * @post
 * - @ref nn::audio::GetAudioRendererRenderingTimeLimit "nn::audio::GetAudioRendererRenderingTimeLimit(handle)" == limitPercent
 */
nn::Result SetAudioRendererRenderingTimeLimit(AudioRendererHandle handle, int limitPercent) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラに割り当てられたレンダリング時間の上限を取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      オーディオレンダラに割り当てられたレンダリング時間の上限を返します。単位はパーセントです。
 * @pre
 * - handle は有効なもの
 */
int GetAudioRendererRenderingTimeLimit(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラによるレンダリング処理を実行します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 * @endretresult
 * @pre
 * - handle は有効なもの
 */
Result ExecuteAudioRendererRendering(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラがレンダリング処理を行うデバイスを取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      オーディオレンダラがレンダリング処理を行うデバイス
 * @pre
 * - handle は有効なもの
 */
AudioRendererRenderingDevice GetAudioRendererRenderingDevice(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @brief       オーディオレンダラのレンダリング処理実行モードを取得します。
 * @param[in]   handle                  オーディオレンダラハンドル
 * @return      オーディオレンダラのレンダリング処理実行モード
 * @pre
 * - handle は有効なもの
 */
AudioRendererExecutionMode GetAudioRendererExecutionMode(AudioRendererHandle handle) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
