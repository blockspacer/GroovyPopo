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
 * @brief       Npad に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace hid {

/**
 * @brief       Npad ID を表す型です。
 */
typedef uint32_t NpadIdType;

/**
 * @brief       Npad を識別するための Npad ID の定義です。
 *
 * @details     NpadId はコントローラー番号と携帯機コントローラーに対して NpadId が定義されます。
 *              無線接続または USB 等のケーブルで接続されるコントローラーに対しては、コントローラー番号が割り当てられます。
 *              コントローラーは基本的には接続された順に若いコントローラー番号に割り当てられます。
 *
 *              NpadId::Handheld は携帯機コントローラーからの入力を取得したい場合に使用します。
 *
 *              物理的なコントローラーとコントローラー番号の対応関係は変更されることがあります。
 *              たとえば、NpadId::No1 に接続されたコントローラーA とNpadId::No2 に接続されたコントローラーBを
 *              ユーザーは本体機能の中で入れ替えることが可能です。
 *              またスリープ時にコントローラーにはコントローラーが切断されるため、スリープ復帰時に対応関係が変化することもあります。
 */
struct NpadId
{
    static const NpadIdType No1 = 0x00;       //!< コントローラー番号 1 の Npad
    static const NpadIdType No2 = 0x01;       //!< コントローラー番号 2 の Npad
    static const NpadIdType No3 = 0x02;       //!< コントローラー番号 3 の Npad
    static const NpadIdType No4 = 0x03;       //!< コントローラー番号 4 の Npad
    static const NpadIdType No5 = 0x04;       //!< コントローラー番号 5 の Npad
    static const NpadIdType No6 = 0x05;       //!< コントローラー番号 6 の Npad
    static const NpadIdType No7 = 0x06;       //!< コントローラー番号 7 の Npad
    static const NpadIdType No8 = 0x07;       //!< コントローラー番号 8 の Npad
    static const NpadIdType Handheld = 0x20;  //!< 携帯機コントローラー の Npad
};

const int NpadStateCountMax = 16;   //!< 内部的に保持される Npad の入力状態の最大数

/**
 * @brief       Npad のデジタルボタン定義です。
 */
struct NpadButton
{
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<0> A;             //!< Npad の a ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<1> B;             //!< Npad の b ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<2> X;             //!< Npad の x ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<3> Y;             //!< Npad の y ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<4> StickL;        //!< Npad の L スティックボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<5> StickR;        //!< Npad の R スティックボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<6> L;             //!< Npad の L ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<7> R;             //!< Npad の R ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<8> ZL;            //!< Npad の ZL ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<9> ZR;            //!< Npad の ZR ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<10> Plus;         //!< Npad の Plus ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<11> Minus;        //!< Npad の Minus ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<12> Left;         //!< Npad の十字ボタン 左
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<13> Up;           //!< Npad の十字ボタン 上
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<14> Right;        //!< Npad の十字ボタン 右
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<15> Down;         //!< Npad の十字ボタン 下
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<16> StickLLeft;   //!< Npad の左スティックによる十字ボタンエミュレーション 左
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<17> StickLUp;     //!< Npad の左スティックによる十字ボタンエミュレーション 上
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<18> StickLRight;  //!< Npad の左スティックによる十字ボタンエミュレーション 右
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<19> StickLDown;   //!< Npad の左スティックによる十字ボタンエミュレーション 下
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<20> StickRLeft;   //!< Npad の右スティックによる十字ボタンエミュレーション 左
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<21> StickRUp;     //!< Npad の右スティックによる十字ボタンエミュレーション 上
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<22> StickRRight;  //!< Npad の右スティックによる十字ボタンエミュレーション 右
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<23> StickRDown;   //!< Npad の右スティックによる十字ボタンエミュレーション 下
};

/**
 * @brief       Npad のデジタルボタンの集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<64, NpadButton> NpadButtonSet;

/**
 * @brief       Npad の状態を表す属性値です。
 */
struct NpadAttribute
{
    typedef ::nn::util::BitFlagSet<32, NpadAttribute>::Flag<0> IsConnected;    //!< Npad の物理的な接続状態
    typedef ::nn::util::BitFlagSet<32, NpadAttribute>::Flag<1> IsWired;        //!< Npad がケーブルで何か対応デバイスに接続されているかどうか
};

/**
 * @brief       Npad の状態を表す属性値の集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, NpadAttribute> NpadAttributesSet;

struct NpadStyleTag;

/**
 * @brief       Npad の操作スタイルの集合を表す型です。
 */
typedef ::nn::util::BitFlagSet<32, NpadStyleTag> NpadStyleSet;

}} // namespace nn::hid
