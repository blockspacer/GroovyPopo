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

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

#if defined(NN_BUILD_CONFIG_ENDIAN_LITTLE)

#define NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(a,b,c,d)    \
    static_cast<uint32_t >(                      \
          (static_cast<uint8_t>(a) <<  0)        \
        | (static_cast<uint8_t>(b) <<  8)        \
        | (static_cast<uint8_t>(c) << 16)        \
        | (static_cast<uint8_t>(d) << 24)        \
    )

#else

#define NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(a,b,c,d)    \
    static_cast<uint32_t >(                      \
          (static_cast<uint8_t>(a) << 24)        \
        | (static_cast<uint8_t>(b) << 16)        \
        | (static_cast<uint8_t>(c) <<  8)        \
        | (static_cast<uint8_t>(d) <<  0)        \
    )
#endif

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
