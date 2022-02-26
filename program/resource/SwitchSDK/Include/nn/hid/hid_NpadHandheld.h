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

typedef ::nn::util::BitFlagSet<32, NpadStyleTag>::Flag<1> NpadStyleHandheld;          //!< 携帯機コントローラーをモデルとした操作スタイルの定義です。

/**
 * @brief       Npad の携帯機コントローラーをモデルとした操作スタイル (NpadStyleHandheld) の入力状態を表す構造体です。
 *
 * @details     携帯機コントローラーをモデルとした操作スタイルで扱えるデジタルボタン ( 十字ボタン, A, B, X, Y, L, R, ZL, ZR, Plus, Minus ) と
 *              左右アナログスティックの入力状態を提供します。
 *
 *              十字ボタンは禁則処理が行われます
 *              上下または左右の同時押しが発生した場合、それぞれ上と左が優先されます
 */
struct NpadHandheldState
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
 * @brief       Npad の携帯機コントローラーをモデルとした操作スタイル (NpadStyleHandheld) の最新の入力状態を取得します。
 *
 * @details     指定の Npad ID と対応する Npad から GetNpadStates() で 1 つの入力状態を読み出した時と同じ値が返ります。
 *              最低 1 回の入力状態の更新が保証されるため、 SetSupportedNpadIdType() 呼び出し完了直後から利用可能です。
 *
 *              携帯機コントローラーをモデルとした操作スタイルは NpadId::Handheld に対してのみ有効です。
 *              指定の Npad ID で NpadStyleHandheld が有効になっていない場合、入力状態は無入力（デジタルボタンの押下は無く、アナログスティックはニュートラル位置）となります。
 *              Npad ID に対して有効な操作スタイルは GetNpadStyleSet() で取得できます。
 *
 *              入力状態は、 NpadHandheldState の構造体で取得されます。
 *              具体的に読み出せる入力状態は NpadHandheldState のリファレンスを確認してください。
 *
 * @param[out]  pOutValue                入力状態を読み出すバッファ
 * @param[in]   id                      Npad ID
 *
 * @pre
 *              - 指定の Npad ID が SetSupportedNpadIdType() で有効化されている
 *              - pOutValue != nullptr
 * @post
 *              - !(pOutValue->buttons.Get<NpadButton::Left>() && pOutValue->buttons.Get<NpadButton::Right>())
 *              - !(pOutValue->buttons.Get<NpadButton::Up>() && pOutValue->buttons.Get<NpadButton::Down>())
 *              - アナログスティックの入力状態 outValue->analogStickL と outValue->analogStickR は下記の条件を満たす円の内側（境界含む）に位置
 *                  - 原点は (0, 0)
 *                  - 半径は AnalogStickMax
 */
void GetNpadState(NpadHandheldState* pOutValue, const NpadIdType& id
                  ) NN_NOEXCEPT;

/**
 * @brief       Npad の携帯機コントローラーをモデルとした操作スタイル (NpadStyleHandheld) の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に、指定の Npad ID と対応する Npad から入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *
 *              読み出し可能な入力状態の最大数は NpadStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は NpadState::samplingNumber を参照してください。
 *
 *              携帯機コントローラーをモデルとした操作スタイルは NpadId::Handheld に対してのみ有効です。
 *              指定の Npad ID で NpadStyleHandheld が有効になっていない場合、入力状態は無入力（デジタルボタンの押下は無く、アナログスティックはニュートラル位置）となります。
 *              Npad ID に対して有効な操作スタイルは GetNpadStyleSet() で取得できます。
 *
 *              入力状態は、 NpadHandheldState の構造体で取得されます。
 *              具体的に読み出せる入力状態は NpadHandheldState のリファレンスを確認してください。
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
 *                  - !(pOutValues[i].buttons.Get<NpadButton::Left>() && pOutValues[i].buttons.Get<NpadButton::Right>())
 *                  - !(pOutValues[i].buttons.Get<NpadButton::Up>() && pOutValues[i].buttons.Get<NpadButton::Down>())
 *                  - アナログスティックの入力状態 pOutValues[i].analogStickL と pOutValues[i].analogStickR は下記の条件を満たす円の内側（境界含む）に位置
 *                      - 原点は (0, 0)
 *                      - 半径は AnalogStickMax
 */
int GetNpadStates(NpadHandheldState* pOutValues,
                  int count,
                  const NpadIdType& id) NN_NOEXCEPT;
//! @}

}} // namespace nn::hid
