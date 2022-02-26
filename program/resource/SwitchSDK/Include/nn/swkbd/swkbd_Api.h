/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   ソフトウェアキーボードに関する API の宣言
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/swkbd/swkbd_AppearArg.h>

/**
 * @namespace nn::swkbd
 * @brief ソフトウェアキーボードアプレット呼び出しの名前空間です。
 */

namespace nn { namespace swkbd {

//! @name ソフトウェアキーボードの表示
//! @{

//------------------------------------------------------------------------
/**
 * @brief ソフトウェアキーボードを表示します。
 *
 * @param[out] pOutResultString   入力結果文字列です。@nキャンセル時はバイトサイズが 0 で返ります。
 * @param[in]  showKeyboardArg 表示時に必要な設定パラメータです。
 *
 * @return
 *  ソフトウェアキーボード表示結果を返します。
 *  特に問題なければ ResultSuccess を返します。
 *
 * @details
 *  ソフトウェアキーボードが完了するまで、この関数の呼び出し先はブロックされます。@n
 *  また、エラービューアなどシステムによる UI が表示されている間に本関数を呼び出した場合も、起動中の UI が終了するまで本関数は内部で待機し、 @n
 *  先に起動中の UI が終了すると待機が解除されます。 @n
 *  @n
 *  showKeyboardArg の ShowKeyboardArg::workBuf, ShowKeyboardArg::workBufSize など事前に設定済みである必要があります。 @n
 */
nn::Result ShowKeyboard( String* pOutResultString, const ShowKeyboardArg& showKeyboardArg ) NN_NOEXCEPT;

//! @}


//! @name 表示用パラメータの設定
//! @{


//--------------------------------------------------------------------------
/**
 * @brief ソフトウェアキーボード起動用に、内部的に持っている設定値を初期化します。
 *
 * @details
 *  nn::swkbd::SetUserWordList() 関数など、関数経由でパラメータを設定する際、
 *  内部的に保持しているソフトウェアキーボード起動用の設定値を書き換えています。@n
 *  そのため、ソフトウェアキーボード表示用のパラメータを書き換える際は、
 *  まず初めに nn::swkbd::InitializeKeyboardConfig() 関数を呼び出してください。
 */
void InitializeKeyboardConfig( KeyboardConfig* pOutKeyboardConfig ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief 指定したプリセットに合わせてソフトウェアキーボード表示用パラメータを設定します。
 *
 * @param[out]    pOutKeyboardConfig ソフトウェアキーボード表示用パラメータです。
 * @param[in]     preset             ソフトウェアキーボード表示用パラメータを設定するために用いるプリセットです。
 *
 * @details
 *  ソフトウェアキーボードにおいて良く使われそうな設定を、プリセットとしてまとめています。@n
 *  指定したプリセットに合わせて、ソフトウェアキーボード表示用のパラメータを設定します。
 */
void MakePreset( KeyboardConfig* pOutKeyboardConfig, Preset preset ) NN_NOEXCEPT;


//! @}


//--------------------------------------------------------------------------
/**
 * @brief ソフトウェアキーボードが使用するワークバッファが要求するメモリサイズを取得します。@n
 *        useDictionary に true を指定した場合、ユーザー辞書は 500 単語分として計算します。
 *
 * @param[in] useDictionary ユーザー辞書を使う場合は true にしてください。
 *
 * @return
 *  要求するメモリサイズを返します。
 *  nn::os::MemoryPageSize でアライメントされた値を返します。
 */
size_t GetRequiredWorkBufferSize( bool useDictionary ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief ソフトウェアキーボードが使用するワークバッファが要求するメモリサイズを取得します。@n
 *        ユーザー辞書を使う場合は userWordNum 分のメモリを追加で要求します。
 *
 * @param[in] userWordNum   ユーザー登録単語数です。
 *
 * @return
 *  要求するメモリサイズを返します。
 *  nn::os::MemoryPageSize でアライメントされた値を返します。
 */
size_t GetRequiredWorkBufferSize( const int userWordNum ) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief ソフトウェアキーボードでの入力結果文字列が要求するメモリサイズを取得します。
 *
 * @return
 *  要求するメモリサイズを返します。
 *  nn::os::MemoryPageSize でアライメントされた値を返します。
 */
size_t GetRequiredStringBufferSize() NN_NOEXCEPT;


//! @name 数字キーボード用追加文字の設定
//! @{

//--------------------------------------------------------------------------
/**
 * @brief 指定した文字を数字キーボードモードに追加できる左側の文字として設定します。UTF-16版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     code               設定する文字(UTF-16)です。
 */
void SetLeftOptionalSymbolKey( KeyboardConfig* pKeyboardConfig, const char16_t code ) NN_NOEXCEPT;

/**
 * @brief 指定した文字を数字キーボードモードに追加できる左側の文字として設定します。UTF-8版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-8)です。null 終端されている必要があります。
 */
void SetLeftOptionalSymbolKeyUtf8( KeyboardConfig* pKeyboardConfig, const char* pStr ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief 指定した文字を数字キーボードモードに追加できる右側の文字として設定します。UTF-16版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     code               設定する文字(UTF-16)です。
 */
void SetRightOptionalSymbolKey( KeyboardConfig* pKeyboardConfig, const char16_t code ) NN_NOEXCEPT;

/**
 * @brief 指定した文字を数字キーボードモードに追加できる右側の文字として設定します。UTF-8版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-8)です。null 終端されている必要があります。
 */
void SetRightOptionalSymbolKeyUtf8( KeyboardConfig* pKeyboardConfig, const char* pStr ) NN_NOEXCEPT;

//! @}


//! @name UI 表示用文字列の設定
//! @{

//--------------------------------------------------------------------------
/**
* @brief 指定した文字列をOKボタンの文字列として設定します。UTF-16版。
*
* @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
* @param[in]     pStr               設定する文字列(UTF-16)です。
*
* @details
*  指定可能な文字数は最大 nn::swkbd::OkTextMaxLength 文字です。
*/
void SetOkText( KeyboardConfig* pKeyboardConfig, const char16_t* pStr ) NN_NOEXCEPT;

/**
* @brief 指定した文字列をOKボタンの文字列として設定します。UTF-8版。
*
* @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
* @param[in]     pStr               設定する文字列(UTF-8)です。null 終端されている必要があります。
*
* @details
*  指定可能な文字数は最大 nn::swkbd::OkTextMaxLength 文字です。
*/
void SetOkTextUtf8( KeyboardConfig* pKeyboardConfig, const char* pStr ) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief 指定した文字列をヘッダーテキスト用文字列として設定します。UTF-16版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-16)です。
 *
 * @details
 *  指定可能な文字数は最大 nn::swkbd::HeaderTextMaxLength 文字です。
 */
void SetHeaderText( KeyboardConfig* pKeyboardConfig, const char16_t* pStr ) NN_NOEXCEPT;

/**
 * @brief 指定した文字列をヘッダーテキスト用文字列として設定します。UTF-8版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-8)です。null 終端されている必要があります。
 *
 * @details
 *  指定可能な文字数は最大 nn::swkbd::HeaderTextMaxLength 文字です。
 */
void SetHeaderTextUtf8( KeyboardConfig* pKeyboardConfig, const char* pStr ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief 指定した文字列をサブテキスト用文字列として設定します。UTF-16版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-16)です。
 *
 * @details
 *  指定可能な文字数は最大 nn::swkbd::SubTextMaxLength 文字です。
 */
void SetSubText( KeyboardConfig* pKeyboardConfig, const char16_t* pStr ) NN_NOEXCEPT;

/**
 * @brief 指定した文字列をサブテキスト用文字列として設定します。UTF-8版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-8)です。null 終端されている必要があります。
 *
 * @details
 *  指定可能な文字数は最大 nn::swkbd::SubTextMaxLength 文字です。
 */
void SetSubTextUtf8( KeyboardConfig* pKeyboardConfig, const char* pStr ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief 指定した文字列をガイド文字列として設定します。UTF-16版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-16)です。
 *
 * @details
 *  指定可能な文字数は最大 nn::swkbd::GuideTextMaxLength 文字です。
 */
void SetGuideText( KeyboardConfig* pKeyboardConfig, const char16_t* pStr ) NN_NOEXCEPT;

/**
 * @brief 指定した文字列をガイド文字列として設定します。UTF-8版。
 *
 * @param[in,out] pKeyboardConfig    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr               設定する文字列(UTF-8)です。null 終端されている必要があります。
 *
 * @details
 *  指定可能な文字数は最大 nn::swkbd::GuideTextMaxLength 文字です。
 */
void SetGuideTextUtf8( KeyboardConfig* pKeyboardConfig, const char* pStr ) NN_NOEXCEPT;


//! @}


//! @name 起動時に入力されている文字列の設定
//! @{

//--------------------------------------------------------------------------
/**
 * @brief プレビュー欄に起動時に入力されている文字列を設定します。UTF-16版。
 *
 * @param[in,out] pShowKeyboardArg    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr                設定する文字列(UTF-16)です。
 *
 * @details
 *  pShowKeyboardArg の ShowKeyboardArg::workBuf, ShowKeyboardArg::workBufSize は事前に設定済みである必要があります。
 */
void SetInitialText( ShowKeyboardArg* pShowKeyboardArg, const char16_t* pStr ) NN_NOEXCEPT;

/**
 * @brief プレビュー欄に起動時に入力されている文字列を設定します。UTF-8版。
 *
 * @param[in,out] pShowKeyboardArg    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pStr                設定する文字列(UTF-16)です。null 終端されている必要があります。
 *
 * @details
 *  pShowKeyboardArg の ShowKeyboardArg::workBuf, ShowKeyboardArg::workBufSize は事前に設定済みである必要があります。
 */
void SetInitialTextUtf8( ShowKeyboardArg* pShowKeyboardArg, const char* pStr ) NN_NOEXCEPT;


//! @}


//! @name ユーザー辞書の設定
//! @{

//--------------------------------------------------------------------------
/**
 * @brief ユーザー辞書に登録する単語を設定します。
 *
 * @param[in,out] pShowKeyboardArg    ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pUserWord           ユーザー登録単語の配列です。
 * @param[in]     userWordNum         ユーザー登録単語数です。最大 nn::swkbd::UserWordMax まで指定可能です。
 * @details
 *  ユーザー辞書に登録された単語は登録した順序と逆順に予測候補として表示されます。 @n
 *  pShowKeyboardArg の ShowKeyboardArg::workBuf, ShowKeyboardArg::workBufSize は事前に設定済みである必要があります。
 */
void SetUserWordList( ShowKeyboardArg* pShowKeyboardArg, const UserWord* pUserWord, const int32_t userWordNum ) NN_NOEXCEPT;

//! @}


//! @name 入力確定後の確認・エラーダイアログの表示
//! @{

//--------------------------------------------------------------------------
/**
* @brief ソフトウェアキーボードの入力確定後にアプリ側で確認する際に必要なメモリサイズを取得します。
*
* @return
*  要求するメモリサイズを返します。
*/
size_t GetRequiredTextCheckWorkBufferSize() NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
 * @brief ソフトウェアキーボードの入力確定後に、アプリ側で入力結果の文字列を確認するコールバック関数を設定します。
 *
 * @param[in,out] pShowKeyboardArg ソフトウェアキーボード表示用パラメータです。
 * @param[in]     pCallback           入力結果の文字列を確認するコールバック関数です。
 */
void SetTextCheckCallback( ShowKeyboardArg* pShowKeyboardArg, TextCheckCallback pCallback ) NN_NOEXCEPT;

//! @}

//! @name カスタマイズ辞書の設定
//! @{

//--------------------------------------------------------------------------
/**
 * @brief           カスタマイズ辞書セットをソフトウェアキーボードに渡します。
 *
 * @param[in,out]   pShowKeyboardArg ソフトウェアキーボード表示用パラメータです。
 * @param[in]       dicSet カスタマイズ辞書セット
 * @details         CustomizedDictionarySet::pDictionaries に指定したメモリは、ShowKeyboard() が終了するまで確保したままにしておいてください。
 */
void SetCustomizedDictionaries( ShowKeyboardArg* pShowKeyboardArg, const CustomizedDictionarySet& dicSet) NN_NOEXCEPT;

//! @}

}} // namespace nn::swkbd

