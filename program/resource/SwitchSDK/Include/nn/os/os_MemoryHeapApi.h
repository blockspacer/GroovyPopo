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
 * @brief   メモリヒープ機能に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/os/os_MemoryHeapCommon.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//! @name メモリ管理関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   メモリヒープ領域のサイズを変更します。
 *
 * @param[in] size  メモリヒープ領域のサイズ
 *
 * @retresult
 *   @handleresult{nn::os::ResultOutOfMemory}
 *   @handleresult{nn::os::ResultBusy}
 * @endretresult
 *
 * @pre
 *  - size >= 0
 *  - size が nn::os::MemoryHeapUnitSize の整数倍である
 *  - メモリヒープを誰も使用していない
 *
 * @post
 *  - メモリヒープ全体のサイズが size バイトである。
 *
 */
nn::Result  SetMemoryHeapSize(size_t size)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メモリヒープからメモリブロックを獲得します。
 *
 * @param[in] address   獲得したメモリブロックの先頭アドレスの格納先
 * @param[in] size      獲得するメモリブロックのサイズ
 *
 * @retresult
 *   @handleresult{nn::os::ResultOutOfMemory}
 * @endretresult
 *
 * @pre
 *  - size > 0
 *  - size が nn::os::MemoryBlockUnitSize の整数倍である
 *
 * @post
 *  - メモリブロック獲得に成功した場合
 *    - address に獲得したメモリブロックの先頭アドレスが入る
 *    - 獲得したメモリブロックが nn::os::MemoryPermission_ReadWrite 属性である
 *  - メモリブロック獲得に失敗した場合
 *    - メモリヒープ領域は変化しない
 *
 * @details
 *  メモリヒープ領域から size の大きさのメモリブロックを獲得します。@n
 *  メモリブロック獲得に成功すると、address にその先頭アドレスが格納されます。
 *
 *  メモリブロックは管理領域等のオーバーヘッドがないため、
 *  細かく確保を行っても合計してメモリヒープのサイズ分のメモリブロックを確保できることが保証されます。
 */
nn::Result  AllocateMemoryBlock(uintptr_t* address, size_t size) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メモリヒープ領域へメモリブロックを返却します。
 *
 * @param[in] address   返却するメモリブロックの先頭アドレス
 * @param[in] size      返却するメモリブロックのサイズ
 *
 * @pre
 *  - address が nn::os::MemoryBlockUnitSize の整数倍である
 *  - size > 0
 *  - size が nn::os::MemoryBlockUnitSize の整数倍である
 *  - 返却するメモリブロックが、メモリヒープ領域内である
 *
 * @post
 *  - メモリヒープにメモリブロックが返却される
 *
 * @details
 *  address～(address+size) のメモリブロックを、メモリヒープ領域に返却します。
 */
void    FreeMemoryBlock(uintptr_t address, size_t size) NN_NOEXCEPT;


//! @}

}} // namespace nn::os

