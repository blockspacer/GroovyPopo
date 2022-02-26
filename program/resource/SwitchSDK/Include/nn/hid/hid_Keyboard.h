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
 * @brief       Keyboard に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/hid/hid_KeyboardKey.h>
#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace hid {

const int KeyboardStateCountMax = 16;   //!< 内部的に保持される Keyboard の入力状態の最大数

/**
 * @brief       Keyboard の修飾情報定義です。
 */
struct KeyboardModifier
{
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<0> Control;      //!< Ctrl キーが押下中か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<1> Shift;        //!< Shift キーが押下中か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<2> LeftAlt;      //!< 左 Alt キーが押下中か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<3> RightAlt;     //!< 右 Alt キーが押下中か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<4> Gui;          //!< GUI キーが押下中か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<8> CapsLock;     //!< CapsLock が有効か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<9> ScrollLock;   //!< ScrollLock が有効か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<10> NumLock;     //!< NumLock が有効か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<11> Katakana;    //!< カタカナモードが有効か否か
    typedef ::nn::util::BitFlagSet<32, KeyboardModifier>::Flag<12> Hiragana;    //!< ひらがなモードが有効か否か
};

/**
 * @brief       Keyboard の修飾情報の集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, KeyboardModifier> KeyboardModifierSet;

/**
 * @brief       Keyboard の入力状態の属性定義です。
 */
struct KeyboardAttribute
{
    typedef ::nn::util::BitFlagSet<32, KeyboardAttribute>::Flag<0>
            IsConnected;    //!< Keyboard がシステムと接続状態にあるか否か
};

/**
 * @brief       Keyboard の入力状態の属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, KeyboardAttribute> KeyboardAttributeSet;

/**
 * @brief       Keyboard の入力状態を表す構造体です。
 */
struct KeyboardState
{
    int64_t samplingNumber;             //!< Keyboard の入力状態がサンプリングされる度に増加する値です。
    KeyboardModifierSet modifiers;      //!< Keyboard の修飾情報です。
    KeyboardAttributeSet attributes;    //!< Keyboard の入力状態の属性です。
    KeyboardKeySet keys;                //!< Keyboard のキーの状態です。
};

/**
 * @brief       Keyboard のロックキーイベント定義です。
 */
struct KeyboardLockKeyEvent
{
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<0>
            NumLockOn;          //!< Num Lock の有効化イベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<1>
            NumLockOff;         //!< Num Lock の無効化イベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<2>
            NumLockToggle;      //!< Num Lock の切り替えイベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<3>
            CapsLockOn;         //!< Caps Lock の有効化イベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<4>
            CapsLockOff;        //!< Caps Lock の無効化イベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<5>
            CapsLockToggle;     //!< Caps Lock の切り替えイベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<6>
            ScrollLockOn;       //!< Scroll Lock の有効化イベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<7>
            ScrollLockOff;      //!< Scroll Lock の無効化イベント
    typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>::Flag<8>
            ScrollLockToggle;   //!< Scroll Lock の切り替えイベント
};

/**
 * @brief       Keyboard のロックキーイベントの集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, KeyboardLockKeyEvent>
        KeyboardLockKeyEventSet;

//! @name Keyboard 関連 API
//! @{

/**
 * @brief       Keyboard を初期化します。
 */
void InitializeKeyboard() NN_NOEXCEPT;

/**
 * @brief       Keyboard の最新の入力状態を取得します。
 *
 * @details     GetKeyboardStates() で 1 つの入力状態を読み出した時と同じ値が返ります。
 *              最低 1 回の入力状態の更新が保証されるため、InitializeKeyboard() の呼び出し完了直後から利用可能です。
 *
 * @param[out]  pOutValue                   入力状態を読み出すバッファ
 *
 * @pre
 *              - InitializeKeyboard() の呼び出しが完了している
 *              - pOutState != nullptr
 */
void GetKeyboardState(KeyboardState* pOutValue) NN_NOEXCEPT;

/**
 * @brief       Keyboard の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *              読み出し可能な入力状態の最大数は KeyboardStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は KeyboardState::samplingNumber を参照してください。
 *
 * @param[out]  outValues                   入力状態を読み出すバッファ（配列）
 * @param[in]   count                       outValues の数
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - InitializeKeyboard() の呼び出しが完了している
 *              - outValues != nullptr
 *              - count >= 0
 * @post
 *              - 戻り値 n について、n >= 0
 */
int GetKeyboardStates(KeyboardState outValues[], int count) NN_NOEXCEPT;

/**
 * @brief       Keyboard のロックキーイベントを送信します。
 *
 * @details     イベントはフラグの添字の値が小さいものから順に処理されます。
 *              入力モードはキーボード操作からも変更される可能性があるため、
 *              当該関数呼び出し後のロック状態には何の保証も無い点に注意して
 *              ください。
 *
 * @param[in]   value                       ロックキーイベント
 *
 * @pre
 *              - InitializeKeyboard() の呼び出しが完了している
 */
void SendKeyboardLockKeyEvent(KeyboardLockKeyEventSet value) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
