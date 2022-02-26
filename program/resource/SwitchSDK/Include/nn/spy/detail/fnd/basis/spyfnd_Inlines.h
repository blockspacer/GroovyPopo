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

#include <nn/spy/spy_Config.h>

#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <limits>
#include <functional>
#include <algorithm>

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/os.h>
#endif

#include <nn/nn_SdkAssert.h>
#include <nn/nn_StaticAssert.h>

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//----------------------------------------
//! @name ポインタ操作関連
//@{

//---------------------------------------------------------------------------
//! @brief       ポインタからintptr_tへのキャストをおこないます。
//!
//! @param[in]   ptr ポインタ
//!
//! @return      ptrをintptr_tにキャストした値を返します。
//---------------------------------------------------------------------------
inline intptr_t
GetIntPtr( const void* ptr ) NN_NOEXCEPT
{
    return reinterpret_cast<intptr_t>( ptr );
}

//---------------------------------------------------------------------------
//! @brief       ポインタからuintptr_tへのキャストをおこないます。
//!
//! @param[in]   ptr ポインタ
//!
//! @return      ptrをuintptr_tにキャストした値を返します。
//---------------------------------------------------------------------------
inline uintptr_t
GetUintPtr( const void* ptr ) NN_NOEXCEPT
{
    return reinterpret_cast<uintptr_t>( ptr );
}

//@}


//----------------------------------------
//! @name 数値関連
//@{

//---------------------------------------------------------------------------
//! @brief       引数の絶対値を返します。
//!
//! @param[in]   a
//!
//! @return      a の絶対値
//---------------------------------------------------------------------------
template <typename T>
inline T
Abs(T a) NN_NOEXCEPT
{
    return (a < 0) ? static_cast<T>(-a) : a;
}


//---------------------------------------------------------------------------
//! @brief       2つの数値を比較して小さい方の値を返します。
//!
//! @param[in]   a, b    比較する数値
//!
//! @return      a, b のうち小さい方の値を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
Min(T a, T b) NN_NOEXCEPT
{
    return (a > b) ? b: a;
}



//---------------------------------------------------------------------------
//! @brief       2つの数値を比較して大きい方の値を返します。
//!
//! @param[in]   a, b    比較する数値
//!
//! @return      a, b のうち大きい方の値を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
Max(T a, T b) NN_NOEXCEPT
{
    return (a < b) ? b: a;
}


