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
#include <nn/hid/hid_SixAxisSensor.h>

namespace nn { namespace hid {

const int NpadSixAxisSensorHandleCountMax = 8;   //!< Npad から取得できる SixAxisSensorHandle の最大数

//! @name 6 軸センサー関連 API
//! @{

/**
 * @brief       SixAxisSensor のハンドルを取得します。
 *
 * @details     Npad ID で指定された Npad に対応する SixAxisSensor のハンドルを取得します。
 *              SixAxisSensor のハンドルは Npad Id に対して操作スタイル(NpadStyle)毎に取得します。
 *              操作スタイルは NpadStyleSet で指定しますが、複数の NpadStyle を同時に指定することはできません。
 *              NpadStyle によって取得できるハンドルの数が異なるため、取得できたハンドルの数が返り値として取得できます。
 *
 *
 * @param[out]  pOutValues      6軸センサーのハンドルの格納先
 * @param[in]   count           pOutValues で取得するハンドルの最大数
 * @param[in]   id              Npad ID
 * @param[in]   style           ハンドルを取得する Npad の操作スタイル
 *
 * @return      取得されたハンドルの数
 *
 * @pre
 *              - style.IsAnyOn() == true
 *              - style.CountPopulation() == 1
 *              - 0 < count && count <= NpadSixAxisSensorHandleCountMax
 */
int GetSixAxisSensorHandles(SixAxisSensorHandle* pOutValues, int count, const NpadIdType& id, NpadStyleSet style) NN_NOEXCEPT;


//! @}

}} // namespace nn::hid
