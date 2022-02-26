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

#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_ButtonBase.h>

namespace nn
{

namespace ui2d
{

class Animator;
class ControlSrc;
class Layout;
class Pane;

//---------------------------------------------------------------------------
//! @brief ButtonBaseに、アニメーションやあたり判定、ButtonGroupとの連携等の機能を追加したクラスです。
//!
//! @details
//! ctrlモジュールのボタンの基底としては、ButtonBaseではなくこちらを使用してください。
//!
//---------------------------------------------------------------------------
class AnimButton : public ButtonBase
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //! @brief コンストラクタです。
    //!
    AnimButton();

    //! @brief ボタンの構築を行います。
    //!
    //! @param[in] pDevice  デバイス
    //! @param[in] pLayout   所属するレイアウト
    //! @param[in] controlSrc   コントロールの情報
    //!
    void Build(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc);

    //! @brief ボタンの構築を行います。（評価中のため直接の利用はしないでください）
    //!
    //! @details
    //! 追加情報を保持する Animator クラスを内部で生成するようにして、
    //! ボタンの複製処理を容易におこなうことが可能なボタンを構築します。
    //!
    //! @param[in] pDevice  デバイス
    //! @param[in] pLayout   所属するレイアウト
    //! @param[in] controlSrc   コントロールの情報
    //!
    void BuildEx(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc);

    //! @brief ボタン名の取得
    //!
    //! @details
    //! ボタンの名前は、ボタンが所属するレイアウトが部品レイアウトの場合は
    //! 部品ペイン名、部品レイアウトでない場合はレイアウト名に等しくなります。
    //!
    const char* GetName() const { return m_pName; }

    //----------------------------------------
    //! @name 当たり判定
    //@{
    //! @brief 当たり判定の範囲を更新します。
    //!
    //! @details
    //! この処理はcalcでは行われないため、明示的に呼び出す必要があります。
    //! ただし、通常はButtonGroupのcalcで呼ばれますので、個別に行う必要はありません。
    //! ButtonGroupのオプションで、更新を行うか否かを設定できるようになっています。
    //!
    //! 大きさが大きく変わらないボタンであれば、当たりの範囲を更新しないことにより、
    //! 処理を節約することができます。
    //!
    //! この処理はボタンの当たりを示すペインのGlobalMatrixを使用するため、
    //! 一回以上 nn::ui2d::Layout::Calculate() を行ってから呼び出すようにしてください。
    //!
    //! なお、このクラスでは、回転しているペインの当たり判定には対応していません。
    //! 回転しているペインの当たり判定に対応するには、このメソッド及びIsHit
    //! メソッドをオーバーライドする必要があります。
    //!
    virtual void UpdateHitBox();

    //! @brief 指定した位置にボタンが当たっているか否かを取得します。
    //!
    virtual bool IsHit(const nn::util::Float2& vec2) const;

    //! @brief 当たりの大きさを示すペインを取得します。
    //!
    ui2d::Pane* GetHitPane() const { return m_pHitPane; }

    //! @brief ボタンが所属するレイアウトを取得します。(試験実装中のため利用しないでください)
    //!
    //! @details
    //! デフォルトでは nullptr を返します。必要に応じて派生クラスで対応をするようにします。
    //!
    const ui2d::Layout* GetLayout() const { return const_cast<AnimButton*>(this)->GetLayout(); }

    //! @brief ボタンが所属するレイアウトを取得します。(試験実装中のため利用しないでください)
    //!
    //! @details
    //! デフォルトでは nullptr を返します。必要に応じて派生クラスで対応をするようにします。
    //!
    virtual ui2d::Layout* GetLayout() { return nullptr; }

    //! @brief ボタンが所属するレイアウトを設定します。(試験実装中のため利用しないでください)
    //!
    //! @param[in] pLayout  ボタンが所属するレイアウトです。
    //!
    //! @details
    //! デフォルトでは 何も処理を行いません。必要に応じて派生クラスで対応をするようにします。
    //!
    virtual void SetLayout(ui2d::Layout* pLayout)
    {
        NN_UNUSED(pLayout);
    }

