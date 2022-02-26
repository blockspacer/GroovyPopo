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
#include <nn/gfx/gfx_BufferLayoutData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {
namespace detail {

template<>
class TimestampBufferImpl< ApiVariationGl4 >
    : public DataContainer< TimestampBufferImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;

    int64_t GetValue() const NN_NOEXCEPT;
};

template<>
class QueryBufferImpl< ApiVariationGl4 >
    : public DataContainer< QueryBufferImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;

    int64_t GetValue() const NN_NOEXCEPT;
};

template<>
nn::TimeSpan GetDuration< ApiVariationGl4 >(
    int64_t startTimestampValue, int64_t endTimestampValue ) NN_NOEXCEPT;

}
}
}
