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

#include <nn/nn_Macro.h>
#include <nn/audio/audio_Common.h>
#include <nn/audio/audio_FinalMixTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ FinalMix 関連 API
 * @{
 */

/**
 * @brief       FinalMixType を初期化し AudioRendererConfig に追加します。
 * @param[out]  pOutConfig              FinalMixType を追加するオーディオレンダラの取得時に利用した AudioRendererConfig
 * @param[out]  pOutFinalMix            FinalMixType 構造体
 * @param[in]   bufferCount             FinalMixType のバッファ数
 * @return      FinalMixType が利用可能であれば true を、そうでなければ false を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutConfig と紐づく AudioRenderer が停止状態
 * - pOutFinalMix != nullptr
 * - bufferCount > 0 && bufferCount <= nn::audio::MixBufferCountMax
 */
bool AcquireFinalMix(AudioRendererConfig* pOutConfig, FinalMixType* pOutFinalMix, int bufferCount) NN_NOEXCEPT;

/**
 * @brief       FinalMixType を AudioRendererConfig から除きます。
 * @param[out]  pOutConfig              nn::audio::AcquireFinalMix() で pFinalMix を初期化する際に利用した AudioRendererConfig
 * @param[in]   pFinalMix               FinalMixType 構造体
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutConfig と紐づく AudioRenderer が停止状態
 * - pOutFinalMix != nullptr
 */
void ReleaseFinalMix(AudioRendererConfig* pOutConfig, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       FinalMixType のバッファ数を取得します。
 * @param[in]   pFinalMix               FinalMixType 構造体
 * @return      バッファ数を返します。
 * @pre
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 */
int GetFinalMixBufferCount(const FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       FinalMixType のボリュームを取得します。
 * @param[in]   pFinalMix               FinalMixType 構造体
 * @return      ボリュームを返します。
 * @pre
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 */
float GetFinalMixVolume(const FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       FinalMixType のボリュームを設定します。
 * @param[in]   pFinalMix               FinalMixType 構造体
 * @param[in]   volume                  ボリューム
 * @pre
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * - nn::audio::FinalMixType::GetVolumeMin() <= volume && volume <= nn::audio::FinalMixType::GetVolumeMax()
 */
void SetFinalMixVolume(FinalMixType* pFinalMix, float volume) NN_NOEXCEPT;

/**
 * @brief       FinalMixType のオーディオレンダラ上での ID を取得します。
 * @param[in]   pFinalMix                 FinalMixType
 * @return      NodeId を返します。
 * @pre
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @details
 * NodeId はオーディオレンダラに登録されている VoiceType / SubMixType / FinalMixType / DeviceSinkType / CircularBufferSinkType を一意に特定する数値です。
 * 各 NodeId は単独のオーディオレンダラ内で重複することはありません。
 * NodeId は VoiceType / SubMixType / FinalMixType / DeviceSinkType / CircularBufferSinkType が追加されるたびに、それぞれのインスタンスに割り振られます。
 */
NodeId GetFinalMixNodeId(const FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief           FinalMixType のサンプルレートを取得します。
 * @param[in]       pFinalMix             FinalMixType
 * @return          サンプルレートを返します。
 * @pre
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @details
 * FinalMixType のサンプルレートはオーディオレンダラの初期化時に決定され、
 * nn::audio::AudioRendererParemter.sampleRate の値で指定されます。
 * この関数で取得されるサンプルレートは、必ず初期化時に指定したこの値に一致します。
 */
int GetFinalMixSampleRate(const FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @}
 */
}  // namespace audio
}  // namespace nn
