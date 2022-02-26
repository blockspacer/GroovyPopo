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

#if !defined( NN_BUILD_CONFIG_OS_SUPPORTS_WIN32 )
    #error "OS 種別として Win32 が指定されていません。"
#endif

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_SwapChainData-api.gl.4-os.win32.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class SwapChainInfo;

namespace detail {

template<>
class SwapChainImpl< ApiVariationGl4 >
    : public DataContainer< SwapChainImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( SwapChainImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef SwapChainInfo InfoType;

    static const bool IsMemoryPoolRequired = false;

    static size_t GetScanBufferAlignment( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    static size_t CalculateScanBufferSize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    SwapChainImpl() NN_NOEXCEPT;

    ~SwapChainImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info,
        MemoryPoolImpl< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    int GetScanBufferViews( TColorTargetView< Target >** ppOutScanBufferArray,
        int maxScanBufferCount ) NN_NOEXCEPT;

    int GetScanBuffers( TTexture< Target >** ppOutScanBufferTextureArray,
        int maxScanBufferTextureCount ) NN_NOEXCEPT;

    AcquireScanBufferResult AcquireNextScanBufferIndex( int* pOutScanBufferIndex,
        SemaphoreImpl< Target >* pSemaphore, FenceImpl< Target >* pFence ) NN_NOEXCEPT;

    int AcquireNextScanBufferIndex() NN_NOEXCEPT;

    ColorTargetViewImpl< Target >* AcquireNextScanBufferView() NN_NOEXCEPT;

    TextureImpl< Target >* AcquireNextScanBuffer() NN_NOEXCEPT;

    ColorTargetViewImpl< Target >* GetCurrentScanBuffer() NN_NOEXCEPT;

    TextureImpl< Target >* GetCurrentScanBufferTexture() NN_NOEXCEPT;
};

}
}
}