    //! @brief 当たりの大きさを示すペインを取得します。
    //!
    void SetHitPane(ui2d::Pane* pHitPane) { m_pHitPane = pHitPane; }

    //! @brief 当たりの範囲の左下の位置（最小点）を取得します。
    //!
    const nn::util::Float2& GetHitBoxBottomLeft() const { return m_HitBoxBottomLeft; }

    //! @brief 当たりの範囲の右上の位置（最大点）を取得します。
    //!
    const nn::util::Float2& GetHitBoxTopRight() const { return m_HitBoxTopRight; }
    //@}

    //----------------------------------------
    //! @name タグ
    //@{
    //! @brief タグの値を取得します。
    //!
    int GetTag() const { return m_Tag; }

    //! @brief タグの値を取得します。
    //!
    //! @details
    //! タグとは、各ボタンに一つ設定できる整数値です。
    //! タグはボタンの内部では一切使用されませんので、ボタンを使う側が使い方を決めることができます。
    //!
    //! 主にボタンの判別を行いやすくする用途を想定しています。
    //!
    //! 初期値は0になっています。
    //!
    void SetTag(int tag) { m_Tag = tag; }
    //@}

    //----------------------------------------
    //! @name ボタンを押したときの排他処理
    //@{
    //! @brief ボタンが押されたときに、同じボタングループに属するボタンが押されることを防ぐか否かを取得します。
    //!
    //! @details
    //! この設定は、AnimButton内では使用せず、ButtonGroupから使用されます。
    //!
    //! 押した後に画面が切り替わるボタンなど、同時に複数のボタンが押されて欲しくない場合に使用する機能です。
    //!
    //! @return
    //! trueのときは、同じボタングループに属するボタンが押されることを防ぎます。
    //!
    bool IsExcludeDown() const { return GetFlag().IsAllBitOn(FlagMask_MaskExcludeDown); }

    //! @brief ボタンが押されたときに、同じボタングループに属するボタンが押されることを防ぐか否かを設定します。
    //!
    //! @details
    //! デフォルトではfalse（同じボタングループに属するボタンが押されることを防がない）です。
    //!
    //! @param[in] excludeDown  trueを与えると、同じボタングループに属するボタンが押されることを防ぎます。
    //!
    void SetExcludeDown(bool excludeDown) { GetFlag().SetMaskedBits(FlagMask_MaskExcludeDown, excludeDown ? FlagMask_MaskExcludeDown : 0); }
    //@}

    //----------------------------------------
    //! @name ドラッグに関する機能
    //@{
    //! ドラッグのモードです。
    enum DragMode {
        DragMode_Disable,  //!< ドラッグは無効です。
        DragMode_OnDown,  //!< ボタンが押されたときにドラッグ状態になります。
        DragMode_OnHit    //!< ボタンに当たったときにドラッグ状態になります。
    };

    //! @brief ドラッグのモードを取得します。
    //!
    //! @details
    //! この設定は、AnimButton内では使用せず、ButtonGroupから使用されます。
    //!
    //! @return
    //! ドラッグのモード
    //!
    DragMode GetDragMode() const { return static_cast<DragMode>((GetFlag().storage >> FlagMask_BitDragModeStart) & 0x3); }

    //! @brief ドラッグのモードを設定します。
    //!
    //! @details
    //! この設定は、AnimButton内では使用せず、ButtonGroupから使用されます。
    //!
    //! デフォルトではDRAG_MODE_DISABLE（ドラッグが無効）です。
    //!
    //! @param[in] mode ドラッグのモード
    //!
    void SetDragMode(DragMode mode) { GetFlag().storage = (GetFlag().storage & ~FlagMask_MaskDragMode) | (mode << FlagMask_BitDragModeStart) ; }

    //! @brief ドラッグ時の位置を初期化します。
    //!
    //! @details
    //! このクラスでは空の実装となっています。ドラッグを使用する場合は、継承先でオーバーライド
    //! する必要があります。
    //!
    //! @param[in] pos  現在ドラッグしている位置です。
    //!
    virtual void InitializeDragPosition(const nn::util::Float2& pos);

