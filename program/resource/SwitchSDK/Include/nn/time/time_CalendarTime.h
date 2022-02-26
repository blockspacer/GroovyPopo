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
 * @brief  カレンダー情報を扱う構造体の型宣言
 */

#pragma once

#include <nn/time/time_Common.h>
#include <nn/nn_TimeSpan.h>

namespace nn
{
namespace time
{

/**
 * @brief    カレンダー情報を扱う構造体です。
 *
 * @details
 *  西暦、月、日、時、分、秒からなる日時を扱う構造体です。
 *
 *  nn::TimeSpan との加減算が可能ですが、結果は秒単位に切り捨てられます。
 */
struct CalendarTime
{
    int16_t year;  //!< 西暦
    int8_t month;  //!< 月（1から12の範囲の値をとります）
    int8_t day;    //!< 日（1から31の範囲の値をとります）
    int8_t hour;   //!< 時（0から23の範囲の値をとります）
    int8_t minute; //!< 分（0から59の範囲の値をとります）
    int8_t second; //!< 秒（0から59の範囲の値をとります）

    /**
     * @brief   有効なカレンダー情報を保持しているか判定します。
     *
     * @return  有効かどうか
     * @retval  true    有効
     * @retval  false   無効
     *
     * @details
     *  year が 0 以下の場合は必ず false が返ります。
     *
     *  以下をすべて満たす場合に true を返します。@n
     *  - year, month, day が有効な日付である( nn::time::IsValidDate() と同等の判定 )
     *  - 0 <= hour <= 23
     *  - 0 <= minute <= 59
     *  - 0 <= second <= 59
     */
    bool IsValid() const NN_NOEXCEPT;

//! @name nn::TimeSpan との加減算
//! @{

    /**
     * @brief   nn::TimeSpan との加算代入演算子のオーバーロードです。
     *
     * @param[in]   timeSpan    日時の差分を指定します。
     *
     * @return  操作後の自身への参照を返します。
     *
     * @pre
     *  - IsValid() == true
     *
     * @details
     *  この構造体が指す日時を timeSpan 分だけ進めます。@n
     *  夏時間の影響などによる時差の切り替わりをまったく考慮せず計算を行います。
     *  うるう年は考慮されます。
     *
     *  timeSpan のうち秒未満の値は捨てられます。
     */
    CalendarTime& operator+=(const nn::TimeSpan& timeSpan) NN_NOEXCEPT;

    /**
     * @brief   nn::TimeSpan との減算代入演算子のオーバーロードです。
     *
     * @param[in]   timeSpan    日時の差分を指定します。
     *
     * @return  操作後の自身への参照を返します。
     *
     * @pre
     *  - IsValid() == true
     *
     * @details
     *  この構造体が指す日時を timeSpan 分だけ戻します。@n
     *  夏時間の影響などによる時差の切り替わりをまったく考慮せず計算を行います。
     *  うるう年は考慮されます。
     *
     *  timeSpan のうち秒未満の値は捨てられます。
     */
    CalendarTime& operator-=(const nn::TimeSpan& timeSpan) NN_NOEXCEPT;

    /**
     * @brief   nn::TimeSpan との加算演算子のオーバーロードです。
     *
     * @param[in]   calendarTime   起点となる日時を指定します。
     * @param[in]   timeSpan    日時の差分を指定します。
     *
     * @return  操作後の日時を nn::time::CalendarTime 型で返します。
     *
     * @pre
     *  - calendarTime.IsValid() == true
     *
     * @details
     *  この構造体が指す日時を timeSpan 分だけ進めた日時を返します。@n
     *  夏時間の影響などによる時差の切り替わりをまったく考慮せず計算を行います。
     *  うるう年は考慮されます。
     *
     *  timeSpan のうち秒未満の値は捨てられます。
     */
    friend CalendarTime operator+(const CalendarTime& calendarTime, const nn::TimeSpan& timeSpan) NN_NOEXCEPT;

