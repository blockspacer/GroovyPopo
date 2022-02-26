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

#include <nn/gfx.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/util/util_MathTypes.h>

#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_Util.h>

#include <nn/ui2d/ui2d_Screen.h>


namespace nn
{

namespace ui2d
{

class Animator;
class ControlSrc;
class Layout;
class Pane;
class Parts;

//------------------------------------------------------------------------------
class SlideListPage;
class SlideListElement;
class DragScrollButton;
class Screen;

//------------------------------------------------------------------------------

//!
//!    @brief スライドリストコントロールです。
//!
//!   - SlideListOnを指定したペインに関連付けられます。名前もそのペイン名で付けられます。
//!   - SlideListPageの配列と、SlideListElementの配列を管理します。
//!
class SlideListControl : public ControlBase
{
public:

    //! @brief 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(ControlBase);

    //!
    //!  @brief コンストラクタです。
    //!
    //!   @param pBasePane    基準のペイン
    //!   @param pLayout        基準のペインが所属するレイアウト
    //!
    SlideListControl(nn::ui2d::Pane* pBasePane, nn::ui2d::Layout* pLayout, nn::ui2d::Screen* pScreen);

    //!
    //!   @brief コピーコンストラクタです。
    //!
    //!   @param src        コピー元のコントロール
    //!   @param pLayout    コピー先のコントロールが所属するレイアウト
    //!
    SlideListControl(const SlideListControl& src, nn::ui2d::Layout* pLayout, nn::ui2d::Screen* pScreen);

    //!
    //!   @brief デストラクタです。
    //!
    virtual ~SlideListControl();

    /// 初期化引数構造体です。
    struct InitializeArg {
        int element_num;
        nn::gfx::Device* pDevice;

        InitializeArg();
    };

    //!
    //!   @brief 初期化処理を行います。
    //!
    //!   @param initArg        初期化変数です。
    //!
    template <typename SlideListPageClass, typename SlideListElementClass> void Initialize(const InitializeArg& initArg);

    //!   @brief 初期化処理を行います。
    //!
    //!   @param pDevice        デバイスです。
    //!   @param pLayout        レイアウトです。
    virtual void Initialize(nn::gfx::Device* pDevice, nn::ui2d::Layout* pLayout) NN_OVERRIDE
    {
        NN_UNUSED(pDevice);
        NN_UNUSED(pLayout);
    }

    //!   @brief 終了処理を行います。
    //!
    //!   @param pDevice        デバイスです。
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE
    {
        NN_UNUSED(pDevice);
    }

    //!   @brief ユーザー入力に関連する更新処理を行います。
    //!
    //!   @param pPos        位置です。
    //!   @param isDown      押下状態か。
    //!   @param isRelease   離した状態か。
    virtual void UpdateControlUserInput(const nn::util::Float2* pPos, bool isDown, bool isRelease = false) NN_OVERRIDE
    {
        NN_UNUSED(pPos);
        NN_UNUSED(isDown);
        NN_UNUSED(isRelease);

    }

    //!
    //!   @brief 更新処理です。
    //!
    //!   Screenから自動的に呼び出されます。
    //!
    //!   @param[in] animationStep 可変フレーム対応時に、アニメのスピードを調整するステップ
    //!
    virtual void UpdateControl(float animationStep = 1.f) NN_OVERRIDE;

    //!
    //!    @brief ページをいったん全て無効化します。
    //!
    //!    現在表示されているページ上の要素は、この関数の中で、全ていったんleaveが呼び出されます。
    //!
    //!    次にSlideListControlのUpdateが呼び出されたときに、表示されるページに応じてenterが呼び
    //!    出されます。表示したいページに応じて、setMoveDirectPage()を呼び出してください。
    //!
    //!    このメソッドは、全ての要素についてenterを呼び直したい場合に使用します。
    //!    例えば、スライドリストの上にタブがあって、スライドリストは１つだけある、という場合に、
    //!    タブが選択されたときにスライドリストの内容を全て置き換える、というような場合です。
    //!
    void InvalidateAllPages();

    //!
    //!    @brief コントロールの基準となるペインを返します。
    //!
    const nn::ui2d::Pane* GetBasePane() const { return m_pBasePane; }

