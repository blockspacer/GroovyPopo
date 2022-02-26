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

#include <nn/ui2d/viewer/ui2d_Config.h>
#if defined(NN_UI2D_VIEWER_ENABLED)

#include <nn/util/util_FormatString.h>
#include <nn/util/util_StringUtil.h>

#include <nn/ui2d.h>
#include <nn/font.h>

#if defined(NN_BUILD_CONFIG_OS_WIN)
#include <windows.h>
#endif

#if defined(NN_BUILD_CONFIG_OS_WIN)
#include <nn/ui2d/viewer/win/ui2d_DirResourceAccessorWin.h>
#include <nn/ui2d/viewer/win/ui2d_ScreenShotWin.h>
#else
#include <nn/ui2d/viewer/ui2d_FindableArcResourceAccessor.h>
#endif

#include <cstdarg>

namespace nn
{
namespace ui2d
{
namespace viewer
{

//---------------------------------------------------------------------------
class ScalableFontManager
{
public:
    //---------------------------------------------------------------------------
    //! @brief 複合フォントです。
    //---------------------------------------------------------------------------
    struct ComplexFont
    {
        nn::font::Font* pComplexFontTree;
        char            name[FontNameMax];
    };

    //---------------------------------------------------------------------------
    ScalableFontManager()
        : m_TextureCacheWidth(0)
        , m_TextureCacheHeight(0)
        , m_WorkMemorySize(0)
        , m_NoPlotWorkMemorySize(0)
        , m_pComplexFontSet(NULL)
        , m_ComplexFontCount(0)
        , m_pTextureCache(NULL)
    {
    }

public:

    //---------------------------------------------------------------------------
    void SetTextureCacheParamaters(int textureCacheWidth, int textureCacheHeight, size_t workMemorySize, size_t noPlotWorkMemorySize)
    {
        m_TextureCacheWidth = textureCacheWidth;
        m_TextureCacheHeight = textureCacheHeight;
        m_WorkMemorySize = workMemorySize;
        m_NoPlotWorkMemorySize = noPlotWorkMemorySize;
    }

    //---------------------------------------------------------------------------
    void InitializeComplexFontSet(nn::gfx::Device* pDevice, nn::ui2d::IFindableResourceAccessor& resourceAccessor, RegisterTextureViewSlot registerTextureViewSlot, void* pUserDataForDescriptorSlotAllocator);

    //---------------------------------------------------------------------------
    void FinalizeComplexFontSet(nn::gfx::Device* pDevice, UnregisterTextureViewSlot unregisterTextureViewSlot, void* pUserDataForDescriptorSlotAllocator);

    //----------------------------------------------
    nn::font::TextureCache* GetTextureCache()
    {
        return m_pTextureCache;
    }

    //----------------------------------------------
    int GetComplexFontCount()
    {
        return m_ComplexFontCount;
    }

    //----------------------------------------------
    ComplexFont* GetComplexFontByIndex(int index)
    {
        NN_SDK_ASSERT_NOT_NULL(m_pComplexFontSet);
        NN_SDK_ASSERT(index < m_ComplexFontCount);

        return &m_pComplexFontSet[index];
    }

private:
    //----------------------------------------------
    int                         m_TextureCacheWidth;
    int                         m_TextureCacheHeight;
    size_t                      m_WorkMemorySize;
    size_t                      m_NoPlotWorkMemorySize;

    ComplexFont*                m_pComplexFontSet;
    int                         m_ComplexFontCount;

    nn::font::TextureCache*     m_pTextureCache;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
