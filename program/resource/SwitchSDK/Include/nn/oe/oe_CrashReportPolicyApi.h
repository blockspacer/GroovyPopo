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
 * @brief   クラッシュレポート取得の制御に関する API の宣言
 *
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace oe {

//--------------------------------------------------------------------------

//! @name   クラッシュレポート取得の制御に関する API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   例外発生時のクラッシュレポート作成を有効化します。
 *
 * @details
 *  アプリケーション実行中に例外が発生した場合のクラッシュレポートの作成を有効化します。
 *
 *  .nmeta で &lt;CrashReport&gt;Allow&lt;/CrashReport&gt; が設定されている場合、
 *  プログラムが起動した時点でクラッシュレポートの作成は有効です。
 *  本 API は DisableCrashReport() で作成を無効化した後、再び有効化する際に呼び出してください。
 *
 *  .nemta で &lt;CrashReport&gt;Allow&lt;/CrashReport&gt; が設定されていない場合、本 API は何も行いません。
 *
 *  詳細は @confluencelink{267167329, 「クラッシュレポートに関する注意事項」} を参照してください。
 *
 */
void EnableCrashReport() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   例外発生時のクラッシュレポート作成を無効化します。
 *
 * @details
 *  アプリケーション実行中に例外が発生した場合のクラッシュレポートの作成を無効化します。
 *
 *  .nmeta で &lt;CrashReport&gt;Allow&lt;/CrashReport&gt; が設定されている場合、
 *  プログラムが起動した時点でクラッシュレポートの作成は有効です。
 *  クラッシュレポートの作成を無効化する際に本 API を呼び出してください。
 *  その後、再び作成を有効化するには EnableCrashReport() を呼び出してください。
 *
 *  .nemta で &lt;CrashReport&gt;Allow&lt;/CrashReport&gt; が設定されていない場合、本 API は何も行いません。
 *
 *  詳細は @confluencelink{267167329, 「クラッシュレポートに関する注意事項」} を参照してください。
 *
 */
void DisableCrashReport() NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::oe
