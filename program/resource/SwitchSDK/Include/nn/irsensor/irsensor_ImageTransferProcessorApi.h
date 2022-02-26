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
 * @brief イメージ転送プロセッサに関する API の宣言。
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/irsensor/irsensor_Result.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_ImageTransferProcessorTypes.h>

namespace nn { namespace irsensor {

/**
 * @name イメージ転送プロセッサ
 * @{
 */

/**
 * @brief      イメージ転送プロセッサのデフォルト設定を取得します。
 * @param[out] pOutValue イメージ転送プロセッサのデフォルト設定
 * @pre
 *             - pOutValue != nullptr
 * @post
 *             - イメージ転送プロセッサの設定 pOutValue について
 *                 - pOutValue->format == ImageTransferProcessorFormat_320x240
 */
void GetImageTransferProcessorDefaultConfig(ImageTransferProcessorConfig* pOutValue) NN_NOEXCEPT;

/**
 * @brief      イメージ転送プロセッサの詳細設定用のデフォルト設定を取得します。
 * @param[out] pOutValue イメージ転送プロセッサのデフォルト設定
 * @pre
 *             - pOutValue != nullptr
 * @post
 *             - イメージ転送プロセッサの設定 pOutValue について
 *                 - pOutValue->origFormat == ImageTransferProcessorFormat_320x240
 *                 - pOutValue->trimmingFormat == ImageTransferProcessorFormat_320x240
 *                 - pOutValue->trimmingStartX == 0
 *                 - pOutValue->trimmingStartY == 0
 */
void GetImageTransferProcessorDefaultConfig(ImageTransferProcessorExConfig* pOutValue) NN_NOEXCEPT;

/**
 * @brief      IR カメラとイメージ転送プロセッサの処理を開始します。
 *             すでにプロセッサが起動中の場合は、実行中のプロセッサ処理を停止し、イメージ転送プロセッサの処理を再実行します。
 * @param[in]  handle     IR カメラのハンドル
 * @param[in]  config     イメージ転送プロセッサの設定
 * @param[in]  workBuffer イメージ転送プロセッサが利用するワークバッファ
 * @param[in]  size       イメージ転送プロセッサが利用するワークバッファのサイズ
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *             - イメージ転送プロセッサの設定 config について
 *                 - ImageTransferProcessorExposureTimeMin <= config.irCameraConfig.exposureTime
 *                 - config.irCameraConfig.exposureTime <= ImageTransferProcessorExposureTimeMax
 *                 - IrCameraGainMin <= config.irCameraConfig.gain
 *                 - config.irCameraConfig.gain <= IrCameraGainMax
 *                 - workBuffer != nullptr
 *                 - workBuffer が 4 KB 境界でアラインメントされている
 *                 - size について
 *                     - config.format == ImageTransferProcessorFormat_320x240 の場合 size == ImageTransferProcessorWorkBufferSize_320x24
 *                     - config.format == ImageTransferProcessorFormat_160x120 の場合 size == ImageTransferProcessorWorkBufferSize_160x120
 *                     - config.format == ImageTransferProcessorFormat_80x60 の場合 size == ImageTransferProcessorWorkBufferSize_80x60
 *                     - config.format == ImageTransferProcessorFormat_40x30 の場合 size == ImageTransferProcessorWorkBufferSize_40x30
 *                     - config.format == ImageTransferProcessorFormat_20x15 の場合 size == ImageTransferProcessorWorkBufferSize_20x15
 * @post
 *             - 指定のハンドルに対応する IR カメラがイメージ転送プロセッサを利用して処理実行状態
 * @details    指定のハンドルに対応する IR カメラが停止状態の場合は、IR カメラとイメージ転送プロセッサに設定を反映し
 *             モーションIRカメラを処理実行状態にします。
 *
 *             すでに指定のハンドルに対応する、 IR カメラが処理実行状態の場合は、既存のプロセッサを停止して、
 *             IR カメラとイメージ転送プロセッサに設定を反映しモーションIRカメラを再度処理実行状態にします。
 *             既存の処理実行状態のプロセッサの種類は問いません。
 *             処理実行状態の間は IR カメラの撮影とイメージ転送プロセッサによる処理を定期的に実行します。
 *
 *             イメージ転送プロセッサが処理した結果は GetImageTransferProcessorState() で取得できます。
 *             指定したワークバッファはこの関数を呼び出してから StopImageProcessor() 、StopImageProcessorAsync()、
 *             または RunImageTransferProcessor() 以外の Run 関数を呼び出す、または RunImageTransferProcessor() で、
 *             元々指定したものと別のワークバッファを渡すまでアクセスできなくなります。
 */
void RunImageTransferProcessor(const IrCameraHandle& handle, const ImageTransferProcessorConfig& config, void* workBuffer, size_t size) NN_NOEXCEPT;

/**
 * @brief      IR カメラとイメージ転送プロセッサの詳細設定を伴う処理を開始します。
 *             すでにプロセッサが起動中の場合は、実行中のプロセッサ処理を停止し、イメージ転送プロセッサの処理を再実行します。
 * @param[in]  handle     IR カメラのハンドル
 * @param[in]  config     イメージ転送プロセッサの詳細設定
 * @param[in]  workBuffer イメージ転送プロセッサが利用するワークバッファ
 * @param[in]  size       イメージ転送プロセッサが利用するワークバッファのサイズ
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *             - イメージ転送プロセッサの設定 config について
 *                 - ImageTransferProcessorExposureTimeMin <= config.irCameraConfig.exposureTime
 *                 - config.irCameraConfig.exposureTime <= ImageTransferProcessorExposureTimeMax
 *                 - IrCameraGainMin <= config.irCameraConfig.gain
 *                 - config.irCameraConfig.gain <= IrCameraGainMax
 *                 - workBuffer != nullptr
 *                 - workBuffer が 4 KB 境界でアラインメントされている
 *                 - size について
 *                     - config.trimmingFormat == ImageTransferProcessorFormat_320x240 の場合 size == ImageTransferProcessorWorkBufferSize_320x24
 *                     - config.trimmingFormat == ImageTransferProcessorFormat_160x120 の場合 size == ImageTransferProcessorWorkBufferSize_160x120
 *                     - config.trimmingFormat == ImageTransferProcessorFormat_80x60 の場合 size == ImageTransferProcessorWorkBufferSize_80x60
 *                     - config.trimmingFormat == ImageTransferProcessorFormat_40x30 の場合 size == ImageTransferProcessorWorkBufferSize_40x30
 *                     - config.trimmingFormat == ImageTransferProcessorFormat_20x15 の場合 size == ImageTransferProcessorWorkBufferSize_20x15
 *                 - config.origFormat について
 *                     - config.origFormat <= config.trimmingFormat
 *                     - 0 <= config.trimmingStartX
 *                     - config.trimmingStartX <= config.origFormat の幅 - config.trimmingFormat の幅
 *                     - 0 <= config.trimmingStartY
 *                     - config.trimmingStartY <= config.origFormat の高さ - config.trimmingFormat の高さ
 * @post
 *             - 指定のハンドルに対応する IR カメラがイメージ転送プロセッサを利用して処理実行状態
 * @details    指定のハンドルに対応する IR カメラが停止状態の場合は、IR カメラとイメージ転送プロセッサに設定を反映し
 *             モーションIRカメラを処理実行状態にします。
 *
 *             すでに指定のハンドルに対応する、 IR カメラが処理実行状態の場合は、既存のプロセッサを停止して、
 *             IR カメラとイメージ転送プロセッサに設定を反映しモーションIRカメラを再度処理実行状態にします。
 *             既存の処理実行状態のプロセッサの種類は問いません。
 *             処理実行状態の間は IR カメラの撮影とイメージ転送プロセッサによる処理を定期的に実行します。
 *
 *             イメージ転送プロセッサが処理した結果は GetImageTransferProcessorState() で取得できます。
 *             指定したワークバッファはこの関数を呼び出してから StopImageProcessor() 、StopImageProcessorAsync()、
 *             または RunImageTransferProcessor() 以外の Run 関数を呼び出す、または RunImageTransferProcessor() で、
 *             元々指定したものと別のワークバッファを渡すまでアクセスできなくなります。
 */
void RunImageTransferProcessor(const IrCameraHandle& handle, const ImageTransferProcessorExConfig& config, void* workBuffer, size_t size) NN_NOEXCEPT;

/**
 * @brief      イメージ転送プロセッサの最新の処理結果を取得します。
 * @param[out] pOutState 処理結果画像の付加情報を読み出すバッファ
 * @param[out] pOutImage 処理結果画像を読み出すバッファ
 * @param[in]  size      処理結果画像を読み出すバッファのサイズ
 * @param[in]  handle    IR カメラのハンドル
 * @retresult
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceNotReady}
 *   @handleresult{nn::irsensor::ResultIrsensorUnconnected}
 *   @handleresult{nn::irsensor::ResultIrsensorUnsupported}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceError}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceResourceNotAvailable}
 * @endretresult
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *             - 指定のハンドルに対応する IR カメラがイメージ転送プロセッサを利用して処理実行状態
 *             - pOutState != nullptr
 *             - pOutImage != nullptr
 *             - 直近で呼び出した RunImageTransferProcessor() で指定した config.workBufferSize について
 *                 - ImageTransferProcessorFormat_320x240 の場合 size >= ImageTransferProcessorImageSize_320x240
 *                 - ImageTransferProcessorFormat_160x120 の場合 size >= ImageTransferProcessorImageSize_160x120
 *                 - ImageTransferProcessorFormat_80x60 の場合 size >= ImageTransferProcessorImageSize_80x60
 *                 - ImageTransferProcessorFormat_40x30 の場合 size >= ImageTransferProcessorImageSize_40x30
 *                 - ImageTransferProcessorFormat_20x15 の場合 size >= ImageTransferProcessorImageSize_20x15
 * @details    最新の指定のハンドルに対応する IR カメラとイメージ転送プロセッサによる処理結果を取得します。
 *             処理を開始した直後で処理結果がまだない場合や IR カメラが利用可能でない場合には、この関数は ::nn::ResultSuccess 以外を返します。
 *             その場合でも、 IR カメラに何も映っていない状態での処理結果
 *            （0 <= j < 直近で呼び出した RunImageTransferProcessor() で指定した config.workBufferSize において pOutImage[j] = 0）が取得できますが、
 *             ImageTransferProcessorState::samplingNumber は常に 0 になります。
 *
 *             モーションIRカメラの状態が異常である場合は、システムがそのエラー内容を画面に表示します。
 *             エラー表示を行っている間は、呼び出し元をブロックします。
 *             ユーザが操作した後に返り値として nn::irsensor::ResultIrsensorDeviceError を返し、呼び出し元に戻ります。
 *
 *             コントローラのファームウェアに更新が必要な場合は、コントローラサポートアプレットが起動し、アップデートが行われます。
 *             アップデートを行っている間は、呼び出し元をブロックします。
 *             アップデートに成功していた場合は nn::irsensor::ResultIrsensorDeviceNotReady を、失敗していた場合は
 *             nn::irsensor::ResultIrsensorDeviceError を返し、呼び出し元に戻ります。
 *
 *             上記のとおり、本関数は呼び出し元をブロックする可能性があるため、メインループとは別のスレッドから呼び出すことを推奨します。
 *
 *             少なくとも 1 台以上のコントローラで NFP 機能を使用している場合は IR センサーは利用できません。
 *             また、接続されているコントローラが 5 台以上の場合は IR センサーの複数台同時利用はできません。
 *             これらの使用条件を満たさない場合にこの関数を呼び出すと nn::irsensor::ResultIrsensorDeviceResourceNotAvailable を返します。
 */
::nn::Result GetImageTransferProcessorState(ImageTransferProcessorState* pOutState, void* pOutImage, size_t size, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @}
 */

}} // namespace nn::irsensor
