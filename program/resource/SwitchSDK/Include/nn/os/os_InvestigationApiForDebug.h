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
 * @brief  OS ライブラリのデバッグ機能に関する API の宣言（製品機では使用不可）
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_DebugTypes.h>
#include <nn/os/os_TickTypes.h>

namespace nn { namespace os {

//! @name OS ライブラリのデバッグ関連 API（製品機では使用不可）
//! @{

//-----------------------------------------------------------------------------
/**
 * @brief   指定されたメモリ領域がロックされているかどうかを検査します。
 *
 * @param[in] address   検査対象のメモリ領域の先頭アドレス
 * @param[in] size      検査対象のメモリ領域のサイズ
 *
 * @pre
 *  - size > 0
 *
 * @return  指定されたメモリ領域がロックされていれば true を、そうでなければ false を返します。
 *
 * @details
 *  特定のメモリ領域が他の用途で既に使用中の場合、間違った重複利用を
 *  避けるために、当該メモリ領域はシステムによってロックされます。
 *
 *  本 API は指定されたメモリ領域が、上記のような
 *  ロックされた状態にあるか否かを検査し、その結果を bool 値で返します。
 *
 *  なお、本 API は製品機では使用できません。
 *
 *  @platformbegin{Windows}
 *   - Windows 環境では本 API はサポートされていません。
 *     本 API を発行すると内部でアボートします。
 *
 *  @platformend
 */
bool IsMemoryLocked(void* address, size_t size) NN_NOEXCEPT;

//-----------------------------------------------------------------------------
/**
 * @brief   指定されたメモリ領域がロックされているかどうかを検査します。
 *
 * @param[in] address   検査対象のメモリ領域の先頭アドレス
 * @param[in] size      検査対象のメモリ領域のサイズ
 *
 * @pre
 *  - size > 0
 *
 * @return  指定されたメモリ領域がロックされていれば true を、そうでなければ false を返します。
 *
 * @details
 *  @ref nn::os::IsMemoryLocked(void*, size_t) と同機能です。
 *
 *  @platformbegin{Windows}
 *   - Windows 環境では本 API はサポートされていません。
 *     本 API を発行すると内部でアボートします。
 *
 *  @platformend
 */
bool IsMemoryLocked(uintptr_t address, size_t size) NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::os

