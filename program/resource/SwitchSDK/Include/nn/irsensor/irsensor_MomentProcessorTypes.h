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
 * @brief モーメントプロセッサに関する型の宣言。
 */

#pragma once

#include <nn/nn_TimeSpan.h>
#include <nn/util/util_MathTypes.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_ImageProcessorTypes.h>

namespace nn { namespace irsensor {

const int MomentProcessorStateCountMax = 5; //!< 内部的に保持される MomentProcessorState の最大数

const int MomentProcessorBlockColumnCount = 8; //!< モーメントプロセッサの X 軸方向のブロック分割数です。
const int MomentProcessorBlockRowCount    = 6; //!< モーメントプロセッサの Y 軸方向のブロック分割数です。
const int MomentProcessorBlockCount       = MomentProcessorBlockColumnCount * MomentProcessorBlockRowCount; //!< モーメントプロセッサのブロック分割数です。

const ::nn::TimeSpanType MomentProcessorExposureTimeMin = nn::TimeSpanType::FromMicroSeconds(7);  //!< モーメントプロセッサを利用するときの IR カメラの露光時間の最小値です。
const ::nn::TimeSpanType MomentProcessorExposureTimeMax = nn::TimeSpanType::FromMicroSeconds(600); //!< モーメントプロセッサを利用するときの IR カメラの露光時間の最大値です。

/**
 * @brief モーメントプロセッサのモーメントの算出前に画像に対しておこなう処理です。
 */
enum MomentProcessorPreprocess
{
    MomentProcessorPreprocess_Binarize, //!< 閾値以下の輝度値を 0、閾値を超える輝度値を IrCameraIntensityMax にします。
    MomentProcessorPreprocess_Cutoff,   //!< 閾値以下の輝度値を 0 にします。
};

/**
 * @brief モーメントプロセッサの設定をおこなう構造体です。
 */
struct MomentProcessorConfig
{
    IrCameraConfig irCameraConfig;        //!< IR カメラの設定です。
    Rect windowOfInterest;                //!< 撮影画像で処理対象となる矩形領域です（左上隅のピクセルを (0, 0) とする、幅 IrCameraImageWidth、高さ IrCameraImageHeight の領域内）。
    MomentProcessorPreprocess preprocess; //!< 前処理の種類です。
    int preprocessIntensityThreshold;     //!< 前処理に利用する閾値です（0 以上 IrCameraIntensityMax 以下）。
};

/**
 * @brief ブロックの統計量を表す構造体です。
 */
struct MomentStatistic
{
    float averageIntensity;      //!< ブロックの平均輝度値です（0 以上 IrCameraIntensityMax 以下）。
    ::nn::util::Float2 centroid; //!< ブロックの重心座標です（MomentProcessorConfig::windowOfInterest で指定した領域内）。
};

/**
 * @brief モーメントプロセッサの処理結果を表す構造体です。
 */
struct MomentProcessorState
{
    int64_t samplingNumber;                            //!< モーメントプロセッサから処理結果を得る度に増加する値です（0 以上）。
    ::nn::TimeSpanType timeStamp;                      //!< 処理開始から処理結果を取得した時点までの時間情報です。
    IrCameraAmbientNoiseLevel ambientNoiseLevel;       //!< 検出した環境ノイズの状態です。
    char reserved[4];
    MomentStatistic blocks[MomentProcessorBlockCount]; //!< ブロックごとの統計量です。
};

}} // namespace nn::irsensor
