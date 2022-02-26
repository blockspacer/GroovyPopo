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

#include <cwchar>
#include <nn/font/font_Types.h>
#include <nn/font/font_Util.h>
#include <nn/font/font_TextWriterBase.h>

namespace nn {
namespace font {

//!
//! @brief ワードラッピングの設定です。
//!
//! @sa WordWrapping::CalculateWordWrapping
//!
struct WordWrapConfig
{
    //! @name 設定
    //@{

    //!
    //! @brief 初期値に設定します。
    //!
    void SetDefault()
    {
        m_LineCountMax = 0;
        m_IsIgnoreFirstSpOver2ndLine = true;
        m_ProhibitBeginningPercent = false;
        m_ProhibitEndingOneCyrillicCharacter = false;
    }

    //! @brief 最大行数を設定します。
    //!
    //! @param[in] value  最大行数を指定します(初期値 = 0)。
    //!
    //! @return WordWrapConfig への参照を返します。
    //!
    //! @details
    //! 最大行数を指定した場合、 WordWrapping::CalculateWordWrapping() で
    //! 行数が上限を超えた場合に処理を中断します。
    //!
    //! value に0を指定した場合は無制限を表します。
    //!
    //! @sa GetLineCountMax
    //!
    WordWrapConfig& SetLineCountMax(int value)
    {
        m_LineCountMax = value;
        return *this;
    }

    //! @brief 最大行数の設定を取得します。
    //!
    //! @return
    //! 設定値を返します。
    //!
    //! @sa SetLineCountMax
    //!
    int GetLineCountMax() const
    {
        return m_LineCountMax;
    }

    //! @brief 2行目以降の行頭の空白を無視します。
    //!
    //! @param[in] value  機能を有効にする場合は true を指定します(初期値 = true)。
    //!
    //! @return WordWrapConfig への参照を返します。
    //!
    //! @sa GetIgnoreFirstSpOver2ndLine
    //!
    WordWrapConfig& SetIgnoreFirstSpOver2ndLine(bool value)
    {
        m_IsIgnoreFirstSpOver2ndLine = value;
        return *this;
    }

    //! @brief 2行目以降の行頭の空白を無視するかの設定を取得します。
    //!
    //! @return
    //! 設定値を返します。
    //!
    //! @sa SetIgnoreFirstSpOver2ndLine
    //!
    bool GetIgnoreFirstSpOver2ndLine() const
    {
        return m_IsIgnoreFirstSpOver2ndLine;
    }

    //! @brief 数値に続く「%」が、行頭に折り返されてしまうケースを禁止するかどうかを設定します。
    //!
    //! @details
    //! 互換性のためにデフォルト値を false にしていますが、正しい仕様に沿った動作にするには true にしてください。
    //!
    //! @param[in] value 数値に続く行頭の「%」を禁止する場合は true を設定します(初期値 = false)。
    //!
    //! @return WordWrapConfig への参照を返します。
    //!
    WordWrapConfig& SetProhibitBeginningPercent(bool value)
    {
        m_ProhibitBeginningPercent = value;
        return *this;
    }

    //! @brief 行頭の「%」を禁止するかどうかを取得します。
    //!
    //! @return 設定値を返します。
    //!
    bool GetProhibitBeginningPercent() const
    {
        return m_ProhibitBeginningPercent;
    }

    //! @brief 行末のキリル文字の 1 文字単語を禁止するかどうかを設定します。
    //!
    //! @param[in] value 行末のキリル文字の 1 文字単語を禁止する場合は true を設定します(初期値 = false)。
    //!
    //! @return WordWrapConfig への参照を返します。
    //!
    WordWrapConfig& SetProhibitEndingOneCyrillicCharacter(bool value)
    {
        m_ProhibitEndingOneCyrillicCharacter = value;
        return *this;
    }

    //! @brief 行末のキリル文字の 1 文字単語を禁止するかどうかを取得します。
    //!
    //! @return 設定値を返します。
    //!
    bool GetProhibitEndingOneCyrillicCharacter() const
    {
        return m_ProhibitEndingOneCyrillicCharacter;
    }

    //@}

    //! @cond PRIVATE

