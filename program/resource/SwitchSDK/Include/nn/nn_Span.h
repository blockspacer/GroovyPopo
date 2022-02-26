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

#include <cstddef>
#include <type_traits>
#include <array>
#include <iterator>
#include <nn/util/util_IntUtil.h>
#include <nn/nn_SdkAssert.h>

namespace nn {

namespace detail {

    template <typename Span>
    class SpanConstIterator;

    template <typename Span, typename DerivedIterator, typename Reference>
    class SpanIteratorBase
    {
        friend SpanConstIterator<Span>;
    public:

        using difference_type = typename Span::difference_type;
        using value_type = typename std::remove_cv<typename Span::element_type>::type;
        using reference = Reference;
        using pointer = typename std::add_pointer<reference>::type;
        using iterator_category = std::random_access_iterator_tag;

        SpanIteratorBase() NN_NOEXCEPT = default;

        SpanIteratorBase(const Span* p, difference_type offset) NN_NOEXCEPT
            : m_P(p)
            , m_Offset(offset)
        {
        }

        pointer operator->() const NN_NOEXCEPT
        {
            return m_P->data() + m_Offset;
        }

        reference operator*() const NN_NOEXCEPT
        {
            return *this->operator->();
        }

        DerivedIterator& operator++() NN_NOEXCEPT
        {
            ++m_Offset;
            return static_cast<DerivedIterator&>(*this);
        }

        DerivedIterator operator++(int) NN_NOEXCEPT
        {
            auto ret = static_cast<DerivedIterator&>(*this);
            this->operator++();
            return ret;
        }

        DerivedIterator& operator--() NN_NOEXCEPT
        {
            --m_Offset;
            return static_cast<DerivedIterator&>(*this);
        }

        DerivedIterator operator--(int) NN_NOEXCEPT
        {
            auto ret = static_cast<DerivedIterator&>(*this);
            this->operator--();
            return ret;
        }

        DerivedIterator& operator+=(difference_type n) NN_NOEXCEPT
        {
            m_Offset += n;
            return static_cast<DerivedIterator&>(*this);
        }

        DerivedIterator operator+(difference_type rhs) const NN_NOEXCEPT
        {
            auto ret = static_cast<const DerivedIterator&>(*this);
            return ret += rhs;
        }

        friend DerivedIterator operator+(difference_type lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            return rhs + lhs;
        }

        DerivedIterator& operator-=(difference_type n) NN_NOEXCEPT
        {
            m_Offset += -n;
            return static_cast<DerivedIterator&>(*this);
        }

        DerivedIterator operator-(difference_type rhs) const NN_NOEXCEPT
        {
            auto ret = static_cast<const DerivedIterator&>(*this);
            return ret -= rhs;
        }

        difference_type operator-(DerivedIterator rhs) const NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(this->m_P == rhs.m_P);
            return m_Offset - rhs.m_Offset;
        }

        reference operator[](difference_type n) const NN_NOEXCEPT
        {
            return *(*this + n);
        }

        friend bool operator==(DerivedIterator lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            return lhs.m_P == rhs.m_P && lhs.m_Offset == rhs.m_Offset;
        }

        friend bool operator!=(DerivedIterator lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            return !(lhs == rhs);
        }

        friend bool operator<(DerivedIterator lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(lhs.m_P == rhs.m_P);
            return lhs.m_Offset < rhs.m_Offset;
        }

        friend bool operator>(DerivedIterator lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            return rhs < lhs;
        }

        friend bool operator<=(DerivedIterator lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            return !(lhs > rhs);
        }

        friend bool operator>=(DerivedIterator lhs, DerivedIterator rhs) NN_NOEXCEPT
        {
            return !(lhs < rhs);
        }

    private:

        const Span* m_P = nullptr;
        difference_type m_Offset = 0;

    };

    template <typename Span>
    class SpanIterator
        : public SpanIteratorBase<Span, SpanIterator<Span>, typename Span::element_type&>
    {
    public:
        using SpanIteratorBase<Span, SpanIterator<Span>, typename Span::element_type&>::SpanIteratorBase;
    };

    template <typename Span>
    class SpanConstIterator
        : public SpanIteratorBase<Span, SpanConstIterator<Span>, const typename Span::element_type&>
    {
    public:
        SpanConstIterator() NN_NOEXCEPT = default; // WA for VS
        using SpanIteratorBase<Span, SpanConstIterator<Span>, const typename Span::element_type&>::SpanIteratorBase;
        NN_IMPLICIT SpanConstIterator(const SpanIterator<Span>& other) NN_NOEXCEPT
            : SpanConstIterator(other.m_P, other.m_Offset)
        {
        }
    };

}

