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
#include <nn/gfx/gfx_BufferData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class BufferInfo;
class BufferTextureViewInfo;
class GpuAddress;

namespace detail {

template<>
class BufferImpl< ApiVariationGl4 >
    : public DataContainer< BufferImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( BufferImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef BufferInfo InfoType;

    static const bool IsMemoryPoolRequired = false;

    static size_t GetBufferAlignment( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    BufferImpl() NN_NOEXCEPT;

    ~BufferImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info,
        MemoryPoolImpl< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void* Map() const NN_NOEXCEPT;

    template< typename T >
    T* Map() const NN_NOEXCEPT
    {
        return static_cast< T* >( Map() );
    }

    void Unmap() const NN_NOEXCEPT;

    void FlushMappedRange( ptrdiff_t offset, size_t size ) const NN_NOEXCEPT;

    void InvalidateMappedRange( ptrdiff_t offset, size_t size ) const NN_NOEXCEPT;

    void GetGpuAddress( GpuAddress* pOutGpuAddress ) const NN_NOEXCEPT;

    void SetDebugLabel( DeviceImpl< Target >* pDevice, const char* label ) NN_NOEXCEPT;
};

template<>
class BufferTextureViewImpl< ApiVariationGl4 >
    : public DataContainer< BufferTextureViewImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( BufferTextureViewImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef BufferTextureViewInfo InfoType;

    static size_t GetOffsetAlignment( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    BufferTextureViewImpl() NN_NOEXCEPT;

    ~BufferTextureViewImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

}
}
}
