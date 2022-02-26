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
#include <nn/gfx/gfx_QueueData-api.gl.4-os.win32.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

#if !defined( NN_BUILD_CONFIG_OS_SUPPORTS_WIN32 )
    #error
#endif

namespace nn {
namespace gfx {

class QueueInfo;

namespace detail {

template<>
class QueueImpl< ApiVariationGl4 >
    : public DataContainer< QueueImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( QueueImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef QueueInfo InfoType;

    QueueImpl() NN_NOEXCEPT;

    ~QueueImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void ExecuteCommand( const CommandBufferImpl< Target >* pCommandBuffer,
        FenceImpl< Target >* pFence ) NN_NOEXCEPT;

    void Flush() NN_NOEXCEPT;

    void Sync() NN_NOEXCEPT;

    void SetSemaphore( SemaphoreImpl< Target >* pSemaphore ) NN_NOEXCEPT;

    void SyncSemaphore( const SemaphoreImpl< Target >* pSemaphore ) NN_NOEXCEPT;

    void CopyToScanBuffer( SwapChainImpl< Target >* pSwapChain,
        const ColorTargetViewImpl< Target >* pColorTarget ) NN_NOEXCEPT;

    void Present( SwapChainImpl< Target >* pSwapChain, int presentInterval ) NN_NOEXCEPT;
};

}
}
}
