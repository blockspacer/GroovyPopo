/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief    侵入型リストです。
*/

#pragma once

#include <nn/nn_Common.h>
#include <iterator>
#include <nn/nn_SdkAssert.h>

namespace nn { namespace util {

namespace detail
{
    class IntrusiveListImplementation;
}

/**
* @brief    侵入型リストのノードです。
*/
class IntrusiveListNode
{
    NN_DISALLOW_COPY(IntrusiveListNode);
public:
    friend class nn::util::detail::IntrusiveListImplementation;

    /**
    * @brief    コンストラクタです。
    */
    IntrusiveListNode() NN_NOEXCEPT
    {
        m_Prev = this;
        m_Next = this;
    }

    /**
    * @brief    リストに含まれているかどうかを返します。
    * @return   リストに含まれているなら true、それ以外なら false です。
    */
    bool IsLinked() const NN_NOEXCEPT
    {
        return m_Next != this;
    }

private:
    IntrusiveListNode* GetPrev() NN_NOEXCEPT
    {
        return m_Prev;
    }

    const IntrusiveListNode* GetPrev() const NN_NOEXCEPT
    {
        return m_Prev;
    }

    IntrusiveListNode* GetNext() NN_NOEXCEPT
    {
        return m_Next;
    }

    const IntrusiveListNode* GetNext() const NN_NOEXCEPT
    {
        return m_Next;
    }

    void LinkPrev(IntrusiveListNode* node) NN_NOEXCEPT
    {
        LinkPrev(node, node);
    }

    void LinkPrev(IntrusiveListNode* first, IntrusiveListNode* last) NN_NOEXCEPT
    {
        IntrusiveListNode* node = last->m_Prev;
        first->m_Prev = m_Prev;
        node->m_Next = this;
        m_Prev->m_Next = first;
        m_Prev = node;
    }

    void LinkNext(IntrusiveListNode* node) NN_NOEXCEPT
    {
        LinkNext(node, node);
    }

    void LinkNext(IntrusiveListNode* first, IntrusiveListNode* last) NN_NOEXCEPT
    {
        IntrusiveListNode* node = last->m_Prev;
        first->m_Prev = this;
        node->m_Next = m_Next;
        m_Next->m_Prev = node;
        m_Next = first;
    }

    void Unlink() NN_NOEXCEPT
    {
        Unlink(m_Next);
    }

    void Unlink(IntrusiveListNode* last) NN_NOEXCEPT
    {
        IntrusiveListNode* node = last->m_Prev;
        m_Prev->m_Next = last;
        last->m_Prev = m_Prev;
        node->m_Next = this;
        m_Prev = node;
    }

private:
    IntrusiveListNode* m_Prev;
    IntrusiveListNode* m_Next;
};

namespace detail
{
    class IntrusiveListImplementation
    {
        NN_DISALLOW_COPY(IntrusiveListImplementation);
    public:
        class const_iterator;
        class iterator;

        typedef IntrusiveListNode value_type;
        typedef int difference_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef int size_type;

        class const_iterator
        {
        public:
            typedef const IntrusiveListImplementation::value_type value_type;
            typedef IntrusiveListImplementation::difference_type difference_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef std::bidirectional_iterator_tag iterator_category;

        public:
            explicit const_iterator(pointer node) NN_NOEXCEPT
                : m_Node(node)
            {
            }

            reference operator*() const NN_NOEXCEPT
            {
                return *m_Node;
            }

            pointer operator->() const NN_NOEXCEPT
            {
                return m_Node;
            }

            const_iterator& operator++() NN_NOEXCEPT
            {
                m_Node = m_Node->GetNext();
                return *this;
            }

            const_iterator operator++(int) NN_NOEXCEPT
            {
                const_iterator temporary(*this);
                ++(*this);
                return temporary;
            }

            const_iterator& operator--() NN_NOEXCEPT
            {
                m_Node = m_Node->GetPrev();
                return *this;
            }

            const_iterator operator--(int) NN_NOEXCEPT
            {
                const_iterator temporary(*this);
                --(*this);
                return temporary;
            }

