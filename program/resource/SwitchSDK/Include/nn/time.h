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
 *  @file
 *  @brief  時間・時刻に関する API の宣言
 */


#pragma once


/**
 *  @namespace  nn::time
 *  @brief      TIME ライブラリの名前空間です。
 *
 *              ### 機能概要
 *              TIME ライブラリは、時間・時刻の取得・計算・変換などにまつわる機能を提供します。@n
 *
 *              ### 共通の事前条件
 *              - TIME ライブラリが提供する API は原則として、利用前に nn::time::Initialize 関数でライブラリを初期化しておく必要があります。
 *
 *              ### Windows 上での挙動
 *              Windows 上での動作環境では、内部で利用する一定のペースで単調増加するカウンタに PC の時計を代用しています。
 *              @n
 *              よって PC の時計が操作された場合には、TIME ライブラリを通して得られる時刻がその操作の分だけずれます。
 *              このずれが発生したことを TIME ライブラリで検知する方法はありません。
 *              @n
 *              ただし、TIME ライブラリの初期化から終了までの間に PC の時計が操作されても、TIME ライブラリを通して得られる時刻は影響を受けません。
 *
 *              Windows 上で動作させた場合、このライブラリは fs ライブラリの機能を使用します。
 *              nn::fs::SetAllocator() で設定したアロケータか malloc() によって fs ライブラリがメモリを確保できる状態で利用してください。
 *              詳細は fs ライブラリのマニュアルを参照してください。
 *
 *              ### @anchor about_time_zone_api タイムゾーンを扱う API について
 *              - 現地時刻・絶対時刻 を計算するために必要な @ref TimeZoneRule の内部仕様は変更される可能性があるので、セーブデータ等に保存して次回起動時に使いまわすようなことはしないでください。
 *              - 本体設定や DevMenu で選択可能な @ref LocationName は、将来の運用によって変わる可能性があります。ただし、一度でも選択可能であったものは将来に渡って TIME ライブラリで扱えるようサポートします。
 *              - @ref ToCalendarTime 関数で動作保証する入力範囲は @ref InputPosixTimeMin から @ref InputPosixTimeMax までです。
 *              - @ref ToPosixTime 関数で動作保証する入力範囲は 2000年1月1日午前0時0分0秒 から 2100年1月1日午前0時0分0秒 までです。
 *              - タイムゾーンごとの時差の運用ルールは、本体更新等で更新される可能性があり、将来に渡って同じルールが適用される保証はありません。
 *              - 未来における時差の運用ルールにおいては、現時点で分かっている最新のルールが未来でもそのまま適用されているとして計算されます。
 *              - TIME ライブラリを通して得られる時刻は必ず動作保証の範囲内であることが保証されます。ただし、 Windows 上での動作環境においてはこの限りではありません。
 */

#include <nn/time/time_Common.h>
#include <nn/time/time_Types.h>
#include <nn/time/time_Api.h>
#include <nn/time/time_ApiForClaimPeriodicBenefit.h>
#include <nn/time/time_ApiForClockSnapshot.h>
#include <nn/time/time_TimeZoneApi.h>
#include <nn/time/time_AdjustableUserSystemClock.h>
#include <nn/time/time_AdjustableNetworkSystemClock.h>
#include <nn/time/time_StandardNetworkSystemClock.h>
#include <nn/time/time_StandardUserSystemClock.h>
#include <nn/time/time_StandardSteadyClock.h>
