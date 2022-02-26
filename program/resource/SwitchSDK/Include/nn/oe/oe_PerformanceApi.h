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
 * @brief アプリケーション動作環境を操作する機能の API 宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/oe/oe_Performance.h>

namespace nn { namespace oe {

//-----------------------------------------------------------------------------
//! @name アプリケーション動作環境の性能操作 API
//! @{

/**
 * @brief       全ての性能モードで使用される性能を変更します。
 *
 * @param[in]   config 設定する性能を示す @ref nn::oe::PerformanceConfiguration 型の値です。@n
 *              全ての性能モードで使用可能な性能のみ指定可能です。
 *
 * @post        性能が変更されます。@n
 *              また、全ての性能モードで指定された性能が使用されるようになります。@n
 *              本関数以前設定した性能はすべて上書きされます。
 *
 * @details     全ての性能モードで使用される性能を変更します。
 */
void SetPerformanceConfiguration(nn::oe::PerformanceConfiguration config) NN_NOEXCEPT;

/**
 * @brief       特定の性能モードで動作しているときに使用される性能を変更します。
 *
 * @param[in]   mode 性能モードを示す @ref nn::oe::PerformanceMode 型の列挙値です。
 * @param[in]   config 性能を示す @ref nn::oe::PerformanceConfiguration 型の値です。@n
 *              mode で指定した性能モードで使用可能な性能のみ指定可能です。
 *
 * @post        現在、指定した性能モードで動作している場合、性能が変更されます。@n
 *              指定した性能モードでない場合は、以降その性能モードに遷移したときに、指定した性能が使用されます。
 *
 * @details     特定の性能モードで動作しているときに使用される性能を変更します。
 */
void SetPerformanceConfiguration(nn::oe::PerformanceMode mode, nn::oe::PerformanceConfiguration config) NN_NOEXCEPT;

/**
 * @brief       特定の性能モードで動作しているときに使用される性能を取得します。
 *
 * @return      性能を示す @ref nn::oe::PerformanceConfiguration 型の値です。@n
 *              アプリケーションが引数で指定した性能モードに対して過去に　@ref nn::oe::SetPerformanceConfiguration を使用していない場合はデフォルトの性能を返します。
 *
 * @param[in]   mode 性能モードを示す @ref nn::oe::PerformanceMode 型の列挙値です。
 *
 * @details     特定の性能モードで動作しているときに使用される性能を取得します。
 */
PerformanceConfiguration GetPerformanceConfiguration(nn::oe::PerformanceMode mode) NN_NOEXCEPT;

/**
 * @brief       CPU オーバークロック機能が有効かを取得します。
 *
 * @return      CPU オーバークロック機能が有効か
 * @retval      true CPU オーバークロック機能が有効
 * @retval      false CPU オーバークロック機能が無効
 *
 * @supportversion NX Addon 7.0.0 以降
 * @supportfirmware 7.0.0-1.0 以降
 *
 * @details     CPU オーバークロック機能が有効かを取得します。@n
 *              CPU オーバークロック機能は開発用の機能です。@n
 *              製品機では本関数は必ず false を返します。
 */
bool IsCpuOverclockEnabled() NN_NOEXCEPT;

//! @}
//-----------------------------------------------------------------------------

}} // namespace nn::oe
