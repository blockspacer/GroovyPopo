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
 * @brief   キー入力の文字コード変換に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/hid/hid_Keyboard.h>
#include <nn/util/util_BitFlagSet.h>
#include <nn/util/util_TypedStorage.h>

namespace nn { namespace kpr {

/**
 * @brief   キー入力の文字コードの属性定義です。
 */
struct KeyCodeAttribute
{
    typedef ::nn::util::BitFlagSet<32, KeyCodeAttribute>::Flag<0>
            IsPrintable;        //!< 印字可能か否か
    typedef ::nn::util::BitFlagSet<32, KeyCodeAttribute>::Flag<1>
            IsAsciiConvertible; //!< 対応する ASCII コードが存在するか否か
};

/**
 * @brief   キー入力の文字コードの属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, KeyCodeAttribute> KeyCodeAttributeSet;

/**
 * @brief   キー入力の文字コードを表す構造体です。
 */
struct KeyCode
{
    KeyCodeAttributeSet attributes; //!< 文字コードの属性
    uint16_t code;                  //!< 文字コード (UTF-16)
    ::nn::Bit8 _padding[2];
};

namespace detail {

struct KeyCodeMapStorage;

} // namespace detail

/**
 * @brief   キー入力の文字コード変換表を表す構造体です。
 */
struct KeyCodeMap
{
    ::nn::util::TypedStorage<detail::KeyCodeMapStorage, 4 * 1024, 8> _storage;
};

/**
 * @brief   キー入力の文字コード変換表の変換モードを表す列挙型です。
 */
enum KeyCodeMapMode
{
    KeyCodeMapMode_Default,     //!< 既定
    KeyCodeMapMode_Hiragana,    //!< かな入力（ひらがな）
    KeyCodeMapMode_Katakana,    //!< かな入力（カタカナ）
    KeyCodeMapMode_Cyrillic,    //!< キリル文字入力
    KeyCodeMapMode_Hangul,      //!< ハングル入力
    KeyCodeMapMode_Zhuyin,      //!< 注音入力
    KeyCodeMapMode_Cangjie,     //!< 倉頡入力
};

/**
 * @brief   キー入力の文字コード変換表を取得します。
 *
 * @param[out]  pOutValue                   文字コード変換表
 *
 * @pre
 *      - pOutValue != nullptr
 *
 * @post
 *      - *pOutValue は有効な文字コード変換表
 *
 * @details 呼び出し時点のキーボード入力設定に対応した文字コード変換表を取得します。
 *          取得した文字コード変換表は書き換えられない限り常に有効です。
 *          キーボード入力設定の変更を反映するためには、文字コード変換表を取得し直す必要があります。
 */
void GetKeyCodeMap(KeyCodeMap* pOutValue) NN_NOEXCEPT;

/**
 * @brief   キー入力の文字コード変換表が指定された変換モードをサポートするか否かを表す値を返します。
 *
 * @param[in]   map                         文字コード変換表
 * @param[in]   mode                        文字コード変換表の変換モード
 *
 * @return  キー入力の文字コード変換表が指定された変換モードをサポートするか否かを表す値です。
 *
 * @pre
 *      - map は有効な文字コード変換表
 *
 * @details KeyCodeMapMode_Default が指定された場合は常に true を返します。
 */
bool IsKeyCodeMapModeSupported(
    const KeyCodeMap& map, KeyCodeMapMode mode) NN_NOEXCEPT;

/**
 * @brief   キー入力の HID Usage ID に対応する文字コードを取得します。
 *
 * @param[out]  pOutValue                   文字コード
 * @param[in]   usageId                     キー入力の HID Usage ID
 * @param[in]   modifiers                   キー入力の修飾情報
 * @param[in]   map                         文字コード変換表
 * @param[in]   mode                        文字コード変換表の変換モード
 *
 * @pre
 *      - pOutValue != nullptr
 *      - 0x0000 <= usageId && usageId <= 0xffff
 *      - map は有効な文字コード変換表
 *
 * @details 対応する文字コードが存在しない場合は pOutValue->code の値には 0 を設定します。
 *          サポート外の変換モードが指定された場合は KeyCodeMapMode_Default が指定された場合と同じ結果が得られます。
 */
void GetKeyCode(
    KeyCode* pOutValue,
    int usageId, ::nn::hid::KeyboardModifierSet modifiers,
    const KeyCodeMap& map, KeyCodeMapMode mode) NN_NOEXCEPT;

}} // namespace nn::kpr
