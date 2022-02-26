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
#include <nn/nn_Result.h>
#include <nn/util/util_MathTypes.h>
#include <nn/hid/hid_NpadCommonTypes.h>

namespace nn { namespace hid {


/**
 * @brief      Npad に割り当てられたコントローラーの色情報を表す構造体です。
 */
struct NpadControllerColor
{
    nn::util::Color4u8Type main;                               //!< コントローラーの主となる色 (本体部分)
    nn::util::Color4u8Type sub;                                //!< コントローラーのサブ部分の色 (ボタン部)
};


//! @name Npad 共通の API
//! @{

/**
 * @brief       Npad に割り当てられたコントローラーの色を取得します。
 *
 * @details
 *              Npad ID で指定された Npad について、コントローラーの色情報を取得します。
 *
 *              色情報は、nn::util::Color4u8Type で取得されます。
 *              コントローラーの色にはアルファ値の設定がないため、アルファ値は常に 255 を返します。
 *
 *              本関数では、NpadIdType で指定した Npad の GetNpadStyle() で取得される現在の操作スタイルが、NpadStyleJoyLeft / NpadStyleJoyRight / NpadStyleFullKey のいずれかのときに
 *              色情報を取得することが可能です。
 *              NpadStyleJoyDual または NpadStyleHandheld で動作している Npad については、 GetNpadControllerColor(NpadControllerColor* pOutLeftColor, NpadControllerColor* pOutLeftColor, const NpadIdType& id) を使用してください。
 *
 * @param[out]  pOutValue               Npad に割り当てられたコントローラーの色情報
 * @param[in]   id                      Npad ID
 *
 * @retresult
 *   @handleresult{nn::ResultSuccess,                      処理に成功しました。}
 *   @handleresult{nn::hid::ResultNpadColorNotAvailable,       色情報を取得できませんでした。}
 *   @handleresult{nn::hid::ResultNpadControllerNotConnected,  指定された Npad に対してコントローラーが接続されていません。}
 * @endretresult
 *
 * @pre
 *              - id が SetSupportedNpadIdType() で有効になっていること。
 */
Result GetNpadControllerColor(NpadControllerColor* pOutValue, const NpadIdType& id) NN_NOEXCEPT;

/**
 * @brief       Npad に割り当てられた左右の Joy-Con の色を取得します。
 *
 * @details
 *              Npad Id で指定された Npad について、左右の Joy-Con の色情報を取得します。
 *
 *              色情報は、nn::util::Color4u8Type で取得されます。
 *              コントローラーの色にはアルファ値の設定がないため、アルファ値は常に 255 を返します。
 *
 *              本関数では、NpadIdType で指定した Npad の GetNpadStyle() で取得される現在の操作スタイルが、NpadStyleJoyLeft / NpadStyleJoyRight / NpadStyleHandheld のいずれかのときに
 *              色情報を取得することが可能です。
 *              NpadStyleFullKey で動作している Npad については、 GetNpadControllerColor(NpadControllerColor* pOutValue, const NpadIdType& id) を使用してください。
 *
 * @param[out]  pOutRightColor          Npad に割り当てられた Joy-Con (R) の色情報
 * @param[out]  pOutLeftColor           Npad に割り当てられた Joy-Con (L) の色情報
 * @param[in]   id                      Npad ID
 *
 * @retresult
 *   @handleresult{nn::ResultSuccess,                      処理に成功しました。}
 *   @handleresult{nn::hid::ResultNpadColorNotAvailable,       色情報を取得できませんでした。}
 *   @handleresult{nn::hid::ResultNpadControllerNotConnected,  指定された Npad に対してコントローラーが接続されていません。}
 * @endretresult
 *
 * @pre
 *              - id が SetSupportedNpadIdType() で有効になっていること。
 */
Result GetNpadControllerColor(NpadControllerColor* pOutLeftColor, NpadControllerColor* pOutRightColor, const NpadIdType& id) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