    int32_t m_LineCountMax;
    bool m_IsIgnoreFirstSpOver2ndLine;
    bool m_ProhibitBeginningPercent;
    bool m_ProhibitEndingOneCyrillicCharacter;
    //! @endcond
};

//!
//! @brief ワードラッピングの処理中に呼び出されるコールバックです。
//!
//! @sa WordWrapping::CalculateWordWrapping
//!
template <typename CharType>
class WordWrapCallbackBase
{
public:
    //!
    //! @brief 与えられた文字列のうち一行の表示に収まる範囲を求めます。
    //!
    //! @details
    //! 引数 (pBegin, pEnd) が示す文字列の、一行で表示できる範囲の終端を返します。
    //! 戻り値が指す文字は一行の表示には含まれません。
    //!
    //! 本関数はワードラッピング関数( WordWrapping::CalculateWordWrapping(),
    //! WordWrapping::FindLineBreak() ) から呼び出されます。
    //! 本関数により一行に表示できる範囲の上限が決定され、その範囲内で、
    //! ワードラッピング関数により適切な改行位置が決定されます。
    //!
    //! (pBegin, pEnd) はユーザがワードラッピング関数に与えた文字列内を指しています。
    //! 文字列のヌル終端は保証されません。
    //!
    //! 一行に表示できる文字列の範囲を取得するには WideTextWriter::FindPosOfWidthLimit() が使用できます。
    //!
    //! @param[in] pBegin  現在処理中の行の先頭の文字を指します。
    //! @param[in] pEnd  ユーザがワードラッピング関数に与えた文字列の終端です。
    //!
    //! @return
    //! 一行に収まらない最初の文字の位置を返します。
    //!
    //! @sa WideTextWriter::FindPosOfWidthLimit
    //! @sa WordWrapping::CalculateWordWrapping
    //! @sa WordWrapping::FindLineBreak
    //!
    virtual const CharType* GetLineBreakLimit(const CharType* pBegin, const CharType* pEnd) = 0;

    //!
    //! @brief タグのバイト数を求めます。
    //!
    //! @details
    //! タグはワードラップ処理の対象外にすべきため、タグが何バイトで構成されているかをこの関数が返すことで
    //! タグのワードラップ処理を指定サイズ分スキップします。
    //! 本関数は 0x20 未満の文字コードが現れるたびに呼び出されます。
    //!
    //! @param[out] pNonBreakingLength 指定した文字数分、タグに続く文字列を改行禁止にします。単位はバイト数ではなく文字数です。
    //! @param[in] pStr タグの先頭へのポインタです。
    //! @param[in] pEnd ユーザがワードラッピング関数に与えた文字列の終端です。
    //!
    //! @return スキップするバイト数です。タグでなければ 0 を返します。
    //!
    virtual size_t GetTagSize(size_t* pNonBreakingLength, const CharType* pStr, const CharType* pEnd)
    {
        NN_UNUSED(pStr);
        NN_UNUSED(pEnd);
        *pNonBreakingLength = 0;
        return 0;
    }
};

typedef WordWrapCallbackBase<uint16_t> WordWrapCallback;
typedef WordWrapCallbackBase<char> WordWrapCallbackUtf8;

//!
//! @brief ワードラッピングの処理中に呼び出されるコールバックのデフォルト実装です。
//!
//! @details
//! 本クラスは一般的なワードラッピング処理を行います。
//! 改行方法を細かく調整したい場合は nn::font::WordWrapCallbackBase を継承して
//! コールバッククラスを作成してください。
//!
//! @sa WordWrapping::CalculateWordWrapping
//!
template <typename CharType>
class DefaultWordWrapCallbackBase: public WordWrapCallbackBase<CharType>
{
public:
    //!
    //! @brief コンストラクタです。
    //!
    //! @param[in] pTextWriter TextWriter のポインタです。
    NN_IMPLICIT DefaultWordWrapCallbackBase(const TextWriterBase<CharType>* pTextWriter)
        : m_TextWriterLineBeginningState(*pTextWriter)
        , m_pCurrentLine(NULL)
    {
    }

    //!
    //! @brief デストラクタです。
    //!
    virtual ~DefaultWordWrapCallbackBase()
    {
    }

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pBegin  現在処理中の行の先頭の文字を指します。
    //! @param[in] pEnd  ユーザがワードラッピング関数に与えた文字列の終端です。
    //!
    //! @return
    //! 一行に収まらない最初の文字の位置を返します。
    //!
    const CharType* GetLineBreakLimit(const CharType* pBegin, const CharType* pEnd);

private:
    TextWriterBase<CharType> m_TextWriterLineBeginningState; // 行頭での情報を保存する TextWriter
    const CharType* m_pCurrentLine;
};

typedef DefaultWordWrapCallbackBase<uint16_t> DefaultWordWrapCallback;
typedef DefaultWordWrapCallbackBase<char> DefaultWordWrapCallbackUtf8;

//!
//! @brief ワードラッピングを行うクラスです。
//!
//! @li WordWrapping::CalculateWordWrapping() @n
//!     文字列の適切な位置に改行コードを挿入します。
//!
class WordWrapping
{
public:
    //!
    //! @brief 改行して良い場所を検索します。UTF-16版。
    //!
    //! @param[in] pBegin  行の先頭文字を指します。
    //! @param[in] pEnd  文字列の終端を指します。
    //! @param[in] func  一行の表示範囲を得るコールバックです。
    //! @param[in] config  ワードラップの動作を指定します。
    //!
    //! @return 改行直前の文字のアドレスを返します。
    //!
    //! @details
    //! 単体の機能として利用したい場合の関数です。
    //!
    //! func が示す一行の範囲のなかで改行して良い場所を探します。
    //!
    //! 改行して良い場所が無かった場合には func が示す範囲の最後の文字を行末とします。
    //!
    static const uint16_t*
    FindLineBreak(
        const uint16_t* pBegin,
        const uint16_t* pEnd,
        WordWrapCallback& func,
        const WordWrapConfig& config);

