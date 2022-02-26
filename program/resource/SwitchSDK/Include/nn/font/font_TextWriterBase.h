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

#include <nn/util/util_FormatString.h>
#include <nn/font/font_Font.h>
#include <nn/font/font_CharWriter.h>
#include <nn/font/font_TagProcessorBase.h>
#include <nn/font/font_ExtendedTagProcessorBase.h>

#include <nn/util/util_CharacterEncoding.h>

namespace nn {
namespace font {

inline int
CalculateWideCharString16Length(const uint16_t* pSrc)
{
    int length = 0;
    for (const uint16_t* cur = pSrc; *cur != 0; ++cur, ++length) {}
    return length;
}

//---------------------------------------------------------------------------
//! @brief        nn::font::Font クラスを用いて文字列の描画を行うためのクラスです。
//!
//! @tparam       CharType 文字の型。
//---------------------------------------------------------------------------
template <typename CharType>
class TextWriterBase: public CharWriter
{
public:
    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */

    //! SetDrawFlag() 関数で使用する文字列の配置を行う文字列描画フラグです。
    enum PositionFlag
    {
        //水平方向の寄せ
        PositionFlag_HorizontalAlignLeft       = 0x0,      //!< 複数行文字列を左寄せする。
        PositionFlag_HorizontalAlignCenter     = 0x1,      //!< 複数行文字列を中央寄せする。
        PositionFlag_HorizontalAlignRight      = 0x2,      //!< 複数行文字列を右寄せする。
        PositionFlag_HorizontalAlignMask       = 0x3,

        // 原点の水平方向位置
        PositionFlag_HorizontalOriginLeft      = 0x00,     //!< 原点を文字列の左端に置く。
        PositionFlag_HorizontalOriginCenter    = 0x10,     //!< 原点を文字列の水平方向中央に置く。
        PositionFlag_HorizontalOriginRight     = 0x20,     //!< 原点を文字列の右端に置く。
        PositionFlag_HorizontalOriginMask      = 0x30,

        // 原点の垂直方向位置
        PositionFlag_VerticalOriginTop         = 0x000,    //!< 原点を文字列の上端に置く。
        PositionFlag_VerticalOriginMiddle      = 0x100,    //!< 原点を文字列の垂直方向中央に置く。
        PositionFlag_VerticalOriginBottom      = 0x200,    //!< 原点を文字列の 1 行目ベースラインに置く。
        PositionFlag_VerticalOriginBaseLine    = 0x300,    //!< 原点を文字列の下端に置く。
        PositionFlag_VerticalOriginMask        = 0x300
    };

    //! コンテキストフラグです。
    enum ContextFlag
    {
        //! 前の文字との間に文字間が開けられていないことを示します
        ContextFlag_NoCharSpace       = 0x1
    };

     /* ------------------------------------------------------------------------
            定数
       ------------------------------------------------------------------------ */
    //! テキストのフォーマット書式を引数にとるメソッドの対応最大文字数です。
    static const int DefaultFormatBufferSize = 512;

    //! コンストラクタで設定されるデフォルトの描画フラグです。
    static const uint32_t  DefaultDrawFlag =
        PositionFlag_HorizontalAlignLeft | PositionFlag_HorizontalOriginLeft | PositionFlag_VerticalOriginTop;


    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ / デストラクタ
    //@{

    //! コンストラクタです。
                        TextWriterBase();

    //! デストラクタです。
                        ~TextWriterBase();

    //@}

    //! @name 文字間 / 行間 / タブ幅
    //@{

    //! @brief      行間を設定します。
    //!
    //! @param[in]  space   新しい行間。
    //!
    void                SetLineSpace(float space)     { m_LineSpace = space; }

    //! @brief      行間を取得します。
    //!
    //! @return     行間を返します。
    //!
    float                 GetLineSpace() const        { return m_LineSpace; }

    //! @brief      1 行の高さを設定します。
    //!
    //! @param[in]  height  新しい行の高さ。
    //!
    void                SetLineHeight(float height);

    //! @brief      1 行の高さを取得します。
    //!
    //! @return     1 行の高さを返します。
    //!
    float                 GetLineHeight() const;

