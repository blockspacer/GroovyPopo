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
*   @brief  ユーザーが操作可能なデバイスの時計(ユーザー時計)を表すクロックのクラス宣言
*/

#pragma once

#include <nn/time/time_Common.h>

#include <nn/time/time_Types.h>
#include <nn/time/time_SystemClockTraits.h>

#include <chrono>
#include <ctime>


namespace nn
{
namespace time
{

/**
 * @brief   ユーザーが操作可能なデバイスの時計(ユーザー時計)を表すクロックです。
 *
 * @details
 *  このクロックはデバイスが持つ現在時刻を指し示し、人間の理解可能な日時表現に変換できる値を取得することができます。@n
 *  ユーザーによる時刻操作だけでなく、以下のいずれかに該当する場合システムによる自動的な時刻補正の影響を受け、
 *  @ref nn::time::StandardNetworkSystemClock と同等の時刻を返します。@n
 *  - 本体設定の「インターネットで時間をあわせる」が ON になっている
 *  - DevMenu による時計設定で "Automatic Correction" にチェックが付いている
 *  - "DevMenuCommand time get-user-clock-auto-correction" で enabled と表示される
 *
 *  このクロックはユーザーやシステムによる時計の変更・補正の影響を受け、指し示す日時が実際の時間経過以上に進んだり過去へ巻き戻ったりすることがあります。
 *  また、デバイスの電源が落ちているときもカウントアップされますが、電池切れにより電源の供給が途絶えた場合は値がリセットされます。
 *  このとき、時計の操作やリセットの発生したことは検出できますが、それらをまたいでおこなわれた時間経過の計算結果は信頼できません。@n
 *
 *  このクラスは C++ 標準の TrivialClock の要件を満たしており、 std::chrono::system_clock に準ずる機能を提供します。
 *  ただし C++ 標準のインターフェースのみを利用している場合、時計の操作や値のリセットが発生したことは検知できません。
 *
 *  このクロックは時刻のカウントアップに nn::time::StandardSteadyClock を利用しており、
 *  nn::time::StandardSteadyClock の累積誤差の影響を受けます。@n
 *  @link standard_steady_clock_accuracy 単調増加クロックの累積誤差について @endlink も併せて参照してください。
 */
class StandardUserSystemClock
{
public:
    /**
     * @brief   クロックの指し示す現在時刻を取得します。
     *
     * @param[out]  pOutPosixTime   時刻を代入するバッファへのポインタを指定します。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *  - pOutPosixTime != nullptr
     *
     * @details
     */
    static Result GetCurrentTime(PosixTime* pOutPosixTime) NN_NOEXCEPT;


    /**
     * @brief   クロックが利用しているコンテキストの値を取得します。
     *
     * @param[out]  pOutSystemClockContext    コンテキストを代入するバッファへのポインタを指定します。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *  - pOutSystemClockContext != nullptr
     *
     * @details
     *  コンテキストを比較することで時計に対する操作がおこなわれたか確認することができます。
     *
     *  ユーザーによる時刻操作だけでなく、
     *  システムによる自動的な時刻補正によっても2つのコンテキストが不一致となることがある点に注意してください。@n
     *  システムによる本クロックの自動的な時刻補正が有効であるかどうかは以下のいずれかで確認できます。
     *
     *  - 本体設定の「インターネットで時間をあわせる」が ON になっている
     *  - DevMenu による時計設定で "Automatic Correction" にチェックが付いている
     *  - "DevMenuCommand time get-user-clock-auto-correction" で enabled と表示される
     *
     */
    static Result GetSystemClockContext(SystemClockContext* pOutSystemClockContext) NN_NOEXCEPT;


    /**
     * @brief   このクロックから取得・計算される時刻・時間の内部表現となる算術型を表します。
     *
     * @details
     */
    typedef SystemClockTraits::rep rep;

    /**
     * @brief   このクロックの持つ分解能を std::ratio 型で表したものです。
     *
     * @details
     *  1単位当たりの秒数で表されています。
     */
    typedef SystemClockTraits::period period;

    /**
     * @brief   経過時間を表す型です。
     *
     * @details
     */
    typedef SystemClockTraits::duration duration;

    /**
     * @brief   このクロックから取得される、時間の流れの中の一点を指す型です。
     *
     * @details
     */
    typedef SystemClockTraits::time_point time_point;

    /**
     * @brief   このクロックの単調増加性を表す値です。
     *
     * @details
     *  false です。
     *  ユーザー操作やシステムによる自動補正により、このクロックが指す時刻は逆行する可能性があります。
     */
    static const bool is_steady = false;

    /**
     * @brief   このクロックの指し示す現在時刻を取得します。
     *
     * @return  このクロックが指し示す現在時刻を nn::time::StandardUserSystemClock::time_point 型で返します。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     */
    static time_point now() NN_NOEXCEPT;

    /**
     * @brief   nn::time::StandardUserSystemClock::time_point 型の時刻を std::time_t 型に変換します。
     *
     * @param[in]   t   nn::time::StandardUserSystemClock::time_point 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す std::time_t 型の値が返ります。
     */
    static std::time_t to_time_t(const time_point& t) NN_NOEXCEPT;

    /**
     * @brief   std::time_t 型の時刻を nn::time::StandardUserSystemClock::time_point 型に変換します。
     *
     * @param[in]   t   std::time_t 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す nn::time::StandardUserSystemClock::time_point 型の値が返ります。
     *
     * @details
     *  nn::time::StandardUserSystemClock::time_point 型の時刻の分解能が std::time_t 型のそれより低い場合、 nn::time::StandardUserSystemClock::time_point 型の分解能に丸められます。
     */
    static time_point from_time_t(std::time_t t) NN_NOEXCEPT;
};

}
}
