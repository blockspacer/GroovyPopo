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
#include <nn/hid/hid_NpadCommonTypes.h>
#include <nn/hid/hid_AnalogStickState.h>
#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace hid {

/**
 * @brief       Joy-Con 2本持ちをモデルとした操作スタイルの定義です。
 *
 * @details     本操作スタイルが有効になるためには、Npad に対する Joy-Con の割り当て状態が nn::hid::SetNpadJoyAssignmentModeDual() で 2本持ちになっている必要があります。
 *              nn::hid::NpadStyleJoyDual や nn::hid::NpadStyleFullkey など 2本持ちを前提とする操作スタイルのみが有効になっている場合、
 *              Npad に対する Joy-Con の割り当て状態は自動的に 2本持ち (nn::hid::NpadJoyAssignmentMode_Dual) となります。
 *
 *              例えば、Home メニューの「つなぎなおし」の画面などで 1本持ちとして Npad に割り当てられた Joy-Con がある状況で、
 *              2本持ちのみに対応したアプリケーションに遷移すると、1本持ち状態の Joy-Con は切断されます。
 *              上記のようなケースにおいてコントローラーが切断されないようにしたい場合は、 nn::hid::NpadStyleJoyLeft 及び nn::hid::NpadStyleJoyRight を有効にし、
 *              1本持ちとして接続された Joy-Con を nn::hid::SetNpadJoyAssignmentModeDual() で 2本持ちへと切り替えてください。
 */
typedef ::nn::util::BitFlagSet<32, NpadStyleTag>::Flag<2> NpadStyleJoyDual;

/**
 * @brief       Npad の Joy-Con 2本持ちをモデルとした操作スタイル (NpadStyleJoyDual) の入力状態を表す構造体です。
 *
 * @details     Joy-Con  2本持ち操作で扱えるデジタルボタン
 *              ( 方向ボタン, A, B, X, Y, L, R, ZL, ZR, Joy-Con (L) SL, Joy-Con (L) SR, Joy-Con (R) SL, Joy-Con (R) SR, Plus, Minus ) と
 *              左右アナログスティックの入力状態を提供します。
 *              左右の Joy-Con のいずれかが接続されると、attributes::IsConnected のビットがセットされます。
 *              左右の個別の接続状態は attributes::IsLeftConnected と attributes::IsRightConnected で取得できます。
 *
 *              方向ボタンは禁則処理が行われません
 */
struct NpadJoyDualState
{
    int64_t samplingNumber;             //!< Npad の入力状態が更新される度に増加する値です。
    NpadButtonSet buttons;              //!< Npad のデジタルボタンの状態です。
    AnalogStickState analogStickL;      //!< Npad の左アナログスティックの状態です。
    AnalogStickState analogStickR;      //!< Npad の右アナログスティックの状態です。
    NpadAttributesSet attributes;       //!< Npad の状態を表す属性値です。
};

//! @name Npad の操作スタイル毎の API
//! @{

/**
 * @brief       Npad の Joy-Con  2本持ち操作 (NpadStyleJoyDual) の最新の入力状態を取得します。
 *
 * @details     指定の Npad ID と対応する Npad から GetNpadStates() で 1 つの入力状態を読み出した時と同じ値が返ります。
 *              最低 1 回の入力状態の更新が保証されるため、 SetSupportedNpadIdType() 呼び出し完了直後から利用可能です。
 *
 *              指定の Npad ID で NpadStyleJoyDual が有効になっていない場合、入力状態は無入力（デジタルボタンの押下は無く、アナログスティックはニュートラル位置）となります。
 *              Npad ID に対して有効な操作スタイルは GetNpadStyleSet() で取得できます。
 *
 *              入力状態は、 NpadJoyDualState の構造体で取得されます。
 *              具体的に読み出せる入力状態は NpadJoyDualState のリファレンスを確認してください。
 *
 * @param[out]  pOutValue               入力状態を読み出すバッファ
 * @param[in]   id                      Npad ID
 *
 * @pre
 *              - 指定の Npad ID が SetSupportedNpadIdType() で有効化されている
 *              - pOutValue != nullptr
 * @post
 *              - アナログスティックの入力状態 outValue->analogStickL と outValue->analogStickR は下記の条件を満たす円の内側（境界含む）に位置
 *                  - 原点は (0, 0)
 *                  - 半径は AnalogStickMax
 */
void GetNpadState(NpadJoyDualState* pOutValue, const NpadIdType& id
                  ) NN_NOEXCEPT;

/**
 * @brief       Npad の Joy-Con  2本持ち操作 (NpadStyleJoyDual) の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に、指定の Npad ID と対応する Npad から入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *
 *              読み出し可能な入力状態の最大数は NpadStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は NpadJoyDualState::samplingNumber を参照してください。
 *
 *              指定の Npad ID で NpadStyleJoyDual が有効になっていない場合、入力状態は無入力（デジタルボタンの押下は無く、アナログスティックはニュートラル位置）となります。
 *              Npad ID に対して有効な操作スタイルは GetNpadStyleSet() で取得できます。
 *
 *              入力状態は、 NpadJoyDualState の構造体で取得されます。
 *              具体的に読み出せる入力状態は NpadJoyDualState のリファレンスを確認してください。
 *
 * @param[out]  pOutValues              入力状態を読み出すバッファ（配列）
 * @param[in]   count                   読み出す入力状態の数
 * @param[in]   id                      Npad ID
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - 指定の Npad ID が SetSupportedNpadIdType() で有効化されている
 *              - pOutValues != nullptr
 *              - count >= 0
 * @post
 *              - 戻り値 n について、n >= 0
 *              - 読み出した入力状態 pOutValues[i] について
 *                  - アナログスティックの入力状態 pOutValues[i].analogStickL と pOutValues[i].analogStickR は下記の条件を満たす円の内側（境界含む）に位置
 *                      - 原点は (0, 0)
 *                      - 半径は AnalogStickMax
 */
int GetNpadStates(NpadJoyDualState* pOutValues,
                  int count,
                  const NpadIdType& id) NN_NOEXCEPT;
//! @}

}} // namespace nn::hid