/**
    @brief 連続領域のビューを表すクラステンプレートです。

    @tparam T 要素の型を指定します。

    @details
     nn::Span は C++20 に追加される予定の std::span のサブセットです(http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0122r6.pdf)。
     今後 NintendoSDK が C++20 に対応した際には、std::span のエイリアステンプレートになる予定です。

     内部に連続した領域の先頭へのポインタと長さを持ちます。長さやインデックスは符号付き整数である std::ptrdiff_t であることに注意してください。

     Span は連続領域へのビューであり、領域を所有はしません。領域の生存管理は別途行う必要があります。

     MakeSpan() を使用することで、要素型 T の指定を省略して Span のインスタンスを生成することができます。
*/
template <typename T>
class Span
{
public:

    /**
        @brief 要素型を表します。T です。
    */
    using element_type = T;

    /**
        @brief 値型を表します。std::remove_cv_t<T> です。
    */
    using value_type = typename std::remove_cv<T>::type;

    /**
        @brief インデックス型を表します。std::ptrdiff_t です。
    */
    using index_type = std::ptrdiff_t;

    /**
        @brief 差分型を表します。std::ptrdiff_t です。
    */
    using difference_type = std::ptrdiff_t;

    /**
        @brief 要素へのポインタ型を表します。T* です。
    */
    using pointer = T*;

    /**
        @brief 要素への参照を表します。T& です。
    */
    using reference = T&;

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

    class UnspecifiedTypeForIterator;
    class UnspecifiedTypeForConstIterator;

    /**
        @brief イテレータ型を表します。

        @details
         RandomAccessIterator と ContiguousIterator を満たします。
    */
    using iterator = UnspecifiedTypeForIterator;

    /**
        @brief 読み取り専用イテレータ型を表します。

        @details
         読み取り専用の RandomAccessIterator と ContiguousIterator を満たします。
    */
    using const_iterator = UnspecifiedTypeForConstIterator;

#else

    using iterator = nn::detail::SpanIterator<Span>;
    using const_iterator = nn::detail::SpanConstIterator<Span>;

#endif

    /**
        @brief std::reverse_iterator<iterator> のエイリアスです。
    */
    using reverse_iterator = std::reverse_iterator<iterator>;

    /**
        @brief std::reverse_iterator<const_iterator> のエイリアスです。
    */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
        @brief デフォルトコンストラクタ: 長さ 0 の Span を生成します。

        @post size() == 0
    */
    Span() NN_NOEXCEPT
        : m_Data(nullptr)
        , m_Size(0)
    {
    }

    /**
        @brief コンストラクタ: 先頭へのポインタとサイズを指定して Span を生成します。

        @param[in] p 先頭要素へのポインタを指定します。
        @param[in] size 要素数を指定します。

        @pre size >= 0
        @pre size > 0 ならば p != nullptr

        @post data() == p
        @post size() == size
    */
    Span(T* p, index_type size) NN_NOEXCEPT
        : m_Data(p)
        , m_Size(size)
    {
        NN_SDK_REQUIRES(size > 0 ? p != nullptr : true);
        NN_SDK_REQUIRES(size >= 0);
    }

    /**
        @brief コンストラクタ: 先頭へのポインタと末尾の次のポインタを指定して Span を生成します。

        @param[in] first 先頭要素へのポインタを指定します。
        @param[in] last 末尾の次の要素へのポインタを指定します。

        @pre first != nullptr
        @pre last != nullptr
        @pre first <= last

        @post data() == first
        @post size() == last - first

        @details
         Span(first, last - first) と同等です。
    */
    Span(T* first, T* last) NN_NOEXCEPT
        : Span(first, last - first)
    {
    }

    /**
        @brief コンストラクタ: cv 修飾子の異なる要素を持つ Span から Span を生成します。

        @tparam U 変換元の要素型を指定します。通常は指定を省略します。

        @param[in] other 配列を指定します。

        @post data() == other.data()
        @post size() == other.size()
    */
    template <typename U, typename = typename std::enable_if<std::is_convertible<U(*)[], T(*)[]>::value>::type>
    NN_IMPLICIT Span(const Span<U>& other) NN_NOEXCEPT
        : Span(other.data(), other.size())
    {
    }

    /**
        @brief 暗黙変換コンストラクタ: 配列から Span を生成します。

        @tparam Size 配列のサイズを指定します。通常は指定を省略します。

        @param[in] other 配列を指定します。

        @post data() == other
        @post size() == Size
    */
    template <size_t Size>
    NN_IMPLICIT Span(T (&other)[Size]) NN_NOEXCEPT
        : Span(static_cast<T*>(other), static_cast<index_type>(Size))
    {
    }

