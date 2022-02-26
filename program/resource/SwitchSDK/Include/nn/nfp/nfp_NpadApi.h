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
    @brief      Npad に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/hid/hid_NpadCommonTypes.h>
#include <nn/nfp/nfp_Result.h>
#include <nn/nfp/nfp_Types.h>

namespace nn {
namespace nfp {

/**
 * @name    Npad
 * @{
 */

/**
 * @brief  Npad ID を取得します。
 *
 * @param[out] pOutNpadId          Npad ID です。
 * @param[in]  deviceHandle        NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutNpadId はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @details
 *  指定した NFC デバイスを有する Npad の Npad ID を取得します。
 *
 */
nn::Result GetNpadId(nn::hid::NpadIdType* pOutNpadId, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfp
}  // nn
