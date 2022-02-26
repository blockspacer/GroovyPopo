/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_DataTypes.h
///       Helper classes implementing a collection of vector and scalar data
///       types for the NVN texture packager tool.  This support includes
///       packed data types used to represent data for individual pixels.

#ifndef NVNTOOL_DATATYPES_H
#define NVNTOOL_DATATYPES_H

#include <assert.h>
#include <nvn/nvn.h>
#include <stdint.h>

#if !defined(ct_assert)
#define ct_assert(x)
#endif

//
//              NVN FORMAT AND VECTOR DATA TYPE CLASSES
//
// The classes below define a collection of vector and scalar data types of
// various component formats, each of which is associated with an NVN format
// enum that can be used for setting up vertex and/or texture data.
//
// All of these classes are stored in the "nvnTool::texpkg::dt" namespace and
// have GLSL-like typenames (e.g., vec4, u8vec2).  We use "vec1"-like names to
// represent scalars.
//


#if !defined(__cplusplus)
#error nvnTool_DataTypes.h only supports C++!
#endif // #if defined(__cplusplus)

namespace nvnTool {
namespace texpkg {
namespace dt {

//////////////////////////////////////////////////////////////////////////
//
//                  ENCODED COMPONENT REPRESENTATIONS
//
// Defines various classes to represent "encoded" data types whose values are
// encoded forms of "base" data types (e.g., 32-bit int/uint/float) used in
// the API.  All these classes provide are constructors/operators to convert
// between the encoded and base types.
//

// EncodedComponent:  Base template class representing a <Bits>-bit value
// stored with type <StorageType>.  This <isSigned> template parameter is used
// to determine if we need to sign-extend a smaller value when stored in a
// larger signed storage type.
template <typename StorageType, int Bits, bool isSigned>
class EncodedComponent
{
    static const int StorageBits = 8 * sizeof(StorageType);
    ct_assert(StorageBits >= Bits);
protected:
    StorageType m_value;
public:
    // Public typedef allowing generic code to determine the data type used to
    // store components given any type inheriting from this base class.
    typedef StorageType StoredComponentType;

    // Returns the number of bits used for the stored type, for generic code.
    static int nBits()                  { return Bits; }

    // Get the raw bits of <m_value>, stripping off any padding (or sign
    // extension).
    uint32_t getBits() const
    {
        if (Bits == StorageBits) {
            return m_value;
        } else {
            return uint32_t(m_value) & (0xFFFFFFFFU >> (32 - Bits));
        }
    }

    // Set the raw bits of <m_value> from the unsigned integer <bits>.  If we
    // are using a signed type for storage, use shifts to sign-extend <bits>.
    void setBits(uint32_t bits)
    {
        m_value = bits;
        if (isSigned && Bits != StorageBits) {
            m_value <<= (StorageBits - Bits);
            m_value >>= (StorageBits - Bits);
        }
    }
};

// float16:  Class for 16-bit floating-point components.
class float16 : public EncodedComponent<uint16_t, 16, false>
{
public:
    float16() {}
    float16(double f);
    operator double() const;
};

// ComponentUnorm:  Template class defining an unsigned normalized component
// type stored with <Bits> bits using storage type <StorageType>.
template <typename StorageType, int Bits>
class ComponentUnorm : public EncodedComponent<StorageType, Bits, false>
{
private:
    static const StorageType max_value = (1U << Bits) - 1;

public:
    ComponentUnorm() {}

    ComponentUnorm(double v)
    {
        ct_assert(Bits <= 16);
        if (v <= 0.0) {
            this->m_value = 0;
        } else if (v < +1.0) {
            this->m_value = StorageType(v * (double)(max_value) + 0.5);
        } else {
            this->m_value = max_value;
        }
    }

    operator double() const
    {
        ct_assert(Bits <= 16);
        return (double)(this->m_value) / (double)max_value;
    }
};

typedef ComponentUnorm<uint8_t, 1> unorm1;
typedef ComponentUnorm<uint8_t, 2> unorm2;
typedef ComponentUnorm<uint8_t, 4> unorm4;
typedef ComponentUnorm<uint8_t, 5> unorm5;
typedef ComponentUnorm<uint8_t, 6> unorm6;
typedef ComponentUnorm<uint8_t, 8> unorm8;
typedef ComponentUnorm<uint16_t, 10> unorm10;
typedef ComponentUnorm<uint16_t, 16> unorm16;

// Dummy type for unused fields in packed UNORM vectors.
typedef ComponentUnorm<uint8_t, 8> unorm0;

// ComponentSnorm:  Template class defining a signed normalized component
// type stored with <Bits> bits using storage type <StorageType>.
template <typename StorageType, int Bits>
class ComponentSnorm : public EncodedComponent<StorageType, Bits, true>
{
private:
    static const StorageType max_value = (1U << (Bits - 1)) - 1;
public:
    ComponentSnorm() {}

