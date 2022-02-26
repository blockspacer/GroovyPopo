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

#include <nn/g3d/g3d_Configuration.h>
#include <nn/util/util_BitUtil.h>
#include <nn/util/util_Endian.h>
#include <cstring>

namespace nn { namespace g3d { namespace detail {

enum Alignment
{
    Alignment_Default = 8
};

//--------------------------------------------------------------------------------------------------

NN_FORCEINLINE
void* AddOffset(void* ptr, ptrdiff_t offset) NN_NOEXCEPT
{
    return static_cast<char*>(ptr) + offset;
}

NN_FORCEINLINE
const void* AddOffset(const void* ptr, ptrdiff_t offset) NN_NOEXCEPT
{
    return static_cast<const char*>(ptr) + offset;
}

template <typename ResultT>
NN_FORCEINLINE
ResultT* AddOffset(void* ptr, ptrdiff_t offset) NN_NOEXCEPT
{
    return static_cast<ResultT*>(AddOffset(ptr, offset));
}

template <typename ResultT>
NN_FORCEINLINE
const ResultT* AddOffset(const void* ptr, ptrdiff_t offset) NN_NOEXCEPT
{
    return static_cast<const ResultT*>(AddOffset(ptr, offset));
}

template <typename T>
NN_FORCEINLINE
bool IsAligned(T ptr, size_t alignment)
{
    return nn::util::is_aligned(reinterpret_cast<uintptr_t>(ptr), alignment);
}

//--------------------------------------------------------------------------------------------------

template <bool swap>
NN_FORCEINLINE
void Copy32(void* pDst, const void* pSrc, int count) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_OS_COS )
    if (swap)
    {
        Bit32* dst = static_cast<Bit32*>(pDst);
        const Bit32* src = static_cast<const Bit32*>(pSrc);
        for (const Bit32* end = src + count; src != end; ++dst, ++src)
        {
            nn::util::StoreEndianReversed(dst, *src);
        }
    }
    else
#endif
    {
        memcpy(pDst, pSrc, sizeof(Bit32) * count);
    }
}

//--------------------------------------------------------------------------------------------------

//! @brief 高速キャストを使用して static_cast を行います。
//!
//! 高速キャストが用意されていない型の場合はリンクエラーになります。
//!
template <typename DstT, typename SrcT>
DstT FastCast(SrcT x) NN_NOEXCEPT;

template <>
NN_FORCEINLINE
int16_t FastCast<int16_t, float>(float x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return __OSf32tos16Value(x);
#else
    return static_cast<int16_t>(x);
#endif
}

template <>
NN_FORCEINLINE
uint16_t FastCast<uint16_t, float>(float x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return __OSf32tou16Value(x);
#else
    return static_cast<uint16_t>(x);
#endif
}

template <>
NN_FORCEINLINE
int8_t FastCast<int8_t, float>(float x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return __OSf32tos8Value(x);
#else
    return static_cast<int8_t>(x);
#endif
}

template <>
NN_FORCEINLINE
uint8_t FastCast<uint8_t, float>(float x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return __OSf32tou8Value(x);
#else
    return static_cast<uint8_t>(x);
#endif
}

template <>
NN_FORCEINLINE
float FastCast<float, int16_t>(int16_t x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return OSs16tof32Value(&x);
#else
    return static_cast<float>(x);
#endif
}

template <>
NN_FORCEINLINE
float FastCast<float, uint16_t>(uint16_t x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return OSu16tof32Value(&x);
#else
    return static_cast<float>(x);
#endif
}

template <>
NN_FORCEINLINE
float FastCast<float, int8_t>(int8_t x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return OSs8tof32Value(&x);
#else
    return static_cast<float>(x);
#endif
}

template <>
NN_FORCEINLINE
float FastCast<float, uint8_t>(uint8_t x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return OSu8tof32Value(&x);
#else
    return static_cast<float>(x);
#endif
}

//--------------------------------------------------------------------------------------------------

//! @brief 可能であれば高速キャストを使用して static_cast を行います。
//!
//! テンプレートでの使用を想定しています。
//!
template <typename DstT, typename SrcT>
NN_FORCEINLINE
DstT StaticCast(SrcT x) NN_NOEXCEPT
{
    return static_cast<DstT>(x);
}

template <>
NN_FORCEINLINE
int16_t StaticCast<int16_t, float>(float x) NN_NOEXCEPT
{
    return FastCast<int16_t, float>(x);
}

template <>
NN_FORCEINLINE
uint16_t StaticCast<uint16_t, float>(float x) NN_NOEXCEPT
{
    return FastCast<uint16_t, float>(x);
}

template <>
NN_FORCEINLINE
int8_t StaticCast<int8_t, float>(float x) NN_NOEXCEPT
{
    return FastCast<int8_t, float>(x);
}

template <>
NN_FORCEINLINE
uint8_t StaticCast<uint8_t, float>(float x) NN_NOEXCEPT
{
    return FastCast<uint8_t, float>(x);
}

template <>
NN_FORCEINLINE
float StaticCast<float, int16_t>(int16_t x) NN_NOEXCEPT
{
    return FastCast<float, int16_t>(x);
}

template <>
NN_FORCEINLINE
float StaticCast<float, uint16_t>(uint16_t x) NN_NOEXCEPT
{
    return FastCast<float, uint16_t>(x);
}

template <>
NN_FORCEINLINE
float StaticCast<float, int8_t>(int8_t x) NN_NOEXCEPT
{
    return FastCast<float, int8_t>(x);
}

template <>
NN_FORCEINLINE
float StaticCast<float, uint8_t>(uint8_t x) NN_NOEXCEPT
{
    return FastCast<float, uint8_t>(x);
}

//--------------------------------------------------------------------------------------------------

NN_FORCEINLINE
int CountOnes(Bit32 x) NN_NOEXCEPT
{
    x = (x & 0x55555555) + (x >>  1 & 0x55555555);
    x = (x & 0x33333333) + (x >>  2 & 0x33333333);
    x = (x & 0x0F0F0F0F) + (x >>  4 & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + (x >>  8 & 0x00FF00FF);
    x = (x & 0x0000FFFF) + (x >> 16 & 0x0000FFFF);
    return x;
}

NN_FORCEINLINE
int CountLeadingZeros(Bit32 x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return __CLZ32(x);
#else
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return CountOnes(~x);
#endif
}

NN_FORCEINLINE
int CountTrailingZeros(Bit32 x) NN_NOEXCEPT
{
#if defined( NN_BUILD_CONFIG_TOOLCHAIN_GHS )
    return 32 - CountLeadingZeros(~x & (x - 1));
#else
    return CountOnes((x & (~x + 1)) - 1);
#endif
}

}}} // namespace nn::g3d::detail

