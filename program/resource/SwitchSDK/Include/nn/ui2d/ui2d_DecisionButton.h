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

namespace nn
{

namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief DOWNアニメーションを再生した後にDOWN_IDLEのままになるボタンです。
//!
//! @details
//! DecisionButtonは、ゲームのUIでよく見られる、押した後にそのままになって
//! 戻ってこないボタンです。
//!
//! 押した後戻ってこないため、その後画面が切り替わったりして、ボタンが
//! そのまま隠れるような状況でのみ使うことができます。
//!
//! NormalButtonと異なり、Downアニメーションの最初と最後の状態を合わせる必要が
//! ないため、デザインの自由度が高くなります。
//!
//! また、NormalButtonと異なり、DecisionButtonはデフォルトではIsExcludeDown
//! がtrueになっていますので、押されたときに他のボタンが押されることを妨げます。
//!
//! 押された後に状態がDOWN_IDLEのままになることから、一度DecisionButtonが
//! 押されると、同じボタングループ内では当たりが発生しなくなります。
//!
//---------------------------------------------------------------------------
class DecisionButton : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    DecisionButton() : AnimButton()
    {
        SetExcludeDown(true);
    }

protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual bool ProcessOn() NN_OVERRIDE;
    virtual bool ProcessOff() NN_OVERRIDE;

};

} // namespace nn::ui2d
} // namespace nn
