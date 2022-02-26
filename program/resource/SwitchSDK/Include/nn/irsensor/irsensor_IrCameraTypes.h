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
 * @brief IR カメラに関する型の宣言。
 */

#pragma once

#include <nn/nn_TimeSpan.h>

namespace nn { namespace irsensor {

const int IrCameraImageWidth  = 320; //!< IR カメラで撮影できる画像の幅の最大値です。
const int IrCameraImageHeight = 240; //!< IR カメラで撮影できる画像の高さの最大値です。

const int IrCameraGainMin = 1;  //!< IR カメラのディジタルゲインの最小値です。
const int IrCameraGainMax = 16; //!< IR カメラのディジタルゲインの最大値です。

/**
 * @brief IR カメラのハンドルです。
 */
struct IrCameraHandle
{
    uint32_t _handle;
};

/**
 * @brief IR カメラの状態です。
 */
enum IrCameraStatus
{
    IrCameraStatus_Available,   //!< デバイスに IR カメラが搭載されており、利用可能です。
    IrCameraStatus_Unsupported, //!< デバイスに IR カメラが搭載されていません。
    IrCameraStatus_Unconnected, //!< デバイスが接続されていません。
};

/**
 * @brief IR カメラに対する環境ノイズの状態を表す構造体です。
 */
enum IrCameraAmbientNoiseLevel
{
    IrCameraAmbientNoiseLevel_Low,     //!< 環境ノイズが少なく、IR プロセッサが正常に使える状況です。
    IrCameraAmbientNoiseLevel_Middle,  //!< 環境ノイズが中程度にあり、場合によっては IR プロセッサの挙動に影響が出る状況です。
    IrCameraAmbientNoiseLevel_High,    //!< 環境ノイズが多く、IR プロセッサが正常に使えない状況です。
    IrCameraAmbientNoiseLevel_Unknown, //!< 環境ノイズの測定機能が無効で、ノイズ状態は不明です。
};

/**
 * @brief IR カメラの照明の設定です。
 */
enum IrCameraLightTarget
{
    IrCameraLightTarget_AllObjects,  //!< すべての照明を点灯します。最も明るい設定です。
    IrCameraLightTarget_FarObjects,  //!< 遠くまで狭い範囲を照らす照明だけを点灯します。
    IrCameraLightTarget_NearObjects, //!< 近くの広い範囲を照らす照明だけを点灯します。
    IrCameraLightTarget_None,        //!< すべての照明を消灯します。最も暗い設定です。
};

/**
 * @brief IR カメラの設定をおこなう構造体です。
 */
struct IrCameraConfig
{
    ::nn::TimeSpanType exposureTime; //!< 撮影時の露光時間です（イメージプロセッサごとに定義された ExposureTimeMin 以上 ExposureTimeMax 以下）。
    IrCameraLightTarget lightTarget; //!< 撮影時の照明の設定です。
    int gain;                        //!< 撮影時のディジタルゲインです（IrCameraGainMin 以上 IrCameraGainMax 以下）。
    bool isNegativeImageUsed;        //!< 撮影画像を反転するかどうか。
};

}} // namespace nn::irsensor
