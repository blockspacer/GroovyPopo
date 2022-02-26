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
 * @brief   クラッシュレポートに含めるユーザー定義のデータに関する API の宣言
 *
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------

//! @name   クラッシュレポートに含めるユーザー定義のデータに関する API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   クラッシュレポートに含めるユーザー定義のデータ（ダイイング・メッセージ）を格納するメモリ領域を指定します。
 *
 * @param[in]   address   ダイイング・メッセージ領域の先頭アドレス
 * @param[in]   size      ダイイング・メッセージ領域のサイズ
 *
 * @pre
 *  - address が nn::os::MemoryPageSize の整数倍である
 *  - size <= 4096
 *
 * @details
 *  本 API で指定したメモリ領域の内容がクラッシュレポートに格納されサーバーに送信されます。
 *
 *  指定できるメモリ領域のサイズは 4KB 以下です。
 *
 *  メモリ領域の内容を書き換える度に本 API を呼び出す必要はありません。
 *  アプリケーション起動時に一度だけ呼び出せば十分です。
 *
 *  本 API で指定するメモリ領域に個人情報が含まれないようにしてください。
 *
 */
void SetDyingMessageRegion(uintptr_t address, size_t size) NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::os
