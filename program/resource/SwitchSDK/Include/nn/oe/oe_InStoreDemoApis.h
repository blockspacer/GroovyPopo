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
 * @brief   試遊台環境で動作する体験版アプリケーション向けの API の宣言
 *
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace oe {

//! @name   試遊台環境で動作する体験版アプリケーション向けの API です。
//! @{

/**
 * @brief   本体が試遊台環境か否かを判定して返します。
 *
 * @return  本体が試遊台環境の場合は true を、そうでなければ false を返します。
 *
 * @details
 *  プログラムが動作している本体が試遊台環境か否かを判定します。
 *
 */
bool IsInStoreDemoModeEnabled() NN_NOEXCEPT;

/**
 * @brief   プログラムを終了し、試遊台メニューへ復帰します。
 *
 * @pre
 *  - nn::oe::IsInStoreDemoModeEnabled() == true
 *
 * @details
 *  自プログラムを終了し、試遊台メニューへ遷移します。
 *  本 API からはリターンされません。@n
 *  また、試遊台環境でない場合には内部的にアボートします。
 *
 *  本 API は nn::oe::EnterExitRequestHandlingSection() による終了要求の
 *  ハンドリング処理区間内であっても強制的に自プログラムを終了します。
 *
 */
NN_NORETURN void ExitApplicationAndGoBackToInStoreDemoMenu() NN_NOEXCEPT;

//! @}

}} // namespace nn::oe
