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
#include <nn/gfx/gfx_VariationBase.h>

#include <nn/gfx/detail/gfx_Declare.h>

namespace nn {
namespace gfx {

class ResShaderProgram;
class ResShaderContainer;

namespace detail {

class ResShaderProgramImpl
{
public:
    template< typename TTarget >
    static size_t NvnGetRecommendedScrachMemorySize(
        const ResShaderProgram* pThis, DeviceImpl< TTarget >* pDevice ) NN_NOEXCEPT;
};

class ResShaderContainerImpl
{
public:
    template< typename TTarget >
    static void Initialize( ResShaderContainer* pThis, DeviceImpl< TTarget >* pDevice ) NN_NOEXCEPT
    {
        Initialize< TTarget >( pThis, pDevice, NULL, 0, 0 );
    }

    template< typename TTarget >
    static void Initialize( ResShaderContainer* pThis, DeviceImpl< TTarget >* pDevice,
        MemoryPoolImpl< TTarget >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    template< typename TTarget >
    static void Finalize( ResShaderContainer* pThis, DeviceImpl< TTarget >* pDevice ) NN_NOEXCEPT;
};

template<>
void ResShaderContainerImpl::Initialize< ApiVariation< ApiType< 2 >, ApiVersion< 2 > > >(
    ResShaderContainer*, DeviceImpl< ApiVariation< ApiType< 2 >, ApiVersion< 2 > > >*,
    MemoryPoolImpl< ApiVariation< ApiType< 2 >, ApiVersion< 2 > > >*, ptrdiff_t, size_t ) NN_NOEXCEPT;
template<>
void ResShaderContainerImpl::Finalize< ApiVariation< ApiType< 2 >, ApiVersion< 2 > > >(
    ResShaderContainer*, DeviceImpl< ApiVariation< ApiType< 2 >, ApiVersion< 2 > > >* ) NN_NOEXCEPT;

}
}
}
