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
#include <nn/gfx/gfx_StateData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class RasterizerStateInfo;
class BlendStateInfo;
class DepthStencilStateInfo;
class VertexStateInfo;
class TessellationStateInfo;
class ViewportScissorStateInfo;

namespace detail {

template<>
class RasterizerStateImpl< ApiVariationGl4 >
    : public DataContainer< RasterizerStateImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;
    typedef RasterizerStateInfo InfoType;

    RasterizerStateImpl() NN_NOEXCEPT;

    ~RasterizerStateImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class BlendStateImpl< ApiVariationGl4 >
    : public DataContainer< BlendStateImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;
    typedef BlendStateInfo InfoType;

    BlendStateImpl() NN_NOEXCEPT;

    ~BlendStateImpl() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT;

    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT;

    void* GetMemory() NN_NOEXCEPT;
    const void* GetMemory() const NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class DepthStencilStateImpl< ApiVariationGl4 >
    : public DataContainer< DepthStencilStateImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;
    typedef DepthStencilStateInfo InfoType;

    DepthStencilStateImpl() NN_NOEXCEPT;

    ~DepthStencilStateImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class VertexStateImpl< ApiVariationGl4 >
    : public DataContainer< VertexStateImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;
    typedef VertexStateInfo InfoType;

    VertexStateImpl() NN_NOEXCEPT;

    ~VertexStateImpl() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT;

    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT;

    void* GetMemory() NN_NOEXCEPT;
    const void* GetMemory() const NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info,
        const ShaderImpl< Target >* pVertexShader ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class TessellationStateImpl< ApiVariationGl4 >
    : public DataContainer< TessellationStateImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;
    typedef TessellationStateInfo InfoType;

    TessellationStateImpl() NN_NOEXCEPT;

    ~TessellationStateImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class ViewportScissorStateImpl< ApiVariationGl4 >
    : public DataContainer< ViewportScissorStateImplData< ApiVariationGl4 > >
{
public:
    typedef ApiVariationGl4 Target;
    typedef ViewportScissorStateInfo InfoType;

    ViewportScissorStateImpl() NN_NOEXCEPT;

    ~ViewportScissorStateImpl() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT;

    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT;

    void* GetMemory() NN_NOEXCEPT;
    const void* GetMemory() const NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

}
}
}
