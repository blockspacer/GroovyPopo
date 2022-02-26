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

#include <nn/ui2d/viewer/ui2d_AnimationManager2.h>
#include <nn/ui2d/viewer/ui2d_ScalableFontManager.h>
#include <nn/ui2d/ui2d_Screen.h>
#include <nn/ui2d/ui2d_ScreenManager.h>

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


#include <nn/ui2d/viewer/ui2d_Viewer.h>

namespace nn
{
namespace ui2d
{
namespace viewer
{

//---------------------------------------------------------------------------
//! @brief LayoutEditor と通信してプレビューするための ScreenManager クラスです。(注意：開発中のため利用しないでください)
//!
//---------------------------------------------------------------------------
class ViewerScreenManager : public nn::ui2d::ScreenManager
{
public:
    NN_UI2D_RUNTIME_TYPEINFO(nn::ui2d::ScreenManager);

    ViewerScreenManager();
    virtual ~ViewerScreenManager();

    //---------------------------------------------------------------------------
    // スケーラブルフォントのテクスチャキャッシュ初期化
    void SetTextureCacheParamaters(int textureCacheWidth, int textureCacheHeight, size_t workMemorySize, size_t noPlotWorkMemorySize)
    {
        m_ScalableFontManager.SetTextureCacheParamaters(textureCacheWidth, textureCacheHeight, workMemorySize, noPlotWorkMemorySize);
    }

    // スケーラブルフォントの初期化終了処理
    void InitializeComplexFontSet(nn::gfx::Device* pDevice, const char* pArcPath, void* pData);
    void FinalizeComplexFontSet(nn::gfx::Device* pDevice);

    // スケーラブルフォントのテクスチャキャッシュ更新
    void RegisterAndCompleteAllGlyphsForScalableFontTextureCache(nn::ui2d::Screen* pScreen);

    nn::ui2d::viewer::ScalableFontManager* GetScalableFontManager() { return &m_ScalableFontManager; }
    const nn::ui2d::viewer::ScalableFontManager* GetScalableFontManager() const { return &m_ScalableFontManager; }

private:
    viewer::ScalableFontManager             m_ScalableFontManager;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
