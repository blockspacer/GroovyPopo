/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief デバイス初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief デバイスを初期化するための情報を表す構造体です。
*/
struct DeviceInfoData
{
    Bit8 debugMode; //!< nn::gfx::DebugMode によって表される、デバッグモードです。
    char reserved2[ 3 ]; //!< 予約領域です。
    uint16_t apiMajorVersion;
    uint16_t apiMinorVersion;

    char reserved[ 56 ]; //!< 予約領域です
};

}
}
