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

#include <nn/ui2d/ui2d_ControlCreator.h>
#include <nn/ui2d/ui2d_ButtonGroup.h>

#include <nn/gfx.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief レイアウトでデフォルトで用意しているコントロールを作成するためのクラスです。
//!
//---------------------------------------------------------------------------
class DefaultControlCreator : public ui2d::ControlCreator
{
public:
    //! @brief コンストラクタ
    //!
    //! SetButtonGroup() で ButtonGroup を設定する必要があります。
    //!
    DefaultControlCreator()
        : m_pButtonGroup(NULL)
    {}

    //! @brief コンストラクタ
    //!
    //! @param[in] pButtonGroup 生成したコントロールを登録する ButtonGroup を指定します。
    //!
    explicit DefaultControlCreator(ButtonGroup* pButtonGroup);

    //! @brief ButtonGroup を設定します。
    //!
    //! @param[in] pButtonGroup 生成したコントロールを登録する ButtonGroup を指定します。
    //!
    //! @returns
    //! DefaultControlCreator 自身への参照を返します。
    //!
    //! @sa GetButtonGroup
    //!
    DefaultControlCreator& SetButtonGroup(ButtonGroup* pButtonGroup)
    {
        m_pButtonGroup = pButtonGroup;
        return *this;
    }

    //! @brief 設定されている ButtonGroup を取得します。
    //!
    //! @returns
    //! 設定されている ButtonGroup へのポインタを返します。
    //!
    //! @sa SetButtonGroup
    //!
    ButtonGroup* GetButtonGroup() const
    {
        return m_pButtonGroup;
    }

    //! @brief 指定されたコントロールソースに対応するコントロールを作成します。
    //!
    //! @details
    //! 現在はボタンのみに対応しています。作成したボタンは、コンストラクタで与えられた
    //! ButtonGroupに格納します。
    //!
    //! @param[in] pDevice      デバイス
    //! @param[in] pLayout   コントロールを作成するレイアウト
    //! @param[in] controlSrc   作成すべきコントロールの情報
    //!
    virtual void CreateControl(nn::gfx::Device* pDevice, ui2d::Layout* pLayout, const ui2d::ControlSrc& controlSrc) NN_OVERRIDE;

private:

    ButtonGroup* m_pButtonGroup;
};

} // namespace nn::ui2d
} // namespace nn
