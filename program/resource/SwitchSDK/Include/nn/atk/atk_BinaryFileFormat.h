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

#include <nn/atk/atk_Global.h>

namespace nn {
namespace atk {
namespace detail {

struct BinaryFileHeader
{
    int32_t  signature;            // ファイルシグネチャ   : 4 Byte
    uint16_t byteOrder;            // バイトオーダーマーク : 2 Byte
    uint16_t headerSize;           // ヘッダサイズ         : 2 Byte
    uint32_t version;              // ファイルバージョン   : 4 Byte
    uint32_t fileSize;             // ファイルサイズ       : 4 Byte
    uint16_t dataBlocks;           // ブロック数           : 2 Byte
    uint16_t reserved;             // 予約                 : 2 Byte

    static const uint16_t ValidByteOrderMark = 0xfeff;
};

struct BinaryBlockHeader
{
    int32_t  kind;                 // ブロック種別名
    uint32_t size;                 // ブロック全体サイズ
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