    ComponentSnorm(double v)
    {
        ct_assert(Bits <= 16);
        if (v <= -1.0) {
            this->m_value = -max_value;
        } else if (v < 0.0) {
            this->m_value = -StorageType(-v * double(max_value) + 0.5);
        } else if (v < +1.0) {
            this->m_value = +StorageType(v * double(max_value) + 0.5);
        } else {
            this->m_value = +max_value;
        }
    }

    operator double() const
    {
        ct_assert(Bits <= 16);
        double f = (double)(this->m_value) / (double)max_value;
        if (f < -1.0) {
            f = -1.0;   // map the "overflowed" -128 in 8-bit to -1.0
        }
        return f;
    }
};

typedef ComponentSnorm<int8_t,  2> snorm2;
typedef ComponentSnorm<int8_t,  8> snorm8;
typedef ComponentSnorm<int16_t, 10> snorm10;
typedef ComponentSnorm<int16_t, 16> snorm16;

// ComponentCast:  Template class defining an encoded type to be converted to
// the external type via a straight C cast.
template <typename StorageType, int Bits, typename ExternalType, bool isSigned>
class ComponentCast : public EncodedComponent < StorageType, Bits, isSigned >
{
public:
    ComponentCast() {}
    ComponentCast(ExternalType v)   { this->m_value = StorageType(v); }
    operator ExternalType() const   { return ExternalType(this->m_value); }
};

typedef ComponentCast<uint8_t,   2, uint32_t, false> uint2;
typedef ComponentCast<uint8_t,   8, uint32_t, false> uint8;
typedef ComponentCast<uint16_t, 10, uint32_t, false> uint10;
typedef ComponentCast<uint16_t, 16, uint32_t, false> uint16;

typedef ComponentCast<int8_t,   2, int32_t, true> int2;
typedef ComponentCast<int8_t,   8, int32_t, true> int8;
typedef ComponentCast<int16_t, 10, int32_t, true> int10;
typedef ComponentCast<int16_t, 16, int32_t, true> int16;

typedef ComponentCast<uint8_t,   2, float, false> uint2_to_f;
typedef ComponentCast<uint8_t,   8, float, false> uint8_to_f;
typedef ComponentCast<uint16_t, 10, float, false> uint10_to_f;
typedef ComponentCast<uint16_t, 16, float, false> uint16_to_f;
typedef ComponentCast<uint32_t, 32, float, false> uint32_to_f;

typedef ComponentCast<int8_t,   2, float, true> int2_to_f;
typedef ComponentCast<int8_t,   8, float, true> int8_to_f;
typedef ComponentCast<int16_t, 10, float, true> int10_to_f;
typedef ComponentCast<int16_t, 16, float, true> int16_to_f;
typedef ComponentCast<int32_t, 32, float, true> int32_to_f;


//////////////////////////////////////////////////////////////////////////
//
//                      GENERIC VECTOR CLASSES
//
// Template class defining generic <N>-component vectors with component type
// <ComponentType>.  This class is used only for vectors of "basic" types
// (e.g., signed/unsigned integers, float) whose values will be used by the
// API in the exact same way they are stored.
//
template <typename ComponentType, int N>
class Vector
{
private:
    typedef Vector<ComponentType, N> Type;
    typedef Vector<bool, N> BoolType;
protected:
    ComponentType m_data[N];
public:
    // Public typedefs and a flag allowing generic code to determine the data
    // types used to represent this class.
    typedef ComponentType   StoredComponentType;
    typedef ComponentType   ExternalScalarType;
    typedef Type            ExternalVectorType;
    static const bool isPacked = false;

    Vector(void) {}

    // Scalar-to-vector constructor.
    Vector(const ComponentType c)
    {
        for (int i = 0; i < N; i++) {
            m_data[i] = c;
        }
    }

    // Constructors for 2..4 scalars.  We only support constructors with <N>
    // scalar arguments.
    Vector(const ComponentType x, const ComponentType y)
    {
        ct_assert(N == 2);
        m_data[0] = x;
        m_data[1] = y;
    }
    Vector(const ComponentType x, const ComponentType y, const ComponentType z)
    {
        ct_assert(N == 3);
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
    }
    Vector(const ComponentType x, const ComponentType y, const ComponentType z, const ComponentType w)
    {
        ct_assert(N == 4);
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
        m_data[3] = w;
    }

