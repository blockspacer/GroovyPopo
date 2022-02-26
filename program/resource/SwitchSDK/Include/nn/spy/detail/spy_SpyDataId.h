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

#include <nn/spy/spy_Config.h>
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

namespace nn {
namespace spy {
namespace detail {

//! @briefprivate
//! @brief Spy データの識別子です。
//!
//! @details
//! nn::spy ライブラリがサポートする各データ名に対して、Spy.exe が一意な SpyDataId を割り当てます。
enum SpyDataId
{
    SpyDataId_Invalid = 0, //!< 無効値
    SpyDataId_Min = 1, //!< SpyDataId の最小値
    SpyDataId_Max = 1024 //!< SpyDataId の最大値（SelectDataIdパケットのサイズによる制限）
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