            bool operator==(const const_iterator& target) const NN_NOEXCEPT
            {
                return m_Node == target.m_Node;
            }

            bool operator!=(const const_iterator& target) const NN_NOEXCEPT
            {
                return m_Node != target.m_Node;
            }

        private:
            pointer m_Node;
        };

        class iterator
        {
        public:
            typedef IntrusiveListImplementation::value_type value_type;
            typedef IntrusiveListImplementation::difference_type difference_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef std::bidirectional_iterator_tag iterator_category;

        public:
            explicit iterator(pointer node) NN_NOEXCEPT
                : m_Node(node)
            {
            }

            NN_IMPLICIT operator const_iterator() const NN_NOEXCEPT
            {
                return const_iterator(m_Node);
            }

            reference operator*() const NN_NOEXCEPT
            {
                return *m_Node;
            }

            pointer operator->() const NN_NOEXCEPT
            {
                return m_Node;
            }

            iterator& operator++() NN_NOEXCEPT
            {
                m_Node = m_Node->GetNext();
                return *this;
            }

            iterator operator++(int) NN_NOEXCEPT
            {
                iterator temporary(*this);
                ++(*this);
                return temporary;
            }

            iterator& operator--() NN_NOEXCEPT
            {
                m_Node = m_Node->GetPrev();
                return *this;
            }

            iterator operator--(int) NN_NOEXCEPT
            {
                iterator temporary(*this);
                --(*this);
                return temporary;
            }

            bool operator==(const iterator& target) const NN_NOEXCEPT
            {
                return m_Node == target.m_Node;
            }

            bool operator!=(const iterator& target) const NN_NOEXCEPT
            {
                return m_Node != target.m_Node;
            }

        private:
            pointer m_Node;
        };

    public:
        IntrusiveListImplementation() NN_NOEXCEPT
            : m_Root()
        {
        }

        void push_back(reference node) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(!node.IsLinked());
            m_Root.LinkPrev(&node);
        }

        void push_front(reference node) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(!node.IsLinked());
            m_Root.LinkNext(&node);
        }

        void pop_back() NN_NOEXCEPT
        {
            m_Root.GetPrev()->Unlink();
        }

        void pop_front() NN_NOEXCEPT
        {
            m_Root.GetNext()->Unlink();
        }

        reference back() NN_NOEXCEPT
        {
            return *m_Root.GetPrev();
        }

        const_reference back() const NN_NOEXCEPT
        {
            return *m_Root.GetPrev();
        }

        reference front() NN_NOEXCEPT
        {
            return *m_Root.GetNext();
        }

        const_reference front() const NN_NOEXCEPT
        {
            return *m_Root.GetNext();
        }

        iterator begin() NN_NOEXCEPT
        {
            return iterator(m_Root.GetNext());
        }

        const_iterator begin() const NN_NOEXCEPT
        {
            return const_iterator(m_Root.GetNext());
        }

        iterator end() NN_NOEXCEPT
        {
            return iterator(&m_Root);
        }

        const_iterator end() const NN_NOEXCEPT
        {
            return const_iterator(&m_Root);
        }

        iterator iterator_to(reference value) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(value.IsLinked());
            return iterator(&value);
        }

