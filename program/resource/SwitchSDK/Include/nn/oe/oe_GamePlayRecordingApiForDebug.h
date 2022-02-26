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
 * @brief   常時動画撮影機能に関する開発専用 API の宣言
 *
 * @details
 *  本ヘッダに含まれる API は製品機では使用できません。
 *
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace oe {

//! @name   アプリケーションの常時動画撮影機能に関する API
//! @{

//-----------------------------------------------------------------------------
/**
 * @brief   動画撮影されているアプリケーションの動画ファイルを保存するようにシステムに要求します。
 *
 * @details
 *  常時動画撮影されているアプリケーションの動画ファイルを
 *  保存するようにシステムに要求します。
 *
 *  本 API は、キャプチャボタン長押し相当の処理が内部的に行なわれるだけです。
 *  そのため、実際に動画ファイルの保存に成功するか否かや、
 *  動画ファイルの保存先は、常時動画撮影機能の仕様に従います。
 *
 *  なお、本 API は製品機では使用できません。
 *  開発時のデバッグ用途でのみ使用できます。
 *
 */
void RequestToSaveRecordingForDebug() NN_NOEXCEPT;


//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::oe

