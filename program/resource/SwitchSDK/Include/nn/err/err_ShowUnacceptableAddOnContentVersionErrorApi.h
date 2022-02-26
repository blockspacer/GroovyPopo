/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief エラー表示用APIの定義
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace err {

//! @name エラー表示用API
//! @{

/**
 * @brief 現在インストールされている追加コンテンツのバージョンが許容できない場合に更新を促すエラーを表示します。
 * @details
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 * アプリケーション独自のロジックで、インストールされている追加コンテンツのバージョンが許容できないことを判定した場合に使用してください。
 */
void ShowUnacceptableAddOnContentVersionError() NN_NOEXCEPT;

//! @}
//-----------------------------------------------------------------------------

}} // namespace nn::err
