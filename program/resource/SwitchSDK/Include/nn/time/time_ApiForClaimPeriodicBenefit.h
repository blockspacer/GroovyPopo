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
 *   @brief  時計の日時を考慮したイベント管理のための API 宣言
 */

#pragma once

#include <nn/nn_TimeSpan.h>
#include <nn/time/time_ClockSnapshot.h>

namespace nn
{
namespace time
{


/**
 * @brief   日時を確認するためのコールバッククラス
 * @details
 *  nn::time::ClaimPeriodicBenefitWithUserSystemClock() で利用するクラスです。
 *
 *  本クラスを継承したクラスで日時の確認処理( @ref IsSameTiming(), @ref IsMatched() )
 *  を実装する必要があり、そのクラスのオブジェクトを nn::time::ClaimPeriodicBenefitWithUserSystemClock() に渡して利用します。
 *
 *  IsSameTiming(), IsMatched() は nn::time::ClaimPeriodicBenefitWithUserSystemClock() 経由で適切に呼ばれますが、
 *  必ず呼ばれることを期待した実装はしないでください。
 */
class CheckCalendarTimeCallback
{
public:
    /**
     * @brief   2つの日時が、同じ利益の受け取りタイミングかどうかを判定します。
     * @param[in]   lhs     比較する一方の CalendarTime
     * @param[in]   rhs     比較する他方の CalendarTime
     *
     * @return  判定結果
     * @retval  true    同じタイミングである
     * @retval  false   同じタイミングではない
     *
     * @details
     *  この関数は、同じタイミングで繰り返し利益を受け取ることを防ぎます。@n
     *  2つの日時を比較して、利益を受け取るタイミングとして同じであれば true を、
     *  そうでなければ false を返すよう実装してください。@n
     *  なお、繰り返し利益を受けることを防ぐ必要がない場合には、常に false を返すように実装してください。
     *
     *  以下に実装の具体例を挙げます。@n
     *  - 同じ日に再び利益を受け取ることができない場合には、lhs と rhs の日付が同じであれば true を返します。
     *  - 同じ週に再び利益を受け取ることができない場合には、lhs と rhs の属する週が同じであれば true を返します。
     *
     * @see nn::time::ClaimPeriodicBenefitWithUserSystemClock()
     */
    virtual bool IsSameTiming(
        const CalendarTime& lhs,
        const CalendarTime& rhs) const NN_NOEXCEPT = 0;

    /**
     * @brief   与えられた日時が利益を受け取ることができる日時として適切かどうかを判定します。
     * @param[in]   calendarTime    判定対象の日時
     *
     * @return  判定結果
     * @retval  true    利益を受け取ることができるタイミングとして適切である
     * @retval  false   利益を受け取ることができるタイミングとして適切ではない
     *
     * @details
     *  この関数は、与えられた日時が(前回利益を受け取った日時との比較なしに)利益を受け取るタイミングとして適切かどうかを判定します。@n
     *  calendarTime が利益を受け取ることができるタイミングであれば true を、
     *  そうでなければ false を返すよう実装してください。
     *
     *  以下に実装の具体例を挙げます。@n
     *  - 日付が変わったら、週が変わったら、などのある特定の一日において、
     *    いつでも利益を得られる可能性のある場合は何もせず true を返します。
     *  - 毎週日曜日にだけ利益を受け取ることができるのであれば、
     *    calendarTime が日曜日であれば true を返します。(曜日の取得には @ref nn::time::GetDayOfWeek() を利用可能)
     *  - 毎日、午前中の9:00～12:00 にだけ利益を受け取ることができるのであれば、
     *    calendarTime がその時間内であれば true を返します。
     *
     * @see nn::time::ClaimPeriodicBenefitWithUserSystemClock()
     */
    virtual bool IsMatched(const CalendarTime& calendarTime) const NN_NOEXCEPT = 0;
};

/**
 * @brief   定期的な利益を得られるかどうかの判定に用いる情報を保持する構造体です。
 *
 * @details
 *
 *  nn::time::ClaimPeriodicBenefitWithUserSystemClock() の機能を実現するための情報を保持するクラスです。
 *
 *  この構造体は C++ 標準の TriviallyCopyable の要件を満たしており、 std::memcpy() が可能な型です。
 *
 * @see nn::time::ClaimPeriodicBenefitWithUserSystemClock()
 */
struct PeriodicBenefitClaimContext
{
    /**
     * @brief   最後に利益を受け取った瞬間のユーザー時計のカレンダー情報を返します。
     * @param[out]  pOut    カレンダー情報を格納するバッファへのポインタを指定します。
     *
     * @pre
     *  - pOut != nullptr
     *
     * @return  カレンダー情報取得の成否
     * @retval  true    成功
     * @retval  false   失敗
     *
     * @details
     *  利益を一度も受け取ったことがない場合には false が返ります。
     *
     * @see ClaimPeriodicBenefitWithUserSystemClock()
     */
    bool GetLastReceivedStandardUserSystemClockCalendarTime(CalendarTime* pOut) const NN_NOEXCEPT;

