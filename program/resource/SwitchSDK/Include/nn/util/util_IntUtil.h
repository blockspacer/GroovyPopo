/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <type_traits>
#include <limits>

namespace nn { namespace util {

/**
    @brief 引数で指定した整数値が、指定した型で表現できるかどうかを取得します。

    @tparam Destination 変換先の整数型を指定します。
    @tparam Source 変換元の整数型を指定します。通常は省略可能です。

    @param[in] n 変換元の整数値を指定します。
    @return 整数値 n を、整数型 Destination で表現できれば true を、そうでなければ false を返します。
*/
template <typename Destination, typename Source>
bool IsIntValueRepresentable(Source n) NN_NOEXCEPT;


/**
    @brief オーバーフローを考慮して整数値の足し算(x + y)が可能かを判定します。

    @tparam Int 処理の対象となる整数型を指定します。

    @param[in] x 足し算を行う片方の値を指定します。
    @param[in] y 足し算を行うもう片方の値を指定します。

    @return 足し算がオーバーフローなく可能であれば true を、そうでなければ false を返します。

    @see nn::util::TryAddWithoutOverflow()
 */
template <typename Int>
bool CanAddWithoutOverflow(Int x, Int y) NN_NOEXCEPT;

/**
    @brief オーバーフローを考慮して整数値の引き算(x - y)が可能かを判定します。

    @tparam Int 処理の対象となる整数型を指定します。

    @param[in] x 引かれる側の値を指定します。
    @param[in] y 引く側の値を指定します。

    @return 引き算がオーバーフローなく可能であれば true を、そうでなければ false を返します。

    @see nn::util::TrySubtractWithoutOverflow()
 */
template <typename Int>
bool CanSubtractWithoutOverflow(Int x, Int y) NN_NOEXCEPT;

/**
    @brief オーバーフローを考慮して整数値の足し算(x + y)を行います。

    @tparam Int 処理の対象となる整数型を指定します。

    @param[out] pOut 処理結果の格納先を指定します。
    @param[in] x 足し算を行う片方の値を指定します。
    @param[in] y 足し算を行うもう片方の値を指定します。

    @pre
        - pOut != nullptr

    @post
        - nn::util::CanAddWithoutOverflow(x, y) == true であれば *pOut == x + y

    @return nn::util::CanAddWithoutOverflow(x, y) を返します。

    @details
      x + y がオーバーフローなく計算可能であれば、計算結果を pOut へ格納します。
 */
template <typename Int>
bool TryAddWithoutOverflow(Int* pOut, Int x, Int y) NN_NOEXCEPT;

/**
    @brief オーバーフローを考慮して整数値の引き算(x - y)を行います。

    @tparam Int 処理の対象となる整数型を指定します。

    @param[out] pOut 処理結果の格納先を指定します。
    @param[in] x 引かれる側の値を指定します。
    @param[in] y 引く側の値を指定します。

    @pre
        - pOut != nullptr

    @post
        - nn::util::CanSubtractWithoutOverflow(x, y) == true であれば *pOut == x - y

    @return nn::util::CanSubtractWithoutOverflow(x, y) を返します。

    @details
      x - y がオーバーフローなく計算可能であれば、計算結果を pOut へ格納します。
 */
template <typename Int>
bool TrySubtractWithoutOverflow(Int* pOut, Int x, Int y) NN_NOEXCEPT;

namespace detail {

    // Destination と Source の符号によって SIFNAE で分岐
    template <typename Destination, typename Source, typename = void>
    struct IsIntValueRepresentableImpl;

