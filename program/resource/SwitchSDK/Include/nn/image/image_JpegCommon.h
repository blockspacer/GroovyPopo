/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/image/image_Common.h>

/**
    @file
    @brief      JPEG データの取り扱いに関するデータ型の宣言
    @details    JPEG ライブラリの各機能が共通して使用する種々のデータ型を定義します。
*/

namespace nn { namespace image {

/**
    @brief      JPEGライブラリが返し得るステータスコード
    @details    JPEGライブラリが提供する機能のうち処理の成否があるものは、ここに挙げるステータスコードのうちいずれかを返却
                します。
                返るステータスコードやその要因は各APIごとに異なります。詳細は各APIのリファレンスを参照してください。
*/
enum JpegStatus
{
    JpegStatus_Ok = 0,                  //!< 成功
    JpegStatus_WrongFormat = -32,       //!< 不正な形式のデータの入力
    JpegStatus_UnsupportedFormat = -33, //!< サポートしていない形式のデータの入力
    JpegStatus_OutOfMemory = -64,       //!< ワークメモリの不足
    JpegStatus_ShortOutput = -65,       //!< 出力バッファの不足
    JpegStatus_OutputOverabounds = -66, //!< アプリケーションによる入力データの過剰
};

/**
    @brief      JPEGデータのサンプリング比
    @details    JPEGエンコードを行う際に指定可能な、サンプリング比を表します。
                一般に420, 422, 444の順で画質が向上し、出力データサイズが増加します。
*/
enum JpegSamplingRatio
{
    JpegSamplingRatio_444 = 0,  //!< Y,Cb,Crをそれぞれ4:4:4の割合でサンプリングします。
    JpegSamplingRatio_422 = 1,  //!< Y,Cb,Crをそれぞれ4:2:2の割合でサンプリングします。
    JpegSamplingRatio_420 = 2   //!< Y,Cb,Crをそれぞれ4:2:0の割合でサンプリングします。
};

/**
    @brief      画像の方向
    @details    JPEGデータのメタ情報の一つである
                「画像方向」を表します。

                Rotate とつく項目は「画像がどう回転したか」を示すもので、
                「撮影機器がどう回転したか」を示さないことに注意してください。
                撮影機器の回転角を考えるときは、符号が逆になります。
                また、回転角は時計回りでみた値を示します。
 */
enum ExifOrientation
{
    ExifOrientation_Normal = 1,                     //!< 回転していない状態
    ExifOrientation_FlipHorizontal = 2,             //!< 画像を左右反転した状態
    ExifOrientation_Rotate180 = 3,                  //!< 画像を180度回転した状態
    ExifOrientation_FlipVertical = 4,               //!< 画像を上下反転した状態
    ExifOrientation_FlipTopRightToLeftBottom = 5,   //!< 画像を、左上と右下を結ぶ直線を軸に反転した状態
    ExifOrientation_Rotate270 = 6,                  //!< 画像を時計回りに270度回転した状態
    ExifOrientation_FlipTopLeftToRightBottom = 7,   //!< 画像を、右上と左下を結ぶ直線を軸に反転した状態
    ExifOrientation_Rotate90 = 8,                   //!< 画像を時計回りに90度回転した状態
};

}}
