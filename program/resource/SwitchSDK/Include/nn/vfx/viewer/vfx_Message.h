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

#include <nn/vfx/vfx_EndianUtil.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//--------------------------------------------------------------------------------------------
//! @brief      メッセージです.
//--------------------------------------------------------------------------------------------
struct Message
{
    uint32_t    type;           //!< メッセージタイプです.
    uint32_t      bufferSize;     //!< メッセージデータのサイズです.
};

//--------------------------------------------------------------------------------------------
//! @brief      メッセージ情報です.
//--------------------------------------------------------------------------------------------
struct MessageInfo
{
    uint32_t      packetSize;     //!< パケット全体のサイズです.
    int32_t       packetCount;    //!< パケット数です.
    int32_t       processId;      //!< プロセスIDです.(送信順番を判定するために使用します).
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
