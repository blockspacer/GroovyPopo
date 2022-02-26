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
#include <nn/nn_Macro.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_BufferLayoutData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {
namespace detail {

template<>
class TimestampBufferImpl< ApiVariationNvn8 >
    : public DataContainer< TimestampBufferImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;

    int64_t GetValue() const NN_NOEXCEPT;
};

template<>
class QueryBufferImpl< ApiVariationNvn8 >
    : public DataContainer< QueryBufferImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;

    int64_t GetValue() const NN_NOEXCEPT;
};

template<>
nn::TimeSpan GetDuration< ApiVariationNvn8 >(
    int64_t startTimestampValue, int64_t endTimestampValue ) NN_NOEXCEPT;

}
}
}