        const_iterator iterator_to(const_reference value) const NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(value.IsLinked());
            return const_iterator(&value);
        }

        size_type size() const NN_NOEXCEPT
        {
            return static_cast<size_type>(std::distance(begin(), end()));
        }

        bool empty() const NN_NOEXCEPT
        {
            return !m_Root.IsLinked();
        }

        iterator erase(const_iterator position) NN_NOEXCEPT
        {
            if (position == end())
            {
                return end();
            }
            iterator temporary(ToMutable(position));
            (temporary++)->Unlink();
            return temporary;
        }

        void clear() NN_NOEXCEPT
        {
            while (!empty())
            {
                pop_front();
            }
        }

        iterator insert(const_iterator position, reference node) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(!node.IsLinked());
            ToMutable(position)->LinkPrev(&node);
            return iterator(&node);
        }

        void splice(const_iterator position, IntrusiveListImplementation& other) NN_NOEXCEPT
        {
            SpliceImplementation(position, other.begin(), other.end());
        }

        void splice(const_iterator position, IntrusiveListImplementation& other, const_iterator element) NN_NOEXCEPT
        {
            NN_UNUSED(other);
            const_iterator nextIterator(element);
            std::advance(nextIterator, 1);
            SpliceImplementation(position, element, nextIterator);
        }

        void splice(const_iterator position, IntrusiveListImplementation& other, const_iterator first, const_iterator last) NN_NOEXCEPT
        {
            NN_UNUSED(other);
            SpliceImplementation(position, first, last);
        }

    private:
        void SpliceImplementation(const_iterator position, const_iterator first, const_iterator last) NN_NOEXCEPT
        {
            if (first == last)
            {
                return;
            }
            iterator iteratorPosition(ToMutable(position));
            iterator iteratorFirst(ToMutable(first));
            iterator iteratorLast(ToMutable(last));
            iteratorFirst->Unlink(&*iteratorLast);
            iteratorPosition->LinkPrev(&*iteratorFirst, &*iteratorFirst);
        }

        iterator ToMutable(const_iterator immutableIterator) const
        {
            // std::next(begin(), std::distance(cbegin(), immutableIterator))となるべき処理の最適化コードです。
            return iterator(const_cast<IntrusiveListNode*>(&*immutableIterator));
        }

    private:
        IntrusiveListNode m_Root;
    };
}

/**
* @brief    侵入型リストです。
*           要求があるまでは暫定的にTにconstが含まれないこと前提の仕様としています。
*/
template<class T, class NodeTraits>
class IntrusiveList
{
    NN_DISALLOW_COPY(IntrusiveList);
public:
    class const_iterator;
    class iterator;

    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef int size_type;
    typedef int difference_type;

    /**
    * @brief    constイテレータです。
    */
    class const_iterator
    {
    public:
        typedef const typename IntrusiveList::value_type value_type;
        typedef typename IntrusiveList::difference_type difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        friend class nn::util::IntrusiveList<T, NodeTraits>;

    public:
        /**
        * @brief    内部オブジェクトへの参照を返します。
        */
        reference operator*() const NN_NOEXCEPT
        {
            return NodeTraits::GetItem(*m_Iterator);
        }

        /**
        * @brief    内部オブジェクトへのポインタを返します。
        */
        pointer operator->() const NN_NOEXCEPT
        {
            return &NodeTraits::GetItem(*m_Iterator);
        }

        /**
        * @brief    イテレータを次へ進めます。
        */
        const_iterator& operator++() NN_NOEXCEPT
        {
            ++m_Iterator;
            return *this;
        }

        /**
        * @brief    イテレータを次へ進めます。
        */
        const_iterator operator++(int) NN_NOEXCEPT
        {
            const_iterator temporary(*this);
            ++m_Iterator;
            return temporary;
        }

        /**
        * @brief    イテレータを前へ戻します。
        */
        const_iterator& operator--() NN_NOEXCEPT
        {
            --m_Iterator;
            return *this;
        }

        /**
        * @brief    イテレータを前へ戻します。
        */
        const_iterator operator--(int) NN_NOEXCEPT
        {
            const_iterator temporary(*this);
            --m_Iterator;
            return temporary;
        }

        /**
        * @brief    イテレータの一致を比較します。
        */
        bool operator==(const const_iterator& target) const NN_NOEXCEPT
        {
            return m_Iterator == target.m_Iterator;
        }

        /**
        * @brief    イテレータの不一致を比較します。
        */
        bool operator!=(const const_iterator& target) const NN_NOEXCEPT
        {
            return m_Iterator != target.m_Iterator;
        }

    private:
        explicit const_iterator(nn::util::detail::IntrusiveListImplementation::const_iterator sourceIterator) NN_NOEXCEPT
            : m_Iterator(sourceIterator)
        {
        }

