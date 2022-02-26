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
* @brief ユーザーデータリソースのデータに関する API の宣言
*/

#pragma once

#include <nn/util/util_BinTypes.h>

namespace nn {
namespace gfx {

/**
* @brief ユーザーデータリソースを表す構造体です。
*/
struct ResUserDataData
{
    nn::util::BinPtrToString pName; //!< 名前です。
    nn::util::BinTPtr< void > pData; //!< データへのポインターです。
    uint32_t count; //!< データの数です。
    uint8_t type; //!< nn::gfx::ResUserData::Type で表される、データのタイプです。

    char reserved[ 43 ]; //!< 予約領域です。
};

}
}