    // Array element operators.
    ComponentType & operator[](int c)                 { return m_data[c]; }
    const ComponentType & operator[](int c) const     { return m_data[c]; }

    // Alternate component setting methods.  These work on all of our vector
    // types, including packed vectors.
    void setComponent(int c, ComponentType v)         { m_data[c] = v; }
    void setX(ComponentType v)                        { ct_assert(N >= 1); m_data[0] = v; }
    void setY(ComponentType v)                        { ct_assert(N >= 2); m_data[1] = v; }
    void setZ(ComponentType v)                        { ct_assert(N >= 3); m_data[2] = v; }
    void setW(ComponentType v)                        { ct_assert(N >= 4); m_data[3] = v; }

    // Component selection methods.
    ComponentType x() const                           { ct_assert(N >= 1); return m_data[0]; }
    ComponentType y() const                           { ct_assert(N >= 2); return m_data[1]; }
    ComponentType z() const                           { ct_assert(N >= 3); return m_data[2]; }
    ComponentType w() const                           { ct_assert(N >= 4); return m_data[3]; }

    // Conversion-to-pointer methods.
    ComponentType *ptr()                              { return m_data; }
    const ComponentType *ptr() const                  { return m_data; }

    // Vector-wise unary and binary operators.
    Type operator +() const
    {
        Type result;
        for (int i = 0; i < N; i++) result[i] = m_data[i];
        return result;
    }
    Type operator -() const
    {
        Type result;
        for (int i = 0; i < N; i++) result[i] = -m_data[i];
        return result;
    }

    Type operator +(const Type &other) const
    {
        Type result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] + other[i];
        return result;
    }
    Type operator -(const Type &other) const
    {
        Type result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] - other[i];
        return result;
    }
    Type operator *(const Type &other) const
    {
        Type result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] * other[i];
        return result;
    }
    Type operator /(const Type &other) const
    {
        Type result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] / other[i];
        return result;
    }
    BoolType operator ==(const Type &other) const
    {
        BoolType result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] == other[i];
        return result;
    }
    BoolType operator !=(const Type &other) const
    {
        BoolType result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] != other[i];
        return result;
    }
    BoolType operator <(const Type &other) const
    {
        BoolType result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] < other[i];
        return result;
    }
    BoolType operator <=(const Type &other) const
    {
        BoolType result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] <= other[i];
        return result;
    }
    BoolType operator >(const Type &other) const
    {
        BoolType result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] > other[i];
        return result;
    }
    BoolType operator >=(const Type &other) const
    {
        BoolType result;
        for (int i = 0; i < N; i++) result[i] = m_data[i] >= other[i];
        return result;
    }

    // Returns the number of bits consumed by a given vector component.
    static int componentBits(int c)     { return 8 * sizeof(ComponentType); }
};

// Boolean vector types.
typedef Vector<bool, 1> bvec1;
typedef Vector<bool, 2> bvec2;
typedef Vector<bool, 3> bvec3;
typedef Vector<bool, 4> bvec4;

// Basic floating-point types (FP32).
typedef Vector<float, 1> vec1;
typedef Vector<float, 2> vec2;
typedef Vector<float, 3> vec3;
typedef Vector<float, 4> vec4;

// Basic unsigned integer types (not normalized or converted to float).
typedef Vector<uint32_t, 1> uvec1;
typedef Vector<uint32_t, 2> uvec2;
typedef Vector<uint32_t, 3> uvec3;
typedef Vector<uint32_t, 4> uvec4;

// Basic signed integer types (not normalized or converted to float).
typedef Vector<int32_t, 1> ivec1;
typedef Vector<int32_t, 2> ivec2;
typedef Vector<int32_t, 3> ivec3;
typedef Vector<int32_t, 4> ivec4;

// Basic double precision floating-point types (FP64).
typedef Vector<double, 1> dvec1;
typedef Vector<double, 2> dvec2;
typedef Vector<double, 3> dvec3;
typedef Vector<double, 4> dvec4;


//////////////////////////////////////////////////////////////////////////
//
//                      STORAGE VECTOR CLASSES
//
// Template class representing vectors where components of type
// <ExternalComponent> are represented with (smaller) components of type
// <StorageComponent>.  <StorageComponent> is expected to be one of the
// encoded component classes above, which include constructors/operators to
// convert between storage and external types.
//
// The only functionality we provide here is the ability to convert the
// storage vector to an equivalent "external" vector; we don't ever operate on
// these vectors directly.
//
template < typename StorageComponent, typename ExternalComponent, int N >
class StorageVector
{
private:
    typedef Vector<ExternalComponent, N> ExternalType;
    StorageComponent m_data[N];
public:
    // Public typedef and a flag allowing generic code to determine the data
    // types used for storage in the encoded vector classes below.
    typedef typename StorageComponent::StoredComponentType StoredComponentType;
    static const bool isPacked = false;

