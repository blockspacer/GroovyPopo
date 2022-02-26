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
#include <type_traits>

namespace nn { namespace util {

/*!
    @brief 型の定義を隠したまま配置する領域を確保するための構造体です。

    @tparam     T 前方宣言された内部オブジェクトの型です。
    @tparam     Size 内部オブジェクトのサイズです。
    @tparam     Alignment 内部オブジェクトのアライメントです。
    @details    内部オブジェクトの前方宣言およびサイズとアライメントにより領域を確保します。
    @see        nn::util::Get
*/
template< typename T, size_t Size, size_t Alignment >
struct TypedStorage
{
    typename std::aligned_storage< Size, Alignment >::type storage;
};

//! @{
//! @name TypedStorage 関連関数

/*!
    @brief 指定された領域から実際の型の参照を取り出します。

    @param[in]  storage 取り出す型と同一のサイズとアライメントの領域です。
    @details    storage のサイズまたはアライメントが実際と異なる際はコンパイルエラーとなります。
    @return     内部に持つ実際の型の参照を返します。
    @overloadlist{nn_util_Get}
*/
template < typename T >
NN_FORCEINLINE
T& Get( TypedStorage< T, sizeof( T ), std::alignment_of< T >::value >& storage )
{
    return reinterpret_cast< T& >( storage );
}

/*!
    @brief 指定された領域から実際の型の参照を取り出します。

    @param[in]  storage 取り出す型と同一のサイズとアライメントの領域です。
    @details    storage のサイズまたはアライメントが実際と異なる際はコンパイルエラーとなります。
    @return     内部に持つ実際の型の参照を返します。
    @overloadlist{nn_util_Get}
*/
template < typename T >
NN_FORCEINLINE
const T& Get( const TypedStorage< T, sizeof( T ), std::alignment_of< T >::value >& storage )
{
    return reinterpret_cast< const T& >( storage );
}

//! @}

}} // namespace nn::util
