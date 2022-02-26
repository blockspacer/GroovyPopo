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
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_BinTypes.h>
#include <nn/util/util_StringView.h>
#include <nn/util/util_AccessorBase.h>

namespace nn { namespace util {

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         辞書構造体です。
struct ResDicData
{
    BinBlockSignature signature;    //!< シグネチャです。
    int32_t count;                  //!< 登録している項目の数です。
    struct Entry
    {
        int32_t refBit;             //!< キーにおける参照ビット位置です。
        uint16_t children[ 2 ];     //!< 子のインデックスです。
        BinPtrToString pKey;        //!< キーへのオフセットです。
    } entries[ 1 ]; //!< count + 1 個の Entry の配列です。
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         辞書構造体のアクセサクラスです。
class ResDic : public AccessorBase< ResDicData >
{
    NN_DISALLOW_COPY( ResDic );

public:
    //! @brief 無効なインデックスです。
    static const int Npos = -1;

    //! @brief  辞書に登録されている要素の数を取得します。
    //! @return 辞書に登録されている要素の数を返します。
    int GetCount() const NN_NOEXCEPT
    {
        return static_cast< int >( ToData().count );
    }

    //! @brief      指定したインデックスの要素の名前を取得します。
    //! @param[in]  entryIndex  指定するインデックスです。
    //! @return     指定したインデックスの要素の名前を返します。
    nn::util::string_view GetKey( int entryIndex ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( 0 <= entryIndex );
        NN_SDK_REQUIRES( entryIndex < GetCount() );
        return ToData().entries[ 1 + entryIndex ].pKey.Get()->Get();
    }

    //! @brief      指定したキーを持つ要素のインデックスを取得します。
    //! @param[in]  key     指定するキーです。
    //! @return     指定したキーを持つ要素のインデックスを返します。
    int FindIndex( const nn::util::string_view& key ) const NN_NOEXCEPT
    {
        const Entry* pEntry = FindImpl( key );
        return *pEntry->pKey.Get() == key ? static_cast< int >( std::distance( &ToData().entries[ 1 ], pEntry )) : Npos;
    }

    //! @brief  辞書を構築します。
    //! @return 構築に成功したかどうかを返します。
    bool Build() NN_NOEXCEPT;

    //! @brief      辞書のサイズを計算します。
    //! @param[in]  length  辞書内の要素の数です。
    //! @return     辞書のサイズを返します。
    static size_t CalculateSize( int length ) NN_NOEXCEPT
    {
        size_t size = sizeof( ResDicData );
        size += sizeof( ResDicData::Entry ) * length;
        return size;
    }

private:
    //! @brief 参照ビットを抽出します。
    static int ExtractRefBit( const nn::util::string_view& key, int refBit ) NN_NOEXCEPT
    {
        // 文字列の末尾から数えて refBit 番目のビットを返します。
        int charIndex = refBit >> nn::util::detail::Log2< CHAR_BIT >::Value;
        if ( static_cast< size_t >( charIndex ) < key.length() )
        {
            int bitIndex = refBit & ( CHAR_BIT - 1 );
            return ( key[ key.length() - charIndex - 1 ] >> bitIndex ) & 0x1;
        }
        else
        {
            // 参照ビットが key の外の場合は 0 を返します。
            return 0;
        }
    }

    //! @brief 参照ビットを探索します。
    static int FindRefBit( const nn::util::string_view& lhs, const nn::util::string_view& rhs ) NN_NOEXCEPT;

    //! @brief 左辺が右辺よりルートに近いかどうかを返します。
    static bool Older( const Entry* pParent, const Entry* pChild )
    {
        return pParent->refBit < pChild->refBit;
    }

    //! @brief キーに対応する子を取得します。
    Entry* GetChild( const Entry* pParent, const nn::util::string_view& key ) NN_NOEXCEPT
    {
        // ビットが 1 なら右、0 なら左をたどります。
        int childIndex = ExtractRefBit( key, pParent->refBit );
        int entryIndex = pParent->children[ childIndex ];
        return &ToData().entries[ entryIndex ];
    }

    //! @brief キーに対応する子を取得します。
    const Entry* GetChild( const Entry* pParent, const nn::util::string_view& key ) const NN_NOEXCEPT
    {
        // ビットが 1 なら右、0 なら左をたどります。
        int childIndex = ExtractRefBit( key, pParent->refBit );
        int entryIndex = pParent->children[ childIndex ];
        return &ToData().entries[ entryIndex ];
    }

    //! @brief ノードを子ノードとして設定します。
    void SetChild( Entry* pParent, Entry* pChild ) NN_NOEXCEPT
    {
        // ビットが 1 なら右、0 なら左の子供にします。
        int childIndex = ExtractRefBit( *pChild->pKey.Get(), pParent->refBit );
        uint16_t entryIndex = static_cast< uint16_t >( std::distance( &ToData().entries[ 0 ], pChild ) );
        pParent->children[ childIndex ] = entryIndex;
    }

    //! @brief キーに対応するエントリの候補を探索します。
    const Entry* FindImpl( const nn::util::string_view& key ) const NN_NOEXCEPT
    {
        const Entry* pParent = &ToData().entries[ 0 ];
        const Entry* pChild = &ToData().entries[ pParent->children[ 0 ] ];

        while ( Older( pParent, pChild ) )
        {
            pParent = pChild;
            pChild = GetChild( pChild,  key );
        }

        return pChild;
    }
};

}} // namespace nn::util
