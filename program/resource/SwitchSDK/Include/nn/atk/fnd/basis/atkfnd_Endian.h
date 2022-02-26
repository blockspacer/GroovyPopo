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

#include <cstdint>
#include <nn/nn_Macro.h>

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(push)
#pragma warning(disable:4200)
#endif
#pragma pack(push, 1)

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  エンディアンユーティリティ用の名前空間
//---------------------------------------------------------------------------
namespace endian {

//! @internal
typedef union
{
    uint64_t UInt64;
    int64_t SInt64;
    double Float64;
} Type64;

//! @internal
typedef union
{
    uint32_t UInt32;
    int32_t SInt32;
    float Float32;
} Type32;

//! @internal
typedef union
{
    uint16_t UInt16;
    int16_t SInt16;
} Type16;

//---------------------------------------------------------------------------
//! @internal
//! @brief        uint64_t のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline uint64_t ByteSwap( uint64_t val ) NN_NOEXCEPT
{
    const uint64_t Mask  = 0xFF00FF00FF00FF00ULL;
    const uint64_t Mask2 = 0xFFFF0000FFFF0000ULL;
    val = ( (val & Mask) >> 8 ) | ( (val << 8) & Mask );
    val = ( (val & Mask2) >> 16 ) | ( (val << 16) & Mask2 );
    return (val >> 32) | (val << 32);
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        int64_t のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline int64_t ByteSwap( int64_t val ) NN_NOEXCEPT
{
    Type64 data;
    data.SInt64 = val;
    data.UInt64 = ByteSwap( data.UInt64 );
    return data.SInt64;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        double のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline double ByteSwap( double val ) NN_NOEXCEPT
{
    Type64 data;
    data.Float64 = val;
    data.UInt64 = ByteSwap( data.UInt64 );
    return data.Float64;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        uint32_t のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline uint32_t ByteSwap( uint32_t val ) NN_NOEXCEPT
{
    const uint32_t Mask = 0xFF00FF00;
    val = ( (val & Mask) >> 8 ) | ( (val << 8) & Mask );
    return (val >> 16) | (val << 16);
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        int32_t のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline int32_t ByteSwap( int32_t val ) NN_NOEXCEPT
{
    Type32 data;
    data.SInt32 = val;
    data.UInt32 = ByteSwap( data.UInt32 );
    return data.SInt32;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        float のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline float ByteSwap( float val ) NN_NOEXCEPT
{
    Type32 data;
    data.Float32 = val;
    data.UInt32 = ByteSwap( data.UInt32 );
    return data.Float32;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        uint16_t のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline uint16_t ByteSwap( uint16_t val ) NN_NOEXCEPT
{
    return (uint16_t)( (val >> 8) | (val << 8) );
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        int16_t のエンディアンを変換します。
//! @param[in]    val     エンディアン変換する値です。
//! @return       変換結果を返します。
//---------------------------------------------------------------------------
inline int16_t ByteSwap( int16_t val ) NN_NOEXCEPT
{
    return (int16_t)( ((uint16_t)val >> 8) | ((uint16_t)val << 8) );
}

//---------------------------------------------------------------------------
//! @internal
//! @brief  エンディアン変換をサポートする数値クラスです。
//---------------------------------------------------------------------------
template <typename T>
class ReverseEndianNum
{
public:
    ReverseEndianNum() NN_NOEXCEPT {}
    explicit ReverseEndianNum(const ReverseEndianNum& other) NN_NOEXCEPT : m_Bits(other.m_Bits) {}
    NN_IMPLICIT ReverseEndianNum(const T val ) NN_NOEXCEPT : m_Bits( ByteSwap( val ) ) {}

    void    operator = (const ReverseEndianNum& other) NN_NOEXCEPT { m_Bits = other.m_Bits; }
    /* T */ NN_IMPLICIT operator T () const NN_NOEXCEPT { return ByteSwap( m_Bits ); }
    void    operator = (T val) NN_NOEXCEPT { m_Bits = ByteSwap( val ); }

    ReverseEndianNum operator+=(T num) NN_NOEXCEPT { m_Bits = ByteSwap( static_cast<T>(*this) + num ); }
    ReverseEndianNum operator-=(T num) NN_NOEXCEPT { m_Bits = ByteSwap( static_cast<T>(*this) - num ); }

private:
    T m_Bits;
};

} // namespace endian

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

#pragma pack(pop)
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(pop)
#endif
