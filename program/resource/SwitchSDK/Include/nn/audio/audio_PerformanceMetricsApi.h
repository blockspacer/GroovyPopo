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

#include <nn/audio/audio_VoiceTypes.h>
#include <nn/audio/audio_SubMixTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_PerformanceMetricsTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ パフォーマンスメトリクス 関連 API
 * @{
 */

/**
 * @brief           パフォーマンス情報の書き出し先アドレスを設定します。
 * @param[out]      pOutConfig    パフォーマンス情報を取得するオーディオレンダラを取得する際に初期化した AudioRendererConfig
 * @param[in]       buffer        オーディオレンダラがパフォーマンス情報を書き込むメモリ
 * @param[in]       bufferSize    buffer で渡す領域のサイズ
 * @return          前回この関数で buffer に設定したアドレスを返します。
 * @pre
 * - pOutConfig != nullptr
 * - config は初期化されている
 * - buffer が nn::audio::BufferAlignSize にアラインしている
 * @details
 * オーディオレンダラがパフォーマンス情報を書き出す先に buffer で指定した格納先アドレスを設定します。@n
 * 既に設定されていた場合は buffer で再設定した後に、前回 buffer に設定したアドレスを返します。@n
 * @n
 * buffer に設定した領域へのオーディオレンダラからのパフォーマンス情報の書き出しは nn::audio::RequestUpdateAudioRenderer() の呼び出し中に行われます。@n
 * nn::audio::RequestUpdateAudioRenderer() の呼び出し中は buffer へのアクセスをしないでください。@n
 * @n
 * buffer に nullptr を指定した場合は書き出しを行いません。@n
 * デフォルトでは buffer に nullptr が指定された状態であり、この関数で nullptr 以外を指定しない限りは nn::audio::SetPerformanceFrameBuffer() の書き出しは行われません。@n
 * @n
 * この関数の返り値のアドレスはパフォーマンスフレームの先頭アドレスを指しています。
 * このアドレスを nn::audio::PerformanceFrameHeader* にキャストすることで、パフォーマンスフレームの利用・パースをすることができます。@n
 * この処理を簡便に行うために nn::audio::PerformanceInfo が準備されています。@n
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - nn::audio::SetPerformanceFrameBuffer(AudioRendererConfig* pOutConfig, void* buffer, size_t bufferSize)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const VoiceType* pVoice)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const SubMixType* pSubMix)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const FinalMixType* pFinalMix)
 * - nn::audio::ClearPerformanceDetailTarget(AudioRendererConfig* pOutConfig)
 */
void* SetPerformanceFrameBuffer(AudioRendererConfig* pOutConfig, void* buffer, size_t bufferSize) NN_NOEXCEPT;

/**
 * @brief           パフォーマンス情報の書き出し先に必要なメモリサイズを取得します。
 * @param[in]       parameter       パフォーマンス情報を取得するオーディオレンダラの取得時に利用したパラメータ
 * @return          パフォーマンス情報の書き出し先に必要なメモリサイズを返します。
 * @pre
 * - parameter != nullptr
 * @details
 * オーディオレンダラにはその取得時に nn::audio::AudioRendererParameter.performanceFrameCount で指定した分の、パフォーマンス情報が保持されます。@n
 * オーディオレンダラが保持しているパフォーマンス情報は nn::audio::SetPerformanceFrameBuffer() で指定したメモリに書き出されます。@n
 * この関数では nn::audio::SetPerformanceFrameBuffer() で指定するメモリ領域が持つべきサイズを計算し、返します。
 * parameter にはオーディオレンダラのオープン時に指定したパラメータを指定してください。
 */
size_t GetRequiredBufferSizeForPerformanceFrames(const AudioRendererParameter& parameter) NN_NOEXCEPT;

/**
 * @brief           詳細なパフォーマンス情報を取得する対象の VoiceType を設定します。
 * @param[out]      pOutConfig   パフォーマンス情報を取得するオーディオレンダラを取得する際に初期化した AudioRendererConfig
 * @param[in]       pVoice       対象に設定する VoiceType
 * @pre
 * - pOutConfig != nullptr
 * - config は初期化されている
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * @details
 * この関数で指定をするまで PerformanceDetail は取得されません。
 * 設定できる対象は VoiceType / SubMixType / FinalMixType の種類によらず合計で１つのみです。
 * 再度 nn::audio::SetPerformanceDetailTarget() を呼び出して設定した場合、対象は置き換えられます。
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - nn::audio::SetPerformanceFrameBuffer(AudioRendererConfig* pOutConfig, void* buffer, size_t bufferSize)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const VoiceType* pVoice)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const SubMixType* pSubMix)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const FinalMixType* pFinalMix)
 * - nn::audio::ClearPerformanceDetailTarget(AudioRendererConfig* pOutConfig)
 */
void SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief           詳細なパフォーマンス情報を取得する対象の SubMixType を設定します。
 * @param[out]      pOutConfig    パフォーマンス情報を取得するオーディオレンダラを取得する際に初期化した AudioRendererConfig
 * @param[in]       pSubMix       対象に設定する SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - config は初期化されている
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @details
 * この関数で指定をするまで PerformanceDetail は取得されません。
 * 設定できる対象は VoiceType / SubMixType / FinalMixType の種類によらず合計で１つのみです。
 * 再度 nn::audio::SetPerformanceDetailTarget() を呼び出して設定した場合、対象は置き換えられます。
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - nn::audio::SetPerformanceFrameBuffer(AudioRendererConfig* pOutConfig, void* buffer, size_t bufferSize)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const VoiceType* pVoice)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const SubMixType* pSubMix)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const FinalMixType* pFinalMix)
 * - nn::audio::ClearPerformanceDetailTarget(AudioRendererConfig* pOutConfig)
 */
void SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief           詳細なパフォーマンス情報を取得する対象の FinalMixType を設定します。
 * @param[out]      pOutConfig      パフォーマンス情報を取得するオーディオレンダラを取得する際に初期化した AudioRendererConfig
 * @param[in]       pFinalMix       対象に設定する FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - config は初期化されている
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @details
 * この関数で指定をするまで PerformanceDetail は取得されません。
 * 設定できる対象は VoiceType / SubMixType / FinalMixType の種類によらず合計で１つのみです。
 * 再度 nn::audio::SetPerformanceDetailTarget() を呼び出して設定した場合、対象は置き換えられます。
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - nn::audio::SetPerformanceFrameBuffer(AudioRendererConfig* pOutConfig, void* buffer, size_t bufferSize)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const VoiceType* pVoice)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const SubMixType* pSubMix)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const FinalMixType* pFinalMix)
 * - nn::audio::ClearPerformanceDetailTarget(AudioRendererConfig* pOutConfig)
 */
void SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief           詳細なパフォーマンス情報の取得する対象を取り除きます。
 * @param[out]       pOutConfig    パフォーマンス情報を取得するオーディオレンダラを取得する際に初期化した AudioRendererConfig
 * @pre
 * - pOutConfig != nullptr
 * - config は初期化されている
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - nn::audio::SetPerformanceFrameBuffer(AudioRendererConfig* pOutConfig, void* buffer, size_t bufferSize)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const VoiceType* pVoice)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const SubMixType* pSubMix)
 * - nn::audio::SetPerformanceDetailTarget(AudioRendererConfig* pOutConfig, const FinalMixType* pFinalMix)
 * - nn::audio::ClearPerformanceDetailTarget(AudioRendererConfig* pOutConfig)
 */
void ClearPerformanceDetailTarget(AudioRendererConfig* pOutConfig) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
