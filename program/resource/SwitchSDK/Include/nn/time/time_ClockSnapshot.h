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
*   @brief  時計の各種情報を保持するクラスの宣言
*/

#pragma once

#include <nn/nn_TimeSpan.h>
#include <nn/time/time_Common.h>
#include <nn/time/time_LocationName.h>
#include <nn/time/time_Types.h>
#include <nn/time/time_CalendarTime.h>
#include <nn/time/time_CalendarAdditionalInfo.h>
#include <nn/util/util_TypedStorage.h>

namespace nn
{
namespace time
{

/**
 * @brief 時計の各種情報を保持するクラスです。
 *
 *  このクラスは C++ 標準の TriviallyCopyable の要件を満たしており、 std::memcpy() が可能なクラスです。
 *
 *  アプリケーションが動作していない期間を含め、本クラスを扱いたい場合には、
 *  オブジェクトをセーブデータに保存して次回の起動へ受け渡してください。
 *
 *  メンバ関数はスレッドアンセーフです。
 *
 * @see nn::time::CalculateStandardUserSystemClockDifferenceByUser(), nn::time::CalculateSpanBetween()
 */
class ClockSnapshot
{
public:
//! @name コンストラクタ
//! @{

    /**
     * @brief   コンストラクタです。
     * @details
     *  オブジェクトが保持する情報は以下になります@n
     *  - nn::time::PosixTime, nn::time::CalendarTime, nn::time::CalendarAdditionalInfo は協定世界時 (UTC) の1970年1月1日午前0時0分0秒 を指す値
     *  - nn::time::LocationName は協定世界時 (UTC) を指す値
     *  - nn::time::SystemClockContext, nn::time::SteadyClockTimePoint は無効な値
     */
    ClockSnapshot() NN_NOEXCEPT;

    /**
     * @brief   システムの最新時刻でオブジェクトを生成します。
     * @param[out]  pOut    オブジェクトを格納するバッファへのポインタを指定します。
     * @pre
     *  - TIME ライブラリが初期化済である
     *  - pOut != nullptr
     * @details
     *  nn::time::StandardUserSystemClock, nn::time::StandardNetworkSystemClock から得られるシステムの最新時刻を使ってオブジェクトを生成します。
     *
     *  ネットワーク時計が補正されていない場合には、
     *  ネットワーク時計の時刻として協定世界時 (UTC) の1970年1月1日午前0時0分0秒を保持します。
     */
    static void CreateWithStandardSystemClock(ClockSnapshot* pOut) NN_NOEXCEPT;

    /**
     * @brief   システムの最新時刻に任意のタイミングで追従できる時計を使ってオブジェクトを生成します。
     * @param[out]  pOut    オブジェクトを格納するバッファへのポインタを指定します。
     * @pre
     *  - TIME ライブラリが初期化済である
     *  - pOut != nullptr
     * @details
     *  nn::time::AdjustableUserSystemClock, nn::time::AdjustableNetworkSystemClock から得られる時刻を使ってオブジェクトを生成します。
     *
     *  ネットワーク時計が補正されていない場合には、
     *  ネットワーク時計の時刻として協定世界時 (UTC) の1970年1月1日午前0時0分0秒を保持します。
     */
    static void CreateWithAdjustableSystemClock(ClockSnapshot* pOut) NN_NOEXCEPT;

//! @}

//! @name ユーザー時計の情報取得
//! @{

    /**
     * @brief   オブジェクトが保持するユーザー時計の POSIX 標準の時刻を取得します。
     * @return  ユーザー時計の POSIX 標準の時刻
     *
     * @details
     */
    nn::time::PosixTime GetStandardUserSystemClockPosixTime() const NN_NOEXCEPT;

    /**
     * @brief   オブジェクトが保持するユーザー時計のコンテキストを取得します。
     * @return  ユーザー時計のコンテキスト
     *
     * @details
     */
    nn::time::SystemClockContext GetStandardUserSystemClockContext() const NN_NOEXCEPT;

    /**
     * @brief   オブジェクトが保持するユーザー時計のカレンダー情報を取得します。
     * @return  ユーザー時計のカレンダー情報
     *
     * @details
     */
    nn::time::CalendarTime GetStandardUserSystemClockCalendarTime() const NN_NOEXCEPT;

    /**
     * @brief   オブジェクトが保持するユーザー時計のカレンダー追加情報を取得します。
     * @return  ユーザー時計のカレンダー追加情報
     *
     * @details
     */
    nn::time::CalendarAdditionalInfo GetStandardUserSystemClockCalendarAdditionalInfo() const NN_NOEXCEPT;

//! @}

//! @name ネットワーク時計の情報取得
//! @{

    /**
     * @brief   オブジェクトが保持するネットワーク時計の POSIX 標準の時刻を取得します。
     * @return  ネットワーク時計の POSIX 標準の時刻
     *
     * @details
     */
    nn::time::PosixTime GetStandardNetworkSystemClockPosixTime() const NN_NOEXCEPT;

    /**
     * @brief   オブジェクトが保持するネットワーク時計のコンテキストを取得します。
     * @return  ネットワーク時計のコンテキスト
     *
     * @details
     */
    nn::time::SystemClockContext GetStandardNetworkSystemClockContext() const NN_NOEXCEPT;

    /**
     * @brief   オブジェクトが保持するネットワーク時計のカレンダー情報を取得します。
     * @return  ネットワーク時計のカレンダー情報
     *
     * @details
     */
    nn::time::CalendarTime GetStandardNetworkSystemClockCalendarTime() const NN_NOEXCEPT;

    /**
     * @brief   オブジェクトが保持するネットワーク時計のカレンダー追加情報を取得します。
     * @return  ネットワーク時計のカレンダー追加情報
     *
     * @details
     */
    nn::time::CalendarAdditionalInfo GetStandardNetworkSystemClockCalendarAdditionalInfo() const NN_NOEXCEPT;

//! @}

//! @name 単調増加クロックの情報取得
//! @{

    /**
     * @brief   オブジェクトが保持する単調増加クロックの時点を取得します。
     * @return  単調増加クロックの時点
     *
     * @details
     */
    nn::time::SteadyClockTimePoint GetStandardSteadyClockTimePoint() const NN_NOEXCEPT;

//! @}

//! @name 本体設定の情報取得
//! @{

    /**
     * @brief   オブジェクトが保持する地域名を取得します。
     * @return  地域名
     *
     * @details
     */
    nn::time::LocationName GetLocationName() const NN_NOEXCEPT;

//! @}

    /**
     * @brief   nn::time::ClockSnapshot 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が同じなら true を、異なるなら false を返します。
     *
     * @details
     */
    friend bool operator == (const ClockSnapshot& lhs, const ClockSnapshot& rhs) NN_NOEXCEPT;

    /**
     * @brief   nn::time::ClockSnapshot 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が異なるなら true を、同じなら false を返します。
     *
     * @details
     */
    friend bool operator != (const ClockSnapshot& lhs, const ClockSnapshot& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

private:
    nn::util::TypedStorage<char, 208, 8u> m_Storage;
};

}
}
