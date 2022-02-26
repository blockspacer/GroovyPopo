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
//! @brief DOWNアニメーションを再生した後にON状態に戻ることができるボタンです。
//!
//! @details
//! NormalButtonは、いわゆるPCのUI等でよく見られる、押した後に元の状態に
//! 戻ることができるボタンです。
//!
//! このため、デザイナには、DOWNアニメーションの始まりと終わりの状態が同じになる
//! ようにアニメーションを作成していただく必要があります。
//!
//! 状態の変化の仕方としては、DOWNアニメーションが終了してSTATE_DOWN_IDLEとなった
//! 直後にSTATE_ON_IDLEとなります。
//!
//! 同じフレームに、STATE_DOWNからSTATE_DOWN_IDLE、STATE_DOWN_IDLEから
//! STATE_ON_IDLEという二回のコールバックが来ることになりますのでご注意
//! ください。
//!
//! また、DecisionButtonと異なり、NormalButtonはデフォルトではIsExcludeDown
//! がfalseになっていますので、押されたときに他のボタンが押されることを
//! 妨げません。
//!
//---------------------------------------------------------------------------
class NormalButton : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    NormalButton();


protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual void FinishDown() NN_OVERRIDE;
};

//---------------------------------------------------------------------------
//! @brief DOWNアニメーションを再生した後にON状態に戻ることができるボタンです。
//!
//! @details
//! NormalButton と同様の挙動を取りますが、LayoutEx AnimatorEx を使って動作します。
//! 試験実装版につき利用しないでください。
//!
//---------------------------------------------------------------------------
class NormalButtonEx : public AnimButton
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimButton);

    //! @brief コンストラクタです。
    //!
    NormalButtonEx();
    NormalButtonEx(nn::gfx::Device* pDevice, const NormalButtonEx& src, nn::ui2d::Layout* pLayout);

    //! @brief ボタンが所属するレイアウトを取得します。
    //!
    virtual ui2d::Layout* GetLayout() NN_OVERRIDE
    {
        return m_pLayout;
    }

    //! @brief ボタンが所属するレイアウトを設定します。
    //!
    //! @param[in] pLayout  ボタンが所属するレイアウトです。
    //!
    virtual void SetLayout(ui2d::Layout* pLayout) NN_OVERRIDE
    {
        m_pLayout = pLayout;
    }

protected:
    //! @brief ボタンの挙動をカスタマイズするためにオーバーライドします。
    //!
    virtual void FinishDown() NN_OVERRIDE;

    ui2d::Layout* m_pLayout;
};

} // namespace nn::ui2d
} // namespace nn
