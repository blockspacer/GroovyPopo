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
 * @brief       Keyboard のキーに関する API の宣言
 */

#pragma once

//#include <nn/TargetConfigs/build_Base.h>
#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace hid {

struct KeyboardKey;

}} // namespace nn::hid

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG) || \
    defined(NN_BUILD_CONFIG_TOOLCHAIN_GCC)
#include <nn/hid/detail/hid_KeyboardKeyDeclaration.h>
#endif

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#include <nn/hid/detail/hid_KeyboardKeyDefinition.h>
#endif

namespace nn { namespace hid {

/**
 * @brief       Keyboard のキー定義です。
 */
struct KeyboardKey
{
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x04> A;                 //!< A キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x05> B;                 //!< B キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x06> C;                 //!< C キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x07> D;                 //!< D キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x08> E;                 //!< E キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x09> F;                 //!< F キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x0A> G;                 //!< G キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x0B> H;                 //!< H キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x0C> I;                 //!< I キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x0D> J;                 //!< J キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x0E> K;                 //!< K キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x0F> L;                 //!< L キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x10> M;                 //!< M キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x11> N;                 //!< N キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x12> O;                 //!< O キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x13> P;                 //!< P キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x14> Q;                 //!< Q キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x15> R;                 //!< R キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x16> S;                 //!< S キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x17> T;                 //!< T キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x18> U;                 //!< U キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x19> V;                 //!< V キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x1A> W;                 //!< W キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x1B> X;                 //!< X キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x1C> Y;                 //!< Y キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x1D> Z;                 //!< Z キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x1E> D1;                //!< 1 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x1F> D2;                //!< 2 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x20> D3;                //!< 3 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x21> D4;                //!< 4 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x22> D5;                //!< 5 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x23> D6;                //!< 6 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x24> D7;                //!< 7 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x25> D8;                //!< 8 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x26> D9;                //!< 9 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x27> D0;                //!< 0 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x28> Return;            //!< Return キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x29> Escape;            //!< Escape キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x2A> Backspace;         //!< Backspace キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x2B> Tab;               //!< Tab キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x2C> Space;             //!< Space キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x2D> Minus;             //!< <c>-</c> キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x2E> Plus;              //!< <c>+</c> キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x2F> OpenBracket;       //!< [ キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x30> CloseBracket;      //!< ] キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x31> Pipe;              //!< | キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x32> Tilde;             //!< ~ キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x33> Semicolon;         //!< ; キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x34> Quote;             //!< ' キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x35> Backquote;         //!< ` キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x36> Comma;             //!< , キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x37> Period;            //!< Period(.) キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x38> Slash;             //!< / キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x39> CapsLock;          //!< Caps Lock キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x3A> F1;                //!< F1 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x3B> F2;                //!< F2 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x3C> F3;                //!< F3 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x3D> F4;                //!< F4 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x3E> F5;                //!< F5 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x3F> F6;                //!< F6 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x40> F7;                //!< F7 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x41> F8;                //!< F8 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x42> F9;                //!< F9 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x43> F10;               //!< F10 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x44> F11;               //!< F11 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x45> F12;               //!< F12 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x46> PrintScreen;       //!< Print Screen キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x47> ScrollLock;        //!< Scroll Lock キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x48> Pause;             //!< Pause キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x49> Insert;            //!< Insert キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x4A> Home;              //!< Home キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x4B> PageUp;            //!< Page Up キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x4C> Delete;            //!< Delete キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x4D> End;               //!< End キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x4E> PageDown;          //!< Page Down キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x4F> RightArrow;        //!< 右矢印キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x50> LeftArrow;         //!< 左矢印キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x51> DownArrow;         //!< 下矢印キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x52> UpArrow;           //!< 上矢印キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x53> NumLock;           //!< Num Lock キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x54> NumPadDivide;      //!< NumPad の / キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x55> NumPadMultiply;    //!< NumPad の * キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x56> NumPadSubtract;    //!< NumPad の - キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x57> NumPadAdd;         //!< NumPad の + キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x58> NumPadEnter;       //!< NumPad の Enter キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x59> NumPad1;           //!< NumPad の 1 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x5A> NumPad2;           //!< NumPad の 2 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x5B> NumPad3;           //!< NumPad の 3 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x5C> NumPad4;           //!< NumPad の 4 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x5D> NumPad5;           //!< NumPad の 5 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x5E> NumPad6;           //!< NumPad の 6 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x5F> NumPad7;           //!< NumPad の 7 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x60> NumPad8;           //!< NumPad の 8 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x61> NumPad9;           //!< NumPad の 9 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x62> NumPad0;           //!< NumPad の 0 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x63> NumPadDot;         //!< NumPad の Dot(.) キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x64> Backslash;         //!< \ キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x65> Application;       //!< アプリケーションキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x66> Power;             //!< Power キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x67> NumPadEquals;      //!< NumPad の = キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x68> F13;               //!< F13 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x69> F14;               //!< F14 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x6A> F15;               //!< F15 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x6B> F16;               //!< F16 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x6C> F17;               //!< F17 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x6D> F18;               //!< F18 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x6E> F19;               //!< F19 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x6F> F20;               //!< F20 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x70> F21;               //!< F21 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x71> F22;               //!< F22 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x72> F23;               //!< F23 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x73> F24;               //!< F24 キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x85> NumPadComma;       //!< NumPad の , キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x87> Ro;                //!< ろキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x88> KatakanaHiragana;  //!< カタカナひらがなキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x89> Yen;               //!< 円記号キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x8A> Henkan;            //!< 変換キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x8B> Muhenkan;          //!< 無変換キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x8C> NumPadCommaPc98;   //!< NumPad の , キー（PC98 用）
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x90> HangulEnglish;     //!< Hangul モードキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x91> Hanja;             //!< Hanja モードキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x92> Katakana;          //!< カタカナキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x93> Hiragana;          //!< ひらがなキー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0x94> ZenkakuHankaku;    //!< 半角/全角キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE0> LeftControl;       //!< 左 Ctrl キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE1> LeftShift;         //!< 左 Shift キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE2> LeftAlt;           //!< 左 Alt キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE3> LeftGui;           //!< 左 GUI キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE4> RightControl;      //!< 右 Ctrl キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE5> RightShift;        //!< 右 Shift キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE6> RightAlt;          //!< 右 Alt キー
    typedef ::nn::util::BitFlagSet<256, KeyboardKey>::Flag<0xE7> RightGui;          //!< 右 GUI キー
};

/**
 * @brief       Keyboard のキーの集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<256, KeyboardKey> KeyboardKeySet;

}} // namespace nn::hid