    /**
        @brief 暗黙変換コンストラクタ: std::array から Span を生成します。

        @tparam Size std::array のサイズを指定します。通常は指定を省略します。

        @param[in] other std::array を指定します。

        @post data() == other.data()
        @post size() == Size
    */
    template <size_t Size>
    NN_IMPLICIT Span(std::array<value_type, Size>& other) NN_NOEXCEPT
        : Span(other.data(), static_cast<index_type>(Size))
    {
    }

    /**
        @brief 暗黙変換コンストラクタ: const std::array から Span を生成します。

        @tparam Size std::array のサイズを指定します。通常は指定を省略します。

        @param[in] other const std::array を指定します。

        @post data() == other.data()
        @post size() == Size

        @details
         Span<const T> に対応するものは const std::array<T> であることにご注意ください(std::array<const T> ではない)。
    */
    template <size_t Size>
    NN_IMPLICIT Span(const std::array<value_type, Size>& other) NN_NOEXCEPT
        : Span(other.data(), static_cast<index_type>(Size))
    {
    }

    /**
        @brief 先頭要素へのイテレータを取得します。

        @return 先頭要素へのイテレータを返します。
    */
    iterator begin() const NN_NOEXCEPT
    {
        return {this, 0};
    };

    /**
        @brief 末尾の次の要素へのイテレータを取得します。

        @return 末尾の次の要素へのイテレータを返します。
    */
    iterator end() const NN_NOEXCEPT
    {
        return {this, m_Size};
    };

    /**
        @brief 先頭要素への読み取り専用イテレータを取得します。

        @return 先頭要素への読み取り専用イテレータを返します。
    */
    const_iterator cbegin() const NN_NOEXCEPT
    {
        return {this, 0};
    };

    /**
        @brief 末尾の次の要素への読み取り専用イテレータを取得します。

        @return 末尾の次の要素への読み取り専用イテレータを返します。
    */
    const_iterator cend() const NN_NOEXCEPT
    {
        return {this, m_Size};
    };

    /**
        @brief 逆順での先頭要素へのイテレータを取得します。

        @return 逆順での先頭要素へのイテレータ返します。
    */
    reverse_iterator rbegin() const NN_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    /**
        @brief 逆順での末尾の次の要素へのイテレータを取得します。

        @return 逆順での末尾の次の要素へのイテレータ返します。
    */
    reverse_iterator rend() const NN_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    /**
        @brief 逆順での先頭要素への読み取り専用イテレータを取得します。

        @return 逆順での先頭要素への読み取り専用イテレータ返します。
    */
    const_reverse_iterator crbegin() const NN_NOEXCEPT
    {
        return const_reverse_iterator(cend());
    }

    /**
        @brief 逆順での末尾の次の要素への読み取り専用イテレータを取得します。

        @return 逆順での末尾の次の要素への読み取り専用イテレータ返します。
    */
    const_reverse_iterator crend() const NN_NOEXCEPT
    {
        return const_reverse_iterator(cbegin());
    }

    /**
        @brief 先頭要素へのポインタを取得します。

        @return 先頭要素へのポインタをポインタを返します。
    */
    pointer data() const NN_NOEXCEPT
    {
        return m_Data;
    }

    /**
        @brief インデックスアクセスを提供します。

        @param[in] index インデックスを指定します。
        @return 指定したインデックスの要素への参照を返します。

        @pre index >= 0
        @pre index < size()
    */
    reference operator[](index_type index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(index >= 0);
        NN_SDK_REQUIRES(index < m_Size);
        return m_Data[index];
    }

    /**
        @brief インデックスアクセスを提供します。

        @param[in] index インデックスを指定します。
        @return 指定したインデックスの要素への参照を返します。

        @pre index >= 0
        @pre index < size()

        @details
         operator[] と同等です。
    */
    reference operator()(index_type index) const NN_NOEXCEPT
    {
        return (*this)[index];
    }

    /**
        @brief 要素数を取得します。

        @return 要素数を返します。
    */
    index_type size() const NN_NOEXCEPT
    {
        return m_Size;
    }

    /**
        @brief バイト数を取得します。

        @return バイト数(size() * sizeof(T))を返します。
    */
    index_type size_bytes() const NN_NOEXCEPT
    {
        return m_Size * sizeof(T);
    }

    /**
        @brief 要素数が 0 であるかどうかを取得します。

        @return 要素数が 0 であれば true を、そうでなければ false を返します。
    */
    bool empty() const NN_NOEXCEPT
    {
        return m_Size == 0;
    }

