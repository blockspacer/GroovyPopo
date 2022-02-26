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

#include <nn/websocket/websocket_Types.h>
#include <nn/nn_Allocator.h>

namespace nn { namespace websocket {

//! @name 設定
//! @{

/**
 * @brief   アロケータを設定します。
 *
 * @param[in]   pAllocateFunction   メモリ確保関数
 * @param[in]   pFreeFunction       メモリ解放関数
 *
 * @pre
 *  - pAllocateFunction != nullptr
 *  - pFreeFunction != nullptr
 *
 * @details
 *  ライブラリが使用するアロケータをアプリケーション独自のものに置き換えます。
 *
 * @note
 *  デフォルトでは std::malloc() と std::free() を使用します。
 */
void SetAllocator(nn::AllocateFunction pAllocateFunction, nn::FreeFunction pFreeFunction) NN_NOEXCEPT;

/**
 * @brief   ログレベルを設定します。
 *
 * @param[in]   level   ログレベル
 *
 * @details
 *  ログレベルを設定することで、ライブラリ内のログが出力されるようになります。
 *
 *  ログレベルと出力される内容の関係は以下のとおりです。
 *
 *  | ログレベル | 内容 |
 *  | :- | :- |
 *  | LogLevel::Trace       | 通信毎の情報 |
 *  | LogLevel::Information | 接続処理の通信内容や時間等 |
 *  | LogLevel::Warning     | 継続可能なエラー発生時のエラー内容 |
 *  | LogLevel::Error       | 継続不能なエラー発生時のエラー内容 |
 *  | LogLevel::None        | 出力なし |
 *
 *  下位のログレベルを設定すると、上位のログレベルのログも出力されます。
 *
 * @attention
 *  Release ビルドではログレベルに関わらず、ログは一切出力されません。
 *
 * @note
 *  デフォルト値は LogLevel::Error です。
 */
void SetLogLevel(LogLevel level) NN_NOEXCEPT;

//! @}

}}
