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
    @brief      NFC タグへのパススルーアクセスに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_TimeSpan.h>
#include <nn/nfc/nfc_Result.h>
#include <nn/nfc/nfc_Types.h>

namespace nn {
namespace nfc {

/**
 * @name    NFC タグパススルーアクセス
 * @{
 */

/**
 * @brief   タグにコマンドをパススルー送信します。
 *
 * @param[out] pOutBuffer         レスポンスデータの取得先バッファです。
 * @param[out] pOutSize           実際に取得できたレスポンスデータのサイズです。
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 * @param[in]  pData              送信するデータです。
 * @param[in]  dataSize           送信するデータのサイズです。
 * @param[in]  bufferSize         レスポンスデータのバッファサイズです。
 * @param[in]  timeout            レスポンス最大待ち時間
 *
 * @retresult
 *     @handleresult{ResultSuccess,               送信に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultAccessError,           送信に失敗しました。タグへのアクセス方法（コマンドの内容やレスポンス最大待ち時間など）が正しいか確認し、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           送信に失敗しました。送信対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNotSupported,          サポートしている NFC タグではありません。使用しているタグを確認する必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutBuffer はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutSize はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a bufferSize
 *  - @a pData はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a dataSize
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグにコマンドをパススルー送信します。@n
 *  タグが検知済みで喪失されていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  実際に取得できたレスポンスデータのサイズを超えるレスポンスデータバッファの中身は保証されません。参照しないでください。
 */
nn::Result SendCommandByPassThrough(void* pOutBuffer, size_t* pOutSize, const DeviceHandle& deviceHandle, const void* pData, size_t dataSize, size_t bufferSize, nn::TimeSpan timeout) NN_NOEXCEPT;

/**
 * @brief   パススルー送信で使用するセッションの維持を行います。
 *
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               セッション維持に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           セッション維持に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグが検知済みで喪失もセッション維持もされていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  本 API を実行していない状態で、@ref SendCommandByPassThrough() を複数回呼び出した場合、各コマンド送信で利用されるセッションは同一であるとは限りません。@n
 *  同一であることを保証する必要がある場合は、本 API を実行した後、@ref SendCommandByPassThrough() を呼び出してください。@n
 *  例えば、事前にパスワード認証が必要なコマンドを送るような場合、@n
 *  タグの検出後、本 API を実行し、「パスワード認証のコマンド」、「パスワード認証が必要なコマンド」の順に送信するような使い方を想定しています。@n
 *  本 API を実行したのち、セッションの維持が不要になった際には、@n
 *  @ref ReleasePassThroughSession() を実行して、セッション維持をしない状態に戻してください。@n
 *  セッション維持をしない状態と比べ、セッション維持中は、消費電力の増加、および、タグの喪失を通知するイベントが発生しないという制限がありますので、セッション維持は必要最小限にとどめてください。
 */
nn::Result KeepPassThroughSession(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   パススルー送信で使用するセッションを解放します。
 *
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               セッションの解放に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。そのため、セッションを解放する必要はありません。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。そのため、セッションを解放する必要はありません。}
 *     @handleresult{ResultNeedRestart,           セッションの解放に失敗しました。そのため、セッションを解放する必要はありません。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグのセッションが維持されている必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  @ref KeepPassThroughSession() を実行して維持したセッションを解放します。
 */
nn::Result ReleasePassThroughSession(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfc
}  // nn
