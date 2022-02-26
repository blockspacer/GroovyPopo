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
    @brief      NFP(amiibo) に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nfp/nfp_Result.h>
#include <nn/nfp/nfp_Types.h>

namespace nn {
namespace nfp {

/**
 * @name    その他
 * @{
 */

/**
 * @brief   NFP ライブラリの状態を取得します。
 *
 * @return  NFP ライブラリの状態を返します。
 *
 * @details NFP ライブラリの状態を取得します。
 *
 */
State GetState() NN_NOEXCEPT;

/**
 * @brief   NFC デバイスの状態を取得します。
 *
 * @param[in]  deviceHandle      NFC デバイスのハンドル
 *
 * @return  NFC デバイスの状態を返します。
 *
 * @details NFC デバイスの状態を取得します。
 *
 */
DeviceState GetDeviceState(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfp
}  // nn
