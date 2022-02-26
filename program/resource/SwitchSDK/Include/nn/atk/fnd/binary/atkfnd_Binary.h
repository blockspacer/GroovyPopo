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

#include <nn/atk/fnd/basis/atkfnd_Config.h>

#if defined(NN_ATK_FND_IS_LITTLE_ENDIAN)

//! @internal
#define NN_ATK_BINARY_MAKE_SIGNATURE(a,b,c,d) \
    static_cast<uint32_t >(  \
    (static_cast<uint8_t>(a) <<  0)      \
    | (static_cast<uint8_t>(b) <<  8)    \
    | (static_cast<uint8_t>(c) << 16)    \
    | (static_cast<uint8_t>(d) << 24)    \
    )

#else

//! @internal
#define NN_ATK_BINARY_MAKE_SIGNATURE(a,b,c,d) \
    static_cast<uint32_t >(  \
    (static_cast<uint8_t>(a) << 24)      \
    | (static_cast<uint8_t>(b) << 16)    \
    | (static_cast<uint8_t>(c) <<  8)    \
    | (static_cast<uint8_t>(d) <<  0)    \
    )

#endif

//! @internal
#define NN_ATK_MAKE_VERSION(major, minor, micro, binaryBugFix) \
    static_cast<uint32_t>(                                     \
    (static_cast<uint8_t>(major) << 24)                        \
    | (static_cast<uint8_t>(minor) << 16)                      \
    | (static_cast<uint8_t>(micro) <<  8)                      \
    | (static_cast<uint8_t>(binaryBugFix))                     \
    )
