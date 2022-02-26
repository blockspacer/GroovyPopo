/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief グラフィックスライブラリーの API を宣言します。
 */

#pragma once

/**
 * @namespace nn::gfx
 * @brief グラフィックスライブラリーの名前空間です。
 */

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_Core.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Queue.h>
#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_Pipeline.h>
#include <nn/gfx/gfx_State.h>
#include <nn/gfx/gfx_Texture.h>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/gfx/gfx_Shader.h>
#include <nn/gfx/gfx_Sync.h>
#include <nn/gfx/gfx_SwapChain.h>
#include <nn/gfx/gfx_DescriptorPool.h>
#include <nn/gfx/gfx_RootSignature.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/gfx/gfx_DescriptorSlot.h>

#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/gfx_ResTexture.h>
#include <nn/gfx/gfx_BufferLayout.h>

#include <nn/gfx/gfx_Interop.h>