    //! @brief      文字間を設定します。
    //!
    //! @param[in]  space   新しい文字間。
    //!
    void                SetCharSpace(float space)     { m_CharSpace = space; }

    //! @brief      文字間を取得します。
    //!
    //! @return     文字間を返します。
    //!
    float                 GetCharSpace() const        { return m_CharSpace; }

    //! @brief 折り返しを計算するときのオフセットを設定します。
    //!
    //! @param[in]  offset オフセット。
    //!
    void                SetWidthLimitOffset(float offset)     { m_WidthLimitOffset = offset; }

    //! @brief 折り返しを計算するときのオフセットを取得します。
    //!
    //! @return     オフセットを返します。
    //!
    float               GetWidthLimitOffset() const            { return m_WidthLimitOffset; }

    //! @brief      タブ幅を指定します。
    //!
    //! @param[in]  tabWidth    文字数換算のタブ幅。
    //!
    void                SetTabWidth(int tabWidth)   { m_TabWidth = tabWidth; }

    //! @brief      タブ幅を取得します。
    //!
    //! @return     文字数換算のタブ幅を返します。
    //!
    int                 GetTabWidth() const         { return m_TabWidth; }

    //@}

    //! @name 文字列の自動折り返し
    //@{

    //! @brief      自動的に折り返す幅を指定します。
    //!
    //! @param[in]  limit   自動的に折り返す幅。
    //!
    void                SetWidthLimit(float limit)    { m_WidthLimit  = limit; }

    //! @brief      設定されている折り返し幅を取得します。
    //!
    //! @return     設定されている折り返し幅を返します。@n
    //!             折り返し幅が無効に設定されているときは正の無限大を返します。
    //!
    float                 GetWidthLimit() const       { return m_WidthLimit; }

    //! 自動折り返し機能を無効に設定します。
    void                ResetWidthLimit()           { SetWidthLimit(std::numeric_limits<float>::max()); }

    //@}


    //! @name 文字列描画フラグ設定 / 取得
    //@{

    //! @brief      文字列描画フラグを設定します。
    //!
    //! @param[in]  flags   新しい文字列描画フラグ
    //!
    void                SetDrawFlag(uint32_t  flags)      { m_DrawFlag = flags; }

    //! @brief      文字列描画フラグを取得します。
    //!
    //! @return     文字列描画フラグを返します。
    //!
    uint32_t                  GetDrawFlag() const         { return m_DrawFlag; }

    //@}


    //! @name タグプロセッサ設定 / 取得
    //@{

    //! @brief      タグプロセッサを設定します。
    //!
    //! @param[in]  pTagProcessor    新しいタグプロセッサへのポインタ。
    //!
    void                SetTagProcessor(TagProcessorBase<CharType>* pTagProcessor)
    {
        NN_SDK_ASSERT_NOT_NULL(pTagProcessor);

        m_pTagProcessor = pTagProcessor;
    }

    //! @brief      タグプロセッサを取得します。
    //!
    //! @return     タグプロセッサのリファレンスを返します。
    //!
    TagProcessorBase<CharType>&
                        GetTagProcessor() const     { return *m_pTagProcessor; }

    //! タグプロセッサをデフォルトに戻します。
    void                ResetTagProcessor()         { m_pTagProcessor = &g_DefaultTagProcessor; }

    //! @brief 拡張タグを解釈するタグプロセッサを取得します。
    //!
    //! @return 拡張タグを解釈するタグプロセッサです。
    //!
    static ExtendedTagProcessorBase<CharType>* GetExtendedTagProcessor()
    {
        return &g_ExtendedTagProcessor;
    }

    //@}


    //! @name 文字列の描画サイズ計算
    //@{

    //! @brief      文字列の幅を計算します。
    //!
    //! @param[in]  pFormat 書式文字列。
    //! @param[in]  ...     文字列パラメータ。
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 CalculateFormatStringWidth(
                            const CharType* pFormat,
                            ...
                        ) const;

    //! @brief      文字列の幅を計算します。
    //!
    //! @param[in]  pStr    文字列。
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 CalculateStringWidth(
                            const CharType* pStr
                        ) const
    {
        NN_SDK_ASSERT_NOT_NULL(pStr);

        return CalculateStringWidth(pStr, StrLen(pStr));
    }

