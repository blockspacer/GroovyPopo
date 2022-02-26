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
//! @brief ボタンが押されたときにSTATE_DOWN_IDLEのままになりますが、そこからCancelアクションを
//!        発行することでSTATE_OFFに戻ることができるボタンです。
//!
//! @details
//! DecisionButtonと挙動が似ていますが、STATE_DOWN_IDLEの状態でCancelアニメーションが発行されると、
//! STATE_OFFに戻ることができます。
//!
//! このため、ON/OFFとDOWNの他にもう一つ、DOWN_IDLEからOFF_IDLEに戻るアニメーション(Cancelアニメーション)が
//! 必要です。Cancelアニメーションはパラメータアニメーションの3つ目に指定してください。
//!
//! SelectButtonは、NormalButtonと同様にデフォルトではIsExcludeDownがfalseになっています。
//!
//---------------------------------------------------------------------------
class SelectButton : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    SelectButton() : AnimButton() {}

    //! @brief ボタンの構築を行います。
    //!
    //! AnimButtonの標準の動作に加えて、パラメータアニメーションの3つ目に指定された
    //! キャンセルアニメーションを作成します。
    //!
    //! @param[in] pDevice      デバイス
    //! @param[in] pLayout       所属するレイアウト
    //! @param[in] controlSrc   コントロールの情報
    //!
    void Build(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc);

    //! @brief 指定した位置にボタンが当たっているか否かを取得します。
    //!
    //! @details
    //! Cancelアニメーションを再生しているときは当たりが発生しないようにします。
    //!
    virtual bool IsHit(const nn::util::Float2& vec2) const NN_OVERRIDE;

protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual bool ProcessOff() NN_OVERRIDE;
    virtual bool ProcessCancel() NN_OVERRIDE;
    virtual void FinishCancel() NN_OVERRIDE;

};

} // namespace nn::ui2d
} // namespace nn
