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

#include <nn/nn_Common.h>

namespace nn { namespace oe {

//-----------------------------------------------------------------------------
//! @name アプリケーションの状態管理機能 API
//! @{

/**
 * @brief       アプリケーション動作環境を操作する OE ライブラリを初期化します。
 *
 * @post        OE ライブラリは初期化済の状態になります。
 *
 * @details     アプリケーション動作環境を操作する OE ライブラリを初期化します。
 *              本関数の呼び出し回数はライブラリ内部でカウントされます。
 */
void Initialize() NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::oe