    //! @brief      文字列の幅を計算します。
    //!
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長。
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 CalculateStringWidth(
                            const CharType* pStr,
                            int             length
                        ) const;


    //! @brief      文字列の高さを計算します。
    //!
    //! @param[in]  pFormat 書式文字列。
    //! @param[in]  ...     文字列パラメータ。
    //!
    //! @return     文字列の高さを返します。
    //!
    float                 CalculateFormatStringHeight(
                            const CharType* pFormat,
                            ...
                        ) const;

    //! @brief      文字列の高さを計算します。
    //!
    //! @param[in]  pStr    文字列。
    //!
    //! @return     文字列の高さを返します。
    //!
    float                 CalculateStringHeight(
                            const CharType* pStr
                        ) const
    {
        NN_SDK_ASSERT_NOT_NULL(pStr);

        return CalculateStringHeight(pStr, StrLen(pStr));
    }

    //! @brief      文字列の高さを計算します。
    //!
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長。
    //!
    //! @return     文字列の高さを返します。
    //!
    float                 CalculateStringHeight(
                            const CharType* pStr,
                            int             length
                        ) const;

    //! @brief      文字列の描画矩形を計算します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は無視して計算されます。
    //!
    //! DefaultFormatBufferSize 以上の文字数には対応していません。
    //! 上記以上の文字数を扱いたい場合は、nn::util::SNPrintf と TextWriter::CalcStringRect を利用してください。
    //!
    //! @param[out] pRect   文字列描画矩形を受け取るバッファへのポインタ。
    //! @param[in]  pFormat 書式文字列。
    //! @param[in]  ...     文字列パラメータ。
    //!
    void                CalculateFormatStringRect(
                            nn::font::Rectangle*    pRect,
                            const CharType*         pFormat,
                            ...
                        ) const;


    //! @brief      文字列の描画矩形を計算します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は無視して計算されます。
    //!
    //! DefaultFormatBufferSize 以上の文字数には対応していません。
    //! 上記以上の文字数を扱いたい場合は、nn::util::SNPrintf と TextWriter::CalcStringRect を利用してください。
    //!
    //! @param[out] pRect   文字列描画矩形を受け取るバッファへのポインタ。
    //! @param[in]  pFormat 書式文字列。
    //! @param[in]  args    文字列パラメータ。
    //!
    void                CalculateVStringRect(
                            nn::font::Rectangle*    pRect,
                            const CharType*         pFormat,
                            std::va_list            args
                        ) const;


    //! @brief      文字列の描画矩形を計算します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は無視して計算されます。
    //!
    //! @param[out] pRect   文字列描画矩形を受け取るバッファへのポインタ。
    //! @param[in]  pStr    文字列。
    //!
    void                CalculateStringRect(
                            nn::font::Rectangle*    pRect,
                            const CharType*         pStr
                        ) const
    {
        NN_SDK_ASSERT_NOT_NULL(pRect);
        NN_SDK_ASSERT_NOT_NULL(pStr);

        CalculateStringRect(pRect, pStr, StrLen(pStr));
    }

    //! @brief      文字列の描画矩形を計算します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は無視して計算されます。
    //!
    //! @param[out] pRect   文字列描画矩形を受け取るバッファへのポインタ。
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長。
    //!
    void                CalculateStringRect(
                            nn::font::Rectangle* pRect,
                            const CharType*      pStr,
                            int                  length
                        ) const;

    //! @brief      一行に表示される文字列の範囲を取得します。
    //!
    //! @details
    //! SetWidthLimit() が指定されている場合には、表示幅の上限に達した時点で
    //! 改行されます。
    //!
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長です。
    //!
    //! @return
    //! 改行された位置を返します。戻り値が指す文字は一行に含まれません。
    //!
     const CharType*    FindPosOfWidthLimit(
                            const CharType* pStr,
                            int             length
                        ) const;

