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

#include <nn/ui2d/viewer/ui2d_Config.h>
#if defined(NN_UI2D_VIEWER_ENABLED)

#include <nn/ui2d.h>

namespace nn
{
namespace ui2d
{
namespace viewer
{

//---------------------------------------------------------------------------
//! @brief 指定されたレイアウトの中で、アニメーションタグにキーが含まれている全てのペインをアニメーションさせるAnimatorです。
//!
//! @since 2010/12/2 初版。
//---------------------------------------------------------------------------
class AllPaneAnimator : public nn::ui2d::Animator
{
public:
    AllPaneAnimator()
        : nn::ui2d::Animator()
        , m_Layout(NULL)
    {}

    /**
     *    @brief 指定されたレイアウトの中で、アニメーションタグにキーが含まれている全てのペインにバインドし、
     *           内部にレイアウトのポインタを保持します。
     *
     *  @param[in] layout    ペインをバインドしたいレイアウト
     *  @param[in] enabled   開始時にアニメーションを有効状態にするか
     */
    void Setup(nn::ui2d::Layout* layout, bool enabled);

    /**
     *  @brief Setupで関連付けたアニメーションをバインド解除します。
     */
    virtual void Unbind();

private:

    nn::ui2d::Layout* m_Layout;      //! 関連づけられたレイアウト

};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
