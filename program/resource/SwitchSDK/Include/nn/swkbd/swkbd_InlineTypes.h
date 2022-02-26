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

#include "swkbd_Types.h"
#include <nn/account/account_Types.h>

#include <nvn/nvn.h>
#include <cstring>

/**
 * @file
 * @brief   ソフトウェアキーボードをインラインで使用する際の型や定数の宣言
 */

namespace nn { namespace swkbd {

/**
 * @brief   ソフトウェアキーボードの状態です。
 */
enum State : uint32_t
{
    State_None,        //!< ソフトウェアキーボードが起動していない、または終了後の状態です。
    State_Disappear,   //!< ソフトウェアキーボードが起動しているが、非表示の状態です。
    State_InAppear,    //!< ソフトウェアキーボードの表示アニメ再生中の状態です。
    State_Appear,      //!< ソフトウェアキーボードが表示中の状態です。
    State_InDisappear, //!< ソフトウェアキーボードの非表示アニメ再生中の状態です。
    State_SelectKeyTop,//!< 使用する配列を選択中の状態です。
    State_Max          //!< 列挙子の総数です。
};


/**
 * @brief   ソフトウェアキーボードに対して入力が禁止できるキーの種類です。
 *          指定したキーをソフトウェアキーボードが使わなくなります。
 */
enum InvalidButton : uint32_t
{
    InvalidButton_AnalogStickL = 1 << 1, //!< 左アナログスティックの入力を禁止させます。
    InvalidButton_AnalogStickR = 1 << 2, //!< 右アナログスティックの入力を禁止させます。
    InvalidButton_ZL           = 1 << 3, //!< ZL ボタンの入力を禁止させます。
    InvalidButton_ZR           = 1 << 4  //!< ZR ボタンの入力を禁止させます。
};


#if defined (NN_BUILD_CONFIG_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic warning "-Wpadded"
#endif


/**
 * @brief 矩形です。タッチで使用する矩形の取得に使用します。
 */
struct Rect
{
    int16_t left;   //!< X 座標
    int16_t top;    //!< Y 座標
    int16_t width;  //!< 幅
    int16_t height; //!< 高
};



/**
 * @brief ソフトウェアキーボードを表示する際に使用する構造体です。
 */
class AppearArg
{
public:
    AppearArg()
        : keyboardMode( KeyboardMode_ASCII )
        , okText()
        , leftOptionalSymbolKey( 0 )
        , rightOptionalSymbolKey( 0 )
        , isPredictionEnabled( false )
        , invalidCharFlag( 0 )
        , textMaxLength( -1 )
        , textMinLength( -1 )
        , isUseNewLine( false )
        , invalidButtonFlag( 0 )
        , isUseSaveData( true )
        , uid( nn::account::InvalidUid )
    {
        std::memset( _padding1, 0, sizeof(_padding1) );
        std::memset( _padding2, 0, sizeof(_padding2) );
        std::memset( _padding3, 0, sizeof(_padding3) );
    }

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
    bool              isPredictionEnabled;

    uint8_t           _padding1[1];

    //! @brief 入力禁止文字フラグです。@n
    //!        入力禁止したい文字（キー）群を表すビットフラグ（nn::swkbd::InvalidChar の各値を指定）を指定してください。
    uint32_t          invalidCharFlag;
    //! @brief 入力テキストとして有効な最大文字数です。@n
    //!        プレビュー欄の右下に「入力中の文字数/最大文字数」が表示されます。@n
    //!        最大 nn::swkbd::TextMaxLength まで指定可能です。
    int32_t           textMaxLength;
    //! @brief 入力テキストとして有効な最小文字数です。@n
    //!        プレビュー欄の右下に「入力中の文字数/最大文字数」が表示されます。@n
    //!        最大 nn::swkbd::TextMaxLength まで指定可能です。
    int32_t           textMinLength;
    //! @brief 改行を有効にするかどうかを指定できます。@n
    //!        true にすると、改行ボタンが押せるようになります。
    bool              isUseNewLine;

    uint8_t              _padding2[3];

    //! @brief ソフトウェアキーボードに対して入力が禁止できるキーのフラグです。@n
    //!        ソフトウェアキーボードに対して禁止したいキー群を表すビットフラグ(nn::swkbd::InvalidButton の各値)を指定してください。
    uint32_t          invalidButtonFlag;

    // ユーザーアカウントのセーブデータを使用するかどうかを指定できます。@n
    // true にすると「ユーザーアカウントが 1 つだけ Open 状態に設定されている場合」に限り、
    // Open 状態に設定されたユーザーアカウントのセーブデータを使用できるようになります。@n
    // false にすると、ユーザーアカウントの状態にかかわらず、セーブデータを使用しません。@n
    // デフォルトは true です。@n
    // この値は変更しないでください。
    bool              isUseSaveData;
    uint8_t           _padding3[7];

