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
 * @brief   無操作状態の制御に関する API の宣言
 *
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace oe {

//--------------------------------------------------------------------------

//! @name   無操作状態の制御に関する API
//! @{

/**
 * @brief   ユーザの操作があったことを明示的に宣言し、無操作状態の継続時間のカウントをリセットします。
 *
 * @details
 *  ユーザの操作があったことを明示的に宣言し、無操作状態の継続時間のカウントをリセットします。 @n
 *  @n
 *  本関数は、システム自身では操作として検出できない特殊な操作形態を利用するアプリケーションにおいて、アプリケーション独自の定義に基づいて
 *  ユーザが操作を行ったことを検出した場合に呼び出すことを想定しています。 @n
 *  @n
 *  アプリケーションは、明示的にユーザ操作があったことを検出した場合に限り本関数を呼び出すように注意してください。
 *
 * @note
 *  本関数を製品で使用する場合は、事前に使用許可の申請が必要です。
 */
void ReportUserIsActive() NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::oe