    StorageVector() {}

    StorageVector(ExternalType v)
    {
        for (int i = 0; i < N; i++) {
            m_data[i] = StorageComponent(v[i]);
        }
    }

    operator ExternalType() const
    {
        ExternalType result;
        for (int i = 0; i < N; i++) {
            result[i] = ExternalComponent(m_data[i]);
        }
        return result;
    }

    // Array element operators.
    StorageComponent &operator[](int c)                 { return m_data[c]; }
    const StorageComponent &operator[] (int c) const    { return m_data[c]; }

    // Alternate component setting methods.  These work on all of our vector
    // types, including packed vectors.
    void setComponent(int c, ExternalComponent v)       { m_data[c] = v; }
    void setX(ExternalComponent v)                      { ct_assert(N >= 1); m_data[0] = v; }
    void setY(ExternalComponent v)                      { ct_assert(N >= 2); m_data[1] = v; }
    void setZ(ExternalComponent v)                      { ct_assert(N >= 3); m_data[2] = v; }
    void setW(ExternalComponent v)                      { ct_assert(N >= 4); m_data[3] = v; }

    // Returns the number of bits consumed by a given vector component.
    static int componentBits(int c)     { return StorageComponent::nBits(); }
};


//////////////////////////////////////////////////////////////////////////
//
//                      PACKED VECTOR CLASSES
//
// Template classes representing vectors stored packed in a single value of
// type <StoredType>, where each component represents a value with the
// "external" type of <ExternalComponent>.  The (up to) four components are
// stored with type <RType>, <GType>, <BType>, and <AType>.  These types are
// expected to be one of the encoded component classes above, which include
// constructors/operators to convert between storage and external types.
//
// The only functionality we provide here is the ability to convert the
// storage vector to an equivalent "external" vector; we don't ever operate on
// these vectors directly.
//

// Enum indicating whether components are packed in RGBA order (red in LSBs),
// BGRA order (red in MSBs) or ABGR (red in MSB).
enum PackedVectorOrder {
    PackedVectorOrderRGBA,
    PackedVectorOrderBGRA,
    PackedVectorOrderABGR,
};

template < typename StoredType, typename ExternalComponent,
           int N, typename RType, typename GType, typename BType, typename AType,
           PackedVectorOrder order = PackedVectorOrderRGBA >
class PackedVector
{
private:
    typedef Vector<ExternalComponent, N> ExternalType;
    StoredType m_data;
public:
    // Public typedef and a flag allowing generic code to determine the data
    // types used for storage in the encoded vector classes below.
    typedef StoredType StoredComponentType;
    static const bool isPacked = true;

    PackedVector() {}

    PackedVector(ExternalType v)
    {
        uint32_t bits;
        int shift = 0;
        m_data = 0;

        if (order == PackedVectorOrderRGBA) {
            if (N >= 1) {
                RType rvalue = v[0];
                bits = rvalue.getBits();
                m_data |= (bits << shift);
                shift += RType::nBits();
            }
            if (N >= 2) {
                GType gvalue = v[1];
                bits = gvalue.getBits();
                m_data |= (bits << shift);
                shift += GType::nBits();
            }
            if (N >= 3) {
                BType bvalue = v[2];
                bits = bvalue.getBits();
                m_data |= (bits << shift);
                shift += BType::nBits();
            }
            if (N >= 4) {
                AType value = v[3];
                uint32_t bits = value.getBits();
                m_data |= (bits << shift);
                shift += AType::nBits();
            }
        } else if (order == PackedVectorOrderBGRA) {
            assert(N >= 3);

            BType bvalue = v[2];
            bits = bvalue.getBits();
            m_data |= (bits << shift);
            shift += BType::nBits();

            GType gvalue = v[1];
            bits = gvalue.getBits();
            m_data |= (bits << shift);
            shift += GType::nBits();

            RType rvalue = v[0];
            bits = rvalue.getBits();
            m_data |= (bits << shift);
            shift += RType::nBits();

            if (N >= 4) {
                AType value = v[3];
                uint32_t bits = value.getBits();
                m_data |= (bits << shift);
                shift += AType::nBits();
            }
        } else {
            // PackedVectorOrderABGR
            assert(N >= 4);

            AType value = v[3];
            uint32_t bits = value.getBits();
            m_data |= (bits << shift);
            shift += AType::nBits();

            BType bvalue = v[2];
            bits = bvalue.getBits();
            m_data |= (bits << shift);
            shift += BType::nBits();

            GType gvalue = v[1];
            bits = gvalue.getBits();
            m_data |= (bits << shift);
            shift += GType::nBits();

            RType rvalue = v[0];
            bits = rvalue.getBits();
            m_data |= (bits << shift);
            shift += RType::nBits();
        }
    }

