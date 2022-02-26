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
#include <nn/gfx/gfx_SamplerData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class SamplerInfo;

namespace detail {

template<>
class SamplerImpl< ApiVariationGl4 >
    : public DataContainer< SamplerImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( SamplerImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef SamplerInfo InfoType;

    SamplerImpl() NN_NOEXCEPT;

    ~SamplerImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void SetDebugLabel( DeviceImpl< Target >* pDevice, const char* label ) NN_NOEXCEPT;
};

}
}
}
