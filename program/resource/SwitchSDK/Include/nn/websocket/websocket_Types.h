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

namespace nn { namespace websocket {

/**
 * @brief   ログレベル
 *
 * @details
 *  ログレベルは低い順（情報量の多い順）に並んでいます。
 */
enum class LogLevel
{
    Trace       = 0, //!< トレース情報
    Information = 1, //!< 情報
    Warning     = 2, //!< 警告
    Error       = 3, //!< エラー
    None        = 9, //!< 出力なし
};

/**
 * @brief   キャンセル関数
 *
 * @param[in]   pParam  ユーザーパラメータ
 *
 * @return  キャンセルするかどうか
 *
 * @details
 */
using CancellationFunction = bool (*)(void* pParam);

}}
