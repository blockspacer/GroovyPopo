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
*   @brief  タイムゾーン関連の API の宣言
*/

#pragma once

#include <nn/time/time_CalendarAdditionalInfo.h>
#include <nn/time/time_CalendarTime.h>
#include <nn/time/time_DayOfWeek.h>
#include <nn/time/time_LocationName.h>
#include <nn/time/time_TimeZoneRule.h>
#include <nn/time/time_PosixTime.h>

namespace nn { namespace time {

/**
 * @brief @ref ToCalendarTime 関数で動作保証している入力の最小値です。
 *
 * @details
 *  協定世界時 (UTC) の1999年12月31日午前0時0分0秒を示す絶対時刻です。
 *
 *  @link about_time_zone_api タイムゾーンを扱う API について @endlink も併せて参照してください。
 */
const PosixTime InputPosixTimeMin = { 946598400 };

/**
 * @brief @ref ToCalendarTime 関数で動作保証している入力の最大値です。
 *
 * @details
 *  協定世界時 (UTC) の2100年1月1日午前0時0分0秒を示す絶対時刻です。
 *
 *  @link about_time_zone_api タイムゾーンを扱う API について @endlink も併せて参照してください。
 */
const PosixTime InputPosixTimeMax = { 4102444800 };


//! @name タイムゾーンを扱う API
//! @{

/**
 * @brief   デバイスに設定されている地域名を取得します。
 * @param[out]  pOut    地域名
 *
 * @pre
 *  - TIME ライブラリが初期化されている
 *
 * @details
 *  この関数で取得できる LocationName はシステムの最新の値であり、
 *  アプリケーション起動中であっても変更される可能性があります。
 */
void GetDeviceLocationName(LocationName* pOut) NN_NOEXCEPT;

/**
 * @brief       地域名を指定してタイムゾーンルールを読み込みます。
 * @param[out]  pOut    タイムゾーンルール
 * @param[in]   name    地域名
 *
 * @return  処理結果
 * @retval  ResultSuccess   成功しました。
 *
 * @pre
 *  - TIME ライブラリが初期化されている
 *  - pOut != nullptr
 *  - ライブラリが対応している地域名を指定している
 *
 *
 * @details
 *  本関数は最大数 msec ブロックする可能性があります。
 */
nn::Result LoadTimeZoneRule(TimeZoneRule* pOut, const LocationName& name) NN_NOEXCEPT;

/**
 * @brief   絶対時刻から現地時刻へ変換します。
 * @param[out]  pOutCalendar                現地時刻
 * @param[out]  pOutCalendarAdditionalInfo  現地時刻追加情報 ( 不要な場合は nullptr を入れてください )
 * @param[in]   posixTime                   絶対時刻
 * @param[in]   rule                        タイムゾーンルール
 *
 * @return  処理結果
 * @retval  ResultSuccess   成功しました。
 *
 * @pre
 *  - TIME ライブラリが初期化されている
 *  - pOutCalendar != nullptr
 *  - @ref InputPosixTimeMin <= posixTime <= @ref InputPosixTimeMax
 *  - タイムゾーンルールが読み込み済である
 *
 * @details
 *  この関数に渡す TimeZoneRule の先頭アドレスを 4 KB アライメントにすることで、
 *  Release ビルド時で数マイクロ秒の処理時間低減を行うことができます。
 */
nn::Result ToCalendarTime(
    CalendarTime* pOutCalendar,
    CalendarAdditionalInfo* pOutCalendarAdditionalInfo,
    const PosixTime& posixTime,
    const TimeZoneRule& rule) NN_NOEXCEPT;

/**
 * @brief   絶対時刻から現地時刻へ変換します。
 *          デバイスの設定に従ったタイムゾーンルールが自動で利用されます。
 * @param[out]  pOutCalendar                現地時刻
 * @param[out]  pOutCalendarAdditionalInfo  現地時刻追加情報 ( 不要な場合は nullptr を入れてください )
 * @param[in]   posixTime                   絶対時刻
 *
 * @return  処理結果
 * @retval  ResultSuccess   成功しました。
 *
 * @pre
 *  - TIME ライブラリが初期化されている
 *  - pOutCalendar != nullptr
 *  - @ref InputPosixTimeMin <= posixTime <= @ref InputPosixTimeMax
 *
 * @details
 *  @ref GetDeviceLocationName 関数で得られる @ref LocationName から生成した @ref TimeZoneRule が自動的に利用されます。
 */
nn::Result ToCalendarTime(
    CalendarTime* pOutCalendar,
    CalendarAdditionalInfo* pOutCalendarAdditionalInfo,
    const PosixTime& posixTime) NN_NOEXCEPT;

/**
 * @brief   絶対時刻を協定世界時 (UTC) の日時へ変換します。
 * @param[in]   posixTime                   絶対時刻
 * @return      日時 (UTC)
 *
 * @details
 *  本関数で得られる CalendarTime は協定世界時 (UTC) の時刻です。@n
 *  デバイスに設定されているタイムゾーンルールは考慮されません。
 */
CalendarTime ToCalendarTimeInUtc(const PosixTime& posixTime) NN_NOEXCEPT;

/**
 * @brief   現地時刻から絶対時刻へ変換します。
 * @param[out]  pOutCount               pOutPosixTimeList に格納された要素数
 * @param[out]  pOutPosixTimeList       絶対時刻
 * @param[in]   count                   pOutPosixTimeList の要素数
 * @param[in]   calendarTime            現地時刻
 * @param[in]   rule                    タイムゾーンルール
 *
 * @return  処理結果
 * @retval  ResultSuccess   成功しました。
 *
 * @pre
 *  - TIME ライブラリが初期化されている
 *  - pOutCount != nullptr
 *  - pOutPosixTimeList != nullptr
 *  - count > 0
 *  - 2000 <= calendarTime.year <= 2100
 *  - 1 <= calendarTime.month <= 12
 *  - 1 <= calendarTime.day <= 31
 *  - 0 <= calendarTime.hour <= 23
 *  - 0 <= calendarTime.minute <= 59
 *  - 0 <= calendarTime.second <= 59
 *  - タイムゾーンルールが読み込み済である
 *
 * @post
 *  - 0 <= *pOutCount <= 2
 *  - *pOutCount == 2 の場合、 pOutPosixTimeList[0] < pOutPosixTimeList[1]
 *
 */
nn::Result ToPosixTime(
    int* pOutCount,
    PosixTime* pOutPosixTimeList,
    int count,
    const CalendarTime& calendarTime,
    const TimeZoneRule& rule) NN_NOEXCEPT;

/**
 * @brief   現地時刻から絶対時刻へ変換します。
 *          デバイスの設定に従ったタイムゾーンルールが自動で利用されます。
 * @param[out]  pOutCount               pOutPosixTimeList に格納された要素数
 * @param[out]  pOutPosixTimeList       絶対時刻
 * @param[in]   count                   pOutPosixTimeList の要素数
 * @param[in]   calendarTime            現地時刻
 *
 * @return  処理結果
 * @retval  ResultSuccess   成功しました。
 *
 * @pre
 *  - TIME ライブラリが初期化されている
 *  - pOutCount != nullptr
 *  - pOutPosixTimeList != nullptr
 *  - count > 0
 *  - 2000 <= calendarTime.year <= 2100
 *  - 1 <= calendarTime.month <= 12
 *  - 1 <= calendarTime.day <= 31
 *  - 0 <= calendarTime.hour <= 23
 *  - 0 <= calendarTime.minute <= 59
 *  - 0 <= calendarTime.second <= 59
 *
 * @post
 *  - 0 <= *pOutCount <= 2
 *  - *pOutCount == 2 の場合、 pOutPosixTimeList[0] < pOutPosixTimeList[1]
 *
 */
nn::Result ToPosixTime(
    int* pOutCount,
    PosixTime* pOutPosixTimeList,
    int count,
    const CalendarTime& calendarTime) NN_NOEXCEPT;

/**
 * @brief   協定世界時 (UTC) の日時を絶対時刻へ変換します。
 * @param[in]   calendarTime            日時 (UTC)
 * @return      絶対時刻
 *
 * @pre
 *  - nn::time::IsValidDate(calendarTime.year, calendarTime.month, calendarTime.day) == true
 *  - 0 <= calendarTime.hour <= 23
 *  - 0 <= calendarTime.minute <= 59
 *  - 0 <= calendarTime.second <= 59
 *
 * @details
 *  入力の calendarTime が協定世界時 (UTC) での日時であるとして絶対時刻を計算します。@n
 *  デバイスに設定されているタイムゾーンルールは考慮されません。
 */
PosixTime ToPosixTimeFromUtc(const CalendarTime& calendarTime) NN_NOEXCEPT;

//! @}

}} // nn::time
