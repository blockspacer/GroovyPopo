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

#include <new>

#include <nn/nn_Allocator.h>

#include <nn/util/util_IntrusiveList.h>

#include <nn/ui2d/ui2d_Layout.h>


namespace nn
{
namespace ui2d
{

class Screen;
class AnimatorEx;

//---------------------------------------------------------------------------
//! @brief レイアウト全体を管理するためのクラスです。
//!
//! @details
//! 通常の Animator と以下のような違いがあります。
//!
//! - 生成するペインクラスやアニメーターを拡張されたものに変更(例えば Animator を AnimatorEx)しています。
//! - open / closeを行う機能。In / Outという決まった名前のアニメがある場合、自動的にそれを作成して、レイアウトの表示 / 非表示と関連付けることができます。また、表示 / 非表示のステートも取得できます。
//! - 部品レイアウトにLoopというアニメがあった場合、openされたときに自動的にそのアニメを再生します。(対応中)
//!
//! 注意：試験実装中のため、利用しないでください。
//!
//!
//! @sa Pane
//! @sa AnimTransform
//!
//---------------------------------------------------------------------------
class LayoutEx : public nn::ui2d::Layout
{
public:
    NN_UI2D_RUNTIME_TYPEINFO(nn::ui2d::Layout);

    enum OpenCloseState {
        OpenCloseState_Closed,
        OpenCloseState_Opening,
        OpenCloseState_Opened,
        OpenCloseState_Closing
    };

    //------------------------------------------------------------------------------

    explicit LayoutEx(nn::ui2d::Screen* pScreen);

    /**
    *  @brief コピーコンストラクタです。
    *
    *  @details
    *  ペインツリーごとコピーします。そのペインツリー内に部品レイアウトがあった場合は、その
    *  部品レイアウトもコピーします。
    *
    *  部品レイアウトをコピーした場合、そのルートペインはペインのツリー階層にぶら下がって
    *  いませんので、任意の位置にAppendChild等でぶら下げてください。
    *
    *  コピー元のレイアウトでアニメーションを作成していてもここではコピーしませんのでご注意ください。
    *  グループについてはコピーします。
    *
    *  アプリケーションで独自にペインを継承して拡張している場合は、このメソッドの内部で使用
    *  しているClonePaneTreeWithPartsLayoutメソッドを含めて、Layoutクラスの継承先でコピー
    *  コンストラクタを再実装する必要がありますので、ご注意ください。
    *
    *  @param[in] pDevice          デバイスです。
    *  @param[in] src              コピー元のレイアウト
    *  @param[in] pParentLayout    部品レイアウトをコピーする場合、親のレイアウトのポインタを渡すと、
    *                              親のレイアウトの部品ペインリストにコピーしたレイアウトのルートを
    *                              登録します。
    *  @param[in] pRootPaneName    ルートペインの名前を変更する場合に指定します。
    *                              ルートペインの名前を変更しない場合はNULLを指定してください。
    *                              特に部品レイアウトをコピーする場合は、元と同じ名前だとペインの
    *                              検索を行う場合等に問題が発生するため、名前を変更することを推奨します。
    */
    explicit LayoutEx(nn::gfx::Device* pDevice, const LayoutEx& src, LayoutEx* pParentLayout, const char* pRootPaneName);

    // 内部用機能のため使用しないでください。
    explicit LayoutEx(nn::gfx::Device* pDevice, const LayoutEx& src, LayoutEx* pParentLayout, const char* pRootPaneName, detail::BuildPaneTreeContext* pBuildPaneTreeContext);

    //------------------------------------------------------------------------------

    bool IsOpened() const
    {
        return m_OpenCloseState == OpenCloseState_Opened;
    }

    bool IsOpening() const
    {
        return m_OpenCloseState == OpenCloseState_Opening;
    }

    bool IsClosed() const
    {
        return m_OpenCloseState == OpenCloseState_Closed;
    }

    bool IsClosing() const
    {
        return m_OpenCloseState == OpenCloseState_Closing;
    }

    //------------------------------------------------------------------------------

    void Open();

    //------------------------------------------------------------------------------

    void Close();

    /**
    *	指定したタグ名のアニメーションリソースデータがあるか否かを返します。
    *
    *	@param[in] pTagName  アニメーションのタグ名
    *	@return アニメーションリソースデータがある場合はtrue
    */
    bool IsExistAnimation(const char* pTagName) { return (GetAnimResourceData(pTagName) != nullptr); };

    //------------------------------------------------------------------------------
    // スクリーンで一括で更新するように変更になる予定。
    virtual void UpdateDefaultAnimators()
    {
        UpdateOpenCloseState_(m_pInAnimator);
        UpdateOpenCloseState_(m_pOutAnimator);
        UpdateOpenCloseState_(m_pLoopAnimator);
    }

    //------------------------------------------------------------------------------
    const nn::ui2d::Screen* GetScreen() const
    {
        return m_pScreen;
    }

    nn::ui2d::Screen* GetScreen()
    {
        return m_pScreen;
    }


    //------------------------------------------------------------------------------

