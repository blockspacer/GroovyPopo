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
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_CommandBufferData-api.gl.4.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class CommandBufferInfo;
class TextureArrayRange;
class TextureSubresource;
class TextureCopyRegion;
class BufferTextureCopyRegion;
class TextureSubresourceRange;
class GpuAddress;
class DescriptorSlot;
class ViewportStateInfo;
class ScissorStateInfo;

namespace detail {

template<>
class CommandBufferImpl< ApiVariationGl4 >
    : public DataContainer< CommandBufferImplData< ApiVariationGl4 > >
{
    NN_DISALLOW_COPY( CommandBufferImpl );

public:
    typedef ApiVariationGl4 Target;
    typedef CommandBufferInfo InfoType;

    typedef void ( *OutOfMemoryEventCallback )(
        TCommandBuffer< ApiVariationGl4 >* pCommandBuffer, const OutOfMemoryEventArg& arg );

    static size_t GetCommandMemoryAlignment( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;
    static size_t GetControlMemoryAlignment( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    CommandBufferImpl() NN_NOEXCEPT;

    ~CommandBufferImpl() NN_NOEXCEPT;

    void Initialize( DeviceImpl< Target >* pDevice, const InfoType& Info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    void AddCommandMemory( MemoryPoolImpl< Target >* pMemoryPool,
        ptrdiff_t memoryPoolOffset, size_t memorySize ) NN_NOEXCEPT;
    void AddControlMemory( void* pMemory, size_t memorySize ) NN_NOEXCEPT;
    void SetOutOfCommandMemoryEventCallback( OutOfMemoryEventCallback pEventCallback ) NN_NOEXCEPT;
    void SetOutOfControlMemoryEventCallback( OutOfMemoryEventCallback pEventCallback ) NN_NOEXCEPT;

    void Reset() NN_NOEXCEPT;

    void Begin() NN_NOEXCEPT;

    void End() NN_NOEXCEPT;

    void Dispatch( int groupCountX, int groupCountY, int groupCountZ ) NN_NOEXCEPT;
    void Draw( PrimitiveTopology primitiveTopology, int vertexCount, int vertexOffset ) NN_NOEXCEPT;
    void Draw( PrimitiveTopology primitiveTopology, int vertexCountPerInstance,
        int vertexOffset, int instanceCount, int baseInstance ) NN_NOEXCEPT;
    void DrawIndexed( PrimitiveTopology primitiveTopology, IndexFormat indexFormat,
        const GpuAddress& indexBufferAddress, int indexCount, int baseVertex ) NN_NOEXCEPT;
    void DrawIndexed( PrimitiveTopology primitiveTopology, IndexFormat indexFormat,
        const GpuAddress& indexBufferAddress, int indexCountPerInstance, int baseVertex,
        int instanceCount, int baseInstance ) NN_NOEXCEPT;

    void DispatchIndirect( const GpuAddress& indirectBufferAddress ) NN_NOEXCEPT;
    void DrawIndirect( PrimitiveTopology primitiveTopology, const GpuAddress& indirectBufferAddress ) NN_NOEXCEPT;
    void DrawIndexedIndirect( PrimitiveTopology primitiveTopology, IndexFormat indexFormat,
        const GpuAddress& indexBufferAddress, const GpuAddress& indirectBufferAddress ) NN_NOEXCEPT;

    void SetPipeline( const PipelineImpl< Target >* pPipeline ) NN_NOEXCEPT;

    void SetRenderTargets( int colorTargetCount, const ColorTargetViewImpl< Target >* const * ppColorTargets,
        const DepthStencilViewImpl< Target >* pDepthStencil ) NN_NOEXCEPT;

    void SetVertexBuffer( int bufferIndex, const GpuAddress& vertexBufferAddress,
        ptrdiff_t stride, size_t size ) NN_NOEXCEPT;

    void SetViewportScissorState( const ViewportScissorStateImpl< Target >* pViewportScissor ) NN_NOEXCEPT;

    void CopyBuffer( BufferImpl< Target >* pDstBuffer, ptrdiff_t dstOffset,
        const BufferImpl< Target >* pSrcBuffer, ptrdiff_t srcOffset, size_t size ) NN_NOEXCEPT;
    void CopyImage( TextureImpl< Target >* pDstTexture, const TextureSubresource& dstSubresource,
        int dstOffsetU, int dstOffsetV, int dstOffsetW, const TextureImpl< Target >* pSrcTexture,
        const TextureCopyRegion& srcCopyRegion ) NN_NOEXCEPT;
    void CopyBufferToImage( TextureImpl< Target >* pDstTexture, const BufferImpl< Target >* pSrcBuffer,
        const BufferTextureCopyRegion& copyRegion ) NN_NOEXCEPT;
    void CopyImageToBuffer( BufferImpl< Target >* pDstBuffer, const TextureImpl< Target >* pSrcTexture,
        const BufferTextureCopyRegion& copyRegion ) NN_NOEXCEPT;
    void CopyBufferToImage( TextureImpl< Target >* pDstTexture, const TextureCopyRegion& dstRegion,
        const BufferImpl< Target >* pSrcBuffer, ptrdiff_t srcOffset ) NN_NOEXCEPT;
    void CopyImageToBuffer( BufferImpl< Target >* pDstBuffer, ptrdiff_t dstOffset,
        const TextureImpl< Target >* pSrcTexture, const TextureCopyRegion& srcRegion ) NN_NOEXCEPT;

    void BlitImage( TextureImpl< Target >* pDstTexture, const TextureCopyRegion& dstRegion,
        const TextureImpl< Target >* pSrcTexture, const TextureCopyRegion& srcRegion, int copyFlags ) NN_NOEXCEPT;

    void ClearBuffer( BufferImpl< Target >* pBuffer, ptrdiff_t offset, size_t size, uint32_t value ) NN_NOEXCEPT;
    void ClearColor( ColorTargetViewImpl< Target >* pColorTarget, float red, float green,
        float blue, float alpha, const TextureArrayRange* pArrayRange ) NN_NOEXCEPT;
    void ClearColorTarget( ColorTargetViewImpl< Target >* pColorTarget,
        const ClearColorValue& clearColor, const TextureArrayRange* pArrayRange ) NN_NOEXCEPT;
    void ClearDepthStencil( DepthStencilViewImpl< Target >* pDepthStencil, float depth,
        int stencil, DepthStencilClearMode clearMode, const TextureArrayRange* pArrayRange ) NN_NOEXCEPT;
    void Resolve( TextureImpl< Target >* pDstTexture, int dstMipLevel, int dstStartArrayIndex,
        const ColorTargetViewImpl< Target >* pSrcColorTarget, const TextureArrayRange* pSrcArrayRange ) NN_NOEXCEPT;

    void FlushMemory( int gpuAccessFlags ) NN_NOEXCEPT;
    void InvalidateMemory( int gpuAccessFlags ) NN_NOEXCEPT;

    void CallCommandBuffer( const CommandBufferImpl< Target >* pNestedCommandBuffer ) NN_NOEXCEPT;
    void CopyCommandBuffer( const CommandBufferImpl< Target >* pNestedCommandBuffer ) NN_NOEXCEPT;

    void SetBufferStateTransition( BufferImpl< Target >* pBuffer, int oldState,
        int oldStageBits, int newState, int newStageBits ) NN_NOEXCEPT;
    void SetTextureStateTransition( TextureImpl< Target >* pTexture, const TextureSubresourceRange* pRange,
        int oldState, int oldStageBits, int newState, int newStageBits ) NN_NOEXCEPT;

    void SetDescriptorPool( const DescriptorPoolImpl< Target >* pDescriptorPool ) NN_NOEXCEPT;

    void SetRootSignature( PipelineType pipelineType, RootSignatureImpl< Target >* pRootSignature ) NN_NOEXCEPT;

    void SetRootBufferDescriptorTable( PipelineType pipelineType,
        int indexDescriptorTable, const DescriptorSlot& startBufferDescriptorSlot ) NN_NOEXCEPT;
    void SetRootTextureAndSamplerDescriptorTable( PipelineType pipelineType, int indexDescriptorTable,
        const DescriptorSlot& startTextureDescriptorSlot, const DescriptorSlot& startSamplerDescriptorSlot ) NN_NOEXCEPT;
    void SetRootConstantBuffer( PipelineType pipelineType, int indexDynamicDescriptor,
        const GpuAddress& constantBufferAddress, size_t size ) NN_NOEXCEPT;
    void SetRootUnorderedAccessBuffer( PipelineType pipelineType, int indexDynamicDescriptor,
        const GpuAddress& unorderedAccessBufferAddress, size_t size ) NN_NOEXCEPT;
    void SetRootTextureAndSampler( PipelineType pipelineType, int indexDynamicDescriptor,
        const TextureViewImpl< Target >* pTextureView, const SamplerImpl< Target >* pSampler ) NN_NOEXCEPT;

    void BeginQuery( QueryTarget target ) NN_NOEXCEPT;
    void EndQuery( const GpuAddress& dstBufferAddress, QueryTarget target ) NN_NOEXCEPT;
    void WriteTimestamp( const GpuAddress& dstBufferAddress ) NN_NOEXCEPT;

    void SetDepthBounds( float minDepthBounds, float maxDepthBounds ) NN_NOEXCEPT;

    void SetLineWidth( float lineWidth ) NN_NOEXCEPT;

    void SetViewports( int firstViewport, int viewportCount,
        const ViewportStateInfo* pViewports ) NN_NOEXCEPT;
    void SetScissors( int firstScissor, int scissorCount,
        const ScissorStateInfo* pScissors ) NN_NOEXCEPT;

    void UpdateBuffer( const GpuAddress& dstBufferAddress, size_t bufferSize,
        ptrdiff_t dstOffset, size_t dataSize, const void* pData ) NN_NOEXCEPT;

    void PushDebugGroup( const char* description ) NN_NOEXCEPT;
    void PopDebugGroup() NN_NOEXCEPT;

    // 以下はルートシグネチャを使わない場合
    void SetConstantBuffer( int slot, ShaderStage stage,
        const DescriptorSlot& constantBufferDescriptor ) NN_NOEXCEPT;
    void SetUnorderedAccessBuffer( int slot, ShaderStage stage,
        const DescriptorSlot& unorderedAccessBufferDescriptor ) NN_NOEXCEPT;
    void SetTextureAndSampler( int slot, ShaderStage stage,
        const DescriptorSlot& textureDescriptor, const DescriptorSlot& samplerDescriptor ) NN_NOEXCEPT;
    void SetTexture( int slot, ShaderStage stage, const DescriptorSlot& textureDescriptor ) NN_NOEXCEPT;
    void SetImage( int slot, ShaderStage stage, const DescriptorSlot& imageDescriptor ) NN_NOEXCEPT;

    // 以下はデスクリプタープールを使わない場合
    void SetConstantBuffer( int slot, ShaderStage stage,
        const GpuAddress& constantBufferAddress, size_t size ) NN_NOEXCEPT;
    void SetUnorderedAccessBuffer( int slot, ShaderStage stage,
        const GpuAddress& unorderedAccessBufferAddress, size_t size ) NN_NOEXCEPT;
    void SetTextureAndSampler( int slot, ShaderStage stage,
        const TextureViewImpl< Target >* pTextureView,
        const SamplerImpl< Target >* pSampler ) NN_NOEXCEPT;
    void SetImage( int slot, ShaderStage stage, const TextureViewImpl< Target >* pImage ) NN_NOEXCEPT;

    // 以下はパイプラインを使わない場合
    void SetShader( const ShaderImpl< Target >* pShader, int stageBits ) NN_NOEXCEPT;
    void SetRasterizerState( const RasterizerStateImpl< Target >* pRasterizerState ) NN_NOEXCEPT;
    void SetBlendState( const BlendStateImpl< Target >* pBlendState ) NN_NOEXCEPT;
    void SetDepthStencilState( const DepthStencilStateImpl< Target >* pDepthStencilState ) NN_NOEXCEPT;
    void SetVertexState( const VertexStateImpl< Target >* pVertexState ) NN_NOEXCEPT;
    void SetTessellationState( const TessellationStateImpl< Target >* pTessellationState ) NN_NOEXCEPT;

    // GL4 専用インターフェイス
    typedef void ( *Gl4UserCommandCallbackType )( const void* pParam );
    void Gl4SetUserCommand( Gl4UserCommandCallbackType pCallback, const void* pParam ) NN_NOEXCEPT;
    void Gl4SetUserCommandDynamic( Gl4UserCommandCallbackType pCallback, const void* pParam, size_t paramSize ) NN_NOEXCEPT;
};

}
}
}
