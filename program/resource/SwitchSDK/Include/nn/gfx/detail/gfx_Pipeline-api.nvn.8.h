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
#include <nn/gfx/gfx_PipelineData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/detail/gfx_State-api.nvn.8.h>

namespace nn {
namespace gfx {

class GraphicsPipelineInfo;
class ComputePipelineInfo;

namespace detail {

template<>
class PipelineImpl< ApiVariationNvn8 >
    : public DataContainer< PipelineImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( PipelineImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef GraphicsPipelineInfo GraphicsInfoType;
    typedef ComputePipelineInfo ComputeInfoType;
    typedef GraphicsInfoType InfoType;

    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = 8
    };

    PipelineImpl() NN_NOEXCEPT;

    ~PipelineImpl() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize( const GraphicsInfoType& info ) NN_NOEXCEPT;
    static size_t GetRequiredMemorySize( const ComputeInfoType& info ) NN_NOEXCEPT;

    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT;

    void* GetMemory() NN_NOEXCEPT;
    const void* GetMemory() const NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const GraphicsInfoType& info ) NN_NOEXCEPT;
    void Initialize( DeviceImpl< Target >* pDevice, const ComputeInfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
};

}
}
}
