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

#include <nn/util/util_PlacementArray.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @brief       固定サイズの配列に値を設定します。
//!
//! @tparam      TElement   型指定
//! @tparam      TSize      配列数
//!
//---------------------------------------------------------------------------
template<typename TElement, size_t TSize>
class PointerFixedSizeArray
{
public:
    typedef nn::util::PlacementArray<TElement> PlacementArray;
    typedef typename PlacementArray::iterator  iterator;

    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //---------------------------------------------------------------------------
    PointerFixedSizeArray() NN_NOEXCEPT
    : m_HoldCount(0)
    {
        memset( (void *)m_FixedSizeElements, 0, sizeof(m_FixedSizeElements) );
        m_PlacementArray.ResetWorkMemory( m_FixedSizeElements, m_PlacementArray.CalculateWorkMemorySize( TSize ), TSize );
    }

    //---------------------------------------------------------------------------
    //! @brief        要素が一つもないかのフラグを取得します。確保メモリがないという意味ではありません。
    //!
    //! @return       要素が一つもない場合は true を返します。
    //---------------------------------------------------------------------------
    bool Empty() NN_NOEXCEPT
    {
        return m_HoldCount == 0;
    }

    //---------------------------------------------------------------------------
    //! @brief        先頭要素へのイテレータを取得します。
    //!
    //! @return       先頭要素のイテレータです。
    //---------------------------------------------------------------------------
    iterator Begin() NN_NOEXCEPT
    {
        return m_PlacementArray.begin();
    }

    //---------------------------------------------------------------------------
    //! @brief        最後尾要素へのイテレータを取得します。
    //!
    //! @return       最後尾要素のイテレータです。
    //---------------------------------------------------------------------------
    iterator End() NN_NOEXCEPT
    {
        iterator temp = m_PlacementArray.begin();
        for ( int i = 0; i < m_HoldCount; i++ ) temp++;
        return temp;
    }

    //---------------------------------------------------------------------------
    //! @brief        要素数を０にします。確保メモリの開放はしません。
    //!               また、全ての要素のデストラクタを呼びだしません。
    //---------------------------------------------------------------------------
    void Clear() NN_NOEXCEPT
    {
        for ( int i = 0; i < m_HoldCount; i++ )
        {
            erase( i );
        }
        m_HoldCount = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief        最後尾要素の参照を取得します。
    //!
    //! @return       最後尾要素の参照です。
    //---------------------------------------------------------------------------
    TElement& Back() NN_NOEXCEPT
    {
        return m_PlacementArray.at( m_HoldCount - 1 );
    }

    //---------------------------------------------------------------------------
    //! @brief        末尾の要素を破棄します。
    //---------------------------------------------------------------------------
    void PopBack() NN_NOEXCEPT
    {
        erase( m_HoldCount - 1 );
        m_HoldCount--;
    }

    //---------------------------------------------------------------------------
    //! @brief        末尾に要素を追加します。
    //!
    //! @tparam       TElement 要素の型です。
    //!
    //! @param[in]    element 追加する要素への参照です。
    //---------------------------------------------------------------------------
    void PushBack( TElement element ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( m_HoldCount < m_PlacementArray.size() );
        (m_PlacementArray.data())[m_HoldCount] = element;
        m_HoldCount++;
    }

    //---------------------------------------------------------------------------
    //! @brief 配列のサイズを取得します。
    //!
    //! @return 配列のサイズです。
    //---------------------------------------------------------------------------
    int Size() NN_NOEXCEPT
    {
        return m_HoldCount;
    }

private:
    //---------------------------------------------------------------------------
    //! @brief        指定した要素を削除します。
    //---------------------------------------------------------------------------
    void erase( typename PlacementArray::size_type index ) NN_NOEXCEPT
    {
        (m_PlacementArray.data())[index] = 0x0;
    }

    NN_ALIGNAS(32) unsigned char       m_FixedSizeElements[sizeof(TElement) * TSize];  //!< 固定配列
    int                                m_HoldCount;                                    //!< 要素数
    PlacementArray                     m_PlacementArray;                               //!<  プレースメント配列の変数
};

} // namespace detail
} // namespace vfx
} // namespace nn
