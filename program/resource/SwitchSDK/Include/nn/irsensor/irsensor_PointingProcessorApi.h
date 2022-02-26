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
 * @brief Pointing プロセッサに関する API の宣言。
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/irsensor/irsensor_Result.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_PointingProcessorTypes.h>

namespace nn { namespace irsensor {

/**
 * @name Pointing プロセッサ
 * @{
 */

/**
 * @brief      IR カメラと Pointing プロセッサの処理を開始します。
 *             すでにプロセッサが起動中の場合は、実行中のプロセッサ処理を停止し、Pointing プロセッサの処理を再実行します。
 * @param[in]  handle IR カメラのハンドル
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *
 * @post
 *             - 指定のハンドルに対応する IR カメラが Pointing プロセッサを利用して処理実行状態
 *
 * @details    指定のハンドルに対応する IR カメラとプロセッサが停止状態の場合は、IR カメラと Pointingプロセッサに設定を反映し
 *             モーションIRカメラを処理実行状態にします。
 *
 *             すでに指定のハンドルに対応する、 IR カメラが処理実行状態の場合は、既存のプロセッサを停止して、
 *             IR カメラと Pointing プロセッサに設定を反映しモーションIRカメラを再度処理実行状態にします。
 *             既存の処理実行状態のプロセッサの種類は問いません。
 *             処理実行状態の間は IR カメラの撮影と Pointing プロセッサによる処理を定期的に実行します。
 *
 *             Pointing プロセッサが処理した結果は GetPointingProcessorStates() で取得できます。
 */
void RunPointingProcessor(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      Pointing プロセッサの処理結果を過去に遡って取得します。
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
 *             - 指定のハンドルに対応する IR カメラが Pointing プロセッサを利用して処理実行状態
 *             - pOutStates != nullptr
 *             - countMax >= 1
 * @post
 *             - 読み出した処理結果の数 *pOutCount について 0 <= *pOutCount && *pOutCount <= countMax && *pOutCount <= PointingProcessorStateCountMax
 *                 - pOutStates[i].position は正規化された座標データ
 *
 * @details    最新のものから過去に遡って利用可能な数だけ順に、指定のハンドルに対応する IR カメラと Pointing プロセッサによる処理結果を取得します。
 *             利用可能な処理結果の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *             読み出し可能な処理結果の最大数は PointingProcessorStateCountMax 個です。
 *             利用可能な入力状態には読み出し済みのものも含まれます。
 *             差分だけ利用したい場合は PointingProcessorState::samplingNumber を参照してください。
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
::nn::Result GetPointingProcessorStates(PointingProcessorState* pOutStates, int* pOutCount, int countMax, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @}
 */

}} // namespace nn::irsensor
