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

#include <nn/util/util_IntrusiveList.h>

namespace nn { namespace atk { namespace detail {

    typedef nn::util::IntrusiveListNode IntrusiveListNode;

    template < typename Element >
    class IntrusiveList
    {
    public:
        typedef nn::util::IntrusiveList<Element, util::IntrusiveListMemberNodeTraits<Element,&Element::m_ElementLink>> ElementList;
        typedef typename ElementList::iterator Iterator;
        typedef typename ElementList::const_iterator ConstIterator;

    public:
        IntrusiveList() NN_NOEXCEPT
        {
        }

        ~IntrusiveList() NN_NOEXCEPT
        {
        }

        void PushFront(Element& element) NN_NOEXCEPT
        {
            m_ListImpl.push_front(element);
        }

        void PushBack(Element& element) NN_NOEXCEPT
        {
            m_ListImpl.push_back(element);
        }

        void PopFront() NN_NOEXCEPT
        {
            m_ListImpl.pop_front();
        }

        void PopBack() NN_NOEXCEPT
        {
            m_ListImpl.pop_back();
        }

        Iterator Begin() NN_NOEXCEPT
        {
            return m_ListImpl.begin();
        }

        ConstIterator Begin() const NN_NOEXCEPT
        {
            return m_ListImpl.begin();
        }

        Iterator End() NN_NOEXCEPT
        {
            return m_ListImpl.end();
        }

        ConstIterator End() const NN_NOEXCEPT
        {
            return m_ListImpl.end();
        }

        bool IsEmpty() const NN_NOEXCEPT
        {
            return m_ListImpl.empty();
        }

        void Remove(Element& element) NN_NOEXCEPT
        {
            for ( Iterator iterator = Begin(); iterator != End(); ++iterator )
            {
                if ( &*iterator == &element )
                {
                    m_ListImpl.erase(iterator);
                    break;
                }
            }
        }

        void Clear() NN_NOEXCEPT
        {
            m_ListImpl.clear();
        }

        int Count() const NN_NOEXCEPT
        {
            return m_ListImpl.size();
        }

    private:
        ElementList m_ListImpl;
    };

}}} // namespace nn::atk::detail
