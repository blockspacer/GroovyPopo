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
*   @brief  ネットワークから自動補正される時計を表すクロックのクラス宣言
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
 * @brief   ネットワークから自動補正される時計を表すクロックです。
 *
 * @details
 *  このクロックは現在時刻を指し示し、人間の理解可能な日時表現に変換できる値を取得することができます。@n
 *  補正済であれば、デバイスがネットワークに接続していなくても値を取得することができます。
 *
 *  このクロックはユーザーによる時計の変更・補正の影響は受けませんが、システムによる自動補正の影響を受け、指し示す日時が実際の時間経過以上に進んだり過去へ巻き戻ったりすることがあります。
 *  また、デバイスの電源が落ちているときもカウントアップされますが、電池切れにより電源の供給が途絶えた場合は時計が無効化され、値を返せない状態になります。
 *  この状態は、デバイスが再びネットワークから日時を取得することで回復されます。
 *  時計の補正や無効化の発生をまたいでおこなわれた時間経過の計算結果は厳密ではありませんが、定期的に正しい日時へ補正されることが期待できるため、一定の信頼を置くことができます。@n
 *  なお、デバイスの電源が入っている限りは、このクロックが値を返せる状態から返せない状態へ遷移することはありません。
 *  逆に、値を返せない状態から返せる状態へは、アプリケーションが起動中であっても自動的に遷移することがあります。
 *
 *  このクラスは C++ 標準の TrivialClock の要件を満たしており、 std::chrono::system_clock に準ずる機能を提供します。
 *  ただし C++ 標準のインターフェースのみを利用している場合、時計の操作や無効化が発生したことは検知できません。
 *  また、時計が無効なあいだに取得した時刻は常に固定の値となります。
 *
 *  ### @anchor network_clock_autonomic_correction ネットワーク時計の自動補正について
 *  システムによる自動補正は、デバイスがネットワークを利用可能なときに定期的に自動実行され、
 *  通信エラーやサーバー異常等によって失敗したとしても自動的にリトライされます。@n
 *  ただし、継続的な通信エラーやサーバー異常等が発生している場合、デバイスがネットワークを利用可能な状態であっても、
 *  このクロックが値を返せない可能性があることに注意してください。
 *
 *  ### @anchor network_clock_accuracy ネットワーク時計の時刻精度について
 *  このクロックは時刻のカウントアップに nn::time::StandardSteadyClock を利用しており、月に数分程度の誤差が累積します。@n
 *  この累積誤差はサーバーから本来の時刻で補正されるたびにクリアされますが、
 *  補正処理の通信遅延によっては補正時点で数十秒程度のずれが生じることがあります。@n
 *  @link standard_steady_clock_accuracy 単調増加クロックの累積誤差について @endlink も併せて参照してください。
 */
class StandardNetworkSystemClock
{
public:
    /**
     * @brief   クロックの指し示す現在時刻を取得します。
     *
     * @param[out]  pOutPosixTime   時刻を代入するバッファへのポインタを指定します。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     * @retval  ResultClockInvalid                  時計が無効です。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *  - pOutPosixTime != nullptr
     *
     * @details
     * @platformbegin{Windows}
     *  Windows 環境では本関数は必ず成功し、 PC 時計と同等の時刻が返されます。
     * @platformend
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
     */
    static Result GetSystemClockContext(SystemClockContext* pOutSystemClockContext) NN_NOEXCEPT;

    /**
     * @brief   時計の精度が十分に信頼できるかどうかを返します。
     * @return  時計の精度が十分に信頼できるかどうか
     * @retval  true    時計の精度が十分に信頼できる
     * @retval  false   時計の精度は十分には信頼できない
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     *  インターネットに長期間接続していない間にも、ネットワーク時計の精度を特に気にするアプリケーションで利用してください。
     *
     *  最後の時刻補正から 30 日以上が経過していれば、時計の誤差が一定量を超えて累積していると判断して false を返します。
     *  その場合、後に時刻補正がされることで本関数は true を返すようになります。@n
     *  @link network_clock_autonomic_correction ネットワーク時計の自動補正について @endlink も参照してください。
     *
     *  アプリケーションの起動中においても、本関数の返り値は true から false、もしくはその逆の遷移をすることがあります。@n
     *  時計の精度については @link network_clock_accuracy ネットワーク時計の時刻精度について @endlink を参照してください。
     */
    static bool IsAccuracySufficient() NN_NOEXCEPT;

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
     *  システムによる自動補正により、このクロックが指す時刻は逆行する可能性があります。
     */
    static const bool is_steady = false;

    /**
     * @brief   このクロックの指し示す現在時刻を取得します。
     *
     * @return  このクロックが指し示す現在時刻を nn::time::StandardNetworkSystemClock::time_point 型で返します。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     *  時計が無効な場合、1970年1月1日0時0分0秒を表す nn::time::StandardNetworkSystemClock::time_point 型の値が返ります。
     *
     * @platformbegin{Windows}
     *  Windows 環境では PC 時計と同等の時刻が返されます。
     * @platformend
     */
    static time_point now() NN_NOEXCEPT;

    /**
     * @brief   nn::time::StandardNetworkSystemClock::time_point 型の時刻を std::time_t 型に変換します。
     *
     * @param[in]   t   nn::time::StandardNetworkSystemClock::time_point 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す std::time_t 型の値が返ります。
     */
    static std::time_t to_time_t(const time_point& t) NN_NOEXCEPT;

    /**
     * @brief   std::time_t 型の時刻を nn::time::StandardNetworkSystemClock::time_point 型に変換します。
     *
     * @param[in]   t   std::time_t 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す nn::time::StandardNetworkSystemClock::time_point 型の値が返ります。
     *
     * @details
     *  nn::time::StandardNetworkSystemClock::time_point 型の時刻の分解能が std::time_t 型のそれより低い場合、 nn::time::StandardNetworkSystemClock::time_point 型の分解能に丸められます。
     */
    static time_point from_time_t(std::time_t t) NN_NOEXCEPT;
};

}
}
