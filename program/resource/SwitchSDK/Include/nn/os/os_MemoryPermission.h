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
 * @brief   メモリアクセス権設定機能に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/os/os_Types.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//! @name メモリアクセス権関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   指定されたメモリ領域のメモリアクセス権を変更します。
 *
 * @param[in] address    設定したいメモリ領域の先頭アドレス
 * @param[in] size       設定したいメモリ領域のサイズ
 * @param[in] permission 設定したいメモリ領域のアクセス権
 *
 * @pre
 *  - address が nn::os::MemoryPageSize の整数倍である
 *  - size    が nn::os::MemoryPageSize の整数倍である
 *  - permission が以下のいずれかである
 *    - nn::os::MemoryPermission_None
 *    - nn::os::MemoryPermission_ReadOnly
 *    - nn::os::MemoryPermission_ReadWrite
 *
 * @post
 *  - 指定されたメモリ領域が permission のメモリアクセス権である
 *
 * @details
 *  指定されたメモリ領域を permission のメモリアクセス権に変更します。
 *
 */
void    SetMemoryPermission(uintptr_t address, size_t size, MemoryPermission permission)  NN_NOEXCEPT;

//! @}

}} // namespace nn::os
