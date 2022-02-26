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
 * @brief   システム側時刻に任意のタイミングで揃えることが可能な、ネットワークから自動補正される時計を表すクロックのクラス宣言
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
 * @brief   システム側時刻に任意のタイミングで揃えることが可能な、ネットワークから自動補正される時計を表すクロックです。
 *
 * @details
 *  このクロックは現在時刻を指し示し、人間の理解可能な日時表現に変換できる値を取得することができます。@n
 *  補正済であれば、デバイスがネットワークに接続していなくても値を取得することができます。@n
 *  @ref Adjust 関数を実行しない限りはシステム側の自動補正の影響を受けることがなく、
 *  アプリケーションでの扱いが容易な、時刻の巻き戻りや大きな進みの発生を制御できるクロックです。
 *
 *  このクロックはユーザーによる時計の変更・補正の影響は受けませんが、
 *  @ref Adjust 関数の実行によりシステム側( @ref nn::time::StandardNetworkSystemClock ) の時刻に揃えることができ、
 *  その際指し示す日時が実際の時間経過以上に進んだり過去へ巻き戻ったりすることがあります。
 *  また、デバイスの電源が落ちているときもカウントアップされますが、電池切れにより電源の供給が途絶えた場合は時計が無効化され、値を返せない状態になります。
 *  この状態は、デバイスが再びネットワークから日時を取得することで回復されます。
 *  時計の補正や無効化の発生をまたいでおこなわれた時間経過の計算結果は厳密ではありませんが、定期的に正しい日時へ補正されることが期待できるため、一定の信頼を置くことができます。
 *
 *  メンバ関数はスレッドセーフです。@n
 *  このクラスは C++ 標準の TrivialClock の要件を満たしており、 std::chrono::system_clock に準ずる機能を提供します。
 *  ただし C++ 標準のインターフェースのみを利用している場合、時計の操作や無効化が発生したことは検知できません。
 *  また、時計が無効なあいだに取得した時刻は常に固定の値となります。
 *
 *  ### 時計の時刻精度について
 *  このクロックは、 @ref Adjust 関数の実行なしではシステムによる定期的な時刻の自動補正の恩恵を受けられないため、
 *  基本的に nn::time::StandardNetworkSystemClock より精度の低い時刻を常に提供します。
 *  @link network_clock_autonomic_correction ネットワーク時計の自動補正について @endlink も参照してください。@n
 *  より高い精度の現在時刻が必要な場合には @ref nn::time::StandardNetworkSystemClock::GetCurrentTime 関数を利用してください。
 */
class AdjustableNetworkSystemClock
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
     *  Windows 環境では本関数は必ず成功し、 PC 時計の時刻を起点とした値が返されます。
     * @platformend
     *
     *  得られる時刻はシステムの最新時刻とは異なる可能性があることに注意してください。
     *
     *  得られる時刻をシステムの最新時刻に揃える @ref Adjust 関数を実行します。
     *
     *  デバイスの電源が入っている限りは、現在時刻を返せる状態から返せない状態へ遷移することはありません。
     *  逆に、値を返せない状態から返せる状態へは自動的に遷移することがあります。
     */
    static Result GetCurrentTime(PosixTime* pOutPosixTime) NN_NOEXCEPT;

    /**
     * @brief   クロックの時刻をシステム側で保持する最新の時刻に揃えます。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     *  @ref GetCurrentTime 関数で得られる時刻を、システムの最新時刻に追従させます。@n
     *  本関数の実行は @ref GetCurrentTime 関数 の成功を保証するものではありません。
     *
     *  本関数の実行によって @ref GetCurrentTime 関数で得られる時刻に、巻き戻りや大きな進みが発生することがあります。
     */
    static void Adjust() NN_NOEXCEPT;

    /**
     * @brief   クロックが利用しているコンテキストの値を取得します。
     * @param[out]  pOutSystemClockContext    コンテキストを代入するバッファへのポインタを指定します。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *  - pOutSystemClockContext != nullptr
     *
     * @details
     *  システム側の時計が更新されたかどうかは、この関数で得られるコンテキストと、
     *  @ref nn::time::StandardNetworkSystemClock::GetSystemClockContext()
     *  で得られるコンテキストとを比較することで知ることができます。
     *
     *  @ref Adjust 関数の実行によって、本関数の返り値が変更されることがあります。
     */
    static void GetSystemClockContext(nn::time::SystemClockContext* pOutSystemClockContext) NN_NOEXCEPT;

//! @name C++ 標準インターフェース
//! @{

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
     * @return  このクロックが指し示す現在時刻を nn::time::AdjustableNetworkSystemClock::time_point 型で返します。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     *  時計が無効な場合、1970年1月1日0時0分0秒を表す nn::time::AdjustableNetworkSystemClock::time_point 型の値が返ります。
     *
     * @platformbegin{Windows}
     *  Windows 環境では PC 時計と同等の時刻が返されます。
     * @platformend
     */
    static time_point now() NN_NOEXCEPT;

    /**
     * @brief   nn::time::AdjustableNetworkSystemClock::time_point 型の時刻を std::time_t 型に変換します。
     *
     * @param[in]   t   nn::time::AdjustableNetworkSystemClock::time_point 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す std::time_t 型の値が返ります。
     */
    static std::time_t to_time_t(const time_point& t) NN_NOEXCEPT;

    /**
     * @brief   std::time_t 型の時刻を nn::time::AdjustableNetworkSystemClock::time_point 型に変換します。
     *
     * @param[in]   t   std::time_t 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す nn::time::AdjustableNetworkSystemClock::time_point 型の値が返ります。
     *
     * @details
     *  nn::time::AdjustableNetworkSystemClock::time_point 型の時刻の分解能が std::time_t 型のそれより低い場合、 nn::time::AdjustableNetworkSystemClock::time_point 型の分解能に丸められます。
     */
    static time_point from_time_t(std::time_t t) NN_NOEXCEPT;

//! @}

};

}
}
