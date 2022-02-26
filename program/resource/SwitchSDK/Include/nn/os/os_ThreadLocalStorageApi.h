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
 * @brief  スレッドローカルストレージに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_Result.h>
#include <nn/os/os_ThreadLocalStorageCommon.h>

namespace nn { namespace os {

//! @name スレッド固有領域関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   空き TLS スロットを確保します。
 *
 * @param[out] outTlsSlot   獲得した TLS スロット番号の格納先
 * @param[in]  function     デストラクタ用コールバック関数エントリ
 *
 * @retresult
 *   @handleresult{nn::os::ResultOutOfResource}
 * @endretresult
 *
 * @pre
 *  - なし
 *
 * @post
 *  - TLS の空きスロットを確保し、そのスロット番号を outTlsSlot に格納する
 *  - 確保した TLS スロットの TLS 初期値を 0 にする（全スレッド）
 *  - function が非 NULL の場合、TLS デストラクタ関数を登録する
 *
 */
nn::Result  AllocateTlsSlot(TlsSlot* outTlsSlot, TlsDestructor function)    NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   TLS スロットを返却します。
 *
 * @param[in] tlsSlot   返却する TLS スロット番号
 *
 * @pre
 *  - 確保済みの tlsSlot を指定する
 *
 * @post
 *  - 指定された tlsSlot の TLS スロットを返却（未使用状態に）する
 *
 */
void    FreeTlsSlot(TlsSlot tlsSlot)    NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   指定された TLS スロットから値を取得します。
 *
 * @param[in] tlsSlot   TLS スロット番号
 *
 * @return  指定したスロットの TLS 値が返ります
 *
 * @pre
 *  - 確保済みの tlsSlot を指定する
 *
 * @post
 *  - 指定された tlsSlot の TLS スロットの TLS 値を返す
 *
 * @details
 *  指定された TLS スロットから値を取得します。
 *
 */
uintptr_t   GetTlsValue(TlsSlot tlsSlot)    NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   指定された TLS スロットに値を設定します。
 *
 * @param[in] tlsSlot   TLS スロット番号
 * @param[in] value     設定したい TLS 値
 *
 * @pre
 *  - 確保済みの tlsSlot を指定する
 *
 * @post
 *  - 指定された tlsSlot の TLS スロットに TLS 値を設定する
 *
 * @details
 *  指定された TLS スロットに値を設定します。
 *
 */
void    SetTlsValue(TlsSlot tlsSlot, uintptr_t value)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   確保済みの TLS スロット数を取得します。
 *
 * @return  確保済みの TLS スロット数です。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 現在確保済みの TLS スロット数を返す
 *
 */
int     GetUsedTlsSlotCount()   NN_NOEXCEPT;

//! @}

//--------------------------------------------------------------------------

}} // namespace nn::os

