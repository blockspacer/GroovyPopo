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

#include <nn/vfx/viewer/vfx_Preview.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  モデルプレビュークラスです。
//---------------------------------------------------------------------------
class ModelPreview : public Preview
{
    NN_DISALLOW_COPY( ModelPreview );
public:

    //----------------------------------------
    //! @name 初期化/終了 処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    ModelPreview() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定 処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief        描画のOn/Offを状態を取得します。
    //! @return       描画のOn/Off状態
    //---------------------------------------------------------------------------
    bool IsVisible() const NN_NOEXCEPT
    {
        return m_IsVisible;
    }

    //---------------------------------------------------------------------------
    //! @brief                  描画のOn/Offを設定します。
    //! @param[in]    visibility   描画のOn/Off指定
    //---------------------------------------------------------------------------
    void SetVisible( bool visibility ) NN_NOEXCEPT
    {
        m_IsVisible = visibility;
    };

    //@}

private:
    bool            m_IsVisible;              //!< モデルの描画フラグ
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