        nn::util::detail::IntrusiveListImplementation::const_iterator GetImplementationIterator() const NN_NOEXCEPT
        {
            return m_Iterator;
        }

    private:
        nn::util::detail::IntrusiveListImplementation::const_iterator m_Iterator;
    };

    /**
    * @brief    イテレータです。
    */
    class iterator
    {
    public:
        typedef typename IntrusiveList::value_type value_type;
        typedef typename IntrusiveList::difference_type difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        friend class nn::util::IntrusiveList<T, NodeTraits>;

    public:

        /**
        * @brief    const_iteratorキャスト演算子のオーバーロードです。
        */
        NN_IMPLICIT operator const_iterator() const NN_NOEXCEPT
        {
            return const_iterator(static_cast<nn::util::detail::IntrusiveListImplementation::const_iterator>(m_Iterator));
        }

        /**
        * @brief    内部オブジェクトへの参照を返します。
        */
        reference operator*() const NN_NOEXCEPT
        {
            return NodeTraits::GetItem(*m_Iterator);
        }

        /**
        * @brief    内部オブジェクトへのポインタを返します。
        */
        pointer operator->() const NN_NOEXCEPT
        {
            return &NodeTraits::GetItem(*m_Iterator);
        }

        /**
        * @brief    イテレータを次へ進めます。
        */
        iterator& operator++() NN_NOEXCEPT
        {
            ++m_Iterator;
            return *this;
        }

        /**
        * @brief    イテレータを次へ進めます。
        */
        iterator operator++(int) NN_NOEXCEPT
        {
            iterator temporary(*this);
            ++m_Iterator;
            return temporary;
        }

        /**
        * @brief    イテレータを前へ戻します。
        */
        iterator& operator--() NN_NOEXCEPT
        {
            --m_Iterator;
            return *this;
        }

        /**
        * @brief    イテレータを前へ戻します。
        */
        iterator operator--(int) NN_NOEXCEPT
        {
            iterator temporary(*this);
            --m_Iterator;
            return temporary;
        }

        /**
        * @brief    イテレータの一致を比較します。
        */
        bool operator==(const iterator& target) const NN_NOEXCEPT
        {
            return m_Iterator == target.m_Iterator;
        }

        /**
        * @brief    イテレータの不一致を比較します。
        */
        bool operator!=(const iterator& target) const NN_NOEXCEPT
        {
            return m_Iterator != target.m_Iterator;
        }

    private:
        explicit iterator(nn::util::detail::IntrusiveListImplementation::iterator sourceIterator) NN_NOEXCEPT
            : m_Iterator(sourceIterator)
        {
        }

        nn::util::detail::IntrusiveListImplementation::iterator GetImplementationIterator() const NN_NOEXCEPT
        {
            return m_Iterator;
        }

    private:
        nn::util::detail::IntrusiveListImplementation::iterator m_Iterator;
    };

public:
    /**
    * @brief    コンストラクタです。
    */
    IntrusiveList() NN_NOEXCEPT
        : m_Implementation()
    {
    }

    /**
    * @brief    値をリスト終端に挿入します。
    * @param[in]    value   挿入する値を指定します。
    * @pre      value はリストに挿入済みではいけません。
    */
    void push_back(reference value) NN_NOEXCEPT
    {
        m_Implementation.push_back(ToNode(value));
    }

    /**
    * @brief    値をリスト先頭に挿入します。
    * @param[in]    value   挿入する値を指定します。
    * @pre      value はリストに挿入済みではいけません。
    */
    void push_front(reference value) NN_NOEXCEPT
    {
        m_Implementation.push_front(ToNode(value));
    }

