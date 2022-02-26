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
#include <nn/gfx/gfx_StateData-api.nvn.8.h>

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
class RasterizerStateImpl< ApiVariationNvn8 >
    : public DataContainer< RasterizerStateImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;
    typedef RasterizerStateInfo InfoType;

    RasterizerStateImpl() NN_NOEXCEPT;

    ~RasterizerStateImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class BlendStateImpl< ApiVariationNvn8 >
    : public DataContainer< BlendStateImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;
    typedef BlendStateInfo InfoType;

    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = 8
    };

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
class DepthStencilStateImpl< ApiVariationNvn8 >
    : public DataContainer< DepthStencilStateImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;
    typedef DepthStencilStateInfo InfoType;

    DepthStencilStateImpl() NN_NOEXCEPT;

    ~DepthStencilStateImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class VertexStateImpl< ApiVariationNvn8 >
    : public DataContainer< VertexStateImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;
    typedef VertexStateInfo InfoType;

    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = 8
    };

    VertexStateImpl() NN_NOEXCEPT;

    ~VertexStateImpl() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT;

    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT;

    void* GetMemory() NN_NOEXCEPT;
    const void* GetMemory() const NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info,
        const ShaderImpl< Target >* pVertexShader = NULL ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class TessellationStateImpl< ApiVariationNvn8 >
    : public DataContainer< TessellationStateImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;
    typedef TessellationStateInfo InfoType;

    TessellationStateImpl() NN_NOEXCEPT;

    ~TessellationStateImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& infoType ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class ViewportScissorStateImpl< ApiVariationNvn8 >
    : public DataContainer< ViewportScissorStateImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;
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
