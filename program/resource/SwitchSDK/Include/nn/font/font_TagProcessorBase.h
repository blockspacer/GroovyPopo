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

#include <nn/font/font_Types.h>
#include <nn/font/font_Util.h>

namespace nn {
namespace font {



template <typename CharType>
class TextWriterBase;


template <typename CharType>
class PrintContext
{
public:// TODO:アクセサ
    TextWriterBase<CharType>*   writer;     //!< 文字列描画に使用しているTextWriterへのポインタ。
    const CharType*             str;        //!< 描画中の文字列へのポインタ。
    const CharType*             strEnd;     //!< 描画中の文字列の終端のポインタ。
    const float                   xOrigin;    //!< 描画開始X座標。
    const float                   yOrigin;    //!< 描画開始Y座標。
    const float                   hScale;     //!< 描画開始時点の水平方向スケール。
    const float                   vScale;     //!< 描画開始時点の垂直方向スケール。
    uint32_t                          flags;      //!< TextWriter::ContextFlag のOR。
    uint32_t                          prevCode;   //!< ひとつ前の文字。カーニングの値を求めるために使用します。

    PrintContext(
        TextWriterBase<CharType>*   aWriter,
        const CharType*             aStr,
        const CharType*             aStrEnd,
        const float                   aXOrigin,
        const float                   aYOrigin,
        const float                   aHScale,
        const float                   aVScale,
        uint32_t                          aFlags
    )
    :   writer(aWriter),
        str(aStr),
        strEnd(aStrEnd),
        xOrigin(aXOrigin),
        yOrigin(aYOrigin),
        hScale(aHScale),
        vScale(aVScale),
        flags(aFlags),
        prevCode(0)
    {}

private:
    // コピー演算子を禁止します。
    const PrintContext&   operator=( const PrintContext& rhs);
};

#if defined( __CC_ARM ) || defined( __ARMCC_VERSION )
#pragma push
#pragma diag_suppress 2819  // class XXX has an implicitly instantiated key function YYY
#endif

//---------------------------------------------------------------------------
//! @brief        グリフグループが定義されているフォントリソースを扱うクラスです。
//!
//! @tparam       CharType 文字の型。
//---------------------------------------------------------------------------
template <typename CharType>
class TagProcessorBase
{
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO_ROOT();

    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */
    typedef PrintContext<CharType>  ContextType;

    //! 呼び出し元 (TextWriter) が行うべき処理の列挙子です。
    enum Operation
    {
        //! @brief      行頭では次の文字との間に文字間を開けません。
        //!             行頭以外の位置では次の文字との間に文字間を開けます。
        //!
        Operation_Default,

        //! @brief      行頭以外の位置でタグ処理が行われた場合、
        //!             デフォルトでは次の文字との間に文字間を空けますが、
        //!             これを空けないようにします。
        //!
        Operation_NoCharSpace,

        //! @brief      行頭でタグ処理が行われた場合、
        //!             デフォルトでは次の文字との間に文字間を空けませんが、
        //!             これを空けるようにします。
        //!
        Operation_CharSpace,

        //! @brief      改行時に必要な処理のうち X 座標の調整のみを行います。
        //!             Y 座標の調整は行いません。
        //!             また、次の文字との間に文字間を開けません。
        //!
        Operation_NextLine,

        //! @brief      文字列の途中で文字列描画を終了する場合に使用します。
        //!
        Operation_EndDraw,

        //! @brief 内部用機能のため使用禁止です。
        Operation_MaxOperation
    };

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ / デストラクタ
    //@{

    //! コンストラクタです。
                        TagProcessorBase();

    //! デストラクタです。
    virtual             ~TagProcessorBase();

    //@}


    //! @name タグ処理
    //@{

    //! @brief      タグに従って処理を行います。
    //!
    //! @param[in]      code      タグ処理の起点となった制御文字コード。(0x0000 ～ 0x001F)
    //! @param[in]  pContext  文字列の描画状態情報へのポインタ。
    //!
    //! @return     呼び出し元 (TextWriter) が行うべき処理を返します。
    //!
    virtual Operation   Process(
                            uint32_t                      code,
                            PrintContext<CharType>* pContext);

    //! @brief      タグの影響範囲を計算します。
    //!
    //! @param[out]     pRect     タグの影響範囲を格納する矩形構造体へのポインタ。
    //! @param[in]      pContext  文字列の描画状態情報へのポインタ。
    //! @param[in]      code      タグ処理の起点となった制御文字コード。(0x0000 ～ 0x001F)
    //!
    //! @return     呼び出し元 (TextWriter) が行うべき処理を返します。
    //!             同じタグに対して Process() 関数が返す値と同じ値を返すべきです。
    //!
    virtual Operation   CalculateRect(
                            nn::font::Rectangle*    pRect,
                            PrintContext<CharType>* pContext,
                            uint32_t                code);

