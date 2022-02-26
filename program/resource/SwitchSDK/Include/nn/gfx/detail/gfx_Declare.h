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

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_TargetConfig.h>

#include <nn/nn_TimeSpan.h>

namespace nn {
namespace gfx {

template< typename TTarget >
class TDevice;

template< typename TTarget >
class TSwapChain;

template< typename TTarget >
class TQueue;

template< typename TTarget >
class TMemoryPool;

template< typename TTarget >
class TCommandBuffer;

template< typename TTarget >
class TPipeline;

template< typename TTarget >
class TRootSignature;

template< typename TTarget >
class TDescriptorPool;

template< typename TTarget >
class TShader;

template< typename TTarget >
class TRasterizerState;

template< typename TTarget >
class TBlendState;

template< typename TTarget >
class TDepthStencilState;

template< typename TTarget >
class TVertexState;

template< typename TTarget >
class TTessellationState;

template< typename TTarget >
class TViewportScissorState;

template< typename TTarget >
class TSampler;

template< typename TTarget >
class TBuffer;

template< typename TTarget >
class TBufferTextureView;

template< typename TTarget >
class TTexture;

template< typename TTarget >
class TTextureView;

template< typename TTarget >
class TColorTargetView;

template< typename TTarget >
class TDepthStencilView;

template< typename TTarget >
class TFence;

template< typename TTarget >
class TSemaphore;

template< typename TTarget >
class TInteroperation;

template< typename TTarget >
class TTimestampBuffer;

template< typename TTarget >
class TQueryBuffer;

template< typename TTarget >
class TGraphicsPipelineStatisticsQueriesBuffer;

template< typename TTarget >
struct DeviceImplData;

template< typename TTarget >
struct SwapChainImplData;

template< typename TTarget >
struct QueueImplData;

template< typename TTarget >
struct MemoryPoolImplData;

template< typename TTarget >
struct CommandBufferImplData;

template< typename TTarget >
struct PipelineImplData;

template< typename TTarget >
struct RootSignatureImplData;

template< typename TTarget >
struct DescriptorPoolImplData;

template< typename TTarget >
struct ShaderImplData;

template< typename TTarget >
struct RasterizerStateImplData;

template< typename TTarget >
struct BlendStateImplData;

template< typename TTarget >
struct DepthStencilStateImplData;

template< typename TTarget >
struct VertexStateImplData;

template< typename TTarget >
struct TessellationStateImplData;

template< typename TTarget >
struct ViewportScissorStateImplData;

template< typename TTarget >
struct SamplerImplData;

template< typename TTarget >
struct BufferImplData;

template< typename TTarget >
struct BufferTextureViewImplData;

template< typename TTarget >
struct TextureImplData;

template< typename TTarget >
struct TextureViewImplData;

template< typename TTarget >
struct ColorTargetViewImplData;

template< typename TTarget >
struct DepthStencilViewImplData;

template< typename TTarget >
struct FenceImplData;

template< typename TTarget >
struct SemaphoreImplData;

struct ImageFormatProperty;

template< typename TTarget >
struct TimestampBufferImplData;

template< typename TTarget >
struct QueryBufferImplData;

template< typename TTarget = TargetConfig::Variation >
class TBufferInfo;

template< typename TTarget = TargetConfig::Variation >
class TDeviceInfo;

template< typename TTarget = TargetConfig::Variation >
class TMemoryPoolInfo;

template< typename TTarget = TargetConfig::Variation >
class TQueueInfo;

template< typename TTarget = TargetConfig::Variation >
class TSamplerInfo;

template< typename TTarget = TargetConfig::Variation >
class TTextureInfo;

template< typename TTarget >
struct BufferInfoImplData;

template< typename TTarget >
struct DeviceInfoImplData;

template< typename TTarget >
struct MemoryPoolInfoImplData;

template< typename TTarget >
struct QueueInfoImplData;

template< typename TTarget >
struct SamplerInfoImplData;

template< typename TTarget >
struct TextureInfoImplData;

namespace detail {

template< typename TTarget >
class DeviceImpl;

template< typename TTarget >
class SwapChainImpl;

template< typename TTarget >
class QueueImpl;

template< typename TTarget >
class MemoryPoolImpl;

template< typename TTarget >
class CommandBufferImpl;

template< typename TTarget >
class PipelineImpl;

template< typename TTarget >
class RootSignatureImpl;

template< typename TTarget >
class DescriptorPoolImpl;

template< typename TTarget >
class ShaderImpl;

template< typename TTarget >
class RasterizerStateImpl;

template< typename TTarget >
class BlendStateImpl;

template< typename TTarget >
class DepthStencilStateImpl;

template< typename TTarget >
class VertexStateImpl;

template< typename TTarget >
class TessellationStateImpl;

template< typename TTarget >
class ViewportScissorStateImpl;

template< typename TTarget >
class SamplerImpl;

template< typename TTarget >
class BufferImpl;

template< typename TTarget >
class BufferTextureViewImpl;

template< typename TTarget >
class TextureImpl;

template< typename TTarget >
class TextureViewImpl;

template< typename TTarget >
class ColorTargetViewImpl;

template< typename TTarget >
class DepthStencilViewImpl;

template< typename TTarget >
class FenceImpl;

template< typename TTarget >
class SemaphoreImpl;

template< typename TTarget >
class TimestampBufferImpl;

template< typename TTarget >
class QueryBufferImpl;

template< typename TTarget >
class BufferInfoImpl;

template< typename TTarget >
class DeviceInfoImpl;

template< typename TTarget >
class MemoryPoolInfoImpl;

template< typename TTarget >
class QueueInfoImpl;

template< typename TTarget >
class SamplerInfoImpl;

template< typename TTarget >
class TextureInfoImpl;

template< typename TTarget >
void Initialize() NN_NOEXCEPT;

template< typename TTarget >
void Finalize() NN_NOEXCEPT;

template< typename TTarget >
void GetImageFormatProperty( ImageFormatProperty*, DeviceImpl< TTarget >*, ImageFormat ) NN_NOEXCEPT;

template< typename TTarget >
nn::TimeSpan GetDuration( int64_t, int64_t ) NN_NOEXCEPT;

}
}
}
