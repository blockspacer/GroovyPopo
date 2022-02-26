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
 * @brief イメージ転送プロセッサに関する型の宣言。
 */

#pragma once

#include <nn/nn_TimeSpan.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_ImageProcessorTypes.h>

namespace nn { namespace irsensor {

const int ImageTransferProcessorQvgaImageSize   = 320 * 240;
const int ImageTransferProcessorQqvgaImageSize  = 160 * 120;
const int ImageTransferProcessorQqqvgaImageSize = 80 * 60;

const int ImageTransferProcessorImageSize320x240 = 320 * 240; //!< 8 ビットグレースケールの 320x240px 設定の画像サイズです。
const int ImageTransferProcessorImageSize160x120 = 160 * 120; //!< 8 ビットグレースケールの 160x120px 設定の画像サイズです。
const int ImageTransferProcessorImageSize80x60 = 80 * 60;     //!< 8 ビットグレースケールの 80x60px 設定の画像サイズです。
const int ImageTransferProcessorImageSize40x30 = 40 * 30;     //!< 8 ビットグレースケールの 40x30px 設定の画像サイズです。
const int ImageTransferProcessorImageSize20x15 = 20 * 15;     //!< 8 ビットグレースケールの 20x15px 設定の画像サイズです。

const int ImageTransferProcessorQvgaWorkBufferSize   = 152 * 1024;
const int ImageTransferProcessorQqvgaWorkBufferSize  = 40 * 1024;
const int ImageTransferProcessorQqqvgaWorkBufferSize = 12 * 1024;

const int ImageTransferProcessorWorkBufferSize320x240 = 152 * 1024; //!< 8 ビットグレースケールの 320x240px 設定の画像のためのワークバッファサイズです。
const int ImageTransferProcessorWorkBufferSize160x120 = 40 * 1024;  //!< 8 ビットグレースケールの 160x120px 設定の画像のためのワークバッファサイズです。
const int ImageTransferProcessorWorkBufferSize80x60 = 12 * 1024;    //!< 8 ビットグレースケールの 80x60px 設定の画像のためのワークバッファサイズです。
const int ImageTransferProcessorWorkBufferSize40x30 = 4 * 1024;     //!< 8 ビットグレースケールの 40x30px 設定の画像のためのワークバッファサイズです。
const int ImageTransferProcessorWorkBufferSize20x15 = 4 * 1024;     //!< 8 ビットグレースケールの 20x15px 設定の画像のためのワークバッファサイズです。

const ::nn::TimeSpanType ImageTransferProcessorExposureTimeMin = nn::TimeSpanType::FromMicroSeconds(7);   //!< イメージ転送プロセッサを利用するときの IR カメラの露光時間の最小値です。
const ::nn::TimeSpanType ImageTransferProcessorExposureTimeMax = nn::TimeSpanType::FromMicroSeconds(600); //!< イメージ転送プロセッサを利用するときの IR カメラの露光時間の最大値です。

/**
 * @brief イメージ転送プロセッサで取得する画像フォーマットです。
 */
enum ImageTransferProcessorFormat
{
    ImageTransferProcessorFormat_320x240, //!< 8 ビットグレースケールの 320x240px サイズの画像です。
    ImageTransferProcessorFormat_160x120, //!< 8 ビットグレースケールの 160x120px サイズの画像です。
    ImageTransferProcessorFormat_80x60,   //!< 8 ビットグレースケールの 80x60px サイズの画像です。
    ImageTransferProcessorFormat_40x30,   //!< 8 ビットグレースケールの 40x30px サイズの画像です。
    ImageTransferProcessorFormat_20x15,   //!< 8 ビットグレースケールの 20x15px サイズの画像です。
    ImageTransferProcessorFormat_Qvga   = ImageTransferProcessorFormat_320x240,
    ImageTransferProcessorFormat_Qqvga  = ImageTransferProcessorFormat_160x120,
    ImageTransferProcessorFormat_Qqqvga = ImageTransferProcessorFormat_80x60,
};

/**
 * @brief イメージ転送プロセッサの設定をおこなう構造体です。
 */
struct ImageTransferProcessorConfig
{
    IrCameraConfig irCameraConfig;       //!< IR カメラの設定です。
    ImageTransferProcessorFormat format; //!< 出力画像フォーマットです。
};

/**
 * @brief イメージ転送プロセッサの詳細設定をおこなう構造体です。
 */
struct ImageTransferProcessorExConfig
{
    IrCameraConfig irCameraConfig;               //!< IR カメラの設定です。
    ImageTransferProcessorFormat origFormat;     //!< 切り取り元画像フォーマットです。出力画像フォーマットサイズ以上の領域を設定します。
    ImageTransferProcessorFormat trimmingFormat; //!< 切り取った出力画像フォーマットです。
    int16_t trimmingStartX;                      //!< 切り取り元画像上での、切り取り開始点の X 座標です。ピクセル単位です。
    int16_t trimmingStartY;                      //!< 切り取り元画像上での、切り取り開始点の Y 座標です。ピクセル単位です。
    bool isExternalLightFilterEnabled;           //!< 外部光フィルタ機能が有効かどうか。
};

/**
 * @brief イメージ転送プロセッサの処理結果を表す構造体です。
 */
struct ImageTransferProcessorState
{
    int64_t samplingNumber;                      //!< イメージ転送プロセッサから処理結果を得る度に増加する値です（0 以上）。
    IrCameraAmbientNoiseLevel ambientNoiseLevel; //!< 検出した環境ノイズの状態です。
    char reserved[4];
};

}} // namespace nn::irsensor