//---------------------------------------------------------------------------
//! @brief       low から high の範囲に収まる値を返します。
//!
//! @param[in]   x       比較する数値
//! @param[in]   low     最小値
//! @param[in]   high    最大値
//!
//! @return      x < low ならば low, x > high ならば high それ以外は x を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
Clamp(T x, T low, T high) NN_NOEXCEPT
{
    return (x > high) ? high : ( (x < low) ? low : x );
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り上げます。
//!
//! @param[in]   x       切り上げる数。
//! @param[in]   base    切り上げの基準となる値。
//!                      2の累乗でなければなりません。
//!
//! @return      x を base の倍数に切り上げた値を返します。
//---------------------------------------------------------------------------
template <typename ValueT>
inline ValueT
RoundUp(ValueT x, int base) NN_NOEXCEPT
{
    int mask = base - 1;
    return static_cast<ValueT>(static_cast<ValueT>( x + mask ) & ~mask);
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り上げます。(ポインタ用の特殊化)
//---------------------------------------------------------------------------
template <typename ValueT>
inline void*
RoundUp(ValueT* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    return reinterpret_cast<void*>( RoundUp(value, base) );
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り上げます。(void ポインタ用の特殊化)
//---------------------------------------------------------------------------
template <>
inline void*
RoundUp<void>(void* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    return reinterpret_cast<void*>( RoundUp(value, base) );
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り上げます。(const ポインタ用の特殊化)
//---------------------------------------------------------------------------
template <typename ValueT>
inline const void*
RoundUp(const ValueT* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    return reinterpret_cast<const void*>( RoundUp(value, base) );
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り上げます。(const void ポインタ用の特殊化)
//---------------------------------------------------------------------------
template <>
inline const void*
RoundUp<void>(const void* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    return reinterpret_cast<const void*>( RoundUp(value, base) );
}

//---------------------------------------------------------------------------
//! @brief        ポインタを base の倍数に切り上げ、指定した型のポインタにキャストします。
//!
//! @tparam       TPtr    キャストする型です。
//! @param[out]   x       切り上げるポインタです。
//! @param[in]    base    切り上げの基準となる値です。
//!                       2の累乗でなければなりません。
//!
//! @return       x を base の倍数に切り上げた値を TPtr にキャストして返します。
//---------------------------------------------------------------------------
template <typename TPtr>
inline TPtr
RoundUpTo(void* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    void* rounded = reinterpret_cast<void*>( RoundUp(value, base) );

    return static_cast<TPtr>(rounded);
}

//---------------------------------------------------------------------------
//! @brief        ポインタを base の倍数に切り上げ、指定した型のポインタにキャストします。(const 版)
//!
//! @tparam       TPtr    キャストする型です。
//! @param[out]   x       切り上げるポインタです。
//! @param[in]    base    切り上げの基準となる値です。
//!                       2の累乗でなければなりません。
//!
//! @return       x を base の倍数に切り上げた値を TPtr にキャストして返します。
//---------------------------------------------------------------------------
template <typename TPtr>
inline TPtr
RoundUpTo(const void* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    const void* rounded = reinterpret_cast<const void*>( RoundUp(value, base) );

    return static_cast<TPtr>(rounded);
}


//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り下げます。
//!
//! @param[in]   x       切り下げる数。
//! @param[in]   base    切り下げの基準となる値。2の累乗でなければなりません。
//!
//! @return      x を base の倍数に切り下げた値を返します。
//---------------------------------------------------------------------------
template <typename ValueT>
inline ValueT
RoundDown(ValueT x, int base) NN_NOEXCEPT
{
    return static_cast<ValueT>( x & ~(base - 1) );
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り下げます。(ポインタ用特殊化)
//---------------------------------------------------------------------------
template <typename ValueT>
inline void*
RoundDown(ValueT* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    return reinterpret_cast<void*>( RoundDown(value, base) );
}

//---------------------------------------------------------------------------
//! @brief       x を base の倍数に切り下げます。(const ポインタ用特殊化)
//---------------------------------------------------------------------------
template <typename ValueT>
inline const void*
RoundDown(const ValueT* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    return reinterpret_cast<const void*>( RoundDown(value, base) );
}


//---------------------------------------------------------------------------
//! @brief        ポインタを base の倍数に切り下げ、指定した型のポインタにキャストします。
//!
//! @tparam       TPtr    キャストする型です。
//! @param[out]   x       切り下げるポインタです。
//! @param[in]    base    切り下げの基準となる値です。
//!                       2の累乗でなければなりません。
//!
//! @return       x を base の倍数に切り下げた値を TPtr にキャストして返します。
//---------------------------------------------------------------------------
template <typename TPtr>
inline TPtr
RoundDownTo(void* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    void* rounded = reinterpret_cast<void*>( RoundDown(value, base) );

    return static_cast<TPtr>(rounded);
}

//---------------------------------------------------------------------------
//! @brief        const ポインタを base の倍数に切り下げ、指定した型のポインタにキャストします。
//---------------------------------------------------------------------------
template <typename TPtr>
inline TPtr
RoundDownTo(const void* x, int base) NN_NOEXCEPT
{
    uintptr_t value = GetUintPtr( x );
    const void* rounded = reinterpret_cast<const void*>( RoundDown(value, base) );

    return static_cast<TPtr>(rounded);
}

//---------------------------------------------------------------------------
//! @brief       x を y で割って端数を切り上げます。
//!
//! @param[in]   x   割られる数
//! @param[in]   y   割る数
//!
//! @return      x を y で割って端数を切り上げた値を返します。
//---------------------------------------------------------------------------
template <typename ValueT>
inline ValueT
DivUp(ValueT x, int y) NN_NOEXCEPT
{
    return (x + (static_cast<ValueT>(y) - 1)) / static_cast<ValueT>(y);
}

//---------------------------------------------------------------------------
//! @brief       ビット列の一部を抜き出します。
//!
//! @param[in]   v   ビット列
//! @param[in]   pos 抜き出す最下位ビット位置
//! @param[in]   len 抜き出す長さ
//!
//! @return      抜き出されたビット列を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
ExtractBits(nn::Bit32 v, int pos, int len) NN_NOEXCEPT
{
    return static_cast<T>( v & (((1u << len) - 1) << pos) );
}

//---------------------------------------------------------------------------
//! @brief       ビット列の一部を抜き出します。
//!
//! @param[in]   v   ビット列
//! @param[in]   pos 抜き出す最下位ビット位置
//! @param[in]   len 抜き出す長さ
//!
//! @return      抜き出されたビット列を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
ExtractBits(nn::Bit64 v, int pos, int len) NN_NOEXCEPT
{
    return static_cast<T>( v & (((1ull << len) - 1) << pos) );
}

//---------------------------------------------------------------------------
//! @brief       ビット列の一部を取得します。
//!
//! @param[in]   v   ビット列
//! @param[in]   pos 取得する最下位ビット位置
//! @param[in]   len 取得する長さ
//!
//! @return      取得したビット列を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
GetBits(nn::Bit32 v, int pos, int len) NN_NOEXCEPT
{
    return static_cast<T>( (v >> pos) & ((1u << len) - 1) );
}

//---------------------------------------------------------------------------
//! @brief       ビット列の一部を取得します。
//!
//! @param[in]   v   ビット列
//! @param[in]   pos 取得する最下位ビット位置
//! @param[in]   len 取得する長さ
//!
//! @return      取得したビット列を返します。
//---------------------------------------------------------------------------
template <typename T>
inline T
GetBits(nn::Bit64 v, int pos, int len) NN_NOEXCEPT
{
    return static_cast<T>( (v >> pos) & ((1ull << len) - 1) );
}

//---------------------------------------------------------------------------
//! @brief        浮動少数点型を比較するための等値比較関数オブジェクトです。
//!
//! @tparam       ValueT 比較する型です。
//---------------------------------------------------------------------------
template<typename ValueT>
class
CloseAtTolerance
{
public:
    //! @brief        コンストラクタです。
    //!
    //! @param[in]    tolerance 誤差許容値です。
    //!
    CloseAtTolerance(ValueT tolerance) NN_NOEXCEPT : m_Tolerance(tolerance) {}

    //! @brief        コンストラクタです。
    //!
    //! @param[in]    numberOfRoundingError 丸め誤差の数です。
    //!
    CloseAtTolerance(int numberOfRoundingError) NN_NOEXCEPT
    : m_Tolerance(numberOfRoundingError * std::numeric_limits<ValueT>::epsilon() * 0.5f) {}

    //! @brief        許容範囲内かどうか比較します。
    //!
    //! @param[in]    lhs 左辺です。
    //! @param[in]    rhs 右辺です。
    //! @return       lhs と rhs の値が許容範囲内であれば true を返します。
    //!
    bool operator()(ValueT lhs, ValueT rhs) const NN_NOEXCEPT
    {
        ValueT diff = Abs(lhs - rhs);
        return
            diff <= (m_Tolerance * Abs(lhs)) &&
            diff <= (m_Tolerance * Abs(rhs));
    }

private:
    ValueT m_Tolerance;
};

//---------------------------------------------------------------------------
//! @brief        浮動少数点型を比較するための等値比較関数オブジェクトです。
//!
//! @tparam       ValueT 比較する型です。
//---------------------------------------------------------------------------
template<typename ValueT>
class
CloseAtToleranceWeak
{
public:
    //! @brief        コンストラクタです。
    //!
    //! @param[in]    tolerance 誤差許容値です。
    //!
    CloseAtToleranceWeak(ValueT tolerance) NN_NOEXCEPT : m_Tolerance(tolerance) {}

    //! @brief        コンストラクタです。
    //!
    //! @param[in]    numberOfRoundingError 丸め誤差の数です。
    //!
    CloseAtToleranceWeak(int numberOfRoundingError) NN_NOEXCEPT
    : m_Tolerance(numberOfRoundingError * std::numeric_limits<ValueT>::epsilon() * 0.5f) {}

    //! @brief        許容範囲内かどうか比較します。
    //!
    //! @param[in]    lhs 左辺です。
    //! @param[in]    rhs 右辺です。
    //! @return       lhs と rhs の値が許容範囲内であれば true を返します。
    //!
    bool operator()(ValueT lhs, ValueT rhs) const NN_NOEXCEPT
    {
        ValueT diff = Abs(lhs - rhs);
        return
            diff <= (m_Tolerance * Abs(lhs)) ||
            diff <= (m_Tolerance * Abs(rhs));
    }

private:
    ValueT m_Tolerance;
};

//---------------------------------------------------------------------------
//! @brief       浮動少数点型の誤差範囲を考慮した等値比較です。
//!
//! @param[in]   lhs   左辺です。
//! @param[in]   rhs   右辺です。
//!
//! @return      等値と判断されたら true を返します。
//---------------------------------------------------------------------------
template<typename ValueT>
inline bool
FloatEquals(ValueT lhs, ValueT rhs) NN_NOEXCEPT
{
    return CloseAtTolerance<ValueT>(std::numeric_limits<ValueT>::epsilon())(lhs, rhs);
}

//---------------------------------------------------------------------------
//! @brief       浮動少数点型の誤差範囲を考慮したほぼ等値比較です。
//!
//! @param[in]   lhs   左辺です。
//! @param[in]   rhs   右辺です。
//!
//! @return      等値と判断されたら true を返します。
//---------------------------------------------------------------------------
template<typename ValueT>
inline bool
FloatEqualsWeak(ValueT lhs, ValueT rhs) NN_NOEXCEPT
{
    return CloseAtToleranceWeak<ValueT>(std::numeric_limits<ValueT>::epsilon())(lhs, rhs);
}

//---------------------------------------------------------------------------
//! @brief        float 型の値をビット列を変更することなく uint32_t 型にします
//!
//! @param[in]    x       uint32_t型 にする値。
//!
//! @return       x と等しいビット表現を持つ uint32_t 型の値。
//---------------------------------------------------------------------------
inline uint32_t
FloatAsUint32(float x) NN_NOEXCEPT
{
#if 1
    uint32_t result;
    NN_STATIC_ASSERT(sizeof(result) == sizeof(x));
    std::memcpy(&result, &x, sizeof(result));
    return result;
#else
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( &x );
    return *reinterpret_cast<uint32_t*>(ptr);
#endif
}

//---------------------------------------------------------------------------
//! @brief        float 型のポインタを uint32_t 型のポインタにキャストします
//!
//! @param[in]    x       float型 のポインタ。
//!
//! @return       uint32_t 型のポインタを返します。
//---------------------------------------------------------------------------
inline uint32_t*
FloatAsUint32(float* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( x );
    return reinterpret_cast<uint32_t*>(ptr);
}

//---------------------------------------------------------------------------
//! @brief        float 型のポインタを uint32_t 型のポインタにキャストします
//!
//! @param[in]    x       float型 のポインタ。
//!
//! @return       uint32_t 型のポインタを返します。
//---------------------------------------------------------------------------
inline const uint32_t*
FloatAsUint32(const float* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    const char* ptr = reinterpret_cast<const char*>( x );
    return reinterpret_cast<const uint32_t*>(ptr);
}


//---------------------------------------------------------------------------
//! @brief        uint32_t 型の値をビット列を変更することなく float 型にします
//!
//! @param[in]    x       float型にする値。
//!
//! @return       x と等しいビット表現を持つ float 型の値。
//---------------------------------------------------------------------------
inline float
Uint32AsFloat(uint32_t x) NN_NOEXCEPT
{
#if 1
    float result;
    NN_STATIC_ASSERT(sizeof(result) == sizeof(x));
    std::memcpy(&result, &x, sizeof(result));
    return result;
#else
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( &x );
    return *reinterpret_cast<float*>(ptr);
#endif
}

//---------------------------------------------------------------------------
//! @brief        uint32_t 型のポインタを float 型のポインタにキャストします
//!
//! @param[in]    x       uint32_t型 のポインタ。
//!
//! @return       float 型のポインタを返します。
//---------------------------------------------------------------------------
inline float*
Uint32AsFloat(uint32_t* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( x );
    return reinterpret_cast<float*>(ptr);
}

//---------------------------------------------------------------------------
//! @brief        uint32_t 型のポインタを float 型のポインタにキャストします
//!
//! @param[in]    x       uint32_t型 のポインタ。
//!
//! @return       float 型のポインタを返します。
//---------------------------------------------------------------------------
inline const float*
Uint32AsFloat(const uint32_t* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    const char* ptr = reinterpret_cast<const char*>( x );
    return reinterpret_cast<const float*>(ptr);
}

//---------------------------------------------------------------------------
//! @brief        double 型の値をビット列を変更することなく uint64_t 型にします
//!
//! @param[in]    x       uint64_t型 にする値。
//!
//! @return       x と等しいビット表現を持つ uint64_t 型の値。
//---------------------------------------------------------------------------
inline uint64_t
DoubleAsUint64(double x) NN_NOEXCEPT
{
#if 1
    uint64_t result;
    NN_STATIC_ASSERT(sizeof(result) == sizeof(x));
    std::memcpy(&result, &x, sizeof(result));
    return result;
#else
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( &x );
    return *reinterpret_cast<uint64_t*>(ptr);
#endif
}

//---------------------------------------------------------------------------
//! @brief        double 型のポインタを uint64_t 型のポインタにキャストします
//!
//! @param[in]    x       double 型 のポインタ。
//!
//! @return       uint64_t 型のポインタを返します。
//---------------------------------------------------------------------------
inline uint64_t*
DoubleAsUint64(double* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( x );
    return reinterpret_cast<uint64_t*>(ptr);
}

//---------------------------------------------------------------------------
//! @brief        double 型のポインタを uint64_t 型のポインタにキャストします
//!
//! @param[in]    x       double 型 のポインタ。
//!
//! @return       uint64_t 型のポインタを返します。
//---------------------------------------------------------------------------
inline const uint64_t*
DoubleAsUint64(const double* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    const char* ptr = reinterpret_cast<const char*>( x );
    return reinterpret_cast<const uint64_t*>(ptr);
}


//---------------------------------------------------------------------------
//! @brief        uint64_t 型の値をビット列を変更することなく double 型にします
//!
//! @param[in]    x       double型にする値。
//!
//! @return       x と等しいビット表現を持つ double 型の値。
//---------------------------------------------------------------------------
inline double
Uint64AsDouble(uint64_t x) NN_NOEXCEPT
{
#if 1
    double result;
    NN_STATIC_ASSERT(sizeof(result) == sizeof(x));
    std::memcpy(&result, &x, sizeof(result));
    return result;
#else
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( &x );
    return *reinterpret_cast<double*>(ptr);
#endif
}

//---------------------------------------------------------------------------
//! @brief        uint64_t 型のポインタを double 型のポインタにキャストします
//!
//! @param[in]    x       uint64_t 型 のポインタ。
//!
//! @return       double 型のポインタを返します。
//---------------------------------------------------------------------------
inline double*
Uint64AsDouble(uint64_t* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    char* ptr = reinterpret_cast<char*>( x );
    return reinterpret_cast<double*>(ptr);
}

//---------------------------------------------------------------------------
//! @brief        uint64_t 型のポインタを double 型のポインタにキャストします
//!
//! @param[in]    x       uint64_t 型 のポインタ。
//!
//! @return       double 型のポインタを返します。
//---------------------------------------------------------------------------
inline const double*
Uint64AsDouble(const uint64_t* x) NN_NOEXCEPT
{
    // NOTE: char ポインタは、ANSI aliasing rule では例外扱いなので、これでも -ansi_aliasing で通ります。
    const char* ptr = reinterpret_cast<const char*>( x );
    return reinterpret_cast<const double*>(ptr);
}

//@}


//----------------------------------------
//! @name ポインタ操作関連
//@{

//---------------------------------------------------------------------------
//! @brief       ２つのポインタアドレスのオフセット値をlongで取得します。
//!
//! @param[in]   start   開始アドレス
//! @param[in]   end     終了アドレス
//!
//! @return      ２つのポインタのオフセット値
//---------------------------------------------------------------------------
template <typename TDiff>
inline TDiff
GetOffsetFromPtr( const void* start, const void* end ) NN_NOEXCEPT
{
    return static_cast<TDiff>(GetUintPtr(end)) - static_cast<TDiff>(GetUintPtr(start));
}

inline ptrdiff_t
GetOffsetFromPtr( const void* start, const void* end ) NN_NOEXCEPT
{
    return GetOffsetFromPtr<ptrdiff_t>(start, end);
}


//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。
//!
//! @param[in]   ptr     ポインタ
//! @param[in]   offset  オフセット値
//!
//! @return      voidポインタ型にoffsetバイトを加えたアドレスを引数と同型のポインタとして返します。
//---------------------------------------------------------------------------
template <typename TOffset>
inline void*
AddOffsetToPtr( void* ptr, TOffset offset ) NN_NOEXCEPT
{
    return reinterpret_cast<void*>( GetUintPtr(ptr) + offset );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(const版)
//---------------------------------------------------------------------------
template <typename TOffset>
inline const void*
AddOffsetToPtr( const void* ptr, TOffset offset ) NN_NOEXCEPT
{
    return reinterpret_cast<const void*>( GetUintPtr(ptr) + offset );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(キャスト付)
//!
//! @tparam      TResult キャストする型を指定します。
//---------------------------------------------------------------------------
template <typename TResult, typename TOffset>
inline TResult
AddOffsetToPtr( void* ptr, TOffset offset ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddOffsetToPtr(ptr, offset) );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(キャスト付き, const版)
//!
//! @tparam      TResult キャストする型を指定します。
//---------------------------------------------------------------------------
template <typename TResult, typename TOffset>
inline TResult
AddOffsetToPtr( const void* ptr, TOffset offset ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddOffsetToPtr(ptr, offset) );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(アライメント付き)
//---------------------------------------------------------------------------
template <typename TOffset>
inline void*
AddOffsetToPtr( void* ptr, TOffset offset, int alignment ) NN_NOEXCEPT
{
    return RoundUp(reinterpret_cast<void*>( GetUintPtr(ptr) + offset ), alignment);
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(アライメント付き, const版)
//---------------------------------------------------------------------------
template <typename TOffset>
inline const void*
AddOffsetToPtr( const void* ptr, TOffset offset, int alignment ) NN_NOEXCEPT
{
    return RoundUp(reinterpret_cast<const void*>( GetUintPtr(ptr) + offset ), alignment);
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(キャスト付き, アライメント付き)
//---------------------------------------------------------------------------
template <typename TResult, typename TOffset>
inline TResult
AddOffsetToPtr( void* ptr, TOffset offset, int alignment ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddOffsetToPtr(ptr, offset, alignment) );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(キャスト付き, アライメント付き, const 版)
//---------------------------------------------------------------------------
template <typename TResult, typename TOffset>
inline TResult
AddOffsetToPtr( const void* ptr, TOffset offset, int alignment ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddOffsetToPtr(ptr, offset, alignment) );
}


//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。
//!
//! @param[in]   ptr     ポインタ
//! @param[in]   offset  オフセット値
//!
//! @return      voidポインタ型にoffsetバイトを加えたアドレスを引数と同型のポインタとして返します。
//---------------------------------------------------------------------------
inline void*
AddU32ToPtr( void* ptr, uint32_t offset ) NN_NOEXCEPT
{
    return AddOffsetToPtr( ptr, offset );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(const 版)
//---------------------------------------------------------------------------
inline const void*
AddU32ToPtr( const void* ptr, uint32_t offset ) NN_NOEXCEPT
{
    return AddOffsetToPtr( ptr, offset );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(キャスト付き)
//---------------------------------------------------------------------------
template <typename TResult>
inline TResult
AddU32ToPtr( void* ptr, uint32_t offset ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddU32ToPtr(ptr, offset) );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(キャスト付き, const 版)
//---------------------------------------------------------------------------
template <typename TResult>
inline TResult
AddU32ToPtr( const void* ptr, uint32_t offset ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddU32ToPtr(ptr, offset) );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(アライメント付き)
//---------------------------------------------------------------------------
inline void*
AddU32ToPtr( void* ptr, uint32_t offset, int alignment ) NN_NOEXCEPT
{
    return AddOffsetToPtr( ptr, offset, alignment );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(アライメント付き, const 版)
//---------------------------------------------------------------------------
inline const void*
AddU32ToPtr( const void* ptr, uint32_t offset, int alignment ) NN_NOEXCEPT
{
    return AddOffsetToPtr( ptr, offset, alignment );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(アライメント付き, キャスト付き)
//---------------------------------------------------------------------------
template <typename TResult>
inline TResult
AddU32ToPtr( void* ptr, uint32_t offset, int alignment ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddU32ToPtr(ptr, offset, alignment) );
}

//---------------------------------------------------------------------------
//! @brief       ポインタにオフセット値を加えます。(アライメント付き, キャスト付き, const 版)
//---------------------------------------------------------------------------
template <typename TResult>
inline TResult
AddU32ToPtr( const void* ptr, uint32_t offset, int alignment ) NN_NOEXCEPT
{
    return static_cast<TResult>( AddU32ToPtr(ptr, offset, alignment) );
}



//---------------------------------------------------------------------------
//! @brief       ２つのポインタアドレスを比較します。
//!
//! @param[in]   a   比較元のポインタ
//! @param[in]   b   比較対象となるポインタ
//!
//! @return      > 0 aの指すアドレスがbの指すアドレスよりも大きい場合
//!              = 0 aの指すアドレスとbの指すアドレスが等しい場合
//!              < 0 aの指すアドレスがbの指すアドレスよりも小さい場合
//---------------------------------------------------------------------------
inline int
ComparePtr( const void* a, const void* b ) NN_NOEXCEPT
{
    if ( GetUintPtr( a ) == GetUintPtr( b ) )
    {
        return 0;
    }
    else
    {
        return ( GetUintPtr( a ) > GetUintPtr( b ) ) ? 1 : -1;
    }
}

//@}


//----------------------------------------
//! @name ビット操作関連
//@{

//---------------------------------------------------------------------------
//! @brief       ビット列から部分ビット列を抜き出します。
//!
//! @param[in]   bits    抜き出し元のビット列
//! @param[in]   pos     抜き出すビット列の最下位ビットのbits中での位置
//! @param[in]   len     抜き出すビット長
//!
//! @return      抽出したビット列。
//---------------------------------------------------------------------------
template <typename OutputT>
inline OutputT
BitExtract(OutputT bits, int pos, int len = 1) NN_NOEXCEPT
{
    const OutputT mask = static_cast<OutputT>( ( 1UL << len ) - 1 );
    return static_cast<OutputT>( (bits >> pos) & mask );
}


//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//!
//! @param[in]   x       反転する数。
//!
//! @return      x のエンディアンを反転して返します。
//---------------------------------------------------------------------------
inline uint32_t
ReverseEndian( uint32_t x ) NN_NOEXCEPT
{
    return static_cast<uint32_t>(
        BitExtract( x,  0, 8 ) << 24 |
        BitExtract( x,  8, 8 ) << 16 |
        BitExtract( x, 16, 8 ) <<  8 |
        BitExtract( x, 24, 8 ) <<  0
    );
}

//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//---------------------------------------------------------------------------
inline int32_t
ReverseEndian( int32_t x ) NN_NOEXCEPT
{
    return static_cast<int32_t>(
        BitExtract( x,  0, 8 ) << 24 |
        BitExtract( x,  8, 8 ) << 16 |
        BitExtract( x, 16, 8 ) <<  8 |
        BitExtract( x, 24, 8 ) <<  0
    );
}

//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//---------------------------------------------------------------------------
inline uint16_t
ReverseEndian( uint16_t x ) NN_NOEXCEPT
{
    return static_cast<uint16_t>(
        BitExtract( x,  0, 8 ) << 8 |
        BitExtract( x,  8, 8 ) << 0
    );
}

//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//---------------------------------------------------------------------------
inline int16_t
ReverseEndian( int16_t x ) NN_NOEXCEPT
{
    return static_cast<int16_t>(
        BitExtract( x,  0, 8 ) << 8 |
        BitExtract( x,  8, 8 ) << 0
    );
}

//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//---------------------------------------------------------------------------
inline uint64_t
ReverseEndian( uint64_t x ) NN_NOEXCEPT
{
    return static_cast<uint64_t>(
        BitExtract( x,  0, 8 ) << 56 |
        BitExtract( x,  8, 8 ) << 48 |
        BitExtract( x, 16, 8 ) << 40 |
        BitExtract( x, 24, 8 ) << 32 |
        BitExtract( x, 32, 8 ) << 24 |
        BitExtract( x, 40, 8 ) << 16 |
        BitExtract( x, 48, 8 ) <<  8 |
        BitExtract( x, 56, 8 ) <<  0
    );
}

//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//---------------------------------------------------------------------------
inline int64_t
ReverseEndian( int64_t x ) NN_NOEXCEPT
{
    return static_cast<int64_t>(
        BitExtract( x,  0, 8 ) << 56 |
        BitExtract( x,  8, 8 ) << 48 |
        BitExtract( x, 16, 8 ) << 40 |
        BitExtract( x, 24, 8 ) << 32 |
        BitExtract( x, 32, 8 ) << 24 |
        BitExtract( x, 40, 8 ) << 16 |
        BitExtract( x, 48, 8 ) <<  8 |
        BitExtract( x, 56, 8 ) <<  0
    );
}

//---------------------------------------------------------------------------
//! @brief       エンディアンを反転します。
//!
//! @param[in]   x       反転する数。
//!
//! @return      x のエンディアンを反転して返します。
//---------------------------------------------------------------------------
inline float
ReverseEndian( float x ) NN_NOEXCEPT
{
    uint32_t ux = FloatAsUint32( x );
    uint32_t rux = ReverseEndian( ux );

    return Uint32AsFloat( rux );
}

//@}

//--------------------------------------------------------------------------
//! @brief        ビット列から値を取り出します。
//!
//! @param[in]    bits    元のビット列です。
//! @param[in]    width   ビット幅です。
//! @param[in]    shift   シフト値です。
//!
//! @return       ビット長とシフト値から取得した値を返します。
//---------------------------------------------------------------------------
inline uint32_t
ReadBits(uint32_t bits, int width, int shift) NN_NOEXCEPT
{
    return (bits >> shift) & ((0x1U << width) - 1);
}

//--------------------------------------------------------------------------
//! @brief        ビット長とシフト値を指定してマスクされたビット列を取得します。
//!
//! @param[in]    value   ビット列に対して設定する値です。
//! @param[in]    width   ビット幅です。
//! @param[in]    shift   シフト値です。
//!
//! @return       ビット長とシフト値から生成したビット列を返します。
//---------------------------------------------------------------------------
template <typename T>
inline uint32_t
MakeBits(T value, int width, int shift) NN_NOEXCEPT
{
    return (static_cast<uint32_t>(value) & ((0x1U << width) - 1)) << shift;
}

//--------------------------------------------------------------------------
//! @brief        ビット列に特定の値を書き込みます。
//!
//! @param[in]    bits    元ビット列です。
//! @param[in]    value   ビット列の一部を置き換える為の値です。
//! @param[in]    width   ビット幅です。
//! @param[in]    shift   シフト値です。
//!
//! @return       置き換え後のビット列を返します。
//---------------------------------------------------------------------------
template <typename T>
inline uint32_t
WriteBits(uint32_t bits, T value, int width, int shift) NN_NOEXCEPT
{
    uint32_t mask = (0x1U << width) - 1;

    return (bits & ~(mask << shift)) | ((static_cast<uint32_t>(value) & mask) << shift);
}

template <typename T>
NN_FORCEINLINE
bool
TestBit(
    T       bits,
    int     pos
) NN_NOEXCEPT
{
    const T mask = static_cast<T>(1 << pos);

    return 0 != (bits & mask);
}

template <typename T>
NN_FORCEINLINE
T
GetBits(
    T       bits,
    int     pos,
    int     len
) NN_NOEXCEPT
{
    NN_SDK_ASSERT(len <= 32);
    const uint32_t mask = ~(0xFFFFFFFFU << len);

    return static_cast<T>((bits >> pos) & mask);
}

NN_FORCEINLINE
void* Memset(void* dst, uint8_t value, size_t size) NN_NOEXCEPT
{
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    return OSBlockSet(dst, value, size);
#else
    return ::std::memset(dst, value, size);
#endif
}

//---------------------------------------------------------------------------
//! @brief        配列のサイズを取得します。
//!
//! @param[in]    array サイズを取得したい配列
//!
//! @return       配列のサイズ
//---------------------------------------------------------------------------
template<class T, size_t S>
inline int
GetArrayLength(
    const T (&array)[S]
) NN_NOEXCEPT
{
    NN_UNUSED(array);
    return static_cast<int>(S);
}

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
