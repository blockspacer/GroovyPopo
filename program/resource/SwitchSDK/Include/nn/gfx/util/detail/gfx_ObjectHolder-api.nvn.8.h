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
#include <nn/gfx.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {
namespace util {
namespace detail {

template<int TMemorySize>
struct ApplyHolderMemoryMinConstraint
{
    static const int MinValue = 1;

    enum
    {
        Value = (TMemorySize < MinValue) ? MinValue : TMemorySize
    };
};

template<int TBlendTargetCount>
struct BlendStateHolderRequiredMemorySize
{
    enum
    {
        Value = ApplyHolderMemoryMinConstraint<
            sizeof( NVNblendState ) * TBlendTargetCount
        >::Value
    };
};

template<int TVertexAttributeCount, int TVertexBufferCount>
struct VertexStateHolderRequiredMemorySize
{
    enum
    {
        // NVN では属性数ではなく最大スロット番号に依存するため、
        // 抽象化のために TVertexAttributeCount を使わずに最大値である 15 を指定します。
        Value = ApplyHolderMemoryMinConstraint<
            sizeof( NVNvertexAttribState ) * ( 15 + 1 )
            + sizeof( NVNvertexStreamState ) * TVertexBufferCount
        >::Value
    };
};

template<int TViewportCount>
struct ViewportScissorStateHolderRequiredMemorySize
{
    enum
    {
        Value = ApplyHolderMemoryMinConstraint<
            sizeof( float ) * 4 * ( TViewportCount - 1 )
            + sizeof( float ) * 2 * ( TViewportCount - 1 )
            + sizeof( int32_t ) * 4 * ( TViewportCount - 1 )
        >::Value
    };
};

template<int TBlendTargetCount, int TVertexBufferCount, int TVertexAttributeCount>
struct GraphicsPipelineHolderRequiredMemorySize
{
    enum
    {
        Value = ApplyHolderMemoryMinConstraint<
            BlendStateHolderRequiredMemorySize<TBlendTargetCount>::Value
            + VertexStateHolderRequiredMemorySize<TVertexBufferCount, TVertexAttributeCount>::Value
        >::Value
    };
};

struct ComputePipelineHolderRequiredMemorySize
{
    enum
    {
        Value = ApplyHolderMemoryMinConstraint<0>::Value
    };
};

template<int TDescriptorTableCount, int TDynamicDescriptorCount, int TTotalDescriptorRangeCount>
struct RootSignatureHolderRequiredMemorySize
{
    enum
    {
        Value = ApplyHolderMemoryMinConstraint<
            sizeof( nn::gfx::RootSignatureInfo::DataType )
            + sizeof( nn::gfx::DescriptorTableInfo::DataType ) * TDescriptorTableCount
            + sizeof( nn::gfx::DynamicDescriptorInfo::DataType ) * TDynamicDescriptorCount
            + sizeof( nn::gfx::DescriptorRangeInfo::DataType ) * TTotalDescriptorRangeCount
        >::Value
    };
};

}
}
}
}

NN_PRAGMA_POP_WARNINGS
