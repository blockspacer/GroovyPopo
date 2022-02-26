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

#include <nn/ui2d/ui2d_AnimatorEx.h>
#include <nn/ui2d/ui2d_LayoutEx.h>
#include <nn/ui2d/ui2d_StateMachine.h>

#include <cstdarg>

namespace nn
{
namespace ui2d
{

class ScreenManager;

//----------------------------------------------------------
// 注意：ScreenBase　Screen　機能は検証中および、ライブラリ内部利用のため、アプリケーションの開発には利用しないでください。
//----------------------------------------------------------

//---------------------------------------------------------------------------
// スクリーンが満たすべき、基本的な特性を決める基底クラスです。
class ScreenBase
{
public:
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //---------------------------------------------------------------------------
    //! @brief 入力デバイスの状態です。
    //---------------------------------------------------------------------------
    struct InputDeviceState
    {
        bool isMousePointerOn;
        float pointerX;
        float pointerY;
        bool isTrigMouseLButton;
        bool isReleaseMouseLButton;
        bool isHoldLeft;
        bool isHoldRight;
        bool isTrigUp;
        bool isTrigDown;
        bool isTrigA;
        bool isTrigB;
        bool isTrigX;
        bool isTrigY;
        bool isTrigZ;
        bool isSelect;
        bool isTrigR;
        bool isTrigL;
        bool isHoldR;
        bool isHoldZR;
    };

    virtual void UpdateInputs(nn::gfx::Device* pDevice, const nn::ui2d::ScreenBase::InputDeviceState& inputDeviceState) = 0;
    virtual void UpdateLayout(nn::gfx::Device* pDevice) = 0;
    virtual void DrawCaptureTexture(nn::gfx::Device* pDevice, nn::gfx::CommandBuffer& commandBuffer) = 0;
    virtual void DrawLayout(nn::gfx::CommandBuffer& commandBuffer) = 0;
    virtual nn::ui2d::ResourceAccessor* GetResourceAccessor() = 0;

protected:

    virtual nn::ui2d::ResourceAccessor* CreateResourceAccessor_() = 0;
    virtual nn::ui2d::DrawInfo* CreateDrawInfo_() = 0;
};

typedef nn::util::IntrusiveList<nn::ui2d::AnimatorEx, nn::util::IntrusiveListMemberNodeTraits<nn::ui2d::AnimatorEx, &nn::ui2d::AnimatorEx::m_ActivenessLink> > ActiveAnimatorSet;

//---------------------------------------------------------------------------
//! @brief ボディレイアウトを表すクラスです。
//!
//! @details
//! 通常の Animator と以下のような違いがあります。
//!
//! - 1つのスクリーンは、1つのボディレイアウトに対応します。1つのスクリーンで複数の本体レイアウトを表示することはできません。
//! - スクリーンは、プログラムで定義された列挙子によって判別します。
//! - スクリーンのdrawの順番およびupdateの順番は、列挙子の列挙順で決まります。drawの順番は列挙子の昇順になり、updateの順番は列挙子の降順になります。
//! - スクリーンの生成はファクトリを通して行います。
//! - 表示開始・表示終了(open / close)の制御を行えます。
//! - ユーザーは、基本的に Screen クラスを継承して仮想関数をオーバーライドし、アプリケーション固有の実装します
//!
//! 注意：試験実装中のため、利用しないでください。
//!
class Screen : public ScreenBase
{
public:
    NN_UI2D_RUNTIME_TYPEINFO(ScreenBase);

    //---------------------------------------------------------------------------
    Screen();

    virtual ~Screen()
    {
    };

    //---------------------------------------------------------------------------
    void Initialize(nn::gfx::Device* pDevice, ScreenManager* pScreenManager);
    void Finalize(nn::gfx::Device* pDevice);

    //---------------------------------------------------------------------------
    void Build(nn::gfx::Device* pDevice, const void* pLayoutResource);
    void ResetBuiltLayout(nn::gfx::Device* pDevice);

