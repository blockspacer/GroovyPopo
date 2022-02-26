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

#include <nn/nn_Macro.h>

namespace nn {
namespace gfx {
namespace detail {

template< typename T >
class CasterBase
{
public:
    typedef T Type;

    explicit CasterBase( T* value ) NN_NOEXCEPT
        : m_Value( value )
    {
    }

    template< typename U >
    operator U*() NN_NOEXCEPT
    {
        return static_cast< U* >( m_Value );
    }

    T* operator ->() NN_NOEXCEPT
    {
        return m_Value;
    }

    template< typename U, typename V >
    V& operator ->*( V U::*pMember )
    {
        return m_Value->*pMember;
    }

    NN_IMPLICIT operator bool() NN_NOEXCEPT
    {
        return m_Value != NULL;
    }

    NN_IMPLICIT operator bool() const NN_NOEXCEPT
    {
        return m_Value != NULL;
    }

    template< typename U >
    bool operator ==( const U& value ) const
    {
        return CompareHelper< U >::Equals( m_Value, value );
    }

    template< typename U >
    bool operator !=( const U& value ) const
    {
        return !( *this == value );
    }

    template< typename U >
    inline friend bool operator ==( const U& lhs, const CasterBase< T >& rhs )
    {
        return rhs == lhs;
    }

    template< typename U >
    inline friend bool operator !=( const U& lhs, const CasterBase< T >& rhs )
    {
        return rhs != lhs;
    }

    bool IsNull() NN_NOEXCEPT
    {
        return m_Value == NULL;
    }

protected:
    T* m_Value;

private:
    template< typename U >
    struct CompareHelper
    {
        static bool Equals( T* lhs, const U& rhs )
        {
            NN_SDK_ASSERT( rhs == NULL );
            NN_UNUSED( rhs );
            return lhs == NULL;
        }
    };
    template< typename U >
    struct CompareHelper< U* >
    {
        static bool Equals( T* lhs, const U* rhs )
        {
            return lhs == rhs;
        }
    };
};

template< typename T >
class Caster : public CasterBase< T >
{
public:
    explicit Caster( T* value ) NN_NOEXCEPT
        : CasterBase< T >( value )
    {
    }

    template< typename U >
    operator U&() NN_NOEXCEPT
    {
        return static_cast< U& >( *CasterBase< T >::m_Value );
    }
};

template< typename T >
class Caster< const T > : public CasterBase< const T >
{
public:
    explicit Caster( const T* value ) NN_NOEXCEPT
        : CasterBase< const T >( value )
    {
    }

    template< typename U >
    operator const U&() NN_NOEXCEPT
    {
        return static_cast< const U& >( *CasterBase< const T >::m_Value );
    }
};

template< typename T >
class Caster< volatile T > : public CasterBase< volatile T >
{
public:
    explicit Caster( volatile T* value ) NN_NOEXCEPT
        : CasterBase< volatile T >( value )
    {
    }

    template< typename U >
    operator volatile U&() NN_NOEXCEPT
    {
        return static_cast< volatile U& >( *CasterBase< volatile T >::m_Value );
    }
};

template< typename T >
class Caster< volatile const T > : public CasterBase< volatile const T >
{
public:
    explicit Caster( volatile const T* value ) NN_NOEXCEPT
        : CasterBase< volatile const T >( value )
    {
    }

    template< typename U >
    operator volatile const U&() NN_NOEXCEPT
    {
        return static_cast< volatile const U& >( *CasterBase< volatile const T >::m_Value );
    }
};

template< typename TData >
class DataContainer : protected TData
{
public:
    typedef TData DataType;

    DataContainer() NN_NOEXCEPT
        : TData()
    {
    }

    Caster< DataType > ToData() NN_NOEXCEPT
    {
        return Caster< DataType >( static_cast< DataType* >( this ) );
    }

    Caster< const DataType > ToData() const NN_NOEXCEPT
    {
        return Caster< const DataType >( static_cast< const DataType* >( this ) );
    }

    Caster< volatile DataType > ToData() volatile NN_NOEXCEPT
    {
        return Caster< volatile DataType >( static_cast< volatile DataType* >( this ) );
    }

    Caster< volatile const DataType > ToData() volatile const NN_NOEXCEPT
    {
        return Caster< volatile const DataType >( static_cast< volatile const DataType* >( this ) );
    }

    static Caster< DataContainer< DataType > > DataToAccessor( DataType* pData ) NN_NOEXCEPT
    {
        return Caster< DataContainer< DataType > >(
            static_cast< DataContainer< DataType >* >( pData ) );
    }

    static Caster< const DataContainer< DataType > > DataToAccessor( const DataType* pData ) NN_NOEXCEPT
    {
        return Caster< const DataContainer< DataType > >(
            static_cast< const DataContainer< DataType >* >( pData ) );
    }

    static Caster< volatile DataContainer< DataType > > DataToAccessor( volatile DataType* pData ) NN_NOEXCEPT
    {
        return Caster< volatile DataContainer< DataType > >(
            static_cast< volatile DataContainer< DataType >* >( pData ) );
    }

    static Caster< volatile const DataContainer< DataType > > DataToAccessor( volatile const DataType* pData ) NN_NOEXCEPT
    {
        return Caster< volatile const DataContainer< DataType > >(
            static_cast< volatile const DataContainer< DataType >* >( pData ) );
    }

    static Caster< DataContainer< DataType > > DataToAccessor( DataType& data ) NN_NOEXCEPT
    {
        return DataToAccessor( &data );
    }

    static Caster< const DataContainer< DataType > > DataToAccessor( const DataType& data ) NN_NOEXCEPT
    {
        return DataToAccessor( &data );
    }

    static Caster< volatile DataContainer< DataType > > DataToAccessor( volatile DataType& data ) NN_NOEXCEPT
    {
        return DataToAccessor( &data );
    }

    static Caster< volatile const DataContainer< DataType > > DataToAccessor( volatile const DataType& data ) NN_NOEXCEPT
    {
        return DataToAccessor( &data );
    }
};

}
}
}
