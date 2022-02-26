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

namespace nn { namespace util { namespace detail {

// C++03 用 <type_traits> 代替機能

template< typename T >
struct AddConst
{
    typedef const T Type;
};

template< typename T >
struct AddConst< T& >
{
    typedef T& Type;
};

template< int N > struct SizedInt;

//! @cond

template<> struct SizedInt< 1 >
{
    typedef uint8_t Unsigned;
    typedef int8_t Signed;
};

template<> struct SizedInt< 2 >
{
    typedef uint16_t Unsigned;
    typedef int16_t Signed;
};

template<> struct SizedInt< 4 >
{
    typedef uint32_t Unsigned;
    typedef int32_t Signed;
};

template<> struct SizedInt< 8 >
{
    typedef uint64_t Unsigned;
    typedef int64_t Signed;
};

//! @endcond


template< typename T >
struct ChangeSign
{
    typedef typename SizedInt< sizeof( T ) >::Signed Signed;
    typedef typename SizedInt< sizeof( T ) >::Unsigned Unsigned;
};

template< typename T >
struct ChangeSign< const T >
{
    typedef const typename SizedInt< sizeof( T ) >::Signed Signed;
    typedef const typename SizedInt< sizeof( T ) >::Unsigned Unsigned;
};

template< typename T >
struct ChangeSign< volatile T >
{
    typedef volatile typename SizedInt< sizeof( T ) >::Signed Signed;
    typedef volatile typename SizedInt< sizeof( T ) >::Unsigned Unsigned;
};

template< typename T >
struct ChangeSign< const volatile T >
{
    typedef const volatile typename SizedInt< sizeof( T ) >::Signed Signed;
    typedef const volatile typename SizedInt< sizeof( T ) >::Unsigned Unsigned;
};

template< typename T >
struct MakeSigned
{
    typedef typename ChangeSign< T >::Signed Type;
};

template< typename T >
struct MakeUnsigned
{
    typedef typename ChangeSign< T >::Unsigned Type;
};

}}} // namespace nn::util::detail