    //! @brief 行の高さを返します。
    //!
    //! @details
    //! デフォルトの実装では、「フォントが持つ改行幅 * スケール Y + 行間」で計算される値が返ります。
    //! タグによって文字のスケールを変えたときに、想定よりも行の高さが空いてしまうときには
    //! このメソッドをオーバーライドすることで行の高さを調整することができます。
    //!
    //! @param[in]      pTextWriter TextWriter へのポインタ。
    //!
    //! @return         行の高さです。
    //!
    virtual float GetLineHeight(const TextWriterBase<CharType>* pTextWriter) const;

    //@}

    //! @name 処理開始/終了コールバック
    //@{

    //! @brief      テキスト全体のプリントを行う前に呼び出されるコールバックです。
    //!
    //! @param[in]  pContext  文字列の描画状態情報へのポインタ。
    //!
    virtual void BeginPrint(PrintContext<CharType>* pContext);

    //! @brief      テキスト全体のプリントを行った後に呼び出されるコールバックです。
    //!
    //! @param[in]  pContext  文字列の描画状態情報へのポインタ。
    //!
    virtual void EndPrint(PrintContext<CharType>* pContext);

    //! @brief      テキスト一行の占める領域の計算を行う前に呼び出されるコールバックです。
    //!
    //! @param[in]  pContext  文字列の描画状態情報へのポインタ。
    //!
    virtual void BeginCalculateRect(PrintContext<CharType>* pContext);

    //! @brief      テキスト一行の占める領域の計算を行った後に呼び出されるコールバックです。
    //!
    //! @param[in]  pContext  文字列の描画状態情報へのポインタ。
    //!
    virtual void EndCalculateRect(PrintContext<CharType>* pContext);

    //! @brief      テキスト全体のプリントを行う前に呼び出されるコールバックです。
    //!
    //! @details
    //! nn::font::TagProcessorBase::BeginPrint は、改行位置を計算した後に呼ばれます。
    //! 改行位置の計算に使うパラメータを設定しておくなど、
    //! 改行位置を計算する前に呼びたい場合は本メソッドをお使いください。
    //!
    //! @param[in] pTextWriter TextWriter へのポインタ。
    //! @param[in] pString 文字列の先頭のポインタ。
    //! @param[in] pEnd 文字列の終端のポインタ。
    //!
    virtual void BeginPrintWhole(const TextWriterBase<CharType>* pTextWriter, const CharType* pString, const CharType* pEnd);

    //! @brief      テキスト全体のプリントを行った後に呼び出されるコールバックです。
    //!
    //! @details
    //! nn::font::TagProcessorBase::EndPrint と同じタイミングで呼ばれます。
    //!
    //! @param[in] pTextWriter TextWriter へのポインタ。
    //! @param[in] pString 文字列の先頭のポインタ。
    //! @param[in] pEnd 文字列の終端のポインタ。
    //!
    virtual void EndPrintWhole(const TextWriterBase<CharType>* pTextWriter, const CharType* pString, const CharType* pEnd);

    //! @brief      テキスト全体の占める領域の計算を行う前に呼び出されるコールバックです。
    //!
    //! @details
    //! nn::font::TagProcessorBase::BeginCalculateRect は、テキスト 1 行ごとにコールバックが呼び出されます。
    //! テキスト全体に対して 1 回だけ呼び出されたい場合は本メソッドをお使いください。
    //!
    //! @param[in] pTextWriter TextWriter へのポインタ。
    //! @param[in] pString 文字列の先頭のポインタ。
    //! @param[in] pEnd 文字列の終端のポインタ。
    //!
    virtual void BeginCalculateRectWhole(const TextWriterBase<CharType>* pTextWriter, const CharType* pString, const CharType* pEnd);

    //! @brief      テキスト全体の占める領域の計算を行った後に呼び出されるコールバックです。
    //!
    //! @details
    //! nn::font::TagProcessorBase::EndCalculateRect は、テキスト 1 行ごとにコールバックが呼び出されます。
    //! テキスト全体に対して 1 回だけ呼び出されたい場合は本メソッドをお使いください。
    //!
    //! @param[in] pTextWriter TextWriter へのポインタ。
    //! @param[in] pString 文字列の先頭のポインタ。
    //! @param[in] pEnd 文字列の終端のポインタ。
    //!
    virtual void EndCalculateRectWhole(const TextWriterBase<CharType>* pTextWriter, const CharType* pString, const CharType* pEnd);

    //! @brief      描画可能な文字の位置を返すコールバックです。
    //!
    //! @details
    //! このメソッドは、呼び出し元が描画可能な文字を得るために使用します。
    //! 与えられた position の位置の文字が描画可能かどうかを pIsPrintable に設定し、
    //! position の次の文字の位置を返します。
    //!
    //! @param[out] pIsPrintable position の位置の文字が描画可能かどうか。
    //! @param[in]  position    呼び出し元から与えられる位置。
    //!
    //! @return     position の次の文字の位置。
    //!
    virtual const CharType* AcquireNextPrintableChar(bool* pIsPrintable, const CharType* position);

    //@}

private:
    void                ProcessLinefeed(ContextType* pContext) const;
    void                ProcessTab(ContextType* pContext) const;
};

#if defined( __CC_ARM ) || defined( __ARMCC_VERSION )
#pragma pop
#endif

}   // namespace font
}   // namespace nn
