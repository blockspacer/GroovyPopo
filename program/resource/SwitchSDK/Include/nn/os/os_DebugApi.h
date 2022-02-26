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
 * @brief   OS ライブラリのデバッグ機能に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_DebugTypes.h>
#include <nn/os/os_TickTypes.h>

namespace nn { namespace os {

//! @name OS ライブラリのデバッグ関連 API
//! @{

//-----------------------------------------------------------------------------
/**
 * @brief   現在のコンテキストで使用中のスタック情報を取得します
 *
 * @param[in] outStack      現在使用中のスタックの先頭アドレスの格納先
 * @param[in] outStackSize  現在使用中のスタックのサイズの格納先
 *
 * @details
 *  現在のコンテキストにおいて使用中のスタック領域の先頭アドレスとサイズを
 *  outStack および outStackSize に格納します。outStack または outStackSize に
 *  NULL を指定した場合、それらの情報は返されません。
 *
 *  「現在のコンテキスト」には以下のものが含まれます。
 *
 *  - スレッド
 *  - ファイバ
 *  - ユーザ例外ハンドラ
 *
 *  現在のスタックポインタが、上記いずれのスタック領域内にもない場合、
 *  すでにスタックオーバーフローが発生しているものと判断し、
 *  内部でアボートします。
 *
 */
void GetCurrentStackInfo(uintptr_t* outStack, size_t* outStackSize) NN_NOEXCEPT;


//-----------------------------------------------------------------------------
/**
 * @brief   自プログラムが現在使用しているメモリ使用量などの情報を取得します
 *
 * @param[in] info  情報を格納する構造体の先頭アドレス
 *
 * @details
 *  自プログラムが現在使用しているメモリ使用量などの情報を nn::os::MemoryInfo
 *  構造体に取得します。構造体の各メンバ変数は直接参照可能です。
 *  各メンバに格納される値の詳細はそちらのリファレンスを参照して下さい。
 *
 *  本 API が情報を取得している間に、メモリ使用状況が変化することがあるため、
 *  nn::os::MemoryInfo に格納される値は目安として参照して下さい。
 *  極力、他のスレッドが動作していない状況下で使用されることを推奨します。
 *
 *  スレッドスタックなど、1 つの実メモリ領域を
 *  複数の場所にマップするようなものは 1 つ分のサイズとしてカウントします。
 *
 *  本 API はスレッドセーフではありません。
 *
 */
void QueryMemoryInfo(MemoryInfo* info) NN_NOEXCEPT;


//-----------------------------------------------------------------------------
/**
 * @brief   自コアにおけるアイドル区間中の累積チックカウント値を取得します
 *
 * @return  自コアにおけるアイドル区間中の累積チックカウント値です。
 *
 * @details
 *  自スレッドが現在動作しているコア上で、過去のアイドル区間中の
 *  累積 Tick カウント値を取得します。アイドル区間とは、当該コア上で
 *  動作すべきスレッドがなかった遊休区間を指します。
 *
 *  特定の二点間で本 API を発行し、それぞれの Tick 値の差分を取ることで、
 *  当該区間におけるプロセッサの使用率を算出する目的に使用できます。
 *
 *  本 API で取得できるのは、自スレッドが動作しているコアに関する情報のみです。
 *  プログラムが使用可能なコア全てにおいて、アイドル区間中の
 *  累積 Tick カウント値を取得したい場合には、各コアで本 API を発行して下さい。
 *
 *  @platformbegin{Windows}
 *   - Windows 環境では本 API はサポートされていません。
 *     常に Tick(0) 相当のオブジェクトが返されます。
 *
 *  @platformend
 *
 */
Tick GetIdleTickCount() NN_NOEXCEPT;


//! @}

//-----------------------------------------------------------------------------

}} // namespace nn::os

