/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/** @file
    @brief      NFC ライブラリの初期化/終了に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>

namespace nn {
namespace nfc {

/**
 * @name    初期化/終了処理
 * @{
 */

/**
 * @brief   NFC ライブラリを初期化します。
 *
 * @pre
 *  - GetState() == @ref State_None
 *
 * @post
 *  - GetState() == @ref State_Init
 *
 * @details
 *  この関数は、NFC ライブラリの各機能を使用する前に呼び出す必要があります。
 */
void Initialize() NN_NOEXCEPT;

/**
 * @brief   NFC ライブラリを終了します。
 *
 * @pre
 *  - ライブラリが @ref Initialize() にて初期化済みの状態である必要があります。
 *  - GetState() == @ref State_Init
 *
 * @post
 *  - GetState() == @ref State_None
 *
 * @details
 *  この関数は NFC ライブラリの使用を終える際に呼び出してください。
 *  必要に応じて @ref ReleasePassThroughSession() 、 @ref StopDetection() が内部で自動的に実行されます。
 */
void Finalize() NN_NOEXCEPT;

/**
 * @}
 */

}  // nfc
}  // nn
