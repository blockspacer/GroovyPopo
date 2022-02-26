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
 * @brief   自動スリープ無効化に関する API の宣言
 *
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace oe {

//--------------------------------------------------------------------------

//! @name   無操作状態の制御に関する API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   無操作状態時の自動スリープを無効化します。
 *
 * @details
 *  無操作状態時の自動スリープを無効化します。 @n
 *  @n
 *  本関数は、無操作状態時の画面焼け軽減機能の動作には影響しません。 @n
 *  @n
 *  本関数を呼び出された回数は内部でカウントされています。本関数を呼び出した回数と同じ回数だけ
 *  @ref nn::oe::EndAutoSleepDisabledSection() が呼び出されたとき、初めて自動スリープ無効化が解除されます。@n
 *  @n
 *  本機能の効果は、本関数を呼び出したアプリケーションにフォーカスがある間のみ有効です。
 *
 * @note
 *  本関数を製品で使用する場合は、事前に使用許可の申請が必要です。
 */
void BeginAutoSleepDisabledSection() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   無操作状態時の自動スリープの無効化を解除します。
 *
 * @details
 *  無操作状態時の自動スリープの無効化を解除します。 @n
 *  @n
 *  本関数を呼び出された回数は内部でカウントされています。 @ref nn::oe::BeginAutoSleepDisabledSection() を
 *  呼び出した回数と同じ回数だけ本関数が呼び出されたとき、初めて自動スリープ無効化が解除されます。 @n
 *  @n
 *  本関数により自動スリープ無効化を解除した時点で、自動スリープの本来の発動時間をすでに過ぎていた場合は、即座に自動スリープが発動します。 @n
 *
 * @note
 *  本関数を製品で使用する場合は、事前に使用許可の申請が必要です。
 */
void EndAutoSleepDisabledSection() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   無操作状態時の自動スリープが現在無効化されているかどうかを取得します。
 *
 * @return  自動スリープが現在無効化されているかどうかが返ります。
 *
 * @details
 *  無操作状態時の自動スリープが @ref nn::oe::BeginAutoSleepDisabledSection() の呼び出しによって無効化されているかどうかを取得します。
 */
bool IsAutoSleepDisabled() NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::oe

