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
#include <nn/util/util_Endian.h>

namespace nn { namespace util {

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         リソースクラスのエンディアン反転用ユーティリティです。
class ResEndian
{
public:
    //! @brief      コンストラクタです。
    //! @param[in]  pHeader ファイルヘッダです。
    explicit ResEndian( BinaryFileHeader* pHeader ) NN_NOEXCEPT
        : m_pHeader( pHeader )
        , m_HostEndian( pHeader->_byteOrderMark == nn::util::ByteOrderMark_Normal )
    {
    }

    //! @brief  基準となるアドレスを取得します。
    //! @return 基準となるアドレスを返します。
    void* GetBase() const NN_NOEXCEPT
    {
        return m_pHeader;
    }

    //! @brief  リソースのエンディアンが動作中のマシンと一致しているかどうかを取得します。
    //! @return リソースのエンディアンが動作中のマシンと一致しているかどうかを返します。
    bool IsHostEndian() const NN_NOEXCEPT
    {
        return m_HostEndian;
    }

    //! @brief      ポインタが指す対象のエンディアンを反転します。
    //! @tparam     T       ポインタが指す対象の型です。
    //! @param[in]  pBinPtr エンディアン反転するポインタです。
    template< typename T >
    void SwapPtr( BinTPtr< T >* pBinPtr ) NN_NOEXCEPT
    {
        if ( T* ptr = pBinPtr->ToPtr( m_pHeader ) )
        {
            SwapEndian( this, ptr );
        }
    }

    //! @brief      配列のエンディアンを反転します。
    //! @tparam     T       ポインタが指す対象の型です。
    //! @param[in]  pBinPtr エンディアン反転する配列です。
    //! @param[in]  length  配列の長さです。
    template< typename T >
    void SwapArray( BinTPtr< T >* pBinPtr, int length ) NN_NOEXCEPT
    {
        T* iter = pBinPtr->ToPtr( m_pHeader );
        for ( T* end = iter + length; iter != end; ++iter )
        {
            SwapEndian( this, iter );
        }
    }

    //! @brief      ポインタ配列のエンディアンを反転します。
    //! @tparam     T       ポインタが指す対象の型です。
    //! @param[in]  pBinPtr エンディアン反転するポインタ配列です。
    //! @param[in]  length  配列の長さです。
    template< typename T >
    void SwapArray( BinTPtr< T* >* pBinPtr, int length ) NN_NOEXCEPT
    {
        BinTPtr< T >* iter = reinterpret_cast< BinTPtr< T >* >( pBinPtr->ToPtr( m_pHeader ) );
        for ( BinTPtr< T >* end = iter + length; iter != end; ++iter )
        {
            SwapPtr( iter );
        }
    }

    //! @brief      文字列配列のエンディアンを反転します。
    //! @tparam     CharT       文字の型です。
    //! @param[in]  pBinString  エンディアン反転する文字列の配列です。
    //! @param[in]  length      配列の長さです。
    template< typename CharT >
    void SwapStrings( BinTString< CharT >* pBinString, int length ) NN_NOEXCEPT
    {
        if ( m_HostEndian )
        {
            for ( int count = 0; count < length; ++count )
            {
                CharT* iter = pBinString->_data;
                for ( CharT* end = iter + pBinString->_length; iter != end; ++iter )
                {
                    nn::util::SwapEndian( iter );
                }
                BinTString< CharT >* pNext = pBinString->GetNext(); // m_Len の変換前に退避します。
                nn::util::SwapEndian( &pBinString->_length );
                pBinString = pNext;
            }
        }
        else
        {
            for ( int count = 0; count < length; ++count )
            {
                nn::util::SwapEndian( &pBinString->_length );
                CharT* iter = pBinString->_data;
                for ( CharT* end = iter + pBinString->_length; iter != end; ++iter )
                {
                    nn::util::SwapEndian( iter );
                }
                pBinString = pBinString->GetNext();
            }
        }
    }

private:
    BinaryFileHeader* m_pHeader;
    bool m_HostEndian;
};

//! @cond

// エンディアン反転対象の型に対して同じ名前空間にオーバーロードを用意することで
// ResEndian から実引数依存の名前探索によって呼び出すことができます。

inline void SwapEndian( ResEndian* /*pCtx*/, int8_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, int16_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, int32_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, int64_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, uint8_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, uint16_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, uint32_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, uint64_t* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, float* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

inline void SwapEndian( ResEndian* /*pCtx*/, double* pData ) NN_NOEXCEPT
{
    nn::util::SwapEndian( pData );
}

class ResDic;
void SwapEndian( ResEndian* pCtx, ResDic* pRes ) NN_NOEXCEPT;

struct StringPool;
void SwapEndian( ResEndian* pCtx, StringPool* pRes ) NN_NOEXCEPT;

//! @endcond

}} // namespace nn::util