    operator ExternalType() const
    {
        ExternalType result;
        int shift = 0, size;

        if (order == PackedVectorOrderRGBA) {
            if (N >= 1) {
                RType rvalue;
                size = RType::nBits();
                rvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
                result[0] = ExternalComponent(rvalue);
                shift += size;
            }
            if (N >= 2) {
                GType gvalue;
                size = GType::nBits();
                gvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
                result[1] = ExternalComponent(gvalue);
                shift += size;
            }
            if (N >= 3) {
                BType bvalue;
                size = BType::nBits();
                bvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
                result[2] = ExternalComponent(bvalue);
                shift += size;
            }
            if (N >= 4) {
                AType avalue;
                size = AType::nBits();
                avalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
                result[3] = ExternalComponent(avalue);
                shift += size;
            }
        } else if (order == PackedVectorOrderBGRA) {
            assert(N >= 3);

            BType bvalue;
            size = BType::nBits();
            bvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[2] = ExternalComponent(bvalue);
            shift += size;

            GType gvalue;
            size = GType::nBits();
            gvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[1] = ExternalComponent(gvalue);
            shift += size;

            RType rvalue;
            size = RType::nBits();
            rvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[0] = ExternalComponent(rvalue);
            shift += size;

            if (N >= 4) {
                AType avalue;
                size = AType::nBits();
                avalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
                result[3] = ExternalComponent(avalue);
                shift += size;
            }
        } else {
            // PackedVectorOrderABGR
            assert(N >= 4);

            AType avalue;
            size = AType::nBits();
            avalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[3] = ExternalComponent(avalue);
            shift += size;

            BType bvalue;
            size = BType::nBits();
            bvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[2] = ExternalComponent(bvalue);
            shift += size;

            GType gvalue;
            size = GType::nBits();
            gvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[1] = ExternalComponent(gvalue);
            shift += size;

            RType rvalue;
            size = RType::nBits();
            rvalue.setBits((m_data >> shift) & (0xFFFFFFFFU >> (32 - size)));
            result[0] = ExternalComponent(rvalue);
            shift += size;
        }

        return result;
    }

    // The array subscript operator here returns a converted *value* of the
    // external type, and not a *reference* (which could only be to the
    // storage type).
    ExternalComponent operator[](int c) const
    {
        ExternalType unpacked = *this;
        return unpacked[c];
    }

    // Alternate component setting methods.  These work on all of our vector
    // types, including these packed vectors.
    void setComponent(int c, ExternalComponent v)
    {
        ExternalType unpacked = *this;
        unpacked[c] = v;
        *this = unpacked;
    }
    void setX(ExternalComponent v)      { ct_assert(N >= 1); setComponent(0, v); }
    void setY(ExternalComponent v)      { ct_assert(N >= 2); setComponent(1, v); }
    void setZ(ExternalComponent v)      { ct_assert(N >= 3); setComponent(2, v); }
    void setW(ExternalComponent v)      { ct_assert(N >= 4); setComponent(3, v); }

    // Returns the number of bits consumed by a given vector component.
    static int componentBits(int c)
    {
        assert(c < 4);
        switch (c) {
        case 0:     return RType::nBits();
        case 1:     return GType::nBits();
        case 2:     return BType::nBits();
        case 3:     return AType::nBits();
        }
        return 0;
    }
};


//////////////////////////////////////////////////////////////////////////
//
//                      ENCODED VECTOR CLASSES
//
// Template class defining generic <N>-component vectors with nominal
// component type <ExternalComponent> stored using the type <StorageType>,
// which is one of the storage or packed vector types defined above.
//
// The only functionality we provide here is the ability to convert the
// encoded vector to an equivalent "external" vector, and various operators
// that use the external type.
//

template <typename StorageType, typename ExternalComponent, int N>
class EncodedVector : public StorageType
{
private:
    typedef Vector<ExternalComponent, N> ExternalType;
    typedef Vector<bool, N> BoolType;
public:
    // Public typedefs and a flag allowing generic code to determine the data
    // types used to represent this class.  We also inherit an additional type
    // and flag from <StorageType>.
    typedef ExternalComponent   ExternalScalarType;
    typedef ExternalType        ExternalVectorType;

