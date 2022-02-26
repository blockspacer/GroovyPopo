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
 * @brief   TimeSpan クラスの宣言
 */

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_CHRONO)
#include <chrono>
#endif

//----------------------------------------------------------------------------

namespace nn {

//----------------------------------------------------------------------------
/**
 * @brief 時間間隔オブジェクトを定義する構造体です。
 *
 * @details
 *  時間の長さを表すオブジェクトです。@n
 *  値は 64bit 整数かつナノ秒単位で保持されます。オブジェクトの生成には
 *  FromNanoSeconds() などのコンストラクタを使用してください。
 */
struct TimeSpanType
{
public:

//! @name コンストラクタ
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   ナノ秒数から時間間隔オブジェクトを生成します。
     *
     * @param[in] nanoSeconds   時間の長さをナノ秒数で指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromNanoSeconds(int64_t nanoSeconds) NN_NOEXCEPT
    {
        TimeSpanType ret;
        ret._nanoSeconds = nanoSeconds;
        return ret;
    }

    //------------------------------------------------------------------------
    /**
     * @brief   マイクロ秒数から時間間隔オブジェクトを生成します。
     *
     * @param[in] microSeconds  時間の長さをマイクロ秒数で指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromMicroSeconds(int64_t microSeconds) NN_NOEXCEPT
    {
        return FromNanoSeconds(microSeconds * 1000);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   ミリ秒数から時間間隔オブジェクトを生成します。
     *
     * @param[in] milliSeconds  時間の長さをミリ秒数で指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromMilliSeconds(int64_t milliSeconds) NN_NOEXCEPT
    {
        return FromNanoSeconds(milliSeconds * 1000 * 1000);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   秒数から時間間隔オブジェクトを生成します。
     *
     * @param[in] seconds       時間の長さを秒数で指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromSeconds(int64_t seconds) NN_NOEXCEPT
    {
        return FromNanoSeconds(seconds * 1000 * 1000 * 1000);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   分数から時間間隔オブジェクトを生成します。
     *
     * @param[in] minutes       時間の長さを分数で指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromMinutes(int64_t minutes) NN_NOEXCEPT
    {
        return FromNanoSeconds(minutes * 1000 * 1000 * 1000 * 60);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間数から時間間隔オブジェクトを生成します。
     *
     * @param[in] hours         時間の長さを時数で指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromHours(int64_t hours) NN_NOEXCEPT
    {
        return FromNanoSeconds(hours * 1000 * 1000 * 1000 * 60 * 60);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   日数から時間間隔オブジェクトを生成します。
     *
     * @param[in] days          時間(日数)を指定します。
     *
     * @return  指定された時間の長さを表す時間間隔オブジェクトを返します。
     */
    static TimeSpanType FromDays(int64_t days) NN_NOEXCEPT
    {
        return FromNanoSeconds(days * 1000 * 1000 * 1000 * 60 * 60 * 24);
    }

//! @}

//! @name 特定単位での値の取得
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   時間を日数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さを日数に換算した値を
     *          返します。
     */
    int64_t GetDays() const NN_NOEXCEPT
    {
        return DivideNanoSeconds(this->_nanoSeconds, 0x683fff6f48f948e3LL, 45);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間を時数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さを時数に換算した値を
     *          返します。
     */
    int64_t GetHours() const NN_NOEXCEPT
    {
        return DivideNanoSeconds(this->_nanoSeconds, 0x9c5fff26ed75ed55LL, 41);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間を分数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さを分数に換算した値を
     *          返します。
     */
    int64_t GetMinutes() const NN_NOEXCEPT
    {
        return DivideNanoSeconds(this->_nanoSeconds, 0x12533fe68fd3d1dLL, 28);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間を秒数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さを秒数に換算した値を
     *          返します。
     */
    int64_t GetSeconds() const NN_NOEXCEPT
    {
        return DivideNanoSeconds(this->_nanoSeconds, 0x112e0be826d694b3LL, 26);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間をミリ秒数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さをミリ秒数に換算した値を
     *          返します。
     */
    int64_t GetMilliSeconds() const NN_NOEXCEPT
    {
        return DivideNanoSeconds(this->_nanoSeconds, 0x431bde82d7b634dbLL, 18);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間をマイクロ秒数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さをマイクロ秒数に換算した
     *          値を返します。
     */
    int64_t GetMicroSeconds() const NN_NOEXCEPT
    {
        return DivideNanoSeconds(this->_nanoSeconds, 0x20c49ba5e353f7cfLL, 7);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間をナノ秒数で取得します。
     *
     * @return  この時間間隔オブジェクトが表す時間の長さをナノ秒数に換算した値を
     *          返します。
     */
    int64_t GetNanoSeconds() const NN_NOEXCEPT
    {
        return this->_nanoSeconds;
    }

//! @}

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが一致しているか否かを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  一致しているなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator==(const TimeSpanType& lhs,
                           const TimeSpanType& rhs) NN_NOEXCEPT
    {
        return lhs._nanoSeconds == rhs._nanoSeconds;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが不一致か否かを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  不一致なら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator!=(const TimeSpanType& lhs,
                           const TimeSpanType& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが短いかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より短いなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator< (const TimeSpanType& lhs,
                           const TimeSpanType& rhs) NN_NOEXCEPT
    {
        return lhs._nanoSeconds < rhs._nanoSeconds;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが長いかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より長いなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator> (const TimeSpanType& lhs,
                           const TimeSpanType& rhs) NN_NOEXCEPT
    {
        return rhs < lhs;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが短いまたは等しいかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より短いかまたは等しければ true そうでなければ
     *          false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator<=(const TimeSpanType& lhs,
                           const TimeSpanType& rhs) NN_NOEXCEPT
    {
        return !(lhs > rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが長いまたは等しいかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より長いかまたは等しければ true そうでなければ
     *          false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator>=(const TimeSpanType& lhs,
                           const TimeSpanType& rhs) NN_NOEXCEPT
    {
        return !(lhs < rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さを加えます。
     *
     * @param[in] rhs   加える時間の長さ
     *
     * @return  自身へのリファレンスを返します。
     */
    //----------------------------------------------------------------------
    TimeSpanType& operator+=(const TimeSpanType& rhs) NN_NOEXCEPT
    {
        this->_nanoSeconds += rhs._nanoSeconds;
        return *this;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さの和を求めます。
     *
     * @param[in] lhs   和を取る時間
     * @param[in] rhs   和を取る時間
     *
     * @return  lhs と rhs の時間の長さの和と同じ時間の長さを持つ
     *          TimeSpanType を返します。
     */
    //----------------------------------------------------------------------
    friend TimeSpanType operator+(const TimeSpanType& lhs,
                                  const TimeSpanType& rhs) NN_NOEXCEPT
    {
        TimeSpanType ret(lhs);
        return ret += rhs;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さを引きます。
     *
     * @param[in] rhs   差し引く時間の長さ
     *
     * @return  自身へのリファレンスを返します。
     */
    //----------------------------------------------------------------------
    TimeSpanType& operator-=(const TimeSpanType& rhs) NN_NOEXCEPT
    {
        this->_nanoSeconds -= rhs._nanoSeconds;
        return *this;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さの差を求めます。
     *
     * @param[in] lhs   差を取る時間
     * @param[in] rhs   差を取る時間
     *
     * @return  lhs と rhs の時間の長さの差と同じ時間の長さを持つ
     *          TimeSpanType を返します。
     */
    //----------------------------------------------------------------------
    friend TimeSpanType operator-(const TimeSpanType& lhs,
                                  const TimeSpanType& rhs) NN_NOEXCEPT
    {
        TimeSpanType ret(lhs);
        return ret -= rhs;
    }

//----------------------------------------------------------------------------
public:
    int64_t _nanoSeconds;

//----------------------------------------------------------------------------
private:
    static int64_t DivideNanoSeconds(const int64_t nanoSeconds,
                                     const int64_t magic,
                                     const int32_t rightShift) NN_NOEXCEPT
    {
        int64_t n = MultiplyRightShift(nanoSeconds, magic);
        if (magic < 0)
        {
            n += nanoSeconds;
        }
        n >>= rightShift;
        return n + (static_cast<uint64_t>(nanoSeconds) >> 63);
    }

    static int64_t MultiplyRightShift(const int64_t x,
                                      const int64_t y) NN_NOEXCEPT
    {
        const uint64_t x_lo = x & 0xffffffff;
        const int64_t  x_hi = x >> 32;

        const uint64_t y_lo = y & 0xffffffff;
        const int64_t  y_hi = y >> 32;

        const int64_t  z    = x_hi * y_lo + ((x_lo * y_lo) >> 32);
        const int64_t  z_lo = z & 0xffffffff;
        const int64_t  z_hi = z >> 32;

        return x_hi * y_hi + z_hi +
               (static_cast<int64_t>(x_lo * y_hi + z_lo) >> 32);
    }
};

//----------------------------------------------------------------------------
/**
 * @brief 時間の長さを表すクラスです。
 *
 * @details
 *  このクラスでは、時間の長さを 64bit 整数かつナノ秒単位で管理しています。@n
 *  時間を指定する関数に対しては、単位を間違えないためにも
 *  このクラスを使うようになっています。
 *
 *  このクラスのインスタンスは、 FromNanoSeconds() などの From～() 関数を
 *  使うことで各単位で表された整数値から作成することができます。@n
 *  単位が曖昧になるのを防ぐため、整数からこの型への暗黙的な変換は
 *  用意されていませんが、0 だけはこの型への暗黙的な変換が可能です。
 */
class TimeSpan
{
private:
    typedef const class ZeroOnlyTag {} * ZeroOnly;

public:

//! @name コンストラクタ
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   コンストラクタです。
     *
     * @param[in] zeroOnly      引数を省略するか、0 を指定してください。
     *
     * @details
     *  時間を 0 で初期化するコンストラクタです。@n
     *  引数を省略した場合、デフォルトコンストラクタとなり、
     *  時間 0 で初期化されます。@n
     *  引数付きのこのコンストラクタは、時間を指定するべき場所で
     *  0 を指定できるようにするために用意されています。@n
     *  0 以外の引数を与えることはできません。
     */
    NN_IMPLICIT TimeSpan(ZeroOnly zeroOnly = 0) NN_NOEXCEPT
        : m_TimeSpan(TimeSpanType::FromNanoSeconds(0))
    {
        NN_UNUSED(zeroOnly);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   コンストラクタです。
     *
     * @param[in] timeSpan  TimeSpanType で表される時間
     *
     * @details
     *  時間を、 TimeSpanType で表される時間で初期化するコンストラクタです。@n
     */
    NN_IMPLICIT TimeSpan(const TimeSpanType& timeSpan) NN_NOEXCEPT
        : m_TimeSpan(timeSpan) {}

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_CHRONO) || \
    defined(NN_BUILD_FOR_DOCUMENT_GENERATION)
    //------------------------------------------------------------------------
    /**
     * @brief   コンストラクタです。
     *
     * @tparam    Rep       時間の値を格納する数値型
     * @tparam    Period    時間の単位を表す単位型
     * @param[in] timeValue std::chrono::duration で表される時間
     *
     * @details
     *  時間を、 std::chrono::duration で表される時間で
     *  初期化するコンストラクタです。@n
     */
    template<typename Rep, typename Period>
    NN_IMPLICIT TimeSpan(const std::chrono::duration<Rep, Period>& timeValue) NN_NOEXCEPT
        : m_TimeSpan(TimeSpanType::FromNanoSeconds(static_cast<std::chrono::nanoseconds>(timeValue).count())) {}
#endif

    //------------------------------------------------------------------------
    /**
     * @brief   ナノ秒数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] nanoSeconds   時間の長さをナノ秒数で指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromNanoSeconds(int64_t nanoSeconds) NN_NOEXCEPT
    {
        return TimeSpanType::FromNanoSeconds(nanoSeconds);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   マイクロ秒数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] microSeconds  時間の長さをマイクロ秒数で指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromMicroSeconds(int64_t microSeconds) NN_NOEXCEPT
    {
        return TimeSpanType::FromMicroSeconds(microSeconds);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   ミリ秒数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] milliSeconds  時間の長さをミリ秒数で指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromMilliSeconds(int64_t milliSeconds) NN_NOEXCEPT
    {
        return TimeSpanType::FromMilliSeconds(milliSeconds);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   秒数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] seconds       時間の長さを秒数で指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromSeconds(int64_t seconds) NN_NOEXCEPT
    {
        return TimeSpanType::FromSeconds(seconds);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   分数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] minutes       時間の長さを分数で指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromMinutes(int64_t minutes) NN_NOEXCEPT
    {
        return TimeSpanType::FromMinutes(minutes);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] hours         時間の長さを時数で指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromHours(int64_t hours) NN_NOEXCEPT
    {
        return TimeSpanType::FromHours(hours);
    }

    //------------------------------------------------------------------------
    /**
     * @brief   日数から TimeSpan オブジェクトを生成します。
     *
     * @param[in] days          時間(日数)を指定します。
     *
     * @return  指定された時間の長さを表す TimeSpan オブジェクトを返します。
     */
    static TimeSpan FromDays(int64_t days) NN_NOEXCEPT
    {
        return TimeSpanType::FromDays(days);
    }

//! @}

//! @name 特定単位での値の取得
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   時間を日数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さを日数に換算した値を
     *          返します。
     */
    int64_t GetDays() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetDays();
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間を時数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さを時数に換算した値を
     *          返します。
     */
    int64_t GetHours() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetHours();
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間を分数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さを分数に換算した値を
     *          返します。
     */
    int64_t GetMinutes() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetMinutes();
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間を秒数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さを秒数に換算した値を
     *          返します。
     */
    int64_t GetSeconds() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetSeconds();
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間をミリ秒数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さをミリ秒数に換算した
     *          値を返します。
     */
    int64_t GetMilliSeconds() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetMilliSeconds();
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間をマイクロ秒数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さをマイクロ秒数に
     *          換算した値を返します。
     */
    int64_t GetMicroSeconds() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetMicroSeconds();
    }

    //------------------------------------------------------------------------
    /**
     * @brief   時間をナノ秒数で取得します。
     *
     * @return  この TimeSpan オブジェクトが表す時間の長さをナノ秒数に換算した
     *          値を返します。
     */
    int64_t GetNanoSeconds() const NN_NOEXCEPT
    {
        return m_TimeSpan.GetNanoSeconds();
    }

//! @}

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが一致しているか否かを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  一致しているなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator==(const TimeSpan& lhs, const TimeSpan& rhs) NN_NOEXCEPT
    {
        return lhs.m_TimeSpan == rhs.m_TimeSpan;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが不一致か否かを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  不一致なら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator!=(const TimeSpan& lhs, const TimeSpan& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが短いかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より短いなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator< (const TimeSpan& lhs, const TimeSpan& rhs) NN_NOEXCEPT
    {
        return lhs.m_TimeSpan < rhs.m_TimeSpan;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが長いかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より長いなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator> (const TimeSpan& lhs, const TimeSpan& rhs) NN_NOEXCEPT
    {
        return rhs < lhs;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが短いまたは等しいかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より短いかまたは等しければ true そうでなければ
     *          false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator<=(const TimeSpan& lhs, const TimeSpan& rhs) NN_NOEXCEPT
    {
        return !(lhs > rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さが長いまたは等しいかを判断します。
     *
     * @param[in] lhs   比較する時間
     * @param[in] rhs   比較する時間
     *
     * @return  lhs が rhs より長いかまたは等しければ true そうでなければ
     *          false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator>=(const TimeSpan& lhs, const TimeSpan& rhs) NN_NOEXCEPT
    {
        return !(lhs < rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さを加えます。
     *
     * @param[in] rhs   加える時間の長さ
     *
     * @return  自身へのリファレンスを返します。
     */
    //----------------------------------------------------------------------
    TimeSpan& operator+=(const TimeSpan& rhs) NN_NOEXCEPT
    {
        this->m_TimeSpan += rhs.m_TimeSpan;
        return *this;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さの和を求めます。
     *
     * @param[in] lhs   和を取る時間
     * @param[in] rhs   和を取る時間
     *
     * @return  lhs と rhs の時間の長さの和と同じ時間の長さを持つ TimeSpan を
     *          返します。
     */
    //----------------------------------------------------------------------
    friend TimeSpan operator+(const TimeSpan& lhs,
                              const TimeSpan& rhs) NN_NOEXCEPT
    {
        TimeSpan ret(lhs);
        return ret += rhs;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さを引きます。
     *
     * @param[in] rhs   差し引く時間の長さ
     *
     * @return  自身へのリファレンスを返します。
     */
    //----------------------------------------------------------------------
    TimeSpan& operator-=(const TimeSpan& rhs) NN_NOEXCEPT
    {
        this->m_TimeSpan -= rhs.m_TimeSpan;
        return *this;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   時間の長さの差を求めます。
     *
     * @param[in] lhs   差を取る時間
     * @param[in] rhs   差を取る時間
     *
     * @return  lhs と rhs の時間の長さの差と同じ時間の長さを持つ TimeSpan を
     *          返します。
     */
    //----------------------------------------------------------------------
    friend TimeSpan operator-(const TimeSpan& lhs,
                              const TimeSpan& rhs) NN_NOEXCEPT
    {
        TimeSpan ret(lhs);
        return ret -= rhs;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   TimeSpanType オブジェクトを返します。
     *
     * @return  自身が持つ TimeSpanType オブジェクトを返します。
     */
    NN_IMPLICIT operator TimeSpanType() const NN_NOEXCEPT
    {
        return m_TimeSpan;
    }

//----------------------------------------------------------------------------
private:
    TimeSpanType    m_TimeSpan;
};

//----------------------------------------------------------------------------

}   // namespace nn
