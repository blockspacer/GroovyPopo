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
#include <nn/nn_Result.h>
#include <nn/audio/audio_DeviceTypes.h>

namespace nn { namespace audio {

/**
 * @name        その他
 * @{
 */

/**
 * @brief           プラットフォームがサポートするデバイスのリストを取得します。
 * @param[out]      outDeviceNames  デバイスの名前を取得するバッファ
 * @param[in]       count           outDeviceNames に取得可能な AudioDeviceName の数
 * @return          outDeviceNames に取得されたデバイスの数を返します。
 *
 * @pre
 * - outDeviceNames != nullptr
 * - 0 <= count
 * @post
 * - 戻り値 n に対し、outDeviceNames[0] から outDeviceNames[n-1] にデバイスの名前が書き込まれている
 *
 * @details
 * オーディオが出力されるデバイスは、アプリケーションを実行するプラットフォームごとに異なります。@n
 * この関数ではアプリケーションを実行しているプラットフォームに接続されたデバイスのうち、
 * NintendoSDK で利用可能な出力デバイスのリストを取得します。@n
 *
 * 各プラットフォームで利用可能な出力デバイスの具体的なリストは @confluencelink{166500119, NintendoSDK ドキュメントの AUDIO ライブラリの説明} を参照ください。@n
 * @n
 * この関数で取得した outDeviceNames を利用して nn::audio::SetAudioDeviceOutputVolume() で、デバイス毎の出力のマスターボリュームを設定することができます。@n
 *  @platformbegin{Windows}
 *   - Windows 環境では本 API はサポートされていません。 返り値は常に 0 が返され、 outDeviceNames には何も書き込まれません。
 *
 *  @platformend
 */
int ListAudioDeviceName(AudioDeviceName* outDeviceNames, int count) NN_NOEXCEPT;

/**
 * @brief           出力先デバイスのボリュームを設定します。
 * @param[in]       pDeviceName   プラットフォームがサポートするデバイスの名前
 * @param[in]       volume        設定するボリューム値
 *
 * @pre
 * - pDeviceName != nullptr
 * - pDeviceName は nn::audio::ListAudioDeviceName() で取得されるデバイスに含まれる。
 * - nn::audio::AudioDeviceOutputVolumeMin <= volume <= nn::audio::AudioDeviceOutputVolumeMax
 * @post
 * - volume == @ref nn::audio::GetAudioDeviceOutputVolume "nn::audio::GetAudioDeviceOutputVolume(pDeviceName)"
 *
 * @details
 * 出力先のデバイス毎のボリュームを設定します。@n
 * nn::audio::OpenDefaultAudioOut() でオーディオ出力を準備した場合や、
 * nn::audio::AddDeviceSink() に "MainAudioOut" を指定してオーディオ出力デバイス用の SinkType を準備した場合、
 * アプリケーションの動作状況やプラットフォームに応じて、標準の出力先にオーディオの出力を行います。@n
 * この関数は、これらの出力先に対して、個別にボリュームを設定する機能を提供します。@n
 * @n
 * この関数はデバイス個別のボリュームを設定することを目的としており、デバイスに依存なく施したいボリュームには、この関数は利用しないでください。@n
 * デバイス依存なくボリューム設定を施すには、
 * オーディオ出力では nn::audio::SetAudioOutBufferInfo() で buffer に設定するオーディオ出力バッファに含まれるサンプルのレベルの調整を、
 * オーディオレンダラでは nn::audio::SetFinalMixVolume() の利用を、それぞれ検討してください。@n
 * @n
 * pDeviceName には nn::audio::ListAudioDeviceName() で取得されるデバイスを指定することができます。@n
 * 各プラットフォームで利用可能な出力デバイスの具体的なリストは、NintendoSDK ドキュメントの 機能 > AUDIO ライブラリ を参照ください。@n
 * @n
 * pDeviceName で指定したデバイスに、オーディオレンダラ、オーディオアウト等の複数の入力が存在する場合、
 * それらすべての入力結果に対してボリュームが施されます。@n
 * @n
 * 一度もこの関数を呼んでいない場合、デバイスに関係なく初期値 1.0f が設定されています。@n
 * @n
 * この関数によるボリューム設定は、この関数を呼び出したアプリケーションの内部でのみ有効です。システムの挙動には影響を与えません。@n
 *  @platformbegin{Windows}
 *   - Windows 環境では本 API はサポートされていません。 この関数の呼び出しは常に成功しますが、アプリケーションの挙動は変更されません。
 *
 *  @platformend
 *
 *  @platformbegin{NX}
 *   - NX 環境では、本 API により AudioUsbDeviceOutput (USB 音声出力デバイス) にデバイス毎のボリューム値を一度も設定していない場合、AudioUsbDeviceOutput のボリューム値は AudioStereoJackOutput のボリューム値と常に同じになります。
 *
 *  @platformend
 */
void SetAudioDeviceOutputVolume(const AudioDeviceName* pDeviceName, float volume) NN_NOEXCEPT;

/**
 * @brief           出力先デバイスのボリュームを取得します。
 * @param[in]       pDeviceName ボリュームを取得する出力先デバイスの名前
 * @return          pDeviceName に指定されたデバイスに設定されているボリュームを返します。
 * @pre
 * - pDeviceName != nullptr
 * - pDeviceName は nn::audio::ListAudioDeviceName() で取得されるデバイスに含まれる。
 * @details
 * この関数は nn::audio::SetAudioDeviceOutputVolume() で設定した出力先のデバイス毎のボリュームを取得します。@n
 * nn::audio::SetAudioDeviceOutputVolume() の説明も併せて参照してください。
 *  @platformbegin{Windows}
 *   - Windows 環境では本 API はサポートされていません。 常に初期値 1.0f を返します。
 *
 *  @platformend
 */
float GetAudioDeviceOutputVolume(const AudioDeviceName* pDeviceName) NN_NOEXCEPT;

/**
 * @brief       仮想オーディオデバイスに実際のオーディオデバイスを割り当てます。
 * @param[in]   virtualName             仮想オーディオデバイス名
 * @param[in]   pDeviceName             実際のオーディオデバイス
 * @retresult
 *   @handleresult{nn::audio::ResultNotFound}
 *   @handleresult{nn::audio::ResultNotSupported}
 * @endretresult
 *
 * @pre
 * - virtualName != nullptr
 * - pDeviceName != nullptr
 * @details
 * この関数は PC 開発環境上でのみ利用可能です。その他の環境においては処理は行われず ResultNotSupported が返ります。
 *
 * @a virtualName としては現状 "MainAudioOut" のみが指定可能です。
 * "MainAudioOut" は 5.1ch 48kHz のオーディオ出力デバイスとして機能します。
 * @a pDeviceName には nn::audio::ListAudioDeviceName() により得られるデバイスを指定することができます。
 * @a pDeviceName で指定されるデバイスのチャンネル数が 5.1ch でない場合はダウンミックスが行われます。
 *
 * この関数はすべての nn::audio の関数を利用する前に呼んでおく必要があります。
 */
Result SetAudioDeviceMapping(const char* virtualName, const AudioDeviceName* pDeviceName) NN_NOEXCEPT;

/**
 * @brief       仮想オーディオデバイスに実際のオーディオデバイスを割り当てます。
 * @param[in]   virtualName             仮想オーディオデバイス名
 * @param[in]   physicalName            実際のオーディオデバイス名
 * @retresult
 *   @handleresult{nn::audio::ResultNotFound}
 *   @handleresult{nn::audio::ResultNotSupported}
 * @endretresult
 *
 * @pre
 * - virtualName != nullptr
 * - physicalName != nullptr
 * @details
 * この関数は PC 開発環境上でのみ利用可能です。その他の環境においては処理は行われず ResultNotSupported が返ります。
 *
 * @a virtualName としては現状 "MainAudioOut" のみが指定可能です。
 * "MainAudioOut" は 5.1ch 48kHz のオーディオ出力デバイスとして機能します。
 * @a physicalName には nn::audio::ListAudioOuts() により得られるデバイスを指定することができます。
 * @a physicalName で指定されるデバイスのチャンネル数が 5.1ch でない場合はダウンミックスが行われます。
 *
 * この関数はすべての nn::audio の関数を利用する前に呼んでおく必要があります。
 */
Result SetAudioDeviceMapping(const char* virtualName, const char* physicalName) NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::audio