    /**
     * @brief   ペナルティが終わる時刻を取得します。
     *
     * @param[out]  pOut    ペナルティ期間が終わる瞬間の時刻
     *
     * @pre
     *  - pOut != nullptr
     *
     * @return  時刻取得の成否
     * @retval  true    成功
     * @retval  false   失敗(ペナルティ期間が発生していない)
     *
     * @details
     *  最後に @ref nn::time::ClaimPeriodicBenefitWithUserSystemClock() で判定した結果をもとに、
     *  ペナルティ期間が終わる瞬間の nn::time::SteadyClockTimePoint を返します。
     *
     *  得られる時刻がすでに過ぎ去っている場合や、
     *  @ref nn::time::StandardSteadyClock から得られる最新の値と連続性がない場合があります。
     *
     *  本関数は開発の補助を目的としており、製品では使用できません。よって、以下のような実装はできません。@n
     *  - 本関数で得た時刻、もしくはそれから計算した何らかの情報を製品の UI に反映すること
     *  - 本関数で得た時刻を利用して利益を得られるかどうかを判定すること
     *
     * @see nn::time::StandardSteadyClock::GetCurrentTimePoint(), nn::time::GetSpanBetween()
     */
    bool GetEndOfPenaltyAtLastClaimForDebug(SteadyClockTimePoint* pOut) const NN_NOEXCEPT;

    /**
     * @brief   nn::time::PeriodicBenefitClaimContext 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が同じなら true を、異なるなら false を返します。
     *
     * @details
     */
    friend bool operator == (const PeriodicBenefitClaimContext& lhs, const PeriodicBenefitClaimContext& rhs) NN_NOEXCEPT;

    /**
     * @brief   nn::time::PeriodicBenefitClaimContext 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が同じなら true を、異なるなら false を返します。
     *
     * @details
     */
    friend bool operator != (const PeriodicBenefitClaimContext& lhs, const PeriodicBenefitClaimContext& rhs) NN_NOEXCEPT;

