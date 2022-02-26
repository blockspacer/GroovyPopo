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
#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_BufferInfoData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class BufferInfo;

namespace detail {

template<>
class BufferInfoImpl< ApiVariationNvn8 >
    : public DataContainer< BufferInfoImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;

    static void ConvertFromInfo( BufferInfoImpl< Target >* pDstInfo, const BufferInfo& info ) NN_NOEXCEPT;

    BufferInfoImpl() NN_NOEXCEPT;

    void SetDefault() NN_NOEXCEPT;

    void SetSize( size_t value ) NN_NOEXCEPT;

    void SetGpuAccessFlags( int value ) NN_NOEXCEPT;
};

}
}
}