    //---------------------------------------------------------------------------
    nn::ui2d::Size GetViewSize() const
    {
        if (m_Layout != NULL)
        {
            return m_Layout->GetLayoutSize();
        }
        else
        {
            return nn::ui2d::Size::Create(1920.f, 1080.f);
        }
    }

    //---------------------------------------------------------------------------
    void SetLayout(nn::ui2d::Layout* pLayout)
    {
        NN_SDK_ASSERT(m_Layout == nullptr);
        NN_SDK_ASSERT(pLayout != nullptr);

        m_Layout = pLayout;
    }

    //------------------------------------------------------------------------------
    nn::ui2d::Layout* GetLayout()
    {
        return m_Layout;
    }

    const nn::ui2d::Layout* GetLayout() const
    {
        return m_Layout;
    }

    //------------------------------------------------------------------------------

    nn::ui2d::LayoutEx* GetLayoutEx()
    {
        return nn::ui2d::DynamicCast<nn::ui2d::LayoutEx*>(GetLayout());
    }

    const nn::ui2d::LayoutEx* GetLayoutEx() const
    {
        return nn::ui2d::DynamicCast<const nn::ui2d::LayoutEx*>(GetLayout());
    }

    //----------------------------------------------
    nn::ui2d::DrawInfo* GetDrawInfo()
    {
        return m_pDrawInfo;
    }

    const nn::ui2d::DrawInfo* GetDrawInfo() const
    {
        return m_pDrawInfo;
    }

    //----------------------------------------------
    nn::ui2d::ScreenManager* GetScreenManager()
    {
        return m_pScreenManager;
    }

    const nn::ui2d::ScreenManager* GetScreenManager() const
    {
        return m_pScreenManager;
    }

    //----------------------------------------------
    void SetIsPerspectiveProjection(bool isPerspectiveProjection)
    {
        m_IsPerspectiveProjection = isPerspectiveProjection;
    }

    void SetPerspectiveFovyDeg(float perspectiveFovyDeg)
    {
        m_PerspectiveFovyDeg = perspectiveFovyDeg;
    }

    //----------------------------------------------
    void SetPerspectiveNear(float perspectiveNear)
    {
        m_PerspectiveNear = perspectiveNear;
    }

    //----------------------------------------------
    void SetPerspectiveFar(float perspectiveFar)
    {
        m_PerspectiveFar = perspectiveFar;
    }

    //----------------------------------------------
    virtual nn::ui2d::ControlCreator* GetControlCreator() const
    {
        return m_pControlCreator;
    }

    //----------------------------------------------
    Screen& SetControlCreator(nn::ui2d::ControlCreator* pControlCreator)
    {
        m_pControlCreator = pControlCreator;
        return *this;
    }

    //----------------------------------------------
    TextSearcher* GetTextSearcher()
    {
        return nullptr;
    }

    //-------------------------------------------------------------------------
    nn::ui2d::ButtonGroup& GetButtonGroup()
    {
        return m_ButtonGroup;
    }

    nn::ui2d::ControlList& GetControlList()
    {
        return m_ControlList;
    }

    const nn::ui2d::ButtonGroup& GetButtonGroup() const
    {
        return m_ButtonGroup;
    }

    const nn::ui2d::ControlList& GetControlList() const
    {
        return m_ControlList;
    }


    //---------------------------------------------------------------------------
    bool GetVisible() const
    {
        return m_IsVisible;
    }

    void SetVisible(bool isVisible)
    {
        m_IsVisible = isVisible;
    }

    //------------------------------------------------------------------------------

    bool IsOpened() const
    {
        return GetLayoutEx()->IsOpened();
    }

    bool IsOpening() const
    {
        return GetLayoutEx()->IsOpening();
    }

    bool IsClosed() const
    {
        return GetLayoutEx()->IsClosed();
    }

    bool IsClosing() const
    {
        return GetLayoutEx()->IsClosing();
    }

    //------------------------------------------------------------------------------

    void Open()
    {
        auto* pLayout = GetLayoutEx();
        if (pLayout != nullptr)
        {
            pLayout->Open();
        }
    }

    //------------------------------------------------------------------------------

    void Close()
    {
        auto* pLayout = GetLayoutEx();
        if (pLayout != nullptr)
        {
            pLayout->Close();
        }
    }

