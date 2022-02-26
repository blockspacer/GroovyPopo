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

/**
* @file
* @brief   ソフトウェアキーボードの引数に関する構造体の宣言
*/


#include <nn/swkbd/swkbd_Types.h>


namespace nn { namespace swkbd {


//--------------------------------------------------------------------------
/**
 * @brief   ソフトウェアキーボードの設定引数を担う構造体です。
 */
struct KeyboardConfig
{
    //! @brief ソフトウェアキーボードの盤面の種類です。@n
    //!        nn::swkbd::KeyboardMode を元に、表示したい盤面の種類を指定してください。
    KeyboardMode      keyboardMode;
    //! @brief OKボタンの文字列です。@n
    //!        最大で nn::swkbd::OkTextMaxLength 文字までです。
    char16_t          okText[ OkTextMaxLength + 1 ];
    //! @brief 数字キーボードモードに追加できる左側の文字です。@n
    //!        nn::swkbd::KeyboardMode が KeyboardMode_Numeric の時のみ有効です。@n
    //!        キーを使用しない場合は 0 を指定してください。
    char16_t          leftOptionalSymbolKey;
    //! @brief 数字キーボードモードに追加できる右側の文字です。@n
    //!        nn::swkbd::KeyboardMode が KeyboardMode_Numeric の時のみ有効です。@n
    //!        キーを使用しない場合は 0 を指定してください。
    char16_t          rightOptionalSymbolKey;
    //! @brief 予測変換機能を使用するか否かを指定できます。@n
    //!        使用する場合は true を設定ください。
    //!
    //! @details isPredictionEnabled で指定された値は以下の条件がすべて成立するときに有効となります。
    //!          - KeyboardMode が KeyboardMode_LanguageSet1 または KeyboardMode_LanguageSet1Latin または KeyboardMode_Alphabet
    //!          - invalidCharFlag が設定されていない
    //!          - passwordMode が PasswordMode_Show
    bool              isPredictionEnabled;
    //! @brief 入力禁止文字フラグです。@n
    //!        入力禁止したい文字（キー）群を表すビットフラグ（nn::swkbd::InvalidChar の各値を指定）を指定してください。
    uint32_t          invalidCharFlag;
    //! @brief ソフトウェアキーボード起動時の、プレビュー欄に表示されるテキストカーソルの初期位置です。@n
    //!        テキストカーソルを先頭に合わせるか、末尾に合わせるかを指定できます。
    InitialCursorPos  initialCursorPos;
    //! @brief ヘッダーテキストに表示する文字列です。@n
    //!        何を入力するべきかを簡潔に表示する文字列を指定してください。@n
    //!        プレビュー欄が１行の時のみ、プレビュー欄上部に表示されます。@n
    //!        最大 nn::swkbd::HeaderTextMaxLength 文字が指定可能です。
    char16_t          headerText[ HeaderTextMaxLength + 1 ];
    //! @brief サブテキストに表示する文字列です。@n
    //!        ヘッダーテキストに書かれた内容を補足する文字列を指定してください。@n
    //!        プレビュー欄が１行の時のみ、ヘッダーテキストより下部に表示されます。@n
    //!        最大 nn::swkbd::SubTextMaxLength 文字が指定可能です。
    char16_t          subText[ SubTextMaxLength + 1 ];
    //! @brief ガイド文字列です。
    //!        プレビュー欄に文字が１つも入力されていない時に表示されます。@n
    //!        未入力時に入力の目的や、入力例などを表示する文字列を指定してください。@n
    //!        また、複数行の時にも表示したい説明がある場合はこちらを指定してください。@n
    //!        最大 nn::swkbd::GuideTextMaxLength 文字が指定可能です。
    char16_t          guideText[ GuideTextMaxLength + 1 ];
    //! @brief 入力テキストとして有効な最大文字数です。@n
    //!        プレビュー欄の右下に「入力中の文字数/最大文字数」が表示されます。@n
    //!        最大 nn::swkbd::TextMaxLength まで指定可能です。
    int32_t           textMaxLength;
    //! @brief 入力テキストとして有効な最小文字数です。@n
    //!        プレビュー欄の右下に「入力中の文字数/最大文字数」が表示されます。@n
    //!        最大 nn::swkbd::TextMaxLength まで指定可能です。
    int32_t           textMinLength;
    //! @brief パスワードキーボードモードのときの入力文字隠蔽モードです。@n
    //!        指定していない場合、入力した文字がそのまま表示されます。
    //! @details KeyboardMode が以下の場合、強制的に PasswordMode_Show として扱います。
    //!         - KeyboardMode_SimplifiedChinese
    //!         - KeyboardMode_TraditionalChinese
    //!         - KeyboardMode_Korean
    //!         - KeyboardMode_LanguageSet2
    PasswordMode      passwordMode;
    //! @brief プレビュー欄の表示の種類です。@n
    //!        プレビュー欄を1行の見た目で表示するか、複数行の見た目で表示するかを設定できます。
    InputFormMode     inputFormMode;
    //! @brief 改行を有効にするかどうかを指定できます。@n
    //!        true にすると、改行ボタンが押せるようになります。@n
    //!       InputFormMode が InputFormMode_MultiLine でなければ、改行は有効になりません。
    bool              isUseNewLine;
    //! @brief ソフトウェアキーボード終了時に返る文字列を UTF-8 にするかどうかを指定できます。@n
    //!        true にすると、ソフトウェアキーボード終了時に返る文字列が UTF-8 になります。@n
    //!        false にすると、ソフトウェアキーボード終了時に返る文字列が UTF-16 になります。
    bool              isUseUtf8;
    //! @brief ソフトウェアキーボード開始時に、現在表示中の画面をぼかすかどうかを指定できます。@n
    //!        ソフトウェアキーボードの開始時、起動したアプリ側の画面がキャプチャされ、
    //!        キャプチャ結果の画像がソフトウェアキーボードの背景に描画されます。@n
    //!        true にすると、キャプチャ結果の画像がぼかした状態で描画されます。
    bool              isUseBlurBackground;