    EncodedVector() {}
    EncodedVector(ExternalComponent c)  : StorageType(ExternalType(c)) {}
    EncodedVector(ExternalType v)       : StorageType(v) {}

    // Constructors for 2..4 scalars.  We only support constructors with <N>
    // scalar arguments.
    EncodedVector(const ExternalComponent x, const ExternalComponent y) :
        StorageType(ExternalType(x, y)) {}
    EncodedVector(const ExternalComponent x, const ExternalComponent y,
                  const ExternalComponent z) :
        StorageType(ExternalType(x, y, z)) {}
    EncodedVector(const ExternalComponent x, const ExternalComponent y,
                  const ExternalComponent z, const ExternalComponent w) :
        StorageType(ExternalType(x, y, z, w)) {}

    // Component selection methods.
    ExternalComponent x() const             { ct_assert(N >= 1); return *this[0]; }
    ExternalComponent y() const             { ct_assert(N >= 2); return *this[1]; }
    ExternalComponent z() const             { ct_assert(N >= 3); return *this[2]; }
    ExternalComponent w() const             { ct_assert(N >= 4); return *this[3]; }

    // Various other operators simply convert this vector to the external type
    // and then operate on it with the external type.
    ExternalType operator +() const
        { return +ExternalType(*this); }
    ExternalType operator -() const
        { return -ExternalType(*this); }
    ExternalType operator +(const ExternalType &other) const
        { return ExternalType(*this) + other; }
    ExternalType operator -(const ExternalType &other) const
        { return ExternalType(*this) - other; }
    ExternalType operator *(const ExternalType &other) const
        { return ExternalType(*this) * other; }
    ExternalType operator /(const ExternalType &other) const
        { return ExternalType(*this) / other; }
    BoolType operator ==(const ExternalType &other) const
        { return ExternalType(*this) == other; }
    BoolType operator !=(const ExternalType &other) const
        { return ExternalType(*this) != other; }
    BoolType operator <(const ExternalType &other) const
        { return ExternalType(*this) < other; }
    BoolType operator <=(const ExternalType &other) const
        { return ExternalType(*this) <= other; }
    BoolType operator >(const ExternalType &other) const
        { return ExternalType(*this) > other; }
    BoolType operator >=(const ExternalType &other) const
        { return ExternalType(*this) >= other; }

    // Returns the number of bits consumed by a given vector component.
    static int componentBits(int c)     { return StorageType::componentBits(c); }
};

// Macro to stamp out typedefs one- to four-component vectors with stored
// component type <_scomp> and external component type <_ecomp>.  The defined
// type names will be formed by concatenating <_pfx> and the suffixes "vec1"
// through "vec4".
#define MKTYPES(_scomp, _ecomp, _pfx)                                   \
    typedef EncodedVector<StorageVector<_scomp, _ecomp, 1>, _ecomp, 1>  \
        _pfx ## vec1;                                                   \
    typedef EncodedVector<StorageVector<_scomp, _ecomp, 2>, _ecomp, 2>  \
        _pfx ## vec2;                                                   \
    typedef EncodedVector<StorageVector<_scomp, _ecomp, 3>, _ecomp, 3>  \
        _pfx ## vec3;                                                   \
    typedef EncodedVector<StorageVector<_scomp, _ecomp, 4>, _ecomp, 4>  \
        _pfx ## vec4

MKTYPES(unorm8,      float, u8n);
MKTYPES(unorm16,     float, u16n);
MKTYPES(snorm8,      float, i8n);
MKTYPES(snorm16,     float, i16n);
MKTYPES(uint8,       uint32_t, u8);
MKTYPES(uint16,      uint32_t, u16);
MKTYPES(int8,        int32_t, i8);
MKTYPES(int16,       int32_t, i16);
MKTYPES(uint8_to_f,  float, u2f8);
MKTYPES(uint16_to_f, float, u2f16);
MKTYPES(uint32_to_f, float, u2f32);
MKTYPES(int8_to_f,   float, i2f8);
MKTYPES(int16_to_f,  float, i2f16);
MKTYPES(int32_to_f,  float, i2f32);
MKTYPES(float16,     float, f16);

#undef MKTYPES

// Macro to stamp out typedefs for <_N>-component packed vectors with stored
// packed type <_ptype> and external component type <_ecomp>.  The individual
// components are stored with types <_rt>, <_gt>, <_bt>, and <_at>.  The type
// names will be formed by appending <_sfx> to a basic vector type (e.g.,
// "vec4_rgba4").
#define MKTYPE(_ptype, _etype, _N, _rt, _gt, _bt, _at, _sfx)                    \
    typedef EncodedVector<PackedVector<_ptype, _etype, _N, _rt, _gt, _bt, _at>, \
                                       _etype, _N> vec ## _N ## _ ## _sfx

