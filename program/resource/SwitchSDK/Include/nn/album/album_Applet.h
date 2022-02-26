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

/*!
    @file
    @brief   アルバムアプレットに関連する API を宣言します。
*/

#include <nn/nn_Common.h>

namespace nn { namespace album {

/*!
    @brief      アルバム内のファイルを一覧表示します。

    @details
                呼び出し元のアプリケーションに紐づいたファイルの一覧画面を表示します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
void ShowAlbumFiles() NN_NOEXCEPT;

}} // namespace nn::album
