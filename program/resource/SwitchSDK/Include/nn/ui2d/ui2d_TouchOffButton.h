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
//! @brief DOWNアニメーションを再生した後にOFF状態に戻るボタンです。
//!
//! @details
//! TouchOffButtonは、NormalButtonと似た挙動のボタンですが、押したあとON状態
//! ではなく、OFF状態に戻ります。
//!
//! このため、デザイナには、DOWNアニメーションの始まりとONアニメーションの始まりの状態が同じになる
//! ようにアニメーションを作成していただく必要があります。
//!
//! 状態の変化の仕方としては、DOWNアニメーションが終了してSTATE_DOWN_IDLEとなった
//! 直後にSTATE_OFF_IDLEとなります。
//!
//! 同じフレームに、STATE_DOWNからSTATE_DOWN_IDLE、STATE_DOWN_IDLEから
//! STATE_OFF_IDLEという二回のコールバックが来ることになりますのでご注意
//! ください。
//!
//! また、DecisionButtonと異なり、TouchOffButtonはデフォルトではIsExcludeDown
//! がfalseになっていますので、押されたときに他のボタンが押されることを
//! 妨げません。
//!
//! DRC等のタッチスクリーンのデバイスで、タッチしたときにボタンが凹み、その上で離すと
//! ボタンが押されるようにするには、押された後にOFF状態になった方が自然な挙動に
//! なるため、タッチスクリーンのデバイスにはこのボタンが向いています。
//!
//---------------------------------------------------------------------------
class TouchOffButton : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    TouchOffButton() : AnimButton() {}

protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual bool ProcessOff() NN_OVERRIDE;
    virtual bool ProcessOn() NN_OVERRIDE;
    virtual void FinishDown() NN_OVERRIDE;

};

} // namespace nn::ui2d
} // namespace nn
