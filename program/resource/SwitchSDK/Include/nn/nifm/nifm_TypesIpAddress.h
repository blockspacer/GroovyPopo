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

#include <nn/nn_Common.h>

#include <cstring>

namespace nn
{
namespace nifm
{

/**
 * @brief IP アドレスを表す構造体です。
 *
 * @details
 *  この定義は仮実装です。
 *  今後のリリースで変更される可能性があります。
 */
struct IpV4Address
{
    static const size_t Size = 4;  //!< IP アドレスサイズ
    Bit8 data[Size];               //!< IP アドレス
};

inline bool operator==(const IpV4Address& lhs, const IpV4Address& rhs)
{
    return std::memcmp(lhs.data, rhs.data, IpV4Address::Size) == 0;
}

inline bool operator!=(const IpV4Address& lhs, const IpV4Address& rhs)
{
    return !(lhs == rhs);
}

}
}