    // ワークバッファの先頭から起動時に入力されている文字列までのオフセットです。@n
    // この値を書き換えないでください。
    int32_t           _initialStringOffset;
    // 起動時に入力されている文字列数です。@n
    // この値を書き換えないでください。
    int32_t           _initialStringLength;
    // ワークバッファの先頭からユーザ登録単語までのオフセットです。@n
    // この値を書き換えないでください。
    int32_t           _userDictionaryOffset;
    // ワークバッファの先頭からユーザ登録単語数です。@n
    // この値を書き換えないでください。
    int32_t           _userDictionaryNum;
    // ソフトウェアキーボードの入力確定後に、一度アプリ側で入力結果文字列を確認するかどうかの判定フラグです。@n
    // この値を書き換えないでください。
    bool              _isUseTextCheck;
    // 入力結果の文字列を確認するコールバック関数です。@n
    // この値を書き換えないでください。
    TextCheckCallback _textCheckCallback;

    //! @brief 区切り文字を挿入する位置を指定できます。@n
    //!        指定した位置の後に区切り文字が挿入されます。@n
    //!        すべてに入力文字範囲外の値を指定すると、区切り文字が挿入されなくなります。
    int32_t           separateTextPos[ SepareteTextPosMax ];

    // カスタマイズ辞書セットの辞書情報です。
    // カスタマイズ辞書は nn::swkbd::SetCustomizedDictionaries() を利用して設定してください。
    // この値を書き換えないでください。
    DictionaryInfo    _customizedDicInfoList[ CustomizedDicionarySetMax ];
    // カスタマイズ辞書数です。
    // カスタマイズ辞書は nn::swkbd::SetCustomizedDictionaries() を利用して設定してください。
    // この値を書き換えないでください。
    uint8_t          _customizedDicCount;
    // 予約領域です。
    uint8_t          _reserved[15];
};

//--------------------------------------------------------------------------
/**
 * @brief   ソフトウェアキーボードの起動引数を定義する構造体です。
 */
struct ShowKeyboardArg
{
    //! @brief ソフトウェアキーボードの設定引数です。
    KeyboardConfig  keyboardConfig;

    //! @brief ソフトウェアキーボードに渡すワークバッファです。@n
    //!        nn::swkbd::SetInitialText() で設定した初期入力文字列や、
    //!        アプリ側で指定したユーザー辞書が格納されます。@n
    //!        nn::swkbd::GetRequiredWorkBufferSize() で取得できるバイトサイズ分確保したバッファを設定してください。@n
    //!        nn::os::MemoryPageSize でアライメントされたものでなければなりません。
    void* workBuf;
    //! @brief ソフトウェアキーボードに渡すワークバッファのバイトサイズです。@n
    //!        nn::swkbd::GetRequiredWorkBufferSize() で取得できるバイトサイズを指定してください。
    size_t workBufSize;

    //! @brief ソフトウェアキーボードの入力確定後に、アプリ側で入力結果文字列を確認する際に用いるワークバッファです。@n
    //!        nn::swkbd::GetRequiredTextCheckWorkBufferSize() で取得できるバイトサイズ分確保したバッファを設定してください。@n
    //!        アプリ側で確認する必要のない場合、設定する必要はありません。
    void* textCheckWorkBuf;
    //! @brief ソフトウェアキーボードの入力確定後に、アプリ側で入力結果文字列を確認する際に用いるワークバッファのバイトサイズです。@n
    //!        nn::swkbd::GetRequiredTextCheckWorkBufferSize() で取得できるバイトサイズを指定してください。
    size_t textCheckWorkBufSize;

    // カスタマイズ辞書群を配置したメモリの先頭アドレスを指定します。
    // 指定したメモリは nn::swkbd::ShowKeyboard() 実行中は解放しないでください。
    // カスタマイズ辞書は nn::swkbd::SetCustomizedDictionaries() を利用して設定してください。
    const void* _customizedDicBuf;
    // customizedDicBuf に指定したメモリの有効サイズします。
    // カスタマイズ辞書は nn::swkbd::SetCustomizedDictionaries() を利用して設定してください。
    size_t _customizedDicBufSize;
};


} } // namespace nn::swkbd