    // セーブデータを使用するユーザーアカウントを指定できます。@n
    // この値は isUseSaveData が true の時のみ使用します。@n
    // 未指定( = nn::account::InvalidUid )の場合は「ユーザーアカウントが 1 つだけ Open 状態に設定されている場合」に限り、
    // Open 状態に設定されたユーザーアカウントのセーブデータを使用します。@n
    // デフォルトは未指定( = nn::account::InvalidUid )です。@n
    // この値は変更しないでください。
    nn::account::Uid  uid;
};


/**
 * @brief 入力テキストとして入力されている文字列を設定する際に使用する構造体です。
 */
class InputText
{
public:
    InputText()
        : text()
        , textLength( 0 )
        , _reserve( 0 )
    {
        std::memset( _padding, 0, sizeof(_padding) );
    }

    //! @brief 入力テキストとして入力されている文字列を設定します。UTF-16版。
    //! @param[in] pStr 設定する文字列(UTF-16)です。
    void Set( const char16_t* pStr );

    //! @brief 入力テキストとして入力されている文字列を設定します。UTF-8版。
    //! @param[in] pStr 設定する文字列(UTF-8)です。
    void SetUtf8( const char* pStr );

    //! @brief 入力テキストとして入力されている文字列です。
    char16_t text[ TextMaxLength + 1 ];

    uint8_t  _padding[2];

    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
    //
    int32_t _reserve;
};

/// @cond DicLangType
/**
 * @brief 変換機能利用時に使用する辞書の言語です。
 *
 * @details ソフトウェアキーボードの種類に応じて、対応する1つの言語の辞書がロードされます。
 *
 * @deprecated この定義は中韓台辞書を含んでいません。 nn::swkbd::DictionaryLang を参照ください。
 */
enum DicLangType : uint32_t
{
    DicLangType_Jp,        //!< JP日本語
    DicLangType_UsEng,     //!< US英語(US)
    DicLangType_UsFra,     //!< USフランス語
    DicLangType_UsSpa,     //!< USスペイン語
    DicLangType_UsPor,     //!< USポルトガル語(ブラジル)
    DicLangType_EuEng,     //!< EU英語(イギリス)
    DicLangType_EuFra,     //!< EUフランス語
    DicLangType_EuDeu,     //!< EUドイツ語
    DicLangType_EuSpa,     //!< EUスペイン語
    DicLangType_EuIta,     //!< EUイタリア語
    DicLangType_EuNld,     //!< EUオランダ語
    DicLangType_EuPor,     //!< EUポルトガル語
    DicLangType_EuRus,     //!< EUロシア語
    DicLangType_Sign,      //!< 記号配列用特殊辞書
    DicLangType_Max
};


/**
 * @brief カスタマイズ辞書を設定する際に使用する構造体です。
 *
 * @deprecated このクラスは中韓台用のカスタマイズ辞書を設定できません。 nn::swkbd::CustomizedDictionarySet を参照ください。
 */
class CustomizeDicInfo
{
public:
    CustomizeDicInfo()
    {
        for( int i = 0; i < DicLangType_Max; ++i )
        {
            offset[ i ] = 0;
            size[ i ] = 0;
        }
    }

    //! @brief 言語毎の辞書オフセット値です。
    int32_t offset[ DicLangType_Max ];
    //! @brief 言語毎の辞書サイズです。@n
    //!        nn::os::MemoryPageSize でアライメントされたものでなければなりません。
    int32_t size[ DicLangType_Max ];
};
/// @endcond

#if defined (NN_BUILD_CONFIG_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif


/**
 * @brief ユーザ辞書を設定する際に使用する構造体です。
 */
class UserWordInfo
{
public:
    UserWordInfo()
        : pWorkBuf( nullptr )
        , pUserWord( nullptr )
        , userWordNum( 0 )
    {
    }

    /**
     * @brief ユーザ辞書作成に要求するメモリサイズを取得します。
     *
     * @param[in] userWordNum   ユーザー登録単語数です。最大 nn::swkbd::UserWordMax まで指定可能です。
     *
     * @return 要求するメモリサイズを返します。
     *         nn::os::MemoryPageSize でアライメントされた値を返します。
     */
    static size_t GetRequiredWorkBufferSize( int32_t userWordNum );