    //!
    //!    @brief 初期状態のスクロールの目標ページを返します。
    //!
    //!    この値は、0番のページがちょうどスクロールのマイナス方向の端に来るように設定されます。
    //!
    int GetInitialScrollTargetPage() const { return m_InitialScrollTargetPage; }

    //!
    //!    @brief 要素の最大数とは別に設定できる、要素の上限を設定します。
    //!
    //!    デフォルトでは要素の最大数と等しい値になっています。
    //!    この値を設定すると、初期化時には最大で使用する要素の数を指定しておいて、
    //!    状況に応じて実際に使用する要素数をそれより小さい値に設定するということができます。
    //!
    //!    getElementNumMax()より大きい値を指定することはできません。
    //!
    void SetElementNumLimit(int limit);

    //!
    //!    @brief 要素の最大数とは別に設定できる、要素の上限を返します。
    //!
    int GetElementNumLimit() const { return m_ElementNumLimit; }

    //!
    //!    @brief 要素の最大数を返します。
    //!
    //!    初期化時に指定したInitializeArg::element_numと同じ値になります。
    //!
    int GetElementNumMax() const { return m_CountElements; }

    //!
    //!    @brief ページのスクロールの目標となるページ番号を返します。
    //!
    int GetScrollTargetPage() const { return m_ScrollTargetPage; }

    //!
    //!    @brief ページのスクロールの目標となるページ番号を設定します。
    //!
    void SetScrollTargetPage(int page) { m_ScrollTargetPage = page; }

    //!
    //!    @brief スクロールせずに、指定したページに直接移動します。
    //!
    void SetMoveDirectPage(int page) { m_MoveDirectPage = page; m_ScrollTargetPage = page; }

    //!
    //!    @brief ページのスクロールが縦かどうかを返します。
    //!
    bool IsScrollVertical() const { return m_IsScrollVertical; }

    //!
    //!    @brief ページ内の要素の配置が縦かどうかを返します。
    //!
    bool IsElementVertical() const { return m_IsElementVertical; }

    //!
    //!    @brief ページ内の要素の配置のx軸方向の数を返します。
    //!
    int GetElementNumInPageX() const { return m_ElementNumInPageX; }

    //!
    //!    @brief ページ内の要素の配置のy軸方向の数を返します。
    //!
    int GetElementNumInPageY() const { return m_ElementNumInPageY; }

    //!
    //!    @brief ページ内の要素の数を返します。
    //!
    int GetElementNumInPage() const { return m_ElementNumInPageX * m_ElementNumInPageY; }

    //!
    //!    @brief ページ番号の最大数を返します。
    //!
    //!    この値は、setElementNumLimit()で要素の上限数を変更すると変動しますので、ご注意ください。
    //!
    int GetPageIndexMax() const { return m_PageIndexMax; }

    //!
    //!    @brief 現在のスクロール位置を返します。
    //!
    float GetCurrentScrollPos() const { return m_CurrentScrollPos; }

    //!
    //!    @brief 要素の配列を取得します。
    //!
    SlideListPage** const GetPages() { return m_ppPages; };

    int GetCountPages() const { return m_CountPages; };

    //!
    //!    @brief 要素の配列を取得します。
    //!
    SlideListElement** const GetElements() { return m_ppElements; };

    int GetCountElements() const { return m_CountElements; };

    //!
    //!    @brief スライドリストの表示される領域のスクロール方向の幅を取得します。
    //!
    float GetBaseVisibleSpan() const { return m_BaseVisibleSpan; }

    //!
    //!    @brief 囲みカーソルノードがアクティブになったページを設定します。
    //!
    void SetChangeActiveBoxCursorNodePage(SlideListPage* page) { m_ChangeActiveBoxCursorNodePage = page; }

    //!
    //!    @brief 名前を取得します。
    //!
    const char* GetName() const { return m_pName; }

    //!
    //!    @brief GetElementButtonInfo()で取得する情報を格納します。
    //!
    struct ElementButtonInfo {
        int index;                          ///< 要素の通し番号
        const SlideListControl* pControl;    ///< スライドリストコントロール
        SlideListPage* pPage;                ///< 所属するページ
        SlideListElement* pElement;          ///< 対応付けられた要素
        nn::ui2d::Parts* pElementPane;      ///< 対応付けられた要素ペイン
    };

