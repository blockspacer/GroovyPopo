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
#include <nn/gfx/gfx_DeviceData-api.gl.4-os.win32.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

#if !defined( NN_BUILD_CONFIG_OS_SUPPORTS_WIN32 )
    #error "OS 種別として Win32 が指定されていません。"
#endif

namespace nn {
namespace gfx {

class DeviceInfo;
class DisplayHandle;

namespace detail {

template<>
class DeviceImpl< ApiVariationGl4 >
    : public DataContainer< DeviceImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( DeviceImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef DeviceInfo InfoType;

    static const bool IsWorkingMemoryRequired = false;

    DeviceImpl() NN_NOEXCEPT;

    ~DeviceImpl() NN_NOEXCEPT;

    void Initialize( const InfoType& info ) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;
};

}
}
}
