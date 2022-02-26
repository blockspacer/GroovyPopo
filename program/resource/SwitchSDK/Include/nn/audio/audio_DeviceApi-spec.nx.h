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
#include <nn/os/os_SystemEvent.h>

namespace nn { namespace audio {

/**
 * @name        その他
 * @{
 */

/**
 * @brief           現在音声が出力されているデバイスの名前を取得します
 * @param[in]       pOutDeviceName    デバイスの名前を取得するバッファ
 * @pre
 * - pDeviceName != nullptr
 * @details
 * この関数呼び出し時に音声が出力されているデバイスの名前を取得します。
 * デバイスの出力先が切り替わったタイミングは nn::audio::AcquireAudioDeviceNotificationForOutput() で登録する nn::os::SystemEvent によって取得することが可能です。
 * この関数の呼び出し中に音声出力先デバイスが切り替わった場合は、切り替え前のデバイスの名前を返します。
 * この関数呼び出し後に nn::audio::AcquireAudioDeviceNotificationForOutput() で登録する nn::os::SystemEvent がシグナルされているかを確認することで、
 * 呼び出し中の出力先変更の有無を確認することができます。
 *
 * pOutDeviceName に返される名前は必ず nn::audio::ListAudioDeviceName() で取得できる何れかの名前です。 返される名前は動作するプラットフォーム毎に異なる点に注意してください。
 * 各プラットフォームで利用可能な出力デバイスの具体的なリストは @confluencelink{166500119, NintendoSDK ドキュメントの AUDIO ライブラリの説明} を参照ください。@n
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API はサポートされていません。 pOutDeviceName には何も書き込まれません。
 *
 * @platformend
 */
void GetActiveAudioDeviceName(AudioDeviceName* pOutDeviceName) NN_NOEXCEPT;

/**
 * @deprecated      本 API は非推奨となりました。同等の機能を提供している AcquireAudioDeviceNotificationForOutput(nn::os::SystemEvent* pOutSystemEvent) の利用に切り替えてください。
 * @brief           音声出力先デバイスの切り替えを通知するイベントを取得します
 * @param[out]       pOutSystemEvent    切り替え時にシグナルされる nn::os::SystemEvent
 * @pre
 * - pOutSystemEvent != nullptr
 * - pOutSystemEvent は未初期化
 * @post
 * - pOutSystemEvent は初期化される
 * @details
 * pOutSystemEvent は音声出力先切り替え時にシグナルされるイベントとして初期化されます。
 * このイベントは音声出力先切り替えが発生する毎にシグナルされます。
 * このイベントのシグナル待機後に nn::audio::GetActiveAudioDeviceName() を呼び出すことで、切り替わった先の音声出力先デバイスの名前を取得することができます。
 * ただしユーザーの操作によっては nn::audio::GetActiveAudioDeviceName() で名前の取得中に、音声出力先が再度切り替わる可能性があるので注意してください。
 * 詳しくは nn::audio::GetActiveAudioDeviceName() の説明を参照してください。
 *
 * pOutSystemEvent は nn::os::EventClearMode_AutoClear で初期化されています。
 *
 * この関数を複数回呼び出すことは禁止されています。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API はサポートされていません。 pOutSystemEvent は初期化されません。
 *
 * @platformend
 */
NN_DEPRECATED void AcquireAudioDeviceSwitchNotification(nn::os::SystemEvent* pOutSystemEvent) NN_NOEXCEPT;

/**
 * @brief           音声出力先デバイスのイベント発生を通知するイベントを取得します
 * @param[out]       pOutSystemEvent    イベント発生時にシグナルされる nn::os::SystemEvent
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 * @endretresult
 * @pre
 * - pOutSystemEvent != nullptr
 * - pOutSystemEvent は未初期化
 * @post
 * - pOutSystemEvent は初期化される
 * @details
 * pOutSystemEvent は音声出力先デバイスのイベント発生時にシグナルされるイベントとして初期化されます。
 * pOutSystemEvent は音声出力先デバイスのイベントが発生する毎にシグナルされます。どのようなイベント発生の通知を受け取ることが出来るかはプラットフォーム依存です。
 *
 * pOutSystemEvent は nn::os::EventClearMode_AutoClear で初期化されています。
 *
 * この関数を複数回呼び出すことはできません。2 回目以降の呼び出しでは戻り値が失敗となり、SystemEvent は初期化されません。
 *
 * @platformbegin{NX}
 * NX 環境では、音声出力先の切り替わりをイベントとして取得することが出来ます。
 * このイベントのシグナル待機後に nn::audio::GetActiveAudioDeviceName() を呼び出すことで、切り替わった先の音声出力先デバイスの名前を取得することができます。
 * ただしユーザーの操作によっては nn::audio::GetActiveAudioDeviceName() で名前の取得中に、音声出力先が再度切り替わる可能性があるので注意してください。
 * 詳しくは nn::audio::GetActiveAudioDeviceName() の説明を参照してください。
 *
 * @platformend
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API はサポートされていません。 pOutSystemEvent は初期化されません。
 *
 * @platformend
 */
nn::Result AcquireAudioDeviceNotificationForOutput(nn::os::SystemEvent* pOutSystemEvent) NN_NOEXCEPT;

/**
 * @brief           音声入力元デバイスのイベント発生を通知するイベントを取得します
 * @param[out]       pOutSystemEvent    イベント発生時にシグナルされる nn::os::SystemEvent
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 * @endretresult
 * @pre
 * - pOutSystemEvent != nullptr
 * - pOutSystemEvent は未初期化
 * @post
 * - pOutSystemEvent は初期化される
 * @details
 * pOutSystemEvent は音声入力元デバイスのイベント発生時にシグナルされるイベントとして初期化されます。
 * pOutSystemEvent は音声入力元デバイスのイベントが発生する毎にシグナルされます。どのようなイベント発生の通知を受け取ることが出来るかはプラットフォーム依存です。
 *
 * pOutSystemEvent は nn::os::EventClearMode_AutoClear で初期化されています。
 *
 * この関数を複数回呼び出すことはできません。2 回目以降の呼び出しでは戻り値が失敗となり、SystemEvent は初期化されません。
 *
 * @platformbegin{NX}
 * NX 環境では、音声入力元デバイスの挿抜をイベントとして取得することが出来ます。
 *
 * @platformend
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API はサポートされていません。 pOutSystemEvent は初期化されません。
 *
 * @platformend
 */
nn::Result AcquireAudioDeviceNotificationForInput(nn::os::SystemEvent* pOutSystemEvent) NN_NOEXCEPT;

/**
 * @brief           音声出力デバイスから出力されているチャンネル数を取得します
 * @return          出力チャンネル数を返します。
 * @details
 * この関数呼び出し時に音声出力デバイスから出力されているチャンネル数を取得します。@n
 *
 * @platformbegin{NX}
 *  NX 環境では nn::audio::AcquireAudioDeviceNotificationForOutput() で登録する nn::os::SystemEvent がシグナルされているかを確認することで、チャンネル数の変更の有無を確認することができます。@n
 *  本関数の処理には時間がかかる可能性があるため、最初にチャンネル数を取得した後は、シグナルされたタイミングでのみ 本 API を呼び出すことを推奨します。
 *
 * @platformend
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API はサポートされていません。 常に 0 が返ります。
 *
 * @platformend
 */
int GetActiveAudioDeviceChannelCountForOutput() NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::audio