    // 符号の有無が一致しているとき
    // そのまま範囲の比較をして返す。
    // ビット数が違う場合にも、小さいほうがプロモーションされるため問題ない。
    template <typename Destination, typename Source>
    struct IsIntValueRepresentableImpl<Destination, Source, typename std::enable_if<
        std::is_signed<Destination>::value == std::is_signed<Source>::value
    >::type>
    {
        static bool Get(Source n) NN_NOEXCEPT
        {
            typedef std::numeric_limits<Destination> dlimit;
            typedef std::numeric_limits<Source> slimit;
            if ((dlimit::min)() <= (slimit::min)() && (slimit::max)() <= (dlimit::max)())
            {
                return true;
            }
            else
            {
                return (dlimit::min)() <= n && n <= (dlimit::max)();
            }
        }
    };

    // Destination が unsigned で、Source が singned のとき
    // n が負のとき false を返す。
    // n が非負のとき、対応する unsingned 型に変換した後、符号の有無が一致しているときの特殊化に委譲する。
    template <typename Destination, typename Source>
    struct IsIntValueRepresentableImpl<Destination, Source, typename std::enable_if<
        !std::is_signed<Destination>::value && std::is_signed<Source>::value
    >::type>
    {
        static bool Get(Source n) NN_NOEXCEPT
        {
            if (n < 0)
            {
                return false;
            }
            else
            {
                typedef typename std::make_unsigned<Source>::type UnsignedSource;
                return IsIntValueRepresentableImpl<Destination, UnsignedSource>::Get(static_cast<UnsignedSource>(n));
            }
        }
    };

    // Destination が signed で、Source が unsigned のとき
    // Destination に対応する unsinged 型に変換した Destination の最大値と n とを比較する。
    // 両者が unsigned であるため問題なく比較できる。
    template <typename Destination, typename Source>
    struct IsIntValueRepresentableImpl<Destination, Source, typename std::enable_if<
        std::is_signed<Destination>::value && !std::is_signed<Source>::value
    >::type>
    {
        static bool Get(Source n) NN_NOEXCEPT
        {
            typedef std::numeric_limits<Destination> dlimit;
            typedef typename std::make_unsigned<Destination>::type UnsignedDestination;
            return n <= static_cast<UnsignedDestination>((dlimit::max)());
        }
    };

} // detail

template <typename Destination, typename Source>
inline bool IsIntValueRepresentable(Source n) NN_NOEXCEPT
{
    static_assert(std::is_integral<Destination>::value, "Destination must be an integral type.");
    static_assert(std::is_integral<Source>::value, "Source must be an integral type.");
    return nn::util::detail::IsIntValueRepresentableImpl<Destination, Source>::Get(n);
}

template <typename Int>
inline bool CanAddWithoutOverflow(Int x, Int y) NN_NOEXCEPT
{
    static_assert(std::is_integral<Int>::value, "Value must be an integral type.");
    typedef std::numeric_limits<Int> Intlimit;
    return (y >= 0 ? x <= (Intlimit::max)() - y : x >= (Intlimit::min)() - y);
}

template <typename Int>
inline bool CanSubtractWithoutOverflow(Int x, Int y) NN_NOEXCEPT
{
    static_assert(std::is_integral<Int>::value, "Value must be an integral type.");
    typedef std::numeric_limits<Int> Intlimit;
    return (y >= 0 ? x >= (Intlimit::min)() + y : x <= (Intlimit::max)() + y);
}

template <typename Int>
inline bool TryAddWithoutOverflow(Int* pOut, Int x, Int y) NN_NOEXCEPT
{
    static_assert(std::is_integral<Int>::value, "Value must be an integral type.");
    NN_SDK_REQUIRES_NOT_NULL(pOut);

    if (!CanAddWithoutOverflow(x, y))
    {
        return false;
    }

    *pOut = x + y;
    return true;
}

template <typename Int>
inline bool TrySubtractWithoutOverflow(Int* pOut, Int x, Int y) NN_NOEXCEPT
{
    static_assert(std::is_integral<Int>::value, "Value must be an integral type.");
    NN_SDK_REQUIRES_NOT_NULL(pOut);

    if (!CanSubtractWithoutOverflow(x, y))
    {
        return false;
    }

    *pOut = x - y;
    return true;
}


}}
