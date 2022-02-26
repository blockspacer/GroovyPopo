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
 * @brief クラスタリングプロセッサに関する型の宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_TimeSpan.h>
#include <nn/util/util_MathTypes.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_ImageProcessorTypes.h>

namespace nn { namespace irsensor {

const int ClusteringProcessorStateCountMax = 5; //!< 内部的に保持される ClusteringProcessorState の最大数

const int ClusteringProcessorObjectCountMax = 16; //!< 一度に検出できるオブジェクトの数です。

const int ClusteringProcessorObjectPixelCountMax = 76800; //!< 検出できるオブジェクトの最大サイズです。
const int ClusteringProcessorOutObjectPixelCountMax = 65535; //!< 出力されるオブジェクトの最大サイズです。

const ::nn::TimeSpanType ClusteringProcessorExposureTimeMin = nn::TimeSpanType::FromMicroSeconds(7);   //!< クラスタリングプロセッサを利用するときの IR カメラの露光時間の最小値です。
const ::nn::TimeSpanType ClusteringProcessorExposureTimeMax = nn::TimeSpanType::FromMicroSeconds(600); //!< クラスタリングプロセッサを利用するときの IR カメラの露光時間の最大値です。

/**
 * @brief クラスタリングプロセッサの設定をおこなう構造体です。
 */
struct ClusteringProcessorConfig
{
    IrCameraConfig irCameraConfig;     //!< IR カメラの設定です。
    Rect windowOfInterest;             //!< 撮影画像で処理対象となる矩形領域です（左上隅のピクセルを (0, 0) とする、幅 IrCameraImageWidth、高さ IrCameraImageHeight の領域内）。
    int objectPixelCountMin;           //!< 検出するオブジェクトの最小サイズです（0 以上 ClusteringProcessorObjectPixelCountMax 以下）。
    int objectPixelCountMax;           //!< 検出するオブジェクトの最大サイズです（0 以上 ClusteringProcessorObjectPixelCountMax 以下）。
    int objectIntensityMin;            //!< 検出するオブジェクトを構成するピクセルの最小輝度値です（0 以上 IrCameraIntensityMax 以下）。
    bool isExternalLightFilterEnabled; //!< 外部光フィルタ機能が有効かどうか。
};

/**
 * @brief クラスタリングプロセッサでオブジェクトを表す構造体です。
 */
struct ClusteringData
{
    float averageIntensity;      //!< オブジェクトの平均輝度値です（0 以上 IrCameraIntensityMax 以下）。
    ::nn::util::Float2 centroid; //!< オブジェクトの重心座標です（ClusteringProcessorConfig::windowOfInterest で指定した領域内）。
    int32_t pixelCount;          //!< オブジェクトの面積です（ClusteringProcessorOutObjectPixcelCountMax 以下、かつ ClusteringProcessorConfig::objectPixelCountMin 以上 ClusteringProcessorConfig::objectPixelCountMax 以下）。
    Rect bound;                  //!< オブジェクトの外接矩形です（ClusteringProcessorConfig::windowOfInterest で指定した領域内）。
};

/**
 * @brief クラスタリングプロセッサの処理結果を表す構造体です。
 */
struct ClusteringProcessorState
{
    int64_t samplingNumber;                                    //!< クラスタリングプロセッサから処理結果を得る度に増加する値です（0 以上）。
    ::nn::TimeSpanType timeStamp;                              //!< 処理開始から処理結果を取得した時点までの時間情報です。
    int8_t objectCount;                                        //!< 検出したオブジェクトの数です（0 以上 ClusteringProcessorObjectCountMax 以下）。
    char reserved[3];
    IrCameraAmbientNoiseLevel ambientNoiseLevel;               //!< 検出した環境ノイズの状態です。
    ClusteringData objects[ClusteringProcessorObjectCountMax]; //!< 検出したオブジェクトのデータです。
};

}} // namespace nn::irsensor
