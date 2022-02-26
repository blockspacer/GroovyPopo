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
*   @file
*   @brief  日時を返すクロックのコンテキストを表す構造体の型宣言
*/

#pragma once

#include <nn/time/time_Common.h>
#include <nn/time/time_Typedefs.h>
#include <nn/time/time_SteadyClockTimePoint.h>

namespace nn
{
namespace time
{

/**
 * @brief   日時を返すクロックのコンテキストを表す構造体です。
 *
 * @details
 *  コンテキストはクロックが補正や操作、リセットされた際に更新されます。
 *  このため、補正や操作などの有無をコンテキストの比較によって判断することができます。@n
 *
 *  アプリケーションが動作していないあいだの補正や操作を検出したい場合、この構造体の内容をセーブデータに保存して次回の起動へ受け渡してください。
 */
struct SystemClockContext
{
    int64_t offset;
    SteadyClockTimePoint timeStamp;

//! @name 比較演算子
//! @{

    /**
     * @brief   nn::time::SystemClockContext 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較するコンテキストの一方を指定します。
     * @param[in]   rhs     比較するコンテキストの他方を指定します。
     *
     * @return  両者のコンテキストが同じであれば true を、異なれば false を返します。
     *
     * @details
     */
    friend bool operator==(const SystemClockContext& lhs, const SystemClockContext& rhs) NN_NOEXCEPT
    {
        return lhs.offset == rhs.offset && lhs.timeStamp == rhs.timeStamp;
    }

    /**
     * @brief   nn::time::SystemClockContext 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較するコンテキストの一方を指定します。
     * @param[in]   rhs     比較するコンテキストの他方を指定します。
     *
     * @return  両者のコンテキストが異なれば true を、同じであれば false を返します。
     *
     * @details
     */
    friend bool operator!=(const SystemClockContext& lhs, const SystemClockContext& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

//! @}
};

}
}
