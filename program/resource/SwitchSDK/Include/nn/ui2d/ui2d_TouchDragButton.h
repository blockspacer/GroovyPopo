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

#include <nn/ui2d/ui2d_DragButton.h>

#include <nn/gfx.h>

namespace nn
{

namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief タッチスクリーン用のボタンを掴んで移動させることができるボタンです。
//!
//! @details
//! 他のボタンと異なり、このボタンにはON/OFFアニメーションがありません。
//!
//! タッチスクリーン上で当たりが発生すると、Touchアニメーションが再生されて、ドラッグした
//! 状態になります。タッチスクリーンから離すとReleaseアニメーションが再生されて元に戻ります。
//!
//! Touchアニメーションは機能アニメーションの1つ目に、Releaseアニメーションは2つ目に指定してください。
//!
//---------------------------------------------------------------------------
class TouchDragButton : public DragButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(DragButton);

    //! @brief コンストラクタです。
    //!
    TouchDragButton();

    //! @brief ボタンの構築を行います。
    //!
    //! @param[in] pDevice      デバイス
    //! @param[in] pLayout   所属するレイアウト
    //! @param[in] controlSrc   コントロールの情報
    //!
    void Build(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc);

protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual bool ProcessOn() NN_OVERRIDE;
    virtual bool ProcessCancel() NN_OVERRIDE;
    virtual void FinishCancel() NN_OVERRIDE;

};

} // namespace nn::ui2d
} // namespace nn
