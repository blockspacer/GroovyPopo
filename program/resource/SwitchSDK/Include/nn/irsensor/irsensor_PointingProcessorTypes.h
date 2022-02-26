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
 * @brief Pointing プロセッサに関する型の宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_TimeSpan.h>
#include <nn/util/util_MathTypes.h>

namespace nn { namespace irsensor {

const int PointingProcessorStateCountMax = 6; //!< 内部的に保持される PointingProcessorState の最大数

/**
 * @brief Pointing プロセッサの返すデータの状態です。
 */
enum PointingStatus
{
    PointingStatus_DataValid,     //!< データは有効です。
    PointingStatus_LackOfObjects, //!< 検出するオブジェクト数が足りていません。
};

/**
 * @brief Pointing プロセッサの処理結果を表す構造体です。
 */
struct PointingProcessorState
{
    int64_t samplingNumber;         //!< Pointing プロセッサから処理結果を得る度に増加する値です（0 以上）。
    ::nn::TimeSpanType timeStamp;   //!< 処理開始時点から処理結果を取得した時点までの時間情報です。過去データの時間との差分をとって扱います(0 以上)。
    PointingStatus pointingStatus;  //!< 検出した Pointing データの状態です。
    ::nn::util::Float2 position;    //!< 正規化されたポインティングの位置座標です。（中心を (0.0f, 0.0f) として、右上を (1.0f, 1.0f)、アスペクト比4:3 で正規化されています。)
};

}} // namespace nn::irsensor
