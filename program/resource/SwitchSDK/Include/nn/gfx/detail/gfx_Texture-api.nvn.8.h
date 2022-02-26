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
#include <nn/gfx/gfx_TextureData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class TextureInfo;
class TextureViewInfo;
class ColorTargetViewInfo;
class DepthStencilViewInfo;

namespace detail {

template<>
class TextureImpl< ApiVariationNvn8 >
    : public DataContainer< TextureImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( TextureImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef TextureInfo InfoType;

    static const bool IsMemoryPoolRequired = true;

    static size_t CalculateMipDataAlignment( DeviceImpl< Target > *pDevice, const InfoType& info ) NN_NOEXCEPT;
    static size_t CalculateMipDataAlignment( DeviceImpl< Target > *pDevice, const TextureInfoImpl< Target >& info ) NN_NOEXCEPT;

    static size_t CalculateMipDataSize( DeviceImpl< Target > *pDevice, const InfoType& info ) NN_NOEXCEPT;
    static size_t CalculateMipDataSize( DeviceImpl< Target > *pDevice, const TextureInfoImpl< Target >& info ) NN_NOEXCEPT;

    static void CalculateMipDataOffsets( ptrdiff_t* pMipOffsets, DeviceImpl< Target > *pDevice, const InfoType& info ) NN_NOEXCEPT;
    static void CalculateMipDataOffsets( ptrdiff_t* pMipOffsets, DeviceImpl< Target > *pDevice, const TextureInfoImpl< Target >& info ) NN_NOEXCEPT;

    static size_t GetRowPitch( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;
    static size_t GetRowPitch( DeviceImpl< Target >* pDevice, const TextureInfoImpl< Target >& info ) NN_NOEXCEPT;

    TextureImpl() NN_NOEXCEPT;

    ~TextureImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info,
        MemoryPoolImpl< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;
    void Initialize( DeviceImpl< Target >* pDevice, const TextureInfoImpl< Target >& info,
        MemoryPoolImpl< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void SetDebugLabel( DeviceImpl< Target >* pDevice, const char* label ) NN_NOEXCEPT;
};

template<>
class TextureViewImpl< ApiVariationNvn8 >
    : public DataContainer< TextureViewImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( TextureViewImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef TextureViewInfo InfoType;

    TextureViewImpl() NN_NOEXCEPT;

    ~TextureViewImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class ColorTargetViewImpl< ApiVariationNvn8 >
    : public DataContainer< ColorTargetViewImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( ColorTargetViewImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef ColorTargetViewInfo InfoType;

    ColorTargetViewImpl() NN_NOEXCEPT;

    ~ColorTargetViewImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
class DepthStencilViewImpl< ApiVariationNvn8 >
    : public DataContainer< DepthStencilViewImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( DepthStencilViewImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef DepthStencilViewInfo InfoType;

    DepthStencilViewImpl() NN_NOEXCEPT;

    ~DepthStencilViewImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

template<>
void GetImageFormatProperty< ApiVariationNvn8 >( ImageFormatProperty* pOutImageFormatProperty,
    DeviceImpl< ApiVariationNvn8 >* pDevice, ImageFormat imageFormat ) NN_NOEXCEPT;

}
}
}