    /**
    * @brief    リスト終端の要素を削除します。
    * @pre      リストは空であってはいけません。
    * @details  デストラクタは一切呼ばれません。
    */
    void pop_back() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(!m_Implementation.empty());
        m_Implementation.pop_back();
    }

    /**
    * @brief    リスト先頭の要素を削除します。
    * @pre      リストは空であってはいけません。
    */
    void pop_front() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(!m_Implementation.empty());
        m_Implementation.pop_front();
    }

    /**
    * @brief    リスト先頭の要素への参照を返します。
    * @return   リスト先頭の要素への参照です。
    * @pre      リストは空であってはいけません。
    */
    reference front() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(!m_Implementation.empty());
        return ToReference(m_Implementation.front());
    }

    /**
    * @brief    リスト先頭の要素へのconst参照を返します。
    * @return   リスト先頭の要素へのconst参照です。
    * @pre      リストは空であってはいけません。
    */
    const_reference front() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(!m_Implementation.empty());
        return ToReference(m_Implementation.front());
    }

    /**
    * @brief    リスト終端の要素への参照を返します。
    * @return   リスト終端の要素への参照です。
    * @pre      リストは空であってはいけません。
    */
    reference back() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(!m_Implementation.empty());
        return ToReference(m_Implementation.back());
    }

    /**
    * @brief    リスト終端の要素へのconst参照を返します。
    * @return   リスト終端の要素へのconst参照です。
    * @pre      リストは空であってはいけません。
    */
    const_reference back() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(!m_Implementation.empty());
        return ToReference(m_Implementation.back());
    }

    /**
    * @brief    リスト先頭の要素のイテレータを返します。
    * @return   リスト先頭の要素のイテレータです。
    */
    iterator begin() NN_NOEXCEPT
    {
        return iterator(m_Implementation.begin());
    }

    /**
    * @brief    リスト先頭の要素のconstイテレータを返します。
    * @return   リスト先頭の要素のconstイテレータです。
    */
    const_iterator begin() const NN_NOEXCEPT
    {
        return const_iterator(m_Implementation.begin());
    }

    /**
    * @brief    リスト先頭の要素のconstイテレータを返します。
    * @return   リスト先頭の要素のconstイテレータです。
    */
    const_iterator cbegin() const NN_NOEXCEPT
    {
        return begin();
    }

    /**
    * @brief    リスト終端の次の要素のイテレータを返します。
    * @return   リスト終端の次の要素のイテレータです。
    */
    iterator end() NN_NOEXCEPT
    {
        return iterator(m_Implementation.end());
    }

    /**
    * @brief    リスト終端の次の要素のconstイテレータを返します。
    * @return   リスト終端の次の要素のconstイテレータです。
    */
    const_iterator end() const NN_NOEXCEPT
    {
        return const_iterator(m_Implementation.end());
    }

    /**
    * @brief    リスト終端の次の要素のconstイテレータを返します。
    * @return   リスト終端の次の要素のconstイテレータです。
    */
    const_iterator cend() const NN_NOEXCEPT
    {
        return end();
    }

    /**
    * @brief    終端要素を指す逆イテレータを返します。
    * @return   終端要素を指す逆イテレータです。
    */
    reverse_iterator rbegin() NN_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    /**
    * @brief    終端要素を指すconst逆イテレータを返します。
    * @return   終端要素を指すconst逆イテレータです。
    */
    const_reverse_iterator rbegin() const NN_NOEXCEPT
    {
        return const_reverse_iterator(end());
    }

    /**
    * @brief    終端要素を指すconst逆イテレータを返します。
    * @return   終端要素を指すconst逆イテレータです。
    */
    const_reverse_iterator crbegin() const NN_NOEXCEPT
    {
        return rbegin();
    }

    /**
    * @brief    先頭要素の前の要素を指す逆イテレータを返します。
    * @return   先頭要素の前の要素を指す逆イテレータです。
    */
    reverse_iterator rend() NN_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    /**
    * @brief    先頭要素の前の要素を指すconst逆イテレータを返します。
    * @return   先頭要素の前の要素を指すconst逆イテレータです。
    */
    const_reverse_iterator rend() const NN_NOEXCEPT
    {
        return const_reverse_iterator(begin());
    }

    /**
    * @brief    先頭要素の前の要素を指すconst逆イテレータを返します。
    * @return   先頭要素の前の要素を指すconst逆イテレータです。
    */
    const_reverse_iterator crend() const NN_NOEXCEPT
    {
        return rend();
    }

    /**
    * @brief    指定した要素を指すイテレータを返します。
    * @param[in]    value   イテレータを取得する要素を指定します。
    * @pre      value はこのリストに挿入されていなければなりません。
    * @return   指定した要素を指すイテレータです。
    */
    iterator iterator_to(reference value) NN_NOEXCEPT
    {
        return iterator(m_Implementation.iterator_to(ToNode(value)));
    }

    /**
    * @brief    指定した要素を指すconstイテレータを返します。
    * @param[in]    value   イテレータを取得する要素を指定します。
    * @pre      value はこのリストに挿入されていなければなりません。
    * @return   指定した要素を指すconstイテレータです。
    */
    const_iterator iterator_to(const_reference value) const NN_NOEXCEPT
    {
        return const_iterator(m_Implementation.iterator_to(ToNode(value)));
    }

    /**
    * @brief    リストに含まれる要素数を返します。
    * @return   リストに含まれる要素数です。
    */
    size_type size() const NN_NOEXCEPT
    {
        return m_Implementation.size();
    }

    /**
    * @brief    リストが空かどうかを判定します。
    * @return   リストが空であればtrueを返し、それ以外であればfalseを返します。
    */
    bool empty() const NN_NOEXCEPT
    {
        return m_Implementation.empty();
    }

    /**
    * @brief    イテレータが指す要素をリストから削除します。
    * @param[in]    position    削除する要素を指すイテレータを指定します。
    * @return   削除した要素の次の要素のイテレータです。
    * @details  デストラクタは一切呼ばれません。
    */
    iterator erase(const_iterator position) NN_NOEXCEPT
    {
        nn::util::detail::IntrusiveListImplementation::iterator result = m_Implementation.erase(position.GetImplementationIterator());
        return iterator(result);
    }

    /**
    * @brief    リスト内の全要素を削除します。
    * @details  デストラクタは一切呼ばれません。
    */
    void clear() NN_NOEXCEPT
    {
        m_Implementation.clear();
    }

    /**
    * @brief    イテレータが指す要素の直前に値を挿入します。
    * @param[in]    position    値を挿入する位置をイテレータで指定します。
    * @param[in]    value   挿入する値を指定します。
    * @pre      value はリストに挿入済みではいけません。
    * @return   挿入した要素のイテレータです。
    */
    iterator insert(const_iterator position, reference value) NN_NOEXCEPT
    {
        nn::util::detail::IntrusiveListImplementation::iterator result = m_Implementation.insert(position.GetImplementationIterator(), ToNode(value));
        return iterator(result);
    }

    /**
    * @brief    別のリストから全要素を転送します。
    * @param[in]    position    転送先の位置をイテレータで指定します。
    * @param[in]    other   転送元のリストを指定します。
    * @details  指定したイテレータの指す要素の直前に転送されます。
    *           デストラクタおよびコピーコンストラクタは一切呼ばれません。
    */
    void splice(const_iterator position, IntrusiveList& other) NN_NOEXCEPT
    {
        m_Implementation.splice(position.GetImplementationIterator(), other.m_Implementation);
    }

    /**
    * @brief    別のリストから指定した値を転送します。
    * @param[in]    position    転送先の位置をイテレータで指定します。
    * @param[in]    other   転送元のリストを指定します。
    * @param[in]    element 転送する値を指したイテレータを指定します。
    * @details  指定したイテレータの指す要素の直前に転送されます。
    *           デストラクタおよびコピーコンストラクタは一切呼ばれません。
    */
    void splice(const_iterator position, IntrusiveList& other, const_iterator element) NN_NOEXCEPT
    {
        m_Implementation.splice(position.GetImplementationIterator(), other.m_Implementation, element.GetImplementationIterator());
    }

    /**
    * @brief    別のリストから指定した領域を転送します。
    * @param[in]    position    転送先の位置をイテレータで指定します。
    * @param[in]    other   転送元のリストを指定します。
    * @param[in]    first   転送する領域の先頭位置をイテレータで指定します。
    * @param[in]    last    転送する領域の終端位置をイテレータで指定します。
    * @details  指定したイテレータの指す要素の直前に転送されます。
    *           デストラクタおよびコピーコンストラクタは一切呼ばれません。
    */
    void splice(const_iterator position, IntrusiveList& other, const_iterator first, const_iterator last) NN_NOEXCEPT
    {
        m_Implementation.splice(position.GetImplementationIterator(), other.m_Implementation, first.GetImplementationIterator(), last.GetImplementationIterator());
    }