MKTYPE(uint16_t, float,    4, unorm4,  unorm4,  unorm4,  unorm4, rgba4);
MKTYPE(uint16_t, float,    3, unorm5,  unorm5,  unorm5,  unorm0, rgb5);
MKTYPE(uint16_t, float,    4, unorm5,  unorm5,  unorm5,  unorm1, rgb5a1);
MKTYPE(uint16_t, float,    3, unorm5,  unorm6,  unorm5,  unorm0, rgb565);
MKTYPE(uint32_t, float,    4, unorm10, unorm10, unorm10, unorm2, rgb10a2);
MKTYPE(uint32_t, uint32_t, 4, uint10,  uint10,  uint10,  uint2,  rgb10a2ui);
MKTYPE(uint32_t, float,    4, snorm10, snorm10, snorm10, snorm2, rgb10a2sn);
MKTYPE(uint32_t, int32_t,  4, int10,   int10,   int10,   int2,   rgb10a2i);

MKTYPE(uint32_t, float, 4,
       uint10_to_f, uint10_to_f, uint10_to_f, uint2_to_f,
       rgb10a2ui_to_f);
MKTYPE(uint32_t, float, 4,
       int10_to_f, int10_to_f, int10_to_f, int2_to_f,
       rgb10a2i_to_f);

// !!! Add support for packed float types (11/11/10, 9/9/9/E5)!

#undef MKTYPE

// Macro to stamp out packed types with BGRA component ordering; otherwise,
// identical to the macro above.
#define MKTYPE(_ptype, _etype, _N, _rt, _gt, _bt, _at, _sfx)                    \
    typedef EncodedVector<PackedVector<_ptype, _etype, _N, _rt, _gt, _bt, _at,  \
                                       PackedVectorOrderBGRA>,                  \
                          _etype, _N> vec ## _N ## _ ## _sfx

// Macros
MKTYPE(uint16_t, float, 3, unorm5, unorm5, unorm5, unorm0, bgr5);
MKTYPE(uint16_t, float, 4, unorm5, unorm5, unorm5, unorm1, bgr5a1);
MKTYPE(uint16_t, float, 3, unorm5, unorm6, unorm5, unorm0, bgr565);

#undef MKTYPE

// Macro to stamp out packed types with ABGR component ordering; otherwise,
// identical to the macro above.
#define MKTYPE(_ptype, _etype, _N, _rt, _gt, _bt, _at, _sfx)                    \
    typedef EncodedVector<PackedVector<_ptype, _etype, _N, _rt, _gt, _bt, _at,  \
                                       PackedVectorOrderABGR>,                  \
                          _etype, _N> vec ## _N ## _ ## _sfx

// Macros
MKTYPE(uint16_t, float, 4, unorm5, unorm5, unorm5, unorm1, a1bgr5);

#undef MKTYPE


//////////////////////////////////////////////////////////////////////////
//
//                      SCALAR/VECTOR OPERATORS
//
// Define additional binary operators working on a scalar/vector pair, so we can
// use things like:
//
//    dt::vec4 a, b;
//    b = 2.0 * a;
//
// When the first argument is one of our vector types, the operators already
// defined in the vector classes are used.  When the first operator is a
// primitive type, we need to use operator functions.

// Stamp out various binary operators for generic and encoded vector types;
// <_vectype> here specifies the external component type of the vector;
// <_sctype> specifies the component type of the scalar.
#define MKOPERATORS(_vctype, _sctype)          \
    MKOPERATOR(+, _vctype, _vctype, _sctype)   \
    MKOPERATOR(-, _vctype, _vctype, _sctype)   \
    MKOPERATOR(*, _vctype, _vctype, _sctype)   \
    MKOPERATOR(/, _vctype, _vctype, _sctype)   \
    MKOPERATOR(==, bool, _vctype, _sctype)     \
    MKOPERATOR(!=, bool, _vctype, _sctype)     \
    MKOPERATOR(<,  bool, _vctype, _sctype)     \
    MKOPERATOR(<=, bool, _vctype, _sctype)     \
    MKOPERATOR(> , bool, _vctype, _sctype)     \
    MKOPERATOR(>=, bool, _vctype, _sctype)

// Set up a template function for operator <_op>, returning a vector with
// component type <_rctype>.
#define MKOPERATOR(_op, _rctype, _vctype, _sctype)                                  \
    template <typename CT, int N>                                                   \
    Vector<_rctype, N> operator _op(const CT &a, const Vector <CT, N> &b)           \
    {                                                                               \
        return Vector<CT,N>(a) _op b;                                               \
    }                                                                               \
    template <typename ST, typename CT, int N>                                      \
    Vector<_rctype, N> operator _op(const CT &a, const EncodedVector<ST, CT, N> &b) \
    {                                                                               \
        return Vector<CT,N>(a) _op b;                                               \
    }
