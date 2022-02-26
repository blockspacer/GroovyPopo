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
    @brief      NFC タグへのアクセスに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nfc/nfc_Result.h>
#include <nn/nfc/nfc_Types.h>

namespace nn {
namespace nfc {

/**
 * @name    NFC タグアクセス
 * @{
 */

/**
 * @brief   NFC デバイスのハンドルのリストを取得します。
 *
 * @param[out] pOutBuffer  取得したハンドルのリストを格納するバッファへのポインタを指定します。
 * @param[out] pOutCount   実際に取得したハンドルの数を格納するバッファへのポインタを指定します。
 * @param[in]  bufferCount 取得するハンドルの最大数（バッファの要素数）を指定します。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     NFC デバイスが見つかりません。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - @a pOutBuffer はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutCount はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a bufferCount
 *
 * @details
 *  NFC デバイスのハンドルが必要な API を呼ぶには、事前にこの API を実行し、NFC デバイスのハンドルを取得しておく必要があります。取得したハンドルでその API の実行対象となる NFC デバイスを指定することができます。@n
 *  Switch Pro コントローラーを有線 USB 通信で使用している場合、そのコントローラーに搭載されている NFC デバイスは使用できないため、本 API でそのハンドルは取得されません。
 */
nn::Result ListDevices(DeviceHandle* pOutBuffer, int* pOutCount, int bufferCount) NN_NOEXCEPT;

/**
 * @brief   タグの検知を開始します。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               検知を開始しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 *     @handleresult{ResultMaxNfcDeviceActivated, NFC 機能を利用中の NFC デバイスが最大数に達しているため、指定された NFC デバイスで NFC 機能を利用できません。NFC 機能を利用中の NFC デバイスが減った後、再試行する必要があります。}
 *     @handleresult{ResultConflictFunction,      競合する他の機能が利用されているため、NFC 機能を利用できません。競合する他の機能の利用が終わった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - GetDeviceState() == @ref DeviceState_Init || GetDeviceState() == @ref DeviceState_Deactive
 *
 * @post
 *  - GetDeviceState() == @ref DeviceState_Search
 *
 * @details
 * 指定した NFC デバイスにて、タグの検知を開始します。@n
 * ライブラリを初期化直後、あるいは検出したタグ喪失直後である必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。) @n
 * @ref AttachActivateEvent や @ref AttachDeactivateEvent で設定したイベントがシグナル状態の場合、この API によって必ず非シグナル状態にクリアされます。@n
 *  この API は @ref StartDetection(const DeviceHandle& deviceHandle, NfcProtocol protocolFilter) にて、検知対象のプロトコルに @ref NfcProtocol_All を指定した場合と機能は同じです。
 *
 */
nn::Result StartDetection(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグの検知を開始します。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 * @param[in] protocolFilter 検知対象のプロトコルを指定します。複数プロトコル指定可能です。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               検知を開始しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 *     @handleresult{ResultMaxNfcDeviceActivated, NFC 機能を利用中の NFC デバイスが最大数に達しているため、指定された NFC デバイスで NFC 機能を利用できません。NFC 機能を利用中の NFC デバイスが減った後、再試行する必要があります。}
 *     @handleresult{ResultConflictFunction,      競合する他の機能が利用されているため、NFC 機能を利用できません。競合する他の機能の利用が終わった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - GetDeviceState() == @ref DeviceState_Init || GetDeviceState() == @ref DeviceState_Deactive
 *
 * @post
 *  - GetDeviceState() == @ref DeviceState_Search
 *
 * @details
 * 指定した NFC デバイスにて、タグの検知を開始します。@n
 * ライブラリを初期化直後、あるいは検出したタグ喪失直後である必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。) @n
 * @ref AttachActivateEvent や @ref AttachDeactivateEvent で設定したイベントがシグナル状態の場合、この API によって必ず非シグナル状態にクリアされます。
 *
 */
nn::Result StartDetection(const DeviceHandle& deviceHandle, NfcProtocol protocolFilter) NN_NOEXCEPT;

/**
 * @brief   タグの検知を終了します。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               検知を終了しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。既に検知は終了しています。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。そのため、既に検知は終了しています。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @post
 *  - GetDeviceState() == @ref DeviceState_Init
 *
 * @details
 * 指定した NFC デバイスのタグの検知を終了します。@n
 * @ref StartDetection を呼び出した後である必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 */
nn::Result StopDetection(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグの情報を取得します。
 *
 * @param [out] pOutTagInfo       @ref TagInfo の取得先へのポインタです。
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           取得に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutTagInfo はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグが検出された状態で実行してください。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 */
nn::Result GetTagInfo(TagInfo* pOutTagInfo, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfc
}  // nn