    ClockSnapshot _snapshot;
    ClockSnapshot _lastBenefitReceivedSnapshot;
    SteadyClockTimePoint _penaltyEndSteadyClockTimePoint;
    bool _isMaxPenalatyOccurred;
    nn::Bit8 _reserved[327];
};

/**
 * @brief   利益を得られるかどうかの判定結果を表す列挙型です。
 *
 * @details
 *
 * @see nn::time::ClaimPeriodicBenefitWithUserSystemClock()
 */
enum PeriodicBenefitClaimResult
{
    PeriodicBenefitClaimResult_Success = 0,         //!< 利益を得ることができる
    PeriodicBenefitClaimResult_NotReceivableTiming, //!< 利益を得られるタイミングではない
    PeriodicBenefitClaimResult_Penalty,             //!< ペナルティによって利益を得られない
};

//! @name   ClockSnapshot 向けAPI
//! @{

/**
 * @brief   ユーザー時計の操作を考慮した上で、定期的な利益を得られるかどうかを判定します。
 * @param[in,out]  pHandover
 *  前回の判定で得た pHandover@n
 *  (関数実行後、次回の判定の pHandover として本関数の成否に関係なく引き継ぐ必要があります。@n
 *  初回の判定では、コンストラクタで生成されたままの PeriodicBenefitClaimContext を指定してください。
 *  このとき時計の操作がまったくないものとして判定します。)
 *
 * @param[in]   currentSnapshot     利益を得られるかどうかを判定する今現在の ClockSnapshot
 *
 * @param[in]   maxPenaltyTimeSpan
 *  ペナルティ期間の最大値@n
 *  (時計操作のないときに利益を得られる間隔以下の値を指定することを推奨します。
 *  例えば日付が変わると新しく利益を得られるのであれば nn::TimeSpan::FromDays(1) 以下を指定します。)
 *
 * @param[in]   acceptableOperationTimeSpan
 *  未来へ進める方向への時計操作時に許容する時計の操作量@n
 *  (この値を小さくすると、本来意図していたより短い間隔で利益を得ることを防ぐのには有効ですが、
 *  ほんの少し時計を操作するだけで maxPenaltyTimeSpan 分のペナルティを受ける可能性が上がるので、
 *  nn::TimeSpan::FromMinutes(10) 程度の値を指定することを推奨します。
 *  ただし、利益を得られる間隔が短く、acceptableOperationTimeSpan が相対的に大きくなってしまう場合には値を小さくしてください。)
 *
 * @param[in]   pCheckCalendarTimeCallback
 *  日時を確認するためのコールバッククラス@n
 *  (詳細は @ref CheckCalendarTimeCallback クラスを参照してください)
 *
 * @return  判定結果
 * @retval  nn::time::PeriodicBenefitClaimResult_Success                利益を得ることができます。
 * @retval  nn::time::PeriodicBenefitClaimResult_NotReceivableTiming    新たに利益を得られるタイミングになっておらず、利益を得ることができません。
 * @retval  nn::time::PeriodicBenefitClaimResult_Penalty                ペナルティ期間中であり、利益を得ることができません。
 *                                                                      ペナルティ期間が過ぎてもその他の失敗を示す結果が返る可能性があります。
 * @pre
 *  - TIME ライブラリが初期化されている
 *  - currentSnapshot が nn::time::ClockSnapshot::CreateWithStandardSystemClock() か nn::time::ClockSnapshot::CreateWithAdjustableSystemClock() で初期化されている
 *  - pHandover != nullptr
 *  - maxPenaltyTimeSpan > nn::TimeSpan(0)
 *  - acceptableOperationTimeSpan >= nn::TimeSpan(0)
 *  - pCheckCalendarTimeCallback != nullptr
 *
 * @details
 *  本関数は、毎日日付が変わると利益を得られる、毎週特定の曜日にだけイベントが発生する、
 *  といった日時を基準に定期的に発動するイベントを、ユーザー時計( @ref nn::time::StandardUserSystemClock )
 *  の時刻で管理するアプリケーション向けに用意されたユーティリティです。
 *  ユーザーによる意図的な時計操作が行われたとしても、本来意図していたより短い間隔で利益を得たり、
 *  また逆に、必要以上に利益の受け取りを制限したりすることがないような実装が可能です。
 *
 *  時計が操作されると、その操作内容に応じて利益を得られないペナルティ期間が設けられ、
 *  この期間内は新たに利益が得られるはずの日時が来たとしても、利益を得ることができません。
 *  ペナルティ期間は、時計を操作したタイミングからその期間が実時間で経過することによって自動的に解かれます。
 *
 *  ### 時計の操作量を判断できる場合
 *  ペナルティ期間は、過去の判定タイミングと currentSnapshot の2つの時点から求まるユーザーによる意図的な時計の操作量を使って以下のように決定されます。
 *  なお、システムによる自動的な時刻補正がユーザー時計に影響してもペナルティとはなりません。@n
 *  | 時計操作のケース | ペナルティ期間のかかり方 |
 *  |:-----------------|:-------------------------|
 *  |時計がまったく操作されていない場合| ペナルティ期間なし。 |
 *  |時計が未来へ進む方向へ操作された場合| maxPenaltyTimeSpan がペナルティ期間となります。@n ただし、時計の操作量が acceptableOperationTimeSpan 以下の場合、ペナルティ期間は発生しません。 |
 *  |時計が過去へ戻る方向へ操作された場合| maxPenaltyTimeSpan を最大として、時計の操作量がそのままペナルティ期間となります。@n ペナルティ期間が過ぎ去った後、新たに利益を得られるようになるタイミングが、前回利益を得たタイミングと同じになることがあります。@n (日付が変わることで新たな利益を得られる仕様として、7月20日に利益を得て、その後24時間時計を巻き戻して24時間のペナルティ期間が発生した場合、ペナルティ期間が過ぎ去り新たな利益が得られるようになる日付は7月20日となります)|
 *
 *  ### 時計の操作量を判断できない場合
 *  上記はユーザー時計の操作量を正しく判断できる場合についてですが、
 *  単調増加クロックの値同士に連続性がない場合には、時計の操作量を判断することができません
 *  (単調増加クロックのリセット発生時や、セーブデータの別デバイスへの移行時などが該当)。@n
 *  このとき、pHandover が保持する前回利益を得たタイミングからの経過時間取得の成否によって、以下のようにペナルティ期間が決定されます。
 *  経過時間の取得には @ref nn::time::CalculateSpanBetween() を用いており、成否の条件はそちらを参照してください。@n
 *  | 前回の利益受け取りから currentSnapshot までの経過時間の取得結果 | ペナルティ期間のかかり方 |
 *  |:----------------------------------------------------------------|:-------------------------|
 *  |利益を一度も受け取ったことがなく経過時間を取得できない|ペナルティ期間なし。|
 *  |経過時間の取得成功|前回利益を受け取ったタイミングから maxPenaltyTimeSpan がペナルティ期間となります。@nすでにペナルティ期間が過ぎ去っていることもあります。|
 *  |経過時間の取得失敗|currentSnapshot から maxPenaltyTimeSpan がペナルティ期間となります。|
 *
 *  ただし、 過去の判定タイミングの時点と currentSnapshot がそれぞれ
 *  本体設定の「インターネットで時間をあわせる」がON、かつネットワーク時計が補正済の場合は、
 *  ユーザー時計は未操作と判断できるのでペナルティはかかりません。@n
 *
 *  ### その他の事項
 *  - 特に気を付けるハンドリングについて@n
 *  pHandover は本関数の結果に関係なく次回の判定へ引き継ぐ必要があることに注意してください。
 *  - 利益の受け取り結果の保存について@n
 *  @ref PeriodicBenefitClaimResult_Success が返った場合、アプリケーション側で利益の受け取り処理を行ってください。
 *  このとき、アプリケーションの終了やデバイスの電源断によるデータの不整合を防ぐため、
 *  利益を受け取ったアプリケーション側の結果と pHandover とは、不可分にセーブデータに保存することを強く推奨します。
 *  - デバイスのタイムゾーン変更について@n
 *  変更前後のタイムゾーンの時差の差分が、時計の操作量として加味されます。
 *  ただし、夏時間などのデバイスのタイムゾーン変更を伴わない時差の変化は、時計の操作量としては扱われません。
 *  例えば、UTC との時差が -3 から +1 に変更されると、時計を4時間進めたことと同等に扱われます。
 *  タイムゾーン変更を伴わず夏時間の影響などで時間が1時間ずれたとしても、それは時計の操作量としては扱われません。
 *
 */
PeriodicBenefitClaimResult ClaimPeriodicBenefitWithUserSystemClock(
    PeriodicBenefitClaimContext* pHandover,
    const ClockSnapshot& currentSnapshot,
    const nn::TimeSpan& maxPenaltyTimeSpan,
    const nn::TimeSpan& acceptableOperationTimeSpan,
    const CheckCalendarTimeCallback* pCheckCalendarTimeCallback) NN_NOEXCEPT;

//! @}

}
}
