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

#include <nn/ui2d/ui2d_CheckButton.h>

namespace nn
{

namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief ボタンが押されるたびにトグルで特定のアニメーションの状態が切り替わるボタンのTouchOff版です。
//!
//! @details
//! このボタンは、CheckButtonの押されたときの挙動を、NormalButtonではなくTouchOffButtonの
//! 挙動にしたものです。OnOffアニメーション及びDownアニメーションの作り方はTouchOffButtonと同じです。
//!
//! Checkアニメーションについては、CheckButtonと変わりありません。
//!
//---------------------------------------------------------------------------
class TouchOffCheckButton : public CheckButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(CheckButton);

    //! @brief コンストラクタです。
    //!
    TouchOffCheckButton() : CheckButton() {}

protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual bool ProcessOff() NN_OVERRIDE;
    virtual bool ProcessOn() NN_OVERRIDE;
    virtual void FinishDown() NN_OVERRIDE;

};

} // namespace nn::ui2d
} // namespace nn