    //! @brief ドラッグ時の位置を更新します。
    //!
    //! @details
    //! このクラスでは空の実装となっています。ドラッグを使用する場合は、継承先でオーバーライド
    //! する必要があります。
    //!
    //! @param[in] pPos  現在ドラッグしている位置です。ポインタが画面から外れている場合はNULLとなります。
    //!
    virtual void UpdateDragPosition(const nn::util::Float2* pPos);
    //@}

    //----------------------------------------
    //! @name コールバックの設定
    //@{
    //! @brief ボタンのステートが変化したときのコールバックです。
    //!
    //! @details
    //! このコールバック内では、ボタンのステートの変更(ForceXXX)は行わないでください。
    //! ステートを変更しても、その後の処理でステートが移ってしまう可能性があるためです。
    //!
    //! @param[in] pButton   押されたボタン
    //! @param[in] prevState    変化前のステート
    //! @param[in] nextState    変化後のステート
    //! @param[in] pParam    ボタンが押されたときに設定したパラメータ
    //!
    typedef void (*StateChangeCallback)(AnimButton* pButton, State prevState, State nextState, void* pParam);

    //! @brief ボタンのステートが変化したときのコールバックを設定します。
    //!
    //! @details
    //! なお、このコールバックは、ForceXXXメソッドで強制的にステートを変更した場合には呼び出されません。
    //!
    //! コールバックを解除するには、pCallbackにNULLを与えてください。
    //!
    //! @param[in] pCallback 呼び出したいコールバック
    //! @param[in] pParam    コールバックに与えるパラメータ
    //!
    void SetStateChangeCallback(StateChangeCallback pCallback, void* pParam);
    //@}

    //----------------------------------------
    //! @name アニメーションの管理
    //@{
    //! @brief 無効アニメーションを再生します。
    //!
    //! @details
    //! 無効アニメーションを再生します。無効アニメーションの再生は、ボタンの有効・無効状態とは独立であることに
    //! ご注意ください。ボタンを無効にすると同時に無効アニメーションを再生したい場合は、SetActive(false)
    //! とPlayDisableAnim(true)を両方呼び出してください。
    //!
    //! アニメーションの再生は、内部的にPlayFromCurrentで行われているため、既に無効アニメーション状態にした後に
    //! PlayDisableAnim(true)を呼び出しても、状態は変わらないことにご注意ください。
    //!
    //! @param[in] isDisable   trueの場合はDisableアニメーションが順再生されて無効アニメーション状態になり、
    //!                         falseの場合は逆再生されて有効アニメーション状態になります。
    //!
    virtual void PlayDisableAnim(bool isDisable);

    //! @brief ボタンのOn/Offアニメーションを取得します。
    //!
    //! @return アニメーション
    //!
    ui2d::Animator* GetOnOffAnimator() const { return m_pOnOffAnimator; }

    //! @brief ボタンのDownアニメーションを取得します。
    //!
    //! @return アニメーション
    //!
    ui2d::Animator* GetDownAnimator() const { return m_pDownAnimator; }

    //! @brief ボタンのCancelアニメーションを取得します。
    //!
    //! @return アニメーション
    //!
    ui2d::Animator* GetCancelAnimator() const { return m_pCancelAnimator; }

    //! @brief ボタンのDisableアニメーションを取得します。
    //!
    //! @return アニメーション
    //!
    ui2d::Animator* GetDisableAnimator() const { return m_pDisableAnimator; }

    //! @brief ボタンのOn/Offアニメーションを設定します。
    //!
    //! @param[in] pAnimator 設定するアニメーション
    //!
    void SetOnOffAnimator(ui2d::Animator* pAnimator) { m_pOnOffAnimator = pAnimator; }

    //! @brief ボタンのDownアニメーションを設定します。
    //!
    //! @param[in] pAnimator 設定するアニメーション
    //!
    void SetDownAnimator(ui2d::Animator* pAnimator) { m_pDownAnimator = pAnimator; }

