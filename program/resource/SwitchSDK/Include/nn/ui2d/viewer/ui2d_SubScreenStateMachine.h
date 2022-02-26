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

#include <nn/ui2d/viewer/ui2d_ScalableFontManager.h>
#include <nn/ui2d/ui2d_Screen.h>

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

class Screen;
struct SystemDataScreen;

namespace viewer
{
class ViewerScreenManager;

//----------------------------------------------------------
// 複数スクリーンの切り替えのカスタムハンドラー
class SubScreenStateMachineEventHandler :
    public StateMachineEventHandler
{
public:
    NN_IMPLICIT SubScreenStateMachineEventHandler(SubScreenStateMachine* pSubScreenStateMachine, StateMachine* pOwner)
        : StateMachineEventHandler(pOwner)
        , m_pSubScreenStateMachine(pSubScreenStateMachine)
    {
    }

    virtual ~SubScreenStateMachineEventHandler()
    {
    }

    virtual void OnStateChangeCompleted(const char* pStateMachineName, StateMachineUiEventKind uiEvent) NN_OVERRIDE;

private:
    SubScreenStateMachine*  m_pSubScreenStateMachine;
};

//----------------------------------------------------------
// 複数スクリーンの切り替えを管理するステートマシンです。
class SubScreenStateMachine
{
public:

    // コンストラクタ。
    SubScreenStateMachine(nn::ui2d::ScreenManager* pScreenManager, const nn::ui2d::SystemDataScreen* pScreenSettings);

    // デストラクタ
    ~SubScreenStateMachine();

    // 初期化処理。
    void Initialzie(nn::gfx::Device* pDevice);

    // 終了処理。
    void Finalize(nn::gfx::Device* pDevice);

    // 現在の画面を取得します。
    nn::ui2d::Screen* GetCurrentSubScreen();

    // 現在のスクリーンを初期状態に
    void SetCurrentScreenToInitialState_();

    // 次のスクリーンの画面番号を取得します。
    int GetNextSubScreenIndex(int decidedButtonIndex);

    // 次のスクリーンを設定します。
    void SetNextSubScreenByIndex(int nextSubScreenIndex);

private:

    nn::ui2d::Screen**                      m_ppScreenSet;
    int                                     m_ScreenSetCount;
    int                                     m_CurrentScreenIndex;

    const nn::ui2d::SystemDataScreen*       m_pScreenSettings;
    nn::ui2d::ScreenManager*                m_pScreenManager;

    SubScreenStateMachineEventHandler*      m_pUiEventHandler;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
