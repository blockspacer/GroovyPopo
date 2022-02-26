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

#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class DisposeCallback
{
public:
    //! @internal
    util::IntrusiveListNode m_DisposeLink;

    virtual ~DisposeCallback() NN_NOEXCEPT {}
    virtual void InvalidateData( const void* start, const void* end ) NN_NOEXCEPT = 0;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

