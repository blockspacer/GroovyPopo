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

#include <nn/ui2d/ui2d_AnimButton.h>

#include <nn/gfx.h>

namespace nn
{

namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief ボタンが押されるたびにトグルで特定のアニメーションの状態が切り替わるボタンです。
//!
//! @details
//! NormalButtonとほぼ同じ挙動ですが、いわゆるチェックボックスのようなUIを実現するための
//! ボタンです。
//!
//! このボタンには、ON/OFFとDOWNの他にもう一つ、Checkアニメーションと呼ばれるアニメーションが必要です。
//! Cancelアニメーションはパラメータアニメーションの3つ目に指定してください。
//! ボタンが押されると、Checkアニメーションが順再生されます。この状態でもう一度ボタンが押されると、
//! 今度はCheckアニメーションが逆再生されます。
//!
//! CheckButtonのSTATE_DOWNは、Checkアニメーションが再生終了しない限り終了しませんのでご注意ください。
//!
//! また、CheckアニメーションはDownアニメーションと同時に再生しますので、同時に再生しても問題ないアニメーションに
//! してください。例えばDownアニメーションではスケールを使用して、Checkアニメーションではマテリアルカラー
//! を使用する、あるいはCheckアニメーションの対象のペインはDownアニメーションとは全く別のペインにする
//! 等の方法があります。
//!
//! チェックマークが入った状態か否かは、IsChecked()メソッドで取得することができます。
//!
//! CheckButtonは、NormalButtonと同様にデフォルトではIsExcludeDownがfalseになっています。
//!
//---------------------------------------------------------------------------
class CheckButton : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    CheckButton() : AnimButton(), m_IsChecked(false), m_pCheckAnimator(NULL) {}

    //! @brief ボタンの構築を行います。
    //!
    //! AnimButtonの標準の動作に加えて、パラメータアニメーションの3つ目に指定された
    //! Checkアニメーションを作成します。
    //!
    //! @param[in] pDevice      デバイス
    //! @param[in] pLayout   所属するレイアウト
    //! @param[in] controlSrc   コントロールの情報
    //!
    void Build(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc);

    //----------------------------------------
    //! @name アニメーションの管理
    //@{
    //! @brief ボタンのCheckアニメーションを取得します。
    //!
    //! @return アニメーション
    //!
    ui2d::Animator* GetCheckAnimator() const { return m_pCheckAnimator; }

    //! @brief ボタンのCheckアニメーションを設定します。
    //!
    //! @param[in] pAnimator 設定するアニメーション
    //!
    void SetCheckAnimator(ui2d::Animator* pAnimator) { m_pCheckAnimator = pAnimator; }
    //@}

    //----------------------------------------
    //! @name 状態の取得・設定
    //@{
    //! @brief チェック状態か否かを取得します。
    //!
    //! @return チェック状態であればtrue。Build直後はfalseになっています。
    //!
    bool IsChecked() const { return m_IsChecked; }

    //! @brief 強制的にチェック状態を変更し、チェックアニメーションも指定した状態に設定します。
    //!
    //! @details
    //! レイアウトが表示されているときにこのメソッドを使用すると、急に状態が変わった
    //! ように見えてしまう場合がありますのでご注意ください。
    //!
    //! レイアウトを表示する前に、既にチェックされた状態にしておきたいような場合に
    //! 使うことを想定しています。
    //!
    //! @param[in] checked  チェック状態にするか否か。
    //!
    void ForceSetChecked(bool checked);
    //@}

protected:

    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual void FinishDown() NN_OVERRIDE;
    virtual void StartDown() NN_OVERRIDE;
    virtual bool UpdateDown() NN_OVERRIDE;

private:

    bool m_IsChecked;
    ui2d::Animator* m_pCheckAnimator;
};

} // namespace nn::ui2d
} // namespace nn