    //!
    //! @brief 改行して良い場所を検索します。UTF-8版。
    //!
    //! @param[in] pBegin  行の先頭文字を指します。
    //! @param[in] pEnd  文字列の終端を指します。
    //! @param[in] func  一行の表示範囲を得るコールバックです。
    //! @param[in] config  ワードラップの動作を指定します。
    //!
    //! @return 改行直前の文字のアドレスを返します。
    //!
    //! @details
    //! 単体の機能として利用したい場合の関数です。
    //!
    //! func が示す一行の範囲のなかで改行して良い場所を探します。
    //!
    //! 改行して良い場所が無かった場合には func が示す範囲の最後の文字を行末とします。
    //!
    static const char*
    FindLineBreakUtf8(
        const char* pBegin,
        const char* pEnd,
        WordWrapCallbackUtf8& func,
        const WordWrapConfig& config);

    //!
    //! @brief ワードラッピングを行います。UTF-16版。
    //!
    //! @param[out] pWriteSize  pOutTextBuffer に格納された文字列の長さを返します。
    //! @param[out] pOutTextBuffer  適切な位置に改行コードの追加された文字列が格納されます。
    //! @param[in] outTextBufferSize  pOutTextBuffer に格納できる文字数を指定します。終端文字を含みます。
    //! @param[in] pInputText  処理の対象となる文字列です（要ヌル終端）。
    //! @param[in] func  一行の表示範囲を得るコールバックです。
    //! @param[in] config  ワードラップの動作を指定します。
    //!
    //! @return
    //! 処理に成功した場合は true を返します。処理を中断した場合は false を返します。
    //!
    //! @details
    //! 文字列に改行が含まれる場合、改行コードはLFである必要があります。CRやCRLFは扱えません。
    //!
    //! 改行して良い場所が無かった場合には func が示す位置で強制的に改行します。
    //!
    //! 以下の場合は処理を中断します。その際でも pOutTextuBuffer はヌル終端されます。
    //! @li pOutTextuBuffer のサイズが足りない場合。
    //! @li config で指定された行数の上限を超えた場合。
    //!
    //! pWriteSize にアドレスを指定すると、pOutTextBuffer に格納された文字列の長さが書き込まれます。
    //!
    static bool
    CalculateWordWrapping(
        uint32_t * pWriteSize,
        uint16_t* pOutTextBuffer,
        uint32_t  outTextBufferSize,
        const uint16_t* pInputText,
        WordWrapCallback& func,
        const WordWrapConfig& config = WordWrapConfig())
    {
        return CalculateWordWrapping(
            pWriteSize,
            pOutTextBuffer,
            outTextBufferSize,
            pInputText,
            static_cast<uint32_t>(nn::font::CalculateWideCharString16Length(pInputText)),            func,
            config);
    }

    //!
    //! @brief ワードラッピングを行います。UTF-8版。
    //!
    //! @param[out] pWriteSize  pOutTextBuffer に格納されたバイト数を返します。
    //! @param[out] pOutTextBuffer  適切な位置に改行コードの追加された文字列が格納されます。
    //! @param[in] outTextBufferSize  pOutTextBuffer に格納できるバイト数を指定します。終端文字を含みます。
    //! @param[in] pInputText  処理の対象となる文字列です（要ヌル終端）。
    //! @param[in] func  一行の表示範囲を得るコールバックです。
    //! @param[in] config  ワードラップの動作を指定します。
    //!
    //! @return
    //! 処理に成功した場合は true を返します。処理を中断した場合は false を返します。
    //!
    //! @details
    //! 文字列に改行が含まれる場合、改行コードはLFである必要があります。CRやCRLFは扱えません。
    //!
    //! 改行して良い場所が無かった場合には func が示す位置で強制的に改行します。
    //!
    //! 以下の場合は処理を中断します。その際でも pOutTextBuffer はヌル終端されます。
    //! @li pOutTextBuffer のサイズが足りない場合。
    //! @li config で指定された行数の上限を超えた場合。
    //!
    //! pWriteSize にアドレスを指定すると、pOutTextBuffer に格納された文字列の長さが書き込まれます。
    //!
    static bool
    CalculateWordWrappingUtf8(
        uint32_t * pWriteSize,
        char* pOutTextBuffer,
        uint32_t  outTextBufferSize,
        const char* pInputText,
        WordWrapCallbackUtf8& func,
        const WordWrapConfig& config = WordWrapConfig())
    {
        return CalculateWordWrappingUtf8(
            pWriteSize,
            pOutTextBuffer,
            outTextBufferSize,
            pInputText,
            static_cast<uint32_t>(strlen(pInputText)),
            func,
            config);
    }

