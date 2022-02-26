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
*   @brief  単調増加クロックが指し示す値を表す構造体の型宣言
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
 * @brief   単調増加クロックが指し示す値を表す構造体です。
 *
 * @details
 *  nn::time::GetSpanBetween 関数によって2つの値のあいだの経過時間を計算することができます。
 *  @n
 *  アプリケーションが動作していないあいだの経過時間を計算したい場合、この構造体の内容をセーブデータに保存して次回の起動へ受け渡してください。
 */
struct SteadyClockTimePoint
{
    int64_t value;
    SourceId sourceId;

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
    SteadyClockTimePoint& operator+=(const nn::TimeSpan& timeSpan) NN_NOEXCEPT
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
    SteadyClockTimePoint& operator-=(const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        value -= timeSpan.GetSeconds();
        return *this;
    }

    /**
     * @brief   nn::TimeSpan との加算演算子のオーバーロードです。
     *
     * @param[in]   steadyClockTimePoint    起点となる時刻を指定します。
     * @param[in]   timeSpan                時刻の差分を指定します。
     *
     * @return  操作後の時刻を nn::time::SteadyClockTimePoint 型で返します。
     *
     * @details
     *  この構造体が指す時刻を timeSpan 分だけ進めた時刻を返します。@n
     *
     *  結果は秒単位に切り捨てられます。
     */
    friend SteadyClockTimePoint operator+(const SteadyClockTimePoint& steadyClockTimePoint, const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        SteadyClockTimePoint ret;
        ret.value = steadyClockTimePoint.value + timeSpan.GetSeconds();
        ret.sourceId = steadyClockTimePoint.sourceId;
        return ret;
    }

    /**
     * @brief   nn::TimeSpan との減算演算子のオーバーロードです。
     *
     * @param[in]   steadyClockTimePoint    起点となる時刻を指定します。
     * @param[in]   timeSpan                時刻の差分を指定します。
     *
     * @return  操作後の時刻を nn::time::SteadyClockTimePoint 型で返します。
     *
     * @details
     *  この構造体が指す時刻を timeSpan 分だけ戻した時刻を返します。@n
     *
     *  結果は秒単位に切り上げられます。
     */
    friend SteadyClockTimePoint operator-(const SteadyClockTimePoint& steadyClockTimePoint, const nn::TimeSpan& timeSpan) NN_NOEXCEPT
    {
        SteadyClockTimePoint ret;
        ret.value = steadyClockTimePoint.value - timeSpan.GetSeconds();
        ret.sourceId = steadyClockTimePoint.sourceId;
        return ret;
    }

//! @}

//! @name 比較演算子
//! @{

    /**
     * @brief   nn::time::SteadyClockTimePoint 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return
     *  両者が同じ時刻を指していれば true を、異なる時刻を指していれば false を返します。
     *  両者のソース ID が異なる場合は常に false を返します。
     *
     * @details
     */
    friend bool operator==(const SteadyClockTimePoint& lhs, const SteadyClockTimePoint& rhs) NN_NOEXCEPT
    {
        return lhs.value == rhs.value && lhs.sourceId == rhs.sourceId;
    }

    /**
     * @brief   nn::time::SteadyClockTimePoint 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する時刻の一方を指定します。
     * @param[in]   rhs     比較する時刻の他方を指定します。
     *
     * @return
     *  両者が異なる時刻を指していれば true を、同じ時刻を指していれば false を返します。
     *  両者のソース ID が異なる場合は常に true を返します。
     *
     * @details
     */
    friend bool operator!=(const SteadyClockTimePoint& lhs, const SteadyClockTimePoint& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

//! @}
};

}
}
