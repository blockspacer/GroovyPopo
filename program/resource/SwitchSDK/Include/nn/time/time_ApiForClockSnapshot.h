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
 *   @brief  ClockSnapshot 向けAPI宣言
 */

#pragma once

#include <nn/nn_TimeSpan.h>
#include <nn/time/time_ClockSnapshot.h>

namespace nn
{
namespace time
{

//! @name   ClockSnapshot 向けAPI
//! @{

/**
 * @brief   ユーザー時計がユーザーによって意図的に操作された量を取得します。
 * @param[in]   from    操作検知の起点となるオブジェクト
 * @param[in]   to      操作検知の終点となるオブジェクト
 *
 * @return  ユーザーによる意図的な時計の操作量
 *
 * @pre
 *  - TIME ライブラリが初期化済である
 *  - from と to が nn::time::ClockSnapshot::CreateWithStandardSystemClock() 同士、
 *    もしくは nn::time::ClockSnapshot::CreateWithAdjustableSystemClock() 同士で初期化されている
 *
 * @details
 *  from から to までの間に、
 *  ユーザー時計( @ref nn::time::StandardUserSystemClock )から得られる nn::time::PosixTime が、
 *  ユーザーによって意図的に操作された量を取得します。
 *  未来の方向への操作であれば正の、過去の方向への操作であれば負の nn::TimeSpan が返ります。
 *
 *  nn::TimeSpan(0) が返ったとしても、ユーザーによる意図的な操作がなかったことを示すだけであり、
 *  ユーザー時計の nn::time::PosixTime の巻き戻りや、実時間の経過以上の大きな進みがないことを保証するものではありません。
 *
 *  ユーザー時計の時刻を nn::time::CalendarTime に変換した日時が操作されたかどうかを判定するものではないことに注意してください。
 *  夏時間等の影響による時差変化や、ユーザーによって本体のタイムゾーンが変更されたとしても、
 *  nn::time::PosixTime 自体は操作されないため、本関数は nn::TimeSpan(0) を返します。@n
 *  本体のタイムゾーン変更の検知には、 from.GetLocationName() と to.GetLocationName() の比較が別途必要です。
 *
 *  from と to の間に以下のいずれかが発生していた場合、
 *  ユーザーによって時計が操作されたと判断します。@n
 *  - 時計をユーザーが直接操作した(本体設定, DevMenu, DevMenuCommand などで操作可能)
 *  - 本体設定の「インターネットで時間をあわせる」がユーザーによってOFFからONに切り替えられ、ユーザー時計の時刻が変わった
 *
 *  一方で下記のいずれかが発生した場合、 nn::time::PosixTime に変化があったとしても、ユーザーによる操作とは判断されません。@n
 *  - 本体設定の「インターネットで時間をあわせる」が常にONであり、システムによる時計の自動補正が行われた
 *  - 単調増加クロックの電池切れによってユーザー時計の時刻が初期値に戻った
 *  - セーブデータが別デバイスへ移行された
 */
nn::TimeSpan CalculateStandardUserSystemClockDifferenceByUser(
    const ClockSnapshot& from,
    const ClockSnapshot& to) NN_NOEXCEPT;

/**
 * @brief   2つの nn::time::ClockSnapshot 間の実時間での経過時間を計算します。
 * @param[out]  pOut    経過時間を格納するバッファへのポインタ
 * @param[in]   from    経過時間計算の起点となるオブジェクト
 * @param[in]   to      経過時間計算の終点となるオブジェクト
 *
 * @return  計算結果
 * @retval  true    成功
 * @retval  false   失敗
 *
 * @pre
 *  - TIME ライブラリが初期化済である
 *  - pOut != nullptr
 *
 * @details
 *  終点が起点より過去を指していた場合、経過秒数として負の値が返ります。
 *
 *  セーブデータが別デバイスへ移行されたときや、単調増加クロックのリセット発生などにより、
 *  起点と終点が保持する単調増加クロックの値同士に連続性がなくなった場合にも、
 *  ネットワーク時計の時刻を利用して経過時間を計測可能です。@n
 *  ただし、ネットワーク時計の時刻は単調増加が保証されないため、経過時間の計算に少なからず誤差が生じます。
 *  それにより、特に短い期間の計測においては、実際には正の経過時間であっても
 *  誤差によって負の経過時間が返されるなどの可能性があります。
 *  誤差についての詳細は @link network_clock_accuracy ネットワーク時計の時刻精度について @endlink を参照してください。
 *
 *  以下のすべてを満たす場合、経過時間の計算ができず false を返します。@n
 *  - 起点と終点が保持する単調増加クロックの値同士に連続性がない
 *  - 起点と終点のどちらか一方でもネットワーク時計の時刻を保持していない
 */
bool CalculateSpanBetween(
    nn::TimeSpan* pOut,
    const ClockSnapshot& from,
    const ClockSnapshot& to) NN_NOEXCEPT;

//! @}

}
}