    //!
    //! @brief ワードラッピングを行います。UTF-16版。
    //!
    //! @param[out] pWriteSize  pOutTextBuffer に格納された文字列の長さを返します。
    //! @param[out] pOutTextBuffer  適切な位置に改行コードの追加された文字列が格納されます。
    //! @param[in] outTextBufferSize  pOutTextBuffer に格納できる文字数を指定します。終端文字を含みます。
    //! @param[in] pInputText  処理の対象となる文字列です。
    //! @param[in] inSize  pInputText の文字列長です。
    //! @param[in] func  一行の表示範囲を得るコールバックです。
    //! @param[in] config  ワードラップの動作を指定します。
    //!
    //! @return
    //! 処理に成功した場合は true を返します。処理を中断した場合は false を返します。
    //!
    //! @details
    //! コールバック func が示す１行の表示範囲で適切な位置に改行コードを挿入した文字列を生成します。
    //!
    //! 文字列に改行が含まれる場合、改行コードはLFである必要があります。CRやCRLFは扱えません。
    //!
    //! 改行して良い場所が無かった場合には func が示す位置で強制的に改行します。
    //!
    //! 以下の場合は処理を中断します。その際でも pOutTextBuffer はヌル終端されます。
    //! @li pOutTextBuffer のサイズが足りない場合。
    //! @li config で指定された行数の上限を超えた場合。
    //!
    //! pWriteSize にアドレスを指定すると、pOutTextBuffer に格納された文字列の長さが書き込まれます。
    //!
    static bool
    CalculateWordWrapping(
        uint32_t * pWriteSize,
        uint16_t* pOutTextBuffer,
        uint32_t  outTextBufferSize,
        const uint16_t* pInputText,
        uint32_t  inSize,
        WordWrapCallback& func,
        const WordWrapConfig& config = WordWrapConfig());

    //!
    //! @brief ワードラッピングを行います。UTF-8版。
    //!
    //! @param[out] pWriteSize  pOutTextBuffer に格納されたバイト数を返します。
    //! @param[out] pOutTextBuffer  適切な位置に改行コードの追加された文字列が格納されます。
    //! @param[in] outTextBufferSize  pOutTextBuffer に格納できるバイト数を指定します。終端文字を含みます。
    //! @param[in] pInputText  処理の対象となる文字列です。
    //! @param[in] inSize  pInputText のバイト数です。
    //! @param[in] func  一行の表示範囲を得るコールバックです。
    //! @param[in] config  ワードラップの動作を指定します。
    //!
    //! @return
    //! 処理に成功した場合は true を返します。処理を中断した場合は false を返します。
    //!
    //! @details
    //! コールバック func が示す１行の表示範囲で適切な位置に改行コードを挿入した文字列を生成します。
    //!
    //! 文字列に改行が含まれる場合、改行コードはLFである必要があります。CRやCRLFは扱えません。
    //!
    //! 改行して良い場所が無かった場合には func が示す位置で強制的に改行します。
    //!
    //! 以下の場合は処理を中断します。その際でも pOutTextBuffer はヌル終端されます。
    //! @li pOutTextBuffer のサイズが足りない場合。
    //! @li config で指定された行数の上限を超えた場合。
    //!
    //! pWriteSize にアドレスを指定すると、pOutTextBuffer に格納された文字列の長さが書き込まれます。
    //!
    static bool
    CalculateWordWrappingUtf8(
        uint32_t * pWriteSize,
        char* pOutTextBuffer,
        uint32_t  outTextBufferSize,
        const char* pInputText,
        uint32_t  inSize,
        WordWrapCallbackUtf8& func,
        const WordWrapConfig& config = WordWrapConfig());
};

}   // namespace font
}   // namespace nn
