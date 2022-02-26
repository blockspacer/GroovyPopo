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
 * @brief   ディスプレイの画面解像度に関する API の宣言
 *
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os.h>

namespace nn { namespace oe {

//-----------------------------------------------------------------------------

//! @name   ディスプレイの画面解像度に関する API
//! @{

/**
* @brief   デフォルトディスプレイの画面解像度を取得します。
*
* @param[out]   pWidth   取得した画面解像度(横幅)の格納先
* @param[out]   pHeight  取得した画面解像度(縦幅)の格納先
*
* @details
*  デフォルトディスプレイの画面解像度をピクセル単位で取得します。
*
*  据置モードにおいて外部ディスプレイが認識されていない状態では、最後に認識したデフォルトディスプレイの画面解像度を返します。
*  外部ディスプレイが認識され、画面解像度が変化した時点で nn::oe::GetDefaultDisplayResolutionChangeEvent() で取得されるイベントが
*  シグナルされるので、再度 GetDefaultDisplayResolution() を呼び出して最新の画面解像度を取得してください。
*
*  ディスプレイの画面解像度に応じて処理を最適化したい場合に限り、本 API を使用してください。
*  画面解像度はプラットフォームや接続される外部ディスプレイによって多様であり、動的に変化することに注意して十分に検証を行ってください。
*  また、画面解像度の変更によってメモリ使用量や GPU 処理時間、アスペクト比が変化しますので、それらの変化への対応も必要です。
*
*  ディスプレイの画面解像度に応じた最適化が不要な場合は、本 API を使用する必要はありません。
*  システムが自動的にスケーリングを行います。
*
*  システムに与える負荷が大きいため、本 API の頻繁な呼び出しは推奨しません。
*
*/
void GetDefaultDisplayResolution(int* pWidth, int* pHeight) NN_NOEXCEPT;

/**
* @brief   デフォルトディスプレイの画面解像度が変更されたときのシステムイベントを取得します。
*
* @param[out]   pEvent   取得したシステムイベントの格納先
*
* @details
*  デフォルトディスプレイの画面解像度が変更されたときにシグナルされるシステムイベントを取得します。
*  こちらのシステムイベントは自動クリアするように設定されています。
*
*/
void GetDefaultDisplayResolutionChangeEvent(os::SystemEvent* pEvent) NN_NOEXCEPT;


//! @}

}} // namespace nn::oe

