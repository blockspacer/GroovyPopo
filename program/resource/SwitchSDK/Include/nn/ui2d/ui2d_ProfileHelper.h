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

#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

class Layout;
class Pane;

//---------------------------------------------------------------------------
//! @brief レイアウト用のプロファイル支援クラスです。
//---------------------------------------------------------------------------
class ProfileHelper
{
    NN_DISALLOW_COPY(ProfileHelper);

public:
    //----------------------------------------
    //! @name ユーティリティ
    //@{

    //! @brief        レイアウト内のグループの数を取得します。
    //!
    //! @param[in] layout   情報を取得するレイアウトです。
    //!
    //! @return グループ数です。
    //!
    static int GetQuantityOfGroup(const Layout& layout);

    //! @brief        レイアウト内のペインの数を取得します。
    //!
    //! @param[in] pPane 情報を取得するペインです。
    //!
    //! @return ペイン数です。
    //!
    static int GetQuantityOfAllPane(const Pane* pPane);

    //! @brief        レイアウト内の任意のペインの数を取得します。
    //!
    //! @param[in] paneType ペインの種類です。
    //! @param[in] pPane     情報を取得するペインです。
    //!
    //! @return ペイン数です。
    //!
    static int GetQuantityOf(PaneType paneType, const Pane* pPane);

    //@}

private:
    ProfileHelper() {}
    ~ProfileHelper() {}
};

} // namespace ui2d
} // namespace nn