MKOPERATORS(CT, CT);
#undef MKOPERATOR

// Set up template functions for operator <_op> for scalar/vector and
// vector/scalar pairs of mixed type.  We define these functions so we can
// write code like:
//
//     dt::vec4 a, b;
//     b = 2 * a + 1;
//
// instead of being forced to use "2.0f" and "1.0f".
#define MKOPERATOR(_op, _rctype, _vctype, _sctype)                              \
    template <int N>                                                            \
    Vector<_rctype, N> operator _op(const _sctype &a,                           \
                                    const Vector <_vctype, N> &b)               \
    {                                                                           \
        return Vector<_vctype,N>(/*(_vctype)*/(a)) _op b;                       \
    }                                                                           \
    template <int N>                                                            \
    Vector<_rctype, N> operator _op(const Vector <_vctype, N> &a,               \
                                    const _sctype &b)                           \
    {                                                                           \
        return a _op Vector<_vctype,N>(/*(_vctype)*/(b));                       \
    }                                                                           \
    template <typename ST, int N>                                               \
    Vector<_rctype, N> operator _op(const _sctype &a,                           \
                                    const EncodedVector <ST, _vctype, N> &b)    \
    {                                                                           \
        return Vector<_vctype,N>(/*(_vctype)*/(a)) _op b;                       \
    }                                                                           \
    template <typename ST, int N>                                               \
    Vector<_rctype, N> operator _op(const EncodedVector <ST, _vctype, N> &a,    \
                                    const _sctype &b)                           \
    {                                                                           \
        return a _op Vector<_vctype,N>(/*(_vctype)*/(b));                       \
    }

MKOPERATORS(float, double);
MKOPERATORS(float, float);
MKOPERATORS(float, int);
MKOPERATORS(float, unsigned int);
MKOPERATORS(unsigned int, int);
MKOPERATORS(unsigned int, unsigned int);
MKOPERATORS(int, unsigned int);
MKOPERATORS(int, int);

#undef MKOPERATOR

//////////////////////////////////////////////////////////////////////////
//
//                   BOOLEAN VECTOR OPERATORS
//
// Define operators "&&", "||", and "!" for boolean vector types.
//
template <int N>
Vector<bool,N> operator &&(const Vector <bool,N> &a, const Vector <bool,N> &b)
{
    Vector<bool,N> result;
    for (int i = 0; i < N; i++) { result[i] = a[i] && b[i]; }
    return result;
}

template <int N>
Vector<bool, N> operator ||(const Vector <bool, N> &a, const Vector <bool, N> &b)
{
    Vector<bool, N> result;
    for (int i = 0; i < N; i++) { result[i] = a[i] || b[i]; }
    return result;
}

template <int N>
Vector<bool, N> operator !(const Vector <bool, N> &a)
{
    Vector<bool, N> result;
    for (int i = 0; i < N; i++) { result[i] = !a[i]; }
    return result;
}

//////////////////////////////////////////////////////////////////////////
//
//                   VECTOR BUILT-IN FUNCTIONS
//
// Various built-in functions operating on our vector types.
//

// any:  Returns true if any component in a boolean vector is true.
template <int N>
inline bool any(const Vector<bool, N> v)
{
    for (int i = 0; i < N; i++) {
        if (v[i]) return true;
    }
    return false;
}

// all:  Returns true if all components in a boolean vector are true.
template <int N>
inline bool all(const Vector<bool, N> v)
{
    for (int i = 0; i < N; i++) {
        if (!v[i]) return false;
    }
    return true;
}

// abs:  Returns the absolute value of a floating-point vector.
template <int N>
inline Vector<float, N> abs(Vector<float, N> v)
{
    Vector<float, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = fabsf(v[i]);
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////
//
//                       VECTOR TYPE TRAITS
//
// Define a template "traits" class that can be used to tack-on extra
// information for our vector type classes.
//
template <typename T>
class traits {
public:
    // Returns the NVN format enum corresponding to the data type <T>.
    static NVNformat nvnFormat();
};

// Returns the NVN format enum corresponding to the data type of argument <t>.
template <typename T> NVNformat nvnFormat(const T& t)
{
    return traits<T>::nvnFormat();
}

} // namespace dt
} // namespace texpkg
} // namespace nvnTool

#endif // #ifndef NVNTOOL_DATATYPES_H
