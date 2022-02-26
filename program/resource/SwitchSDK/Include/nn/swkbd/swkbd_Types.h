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
#include <cstring>

/**
 * @file
 * @brief   ソフトウェアキーボードで使用される型や定数の宣言
 */

namespace nn { namespace swkbd {

/**
 * @brief   プレビュー欄での確定文字列の最大文字数です。
 */
const int32_t TextMaxLength = 500;

/**
 * @brief   区切り文字を有効化した際の、確定文字列の最大文字数です。
 */
const int32_t SeparateModeTextMaxLength = 24;

/**
 * @brief   ヘッダーテキスト用文字列の最大文字数です。
 */
const int32_t HeaderTextMaxLength = 64;

/**
 * @brief   サブテキスト用文字列の最大文字数です。
 */
const int32_t SubTextMaxLength = 128;

/**
 * @brief   ガイド文字列の最大文字数です。
 */
const int32_t GuideTextMaxLength = 256;

/**
 * @brief   決定文字列の最大文字数です。
 */
const int32_t OkTextMaxLength = 8;

/**
 * @brief   未確定文字列が強制確定になるまでに入力可能な文字数です。
 */
const int32_t UnfixedStringLengthMax = 24;

/**
 * @brief   ユーザー辞書に登録可能な最大単語数です。
 */
const int32_t UserWordMax = 1000;

/**
 * @brief   ダイアログに表示する最大文字数です。
 */
const int32_t DialogTextMaxLength = 500;

/**
 * @brief   区切り文字の最大挿入数です。
 */
const int32_t SepareteTextPosMax = 8;

/**
 * @brief   指定可能なカスタマイズ辞書の総数です。
 */
const int32_t CustomizedDicionarySetMax = 24;


//--------------------------------------------------------------------------
/**
 *  @brief ユーザー辞書登録単語構造体
 *
 *  @details
 *  - 独自の単語を変換候補に提示したい場合に使用します。@n
 *  - reading に指定した文字列をソフトウェアキーボードで入力すると、word が予測変換結果に出力されます。@n
 *  - 例えば「任天堂」という単語を登録する場合、reading に「にんてんどう」、word に「任天堂」と指定してください。@n
 *  - ユーザーがカタカナを入力したとき、辞書に問い合わせるときにはひらがなに変換されるので、@n
 *    読みの登録はひらがなを用いてください。@n
 *  - 表記と読みは、ともに UTF-16 のみ対応しています。
 */
struct UserWord
{
    //! @brief 読み。文字列の最後に NULL終端 を指定する必要があります。
    char16_t reading[ UnfixedStringLengthMax + 1 ];
    //! @brief 表記。文字列の最後に NULL終端 を指定する必要があります。
    char16_t word[ UnfixedStringLengthMax + 1 ];
};



//--------------------------------------------------------------------------
/**
 * @brief   ソフトウェアキーボードのプリセットの種類です。@n
 *          メールアドレス設定・パスワード設定など、
 *          一般的な入力設定をプリセットとして用意しています。
 */
enum Preset
{
    Preset_Default,      //!< 複数行表示の標準プリセットです。
    Preset_Password,     //!< パスワード入力用のプリセットです。
    Preset_UserName,     //!< ユーザ名入力用のプリセットです。
    Preset_DownloadCode, //!< ダウンロードコード入力用のプリセットです。
    Preset_Max,          //!< 列挙子の総数です。
    Preset_Force32 = 0xFFFFFFFF
};


//--------------------------------------------------------------------------
/**
 * @brief   ソフトウェアキーボードの盤面の種類です。
 *
 * @details
 *  入力する内容に対応したモードを指定することで、適切な配列のキーボードを使用することができます。@n
 * - 言語セット1（欧米日本言語）キーボードモード( LanguageSet1 )@n
 * @n
 * - 数字キーボードモード ( Numeric )@n
 *   - 数字入力に特化したキーボードが利用できます。@n
 * @n
 * - ASCII キーボードモード ( ASCII )@n
 *   - ASCII のみ入力できるキーボードが利用できます。@n
 * @n
 * - 言語セット1ラテン文字入力優先モード  ( LanguageSet1Latin )@n
 *   - 選択できるキーボードの種類は「LanguageSet1」と同じです。@n
 *   - 「最後に起動したキーボード」によらず常に半角英数字用のキーボードが初めに表示されます。@n
 *   - 終了時のキーボードが「最後に起動したキーボード」として記録されることもありません。 @n
 *   - 予測変換の学習は行われます。@n
 * @n
 * - 欧米言語キーボードモード ( Alphabet )@n
 *   - 選択できるキーボードの種類は「LanguageSet1」から日本語を除いたキーボードを使用することができます。@n
 * @n
 * - 簡体字中国語キーボードモード ( SimplifiedChinese )@n
 *   - 简体中文キーボードと記号キーボードが利用できます。@n
 *   - 予測変換機能は必ず有効になります。@n
 * @n
 * - 繁体字中国語キーボードモード ( TraditionalChinese )@n
 *   - 繁體中文キーボードと記号キーボードが利用できます。@n
 *   - 予測変換機能は必ず有効になります。@n
 * @n
 * - 韓国語キーボードモード ( Korean )@n
 *   - 한국어キーボードと記号キーボードが利用できます。@n
 *   - 予測変換機能は必ず有効になります。@n
 * @n
 * - 言語セット2（欧米日本中国韓国言語）キーボードモード ( LanguageSet2 )@n
 *   - 「LanguageSet1」のキーボードに加え、简体中文・繁體中文・한국어キーボードも使用することができます。
 *   - 予測変換機能は必ず有効になります。@n
 * @n
 * - 言語セット2ラテン文字入力優先モード  ( LanguageSet2Latin )@n
 *   - 選択できるキーボードの種類は「LanguageSet2」と同じです。@n
 *   - 「最後に起動したキーボード」によらず常に半角英数字用のキーボードが初めに表示されます。@n
 *   - 終了時のキーボードが「最後に起動したキーボード」として記録されることもありません。 @n
 *   - 予測変換の学習は行われます。@n
 */
enum KeyboardMode
{
    KeyboardMode_LanguageSet1,      //!< 言語セット1（欧米日本言語）キーボードモードです。
    KeyboardMode_Numeric,           //!< 数字キーボードモードです。
    KeyboardMode_ASCII,             //!< ASCII キーボードモードです。
    KeyboardMode_LanguageSet1Latin, //!< 言語セット1ラテン文字入力優先モードです。
    KeyboardMode_Alphabet,          //!< 欧米言語キーボードモードです。
    KeyboardMode_SimplifiedChinese, //!< 簡体字中国語キーボードモードです。
    KeyboardMode_TraditionalChinese,//!< 繁体字中国語キーボードモードです。
    KeyboardMode_Korean,            //!< 韓国語キーボードモードです。
    KeyboardMode_LanguageSet2,      //!< 言語セット2（欧米日本中国韓国言語）キーボードモードです。
    KeyboardMode_LanguageSet2Latin, //!< 言語セット2ラテン文字入力優先モードです。
    KeyboardMode_Max,

