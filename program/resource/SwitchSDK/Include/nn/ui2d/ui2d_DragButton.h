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
//! @brief ボタンを掴んで移動させることができるボタンです。
//!
//! @details
//! SelectButtonと挙動が似ていますが、STATE_DOWN_IDLEの状態でCancelアクションが発行されると、
//! STATE_ONに戻ります。
//!
//! ON/OFFとDOWNの他にもう一つ、DOWN_IDLEからON_IDLEに戻るアニメーション(Releaseアニメーション)が
//! 必要です。Releaseアニメーションは機能アニメーションの3つ目に指定してください。
//!
//! Releaseアニメーションは、内部的にはCancelアニメーションとして扱っています。
//!
//! なお、このボタンはタッチスクリーンでは正しく動作しません。タッチスクリーンで使う場合は、
//! TouchDragButtonを使用してください。
//!
//---------------------------------------------------------------------------
class DragButton : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    DragButton();

    //! @brief ボタンの構築を行います。
    //!
    //! AnimButtonの標準の動作に加えて、パラメータアニメーションの3つ目に指定された
    //! キャンセルアニメーションを作成します。
    //!
    //! @param[in] pDevice      デバイス
    //! @param[in] pLayout   所属するレイアウト
    //! @param[in] controlSrc   コントロールの情報
    //!
    void Build(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc);

    //! @brief ドラッグ時の位置を初期化します。
    //!
    //! @param[in] pos  現在ドラッグしている位置です。
    //!
    virtual void InitializeDragPosition(const nn::util::Float2& pos) NN_OVERRIDE;

    //! @brief ドラッグ時の位置を更新します。
    //!
    //! @param[in] pPos  現在ドラッグしている位置です。ポインタが画面から外れている場合はNULLとなります。
    //!
    virtual void UpdateDragPosition(const nn::util::Float2* pPos) NN_OVERRIDE;

    //! @brief ドラッグ時、水平方向に移動するか否かを取得します。
    //!
    //! @return 水平方向に移動するか否か
    //!
    bool IsMoveHorizontal() const
    {
        return m_IsMoveHorizontal;
    }

    //! @brief ドラッグ時、水平方向に移動するか否かを指定します。
    //!
    //! @details
    //! デフォルトでは移動する設定になっています。
    //!
    //! @param[in] b  水平方向に移動するか
    //!
    void SetMoveHorizontal(bool b)
    {
        m_IsMoveHorizontal = b;
    }

    //! @brief ドラッグ時、垂直方向に移動するか否かを取得します。
    //!
    //! @return 垂直方向に移動するか否か
    //!
    bool IsMoveVertical() const
    {
        return m_IsMoveVertical;
    }

    //! @brief ドラッグ時、垂直方向に移動するか否かを指定します。
    //!
    //! @details
    //! デフォルトでは移動する設定になっています。
    //!
    //! @param[in] b  垂直方向に移動するか
    //!
    void SetMoveVertical(bool b)
    {
        m_IsMoveVertical = b;
    }

protected:

    //! @brief ルートペインを設定します。
    //!
    //! @param[in] pPane  ルートペインです。
    //!
    void SetRootPane(ui2d::Pane* pPane) { m_pRootPane = pPane; }

    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual bool ProcessOff() NN_OVERRIDE;
    virtual bool ProcessCancel() NN_OVERRIDE;
    virtual void FinishCancel() NN_OVERRIDE;

private:

    ui2d::Pane* m_pRootPane;
    nn::util::Float2 m_DragStartPos;
    nn::util::Float2 m_DragStartTranslate;
    bool m_IsMoveHorizontal;
    bool m_IsMoveVertical;

};

} // namespace nn::ui2d
} // namespace nn
