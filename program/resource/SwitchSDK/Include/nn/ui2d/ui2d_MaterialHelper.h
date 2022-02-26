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

#include <nn/ui2d/ui2d_Resources.h>
#include <nn/ui2d/ui2d_GraphicsResource.h>
#include <nn/ui2d/ui2d_Material.h>

// SetBlendStateToCommandBuffer 用にインクルード
#if defined NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK

#if defined(NN_BUILD_CONFIG_SPEC_GENERIC)
#elif defined(NN_BUILD_CONFIG_SPEC_NX)
#include <nvn/nvn.h>
#include <nvn/nvn_FuncPtrInline.h>
#endif

#endif // NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK

namespace nn
{
namespace ui2d
{

//----------------------------------------
inline float
ColorU8ToFloat(uint8_t  value)
{
    return 1.0f / 255.0f * value;
}

//----------------------------------------
inline void
InitializeTexSrt(ResTexSrt* pTexSrts, int  count)
{
    for (int  i = 0; i < count; ++i)
    {
        pTexSrts[i].translate.Set(0.f, 0.f);
        pTexSrts[i].rotate    = 0.f;
        pTexSrts[i].scale.Set(1.f, 1.f);
    }
}

//----------------------------------------
inline void LoadTexCoordMatrix(Material::ConstantBufferForVertexShader* pVertexShaderConstantBuffer, int texIdx, float (*v)[3])
{
    NN_SDK_ASSERT_RANGE(texIdx, 0, TexMapMax);

    // 2 x 4 行列をロード。
    float* texMtxXZ = NULL;
    float* texMtxYW = NULL;
    switch (texIdx)
    {
        case 0:
            texMtxXZ = pVertexShaderConstantBuffer->texMtx0XZ;
            texMtxYW = pVertexShaderConstantBuffer->texMtx0YW;
            break;
        case 1:
            texMtxXZ = pVertexShaderConstantBuffer->texMtx1XZ;
            texMtxYW = pVertexShaderConstantBuffer->texMtx1YW;
            break;
        case 2:
            texMtxXZ = pVertexShaderConstantBuffer->texMtx2XZ;
            texMtxYW = pVertexShaderConstantBuffer->texMtx2YW;
            break;
        default:
            NN_SDK_ASSERT(false, "Wrong texture index.");
            return;
    }
    texMtxXZ[0] = v[0][0];
    texMtxXZ[1] = v[0][1];
    texMtxXZ[2] = 0.0f;
    texMtxXZ[3] = v[0][2];
    texMtxYW[0] = v[1][0];
    texMtxYW[1] = v[1][1];
    texMtxYW[2] = 0.0f;
    texMtxYW[3] = v[1][2];
}

//----------------------------------------
inline void LoadTexCoordMatrix(const DrawInfo& drawInfo, Material& material, int texIdx, float(*v)[3])
{
    LoadTexCoordMatrix(material.GetConstantBufferForVertexShader(drawInfo), texIdx, v);
}

inline void SetBlendStateToCommandBuffer(nn::gfx::CommandBuffer& commandBuffer, const nn::gfx::BlendState* pBlendState)
{
#if !(defined NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK)

    commandBuffer.SetBlendState(pBlendState);

#else // NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK

#if NN_GFX_IS_TARGET_NVN
    // NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK が指定された場合、
    // nvn では、SetBlendState の中で、ChannelMaskState の設定をスキップします。
    const nn::gfx::BlendStateImplData< nn::gfx::ApiVariationNvn8 >& blendState = pBlendState->ToData();
    const NVNblendState* pBlendStates = blendState.pNvnBlendStateData;
    for (int idxTarget = 0, targetCount = static_cast<int>(blendState.targetCount); idxTarget < targetCount; ++idxTarget)
    {
        nvnCommandBufferBindBlendState(commandBuffer.ToData()->pNvnCommandBuffer, pBlendStates + idxTarget);
    }

    // ChannelMaskState は設定しない
    //nvnCommandBufferBindChannelMaskState(commandBuffer.ToData()->pNvnCommandBuffer, reinterpret_cast<const NVNchannelMaskState*>(&blendState.nvnChannelMaskState));

    nvnCommandBufferBindColorState(commandBuffer.ToData()->pNvnCommandBuffer, reinterpret_cast<const NVNcolorState*>(&blendState.nvnColorState));

    nvnCommandBufferSetBlendColor(commandBuffer.ToData()->pNvnCommandBuffer, blendState.nvnBlendConstant);
#else
    commandBuffer.SetBlendState(pBlendState);
#endif

#endif // NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK

}



} // namespace ui2d
} // namespace nn
