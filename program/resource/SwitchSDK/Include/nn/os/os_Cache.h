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
 * @brief   キャッシュ操作に関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>

namespace nn { namespace os {

//! @name キャッシュ操作関連 API
//! @{

/**
 * @brief   データキャッシュをメモリに書き出したのち破棄します。
 *
 * @param[in] addr 開始アドレス
 * @param[in] size サイズ
 *
 * @pre
 *  - 指定されたメモリ領域がキャッシュ対象である
 *  - 指定されたメモリ領域が読み込みまたは書き込み可能である
 *
 * @details
 *  指定されたメモリ領域のデータキャッシュをメモリに書き出したのち破棄します。
 */
void FlushDataCache( const void* addr, size_t size ) NN_NOEXCEPT;

//! @}

}}  // namespace nn::os
