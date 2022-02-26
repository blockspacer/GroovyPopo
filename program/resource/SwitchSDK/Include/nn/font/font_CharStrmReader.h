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

#include <nn/nn_SdkAssert.h>
#include <nn/font/font_Types.h>
#include <nn/font/font_Util.h>

namespace nn {
namespace font {

//---------------------------------------------------------------------------
//! @brief        文字ストリームを読み取るための方法を提供します。
//---------------------------------------------------------------------------
class CharStrmReader
{
    friend class Font;

public:
    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ / デストラクタ
    //@{

    //! @brief      コンストラクタです。
    //!
    //! @param[in]  rhs     コピー元のオブジェクト。
    //!
    NN_IMPLICIT CharStrmReader(const CharStrmReader& rhs)
    : m_pCharStream(rhs.m_pCharStream),
      m_pReadFunc(rhs.m_pReadFunc)
    {
    }

    //! デストラクタです。
                            ~CharStrmReader()
    {
    }

    //@}


    //! @name ストリーム読み取り
    //@{

    //! @brief      CharStrmReader が解釈する文字ストリームを設定します。
    //!
    //! @param[in]  pCharStream  新しく設定するストリームの開始点。
    //!
    void                    Set(const char* pCharStream)
    {
        NN_SDK_ASSERT_NOT_NULL(pCharStream);
        NN_SDK_ASSERT(  m_pReadFunc == &CharStrmReader::ReadNextCharUtf8
                   || m_pReadFunc == &CharStrmReader::ReadNextCharCp1252
                   || m_pReadFunc == &CharStrmReader::ReadNextCharSjis   );
        m_pCharStream = pCharStream;
    }

    //! @brief      CharStrmReader が解釈する文字ストリームを設定します。
    //!
    //! @param[in]  pCharStream  新しく設定するストリームの開始点。
    //!
    void                    Set(const uint16_t* pCharStream)
    {
        NN_SDK_ASSERT(detail::IsAligned(pCharStream, 2));
        NN_SDK_ASSERT_NOT_NULL(pCharStream);
        NN_SDK_ASSERT(m_pReadFunc == &CharStrmReader::ReadNextCharUtf16);
        m_pCharStream = pCharStream;
    }

    //! @brief      ストリームの現在位置へのポインタを取得します。
    //!
    //! @return     ストリーム中の次に読み取られる文字へのポインタを返します。
    //!
    const void*             GetCurrentPos() const   { return m_pCharStream; }

    //! @brief      文字ストリームの次の文字の文字コードを取得すると共に、
    //!             現在位置を一文字分進めます。
    //!
    //! @return     次の文字の文字コードを返します。
    //!
    uint32_t                      Next()                  { return (this->*m_pReadFunc)(); }

    //@}

private:
    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */
    typedef uint32_t  (CharStrmReader::*ReadNextCharFunc)();


    /* ------------------------------------------------------------------------
            変数
       ------------------------------------------------------------------------ */
    const void*             m_pCharStream;      // ストリーム現在位置へのポインタ
    const ReadNextCharFunc  m_pReadFunc;      // ストリーム解釈関数へのポインタ


    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */
    //---- コンストラクタ
    explicit                CharStrmReader(ReadNextCharFunc func)
    : m_pCharStream(NULL),
      m_pReadFunc(func)
    {
    }

    //---- ストリーム操作
    template <typename CharType>
    CharType                GetChar() const
    {
        return GetChar<CharType>(0);
    }

    template <typename CharType>
    CharType                GetChar(int offset) const
    {
        const CharType* pCharStream = reinterpret_cast<const CharType*>(m_pCharStream);
        return *(pCharStream + offset);
    }

    template <typename CharType>
    void                    StepStream()
    {
        StepStream<CharType>(1);
    }

    template <typename CharType>
    void                    StepStream(int step)
    {
        const CharType*& charStream = reinterpret_cast<const CharType*&>(m_pCharStream);
        charStream += step;
    }

    //! @brief      UTF8文字列ストリームの現在位置の文字の文字コードを読み取り、
    //!             文字列ストリームの現在位置を1文字分進めます。
    //!
    //! @return     ストリームの現在位置の文字の文字コード。
    //!
    uint32_t                      ReadNextCharUtf8();

    //! @brief      UTF16文字列ストリームの現在位置の文字の文字コードを読み取り、
    //!             文字列ストリームの現在位置を1文字分進めます。
    //!
    //! @return     ストリームの現在位置の文字の文字コード。
    //!
    uint32_t                      ReadNextCharUtf16();

    //! @brief      CP1252文字列ストリームの現在位置の文字の文字コードを読み取り、
    //!             文字列ストリームの現在位置を1文字分進めます。
    //!
    //! @return     ストリームの現在位置の文字の文字コード。
    //!
    uint32_t                      ReadNextCharCp1252();

    //! @brief      SJIS文字列ストリームの現在位置の文字の文字コードを読み取り、
    //!             文字列ストリームの現在位置を1文字分進めます。
    //!
    //! @return     ストリームの現在位置の文字の文字コード。
    //!
    uint32_t                      ReadNextCharSjis();

    // コピー演算子を禁止します。
    const CharStrmReader&   operator=( const CharStrmReader& rhs);
};



}   // namespace font
}   // namespace nn
