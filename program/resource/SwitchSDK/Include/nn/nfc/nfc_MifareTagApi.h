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
    @brief      MIFARE タグへのアクセスに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nfc/nfc_Result.h>
#include <nn/nfc/nfc_Types.h>
#include <nn/nfc/nfc_MifareTypes.h>

namespace nn {
namespace nfc {

/**
 * @name    MIFARE タグアクセス
 * @{
 */

/**
 * @brief   MIFARE タグから任意の 1 または複数のブロックを読み込みます。
 *
 * @param[out] pOutBlockData      データの取得先バッファです。@a blockCount 分必要です。
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 * @param[in]  pBlockParameter    対象となるブロックの情報です。@a blockCount 分必要です。
 * @param[in]  blockCount         読み込むブロックの数です。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               読み込みに成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultAccessError,           読み込みに失敗しました。タグへのアクセス方法（鍵やアクセスするアドレス）が正しいか確認し、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           読み込みに失敗しました。読み込み対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNotSupported,          MIFARE タグではありません。使用しているタグを確認する必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutBlockData はアクセス可能なアドレスを指している必要があります。
 *  - @a pBlockParameter はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a blockCount
 *  - GetState() == @ref State_Init
 *  - 認証に使う全ての鍵の @ref MifareKeyValueFormat が同じである必要があります。
 *
 * @details
 *  MIFARE タグから任意の 1 または複数のブロックを読み込みます。@n
 *  タグが検知済みで喪失されていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)
 */
nn::Result ReadMifare(MifareReadBlockData* pOutBlockData, const DeviceHandle& deviceHandle, const MifareReadBlockParameter* pBlockParameter, size_t blockCount) NN_NOEXCEPT;

/**
 * @brief   MIFARE タグの任意の 1 または複数のブロックに書き込みます。
 *
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 * @param[in]  pBlockParameter    対象となるブロックの情報です。@a blockCount 分必要です。
 * @param[in]  blockCount         書き込むブロックの数です。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               書き込みに成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultAccessError,           書き込みに失敗しました。タグへのアクセス方法（鍵やアクセスするアドレス）が正しいか確認し、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           書き込みに失敗しました。書き込み対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNotSupported,          MIFARE タグではありません。使用しているタグを確認する必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pBlockParameter はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a blockCount
 *  - GetState() == @ref State_Init
 *  - 認証に使う全ての鍵の @ref MifareKeyValueFormat が同じである必要があります。
 *
 * @details
 *  MIFARE タグの任意の 1 または複数のブロックに書き込みます。@n
 *  タグが検知済みで喪失されていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)
 */
nn::Result WriteMifare(const DeviceHandle& deviceHandle, const MifareWriteBlockParameter* pBlockParameter, size_t blockCount) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfc
}  // nn
