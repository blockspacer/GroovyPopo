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

#include <nn/os/os_ThreadCommon.h>

#include <nn/font/font_ScalableFont.h>
#include <nn/font/font_TextureCache.h>


#include <nn/util/util_FormatString.h>
#include <nn/util/util_StringUtil.h>

#include <nn/ui2d.h>
#include <nn/font.h>

#include <nn/ui2d/ui2d_Screen.h>




#include <cstdarg>
namespace nn
{
namespace ui2d
{

//! @brief アニメーションの一覧を保持します。
//!
typedef nn::util::IntrusiveList<Screen, nn::util::IntrusiveListMemberNodeTraits<Screen, &Screen::m_Link> > ScreenList;

//----------------------------------------------------------
// 注意：ScreenManager　機能は検証中および、ライブラリ内部利用のため、アプリケーションの開発には利用しないでください。
//----------------------------------------------------------

class ScreenManager
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //---------------------------------------------------------------------------
    ScreenManager();
    virtual ~ScreenManager();

    //---------------------------------------------------------------------------
    struct InitializeArgs
    {
        nn::ui2d::GraphicsResource*             pGraphicsResource;

        RegisterTextureViewSlot                 registerTextureViewSlot;
        UnregisterTextureViewSlot               unregisterTextureViewSlot;
        void*                                   pUserDataForDescriptorSlotAllocator;

        nn::AlignedAllocateFunctionWithUserData pAllocateFunction;
        nn::FreeFunctionWithUserData            pFreeFunction;
        void*                                   pUserDataForAllocateFunction;

        InitializeArgs();
    };

    //---------------------------------------------------------------------------
    struct ConstantBufferInitializeArgs
    {
        size_t size;
        bool isUseForMultithread;

        ConstantBufferInitializeArgs();
    };

    //---------------------------------------------------------------------------
    void Initialzie(nn::gfx::Device* pDevice, InitializeArgs args)
    {
        NN_UNUSED(pDevice);

        NN_SDK_ASSERT_NOT_NULL(args.pGraphicsResource);
        NN_SDK_ASSERT_NOT_NULL(args.registerTextureViewSlot);
        NN_SDK_ASSERT_NOT_NULL(args.unregisterTextureViewSlot);
        NN_SDK_ASSERT_NOT_NULL(args.pAllocateFunction);
        NN_SDK_ASSERT_NOT_NULL(args.pFreeFunction);

        m_pGraphicsResource = args.pGraphicsResource;
        m_RegisterTextureViewSlot = args.registerTextureViewSlot;
        m_UnregisterTextureViewSlot = args.unregisterTextureViewSlot;
        m_pUserDataForDescriptorSlotAllocator = args.pUserDataForDescriptorSlotAllocator;

        m_pAllocateFunction = args.pAllocateFunction;
        m_pFreeFunction = args.pFreeFunction;
        m_pUserDataForAllocateFunction = args.pUserDataForAllocateFunction;
    }

    //---------------------------------------------------------------------------
    void Finalize(nn::gfx::Device* pDevice)
    {
        NN_UNUSED(pDevice);

        m_pGraphicsResource = nullptr;
        m_RegisterTextureViewSlot = nullptr;
        m_UnregisterTextureViewSlot = nullptr;
        m_pUserDataForDescriptorSlotAllocator = nullptr;

        m_pAllocateFunction = nullptr;
        m_pFreeFunction = nullptr;
        m_pUserDataForAllocateFunction = nullptr;
    }

    //---------------------------------------------------------------------------
    nn::font::GpuBuffer* GetConstantBuffer() { return &m_Ui2dConstantBuffer; }

    void InitializeConstantBuffer(nn::gfx::Device* pDevice, const ConstantBufferInitializeArgs& initArgs);
    void FinalzeConstantBuffer(nn::gfx::Device* pDevice);

    void MapConstantBuffer()
    {
        this->GetConstantBuffer()->Map(m_ConstantBufferIndex);
    }

    void UnmapConstantBuffer()
    {
        // 呼び出すべき、タイミングについて要確認
        this->GetConstantBuffer()->Unmap();
        this->GetConstantBuffer()->SetGpuAccessBufferIndex(m_ConstantBufferIndex);
    }

    /**
    *	コンスタントバッファのインデックスです。
    */
    int GetConstantBufferIndex() const { return m_ConstantBufferIndex; }

    /**
    *	コンスタントバッファのインデックスを切り替えます。
    *	mapとunmapの間では呼ばないようにしてください。
    */
    void SwapConstantBufferIndex() { m_ConstantBufferIndex = 1 - m_ConstantBufferIndex; }

    //---------------------------------------------------------------------------
    // スクリーンの登録をします。
    void RegisterScreen(nn::ui2d::Screen* pScreen)
    {
        m_ScreenList.push_back(*pScreen);
    }

    // スクリーンの登録解除をします。
    void UnregisterScreen(nn::ui2d::Screen* pScreen)
    {
        auto ittr = m_ScreenList.begin();
        for (;ittr != m_ScreenList.end(); ittr++)
        {
            if (ittr.operator->() == pScreen)
            {
                break;
            }
        }

        if (ittr != m_ScreenList.end())
        {
            m_ScreenList.erase(ittr);
        }
    }

    //---------------------------------------------------------------------------

    nn::ui2d::GraphicsResource* GetGraphicsResource()
    {
        return m_pGraphicsResource;
    }

    const nn::ui2d::GraphicsResource* GetGraphicsResource() const
    {
        return m_pGraphicsResource;
    }

    RegisterTextureViewSlot GetRegisterTextureViewSlot() const
    {
        return m_RegisterTextureViewSlot;
    }

    UnregisterTextureViewSlot GetUnregisterTextureViewSlot() const
    {
        return m_UnregisterTextureViewSlot;
    }

    void* GetUserDataForDescriptorSlotAllocator() const
    {
        return m_pUserDataForDescriptorSlotAllocator;
    }

protected:

    //---------------------------------------------------------------------------
    nn::ui2d::GraphicsResource*             m_pGraphicsResource;

    //---------------------------------------------------------------------------
    RegisterTextureViewSlot                 m_RegisterTextureViewSlot;
    UnregisterTextureViewSlot               m_UnregisterTextureViewSlot;
    void*                                   m_pUserDataForDescriptorSlotAllocator;

    //---------------------------------------------------------------------------
    nn::AlignedAllocateFunctionWithUserData m_pAllocateFunction;
    nn::FreeFunctionWithUserData            m_pFreeFunction;
    void*                                   m_pUserDataForAllocateFunction;
    //---------------------------------------------------------------------------


    //---------------------------------------------------------------------------
    // ui2d 描画用バッファ
    nn::font::GpuBuffer                     m_Ui2dConstantBuffer;
    int                                     m_ConstantBufferIndex;

    nn::gfx::MemoryPool                     m_MemoryPoolForConstantBuffer;
    void*                                   m_MemoryPoolPtrForConstantBuffer;

    // スクリーンのリスト
    ScreenList                              m_ScreenList;
};

} // namespace ui2d
} // namespace nn