    //! @brief      一行に表示される文字列の範囲を取得します。
    //!
    //! @details
    //! SetWidthLimit() が指定されている場合には、表示幅の上限に達した時点で
    //! 改行されます。
    //!
    //! @param[in]  pStr    ヌル終端された文字列。
    //!
    //! @return
    //! 改行された位置を返します。戻り値が指す文字は一行に含まれません。
    //!
    const CharType*     FindPosOfWidthLimit(
                            const CharType* pStr
                        ) const
    {
        return FindPosOfWidthLimit(pStr, StrLen(pStr));
    }

    //@}


    //! @name 文字列描画
    //@{

    //! @brief      文字列を描画します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は描画がスキップされます。
    //!
    //! DefaultFormatBufferSize 以上の文字数には対応していません。
    //! 上記以上の文字数を扱いたい場合は、nn::util::SNPrintf と TextWriter::Print を利用してください。
    //!
    //! @param[in]  pFormat 書式文字列。
    //! @param[in]  ...     文字列パラメータ
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 Printf(
                            const CharType* pFormat,
                            ... );

    //! @brief      文字列を描画します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は描画がスキップされます。
    //!
    //! DefaultFormatBufferSize 以上の文字数には対応していません。
    //! 上記以上の文字数を扱いたい場合は、nn::util::SNPrintf と TextWriter::Print を利用してください。
    //!
    //! @param[in]  pFormat 書式文字列。
    //! @param[in]  args    文字列パラメータ。
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 VPrintf(
                            const CharType* pFormat,
                            std::va_list    args);

    //! @brief      文字列を描画します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は描画がスキップされます。
    //!
    //! @param[in]  pStr    文字列。
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 Print(
                            const CharType* pStr
                        )
    {
        NN_SDK_ASSERT_NOT_NULL(pStr);

        return Print(pStr, StrLen(pStr));
    }

    //! @brief      文字列を描画します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は描画がスキップされます。
    //!
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長。
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 Print(
                            const CharType* pStr,
                            int             length);

    //! @brief      文字列を描画します。
    //!
    //! @details
    //! 双方向テキストの制御文字(U+202A等)は描画がスキップされます。
    //!
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長。
    //! @param[in]  lineWidthOffsetCount 行ごとの幅とオフセットの数
    //! @param[in]  pLineOffset     行ごとのオフセット
    //! @param[in]  pLineWidth      行ごとの幅
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 Print(
        const CharType* pStr,
        int             length,
        int             lineWidthOffsetCount,
        const float*    pLineOffset,
        const float*    pLineWidth);

    //@}

    //---- vsnprintf の 分岐
    static int          VSNPrintf(
                            char*           pBuffer,
                            std::size_t     count,
                            const char*     pFormat,
                            std::va_list    arg
                        )
    {
        return nn::util::VSNPrintf(pBuffer, count, pFormat, arg);
    }

    static int          VSNPrintf(
                            uint16_t*        pBuffer,
                            std::size_t     count,
                            const uint16_t*  pFormat,
                            std::va_list    arg
                        )
    {
        return VSNW16Printf(pBuffer, count, count - 1, pFormat, arg);
    }

    static int          StrLen(const char* pStr)
    {
        return static_cast<int>(std::strlen(pStr));
    }

    static int          StrLen(const uint16_t* pStr)
    {
        return static_cast<int>(CalculateWideCharString16Length(pStr));
    }

    //! @brief      中央揃えのにじみ防止機能を設定します。
    //!
    //! @param[in]  centerCeilingEnabled    有効にするか否かです。
    //!
    void SetCenterCeilingEnabled(bool centerCeilingEnabled)
    {
        m_CenterCeilingEnabled = centerCeilingEnabled;
    }

    // 改行時にカーニングを引き継ぐ機能を設定します。
    // (過去の不具合の互換用に残していますが、削除予定の機能です。)

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] linefeedKerningEnabled 有効にするか否かです。
    //!
    void SetLinefeedKerningEnabled(bool linefeedKerningEnabled)
    {
        m_LinefeedKerningEnabled = linefeedKerningEnabled;
    }

