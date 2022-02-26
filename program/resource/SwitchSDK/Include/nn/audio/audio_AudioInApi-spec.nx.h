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

namespace nn { namespace audio {

/**
 * @brief       オーディオ入力に設定可能なデバイスゲインの最小値です。
 */
const float AudioInDeviceGainMin = 0.0f;

/**
 * @brief       オーディオ入力に設定可能なデバイスゲインの最大値です。
 */
const float AudioInDeviceGainMax = 1.0f;

/**
 * @brief       オーディオ入力のデバイスゲインを設定します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @param[in]   gain                    デバイスゲイン値
 * @retresult
 *   @handleresult{nn::audio::ResultNotSupported}
 * @endretresult
 * @pre
 * - pAudioIn != nullptr
 * - gain >= nn::audio::AudioInDeviceGainMin && gain <= nn::audio::AudioInDeviceGainMax
 *
 * @post
 * - オーディオ入力にデバイスゲインが設定されます。
 *
 * @details
 * オーディオ入力にデバイスゲインを設定します。デバイスゲインは、オーディオ入力に紐づくマイクなどのハードウェアが持つゲイン値です。@n
 * 例えば、マイクの場合は、マイクに搭載されたマイクアンプのゲインが本 API で設定できます。@n
 * オーディオ入力の種類によっては、デバイスゲイン設定に対応していない場合があります。@n
 * その場合、本 API は nn::audio::ResultNotSupported を返して失敗します。@n
 * 本 API で指定した値と実際にハードウェアに反映される値のマッピングは、プラットフォームとデバイス依存です。
 *
 * 本関数の処理には時間がかかる可能性があります。@n
 * この関数の呼び出し中に pAudioIn への操作は行わないでください。
 */
nn::Result SetAudioInDeviceGain(AudioIn* pAudioIn, float gain) NN_NOEXCEPT;


/**
 * @brief       オーディオ入力バッファのデバイスゲインを返します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      デバイスゲインを返します。
 * @pre
 * - pAudioIn != nullptr
 *
 * @details
 * nn::audio::OpenAudioIn() 呼び出し直後（つまり nn::audio::SetAudioInDeviceGain() を呼び出さない場合）は nn::audio::AudioInDeviceGainMax が返ります。
 *
 * オーディオ入力の種類によっては、デバイスゲイン設定に対応していない場合があります。@n
 * その場合、本 API は nn::audio::AudioInDeviceGainMax を常に返します。
 */
float GetAudioInDeviceGain(const AudioIn* pAudioIn) NN_NOEXCEPT;

}}  // namespace nn::audio