    /**
        @brief 先頭から指定した要素数の Span を作成します。

        @param[in] size 要素数を指定します。
        @return 先頭から要素数 size の Span を返します。

        @pre size >= 0
        @pre size <= this->size()

        @details
         Span{data(), size} と同等です。
    */
    Span first(index_type size) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(size >= 0);
        NN_SDK_REQUIRES(size <= m_Size);
        return {m_Data, size};
    }

    /**
        @brief 末尾から指定した要素数の Span を作成します。

        @param[in] size 要素数を指定します。
        @return 末尾から要素数 size の Span を返します。

        @pre size >= 0
        @pre size <= this->size()

        @details
         Span{data() + (this->size() - size), size} と同等です。
    */
    Span last(index_type size) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(size >= 0);
        NN_SDK_REQUIRES(size <= m_Size);
        return {m_Data + (m_Size - size), size};
    }

    /**
        @brief 指定したオフセットから指定した要素数の Span を切り出して作成します。

        @param[in] offset 切り出す先頭オフセットを指定します。
        @param[in] size 切り出す要素数を指定します。
        @return 切り出した Span を返します。

        @pre offset >= 0
        @pre size >= 0
        @pre size <= this->size()
        @pre this->size() - size >= offset

        @details
         Span{data() + offset, size} と同等です。
    */
    Span subspan(index_type offset, index_type size) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(offset >= 0);
        NN_SDK_REQUIRES(size >= 0);
        NN_SDK_REQUIRES(size <= m_Size);
        NN_SDK_REQUIRES(m_Size - size >= offset);
        return {m_Data + offset, size};
    }

private:

    T* m_Data;
    index_type m_Size;

};

/**
    @brief コンストラクタ: 先頭へのポインタとサイズを指定して Span を生成します。

    @tparam T 要素の型を指定します。通常は指定を省略します。

    @param[in] p 先頭要素へのポインタを指定します。
    @param[in] size 要素数を指定します。
    @return Span<T>{p, size} を返します。

    @pre size >= 0
    @pre size > 0 ならば p != nullptr

    @details
     型名 T を省略して Span を生成したいときに使用するユーティリティ関数です。
*/
template <typename T>
Span<T> MakeSpan(T* p, std::ptrdiff_t size) NN_NOEXCEPT
{
    return {p, size};
}

/**
    @brief 先頭へのポインタと末尾の次のポインタを指定して Span を生成します。

    @tparam T 要素の型を指定します。通常は指定を省略します。

    @param[in] first 先頭要素へのポインタを指定します。
    @param[in] last 末尾の次の要素へのポインタを指定します。
    @return Span<T>{first, last}

    @pre first != nullptr
    @pre last != nullptr
    @pre first <= last

    @details
     型名 T を省略して Span を生成したいときに使用するユーティリティ関数です。
*/
template <typename T>
Span<T> MakeSpan(T* first, T* last) NN_NOEXCEPT
{
    return {first, last};
}

/**
    @brief 配列から Span を生成します。

    @tparam T 要素の型を指定します。通常は指定を省略します。
    @tparam Size 要素数を指定します。通常は指定を省略します。

    @param[in] other 元となる配列を指定します。
    @return Span<T>(other) を返します。

    @details
     型名 T を省略して Span を生成したいときに使用するユーティリティ関数です。
*/
template <typename T, size_t Size>
Span<T> MakeSpan(T (&other)[Size]) NN_NOEXCEPT
{
    return Span<T>(other);
}

/**
    @brief std::array から Span を生成します。

    @tparam T 要素の型を指定します。通常は指定を省略します。
    @tparam Size 要素数を指定します。通常は指定を省略します。

    @param[in] other 元となる std::array<T> を指定します。
    @return Span<T>(other) を返します。

    @details
     型名 T を省略して Span を生成したいときに使用するユーティリティ関数です。
*/
template <typename T, size_t Size>
Span<T> MakeSpan(std::array<T, Size>& other) NN_NOEXCEPT
{
    return Span<T>(other);
}

/**
    @brief const std::array から Span を生成します。

    @tparam T 要素の型を指定します。通常は指定を省略します。
    @tparam Size 要素数を指定します。通常は指定を省略します。

    @param[in] other 元となる const std::array<T> を指定します。
    @return Span<const T>(other) を返します。


    @details
     型名 T を省略して Span を生成したいときに使用するユーティリティ関数です。
*/
template <typename T, size_t Size>
Span<const T> MakeSpan(const std::array<T, Size>& other) NN_NOEXCEPT
{
    return Span<const T>(other);
}

}