    KeyboardMode_Full       = KeyboardMode_LanguageSet1,
    KeyboardMode_FullLatin  = KeyboardMode_LanguageSet1Latin,
    KeyboardMode_Force32    = 0xFFFFFFFF
};


//--------------------------------------------------------------------------
/**
 * @brief   入力禁止文字の種類です。@n
 *          ソフトウェアキーボードから入力できないよう、該当するキーが無効化されます。
 */
enum InvalidChar
{
    InvalidCharFlag_Space                  = 1 << 1,    //!< スペース禁止
    InvalidCharFlag_AtMark                 = 1 << 2,    //!< @記号禁止
    InvalidCharFlag_Percent                = 1 << 3,    //!< パーセント記号禁止
    InvalidCharFlag_Slash                  = 1 << 4,    //!< スラッシュ記号禁止
    InvalidCharFlag_BackSlash              = 1 << 5,    //!< バックスラッシュ記号禁止
    InvalidCharFlag_Numeric                = 1 << 6,    //!< 数字禁止
    InvalidCharFlag_OutsideOfDownloadCode  = 1 << 7,    //!< ダウンロード番号範囲外の文字の禁止
    InvalidCharFlag_OutsideOfMiiNickName   = 1 << 8,    //!< Mii のニックネーム範囲外の文字の禁止
    InvalidCharFlag_Force32 = 0xFFFFFFFF
};


//--------------------------------------------------------------------------
/**
 * @brief   パスワードキーボードモード時の入力文字遮蔽モードです。
 */
enum PasswordMode
{
    PasswordMode_Show, //!< 入力文字を表示します。
    PasswordMode_Hide, //!< 入力文字を表示しません。
    PasswordMode_Max,  //!< 列挙子の総数です。
    PasswordMode_Force32 = 0xFFFFFFFF
};

//--------------------------------------------------------------------------
/**
 * @brief   プレビュー欄の表示の種類です。
 */
enum InputFormMode
{
    InputFormMode_OneLine,   //!< 1行の見た目になります。最大入力文字数が33文字以上になると、強制的に InputFormMode_MultiLine になります。
    InputFormMode_MultiLine, //!< 複数行の見た目になります。
    InputFormMode_Separate,  //!< 1行、かつ区切り文字を含んだ見た目になります。区切り文字の位置は nn::swkbd::KeyboardConfig::separateTextPos で指定できます。最大入力文字数が25文字以上になると、強制的に InputFormMode_MultiLine になります。
    InputFormMode_Max,       //!< 列挙子の総数です。
    InputFormMode_Force32 = 0xFFFFFFFF
};


//--------------------------------------------------------------------------
/**
 * @brief   ソフトウェアキーボード起動時の、プレビュー欄に表示されるテキストカーソルの初期位置の種類です。
 */
enum InitialCursorPos
{
    InitialCursorPos_First, //!< 文字列の先頭にテキストカーソルを合わせます。
    InitialCursorPos_Last,  //!< 文字列の末尾にテキストカーソルを合わせます。
    InitialCursorPos_Max,   //!< 列挙子の総数です。
    InitialCursorPos_Force32 = 0xFFFFFFFF
};

//--------------------------------------------------------------------------
/**
 * @brief   入力結果の確認結果の種類です。
 */
enum TextCheckResult
{
    TextCheckResult_Success,           //!< 入力結果に問題がない状態です。ユーザーに確認を求めずにソフトウェアキーボードを終了する際に指定してください。
    TextCheckResult_ShowFailureDialog, //!< 入力結果に修正が必要なことをユーザーに伝えるダイアログを表示します。ユーザーには戻るボタンが1つだけ設置されたダイアログが表示されます。
    TextCheckResult_ShowConfirmDialog, //!< 入力結果に対してユーザーに確認を求めるダイアログを表示します。ユーザーには決定と戻るボタンが設置されたダイアログが表示されます。
    TextCheckResult_Max,               //!< 列挙子の総数です。
    TextCheckResult_Force32 = 0xFFFFFFFF
};


//--------------------------------------------------------------------------
/**
 * @brief   ソフトウェアキーボードでの入力結果文字列を格納する構造体です。
 */
struct String
{
    //! @brief 入力結果文字列を格納する先頭ポインタです。@n
    //!        nn::swkbd::KeyboardConfig::isUseUtf8 を true にした時は、入力結果文字列は UTF-8 形式で格納されます。@n
    //!        nn::swkbd::KeyboardConfig::isUseUtf8 を false にした時は、入力結果文字列は UTF-16 形式で格納されます。@n
    //!        nn::swkbd::GetRequiredStringBufferSize() で取得できるバイトサイズ分確保したバッファを設定してください。@n
    //!        nn::os::MemoryPageSize でアライメントされたものでなければなりません。
    void* ptr;
    //! @brief 入力結果文字列のバイトサイズです。@n
    //!        nn::swkbd::GetRequiredStringBufferSize() で取得できるバイトサイズを指定してください。
    size_t bufSize;
};


//------------------------------------------------------------------------
/**
 * @brief 入力結果の文字列を確認するコールバック関数の型です。@n
 *        pStr に格納された入力結果文字列を確認し、
 *        必要に応じた nn::swkbd::TextCheckResult を返すようにしてください。@n
 *        ダイアログの表示を行う場合は、pOutDialogTextBuf に対してダイアログに表示する文字列を指定してください。
 *
 * @param[out] pOutDialogTextBuf        ダイアログに表示する文字列用バッファです。@n
 *                                      nn::swkbd::ShowKeyboardArg::textCheckWorkBuf で指定したバッファを使用しています。@n
 *                                      nn::swkbd::KeyboardConfig::isUseUtf8 で設定した文字コードで入力し、
 *                                      末尾にヌル終端をつけてください。@n
 *                                      入力した文字数は pOutDialogTextLengthSize と等しくなるようにしてください。
 * @param[out] pOutDialogTextLengthSize ダイアログに表示する文字数です。@n
 *                                      nn::swkbd::DialogTextMaxLength 以下になるようにしてください。
 * @param[in]  pStr                     入力結果文字列です。@n
 *                                      末尾にはヌル終端が付与されています。@n
 *                                      String::bufSize にはヌル終端分も含めたバッファサイズ（バイト）が格納されています。
 */
typedef TextCheckResult( *TextCheckCallback )( void* pOutDialogTextBuf, size_t* pOutDialogTextLengthSize, String* pStr );


//--------------------------------------------------------------------------
/**
* @brief    変換辞書言語を表します。@n
*           複数の変換方式を持つ言語の場合は、変換方式ごとに用意します。
*/
enum DictionaryLang : uint16_t
{
    DictionaryLang_Japanese,                            //!< 日本語 (ja:ローマ字入力、五十音入力)
    DictionaryLang_AmericanEnglish,                     //!< 米国英語 (en-US)
    DictionaryLang_CanadianFrench,                      //!< カナダフランス語 (fr-CA)
    DictionaryLang_LatinAmericanSpanish,                //!< 中南米スペイン語 (es-419)
    DictionaryLang_Reserved1,                           //!< 未使用 (使用しないでください)
    DictionaryLang_BritishEnglish,                      //!< 英国英語 (en-GB)
    DictionaryLang_French,                              //!< フランス語 (fr)
    DictionaryLang_German,                              //!< ドイツ語 (de)
    DictionaryLang_Spanish,                             //!< スペイン語 (es)
    DictionaryLang_Italian,                             //!< イタリア語 (it)
    DictionaryLang_Dutch,                               //!< オランダ語 (nl)
    DictionaryLang_Portuguese,                          //!< ポルトガル語 (pt)
    DictionaryLang_Russian,                             //!< ロシア語 (ru)
    DictionaryLang_Reserved2,                           //!< 未使用 (使用しないでください)
    DictionaryLang_SimplifiedChinesePinyin,             //!< 簡体字中国語 (zh-Hans:拼音入力)
    DictionaryLang_TraditionalChineseCangjie,           //!< 繁体字中国語 (zh-Hant:倉頡入力)
    DictionaryLang_TraditionalChineseSimplifiedCangjie, //!< 繁体字中国語 (zh-Hant:速成入力)
    DictionaryLang_TraditionalChineseZhuyin,            //!< 繁体字中国語 (zh-Hant:注音入力)
    DictionaryLang_Korean,                              //!< 韓国語 (ko:２ボル式入力)
    DictionaryLang_Max                                  //!< 現在指定可能な変換辞書言語の数（将来増加の可能性があります）
};

/**
* @brief 変換辞書情報を表します。
*/
struct DictionaryInfo
{
    uint32_t        offset;                 //!< 変換辞書のオフセット（CustomizedDictionarySet::pDictionaries の先頭からのオフセット）
    uint16_t        size;                   //!< 変換辞書のサイズ
    DictionaryLang  lang;                   //!< 変換辞書言語
};

/**
* @brief    カスタマイズ辞書セットを表します。
* @details  複数のカスタマイズ辞書を連続して配置したメモリを用意し、それぞれのオフセットやサイズを DictionaryInfo として設定します。
*/
class CustomizedDictionarySet
{
public:
    CustomizedDictionarySet()
    {
        std::memset( dicInfoList, 0, sizeof(dicInfoList) );
        count = 0;
        pDictionaries = nullptr;
        dictionariesSize = 0;
    }

    //! @brief  カスタマイズ辞書群を配置したメモリの先頭アドレスを指定します。
    const void*     pDictionaries;
    //! @brief  pDictionaries に指定したメモリの有効サイズ
    uint32_t        dictionariesSize;
    //! @brief  カスタマイズ辞書群の各辞書の情報です。@n
    //!         カスタマイズ辞書の数だけ先頭要素から設定しておく必要があります。
    DictionaryInfo  dicInfoList[ CustomizedDicionarySetMax ];
    //! @brief  カスタマイズ辞書の数を指定します。
    uint16_t         count;
};

}} // namespace nn::swkbd
