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
 * @brief イメージプロセッサに関する API の宣言。
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_ImageProcessorTypes.h>

namespace nn { namespace irsensor {

/**
 * @name イメージプロセッサ
 * @{
 */

/**
 * @brief      モーションIRカメラの処理を停止します。
 * @param[in]  handle IR カメラのハンドル
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 * @post
 *             - 指定のハンドルに対応する IR カメラが処理停止状態
 * @details    指定のハンドルに対応したモーションIRカメラの処理を停止します。
 *             どのイメージプロセッサを使っている場合でも、この関数で処理を停止させることができます。
 *             モーションIRカメラは処理実行状態では電力を消費し続けるので、処理が必要のないシーンではこの関数で停止してください。
 *             モーションIRカメラの停止が完了するまでブロックします。
 *             本関数は呼び出し元をブロックするため、メインループとは別のスレッドから呼び出すことを推奨します。
 */
void StopImageProcessor(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      モーションIRカメラの処理を非同期に停止します。
 * @param[in]  handle IR カメラのハンドル
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 * @post
 *             - 指定のハンドルに対応する IR カメラが処理停止予約状態
 * @details    指定のハンドルに対応したモーションIRカメラの処理を停止します。
 *             どのイメージプロセッサを使っている場合でも、この関数で処理を停止させることができます。
 *             モーションIRカメラは処理実行状態では電力を消費し続けるので、処理が必要のないシーンではこの関数で停止してください。
 *             モーションIRカメラの停止は本 API とは非同期に行われます。
 *             モーションIRカメラが停止したかどうかは、GetimageProcessorStatus() を呼び出すことで確認することができます。
 */
void StopImageProcessorAsync(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      イメージプロセッサの状態を取得します。
 * @param[in]  handle IR カメラのハンドル
 * @return     指定のハンドルに対応したデバイスについてのイメージプロセッサの状態
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 * @details    指定のハンドルに対応した Npad などのデバイスについてイメージプロセッサの状態を取得します。
 *             この関数で取得した状態によらず RunMomentProcessor() などを呼び出して
 *             モーションIRカメラを処理実行状態にしたり、 StopImageProcessor() などを呼び出して、
 *             モーションIRカメラを処理停止状態にできます。
 */
ImageProcessorStatus GetImageProcessorStatus(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      Rect 構造体のインスタンスを作成します。
 * @param[in]  x      始点（左上）の X 座標
 * @param[in]  y      始点（左上）の Y 座標
 * @param[in]  width  幅
 * @param[in]  height 高さ
 * @return     Rect 構造体のインスタンス
 */
Rect MakeRect(int x, int y, int width, int height) NN_NOEXCEPT;

/**
 * @}
 */

}} // namespace nn::irsensor
