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

#include <nn/nn_SdkAssert.h>
#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DescriptorPoolData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class GpuAddress;
class DescriptorSlot;
class DescriptorPoolInfo;

namespace detail {

template<>
class DescriptorPoolImpl< ApiVariationGl4 >
    : public DataContainer< DescriptorPoolImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( DescriptorPoolImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef DescriptorPoolInfo InfoType;

    static size_t CalculateDescriptorPoolSize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    static ptrdiff_t GetDescriptorSlotIncrementSize( DeviceImpl< Target >* pDevice, DescriptorPoolType type ) NN_NOEXCEPT;

    static size_t GetDescriptorPoolAlignment(
        DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    DescriptorPoolImpl() NN_NOEXCEPT;

    ~DescriptorPoolImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info,
        MemoryPoolImpl< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void BeginUpdate() NN_NOEXCEPT;
    void EndUpdate() NN_NOEXCEPT;

    void SetBufferView( int indexSlot, const GpuAddress& gpuAddress, size_t size ) NN_NOEXCEPT;
    void SetSampler( int indexSlot, const SamplerImpl< Target >* pSamlper ) NN_NOEXCEPT;
    void SetTextureView( int indexSlot, const TextureViewImpl< Target >* pTextureView ) NN_NOEXCEPT;
    void SetImage( int indexSlot, const TextureViewImpl< Target >* pImage ) NN_NOEXCEPT;
    void SetBufferTextureView( int indexSlot, const BufferTextureViewImpl< Target >* pBufferTextureView ) NN_NOEXCEPT;
    void SetBufferImage( int indexSlot, const BufferTextureViewImpl< Target >* pBufferTextureView ) NN_NOEXCEPT;

    void GetDescriptorSlot( DescriptorSlot* pOutDescriptorSlot, int indexSlot ) const NN_NOEXCEPT;

    int GetDescriptorSlotIndex( const DescriptorSlot& descriptorSlot ) const NN_NOEXCEPT;
};

}
}
}
