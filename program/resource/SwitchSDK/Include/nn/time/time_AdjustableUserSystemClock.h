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
 * @brief   システム側時刻に任意のタイミングで揃えることが可能な、ユーザー時計を表すクロックのクラス宣言
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
 * @brief   システム側時刻に任意のタイミングで揃えることが可能な、ユーザー時計を表すクロックです。
 *
 * @details
 *  このクロックはデバイスが持つ現在時刻を指し示し、人間の理解可能な日時表現に変換できる値を取得することができます。@n
 *  @ref Adjust 関数を実行しない限りはユーザーやシステムによる時計の変更の影響を受けることがなく、
 *  アプリケーションでの扱いが容易な、時刻の巻き戻りや大きな進みの発生を制御できるクロックです。
 *
 *  このクロックは @ref Adjust 関数の実行により、
 *  @ref Adjust 関数の実行によりシステム側( @ref nn::time::StandardUserSystemClock ) の時刻に揃えることができ、
 *  その際指し示す日時が実際の時間経過以上に進んだり過去へ巻き戻ったりすることがあります。
 *  また、デバイスの電源が落ちているときもカウントアップされますが、電池切れにより電源の供給が途絶えた場合は値がリセットされます。
 *  このとき、時計の操作やリセットの発生したことは検出できますが、それらをまたいでおこなわれた時間経過の計算結果は信頼できません。
 *
 *  メンバ関数はスレッドセーフです。@n
 *  このクラスは C++ 標準の TrivialClock の要件を満たしており、 std::chrono::system_clock に準ずる機能を提供します。
 *  ただし C++ 標準のインターフェースのみを利用している場合、時計の操作や値のリセットが発生したことは検知できません。
 */
class AdjustableUserSystemClock
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
     *  得られる時刻はシステムの最新時刻とは異なる可能性があることに注意してください。
     *
     *  得られる時刻をシステムの最新時刻に追従させるには @ref Adjust 関数を実行します。
     */
    static Result GetCurrentTime(PosixTime* pOutPosixTime) NN_NOEXCEPT;

    /**
     * @brief   クロックの時刻をシステム側で保持する最新の時刻に揃えます。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     *  @ref GetCurrentTime 関数で得られる時刻を、システムの最新時刻に追従させます。
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
     *  @ref nn::time::StandardUserSystemClock::GetSystemClockContext()
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
     *  ユーザー操作やシステムによる自動補正により、このクロックが指す時刻は逆行する可能性があります。
     */
    static const bool is_steady = false;

    /**
     * @brief   このクロックの指し示す現在時刻を取得します。
     *
     * @return  このクロックが指し示す現在時刻を nn::time::AdjustableUserSystemClock::time_point 型で返します。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     */
    static time_point now() NN_NOEXCEPT;

    /**
     * @brief   nn::time::AdjustableUserSystemClock::time_point 型の時刻を std::time_t 型に変換します。
     *
     * @param[in]   t   nn::time::AdjustableUserSystemClock::time_point 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す std::time_t 型の値が返ります。
     */
    static std::time_t to_time_t(const time_point& t) NN_NOEXCEPT;

    /**
     * @brief   std::time_t 型の時刻を nn::time::AdjustableUserSystemClock::time_point 型に変換します。
     *
     * @param[in]   t   std::time_t 型での時刻の値を指定します。
     *
     * @return  入力と同じ時刻を指す nn::time::AdjustableUserSystemClock::time_point 型の値が返ります。
     *
     * @details
     *  nn::time::AdjustableUserSystemClock::time_point 型の時刻の分解能が std::time_t 型のそれより低い場合、 nn::time::AdjustableUserSystemClock::time_point 型の分解能に丸められます。
     */
    static time_point from_time_t(std::time_t t) NN_NOEXCEPT;

//! @}

};

}
}
