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
 * @brief クラスタリングプロセッサに関する API の宣言。
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/irsensor/irsensor_Result.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_ClusteringProcessorTypes.h>

namespace nn { namespace irsensor {

/**
 * @name クラスタリングプロセッサ
 * @{
 */

/**
 * @brief      クラスタリングプロセッサのデフォルト設定を取得します。
 * @param[out] pOutValue クラスタリングプロセッサのデフォルト設定
 * @pre
 *             - pOutValue != nullptr
 */
void GetClusteringProcessorDefaultConfig(ClusteringProcessorConfig* pOutValue) NN_NOEXCEPT;

/**
 * @brief      IR カメラとクラスタリングプロセッサの処理を開始します。
 *             すでにプロセッサが起動中の場合は、実行中のプロセッサ処理を停止し、クラスタリングプロセッサの処理を再実行します。
 * @param[in]  handle IR カメラのハンドル
 * @param[in]  config クラスタリングプロセッサの設定
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *             - クラスタリングプロセッサの設定 config について
 *                 - ClusteringProcessorExposureTimeMin <= config.irCameraConfig.exposureTime
 *                 - config.irCameraConfig.exposureTime <= ClusteringProcessorExposureTimeMax
 *                 - IrCameraGainMin <= config.irCameraConfig.gain
 *                 - config.irCameraConfig.gain <= IrCameraGainMax
 *                 - 0 <= config.windowOfInterest.x
 *                 - 1 <= config.windowOfInterest.width
 *                 - config.windowOfInterest.x + config.windowOfInterest.width <= IrCameraImageWidth
 *                 - 0 <= config.windowOfInterest.y
 *                 - 1 <= config.windowOfInterest.height
 *                 - config.windowOfInterest.y + config.windowOfInterest.height <= IrCameraImageHeight
 *                 - 0 <= config.objectPixelCountMin
 *                 - config.objectPixelCountMin <= ClusteringProcessorObjectPixelCountMax
 *                 - 0 <= config.objectPixelCountMax
 *                 - config.objectPixelCountMax <= ClusteringProcessorObjectPixelCountMax
 * @post
 *             - 指定のハンドルに対応する IR カメラがクラスタリングプロセッサを利用して処理実行状態
 * @details    指定のハンドルに対応する IR カメラが停止状態の場合は、IR カメラとクラスタリングプロセッサに設定を反映し
 *             モーションIRカメラを処理実行状態にします。
 *
 *             すでに指定のハンドルに対応する、 IR カメラが処理実行状態の場合は、既存のプロセッサを停止して、
 *             IR カメラとクラスタリングプロセッサに設定を反映しモーションIRカメラを再度処理実行状態にします。
 *             既存の処理実行状態のプロセッサの種類は問いません。
 *             処理実行状態の間は IR カメラの撮影とクラスタリングプロセッサによる処理を定期的に実行します。
 *
 *             クラスタリングプロセッサが処理した結果は GetClusteringProcessorState() または GetClusteringProcessorStates() で取得できます。
 */
void RunClusteringProcessor(const IrCameraHandle& handle, const ClusteringProcessorConfig& config) NN_NOEXCEPT;

/**
 * @brief      クラスタリングプロセッサの最新の処理結果を取得します。
 * @param[out] pOutValue 処理結果を読み出すバッファ
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
 *             - 指定のハンドルに対応する IR カメラがクラスタリングプロセッサを利用して処理実行状態
 *             - pOutValue != nullptr
 * @post
 *             - 読み出した処理結果 pOutValue について
 *                 - 0 <= pOutValue->objectCount && pOutValue->objectCount <= ClusteringProcessorObjectCountMax
 *                 - 0 < pOutValue->objects[j].averageIntensity && pOutValue->objects[j].averageIntensity <= IrCameraIntensityMax
 *                 - pOutValue->objects[j].centroid はクラスタリングプロセッサに設定した WOI 内の座標
 *                 - pOutValue->objects[j].pixelCount はクラスタリングプロセッサに設定した検出するオブジェクトサイズの範囲内
 * @details    この関数が ::nn::ResultSuccess を返す場合には GetClusteringProcessorStates() で 1 つの入力状態を読み出したときと同じ値が返ります。
 *             処理を開始した直後で処理結果がまだない場合や IR カメラが利用可能でない場合には、この関数は ::nn::ResultSuccess 以外を返します。
 *             その場合でも、 IR カメラに何も映っていない状態での処理結果（pOutValue->objectCount = 0）が取得できますが、
 *             ClusteringProcessorState::samplingNumber および ClusteringProcessorState::timeStamp は常に 0 になります。
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
::nn::Result GetClusteringProcessorState(ClusteringProcessorState* pOutValue, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      クラスタリングプロセッサの処理結果を過去に遡って取得します。
 * @param[out] pOutStates 処理結果を読み出すバッファ（配列）
 * @param[out] pOutCount  読み出した処理結果の数
 * @param[in]  countMax   読み出す処理結果の最大数
 * @param[in]  handle     IR カメラのハンドル
 * @retresult
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceNotReady}
 *   @handleresult{nn::irsensor::ResultIrsensorUnconnected}
 *   @handleresult{nn::irsensor::ResultIrsensorUnsupported}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceError}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceResourceNotAvailable}
 * @endretresult
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *             - 指定のハンドルに対応する IR カメラがクラスタリングプロセッサを利用して処理実行状態
 *             - pOutStates != nullptr
 *             - countMax >= 1
 * @post
 *             - 読み出した処理結果の数 *pOutCount について 0 <= *pOutCount && *pOutCount <= countMax && *pOutCount <= ClusteringProcessorStateCountMax
 *             - 読み出した処理結果 pOutStates[i] について
 *                 - 0 <= pOutStates[i].objectCount && pOutStates[i].objectCount <= ClusteringProcessorObjectCountMax
 *                 - 0 < pOutStates[i].objects[j].averageIntensity && pOutStates[i].objects[j].averageIntensity <= IrSensorIntensityMax
 *                 - pOutStates[i].objects[j].centroid はクラスタリングプロセッサに設定した WOI 内の座標
 *                 - pOutStates[i].objects[j].pixelCount はクラスタリングプロセッサに設定した検出するオブジェクトサイズの範囲内
 * @details    最新のものから過去に遡って利用可能な数だけ順に、指定のハンドルに対応する IR カメラとクラスタリングプロセッサによる処理結果を取得します。
 *             利用可能な処理結果の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *             読み出し可能な処理結果の最大数は ClusteringProcessorStateCountMax 個です。
 *             利用可能な入力状態には読み出し済みのものも含まれます。
 *             差分だけ利用したい場合は ClusteringProcessorState::samplingNumber を参照してください。
 *             処理を開始した直後で処理結果がまだない場合や IR カメラが利用可能でない場合には、
 *             この関数は ::nn::ResultSuccess 以外を返し、処理結果の数として 0 個を返します。
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
::nn::Result GetClusteringProcessorStates(ClusteringProcessorState* pOutStates, int* pOutCount, int countMax, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @}
 */

}} // namespace nn::irsensor