    //!
    //!    @brief 引数 pButton で指定したボタンがスライドリストに所属するボタンだった場合、その情報を
    //!    取得します。結果はpButtonElementInfoで指定した構造体に設定されます。
    //!
    //!    @param pScreen               コントロールを持っているスクリーン
    //!    @param pButtonElementInfo    結果を取得する構造体
    //!    @param pButton    情報を取得するボタン
    //!    @param 情報を取得できたか否か
    //!
    static bool GetElementButtonInfo(nn::ui2d::Screen* pScreen, ElementButtonInfo* pButtonElementInfo, const AnimButton* pButton);

protected:
    nn::ui2d::Pane* m_pBasePane;
    bool m_IsValid;
    bool m_IsScrollVertical;
    bool m_IsElementVertical;
    bool m_IsElementButton;
    int m_ElementNumInPageX;
    int m_ElementNumInPageY;
    float m_BaseVisibleSpan;
    float m_PageScrollSpan;
    int m_PageIndexMax;
    float m_ScrollFraction;
    float m_ScrollConstantSpeed;
    int m_InitialScrollTargetPage;
    SlideListPage** m_ppPages;
    int m_CountPages;
    SlideListElement** m_ppElements;
    int m_CountElements;
    DragScrollButton* m_DragScrollButton;

    int m_ElementNumLimit;
    int m_ScrollTargetPage;
    float m_CurrentScrollPos;
    int m_MoveDirectPage;
    int m_LastButtonScrollCausePage;
    float m_ScrollPosOfDragScrollStart;
    bool m_IsDragScrolling;
    bool m_IsForceUpdatePage;
    SlideListPage* m_ChangeActiveBoxCursorNodePage;

    nn::ui2d::Layout*                   m_pLayout;
    const char*                         m_pName;
    nn::ui2d::Screen*                   m_pScreen;

    enum
    {
        SequenceNumberDigit = 2
    };

    //!
    //! @brief 部品ペインの部品レイアウトに対応する、ボタンを検索します。
    //!
    AnimButton* FindButtonByParts_(nn::ui2d::Layout* pPartsLayout);

    //!
    //!    @brief ページが幾つ必要か求める
    //!
    int CalculatePageNum_(nn::ui2d::Pane* pPagePaneFirst) const;

    //!
    //!    @brief PageとElementが終わった後の初期化処理
    //!
    nn::ui2d::Pane* FindPagePaneFirst_() const;

    //!
    //!    @brief 可視状態の先頭のページを求める
    //!
    int GetVisibleStartPage_(float scrollPos);

    //!
    //!    @brief ページ番号の最大数を求める
    //!
    int CalculatePageIndexMax_() const;

    //!
    //!    @brief PageとElementの生成が終わった後の初期化処理
    //!
    virtual void DoInitialize_(nn::gfx::Device* pDevice, nn::ui2d::Pane* pPagePaneFirst);

    //!
    //!    @brief スクロールの初期化処理
    //!
    virtual void DoInitializeScroll_(nn::gfx::Device* pDevice);

    //!
    //!    @brief 初期化処理の中で、ページ内の要素の並びを調べ、ページの初期化を行う
    //!
    virtual void DoInitializePage_(nn::gfx::Device* pDevice, nn::ui2d::Pane* pPagePaneFirst);

    //!
    //!    @brief 初期化処理の中で、要素の初期化を行う
    //!
    virtual void DoInitializeElement_(nn::gfx::Device* pDevice);

    //!
    //!    @brief コントロールをコピーする(srcが部品ペインの時のみ処理する)
    //!
    virtual void CopyControls_(nn::gfx::Device* pDevice, nn::ui2d::Pane* pSrcPane, nn::ui2d::Pane* pDstPane);

    //!
    //!    @brief ページに割り当てられたページ番号を更新し、スクロール処理を行います。
    //!
    virtual bool UpdateScroll_(float animationStep);

    //!
    //!    @brief ドラッグスクロール処理を行います。
    //!
    virtual void UpdateDragScroll_(float animationStep);