    //! @brief ボタンのCancelアニメーションを設定します。
    //!
    //! @param[in] pAnimator 設定するアニメーション
    //!
    void SetCancelAnimator(ui2d::Animator* pAnimator) { m_pCancelAnimator = pAnimator; }

    //! @brief ボタンのDisableアニメーションを設定します。
    //!
    //! @param[in] pAnimator 設定するアニメーション
    //!
    void SetDisableAnimator(ui2d::Animator* pAnimator) { m_pDisableAnimator = pAnimator; }

    //! @brief ボタンが保持している全てのアニメーションを無効にします。
    //!
    //! @details
    //! これは、GetXXXAnimatorで全てのアニメーションを取得し、それぞれにSetEnabled(false)を呼ぶのと同じ動作を行います。
    //! 通常は使う必要はありませんが、ボタンの通常の動作を止め、別なアニメーションを再生したいとき等に有効です。
    //! なお、ボタンのアニメーションは必要になったら自動的に有効化されますので、状態を戻す必要はありません。
    //!
    virtual void SetAllAnimatorDisable();
    //@}

    // 強制的にステートを変更したときにアニメーションの状態を変更するためにオーバーライドしています。
    virtual void ForceOff() NN_OVERRIDE;
    virtual void ForceOn() NN_OVERRIDE;
    virtual void ForceDown() NN_OVERRIDE;

    //! @details :private
    // ButtonGroupが管理する際に用いるリンクです。
    nn::util::IntrusiveListNode m_Link;

protected:
    static const int    FlagBitMax = 6;    //!< m_Flagで使用しているビットの最大値

    //! @brief 状態フラグのマスク値です。
    enum FlagMask {
        //! このボタンが押されたときに、同じボタングループ内の他のボタンが押されるのを妨げるか否か
        FlagMask_MaskExcludeDown = 1 << (ButtonBase::FlagBitMax + 1),
        //! このボタンのドラッグモードを保持するビットの開始点
        FlagMask_BitDragModeStart = ButtonBase::FlagBitMax + 2,
        //! このボタンのドラッグモードを保持するビットのマスク
        FlagMask_MaskDragMode = 0x3 << FlagMask_BitDragModeStart
    };

    //! @brief ボタン名の設定
    //!
    //! @param[in] pName      ボタン名です。
    //!s
    void SetName(const char* pName) { m_pName = pName; }

    // AnimButtonの機能を実装するためのオーバーライドです。
    virtual void StartOn() NN_OVERRIDE;
    virtual void StartOff() NN_OVERRIDE;
    virtual void StartDown() NN_OVERRIDE;
    virtual void StartCancel() NN_OVERRIDE;

    virtual bool UpdateOn() NN_OVERRIDE;
    virtual bool UpdateOff() NN_OVERRIDE;
    virtual bool UpdateDown() NN_OVERRIDE;
    virtual bool UpdateCancel() NN_OVERRIDE;

    virtual bool ProcessCancel() NN_OVERRIDE;

    virtual void ChangeState(State state) NN_OVERRIDE;

    ///! 指定したアニメーションを有効化し、それ以外を無効化します。
    void EnableAnim(ui2d::Animator* pAnimator);

    ///! クローン処理を行います。
    void CloneImpl_(nn::gfx::Device* pDevice, const AnimButton& src, ui2d::Layout* pLayout);


private:
    StateChangeCallback m_pStateChangeCallback;
    void* m_pStateChangeCallbackParam;
    ui2d::Animator* m_pOnOffAnimator;
    ui2d::Animator* m_pDownAnimator;
    ui2d::Animator* m_pCancelAnimator;
    ui2d::Animator* m_pDisableAnimator;
    ui2d::Pane* m_pHitPane;
    nn::util::Float2 m_HitBoxBottomLeft;
    nn::util::Float2 m_HitBoxTopRight;
    int m_Tag;
    const char* m_pName;
};

} // namespace nn::ui2d
} // namespace nn
