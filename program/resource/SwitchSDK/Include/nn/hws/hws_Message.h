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
*   @file
*   @brief  ウィンドウメッセージに関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace hws {

/*!
*  @brief ウィンドウメッセージを処理します。
*
*  @pre        なし
*
*  @post
*         - 処理待ちのウィンドウメッセージが無い
*
*  @details
*         アプリケーションの終了要求を受け取った場合はプログラムを強制終了します。
*/
void ProcessMessage() NN_NOEXCEPT;

}
} // namespace nn