    //!
    //!    @brief ページに割り当てられたページ番号を更新し、位置を更新します。
    //!
    virtual void UpdatePage_();

};

//------------------------------------------------------------------------------

//!
//!    @brief スライドリストページです。
//!
//!    - 要素を載せて移動するものを表します。
//!    - 移動は一次元で、縦か横です。
//!    - 移動方向には複数存在しますが、移動と垂直の方向には一つしか存在しません。
//!      (要素が垂直の方向に複数存在することはあります。)
//!    - 実際に使われるペインを管理します。
//!      必要に応じて、要素にペインを割り当てます。
//!    - 要素のペインが部品ペインになっていて、部品ごとにメモリが必要な初期化処理を行う場合は、
//!    initialize()を継承してカスタマイズすることで対応してください。
//!
class SlideListPage
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();
public:
    //!
    //!    @brief コンストラクタです。
    //!
    SlideListPage();

    //!
    //!    @brief デストラクタです。
    //!
    virtual ~SlideListPage();

    /// 初期化引数構造体です。
    struct InitializeArg {
        SlideListControl* pControl;
        nn::ui2d::Pane* pPagePane;
        nn::ui2d::Pane** ppElementPanes;
        nn::ui2d::AnimButton** ppElementButtons;

        InitializeArg();
    };

    //!
    //!    @brief 初期化を行います。
    //!
    virtual void Initialize(const InitializeArg& initArg);

    //!
    //!    @brief 現在のページ番号を指定します。
    //!
    virtual void SetCurrentPageIndex(int index);

    //!
    //!    @brief スクロール処理を行います。
    //!
    virtual void Scroll(bool is_virtical, float base_scroll_pos);

    //!
    //!    @brief このページが有効か(ページ番号が割り当てられているか)を返します。
    //!
    bool IsValid() const { return (m_CurrentPageIndex != INT_MAX); }

    //!
    //!    @brief 割り当てられているページ番号を返します。
    //!
    //!    ページが割り当てられていない場合は不定な値となりますので注意してください。
    //!
    int GetCurrentPageIndex() const { return m_CurrentPageIndex; }

    //!
    //!    @brief このページの要素が何番から始まるかを返します。
    //!    isValid()がfalseのときは不定となります。
    //!
    int GetCurrentElementStartIndex() const { return GetElementNumInPageX() * GetElementNumInPageY() * m_CurrentPageIndex; }

    //!
    //!    @brief ページ内の要素の配置が縦かどうかを返します。
    //!
    bool IsElementVertical() const { return m_pSlideListControl->IsElementVertical(); }

    //!
    //!    @brief ページ内の要素の配置のx軸方向の数を返します。
    //!
    int GetElementNumInPageX() const { return m_pSlideListControl->GetElementNumInPageX(); }

    //!
    //!    @brief ページ内の要素の配置のy軸方向の数を返します。
    //!
    int GetElementNumInPageY() const { return m_pSlideListControl->GetElementNumInPageY(); }

    //!
    //!    @brief ページ内の要素の数を返します。
    //!
    int GetElementNumInPage() const { return m_pSlideListControl->GetElementNumInPage(); }

    //!
    //!    @brief ページ番号の最大値を返します。
    //!
    int GetPageIndexMax() const { return m_pSlideListControl->GetPageIndexMax(); }

    //!
    //!    @brief 要素の配列を取得します。
    //!
    nn::ui2d::Pane** const GetElementPanes() { return m_ppElementPanes; };
    // リストの取得に変える

    //!
    //!    @brief 要素のボタンの配列を取得します。
    //!
    nn::ui2d::AnimButton** const GetElementButtons() { return m_ppElementButtons; };

    //!
    //!    @brief ページペインを取得します。
    //!
    nn::ui2d::Pane* GetPagePane() const { return m_pPagePane; };

    enum OutOfBoundsInfo {
        OutOfBoundsInfo_None,
        OutOfBoundsInfo_Short,
        OutOfBoundsInfo_Over
    };

    //!
    //!    @brief ページがスライドリストの領域外にはみ出しているかどうかを返します。
    //!
    OutOfBoundsInfo GetOutOfBoundsInfo() const;

protected:
    SlideListControl* m_pSlideListControl;