    //! @brief  ユーザ辞書作成時に使用するワークバッファです。@n
    //!         nn::swkbd::UserWordInfo::GetRequiredWorkBufferSize() で取得できるバイトサイズ分確保したバッファを設定してください。@n
    //!         nn::os::MemoryPageSize でアライメントされたものでなければなりません。
    void* pWorkBuf;
    //! @brief ユーザー登録単語の配列です。
    UserWord* pUserWord;
    //! @brief ユーザー登録単語数です。最大 nn::swkbd::UserWordMax まで指定可能です。
    int32_t userWordNum;
};


/**
 * @brief ChangedStringCallback で渡される構造体です。UTF-16版。
 */
struct ChangedStringArg
{
    //! @brief 入力テキストとして入力されている文字列です。
    char16_t text[ TextMaxLength + 1 ];
    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
    //! @brief 未確定領域の開始位置です。
    int32_t dirtyFrom;
    //! @brief 未確定領域の終了位置です。
    int32_t dirtyTo;
    //! @brief 入力テキスト先頭からの文字数であらわされたテキストカーソルの位置です。
    int32_t cursorPos;
};


/**
 * @brief ChangedStringCallback で渡される構造体です。UTF-8版。
 */
struct ChangedStringArgUtf8
{
    //! @brief 入力テキストとして入力されている文字列です。
    char text[ ( TextMaxLength * 4 ) + 1 ];
    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
    //! @brief 未確定領域の開始位置です。
    int32_t dirtyFrom;
    //! @brief 未確定領域の終了位置です。
    int32_t dirtyTo;
    //! @brief 入力テキスト先頭からの文字数であらわされたテキストカーソルの位置です。
    int32_t cursorPos;
};


/**
 * @brief MovedCursorCallback で渡される構造体です。UTF-16版。
 *
 * @details テキストカーソルが移動する場合、以下のような状態になります。
 *          - 未確定領域の文字は全て確定されます
 */
struct MovedCursorArg
{
    //! @brief 入力テキストとして入力されている文字列です。
    char16_t text[ TextMaxLength + 1 ];
    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
    //! @brief 入力テキスト先頭からの文字数であらわされたテキストカーソルの位置です。
    int32_t cursorPos;
};


/**
 * @brief MovedCursorCallback で渡される構造体です。UTF-8版。
 *
 * @details テキストカーソルが移動する場合、以下のような状態になります。
 *          - 未確定領域の文字は全て確定されます
 */
struct MovedCursorArgUtf8
{
    //! @brief 入力テキストとして入力されている文字列です。
    char text[ ( TextMaxLength * 4 ) + 1 ];
    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
    //! @brief 入力テキスト先頭からの文字数であらわされたテキストカーソルの位置です。
    int32_t cursorPos;
};


/**
 * @brief DecidedEnterCallback で渡される構造体です。UTF-16版。
 */
struct DecidedEnterArg
{
    //! @brief 入力テキストとして入力されている文字列です。
    char16_t text[ TextMaxLength + 1 ];
    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
};


/**
* @brief DecidedEnterCallback で渡される構造体です。UTF-8版。
*/
struct DecidedEnterArgUtf8
{
    //! @brief 入力テキストとして入力されている文字列です。
    char text[ ( TextMaxLength * 4 ) + 1 ];
    //! @brief 入力テキストとして入力されている文字列の長さです。
    int32_t textLength;
};


/**
 * @brief 初期化終了情報を取得するコールバック関数の型です。
 */
typedef void( *FinishedInitializeCallback )();


/**
* @brief ソフトウェアキーボード終了情報を取得するコールバック関数の型です。
*/
typedef void( *FinishedKeyboardCallback )();


/**
 * @brief 入力テキストとして入力されている文字列情報を取得するコールバック関数の型です。UTF-16版。
 */
typedef void( *ChangedStringCallback )( const ChangedStringArg* pChangedStringArg );


/**
 * @brief 入力テキストとして入力されている文字列情報を取得するコールバック関数の型です。UTF-8版。
 */
typedef void( *ChangedStringCallbackUtf8 )( const ChangedStringArgUtf8* pChangedStringArgUtf8 );


/**
 * @brief テキストカーソル位置情報を取得するコールバック関数の型です。UTF-16版。
 */
typedef void( *MovedCursorCallback )( const MovedCursorArg* pMovedCursorArg );


/**
 * @brief テキストカーソル位置情報を取得するコールバック関数の型です。UTF-8版。
 */
typedef void( *MovedCursorCallbackUtf8 )( const MovedCursorArgUtf8* pMovedCursorArgUtf8 );


/**
 * @brief 確定閉じ情報を取得するコールバック関数の型です。UTF-16版。
 */
typedef void( *DecidedEnterCallback )( const DecidedEnterArg* pDecidedEnterArg );


/**
 * @brief 確定閉じ情報を取得するコールバック関数の型です。UTF-8版。
 */
typedef void( *DecidedEnterCallbackUtf8 )( const DecidedEnterArgUtf8* pDecidedEnterArgUtf8 );


/**
 * @brief キャンセル閉じ情報を取得するコールバック関数の型です。
 */
typedef void( *DecidedCancelCallback )();

/**
 * @brief ユーザ辞書情報の解放情報を取得するコールバック関数の型です。
 */
typedef void( *ReleasedUserWordInfoCallback )( );


}} // namespace nn::swkbd
