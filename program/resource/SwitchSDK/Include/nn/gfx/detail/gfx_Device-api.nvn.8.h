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

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_DeviceData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class DeviceInfo;
class DisplayHandle;

namespace detail {

template<>
class DeviceImpl< ApiVariationNvn8 >
    : public DataContainer< DeviceImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( DeviceImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef DeviceInfo InfoType;

    DeviceImpl() NN_NOEXCEPT;

    ~DeviceImpl() NN_NOEXCEPT;

    void Initialize( const InfoType& info ) NN_NOEXCEPT;
    void Initialize( const DeviceInfoImpl< Target >& info ) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;
};

}
}
}
