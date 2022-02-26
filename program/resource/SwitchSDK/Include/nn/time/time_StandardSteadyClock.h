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
*   @brief  SoC 外部に電源を持つ単調増加クロックのクラス宣言
*/

#pragma once

#include <nn/time/time_Common.h>

#include <nn/time/time_Types.h>

#include <chrono>
#include <ctime>


namespace nn
{
namespace time
{

/**
 * @brief   SoC 外部に電源を持つ単調増加クロックです。
 *
 * @details
 *  このクロックの指し示す値は人間の理解できる日時の表現に変換することはできませんが、ユーザーやシステムによる時刻の操作に関わらず一定間隔でカウントアップされるため、時刻の変更・補正の影響を受けずに時間経過を計算することができます。
 *  また、アプリケーションの終了後やデバイスの電源が落ちている状態でも継続してカウントアップされるため、アプリケーションの終了と起動をまたいで経過時間を計算することができます。@n
 *
 *  電池切れにより電源の供給が途絶えた場合、クロックの指し示す値はリセットされます。
 *  このとき、リセットが発生したことは @ref nn::time::GetSpanBetween 関数の結果から検出できますが、リセットをまたいでの時間経過の計算をおこなうことはできません。@n
 *
 *  アプリケーションの終了と起動をまたがない条件下であれば、 @ref nn::os::GetSystemTick 関数から取得できるシステムチック値によって、より分解能の高い時間経過の計算が可能です。@n
 *
 *  このクロックのソースはハードウェア的な改竄攻撃に対して強靭ではないことに注意してください。@n
 *
 *  このクラスは C++ 標準の TrivialClock の要件を満たしており、 std::steady_clock に準ずる機能を提供しますが、その保証は値のリセットが発生しなかった場合に限られます。
 *  アプリケーションの終了など、クロックへの電源の供給が途絶える可能性のある状況をまたぐ場合はその限りでないことに注意してください。
 *  また C++ 標準のインターフェースのみを利用している場合、値のリセットが発生したことは検知できません。
 *
 *  ### @anchor standard_steady_clock_accuracy 単調増加クロックの累積誤差について
 *  単調増加クロックのカウントアップは、実際の時間の進みと比べて月に数分程度の誤差が累積する可能性があります。
 *  この誤差はデバイスの利用状況や個体差によって増減があります。
 *
 *  @ref nn::time::StandardUserSystemClock, @ref nn::time::StandardNetworkSystemClock は
 *  この単調増加クロックのカウントアップを利用して時間を進めているため、上述の累積誤差がそれらクロックの精度にも影響します。
 */
class StandardSteadyClock
{
public:
    /**
     * @brief   単調増加クロックの指し示す現在の値を取得します。
     *
     * @param[out]  pOutSteadyClockTimePoint    単調増加クロック値を代入するバッファへのポインタを指定します。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *  - pOutSteadyClockTimePoint != nullptr
     *
     * @details
     *
     * @see nn::time::GetSpanBetween()
     */
    static Result GetCurrentTimePoint(SteadyClockTimePoint* pOutSteadyClockTimePoint) NN_NOEXCEPT;

    /**
     * @brief   このクロックから取得・計算される時刻・時間の内部表現となる算術型を表します。
     *
     * @details
     */
    typedef int64_t rep;

    /**
     * @brief   このクロックの持つ分解能を std::ratio 型で表したものです。
     *
     * @details
     *  1単位当たりの秒数で表されています。
     */
    typedef std::ratio<1> period;

    /**
     * @brief   経過時間を表す型です。
     *
     * @details
     */
    typedef std::chrono::duration<rep, period> duration;

    /**
     * @brief   このクロックから取得される、時間の流れの中の一点を指す型です。
     *
     * @details
     *  この型で表された値は、起点となる時点が自明でないため、人間の理解する日時表現や std::time_t 型と単体で相互変換することはできません。
     */
    typedef std::chrono::time_point<StandardSteadyClock> time_point;

    /**
     * @brief   このクロックの単調増加性を表す値です。
     *
     * @details
     *  値としては true ですが、これは電池切れなどで電源供給が途絶えることがない範囲内での保証であることに注意してください。
     */
    static const bool is_steady = true;

    /**
     * @brief   このクロックの指し示す現在時刻を取得します。
     *
     * @return  このクロックが指し示す現在時刻を nn::time::StandardSteadyClock::time_point 型で返します。
     *
     * @pre
     *  - TIME ライブラリが初期化されている
     *
     * @details
     *  本関数の返り値では時計がリセットされたかどうかを検知することはできず、
     *  時計がリセットされた場合には値が巻き戻ることがあるので注意してください。
     *  @n
     *  なお、時計のリセットはアプリケーション起動中に発生することはありません。
     */

    static time_point now() NN_NOEXCEPT;
};

}
}