    nn::ui2d::Pane** m_ppElementPanes;
    int m_CountElementPanes;

    nn::ui2d::AnimButton** m_ppElementButtons;
    int m_CountElementButtons;

    nn::ui2d::Pane* m_pPagePane;

    int m_CurrentPageIndex;

};

//------------------------------------------------------------------------------

//!
//!    @brief スライドリストの要素
//!
//!    - 要素を表します。
//!    - スライドリストで扱うものの最大個数分あります。
//!    - ペインとは一対一対応しません。必要に応じてページからペインが割り当てられます。
//!    - 番号付けされていますが、内部では保持していません。コールバックで与えられます。
//!
class SlideListElement
{
public:
    //!
    //!    @brief コンストラクタです。
    //!
    SlideListElement() {}

    //!
    //!    @brief デストラクタです。
    //!
    virtual ~SlideListElement();

    /// 初期化引数構造体です。
    struct InitializeArg {
        int index;
        SlideListControl* pControl;

        InitializeArg();
    };

    /// enterの引数構造体です。
    struct EnterArg {
        int index;
        SlideListControl* pControl;
        SlideListPage* pPage;
        nn::ui2d::Pane* pElementPane;

        //!
        //!    @brief element_paneを部品ペインにキャストして返すユーティリティメソッドです。
        //!
        //!    element_paneが部品ペインではない場合はnullptrを返します。
        //!
        nn::ui2d::Parts* getElementParts() const
        {
            return nn::ui2d::DynamicCast<nn::ui2d::Parts*>(pElementPane);
        }

        EnterArg();
    };

    /// leaveの引数構造体です。
    struct LeaveArg {
        int index;
        SlideListControl* pControl;
        SlideListPage* pPage;
        nn::ui2d::Pane* pElementPane;

        //!
        //!    @brief element_paneを部品ペインにキャストして返すユーティリティメソッドです。
        //!
        //!    element_paneが部品ペインではない場合はnullptrを返します。
        //!
        nn::ui2d::Parts* GetElementParts() const
        {
            return nn::ui2d::DynamicCast<nn::ui2d::Parts*>(pElementPane);
        }

        LeaveArg();
    };

    //!
    //!    @brief 構築時に呼び出される処理です。
    //!
    virtual void Initialize(const InitializeArg& initArg);

    //!
    //!    @brief 画面内に要素が入った時に呼び出される処理です。
    //!
    virtual void Enter(const EnterArg& enterArg);

    //!
    //!    @brief 画面から要素が出た時に呼び出される処理です。
    //!
    virtual void Leave(const LeaveArg& leaveArg);

protected:

};

//------------------------------------------------------------------------------
template <typename SlideListPageClass, typename SlideListElementClass>
void SlideListControl::Initialize(const InitializeArg& initArg)
{
    m_IsValid = true;

    if (m_pBasePane == nullptr) {
        // データが不正であることにより、初期化不可能。noticeはコンストラクタで出している
        m_IsValid = false;
        return;
    }

    nn::ui2d::Pane* pPagePaneFirst = FindPagePaneFirst_();
    if (pPagePaneFirst == nullptr)
    {
        m_IsValid = false;
        return;
    }

    // Pageが幾つ必要かを求める

    // PageとElementを構築
    {
        const int pageCount = CalculatePageNum_(pPagePaneFirst);

        SlideListPageClass* pPages = Layout::NewArray<SlideListPageClass>(pageCount);
        m_ppPages = Layout::NewArray<SlideListPageClass*>(pageCount);
        m_CountPages = pageCount;

        for (int i = 0; i < pageCount; i++)
        {
            m_ppPages[i] = &pPages[i];
        }
    }

    {
        m_ElementNumLimit = initArg.element_num;

        SlideListElementClass* pElements = Layout::NewArray<SlideListElementClass>(initArg.element_num);
        m_ppElements = Layout::NewArray<SlideListElement*>(initArg.element_num);
        m_CountElements = initArg.element_num;

        for (int i = 0; i < initArg.element_num; i++)
        {
            m_ppElements[i] = &pElements[i];
        }
    }

    DoInitialize_(initArg.pDevice, pPagePaneFirst);
}

} // namespace nn::ui2d
} // namespace nn
