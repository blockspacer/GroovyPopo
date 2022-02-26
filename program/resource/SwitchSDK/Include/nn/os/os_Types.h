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
 * @brief   OS ライブラリで使用する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   メモリアクセス権を表す列挙型です。
 */
enum MemoryPermission
{
    MemoryPermission_None       = 0x0,  //!< メモリはアクセス禁止です。
    MemoryPermission_ReadOnly   = 0x1,  //!< メモリは読み込み専用です。
    MemoryPermission_WriteOnly  = 0x2,  //!< メモリは書き込み専用です。
    MemoryPermission_ReadWrite  = 0x3,  //!< メモリは読み書き可能です。
};


//--------------------------------------------------------------------------

typedef nn::Bit64 ThreadId;

struct ProcessId
{
    Bit64   value;

    static ProcessId GetInvalidId() NN_NOEXCEPT
    {
        ProcessId id = { 0 };
        return id;
    }
};

inline bool operator == (const ProcessId& lhs, const ProcessId& rhs) NN_NOEXCEPT
{
    return lhs.value == rhs.value;
}
inline bool operator != (const ProcessId& lhs, const ProcessId& rhs) NN_NOEXCEPT
{
    return ! (lhs == rhs);
}


//--------------------------------------------------------------------------

}} // namespace nn::os

