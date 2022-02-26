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

namespace nn { namespace crypto { namespace detail {

/**
    The BigNum class holds a large binary number (typically 256, 2048 or 4096 bits) for use in cryptography calculus.
    The
*/
class BigNum
{
    NN_DISALLOW_COPY(BigNum);

public:
    typedef uint32_t Digit;
    typedef uint16_t HalfDigit;
    typedef uint64_t DoubleDigit;

    static const int MaxBits = 4096;
    static const int DigitBits = sizeof(uint32_t) * 8;
    static const Digit MaxDigitValue = UINT32_MAX;
    static const Digit MaxHalfDigitValue = UINT16_MAX;

public:
    BigNum() NN_NOEXCEPT;
    ~BigNum() NN_NOEXCEPT;

public:
    /**
    Import from a portable "octet-string" (IEEE P1363), to the internal format used for calculations
    Returns false if the input exceeds the allocated size
    */
    bool Set(const void* pSrc, size_t srcSize) NN_NOEXCEPT;

    /**
    Export to a portable "octet-string" (IEEE P1363), from the internal format used for calculations
    */
    void Get(void* pDst, size_t dstSize) const NN_NOEXCEPT;

    void ReserveStatic( Digit* buffer, int maxDigits ) NN_NOEXCEPT;

    /**
    Compute: out = ( in ^ exp ) mod this
    Returns false if exp or this (the modulus) is zero
    */
    bool ModExp( void* outBlock, const void* inBlock, const BigNum& exp,
                 size_t blockSize, uint32_t* pWorkBuffer, size_t workBufferSize ) const NN_NOEXCEPT;
    void Cleanse() NN_NOEXCEPT;
    void Recount() NN_NOEXCEPT;

    size_t GetSize() const NN_NOEXCEPT;
    bool IsZero() const NN_NOEXCEPT
    {
        return m_DigitsCount == 0;
    }

    /* Static big number operations */
    class DigitAllocator;

    static int  IsZero(const Digit* a, int digits) NN_NOEXCEPT;
    static int  Compare(const Digit* a, const Digit* b, int digits) NN_NOEXCEPT;
    static int  GetDigits(const Digit* a, int digits) NN_NOEXCEPT;
    static int  GetDigitBits(Digit a) NN_NOEXCEPT;
    static void SetZero(Digit* a, int digits) NN_NOEXCEPT;
    static void AssignDigit(Digit* a, Digit b, int digits) NN_NOEXCEPT;
    static void Copy(Digit* a, const Digit* b, int digits) NN_NOEXCEPT;

    static Digit LeftShift(Digit* a, const Digit* b, int c, int digits) NN_NOEXCEPT;
    static Digit RightShift(Digit* a, const Digit* b, int c, int digits) NN_NOEXCEPT;

    static Digit Add(Digit* r, const Digit* a, const Digit* b, int n) NN_NOEXCEPT;
    static Digit Sub(Digit* r, const Digit* a, const Digit* b, int n) NN_NOEXCEPT;
    static Digit AddMult(Digit* r, const Digit* a, int n, Digit factor) NN_NOEXCEPT;
    static Digit SubMult(Digit* a, const Digit* b, const Digit c, const Digit* d,
                         int digits) NN_NOEXCEPT;
    static void  DigitMult(Digit a[2], Digit b, Digit c) NN_NOEXCEPT;
    static void  DigitDiv(Digit* a, const Digit b[2], Digit c) NN_NOEXCEPT;
    static bool  Mult(Digit* a, const Digit* b, const Digit* c, int digits,
                      DigitAllocator* pAllocator) NN_NOEXCEPT;
    static bool  Div(Digit* a, Digit* b, const Digit* c, int cDigits,
                     const Digit* d, int dDigits, DigitAllocator* pAllocator) NN_NOEXCEPT;
    static bool  Mod(Digit* a, const Digit* b, int b_digits, const Digit* c,
                     int c_digits, DigitAllocator* pAllocator) NN_NOEXCEPT;
    static bool  ModMult(Digit* a, const Digit* b, const Digit* c,
                         const Digit* d, int digits, DigitAllocator* pAllocator) NN_NOEXCEPT;
    static bool  ModExp(Digit* a, const Digit* b, const Digit* c, int cDigits,
                        const Digit* d, int dDigits, DigitAllocator* pAllocator) NN_NOEXCEPT;

    /* APIs for ECC operation */
    static void Mult(Digit* a, const Digit* b, const Digit* c, int digits) NN_NOEXCEPT;
    static void Div(Digit* a, Digit* b, const Digit* c, int cDigits,
                    const Digit* d, int dDigits) NN_NOEXCEPT;
    static void Mod(Digit* a, const Digit* b, int b_digits,
                    const Digit* c, int c_digits) NN_NOEXCEPT;
    static void ModInv(Digit* a, const Digit* b,
                       const Digit* c, int digits) NN_NOEXCEPT;
    static void AddMod(Digit* a, const Digit* b,
                       const Digit* c, const Digit* d, int digits) NN_NOEXCEPT;
    static void SubMod(Digit* a, const Digit* b,
                       const Digit* c, const Digit* d, int digits) NN_NOEXCEPT;

private:
    Digit* m_Digits;
    size_t m_DigitsCount;
    size_t m_Capacity;
    int m_Negative;
    int m_Flags;
};

/* conversions between integers, field elements, elliptic points and
   octet strings as per ieee 1363 */
void OctetStringFromDigits( void* outBytes, size_t len,
                            const BigNum::Digit* inDigits, size_t nDigits) NN_NOEXCEPT;
void DigitsFromOctetString( BigNum::Digit* outDigits, size_t nDigits,
                            const void* inBytes, size_t len) NN_NOEXCEPT;

}}} // namespace nn::crypto::detail
