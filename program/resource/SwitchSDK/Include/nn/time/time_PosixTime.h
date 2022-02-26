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
*   @brief  POSIX 標準の時刻を表す構造体の型宣言
*/

#pragma once

#include <nn/time/time_Common.h>

#include <nn/time/time_Typedefs.h>
#include <nn/nn_TimeSpan.h>


namespace nn
{
namespace time
{

/**
 * @brief   POSIX 標準の時刻を表す構造体です。
 *
 * @details
 *  協定世界時 (UTC) の1970年1月1日午前0時0分0秒からの経過秒数で時刻を保持します。@n
 *
 *  nn::TimeSpan との加減算が可能ですが、結果は秒単位に切り捨てられます。
 */
struct PosixTime
{
    int64_t value;

//! @name nn::TimeSpan との加減算
//! @{

    /**
     * @brief   nn::TimeSpan との加算代入演算子のオーバーロードです。
     *
     * @param[in]   timeSpan    時刻の差分を指定します。
     *
     * @return  操作後の自身への参照を返します。
     *
     * @details
     *  この構造体が指す時刻を timeSpan 分だけ進めます。@n
     *
     *  結果は秒単位に切り捨てられます。
     */
    PosixTime& operator+=(const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        value += timeSpan.GetSeconds();
        return *this;
    }

    /**
     * @brief   nn::TimeSpan との減算代入演算子のオーバーロードです。
     *
     * @param[in]   timeSpan    時刻の差分を指定します。
     *
     * @return  操作後の自身への参照を返します。
     *
     * @details
     *  この構造体が指す時刻を timeSpan 分だけ戻します。@n
     *
     *  結果は秒単位に切り上げられます。
     */
    PosixTime& operator-=(const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        value -= timeSpan.GetSeconds();
        return *this;
    }

    /**
     * @brief   nn::TimeSpan との加算演算子のオーバーロードです。
     *
     * @param[in]   posixTime   起点となる時刻を指定します。
     * @param[in]   timeSpan    時刻の差分を指定します。
     *
     * @return  操作後の時刻を nn::time::PosixTime 型で返します。
     *
     * @details
     *  この構造体が指す時刻を timeSpan 分だけ進めた時刻を返します。@n
     *
     *  結果は秒単位に切り捨てられます。
     */
    friend PosixTime operator+(const PosixTime& posixTime, const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        PosixTime ret;
        ret.value = posixTime.value + timeSpan.GetSeconds();
        return ret;
    }

    /**
     * @brief   nn::TimeSpan との減算演算子のオーバーロードです。
     *
     * @param[in]   posixTime   起点となる時刻を指定します。
     * @param[in]   timeSpan    時刻の差分を指定します。
     *
     * @return  操作後の時刻を nn::time::PosixTime 型で返します。
     *
     * @details
     *  この構造体が指す時刻を timeSpan 分だけ戻した時刻を返します。@n
     *
     *  結果は秒単位に切り上げられます。
     */
    friend PosixTime operator-(const PosixTime& posixTime, const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        PosixTime ret;
        ret.value = posixTime.value - timeSpan.GetSeconds();
        return ret;
    }

    /**
     * @brief   nn::time::PosixTime 同士の減算演算子のオーバーロードです。
     *
     * @param[in]   lhs     起点となる時刻を指定します。
     * @param[in]   rhs     終点となる時刻を指定します。
     *
     * @return  時刻間の差分を nn::TimeSpan 型で返します。
     *
     * @details
     *  9223372036 秒（約 292 年）以上の差分がある PosixTime 同士を減算すると nn::TimeSpan の内部パラメータがオーバーフローし、正しい結果が得られないことに注意してください。
     */
    friend nn::TimeSpan operator-(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return nn::TimeSpan::FromSeconds(lhs.value - rhs.value);
    }

//! @}

//! @name 比較演算子
//! @{

    /**
     * @brief   nn::time::PosixTime 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return  両者が同じ時刻を指していれば true を、異なる時刻を指していれば false を返します。
     *
     * @details
     */
    friend bool operator==(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return lhs.value == rhs.value;
    }

    /**
     * @brief   nn::time::PosixTime 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return  両者が異なる時刻を指していれば true を、同じ時刻を指していれば false を返します。
     *
     * @details
     */
    friend bool operator!=(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

    /**
     * @brief   nn::time::PosixTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return  lhs が rhs と同時刻か過去であれば true を、そうでなければ false を返します。
     *
     * @details
     */
    friend bool operator<=(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return lhs.value <= rhs.value;
    }

    /**
     * @brief   nn::time::PosixTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return  lhs が rhs と同時刻か未来であれば true を、そうでなければ false を返します。
     *
     * @details
     */
    friend bool operator>=(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return lhs.value >= rhs.value;
    }

    /**
     * @brief   nn::time::PosixTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return  lhs が rhs より過去であれば true を、そうでなければ false を返します。
     *
     * @details
     */
    friend bool operator<(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return lhs.value < rhs.value;
    }

    /**
     * @brief   nn::time::PosixTime 同士の比較演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return  lhs が rhs より未来であれば true を、そうでなければ false を返します。
     *
     * @details
     */
    friend bool operator>(const PosixTime& lhs, const PosixTime& rhs) NN_NOEXCEPT
    {
        return lhs.value > rhs.value;
    }

//! @}
};

}
}