private:
    IntrusiveListNode& ToNode(reference ref) const NN_NOEXCEPT
    {
        return NodeTraits::GetNode(ref);
    }

    const IntrusiveListNode& ToNode(const_reference ref) const NN_NOEXCEPT
    {
        return NodeTraits::GetNode(ref);
    }

    reference ToReference(IntrusiveListNode& node) const NN_NOEXCEPT
    {
        return NodeTraits::GetItem(node);
    }

    const_reference ToReference(const IntrusiveListNode& node) const NN_NOEXCEPT
    {
        return NodeTraits::GetItem(node);
    }

private:
    nn::util::detail::IntrusiveListImplementation m_Implementation;
};

/**
* @brief    侵入型リストのノードへのアクセス方法を定義するクラスです。
*           ノードをメンバとして定義した場合に使用します。
*/
template<class HolderT, IntrusiveListNode HolderT::*Member, class T = HolderT, size_t IsCompleteType = sizeof(HolderT)>
class IntrusiveListMemberNodeTraits
{
private:
    friend class IntrusiveList<T, IntrusiveListMemberNodeTraits>;

    static IntrusiveListNode& GetNode(T& ref) NN_NOEXCEPT
    {
        return ref.*Member;
    }

    static const IntrusiveListNode& GetNode(const T& ref) NN_NOEXCEPT
    {
        return ref.*Member;
    }

