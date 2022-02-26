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
 * @brief IR カメラに関する API の宣言。
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/nn_TimeSpan.h>
#include <nn/hid/hid_Npad.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>

namespace nn { namespace irsensor {

/**
 * @name IR カメラ
 * @{
 */

/**
 * @brief      Npad に搭載された IR カメラを操作するためのハンドルを取得します。
 * @param[in]  npadIdType Npad ID
 * @return     IR カメラのハンドル
 * @pre
 *             - 指定の Npad ID について InitializeNpad() の呼び出しが完了している
 */
IrCameraHandle GetIrCameraHandle(const ::nn::hid::NpadIdType& npadIdType) NN_NOEXCEPT;

/**
 * @brief      IR カメラを初期化します。
 * @param[in]  handle IR カメラのハンドル
 * @pre
 *             - 指定のハンドルで IR カメラが初期化されていない
 * @post
 *             - 指定のハンドルで IR カメラが初期化済み
 */
void Initialize(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      IR カメラの終了処理をします。
 * @param[in]  handle IR カメラのハンドル
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 * @post
 *             - 指定のハンドルで IR カメラが初期化されていない
 * @details    指定のハンドルに対応した、 Npad などのデバイスについてIR カメラの終了処理を行います。
 *             原則、終了処理をする前に nn::irsensor::StopImageProcessor() または nn::irsensor::StopImageProcessorAsync()
 *             を呼び出して、プロセッサを停止しておくことを推奨しますが、プロセッサ動作中に呼び出されていなかった場合は、
 *             終了処理を行う前に内部でプロセッサの停止処理も合わせて行います。
 *             この場合、プロセッサの完了を待つため、本関数は呼び出し元をブロックする可能性があります。
 *             特に、nn::irsensor::StopImageProcessorAsync() を利用する場合は、nn::irsensor::GetImageProcessorStatus() で
 *             プロセッサの停止処理が完了していることを確認したうえで、本関数を呼び出すことを推奨します。
 */
void Finalize(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      IR カメラの状態を取得します。
 * @param[in]  handle IR カメラのハンドル
 * @return     指定のハンドルに対応したデバイスについての IR カメラの状態
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 * @details    指定のハンドルに対応した Npad などのデバイスについて IR カメラの状態を取得します。
 *             この関数で取得した状態によらず RunMomentProcessor() などを呼び出して
 *             モーションIRカメラを処理実行状態にできます。
 *
 *             ただし、イメージプロセッサの処理結果を取得する関数を呼び出したときに IR カメラが利用可能でない場合には、
 *             ::nn::ResultSuccess 以外の ::nn::Result が返されます。そのとき呼び出した関数が GetMomentProcessorState()
 *             など最新の処理結果を取得する関数の場合には samplingNumber, deltaTime が 0 で
 *             IR カメラに何も映っていない状態と同等の処理結果を返します。
 *             GetMomentProcessorStates() など処理結果を過去に遡って取得する関数の場合には、
 *             次に IR カメラが利用可能になるまで処理結果の数として 0 を返します。
 */
IrCameraStatus GetIrCameraStatus(const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @brief      モーションIRカメラを使用するためにコントローラのファームウェア更新が必要かどうかをチェックします。
 * @param[out] pOutIsUpdateNeeded   アップデートが必要かどうかを返すフラグ
 * @param[in]  handle               IR カメラのハンドル
 * @retresult
 *   @handleresult{nn::irsensor::ResultIrsensorUnconnected}
 *   @handleresult{nn::irsensor::ResultIrsensorUnsupported}
 *   @handleresult{nn::irsensor::ResultIrsensorFirmwareCheckIncompleted}
 * @endretresult
 * @pre
 *             - 指定のハンドルで IR カメラが初期化済み
 *             - 指定のハンドルに対応する IR カメラのプロセッサが処理停止状態
 * @details    指定のハンドルに対応したIR カメラが搭載されたコントローラのファームウェアを非同期にチェックします。
 *             値が取得できるまでは少し時間がかかり、その間に呼び出すと、
 *             nn::irsensor::ResultIrsensorFirmwareCheckIncompleted を返しますので、リトライしてください。
 *             また、アプリが BG 状態で呼び出した場合にも同エラーを返しますので、同様にリトライしてください。
 *
 *             ファームウェアが期待されるものより古い、または壊れている場合には、pOutIsUpdateNeeded に true が返ります。
 *             この場合、 nn::hid::ShowControllerFirmwareUpdate を呼び出し、コントローラのアップデートを行ってください。
 *             更新なしに利用可能な場合は、 pOutIsUpdateNeeded に false が返ります。
 *
 *             本関数は、モーションIRカメラを起動する前にアップデートの必要性を確認する機能を提供するもので、
 *             呼び出し必須ではありません。
 *             ファームウェアのアップデートを行わなかった、また行ったが失敗した状態でモーションIRカメラを使用した場合、
 *             各モードの Get 関数によるデータ取得の際に行われるファームウェアチェックでアップデートが行われます。
 */
::nn::Result CheckFirmwareUpdateNecessity(bool* pOutIsUpdateNeeded, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @}
 */

}} // namespace nn::irsensor
