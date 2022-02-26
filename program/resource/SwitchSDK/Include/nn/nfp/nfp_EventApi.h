/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/** @file
    @brief      NFP(amiibo) ライブラリのイベントに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_SystemEvent.h>
#include <nn/nfp/nfp_Result.h>
#include <nn/nfp/nfp_Types.h>

namespace nn {
namespace nfp {

/**
 * @name    イベント登録
 * @{
 */

/**
 * @brief   タグの発見を通知するイベントを設定します。
 *
 * @param [in,out]  pEvent             タグ発見通知に使用するイベントを指定します。
 * @param [in]      deviceHandle       NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               設定に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pEvent はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @post
 *  指定した NFC デバイスにて、タグ発見時に @a pEvent がシグナルされるようになります。
 *
 * @details
 *  NFP ライブラリの初期化後、NFC デバイス毎に @ref StartDetection() でタグの検知を開始する前に実行してください。
 *  指定した NFC デバイスにて、タグが検出されると、この API で設定したイベントがシグナル化されます。
 *  このイベントは、NFP 以外のタグが発見された場合も、イベントがシグナル化されます。
 *
 *  イベントは API 内部で初期化されるため、ユーザー側で初期化を行う必要はありません。
 *  イベントは自動クリアモードで初期化されます。
 *
 *  イベントが不要になった場合は @ref nn::os::DestroySystemEvent() を呼び出して必ず破棄してください。
 */
nn::Result AttachActivateEvent(nn::os::SystemEventType* pEvent, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグの喪失を通知するイベントを設定します。
 *
 * @param [in,out]  pEvent             タグ喪失通知に使用するイベントを指定します。
 * @param [in]      deviceHandle       NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               設定に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pEvent はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @post
 *  指定した NFC デバイスにて、タグ喪失時に @a pEvent がシグナルされるようになります。
 *
 * @details
 *  NFP ライブラリの初期化後、NFC デバイス毎に @ref StartDetection() でタグの検知を開始する前に実行してください。
 *  指定した NFC デバイスにて、検出したタグが離されると、この API で設定したイベントがシグナル化されます。
 *  タグが検出された状態で @ref StopDetection() や @ref Finalize() でタグの検出を停止した場合も、イベントがシグナル化されます。
 *
 *  イベントは API 内部で初期化されるため、ユーザー側で初期化を行う必要はありません。
 *  イベントは自動クリアモードで初期化されます。
 *
 *  イベントが不要になった場合は @ref nn::os::DestroySystemEvent() を呼び出して必ず破棄してください。
 */
nn::Result AttachDeactivateEvent(nn::os::SystemEventType* pEvent, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   NFC デバイスの利用可/不可の変化を通知するイベントを設定します。
 *
 * @param [in,out]  pEvent             通知に使用するイベントを指定します。
 *
 * @pre
 *  - @a pEvent はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @post
 *  NFC デバイスの利用可/不可が変化した場合に @a pEvent がシグナルされるようになります。
 *
 * @details
 *  NFP ライブラリの初期化後に実行してください。@n
 *  以下のタイミングで、この API で設定したイベントがシグナル化されます。@n
 *  - NFC デバイスが切断された
 *  - NFC デバイスが接続された
 *  - フォーカスのあるアプリケーションが変わった
 *  - 本体がスリープから復帰した
 *  - NFC 機能が無効になった
 *  - NFC 機能が有効になった
 *
 *  シグナルされたときに @ref ListDevices() や @ref GetDeviceState() を呼ぶことで、効率的に NFC デバイスの状態を確認することができます。@n
 *  イベントは API 内部で初期化されるため、ユーザー側で初期化を行う必要はありません。
 *  イベントは自動クリアモードで初期化されます。
 *
 *  イベントが不要になった場合は @ref nn::os::DestroySystemEvent() を呼び出して必ず破棄してください。
 */
void AttachAvailabilityChangeEvent(nn::os::SystemEventType* pEvent) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfp
}  // nn