    static T& GetItem(IntrusiveListNode& node) NN_NOEXCEPT
    {
        return *reinterpret_cast<T*>(reinterpret_cast<char*>(&node) - GetOffset());
    }

    static const T& GetItem(const IntrusiveListNode& node) NN_NOEXCEPT
    {
        return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(&node) - GetOffset());
    }

    static uintptr_t GetOffset() NN_NOEXCEPT
    {
        return reinterpret_cast<uintptr_t>(&((reinterpret_cast<T*>(0))->*Member));
    }
};

/**
* @brief    侵入型リストのノードです。ノードを基底クラスとして定義するスタイルです。
*
* @tparam   T   侵入型リストの要素の型です。
* @tparam   Tag ノードを多重継承する際にノードを区別するための型です。
*/
template <class T, class Tag = T>
class IntrusiveListBaseNode : public IntrusiveListNode
{
};

/**
* @brief    侵入型リストのノードへのアクセス方法を定義するクラスです。
*           ノードを基底クラスとして定義した場合に使用します。
*
* @tparam   T   侵入型リストの要素の型です。
* @tparam   Tag アクセスしたい IntrusiveListBaseNode に指定した Tag 型です。
*/
template <class T, class Tag = T>
class IntrusiveListBaseNodeTraits
{
private:
    friend class IntrusiveList<T, IntrusiveListBaseNodeTraits>;

    typedef IntrusiveListBaseNode<T, Tag> BaseNodeType;

    static IntrusiveListNode& GetNode(T& ref) NN_NOEXCEPT
    {
        return static_cast<BaseNodeType&>(ref);
    }

    static const IntrusiveListNode& GetNode(const T& ref) NN_NOEXCEPT
    {
        return static_cast<const BaseNodeType&>(ref);
    }

    static T& GetItem(IntrusiveListNode& node) NN_NOEXCEPT
    {
        return *static_cast<T*>(static_cast<BaseNodeType*>(&node));
    }

    static const T& GetItem(const IntrusiveListNode& node) NN_NOEXCEPT
    {
        return *static_cast<const T*>(static_cast<const BaseNodeType*>(&node));
    }
};

}}
