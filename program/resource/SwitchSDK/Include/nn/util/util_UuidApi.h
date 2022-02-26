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
#include <nn/util/util_UuidTypes.h>

namespace nn
{
namespace util
{

/**
    @brief RFC 4122に規定されたUniversally Unique Identifier (UUID)を生成します。
    @details
        RFC 4122 に規定されたUniversally Unique Identifier (UUID)を生成します。

        なお、本 API はスレッドセーフです。
 */
Uuid GenerateUuid() NN_NOEXCEPT;

}
}
