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

#include <climits>
#include <nn/nn_Common.h>

namespace nn { namespace ige {

typedef uint32_t NodeIdType; //!< ノード識別子の型です。

namespace detail {

static const NodeIdType s_InvalidId = UINT32_MAX;   //!< 無効なノードであることを表します。
static const NodeIdType s_RootId = 0;               //!< ルートノードであることを示します。

}}} // namespace nn::ige::detail
