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
#include <nn/nn_TimeSpan.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_SyncData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class FenceInfo;
class SemaphoreInfo;

namespace detail {

template<>
class FenceImpl< ApiVariationGl4 >
    : public DataContainer< FenceImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( FenceImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef FenceInfo InfoType;

    FenceImpl() NN_NOEXCEPT;

    ~FenceImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< ApiVariationGl4 >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< ApiVariationGl4 >* pDevice ) NN_NOEXCEPT;

    bool IsSignaled() const NN_NOEXCEPT;

    SyncResult Sync( TimeSpan timeout ) const NN_NOEXCEPT;
};

template<>
class SemaphoreImpl< ApiVariationGl4 >
    : public DataContainer< SemaphoreImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( SemaphoreImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef SemaphoreInfo InfoType;

    SemaphoreImpl() NN_NOEXCEPT;

    ~SemaphoreImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< ApiVariationGl4 >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< ApiVariationGl4 >* pDevice ) NN_NOEXCEPT;
};

}
}
}