    //---------------------------------------------------------------------------
    float GetAnimationStep() const
    {
        return 1.0f;
    }

    //---------------------------------------------------------------------------
    void SetAnimatorActive(AnimatorEx* pAnimator);
    void EraseAnimatorFromActiveList(AnimatorEx* pAnimator);

    //---------------------------------------------------------------------------

    virtual void UpdateInputs(nn::gfx::Device* pDevice, const nn::ui2d::ScreenBase::InputDeviceState& inputDeviceState) NN_OVERRIDE;

    virtual void UpdateLayout(nn::gfx::Device* pDevice) NN_OVERRIDE;

    virtual void UpdateUserInput(const nn::util::Float2* pPos, bool isDown, bool isRelease /* = false */);

    virtual void HandleEventOnButtonStateChanged(nn::ui2d::AnimButton* pButton, nn::ui2d::AnimButton::State prevState, nn::ui2d::AnimButton::State nextState)
    {
        NN_UNUSED(pButton);
        NN_UNUSED(prevState);
        NN_UNUSED(nextState);
    }

    //---------------------------------------------------------------------------


    //---------------------------------------------------------------------------

    virtual void DrawCaptureTexture(nn::gfx::Device* pDevice, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;
    virtual void DrawLayout(nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    virtual nn::ui2d::ResourceAccessor* GetResourceAccessor() NN_OVERRIDE
    {
        return m_pResourceAccessor;
    }

    virtual nn::ui2d::ResourceAccessor* CreateResourceAccessor_() NN_OVERRIDE
    {
        return nn::ui2d::Layout::AllocateAndConstruct<nn::ui2d::ArcResourceAccessor>();
    }

    virtual nn::ui2d::DrawInfo* CreateDrawInfo_() NN_OVERRIDE;

    virtual nn::ui2d::ControlCreator* CreateControlCreator_()
    {
        return nn::ui2d::Layout::AllocateAndConstruct<nn::ui2d::DefaultControlCreator>(&m_ButtonGroup);
    }

    //--------------------------------------------------------------------------------------------------
    void SetViewSize(const nn::ui2d::Size& targetViewSize, const nn::ui2d::Size& mainViewportSize);


    //-------------------------------------------------------------------------
    //! @brief  プロジェクション行列を取得します。
    //!
    //! @return プロジェクション行列を取得します。
    //-------------------------------------------------------------------------
    nn::util::MatrixT4x4fType GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

    //-------------------------------------------------------------------------
    //! @brief  ビュー行列を取得します。
    //!
    //! @return ビュー行列を取得します。
    //-------------------------------------------------------------------------
    nn::util::MatrixT4x3fType GetViewMatrix() const
    {
        return m_CameraViewMatrix;
    }

    //-------------------------------------------------------------------------
    //! @brief  Utf8 を利用するかどうかを取得します。
    bool GetIsUseUtf8() const { return m_IsUseUtf8; }
    void SetIsUseUtf8(bool flag) { m_IsUseUtf8 = flag; }

    //-------------------------------------------------------------------------
    //! @brief  レイアウトビルドオプションを取得します。
    nn::ui2d::Layout::BuildOption*   GetBuildOption() { return &m_BuildOption;}

    //-------------------------------------------------------------------------
    //! @brief ビルド結果を取得します。
    const BuildResultInformation& GetBuildResultInformation() const { return m_BuildResultInformation; }

    //-------------------------------------------------------------------------
    //! @brief  アニメーション再生状況を取得します。
    //!
    //! @return アニメーション再生状況（一度再生されたかどうか）です。
    //-------------------------------------------------------------------------
    bool GetAnimateOnce() const { return m_AnimateOnce; }

    //-------------------------------------------------------------------------
    //! @brief  アニメーション再生状況を取得します。
    //!
    //! @return ビュー行列を取得します。
    //-------------------------------------------------------------------------
    void SetAnimateOnce(bool flag)  { m_AnimateOnce = flag; }

    //-------------------------------------------------------------------------
    int CopyLayoutBelongingControls(nn::gfx::Device* pDevice, const nn::ui2d::Layout* pSrcLayout, nn::ui2d::Layout* pDstLayout);

public:
    // レイアウトクラスが管理するためのリンク情報です。
    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

protected:

    virtual nn::ui2d::Layout* DoBuild_(nn::gfx::Device* pDevice, const void* pLayoutResource);
    virtual void DoBuildAnimatons_(nn::gfx::Device* pDevice, const void* pLayoutResource);
    virtual void DoInitialize_(nn::gfx::Device* pDevice);
    virtual nn::ui2d::Layout* DoAllocateLayout_(nn::gfx::Device* pDevice, const void* pLayoutResource);

    virtual void SetupPaneAfterBuild_(nn::gfx::Device* pDevice, nn::ui2d::Pane* pPane, nn::ui2d::Layout* pLayout);
    virtual void SetupPaneAfterBuildRecursively_(nn::gfx::Device* pDevice, nn::ui2d::Pane* pPane, nn::ui2d::Layout* pLayout);

    virtual void UpdateControl_(nn::gfx::Device* pDevice);

    virtual void UpdateAnimator_();

    void EraseAnimatorFromActiveListInternal_(AnimatorEx* pAnimator);
    int CountActiveAnimator_() const;


    //---------------------------------------------------------------------------
    ScreenManager*          m_pScreenManager;

    nn::ui2d::DrawInfo*     m_pDrawInfo;
    nn::ui2d::Layout*       m_Layout;

    nn::ui2d::ResourceAccessor* m_pResourceAccessor;

    //----------------------------------------------
    // 拡張ポイント
    // 従来は、コールバックベースでしたが、オーバーライドベースで提供します。
    virtual void OnPostCalculate(nn::ui2d::Layout* pLayout) { NN_UNUSED(pLayout); };

    //----------------------------------------------
    // アニメーション、ボタン関係

    ActiveAnimatorSet               m_ActiveAnimators;
    nn::ui2d::ButtonGroup           m_ButtonGroup;
    nn::ui2d::ControlList           m_ControlList;

    //----------------------------------------------
    nn::util::MatrixT4x4fType   m_ProjectionMatrix;
    nn::util::MatrixT4x3fType   m_CameraViewMatrix;

    bool                        m_IsPerspectiveProjection;
    float                       m_PerspectiveFovyDeg;
    float                       m_PerspectiveNear;
    float                       m_PerspectiveFar;

    bool                        m_IsViewMtxDirty;
    bool                        m_IsVisible;

    //----------------------------------------------
    // ビルド時に利用するメンバー
    nn::ui2d::ControlCreator*       m_pControlCreator;
    nn::ui2d::TextSearcher*         m_pTextSearcher;
    BuildResultInformation          m_BuildResultInformation;
    nn::ui2d::Layout::BuildOption   m_BuildOption;
    bool                            m_IsUseUtf8;

    //----------------------------------------------
    bool                            m_AnimateOnce; // 一回はアニメーションを行ったか
};


//---------------------------------------------------------------------------
class ControlCopier
{
public:
    ControlCopier();
    virtual ~ControlCopier();

    struct CopyLayoutBelongingControlArg
    {
        nn::ui2d::ButtonGroup* pButtonGroup;
        nn::ui2d::ControlList* pControlList;
        nn::gfx::Device* pDevice;

        CopyLayoutBelongingControlArg();
    };

    int CopyLayoutBelongingControlsRecursive(const CopyLayoutBelongingControlArg& arg, const nn::ui2d::Layout* pSrcLayout, nn::ui2d::Layout* pDstLayout);
    int CopyLayoutBelongingControls(const CopyLayoutBelongingControlArg& arg, const nn::ui2d::Layout* pSrcLayout, nn::ui2d::Layout* pDstLayout);

    virtual ControlBase* CopySingleControl(nn::gfx::Device* pDevice, const ControlBase& control, nn::ui2d::Layout* pDstLayout);
    virtual AnimButton* CopySingleButton(nn::gfx::Device* pDevice, const AnimButton& buttonBase, nn::ui2d::Layout* pDstLayout);

};


} // namespace ui2d
} // namespace nn
