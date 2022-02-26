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
 * @brief       DebugPad に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/util/util_BitFlagSet.h>

#include <nn/hid/hid_AnalogStickState.h>

namespace nn { namespace hid {

const int DebugPadStateCountMax = 16;   //!< 内部的に保持される DebugPad の入力状態の最大数

/**
 * @brief       DebugPad のデジタルボタン定義です。
 */
struct DebugPadButton
{
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<0> A;      //!< DebugPad の a ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<1> B;      //!< DebugPad の b ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<2> X;      //!< DebugPad の X ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<3> Y;      //!< DebugPad の Y ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<4> L;      //!< DebugPad の L ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<5> R;      //!< DebugPad の R ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<6> ZL;     //!< DebugPad の ZL ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<7> ZR;     //!< DebugPad の ZR ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<8> Start;  //!< DebugPad の Start ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<9> Select; //!< DebugPad の Select ボタン
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<10> Left;  //!< DebugPad の十字ボタン 左
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<11> Up;    //!< DebugPad の十字ボタン 上
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<12> Right; //!< DebugPad の十字ボタン 右
    typedef ::nn::util::BitFlagSet<32, DebugPadButton>::Flag<13> Down;  //!< DebugPad の十字ボタン 下
};

/**
 * @brief       DebugPad のデジタルボタンの集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, DebugPadButton> DebugPadButtonSet;

/**
 * @brief       DebugPad の入力状態の属性定義です。
 */
struct DebugPadAttribute
{
    typedef ::nn::util::BitFlagSet<32, DebugPadAttribute>::Flag<0>
            IsConnected;    //!< DebugPad がシステムと接続状態にあるか否か
};

/**
 * @brief       DebugPad の入力状態の属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, DebugPadAttribute> DebugPadAttributeSet;

/**
 * @brief       DebugPad の入力状態を表す構造体です。
 */
struct DebugPadState
{
    int64_t samplingNumber;             //!< DebugPad の入力状態が更新される度に増加する値です。
    DebugPadAttributeSet attributes;    //!< DebugPad の入力状態の属性です。
    DebugPadButtonSet buttons;          //!< DebugPad のデジタルボタンの状態です。
    AnalogStickState analogStickR;      //!< DebugPad の右アナログスティックの状態です。
    AnalogStickState analogStickL;      //!< DebugPad の左アナログスティックの状態です。
};

//! @name DebugPad 関連 API
//! @{

/**
 * @brief       DebugPad を初期化します。
 */
void InitializeDebugPad() NN_NOEXCEPT;

/**
 * @brief       DebugPad の最新の入力状態を取得します。
 *
 * @details     GetDebugPadStates() で 1 つの入力状態を読みだした時と同じ値が返ります。
 *              最低 1 回の入力状態の更新が保証されるため、 InitializeDebugPad() の呼び出し完了直後から利用可能です。
 *
 * @param[out]  pOutValue                   入力状態を読み出すバッファ
 *
 * @pre
 *              - InitializeDebugPad() の呼び出しが完了している
 *              - pOutValue != nullptr
 * @post
 *              - !(pOutValue->buttons.Get<DebugPadButton::Left>() && pOutValue->buttons.Get<DebugPadButton::Right>())
 *              - !(pOutValue->buttons.Get<DebugPadButton::Up>() && pOutValue->buttons.Get<DebugPadButton::Down>())
 *              - アナログスティックの入力状態 pOutValue->analogStickL と pOutValue->analogStickR は下記の条件を満たす円の内側（境界含む）に位置
 *                  - 原点は (0, 0)
 *                  - 半径は AnalogStickMax
 */
void GetDebugPadState(DebugPadState* pOutValue) NN_NOEXCEPT;

/**
 * @brief       DebugPad の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *              読み出し可能な入力状態の最大数は DebugPadStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は DebugPadState::samplingNumber を参照してください。
 *
 * @param[out]  outValues                   入力状態を読み出すバッファ（配列）
 * @param[in]   count                       outValues の数
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - InitializeDebugPad() の呼び出しが完了している
 *              - outValues != nullptr
 *              - count >= 0
 * @post
 *              - 戻り値 n について、n >= 0
 *              - 読み出した入力状態 outValues[i] について
 *                  - !(outValues[i].buttons.Get<DebugPadButton::Left>() && outValues[i].buttons.Get<DebugPadButton::Right>())
 *                  - !(outValues[i].buttons.Get<DebugPadButton::Up>() && outValues[i].buttons.Get<DebugPadButton::Down>())
 *              - アナログスティックの入力状態 outValues[i].analogStickL と outValues[i].analogStickR は下記の条件を満たす円の内側（境界含む）に位置
 *                  - 原点は (0, 0)
 *                  - 半径は AnalogStickMax
 */
int GetDebugPadStates(DebugPadState outValues[], int count) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