    virtual void AnimatorDisableCallback(Animator* animator);

    //------------------------------------------------------------------------------


    //------------------------------------------------------------------------------

    /**
    *	バインドする対象を自動で判定してアニメーションを作成します。
    *
    *	特別な事情がない限りは、eui::Animatorはこのメソッドで作成してください。
    *
    *	内部的には、アニメタグに関連付けられているグループに含まれるペインを全て
    *	バインドします。
    *
    *	@param pDevice	デバイス
    *	@param pTagName	アニメタグの名前
    *	@param isEnabled	アニメを作成した時点で有効にするか否か。
    *					trueを指定した場合は、Stop(0.f)を呼び出したのと同じ挙動になります。
    *					つまり、最初のフレームで0フレーム目が再生され、すぐに無効になります。
    *					falseを指定した場合は、無効な状態で作成されます。
    *	@return 作成されたアニメーション
    */
    Animator* CreateAnimatorExAuto(nn::gfx::Device* pDevice, const char* pTagName, bool isEnabled);

    /**
    *	バインドするグループを指定してアニメーションを作成します。
    *
    *	バインドするグループを明示的に指定したいときに使用します。
    *
    *	@param pDevice	デバイス
    *	@param pTagName	アニメタグの名前
    *	@param pGroup	バインドするグループ
    *	@param isEnabled	アニメを作成した時点で有効にするか否か。
    *	@return 作成されたアニメーション
    */
    AnimatorEx* CreateAnimatorExWithGroup(nn::gfx::Device* pDevice, const char* pTagName, nn::ui2d::Group* pGroup, bool isEnabled);

    /**
    *	バインドするグループを指定してアニメーションを作成します。
    *
    *	バインドするグループを明示的に指定したいときに使用します。
    *
    *	@param pDevice	デバイス
    *	@param pTagName	アニメタグの名前
    *	@param groupName バインドするグループ名
    *	@param isEnabled	アニメを作成した時点で有効にするか否か。
    *	@return 作成されたアニメーション
    */
    AnimatorEx* CreateAnimatorExWithGroup(nn::gfx::Device* pDevice, const char* pTagName, const char* pGroupName, bool isEnabled)
    {
        return CreateAnimatorExWithGroup(pDevice, pTagName, GetGroupContainer()->FindGroupByName(pGroupName), isEnabled);
    }

    /**
    *	バインドするグループをインデックス指定してアニメーションを作成します。
    *
    *	アニメタグにグループが複数関連付けられているときに、そのグループのうち何番目をバインドするか
    *	を指定したいときに使用します。
    *
    *	@param pDevice	デバイス
    *	@param pTagName	アニメタグの名前
    *	@param groupIndex バインドするグループのインデックス
    *	@param isEnabled	アニメを作成した時点で有効にするか否か。
    *	@return 作成されたアニメーション
    */
    AnimatorEx* CreateAnimatorExWithGroupIndex(nn::gfx::Device* pDevice, const char* pTagName, uint32_t groupIndex, bool isEnabled);

    /**
    *	バインドするグループをインデックス指定してアニメーションを作成します。
    *
    *	createAnimatorWithGroupIndexとの違いは、こちらは指定したタグにグループが含まれていない
    *	場合はアサートせずにnullptrを返すことです。
    */
    AnimatorEx* TryCreateAnimatorExWithGroupIndex(nn::gfx::Device* pDevice, const char* pTagName, uint32_t groupIndex, bool isEnabled);

    AnimatorEx* TryCreateAnimatorExAuto(nn::gfx::Device* pDevice, const char* pTagName, bool isEnabled);

    //------------------------------------------------------------------------------

protected:

    //------------------------------------------------------------------------------
    virtual bool BuildImpl(
        nn::ui2d::BuildResultInformation* pOutBuildResultInfomation,
        nn::gfx::Device* pDevice,
        const void* lytResBuf,
        nn::ui2d::ResourceAccessor* pResAcsr,
        const nn::ui2d::BuildArgSet& parentBuildArgSet,
        const PartsBuildDataSet* partsBuildDataSet) NN_OVERRIDE;

    //------------------------------------------------------------------------------
    virtual Layout* DoCreatePartsLayout_(const char* pName, const PartsBuildDataSet& partsBuildDataSet, const nn::ui2d::BuildArgSet& buildArgSet) NN_OVERRIDE;


    //------------------------------------------------------------------------------
    void DisableLoopAnimator_(bool isRecursive);

    //------------------------------------------------------------------------------
    virtual void DoBuildDefaultAnimatons_(nn::gfx::Device* pDevice);

private:

    void UpdateOpenCloseState_(nn::ui2d::Animator* pAnimator);

    void StartAnimationOpen_();
    void StartAnimationClose_();
    //------------------------------------------------------------------------------
    nn::ui2d::Screen*       m_pScreen;

    nn::ui2d::Animator*     m_pInAnimator;
    nn::ui2d::Animator*     m_pOutAnimator;
    nn::ui2d::Animator*     m_pLoopAnimator;

    OpenCloseState          m_OpenCloseState;
};

} // namespace nn::ui2d
} // namespace nn
