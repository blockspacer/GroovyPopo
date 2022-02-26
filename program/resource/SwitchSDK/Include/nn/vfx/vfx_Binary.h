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

#include <nn/vfx/vfx_Macro.h>
#include <nn/vfx/vfx_EndianUtil.h>

namespace nn {
namespace vfx {
namespace detail {

//------------------------------------------------------------------------------
//! @brief      バイナリデータフリップフラグ
//------------------------------------------------------------------------------
enum BinaryDataFlipFlag
{
    BinaryDataFlipFlag_Unflipped    = 0,            //!< 未フリップ
    BinaryDataFlipFlag_Flipped      = 1,            //!< フリップ済
};

//------------------------------------------------------------------------------
//! @brief バイナリデータ定数
//------------------------------------------------------------------------------
enum BinaryDataState
{
    BinaryDataState_NoData          = 0xFFFFFFFF,   //!< バイナリデータ無し
};

//------------------------------------------------------------------------------
//! @brief      バイナリデータ
//------------------------------------------------------------------------------
struct BinaryData
{
    char                tag[ 4 ];           //!< バイナリタグ
    uint32_t            binSize;            //!< バイナリサイズ
    uint32_t            child;              //!< 子方向のバイナリオフセット
    uint32_t            next;               //!< 右方向のバイナリオフセット
    uint32_t            sub;                //!< サブバイナリデータ
    uint32_t            offset;             //!< バイナリイメージへのオフセット
    uint32_t            guid;               //!< バイナリツリー内におけるユニークID
    uint16_t            childCount;         //!< 子要素の個数
    uint8_t             flipped;            //!< フリップが必要な場合に、このノードが既にフリップされたかどうか
    uint8_t             endian;             //!< このノードのエンディアン

    //------------------------------------------------------------------------------
    //! @brief          パラメータ初期化
    //------------------------------------------------------------------------------
    void Initialize() NN_NOEXCEPT
    {
        binSize     = 0;
        child       = static_cast< uint32_t >( BinaryDataState_NoData );
        next        = static_cast< uint32_t >( BinaryDataState_NoData );
        sub         = static_cast< uint32_t >( BinaryDataState_NoData );
        offset      = sizeof( BinaryData );
        guid        = 0;
        childCount  = 0;
        flipped     = 0;
        endian      = 0;
    }

    //------------------------------------------------------------------------------
    //! @brief          バイナリタグ設定
    //! @param[in]  a   タグ文字1
    //! @param[in]  b   タグ文字2
    //! @param[in]  c   タグ文字3
    //! @param[in]  d   タグ文字4
    //------------------------------------------------------------------------------
    void SetBinaryTag( char a, char b, char c, char d ) NN_NOEXCEPT
    {
        tag[ 0 ] = a;
        tag[ 1 ] = b;
        tag[ 2 ] = c;
        tag[ 3 ] = d;
    }

    //------------------------------------------------------------------------------
    //! @brief          バイナリタグ取得
    //! @return         バイナリタグを返します。
    //------------------------------------------------------------------------------
    uint32_t GetBinaryTag() const NN_NOEXCEPT
    {
        return VFX_MAKE_TAG( tag[ 0 ], tag[ 1 ], tag[ 2 ], tag[ 3 ] );
    }

    //------------------------------------------------------------------------------
    //! @brief          バイナリ取得
    //! @return         バイナリへのポインタを返します。
    //------------------------------------------------------------------------------
    void* GetBinaryData() const NN_NOEXCEPT
    {
        uint32_t offset_size = offset;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &offset_size );
        }
#endif
        return reinterpret_cast< void* >( const_cast< char* >( tag ) + offset_size );
    }

    //------------------------------------------------------------------------------
    //! @brief          フリップを伴うバイナリ取得
    //! @tparam  T      バイナリの型名
    //! @return         バイナリへのポインタを返します。
    //------------------------------------------------------------------------------
    template< typename T >
    T* GetBinaryDataWithFlip() NN_NOEXCEPT
    {
        uint32_t offset_size = offset;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &offset_size );
        }
#endif
        T* binary = reinterpret_cast< T* >( tag + offset_size );
#if defined( VFX_BI_ENDIAN )
        if( IsNeedFlip() )
        {
            binary->FlipEndian();
            flipped = BinaryDataFlipFlag_Flipped;
        }
#endif
        return binary;
    }

    //------------------------------------------------------------------------------
    //! @brief          バイナリサイズ取得
    //! @return         バイナリサイズを返します。
    //------------------------------------------------------------------------------
    size_t GetBinarySize() const NN_NOEXCEPT
    {
        uint32_t binary_size = binSize;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &binary_size );
        }
#endif
        return binary_size;
    }

    //------------------------------------------------------------------------------
    //! @brief          次要素へのバイナリオフセット取得
    //! @return         次要素へのバイナリオフセットを返します。
    //------------------------------------------------------------------------------
    size_t GetNextBinaryOffset() const NN_NOEXCEPT
    {
        uint32_t binary_size = next;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &binary_size );
        }
#endif
        return binary_size;
    }

    //------------------------------------------------------------------------------
    //! @brief          サブバイナリ取得
    //! @return         サブバイナリへのポインタを返します。
    //------------------------------------------------------------------------------
    BinaryData* GetSubData() const NN_NOEXCEPT
    {
        uint32_t binary_sub = sub;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &binary_sub );
        }
