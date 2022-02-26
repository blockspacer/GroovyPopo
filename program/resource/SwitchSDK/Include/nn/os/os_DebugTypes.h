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
 * @brief   OS ライブラリのデバッグ機能に関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   メモリ情報などを取得した結果を格納する構造体です。
 *
 * @details
 *  この構造体は nn::os::QueryMemoryInfo() で使用します。
 *
 *  各メンバ変数に格納される値の詳細は以下の通りです。
 *
 *  |メンバ変数名|説明|
 *  |:----:|:----|
 *  |totalAvailableMemorySize|自プログラムで使用可能なメモリの総量です。|
 *  |totalUsedMemorySize|自プログラムで使用中のメモリの総量です。@n totalMemoryHeapSize と programSize および仮想アドレスメモリ管理 API によってアドレス領域に割り当てられている物理メモリの合計サイズを含みます。|
 *  |totalMemoryHeapSize|メモリヒープの全体サイズです。@n nn::os::SetMemoryHeapSize() で確保したサイズと同じです。|
 *  |allocatedMemoryHeapSize|メモリヒープから割当て済みのサイズです。@n 成功した nn::os::AllocateMemoryBlock() の累算と同じです。@n nn::os::FreeMemoryBlock() で返却した分は含まれません。|
 *  |programSize|プログラムのコードやデータで使用しているサイズの総量です。|
 *  |totalThreadStackSize|nn::os::CreateThread() したスレッドのスタックサイズの累算値です。@n nn::os::DestroyThread() したスレッドのスタックサイズは含まれません。@n メインスレッドのスタックサイズも含みます。|
 *  |threadCount|現在使用中のスレッド数です。メインスレッドを含みます。|
 *
 * @platformbegin{Windows}
 *  - totalAvailableMemorySize には利用可能な物理メモリ量が返ります。
 *  - programSize には必ず 0 が代入され、このメンバ変数は意味を持ちません。
 *
 * @platformend
 *
 */
struct MemoryInfo
{
    uint64_t totalAvailableMemorySize;  //!< 使用可能なメモリ総量
    size_t   totalUsedMemorySize;       //!< 使用中のメモリ総量
    size_t   totalMemoryHeapSize;       //!< メモリヒープの全体サイズ
    size_t   allocatedMemoryHeapSize;   //!< メモリヒープの割当て済みサイズ
    size_t   programSize;               //!< プログラムのコードやデータのサイズ

    size_t   totalThreadStackSize;      //!< 全スレッドの総スタックサイズ
    int      threadCount;               //!< 現在使用中のスレッド数
};

//--------------------------------------------------------------------------

}} // namespace nn::os

