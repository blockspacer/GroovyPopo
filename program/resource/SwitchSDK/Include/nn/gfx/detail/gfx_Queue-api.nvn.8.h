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
#include <nn/gfx/gfx_QueueData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class QueueInfo;

namespace detail {

template<>
class QueueImpl< ApiVariationNvn8 >
    : public DataContainer< QueueImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( QueueImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef QueueInfo InfoType;

    QueueImpl() NN_NOEXCEPT;

    ~QueueImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;
    void Initialize( DeviceImpl< Target >* pDevice, const QueueInfoImpl< Target >& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void ExecuteCommand( CommandBufferImpl< Target >* pCommandBuffer,
        FenceImpl< Target >* pFence ) NN_NOEXCEPT;

    void Flush() const NN_NOEXCEPT;

    void Sync() const NN_NOEXCEPT;

    void SetSemaphore( SemaphoreImpl< Target >* pSemaphore ) NN_NOEXCEPT;

    void SyncSemaphore( const SemaphoreImpl< Target >* pSemaphore ) NN_NOEXCEPT;

    void CopyToScanBuffer( SwapChainImpl< Target >* pSwapChain,
        const ColorTargetViewImpl< Target >* pColorTarget ) NN_NOEXCEPT;

    void Present( SwapChainImpl< Target >* pSwapChain, int presentInterval ) NN_NOEXCEPT;
};

}
}
}