    //! @brief 文字の高さに応じて改行幅を自動調整するかどうかを設定します。
    //!
    //! @param[in] linefeedByCharacterHeightEnabled 有効にするか否かです。
    //!
    void SetLinefeedByCharacterHeightEnabled(bool linefeedByCharacterHeightEnabled)
    {
        m_LinefeedByCharacterHeightEnabled = linefeedByCharacterHeightEnabled;
    }

    //! @brief 文字の高さに応じて改行幅を自動調整するかどうかを取得します。
    //!
    //! @return 有効か否かです。
    //!
    bool GetLinefeedByCharacterHeightEnabled() const
    {
        return m_LinefeedByCharacterHeightEnabled;
    }

    //! @brief タグを解釈して TextWriter の状態を更新します。
    //!
    //! @details
    //! 文字列中のタグを読み込んで、TagProcessor によって TextWriter の状態を更新したいときにお使いください。
    //!
    //! @param[in]  pStr    文字列。
    //! @param[in]  length  文字列長。
    //!
    void UpdateTextWriterWithTags(const CharType* pStr, int length);

    using CharWriter::Print;

private:
    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */
    typedef TagProcessorBase<CharType>  TagProcessor;
    typedef ExtendedTagProcessorBase<CharType> ExtendedTagProcessor;
    typedef const CharType*             StreamType;

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @brief      1行の長さを計算します。
    //!
    //! @param[in]  pStr    文字列
    //! @param[in]  length  文字列長(sizeof(CharType)単位)
    //!
    float                 CalculateLineWidth(
                            StreamType  pStr,
                            int         length);

    //! @brief      1行の高さを計算します。
    //!
    //! @param[in]  pStr    文字列
    //! @param[in]  length  文字列長(sizeof(CharType)単位)
    //!
    float                 CalculateLineHeight(
                            StreamType  pStr,
                            int         length);

    //! @brief      1行分の描画矩形を計算します。
    //!
    //! @param[out]     pRect   描画矩形を格納するバッファへのポインタ。
    //! @param[in]      pStr    文字列へのポインタを格納するバッファへのポインタ。
    //! @param[in]      length  文字列長(sizeof(CharType)単位)
    //!
    //! @return     WidthLimit を超えたら true を、超えなければ false を返します。
    //!
    bool                CalculateLineRectImpl(
                            nn::font::Rectangle* pRect,
                            StreamType*          pStr,
                            int                  length);

    //! @brief      文字列の描画矩形を計算します。
    //!
    //! @param[out] pRect   描画矩形を格納するバッファへのポインタ。
    //! @param[in]  pStr    文字列
    //! @param[in]  length  文字列長(sizeof(CharType)単位)
    //!
    void                CalculateStringRectImpl(
                            nn::font::Rectangle*    pRect,
                            StreamType              pStr,
                            int                     length);

    //! @brief      文字列を描画します。
    //!
    //! @param[in]  pStr    文字列
    //! @param[in]  length      文字列長(sizeof(CharType)単位)
    //! @param[in]  lineWidthOffsetCount 行ごとの幅とオフセットの数
    //! @param[in]  pLineOffset     行ごとのオフセット
    //! @param[in]  pLineWidth      行ごとの幅
    //!
    float                 PrintImpl(
                            StreamType  pStr,
                            int         length,
                            int         lineWidthOffsetCount,
                            const float*    pLineOffset,
                            const float*    pLineWidth);


    //! @brief      文字列を描画フラグに従って描画するときの1文字目の
    //!             描画開始位置にカーソルを移動します。
    //!
    //! @param[in,out]  pXOrigin  描画基準座標を格納したバッファへのポインタ。
    //!                           補正後の描画基準座標が格納されて返る。
    //! @param[in,out]  pYOrigin  描画基準座標を格納したバッファへのポインタ。
    //!                           補正後の描画基準座標が格納されて返る。
    //! @param[in]      pStr      文字列
    //! @param[in]      length    文字列長(sizeof(CharType)単位)
    //!
    //! @return     文字列の幅を返します。
    //!
    float                 AdjustCursor(
                            float*        pXOrigin,
                            float*        pYOrigin,
                            StreamType  pStr,
                            int         length);

