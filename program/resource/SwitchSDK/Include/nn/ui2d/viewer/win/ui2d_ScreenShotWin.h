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

#include <nn/nn_Allocator.h>
#include <nn/ui2d/viewer/ui2d_Config.h>
#include <nn/gfx.h>

#if defined(NN_UI2D_VIEWER_ENABLED)


namespace nn
{

namespace ui2d
{
namespace viewer
{

//======================================
// スクリーンショットを取る
//======================================
class ScreenShot
{
public:
    static const int ColorComponentSize = 4; //!< キャプチャするバッファの１ピクセルのサイズ

    enum State
    {
        State_NotReady,
        State_ReadyToTake,
        State_Done
    };

    ScreenShot()
         : m_state(State_NotReady)
         , m_frame(0.f)
         , m_fileName(NULL)
         , m_width(720)
         , m_height(1280)
         , m_Buffer(NULL)
         , m_pDevice(NULL)
         , m_pMemoryPool(NULL)
    {
    }

    void SetSize(int  width, int  height) { m_width = width; m_height = height; }

    void SetFrame(float frame) { m_frame = frame; }

    float GetFrame() const { return m_frame; }

    void SetFileName(const char* filename) { m_fileName = filename; }

    bool IsNeedToTake() const { return m_fileName != NULL; }

    State GetState() const { return m_state; }

    size_t GetRequiredMemoryPoolSize() const;
    size_t GetRequiredMemoryPoolAlignment(nn::gfx::Device* pDevice) const;

    void Prepare(
        nn::gfx::Device* pDevice,
        nn::gfx::MemoryPool* pMemoryPool,
        ptrdiff_t* pMemoryPoolOffset,
        nn::AlignedAllocateFunctionWithUserData pAllocateFunction, void* pUserDataForAllocator);

    void PutCommandScreenShot(nn::gfx::CommandBuffer* pCommandBuffer, nn::gfx::Texture* pSrcTexture);

    void Take(nn::FreeFunctionWithUserData pFreeFunction, void* pUserDataForAllocator);

private:
    State m_state;
    float m_frame;
    const char* m_fileName;
    int32_t  m_width;
    int32_t  m_height;
    uint8_t * m_Buffer;

    nn::gfx::Device* m_pDevice;
    nn::gfx::MemoryPool* m_pMemoryPool;
    nn::gfx::Buffer m_CaptureBuffer;

    static const int PixelSize = 4;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
