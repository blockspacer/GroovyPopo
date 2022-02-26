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

/*!
    @file
    @brief nn::util::Exchange() を定義します。

    @platformbegin{Cafe}
    本ファイルは C++11 の機能を使用しているため、Cafe では include できません。
    @platformend
*/

#include <nn/nn_Common.h>
#include <utility>
#include <type_traits>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_StaticAssert.h>

namespace nn { namespace util {

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

/**
    @brief 指定したポインタが指す変数に対し、指定した値を代入し、古い値を返します。
    @tparam T ポインタの指す変数の型 (T はコピー・ムーブ構築可能である必要があります)
    @tparam U 代入する値の型 (U&& は T& に代入可能である必要があります)
    @param[in] p 値を設定・取得する変数のポインタを指定します。
    @param[in] value 設定する値を指定します。
    @return 古い *p の値を返します。
    @pre p != nullptr
    @post *p == static_cast<T>(value)

    @details T がポインタ型の場合、value には、nullptr や 0 リテラルを指定することが可能です。

    @platformbegin{Cafe}
    本関数テンプレートは C++11 の機能を使用しているため、Cafe では使用できません。
    @platformend
*/
template <typename T, typename U>
T Exchange(T* p, U value);

#else

template <typename T, typename U>
inline typename std::enable_if<std::is_assignable<T&, U&&>::value, T>::type Exchange(T* p, U value)
{
    NN_STATIC_ASSERT(std::is_copy_constructible<T>::value);
    NN_STATIC_ASSERT(std::is_move_constructible<T>::value);
    NN_SDK_REQUIRES_NOT_NULL(p);
    auto ret(std::move(*p));
    *p = std::move(value);
    return ret;
}

template <typename T>
inline T* Exchange(T** p, std::nullptr_t) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(p);
    auto ret(*p);
    *p = nullptr;
    return ret;
}

#endif

}} // namespace nn::util