#endif

        if( binary_sub == static_cast< uint32_t >( BinaryDataState_NoData ) )
        {
            return NULL;
        }
        return reinterpret_cast< BinaryData* >( const_cast< char* >( tag ) + binary_sub );
    }

    //------------------------------------------------------------------------------
    //! @brief          子バイナリ取得
    //! @return         子バイナリへのポインタを返します。
    //------------------------------------------------------------------------------
    BinaryData* GetChildData() const NN_NOEXCEPT
    {
        uint32_t child_offset = child;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &child_offset );
        }
#endif

        if( child_offset == static_cast< uint32_t >( BinaryDataState_NoData ) )
        {
            return NULL;
        }
        return reinterpret_cast< BinaryData* >( const_cast< char* >( tag ) + child_offset );
    }

    //------------------------------------------------------------------------------
    //! @brief          Nextバイナリ取得
    //! @return         次データへのポインタを返します。
    //------------------------------------------------------------------------------
    BinaryData* GetNextData() const NN_NOEXCEPT
    {
        uint32_t next_offset = next;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &next_offset );
        }
#endif

        if( next_offset == static_cast< uint32_t >( BinaryDataState_NoData ) )
        {
            return NULL;
        }

        return reinterpret_cast< BinaryData* >( const_cast< char* >( tag ) + next_offset );
    }

    //------------------------------------------------------------------------------
    //! @brief              タグを指定して子バイナリ取得
    //! @param[in] tagValue タグ
    //! @return             指定したタグが存在すればそのポインタを返し、無ければNULLを返します。
    //------------------------------------------------------------------------------
    BinaryData* GetDirectChildData( uint32_t tagValue ) const NN_NOEXCEPT
    {
        BinaryData* pChild = GetChildData();
        if( !pChild )
        {
            return NULL;
        }

        while( pChild )
        {
            if( pChild->GetBinaryTag() == tagValue )
            {
                return pChild;
            }
            pChild = pChild->GetNextData();
        }
        return NULL;
    }

    //------------------------------------------------------------------------------
    //! @brief          子バイナリ数取得
    //! @return         子バイナリの数を返します。
    //------------------------------------------------------------------------------
    int GetDirectChildCount() const NN_NOEXCEPT
    {
        int count = 0;
        const BinaryData* pChild = GetChildData();
        while( pChild )
        {
            count++;
            pChild = pChild->GetNextData();
        }
        return count;
    }

    //------------------------------------------------------------------------------
    //! @brief               タグを指定して子バイナリ数取得
    //! @param[in]  tagValue 子バイナリのタグ
    //! @return              指定したタグが存在すればその子バイナリの数を返します。無い場合は0を返します。
    //------------------------------------------------------------------------------
    int GetDirectChildCount( uint32_t tagValue ) const NN_NOEXCEPT
    {
        int count = 0;
        BinaryData* pChild = GetChildData();
        while( pChild )
        {
            if( pChild->GetBinaryTag() == tagValue )
            {
                count++;
            }
            pChild = pChild->GetNextData();
        }
        return count;
    }

    //------------------------------------------------------------------------------
    //! @brief               タグを指定してNextバイナリ取得
    //! @param[in]  tagValue タグ
    //! @return              指定したタグが存在すればそのポインタを返し、無い場合はNULLを返します。
    //------------------------------------------------------------------------------
    BinaryData* GetNextData( uint32_t tagValue ) const NN_NOEXCEPT
    {
        BinaryData* pNext = GetNextData();
        if( !pNext )
        {
            return NULL;
        }

        while( pNext )
        {
            if( pNext->GetBinaryTag() == tagValue )
            {
                return pNext;
            }
            pNext = pNext->GetNextData();
        }
        return NULL;
    }

    //------------------------------------------------------------------------------
    //! @brief          GUID取得
    //! @return         GUIDを返します。
    //------------------------------------------------------------------------------
    int GetGuid() const NN_NOEXCEPT
    {
        int temp = guid;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &temp );
        }
#endif
        return temp;
    }

    //------------------------------------------------------------------------------
    //! @brief          ヘッダに記載された子バイナリ数取得
    //! @return         ヘッダに記載された子バイナリ数を返します。
    //------------------------------------------------------------------------------
    int GetChildCount() const NN_NOEXCEPT
    {
        uint16_t temp = childCount;
#if defined( VFX_BI_ENDIAN )
        if( endian != SystemEndianType )
        {
            EndianUtil::Flip( &temp );
        }
#endif
        return temp;
    }

    //------------------------------------------------------------------------------
    //! @brief          フリップが必要な状態かを取得
    //! @return         フリップが必要な状態ならtrue,不要ならfalseを返します。
    //------------------------------------------------------------------------------
    bool IsNeedFlip() const NN_NOEXCEPT
    {
#if defined( VFX_BI_ENDIAN )
        return endian != SystemEndianType && static_cast< uint32_t >( flipped ) == BinaryDataFlipFlag_Unflipped;
#else
        return false;
#endif
    }
};

} // namespace detail
} // namespace vfx
} // namespace nn