    //! @brief      描画フラグが設定されているか判定します。
    //!
    //! @param[in]  mask    描画フラグのマスク値。
    //! @param[in]  flag    判定する描画フラグ。
    //!
    //! @return     指定された描画フラグが設定されている場合は真を返します。
    //!
    bool                IsDrawFlagSet(
                            uint32_t  mask,
                            uint32_t  flag
                        ) const
    {
        return (m_DrawFlag & mask) == flag;
    }

    static int VSNW16Printf(uint16_t* pBuffer, std::size_t bufferSize, std::size_t strCount, const uint16_t* pFormat, va_list vargs)
    {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#ifdef _MSC_VER
        int result = ::_vsnwprintf_s( reinterpret_cast<wchar_t*>(pBuffer), bufferSize, strCount, reinterpret_cast<const wchar_t*>(pFormat), vargs );
        pBuffer[bufferSize - 1] = 0;
#elif defined(__ghs__)
        NN_STATIC_ASSERT(sizeof(uint16_t) == sizeof(wchar_t));
        int result = std::vswprintf( reinterpret_cast<wchar_t*>(pBuffer), std::min(bufferSize - 1, strCount), reinterpret_cast<const wchar_t*>(pFormat), vargs );
        pBuffer[bufferSize - 1] = 0;
#elif defined(__clang__)
        // TODO: vswprintf 対応
        const int BUFFER_SIZE = 1024;
        char dstBuffer[BUFFER_SIZE];
        char formatBuffer[BUFFER_SIZE];

        nn::util::ConvertStringUtf16NativeToUtf8(formatBuffer, BUFFER_SIZE, pFormat, strCount);

        int result = std::vsnprintf( dstBuffer, strCount, formatBuffer, vargs );

        dstBuffer[bufferSize - 1] = 0;

        nn::util::ConvertStringUtf8ToUtf16Native(pBuffer, bufferSize, dstBuffer, BUFFER_SIZE);
#else
        // TODO : 十分に検証されていないコードです。
        NN_STATIC_ASSERT(sizeof(char32) == sizeof(wchar_t));
        const int BUFFER_SIZE = 1024;

        char32 dstBuffer[BUFFER_SIZE];
        char32 formatBuffer[BUFFER_SIZE];
        ut::wcs16towcs32(formatBuffer, pFormat, BUFFER_SIZE);

        int result = std::vswprintf( reinterpret_cast<wchar_t*>(dstBuffer), strCount, reinterpret_cast<wchar_t*>(formatBuffer), vargs );
        dstBuffer[bufferSize - 1] = 0;

        ut::wcs32towcs16(pBuffer, dstBuffer, bufferSize);
#endif

        return result;

#ifdef _MSC_VER
#pragma warning(pop)
#endif
    }

    float               GetLineOffset(int lineOffsetCount, const float* pLineOffset, int line) const;
    float               GetLineWidth(int lineWidthCount, const float* pLineWidth, int line) const;
    float               MoveOriginAsFirstLineScale(float originY, StreamType pStr, int length);

    /* ------------------------------------------------------------------------
            変数
       ------------------------------------------------------------------------ */

    //! デフォルトタグプロセッサ
    static TagProcessor     g_DefaultTagProcessor;

    //! 拡張タグを解釈するタグプロセッサ
    static ExtendedTagProcessor g_ExtendedTagProcessor;

    float                   m_WidthLimit;           //!< 描画幅制限
    float                   m_CharSpace;            //!< 文字間
    float                   m_LineSpace;            //!< 行間
    float                   m_WidthLimitOffset;     //!< 折り返しを計算するときのオフセット
    int                     m_TabWidth;             //!< タブ幅
    uint32_t                m_DrawFlag;             //!< 描画フラグ
    TagProcessor*           m_pTagProcessor;         //!< タグ処理機
    bool                    m_CenterCeilingEnabled; //!< 中央揃えのにじみ防止機能を有効にするかどうか
    bool                    m_LinefeedKerningEnabled; //!< 改行時にカーニングを引き継ぐかどうか(過去の不具合の互換用)
    bool                    m_LinefeedByCharacterHeightEnabled; //!< 文字の高さに応じて改行幅を自動調整するかどうか
};

}   // namespace font
}   // namespace nn
