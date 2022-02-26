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

#include <nn/gfx/detail/gfx_Declare.h>

#define NN_GFX_OPTION_EXISTS(base, name) \
    Has##name##Impl< base >::value

#define NN_GFX_OPTION(name) \
    template< typename TTmpImpl > \
    struct Has##name##Impl \
    { \
        template< typename TCheck > static uint8_t Check( typename TCheck::name* = 0 ); \
        template< typename TCheck > static uint16_t Check( ... ); \
        static const bool value = sizeof( Check< TTmpImpl >( NULL ) ) == sizeof( uint8_t ); \
    }

#define NN_GFX_OPTION_FUNCTION(ret, name, ...) \
    template< typename TTmpImpl, ret (*)( __VA_ARGS__ ) > struct name##ImplOption; \
    NN_GFX_OPTION_FUNCTION_IMPL( name )

#define NN_GFX_OPTION_MEMBER_FUNCTION(ret, name, qualifier, ...) \
    template< typename TTmpImpl, ret (TTmpImpl::*)( __VA_ARGS__ ) qualifier > struct name##ImplOption; \
    NN_GFX_OPTION_FUNCTION_IMPL( name )

#define NN_GFX_OPTION_FUNCTION_IMPL(name) \
    template< typename TTmpImpl > \
    struct Has##name##Impl \
    { \
        template< typename TCheck > static uint8_t Check( name##ImplOption< \
            typename TCheck::MemoryImplClass, &TCheck::name >* = 0 ); \
        template< typename TCheck > static uint8_t Check( name##ImplOption< \
            TCheck, &TCheck::name >* = 0 ); \
        template< typename TCheck > static uint16_t Check( ... ); \
        static const bool value = sizeof( Check< TTmpImpl >( NULL ) ) == sizeof( uint8_t ); \
    }

#define NN_GFX_ARRAY_LENGTH(ary) (sizeof(*nn::gfx::detail::ArrayLengthImpl(ary)))

namespace nn {
namespace gfx {
namespace detail {

template< bool Cond, typename TThen, typename TElse >
struct Conditional
{
    typedef TElse Type;
};
template< typename TThen, typename TElse >
struct Conditional< true, TThen, TElse >
{
    typedef TThen Type;
};

template< typename TType, typename TValue = uint64_t >
union Ptr
{
    typedef TType Type;
    typedef TValue ValueType;
    typedef TType* PtrType;
    typedef Ptr< Type, ValueType > SelfType;

    PtrType ptr;
    ValueType value;

    PtrType operator ->() const
    {
        return ptr;
    }

    PtrType* operator &()
    {
        return &ptr;
    }

    operator PtrType() const
    {
        return ptr;
    }

    operator bool() const
    {
        return ptr != NULL;
    }

    template< typename T >
    operator T() const
    {
        return static_cast< T >( ptr );
    }

    SelfType& operator =( PtrType p )
    {
        ptr = p;
        return *this;
    }
};

template< typename T, size_t N >
char (*ArrayLengthImpl( T (&)[ N ] ) )[ N ];

struct InfoHelper
{
    template< typename TInfo, typename Info, void( *)( TInfo*, const Info& ) >
    struct ImplOption;
    template< typename TInfo, typename Info >
    static void ConvertFromInfoImpl( TInfo* pDst, const Info& info, ImplOption< TInfo, Info, &TInfo::ConvertFromInfo >* = 0 ) NN_NOEXCEPT
    {
        return TInfo::ConvertFromInfo( pDst, info );
    }
    template< typename TInfo, typename Info >
    static void ConvertFromInfoImpl( TInfo* pDst, const Info& info, ... ) NN_NOEXCEPT
    {
        *pDst = info;
    }
    template< typename TInfo, typename Info >
    static void ConvertFromInfo( TInfo* pDst, const Info& info ) NN_NOEXCEPT
    {
        return ConvertFromInfoImpl< TInfo, Info >( pDst, info, 0 );
    }
};

struct DebugLabelHelper
{
    template< typename TImpl, typename TTarget, void ( TImpl::* )( DeviceImpl< TTarget >*, const char* label ) >
    struct ImplOption;
    template< typename TImpl, typename TTarget >
    static void SetDebugLabelImpl( TImpl* pImpl, DeviceImpl< TTarget >* pDevice, const char* label,
        ImplOption< TImpl, TTarget, &TImpl::SetDebugLabel >* = 0 ) NN_NOEXCEPT
    {
        return pImpl->SetDebugLabel( pDevice, label );
    }
    template< typename TImpl, typename TTarget >
    static void SetDebugLabelImpl( TImpl*, DeviceImpl< TTarget >*, const char*, ... ) NN_NOEXCEPT
    {
    }
    template< typename TImpl, typename TTarget >
    static void SetDebugLabel( TImpl* pImpl, TDevice< TTarget >* pDevice, const char* label ) NN_NOEXCEPT
    {
        return SetDebugLabelImpl< TImpl, TTarget >(
            pImpl, static_cast< DeviceImpl< TTarget >* >( pDevice ), label, 0 );
    }
};

}
}
}
