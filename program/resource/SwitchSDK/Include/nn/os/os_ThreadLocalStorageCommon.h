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
 * @brief  スレッドローカルストレージの共通定義
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   TLS のスロット番号を表す型です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AllocateTlsSlot()
 * - @ref FreeTlsSlot()
 * - @ref GetTlsValue()
 * - @ref SetTlsValue()
 * - @ref GetUsedTlsSlotCount()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 *
 * ThreadLocalStorage ユーティリティクラスもあわせてご確認ください。
 */
struct TlsSlot
{
    uint32_t    _innerValue;
};

//--------------------------------------------------------------------------
/**
 * @brief   TLS のデストラクタ関数を表す型です。
 */
typedef void    (*TlsDestructor)(uintptr_t value);


//--------------------------------------------------------------------------
/**
 * @brief   TLS スロットの総数を示す定数です。
 */
const int   TlsSlotCountMax = 16;

//--------------------------------------------------------------------------
/**
 * @brief   SDK 内部用の TLS スロットの総数を示す定数です。
 */
const int   SdkTlsSlotCountMax = 16;

//--------------------------------------------------------------------------

}} // namespace nn::os

