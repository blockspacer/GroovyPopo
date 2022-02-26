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
*   @brief  TIME ライブラリの初期化/終了やユーティリティ関数の API 宣言
*/

#pragma once

#include <nn/time/time_Common.h>

#include <nn/time/time_Types.h>


namespace nn
{
namespace time
{

//! @name 初期化と終了処理
//! @{

    /**
     * @brief   TIME ライブラリの初期化をおこない、機能を使用可能な状態にします。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     *
     * @details
     *  TIME ライブラリが提供する API は原則として、利用前にこの関数でライブラリを初期化しておく必要があります。@n
     *
     *  ライブラリがすでに初期化された状態でも重ねて呼ぶことができますが、その場合、ライブラリの利用を完全に終了するにはこの関数を呼んだのと同じ回数だけ
     *  @ref nn::time::Finalize 関数を呼ぶ必要があります。
     */
    Result Initialize() NN_NOEXCEPT;

    /**
     * @brief   TIME ライブラリが初期化済みかを判定します。
     *
     * @return  ライブラリがすでに初期化されていれば true を、まだ初期化されていなければ false を返します。
     *
     * @details
     */
    bool IsInitialized() NN_NOEXCEPT;

    /**
     * @brief   TIME ライブラリの利用を終了します。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     *
     * @details
     *  @ref nn::time::Initialize 関数を複数回呼んでいる場合、TIME ライブラリの利用を完全に終了するには同じ回数だけこの関数を呼ぶ必要があります。@n
     *
     *  ライブラリの利用が完全に終了されると、システムで確保していたリソースが解放ます。@n
     *
     *  ライブラリが未初期化の状態でコールしても副作用はありません。
     */
    Result Finalize() NN_NOEXCEPT;
//! @}

//! @name ユーティリティ API
//! @{

    /**
     * @brief   2つの nn::time::SteadyClockTimePoint 間の経過秒数を計算します。
     *
     * @param[out]  pOutSeconds     計算結果の秒数を格納するバッファへのポインタを指定します。
     * @param[in]   from            起点となる時刻を指定します。
     * @param[in]   to              終点となる時刻を指定します。
     *
     * @return  処理の結果が返ります。事前条件を満たしていない場合、以下に列挙されていない失敗が返ることがあります。
     * @retval  ResultSuccess                       成功しました。
     * @retval  ResultNotComparable                 与えられた2つの値に連続性がなく、比較・減算できません。
     * @retval  ResultOverflowed                    計算結果がオーバーフローもしくはアンダーフローしました。
     *
     * @pre
     *  - pOutSeconds != nullptr
     *
     * @details
     *  終点が起点より過去を指していた場合、経過秒数として負の値が返ります。
     *
     *  @ref nn::time::StandardSteadyClock はハードウェア的な改竄攻撃に対して強靭ではありません。
     *  経過時間がマイナスなどの想定外の値になったとしても、アプリケーションにはまりや止まりがないように実装してください。
     *
     *  より多くのケースで経過時間の計算が可能な @ref nn::time::CalculateSpanBetween 関数も参照してください。
     */
    Result GetSpanBetween(int64_t* pOutSeconds, const SteadyClockTimePoint& from, const SteadyClockTimePoint& to) NN_NOEXCEPT;

    /**
     * @brief       指定した西暦がうるう年かどうかを判定します。
     * @param[in]   year    年
     *
     * @pre
     *  - 1 <= year
     *
     * @return      うるう年かどうか
     * @retval      true    指定した西暦がうるう年であった。
     * @retval      false   指定した西暦がうるう年ではなかった。
     *
     * @details
     */
    bool IsLeapYear(int year) NN_NOEXCEPT;

    /**
     * @brief       指定した年月日が有効かどうかをチェックします。
     * @param[in]   year    年
     * @param[in]   month   月
     * @param[in]   day     日
     *
     * @return  指定した年月日が有効かどうか
     * @retval  true    有効な年月日であった。
     * @retval  false   有効でない年月日であった。
     *
     * @details
     *  year が 0 以下の場合は必ず false が返ります。
     */
    bool IsValidDate(int year, int month, int day) NN_NOEXCEPT;

    /**
     * @brief       年月を指定してその月が何日あるかを取得します。
     * @param[in]   year    年
     * @param[in]   month   月
     *
     * @pre
     *  - 1 <= year
     *  - 1 <= month <= 12
     *
     * @post
     *  - 1 <= 返り値 <= 31
     *
     * @return      指定した月の日数
     * @details
     */
    int GetDaysInMonth(int year, int month) NN_NOEXCEPT;

    /**
     * @brief 年月日を 西暦1年1月1日 からの経過日数へ変換します。
     *
     * @param[in]  year    年
     * @param[in]  month   月
     * @param[in]  day     日
     *
     * @pre
     *  - 1 <= year
     *  - 1 <= month
     *  - 1 <= day
     *
     * @post
     *  - 0 <= 返り値
     *
     * @return      西暦1年1月1日 からの経過日数
     * @details
     *  指定年月日が何日目かではなく、経過日数を返すことに注意してください。
     *  例えば 西暦1年1月1日 の場合は 0 が返ります。
     *
     *  有効でない日付を指定した場合には、有効な日付からの経過を考慮して日付が補正されます。
     *  例えば有効でない 西暦2016年4月31日 を入力した場合、有効な 西暦2016年4月30日 から1日後の 西暦2016年5月1日 として扱われます。
     *
     * @see DaysToDate()
     */
    int DateToDays(int year, int month, int day) NN_NOEXCEPT;

    /**
     * @brief 西暦1年1月1日 からの経過日数を年月日へ変換します。
     *
     * @param[out]  pOutYear    年が入るアドレスを指定してください
     * @param[out]  pOutMonth   月が入るアドレスを指定してください
     * @param[out]  pOutDay     日が入るアドレスを指定してください
     * @param[in]   days        西暦1年1月1日 からの経過日数
     *
     * @pre
     *  - 0 <= days
     *
     * @post
     *  - 1 <= *pOutYear
     *  - 1 <= *pOutMonth <= 12
     *  - 1 <= *pOutDay <= 31
     *
     * @details
     *  days に指定するのは何日目かではなく、経過日数であることに注意してください。
     *  例えば days に 0 を指定した場合は 西暦1年1月1日 が出力されます。
     *
     * @see DateToDays()
     */
    void DaysToDate(int *pOutYear, int *pOutMonth, int *pOutDay, int days) NN_NOEXCEPT;

    /**
     * @brief   年月日を指定して曜日を取得します。
     *
     * @param[in]  year    年
     * @param[in]  month   月
     * @param[in]  day     日
     *
     * @pre
     *  - 1 <= year
     *  - 1 <= month
     *  - 1 <= day
     *
     * @return      曜日
     * @details
     *  有効でない日付を指定した場合には、有効な日付からの経過を考慮して日付が補正され、曜日が計算されます。
     *  例えば有効でない 西暦2016年4月31日 を入力した場合、有効な 西暦2016年4月30日 から1日後の 西暦2016年5月1日 の曜日が返ります。
     */
    DayOfWeek GetDayOfWeek(int year, int month, int day) NN_NOEXCEPT;

//! @}
}
}
