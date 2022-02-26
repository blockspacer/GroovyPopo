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
*   @brief  標準時を表す構造体宣言
*/

#pragma once

#include <nn/time/time_Common.h>
#include <nn/util/util_StringUtil.h>

namespace nn
{
namespace time
{

/**
 * @brief ある地域の特定時期に適用されるタイムゾーンを表す構造体です。
 *
 * @details
 *  @ref ToCalendarTime 関数で得られる @ref CalendarAdditionalInfo から取得できます。
 */
struct TimeZone
{
    static const size_t StandardTimeNameSize = 8; //!< 標準時の名称のサイズ。

    char standardTimeName[StandardTimeNameSize]; //!< 適用される標準時の名称を表す文字列 ("GMT", "BST", "JST" など) 。
    bool isDaylightSavingTime; //!< 夏時間の有効性 ( true だと夏時間の期間、 false だと夏時間でない ) 。
    int32_t utcOffsetSeconds; //!< 現地時刻と協定世界時 (UTC) との時差 (秒単位) 。正の値だと UTC より時間が進んでいることを示します。

    /**
     * @brief   nn::time::TimeZone 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が同じなら true を、異なるなら false を返します。
     *
     * @details
    */
    friend bool operator == (const TimeZone& lhs, const TimeZone& rhs) NN_NOEXCEPT
    {
        return
            nn::util::Strncmp(lhs.standardTimeName, rhs.standardTimeName, StandardTimeNameSize) == 0 &&
            lhs.isDaylightSavingTime == rhs.isDaylightSavingTime &&
            lhs.utcOffsetSeconds == rhs.utcOffsetSeconds;
    }

    /**
     * @brief   nn::time::TimeZone 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が異なるなら true を、同じなら false を返します。
     *
     * @details
     */
    friend bool operator != (const TimeZone& lhs, const TimeZone& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

};

}
}
