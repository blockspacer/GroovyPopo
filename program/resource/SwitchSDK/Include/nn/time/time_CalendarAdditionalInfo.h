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
*   @brief  カレンダーの追加情報を扱う構造体の型宣言
*/

#pragma once

#include <nn/time/time_Common.h>
#include <nn/time/time_DayOfWeek.h>
#include <nn/time/time_TimeZone.h>

namespace nn
{
namespace time
{

/**
 * @brief    カレンダーの追加情報を扱う構造体です。
 *
 * @details
 */
struct CalendarAdditionalInfo
{
    DayOfWeek dayOfWeek; //!< 現地時刻での曜日を取得します。
    int32_t yearDay; //!< 現地時刻で、その年で何日目かを表す年通算日(0-365の範囲)。1月1日だと0になります。
    TimeZone timeZone; //!< 現地時刻に適用されている標準時の情報です。

    /**
     * @brief       nn::time::CalendarAdditionalInfo 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return      両者が同じなら true を、異なるなら false を返します。
     *
     * @details
     */
    friend bool operator == (const CalendarAdditionalInfo& lhs, const CalendarAdditionalInfo& rhs) NN_NOEXCEPT
    {
        return
            lhs.dayOfWeek == rhs.dayOfWeek &&
            lhs.yearDay == rhs.yearDay &&
            lhs.timeZone == rhs.timeZone;
    }
    /**
     * @brief       nn::time::CalendarAdditionalInfo 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return      両者が異なるなら true を、同じなら false を返します。
     *
     * @details
     */
    friend bool operator != (const CalendarAdditionalInfo& lhs, const CalendarAdditionalInfo& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }


};

}
}