    /**
     * @brief   nn::TimeSpan との減算演算子のオーバーロードです。
     *
     * @param[in]   calendarTime   起点となる日時を指定します。
     * @param[in]   timeSpan    日時の差分を指定します。
     *
     * @return  操作後の日時を nn::time::CalendarTime 型で返します。
     *
     * @pre
     *  - calendarTime.IsValid() == true
     *
     * @details
     *  この構造体が指す日時を timeSpan 分だけ戻した日時を返します。@n
     *  夏時間の影響などによる時差の切り替わりをまったく考慮せず計算を行います。
     *  うるう年は考慮されます。
     *
     *  timeSpan のうち秒未満の値は捨てられます。
     */
    friend CalendarTime operator-(const CalendarTime& calendarTime, const nn::TimeSpan& timeSpan) NN_NOEXCEPT;

    /**
     * @brief   nn::time::CalendarTime 同士の減算演算子のオーバーロードです。
     *
     * @param[in]   lhs     起点となる日時を指定します。
     * @param[in]   rhs     終点となる日時を指定します。
     *
     * @return  日時間の差分を nn::TimeSpan 型で返します。
     *
     * @pre
     *  - lhs.IsValid() == true
     *  - rhs.IsValid() == true
     *
     * @details
     *  夏時間の影響などによる時差の切り替わりをまったく考慮せず計算を行います。
     *  うるう年は考慮されます。
     *
     *  9223372036 秒（約 292 年）以上の差分がある CalendarTime 同士を減算すると nn::TimeSpan の内部パラメータがオーバーフローし、正しい結果が得られないことに注意してください。
     */
    friend nn::TimeSpan operator-(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT;

//! @}

//! @name 比較演算子
//! @{

    /**
     * @brief   nn::time::CalendarTime 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が同じなら true を、異なるなら false を返します。
     *
     * @details
     */
    friend bool operator==(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT
    {
        return  lhs.year    == rhs.year &&
                lhs.month   == rhs.month &&
                lhs.day     == rhs.day &&
                lhs.hour    == rhs.hour &&
                lhs.minute  == rhs.minute &&
                lhs.second  == rhs.second;
    }

    /**
     * @brief   nn::time::CalendarTime 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が異なるなら true を、同じなら false を返します。
     *
     * @details
     */
    friend bool operator!=(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

    /**
     * @brief   nn::time::CalendarTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する日時の一方を指定します。
     * @param[in]   rhs     比較する日時の他方を指定します。
     *
     * @return  lhs が rhs と同じ日時か過去であれば true を、そうでなければ false を返します。
     *
     * @pre
     *  - lhs.IsValid() == true
     *  - rhs.IsValid() == true
     *
     * @details
     */
    friend bool operator<=(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT;

    /**
     * @brief   nn::time::CalendarTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する日時の一方を指定します。
     * @param[in]   rhs     比較する日時の他方を指定します。
     *
     * @return  lhs が rhs と同じ日時か未来であれば true を、そうでなければ false を返します。
     *
     * @pre
     *  - lhs.IsValid() == true
     *  - rhs.IsValid() == true
     *
     * @details
     */
    friend bool operator>=(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT;

    /**
     * @brief   nn::time::CalendarTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する日時の一方を指定します。
     * @param[in]   rhs     比較する日時の他方を指定します。
     *
     * @return  lhs が rhs より過去であれば true を、そうでなければ false を返します。
     *
     * @pre
     *  - lhs.IsValid() == true
     *  - rhs.IsValid() == true
     *
     * @details
     */
    friend bool operator<(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT;

    /**
     * @brief   nn::time::CalendarTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する日時の一方を指定します。
     * @param[in]   rhs     比較する日時の他方を指定します。
     *
     * @return  lhs が rhs より未来であれば true を、そうでなければ false を返します。
     *
     * @pre
     *  - lhs.IsValid() == true
     *  - rhs.IsValid() == true
     *
     * @details
     */
    friend bool operator>(const CalendarTime& lhs, const CalendarTime& rhs) NN_NOEXCEPT;

//! @}
};

}
}
