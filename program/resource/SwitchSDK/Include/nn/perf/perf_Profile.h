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
* @brief   Cpuおよび Gpu の負荷計測の結果を集計する API の宣言。
*/

#pragma once
#include <nn/perf/perf_LoadMeter.h>
#include <nn/util/util_PlacementArray.h>

#if defined( NN_PERF_PROFILE_ENABLED )

//! @name 初期化/終了
//! @{

/**
*  @brief LoadMeterCenter を初期化します。
*  @param[in] pDevice デバイスへのポインタです。
*  @param[in] loadMeterCenterInfo 初期化用の情報です。
*  @param[in] pMemory 初期用のバッファへのポインタ
*  @param[in] memorySize バイトでのバッファサイズ
*  @param[in] pMemoryPool Gpuタイムスタンプが書き込まれるメモリプールへのポインタ
*  @param[in] memoryPoolOffset Gpuタイムスタンプが書き込まれるメモリプールへのオフセット
*  @param[in] memoryPoolSize Gpuタイムスタンプが書き込まれるメモリプールのサイズ
*  @pre
*  - pDevice != NULL
*  - pDevice が初期化されている。
*  - info に GpuMeter の計測区間数が 1 以上の値で設定されている。
*  @details 以下のメーターを作成します。
*           - フレーム計測用 CpuMeter : 1つ
*           - コア計測用     CpuMeter : LoadMeterCenterInfo で指定された数
*           - ユーザー計測用 CpuMeter : LoadMeterCenterInfo で指定された数
*           - GPU 計測用     GpuMeter : 1つ
*  @n       フレーム計測用 CpuMeter にすべての CpuMeter と GpuMeter がアタッチされます。
*/
#define NN_PERF_INITIALIZE_METER( pDevice, loadMeterCenterInfo, pMemory, memorySize, pMemoryPool, memoryPoolOffset, memoryPoolSize ) nn::perf::LoadMeterCenter::Initialize( pDevice, loadMeterCenterInfo, pMemory, memorySize, pMemoryPool, memoryPoolOffset, memoryPoolSize )

/**
*  @brief LoadMeterCenter を初期化します。GPU 計測は行いません。
*  @param[in] loadMeterCenterInfo 初期化用の情報です。
*  @param[in] pMemory 初期用のバッファへのポインタ
*  @param[in] memorySize バイトでのバッファサイズ
*  @pre
*  - info に CpuMeter の計測区間数が 1 以上の値で設定されている。
*  @details 以下のメーターを作成します。
*           - フレーム計測用 CpuMeter : 1つ
*           - コア計測用     CpuMeter : LoadMeterCenterInfo で指定された数
*           - ユーザー計測用 CpuMeter : LoadMeterCenterInfo で指定された数
*  @n       フレーム計測用 CpuMeter にすべての CpuMeter がアタッチされます。
*  @n       info の GpuMeter の計測区間数は無視されます。
*/
#define NN_PERF_INITIALIZE_METER_CPU_ONLY(loadMeterCenterInfo, pMemory, memorySize) nn::perf::LoadMeterCenter::Initialize(loadMeterCenterInfo, pMemory, memorySize)

/**
* @brief LoadMeterCenter の終了処理を行います。
* @param[in] pDevice デバイスへのポインタです。
* @details NN_PERF_INITIALIZE_METER を使用して初期化した場合はこのマクロを使って終了してください。
*/
#define NN_PERF_FINALIZE_METER(pDevice) nn::perf::LoadMeterCenter::Finalize(pDevice)

/**
* @brief LoadMeterCenter の終了処理を行います。GpuMeter を初期化していない場合はこのマクロで終了処理をします。
* @details NN_PERF_INITIALIZE_METER_CPU_ONLY、NN_PERF_INITIALIZE_METER_CPU_ONLY_NO_INFO を使用して初期化した場合はこのマクロを使って終了してください。
*/
#define NN_PERF_FINALIZE_METER_CPU_ONLY() nn::perf::LoadMeterCenter::Finalize()

/**
* @brief        初期化に必要なバッファのアラインメントを返します。
* @return       バッファのアラインメントを返します。
*/
#define NN_PERF_GET_BUFFER_ALIGNMENT() nn::perf::LoadMeterCenter::GetBufferAlignment()

/**
* @brief        初期化に必要なバッファのサイズを返します。
* @param[in]    info 初期化用の情報です。
* @return       バッファのサイズを返します。
*/
#define NN_PERF_GET_BUFFER_SIZE(info) nn::perf::LoadMeterCenter::CalculateBufferSize(info)

/**
* @brief        初期化時にメモリプール上にクエリバッファを確保するためのアラインメントを返します。
* @param[in]    pDevice デバイスへのポインタです。
* @param[in]    info 初期化用の情報です。
* @return       メモリプールのアラインメントを返します。
*/
#define NN_PERF_GET_MEMORY_POOL_ALIGNMENT(pDevice, info) nn::perf::LoadMeterCenter::GetMemoryPoolAlignment(pDevice, info)

/**
* @brief        初期化時にクエリバッファを確保するメモリプール上のサイズを返します。
* @param[in]    pDevice デバイスへのポインタです。
* @param[in]    info 初期化用の情報です。
* @return       メモリプールのサイズを返します。
*/
#define NN_PERF_GET_MEMORY_POOL_SIZE(pDevice, info) nn::perf::LoadMeterCenter::CalculateMemoryPoolSize(pDevice, info)

/**
* @brief  現在のスレッドが動作するコア ID を返す関数を登録します。
* @param[in]  getCoreNumberFunction コア番号を返す関数です。
*/
#define NN_PERF_SET_GET_CORE_NUMBER_FUNCTION(getCoreNumberFunction) nn::perf::LoadMeterCenter::SetGetCoreNumberFunction(getCoreNumberFunction)

//! @}
//! @name 有効化 / 無効化
//! @{

/**
* @brief        計測と集計処理の有効・無効を設定します。
* @param[in]    isEnabled   false を指定した場合、計測と集計処理を行いません。
* @details      必ず NN_PERF_END_FRAME と NN_PERF_BEGIN_FRAME の間 (計測を行わない区間) で呼んでください。
*/
#define NN_PERF_SET_ENABLED(isEnabled) nn::perf::LoadMeterCenter::SetEnabled(isEnabled)

/**
* @brief        計測と集計処理の有効・無効を取得します。
* @return       計測と集計処理が有効ならば true を返します。
*/
#define NN_PERF_IS_ENABLED() nn::perf::LoadMeterCenter::IsEnabled()

//! @}
//! @name 設定
//! @{

/**
* @brief        ユーザー計測用の CpuMeter を指定して名前を設定します。
* @param[in]    index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    meterName メーターの名前です。
* @details      コア計測用の CpuMeter の名前は変更出来ません。
*/
#define NN_PERF_SET_METER_NAME(index, meterName) nn::perf::LoadMeterCenter::SetMeterName(index, meterName)

//! @}
//! @name 基準フレームレート設定
//! @{

/**
* @brief        ログ表示のために用いるフレームレートを設定します。
* @param[in]    frameRate 設定するフレームレートです。
*/
#define NN_PERF_SET_FRAME_RATE(frameRate) nn::perf::LoadMeterCenter::SetFrameRate(frameRate)

//! @}
//! @name フレーム時間計測
//! @{

/**
* @brief  frameMeter を使用してフレームの開始時間を記録します。フレーム開始時に呼んでください。
*/
#define NN_PERF_BEGIN_FRAME() nn::perf::LoadMeterCenter::BeginFrameMeasure()

/**
* @brief  frameMeter を使用してフレームの終了時間を記録します。フレーム終了時に呼んでください。
* @details 以下の操作を行います。
*  - フレーム時間の計測の終了
*  - 集計結果の初期化。
*  - 各 CpuMeter と GpuMeter の計測結果の集計とバッファの切り替え。
*  - LoadMeterCenter による計測結果の集計。
*/
#define NN_PERF_END_FRAME() nn::perf::LoadMeterCenter::EndFrameMeasure(true)

/**
* @brief  frameMeter を使用してフレームの終了時間を記録します。フレーム終了時に呼んでください。
* @details 内部では前回の集計結果はクリアしません。結果を蓄積します。
* @n       集計結果のクリアを行いたい場合は、内部で結果のクリアを行う NN_PERF_END_FRAME() を使用するか、任意の場所で NN_PERF_CLEAR() を呼んでください。
* @n 以下の操作を行います。
*  - フレーム時間の計測の終了
*  - 各 CpuMeter と GpuMeter の計測結果の集計とバッファの切り替え。
*  - LoadMeterCenter による計測結果の集計。
*/
#define NN_PERF_END_FRAME_NO_CLEAR_PROFILE() nn::perf::LoadMeterCenter::EndFrameMeasure(false)

//! @}
//! @name CPU 計測（コア計測）
//! @{

/**
* @brief  動作しているコアに対応した CpuMeter で計測開始の時間を記録します。
* @details  計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
* @n      NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE() nn::perf::LoadMeterCenter::BeginMeasure(NULL, 0)

/**
* @brief  動作しているコアに対応したメーターで計測開始の時間を記録します。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @details  計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
* @n      NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_NAME(sectionName) nn::perf::LoadMeterCenter::BeginMeasure(sectionName, 0)

/**
* @brief  動作しているコアに対応したメーターで計測開始の時間を記録します。
* @param[in]    tag 計測する区間に設定する任意のタグです。
* @details  計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
* @n      NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_TAG(tag) nn::perf::LoadMeterCenter::BeginMeasure(NULL, tag)

/**
* @brief  動作しているコアに対応したメーターで計測開始の時間を記録します。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @param[in]    tag 計測する区間に設定する任意のタグです。
* @details  計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
* @n      NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_NAME_AND_TAG(sectionName, tag) nn::perf::LoadMeterCenter::BeginMeasure(sectionName, tag)

/**
* @brief  動作しているコアに対応したメーターで計測終了の時間を記録します。
*/
#define NN_PERF_END_MEASURE() nn::perf::LoadMeterCenter::EndMeasure()

/**
* @brief  動作しているコアに対応したメーターで計測をします。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n       計測終了のマクロを呼ぶ必要はありません。
* @n       計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
*/
#define NN_PERF_AUTO_MEASURE() nn::perf::AutoMeasure autoMeasure(NULL, 0)

/**
* @brief  動作しているコアに対応したメーターで計測をします。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
*/
#define NN_PERF_AUTO_MEASURE_NAME(sectionName) nn::perf::AutoMeasure autoMeasure( sectionName, 0 )

/**
* @brief  動作しているコアに対応したメーターで計測をします。
* @param[in]    tag 計測する区間に設定するタグです。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
*/
#define NN_PERF_AUTO_MEASURE_TAG(tag) nn::perf::AutoMeasure autoMeasure( NULL, tag )

/**
* @brief  動作しているコアに対応したメーターで計測をします。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @param[in]    tag 計測する区間に設定するタグです。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      計測区間を処理するコアが固定されている場合に限りこのマクロを使用することができます。
*/
#define NN_PERF_AUTO_MEASURE_NAME_AND_TAG(sectionName, tag) nn::perf::AutoMeasure autoMeasure( sectionName, tag )

//! @}
//! @name CPU 計測（ユーザー指定）
//! @{

/**
* @brief  指定したユーザー計測用 CpuMeter で計測開始の時間を記録します。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @details NN_PERF_END_MEASURE_INDEX マクロと対になっている必要があります。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_BEGIN_MEASURE_INDEX(index) nn::perf::LoadMeterCenter::BeginMeasureIndex( index, NULL, 0 )

/**
* @brief  指定したユーザー計測用 CpuMeter で計測開始の時間を記録します。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]   sectionName   計測する区間に設定する区間名です。
* @details NN_PERF_END_MEASURE_INDEX マクロと対になっている必要があります。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_BEGIN_MEASURE_INDEX_NAME(index, sectionName) nn::perf::LoadMeterCenter::BeginMeasureIndex( index, sectionName, 0 )

/**
* @brief  指定したユーザー計測用 CpuMeter で計測開始の時間を記録します。
* @param[in]    index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    tag 計測する区間に設定する任意のタグです。
* @details NN_PERF_END_MEASURE_INDEX マクロと対になっている必要があります。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_BEGIN_MEASURE_INDEX_TAG(index, tag) nn::perf::LoadMeterCenter::BeginMeasureIndex(index, NULL, tag)

/**
* @brief  指定したユーザー計測用 CpuMeter で計測開始の時間を記録します。
* @param[in]   index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @param[in]    tag 計測する区間に設定する任意のタグです。
* @details NN_PERF_END_MEASURE_INDEX マクロと対になっている必要があります。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_BEGIN_MEASURE_INDEX_NAME_AND_TAG(index, sectionName, tag) nn::perf::LoadMeterCenter::BeginMeasureIndex(index, sectionName, tag)

/**
* @brief  指定したユーザー計測用 CpuMeter で計測終了の時間を記録します。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
*/
#define NN_PERF_END_MEASURE_INDEX(index) nn::perf::LoadMeterCenter::EndMeasureIndex( index )

/**
* @brief  指定したユーザー計測用 CpuMeter で自動的に計測を行います。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_AUTO_MEASURE_INDEX(index) nn::perf::AutoMeasureIndex autoMeasureIndex(index, NULL, 0)

/**
* @brief  指定したユーザー計測用 CpuMeter で自動的に計測を行います。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_AUTO_MEASURE_INDEX_NAME(index, sectionName) nn::perf::AutoMeasureIndex autoMeasureIndex(index, sectionName, 0)

/**
* @brief  指定したユーザー計測用 CpuMeter で自動的に計測を行います。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    tag 計測する区間に設定するタグです。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_AUTO_MEASURE_INDEX_TAG(index, tag) nn::perf::AutoMeasureIndex autoMeasureIndex(index, NULL, tag )

/**
* @brief  指定したユーザー計測用 CpuMeter で自動的に計測を行います。
* @param[in]   index  保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @param[in]    tag 計測する区間に設定するタグです。
* @details マクロによってコンストラクタが呼ばれて計測が開始され、対応するデストラクタが呼ばれたときに自動的に計測が終了します。
* @n      計測終了のマクロを呼ぶ必要はありません。
* @n      動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
* @n      同時に別々のコアで同じ CpuMeter を指定して計測すると、正常な結果が得られない場合があります。
*/
#define NN_PERF_AUTO_MEASURE_INDEX_NAME_AND_TAG(index, sectionName, tag) nn::perf::AutoMeasureIndex autoMeasureIndex(index, sectionName, tag )

//! @}
//! @name GPU 計測
//! @{

/**
* @brief  GpuMeter で計測開始の時間を記録します。
* @param[in]   pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
* @details  NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_GPU(pCommandBuffer) nn::perf::LoadMeterCenter::BeginMeasureGpu(pCommandBuffer, NULL, 0)

/**
* @brief  GpuMeter で計測開始の時間を記録します。
* @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @details NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_NAME_GPU(pCommandBuffer, sectionName) nn::perf::LoadMeterCenter::BeginMeasureGpu(pCommandBuffer, sectionName, 0)

/**
* @brief  GpuMeter で計測開始の時間を記録します。
* @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
* @param[in]    tag 計測する区間に設定する任意のタグです。
* @details NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_TAG_GPU(pCommandBuffer, tag) nn::perf::LoadMeterCenter::BeginMeasureGpu(pCommandBuffer, NULL, tag)

/**
* @brief  GpuMeter で計測開始の時間を記録します。
* @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
* @param[in]    sectionName 計測する区間に設定する名前です。
* @param[in]    tag 計測する区間に設定する任意のタグです。
* @details NN_PERF_END_MEASURE マクロと対になっている必要があります。
*/
#define NN_PERF_BEGIN_MEASURE_NAME_AND_TAG_GPU(pCommandBuffer, sectionName, tag) nn::perf::LoadMeterCenter::BeginMeasureGpu(pCommandBuffer, sectionName, tag)

/**
* @brief  GpuMeter で計測終了の時間を記録します。
* @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
*/
#define NN_PERF_END_MEASURE_GPU(pCommandBuffer) nn::perf::LoadMeterCenter::EndMeasureGpu(pCommandBuffer)

//! @}
//! @name 表示色変更
//! @{

/**
* @brief 実行中のコアに対応した CpuMeter の色を変更します。
* @param[in]    color 変更する色です。
*/
#define NN_PERF_SET_COLOR(color) nn::perf::LoadMeterCenter::SetColor( color )

/**
* @brief ユーザー計測用 CpuMeter の色を変更します。
* @param[in]    index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @param[in]    color 変更する色です。
*/
#define NN_PERF_SET_COLOR_INDEX(index, color) nn::perf::LoadMeterCenter::SetColorIndex( index, color )

/**
* @brief GpuMeter の色を変更します。
* @param[in]    color 変更する色です。
*/
#define NN_PERF_SET_COLOR_GPU(color) nn::perf::LoadMeterCenter::SetColorGpu( color )

//! @}

/**
* @brief  蓄積された集計結果をクリアします。
* @details NN_PERF_BEGIN_FRAME_NO_CLEAR_PROFILE() を使用して計測を行う場合には任意のタイミングで呼んでください。
*/
#define NN_PERF_CLEAR() nn::perf::LoadMeterCenter::Clear()

/**
* @brief   最新の Next() で集計された後の累計結果をコンソール出力します。
* @details リリースビルド時もログが出力されます。
*/
#define NN_PERF_DUMP() nn::perf::LoadMeterCenter::Dump()



//! @name 計測オブジェクト取得
//! @{

/**
* @brief  フレームを計測する CpuMeter のポインタを返します。
* @return  LoadMeterCenter が管理するフレームを計測する CpuMeter へのポインタ返します。
*/
#define NN_PERF_GET_FRAME_METER() nn::perf::LoadMeterCenter::GetFrameMeter()

/**
* @brief  CPU コア別計測用 CpuMeter のポインタを取得します。
* @param[in] index 「0 <= index < CoreCount」 の範囲で CpuMeter をインデクス指定します。
* @return  指定されたコア計測用 CpuMeter のポインタを返します。
*/
#define NN_PERF_GET_CORE_METER(index) nn::perf::LoadMeterCenter::GetCpuCoreMeter(index)

/**
* @brief  ユーザー計測用 CpuMeter のポインタを取得します。
* @param[in] index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
* @return  指定されたユーザー計測用 CpuMeter のポインタを返します。
*/
#define NN_PERF_GET_USER_METER(index) nn::perf::LoadMeterCenter::GetCpuUserMeter(index)

/**
* @brief  GpuMeter のポインタを取得します。
* @return  GpuMeter のポインタを返します。
*/
#define NN_PERF_GET_GPU_METER() nn::perf::LoadMeterCenter::GetGpuMeter()

//! @}
//! @name CPU 集計結果取得
//! @{

/**
* @brief 指定したセクションが呼ばれた回数を返します。
* @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
*            NULL を指定した場合はすべてのメーターを検索対象とします。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  指定したセクションが呼ばれた回数を返します。
*/
#define NN_PERF_GET_CALL_COUNT_CPU(meterName, sectionName, tag) nn::perf::LoadMeterCenter::GetCallCount(nn::perf::UnitType_Cpu, meterName, sectionName, tag)

/**
* @brief 指定したセクション内の所要時間の合計を nn::TimeSpan で返します。
* @param[in] meterName 指定する CpuMHeter に対応するコア名もしくはメーター名を指定します。
*            NULL を指定した場合はすべてのメーターを検索対象とします。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の合計を返します。
*/
#define NN_PERF_GET_ELAPSED_TIME_CPU(meterName, sectionName, tag) nn::perf::LoadMeterCenter::GetElapsedTime(nn::perf::UnitType_Cpu, meterName, sectionName, tag)

/**
* @brief 指定したセクションの所要時間の最大値を nn::TimeSpan で返します。
* @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
*            NULL を指定した場合はすべてのメーターを検索対象とします。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の最大値を返します。
*/
#define NN_PERF_GET_MAX_ELAPSED_TIME_CPU(meterName, sectionName, tag) nn::perf::LoadMeterCenter::GetMaxElapsedTime(nn::perf::UnitType_Cpu, meterName, sectionName, tag)

/**
* @brief 指定したセクションの所要時間の最小値を nn::TimeSpan で返します。
* @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
*            NULL を指定した場合はすべてのメーターを検索対象とします。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の最小値を返します。
*/
#define NN_PERF_GET_MIN_ELAPSED_TIME_CPU(meterName, sectionName, tag) nn::perf::LoadMeterCenter::GetMinElapsedTime(nn::perf::UnitType_Cpu, meterName, sectionName, tag)

/**
* @brief 指定したセクションの所要時間の平均値を nn::TimeSpan で返します。
* @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
*            NULL を指定した場合はすべてのメーターを検索対象とします。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の平均値を返します。
*/
#define NN_PERF_GET_AVERAGE_ELAPSED_TIME_CPU(meterName, sectionName, tag) nn::perf::LoadMeterCenter::GetAverageElapsedTime(nn::perf::UnitType_Cpu, meterName, sectionName, tag)

//! @}
//! @name GPU 集計結果取得
//! @{

/**
* @brief 指定したセクションが呼ばれた回数を返します。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  指定したセクションが呼ばれた回数を返します。
*/
#define NN_PERF_GET_CALL_COUNT_GPU(sectionName, tag) nn::perf::LoadMeterCenter::GetCallCount(nn::perf::UnitType_Gpu, "GPU", sectionName, tag)

/**
* @brief 指定したセクション内の所要時間の合計を nn::TimeSpan で返します。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の合計を返します。
*/
#define NN_PERF_GET_ELAPSED_TIME_GPU( sectionName, tag) nn::perf::LoadMeterCenter::GetElapsedTime(nn::perf::UnitType_Gpu, "GPU", sectionName, tag)

/**
* @brief 指定したセクションの所要時間の最大値を nn::TimeSpan で返します。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の最大値を返します。
*/
#define NN_PERF_GET_MAX_ELAPSED_TIME_GPU(sectionName, tag) nn::perf::LoadMeterCenter::GetMaxElapsedTime(nn::perf::UnitType_Gpu, "GPU", sectionName, tag)

/**
* @brief 指定したセクションの所要時間の最小値を nn::TimeSpan で返します。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の最小値を返します。
*/
#define NN_PERF_GET_MIN_ELAPSED_TIME_GPU(sectionName, tag) nn::perf::LoadMeterCenter::GetMinElapsedTime(nn::perf::UnitType_Gpu, "GPU", sectionName, tag)

/**
* @brief 指定したセクションの所要時間の平均値を nn::TimeSpan で返します。
* @param[in] sectionName 指定するセクション名です。
*            NULL を指定した場合は名前は無視します。
* @param[in] tag 指定するタグです。
*            0 を指定した場合はタグは無視します。
* @return  所要時間の平均値を返します。
*/
#define NN_PERF_GET_AVERAGE_ELAPSED_TIME_GPU(sectionName, tag) nn::perf::LoadMeterCenter::GetAverageElapsedTime(nn::perf::UnitType_Gpu, "GPU", sectionName, tag)

//! @}

#else

#define NN_PERF_INITIALIZE_METER( pDevice, info, pMemory, memorySize, pMemoryPool, memoryPoolOffset, memoryPoolSize ) (void)0
#define NN_PERF_INITIALIZE_METER_CPU_ONLY(loadMeterCenterInfo, pMemory, memorySize) (void)0
#define NN_PERF_FINALIZE_METER(pDevice) (void)0
#define NN_PERF_FINALIZE_METER_CPU_ONLY() (void)0

#define NN_PERF_GET_BUFFER_ALIGNMENT() (size_t)1
#define NN_PERF_GET_BUFFER_SIZE(info) (size_t)0
#define NN_PERF_GET_MEMORY_POOL_ALIGNMENT(pDevice, info) (size_t)1
#define NN_PERF_GET_MEMORY_POOL_SIZE(pDevice, info) (size_t)0

#define NN_PERF_SET_ENABLED(isEnabled) (void)0
#define NN_PERF_IS_ENABLED() false
#define NN_PERF_SET_GET_CORE_NUMBER_FUNCTION(getCoreNumberFunction) (void)0

#define NN_PERF_SET_METER_NAME(index, meterName) (void)0
#define NN_PERF_SET_FRAME_RATE(frameRate) (void)0

#define NN_PERF_BEGIN_FRAME() (void)0
#define NN_PERF_END_FRAME() (void)0
#define NN_PERF_END_FRAME_NO_CLEAR_PROFILE() (void)0

#define NN_PERF_BEGIN_MEASURE() (void)0
#define NN_PERF_BEGIN_MEASURE_NAME(sectionName) (void)0
#define NN_PERF_BEGIN_MEASURE_TAG(tag) (void)0
#define NN_PERF_BEGIN_MEASURE_NAME_AND_TAG(sectionName, tag) (void)0
#define NN_PERF_END_MEASURE() (void)0
#define NN_PERF_AUTO_MEASURE() (void)0
#define NN_PERF_AUTO_MEASURE_NAME(sectionName) (void)0
#define NN_PERF_AUTO_MEASURE_TAG(tag) (void)0
#define NN_PERF_AUTO_MEASURE_NAME_AND_TAG(sectionName, tag) (void)0

#define NN_PERF_BEGIN_MEASURE_INDEX(index) (void)0
#define NN_PERF_BEGIN_MEASURE_INDEX_NAME(index, sectionName) (void)0
#define NN_PERF_BEGIN_MEASURE_INDEX_TAG(index, tag) (void)0
#define NN_PERF_BEGIN_MEASURE_INDEX_NAME_AND_TAG(index, sectionName, tag) (void)0
#define NN_PERF_END_MEASURE_INDEX(index) (void)0
#define NN_PERF_AUTO_MEASURE_INDEX(index) (void)0
#define NN_PERF_AUTO_MEASURE_INDEX_NAME(index, sectionName) (void)0
#define NN_PERF_AUTO_MEASURE_INDEX_TAG(index, tag) (void)0
#define NN_PERF_AUTO_MEASURE_INDEX_NAME_AND_TAG(index, sectionName, tag) (void)0

#define NN_PERF_BEGIN_MEASURE_GPU(pCommandBuffer) (void)0
#define NN_PERF_BEGIN_MEASURE_NAME_GPU(pCommandBuffer, sectionName) (void)0
#define NN_PERF_BEGIN_MEASURE_TAG_GPU(pCommandBuffer, tag) (void)0
#define NN_PERF_BEGIN_MEASURE_NAME_AND_TAG_GPU(pCommandBuffer, sectionName, tag) (void)0
#define NN_PERF_END_MEASURE_GPU(pCommandBuffer) (void)0

#define NN_PERF_SET_COLOR(color) (void)0
#define NN_PERF_SET_COLOR_INDEX(index, color) (void)0
#define NN_PERF_SET_COLOR_GPU(color) (void)0

#define NN_PERF_CLEAR() (void)0
#define NN_PERF_DUMP() (void)0

#define NN_PERF_GET_FRAME_METER() (nn::perf::CpuMeter*) NULL
#define NN_PERF_GET_CORE_METER(index) (nn::perf::CpuMeter*) NULL
#define NN_PERF_GET_USER_METER(index) (nn::perf::CpuMeter*) NULL
#define NN_PERF_GET_GPU_METER() (nn::perf::GpuMeter*) NULL

#define NN_PERF_GET_CALL_COUNT_CPU(meterName, sectionName, tag) (int)0
#define NN_PERF_GET_ELAPSED_TIME_CPU(meterName, sectionName, tag) (nn::TimeSpan)0
#define NN_PERF_GET_MAX_ELAPSED_TIME_CPU(meterName, sectionName, tag) (nn::TimeSpan)0
#define NN_PERF_GET_MIN_ELAPSED_TIME_CPU(meterName, sectionName, tag) (nn::TimeSpan)0
#define NN_PERF_GET_AVERAGE_ELAPSED_TIME_CPU(meterName, sectionName, tag) (nn::TimeSpan)0

#define NN_PERF_GET_CALL_COUNT_GPU(sectionName, tag) (int)0
#define NN_PERF_GET_ELAPSED_TIME_GPU( sectionName, tag) (nn::TimeSpan)0
#define NN_PERF_GET_MAX_ELAPSED_TIME_GPU(sectionName, tag) (nn::TimeSpan)0
#define NN_PERF_GET_MIN_ELAPSED_TIME_GPU(sectionName, tag) (nn::TimeSpan)0
#define NN_PERF_GET_AVERAGE_ELAPSED_TIME_GPU(sectionName, tag) (nn::TimeSpan)0

#endif

namespace nn
{
namespace perf
{
class AutoMeasure;
class LoadMeterCenter;

//! @brief Cpu と Gpu を指定する定義です。
enum UnitType
{
    UnitType_Cpu, //!< Cpu を表します。
    UnitType_Gpu  //!< Gpu を表します。
};

/**
* @brief LoadMeterCenter クラスの情報を表すクラスです。
*        LoadMeterCenter クラスの初期化時に引数として渡します。
*/
class LoadMeterCenterInfo
{
public:
    /**
    * @brief コンストラクタです。
    */
    LoadMeterCenterInfo() NN_NOEXCEPT
    : m_CoreCount( 0 ),
      m_UserMeterCount( 0 ),
      m_CpuSectionCountMax( 0 ),
      m_GpuSectionCountMax( 0 ),
      m_CpuBufferCount( 2 ),
      m_GpuBufferCount( 2 )
    {}

    /**
    * @brief 各パラメータを既定値に設定するためのヘルパー関数です。
    */
    void SetDefault() NN_NOEXCEPT
    {
        this->SetCoreCount(0);
        this->SetUserMeterCount(0);
        this->SetCpuSectionCountMax(0);
        this->SetGpuSectionCountMax(0);
        this->SetCpuBufferCount( 2 );
        this->SetGpuBufferCount( 2 );
    };

    //! @name 設定
    //! @{

    /**
    * @brief 現在の環境でアプリケーションが動作する CPU コア数を設定します。
    * @param[in] coreCount CPU コア数です。
    * @details 設定した個数分のコア別に計測を行う CpuMeter が作成されます。
    * @n       正常な GPU 計測を行うためにも CPU コア数の設定が必要です。
    */
    void SetCoreCount( int coreCount ) NN_NOEXCEPT
    {
        this->m_CoreCount = coreCount;
    }

    /**
    * @brief ユーザー指定計測用 CpuMeter の数を設定します。
    * @param[in] meterCount ユーザー指定計測用 CpuMeter の個数です。
    */
    void SetUserMeterCount( int meterCount ) NN_NOEXCEPT
    {
        this->m_UserMeterCount = meterCount;
    }

    /**
    * @brief CpuMeter の計測区間の上限数を設定します。
    * @param[in] sectionCountMax 計測区間の上限数です。
    */
    void SetCpuSectionCountMax( int sectionCountMax ) NN_NOEXCEPT
    {
        this->m_CpuSectionCountMax = sectionCountMax;
    }

    /**
    * @brief GpuMeter の計測区間の上限数を設定します。
    * @param[in] sectionCountMax 計測区間の上限数です。
    */
    void SetGpuSectionCountMax( int sectionCountMax ) NN_NOEXCEPT
    {
        this->m_GpuSectionCountMax = sectionCountMax;
    }

    /**
    * @brief CpuMeter の計測結果保存用のバッファの数を指定します。何フレーム前までの計測結果を保持するかを決定します。
    * @param[in] bufferCount 計測結果を保存する配列バッファの数です。ダブルバッファとなる 2以上を指定する必要があります。
    */
    void SetCpuBufferCount( int bufferCount ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(bufferCount > 1, "bufferCount must be more than 2");
        this->m_CpuBufferCount = bufferCount;
    }

    /**
    * @brief GpuMeter の計測結果保存用のバッファの数を指定します。何フレーム前までの計測結果を保持するかを決定します。
    * @param[in] bufferCount 計測結果を保存する配列バッファの数です。ダブルバッファとなる 2以上を指定する必要があります。
    */
    void SetGpuBufferCount( int bufferCount ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(bufferCount > 1, "bufferCount must be more than 2");
        this->m_GpuBufferCount = bufferCount;
    }

    //! @}
    //! @name 取得
    //! @{

    /**
    * @brief コアに対応した CpuMeter の数を取得します。
    * @return 作成するコアに対応した CpuMeter の個数が返ります。
    */
    int GetCoreCount() NN_NOEXCEPT
    {
        return this->m_CoreCount;
    }

    /**
    * @brief ユーザー指定計測用 CpuMeter の数を取得します。
    * @return  作成する CpuMeter の個数が返ります。
    */
    int GetUserMeterCount() NN_NOEXCEPT
    {
        return this->m_UserMeterCount;
    }

    /**
    * @brief CpuMeter の計測区間の上限数を取得します。
    * @return CpuMeter の計測区間の上限数が返ります。
    */
    int GetCpuSectionCountMax() NN_NOEXCEPT
    {
        return this->m_CpuSectionCountMax;
    }

    /**
    * @brief GpuMeter の計測区間の上限数を取得します。
    * @return GpuMeter の計測区間の上限数が返ります。
    */
    int GetGpuSectionCountMax() NN_NOEXCEPT
    {
        return this->m_GpuSectionCountMax;
    }

    /**
    * @brief CpuMeter の計測結果保存用のバッファの数を取得します。
    * @return CpuMeter の計測結果保存用のバッファの数が返ります。
    */
    int GetCpuBufferCount() NN_NOEXCEPT
    {
        return this->m_CpuBufferCount;
    }

    /**
    * @brief GpuMeter の計測結果保存用のバッファの数を取得します。
    * @return GpuMeter の計測結果保存用のバッファの数が返ります。
    */
    int GetGpuBufferCount() NN_NOEXCEPT
    {
        return this->m_GpuBufferCount;
    }

    //! @}

private:
    int m_CoreCount;             //!< CPU コア数です。

    int m_UserMeterCount;        //!< ユーザーがインデクス指定して計測を行う CpuMeter の個数です。

    int m_CpuSectionCountMax;    //!< 各 CpuMeter が計測可能な区間の上限数です。
    int m_GpuSectionCountMax;    //!< GpuMeter が計測可能な区間の上限数です。

    int m_CpuBufferCount;        //!< Cpu の計測結果を保存する配列バッファの数です。何フレーム前までの計測結果を保持するかを決定します。
    int m_GpuBufferCount;        //!< Gpu の計測結果を保存する配列バッファの数です。何フレーム前までの計測結果を保持するかを決定します。
};

//! @brief  複数の CpuMeter 及び GpuMeter オブジェクトを管理して計測結果を集計するクラスです。
//! @details 保持する全てのメーターの計測結果を対象に集計を行います。
class LoadMeterCenter
{
private:
    NN_DISALLOW_COPY(LoadMeterCenter);

public:
    //! @brief 全てのメーターのセクション計測結果をタグと名前別で集計した結果を表します。
    struct ProfileSection
    {
        ProfileSection()
            :elapsedTime(0),
            callCount(0),
            maxElapsedTime(std::numeric_limits<nn::TimeSpan>::min()),
            minElapsedTime(nn::TimeSpan::FromSeconds( 999 )),
            meterName(NULL),
            sectionName(NULL),
            tag(0)
        {}

        nn::TimeSpan       elapsedTime;     //!< 合計の経過時間です。
        int                callCount;       //!< 呼び出し回数です。
        nn::TimeSpan       maxElapsedTime;  //!< 最大の経過時間です。
        nn::TimeSpan       minElapsedTime;  //!< 最小の経過時間です。
        const char*        meterName;       //!< メーターの名前です。
        const char*        sectionName;     //!< セクションの名前です。
        uint32_t           tag;             //!< セクションのタグです。
    };

    //! @name 初期化/終了
    //! @{

    //! @brief LoadMeterCenter を初期化します。
    //! @param[in] pDevice デバイスへのポインタです。
    //! @param[in] info 初期化用の情報です。
    //! @param[in] pMemory 初期用のバッファへのポインタ
    //! @param[in] memorySize バイトでのバッファサイズ
    //! @param[in] pMemoryPool Gpuタイムスタンプが書き込まれるメモリプールへのポインタ
    //! @param[in] memoryPoolOffset Gpuタイムスタンプが書き込まれるメモリプールへのオフセット
    //! @param[in] memoryPoolSize Gpuタイムスタンプが書き込まれるメモリプールのサイズ
    //! @pre
    //! - pDevice != NULL
    //! - pDevice が初期化されている。
    //! - info に GpuMeter の計測区間数が 1 以上の値で設定されている。
    //! @details 以下のメーターを作成します。
    //!          - フレーム計測用 CpuMeter : 1つ
    //!          - 各コア計測用   CpuMeter : LoadMeterCenterInfo で指定された数
    //!          - ユーザー計測用 CpuMeter : LoadMeterCenterInfo で指定された数
    //!          - GPU 計測用     GpuMeter : 1つ
    //! @n       フレーム計測用 CpuMeter にすべての CpuMeter と GpuMeter がアタッチされます。
    static void Initialize( nn::gfx::Device* pDevice, nn::perf::LoadMeterCenterInfo& info, void* pMemory, size_t memorySize, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    //!  @brief LoadMeterCenter を初期化します。GPU 計測は行いません。
    //!  @param[in] info 初期化用の情報です。
    //!  @param[in] pMemory 初期用のバッファへのポインタ
    //!  @param[in] memorySize バイトでのバッファサイズ
    //!  @pre
    //!  - info に CpuMeter の計測区間数が 1 以上の値で設定されている。
    //!  @details 以下のメーターを作成します。
    //!           - フレーム計測用 CpuMeter : 1つ
    //!           - 各コア計測用     CpuMeter : LoadMeterCenterInfo で指定された数
    //!           - ユーザー計測用 CpuMeter : LoadMeterCenterInfo で指定された数
    //!  @n       フレーム計測用 CpuMeter にすべての CpuMeter がアタッチされます。
    //!  @n       info の GpuMeter の計測区間数は無視されます。
    static void Initialize( nn::perf::LoadMeterCenterInfo& info, void* pMemory, size_t memorySize) NN_NOEXCEPT;

    //! @brief LoadMeterCenter の終了処理を行います。
    //! @param[in] pDevice デバイスへのポインタです。
    //! @pre     Initialize( pDevice, info, pMemoryPool, memoryPoolOffset) を使用して LoadMeterCenter が初期化されている。
    //! @details Initialize( pDevice, info, pMemoryPool, memoryPoolOffset) を使用して初期化した場合はこの関数を使って終了してください。
    static void Finalize( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //! @brief LoadMeterCenter の終了処理を行います。GpuMeter を初期化していない場合はこの関数で終了処理をします。
    //! @pre     GpuMeter を持たない LoadMeterCenter が初期化されている。
    //! @details 初期化時に GpuMeter を作成していない場合はこの関数を使って終了してください。
    static void Finalize() NN_NOEXCEPT;

    //! @brief        初期化に必要なバッファのアラインメントを返します。
    //! @return       バッファのアラインメントを返します。
    static size_t GetBufferAlignment() NN_NOEXCEPT;

    //! @brief        初期化に必要なバッファのサイズを返します。
    //! @param[in]    info 初期化用の情報です。
    //! @return       バッファのサイズを返します。
    static size_t CalculateBufferSize(LoadMeterCenterInfo info) NN_NOEXCEPT;

    //! @brief        初期化時に確保するクエリバッファのメモリプール上のアラインメントを返します。
    //! @param[in]    pDevice デバイスへのポインタです。
    //! @param[in]    info 初期化用の情報です。
    //! @return       メモリプールのアラインメントを返します。
    static size_t GetMemoryPoolAlignment(nn::gfx::Device* pDevice, LoadMeterCenterInfo info) NN_NOEXCEPT;

    //! @brief        初期化時に確保するクエリバッファのメモリプールのサイズを返します。
    //! @param[in]    pDevice デバイスへのポインタです。
    //! @param[in]    info 初期化用の情報です。
    //! @return       メモリプールのサイズを返します。
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice, LoadMeterCenterInfo info) NN_NOEXCEPT;

    //! @}
    //! @name 設定
    //! @{

    //! @brief  現在のスレッドが動作するコア ID を返す関数を登録します。
    //! @param[in]  getCoreNumberFunction コア番号を返す関数です。
    static void SetGetCoreNumberFunction(nn::perf::GetCoreNumberFunction getCoreNumberFunction) NN_NOEXCEPT
    {
        LoadMeterCenter::GetInstance().m_GetCoreNumberFunction = getCoreNumberFunction;
        GetGpuMeter()->SetGetCoreNumberFunction(getCoreNumberFunction);
    }

    //! @brief        ユーザー計測用の CpuMeter を指定して名前を設定します。
    //! @param[in]    index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
    //! @param[in]    meterName メーターの名前です。
    //! @details      コア計測用の CpuMeter の名前は変更出来ません。
    static void SetMeterName( int index, const char* meterName ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index < LoadMeterCenter::GetInstance().m_Info.GetUserMeterCount() );
        LoadMeterCenter::GetInstance().m_CpuUserMeterArray[index].SetName( meterName );
    }

    //! @brief        ログ表示のために用いるフレームレートを設定します。
    //! @param[in]    frameRate 設定するフレームレートです。
    static void SetFrameRate( float frameRate ) NN_NOEXCEPT
    {
        LoadMeterCenter::GetInstance().m_FrameRate = frameRate;
    }

    //! @brief        ログ表示のために用いるフレームレートを取得します。
    //! @return    フレームレートです。
    static float GetFrameRate() NN_NOEXCEPT
    {
        return LoadMeterCenter::GetInstance().m_FrameRate;
    }

    //! @brief 計測と集計処理の有効・無効を設定します。
    //! @param[in]    isEnabled   false を指定した場合、計測と集計処理を行いません。
    //! @details      必ず EndFrameMeasure() と BeginFrameMeasure() の間 (計測を行わない区間) で呼んでください。
    static void SetEnabled(bool isEnabled) NN_NOEXCEPT
    {
        LoadMeterCenter::GetInstance().m_IsEnabled = isEnabled;
    }

    //! @brief 計測と集計処理の有効・無効設定を取得します。
    //! @return    有効化されている場合は true を返します。
    static bool IsEnabled() NN_NOEXCEPT
    {
        return LoadMeterCenter::GetInstance().m_IsEnabled;
    }

    //! @}
    //! @name メーター取得
    //! @{

    //! @brief フレーム時間を測る CpuMeter を取得します。
    //! @return   フレーム時間を測る CpuMeter を返します。
    static CpuMeter* GetFrameMeter() NN_NOEXCEPT
    {
        return &LoadMeterCenter::GetInstance().m_FrameMeter;
    }

    //! @brief CPU コア別計測用 CpuMeter の配列 を取得します。
    //! @return  LoadMeterCenter が管理するコア計測用 CpuMeter の配列を返します。
    static CpuMeter* GetCpuCoreMeterArray() NN_NOEXCEPT
    {
        return LoadMeterCenter::GetInstance().m_CpuCoreMeterArray.data();
    }

    //! @brief ユーザー計測用 CpuMeter の配列 を取得します。
    //! @return  LoadMeterCenter が管理するユーザー計測用 CpuMeter の配列を返します。
    static CpuMeter* GetCpuUserMeterArray() NN_NOEXCEPT
    {
        return LoadMeterCenter::GetInstance().m_CpuUserMeterArray.data();
    }

    //! @brief CPU コア別計測用 CpuMeter を取得します。
    //! @param[in] index 「0 <= index < CoreCount」 の範囲で CpuMeter をインデクス指定します。
    //! @return  指定されたコア計測用 CpuMeter を返します。
    static CpuMeter* GetCpuCoreMeter(int index) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index < LoadMeterCenter::GetInstance().m_Info.GetCoreCount() );
        return &LoadMeterCenter::GetInstance().m_CpuCoreMeterArray[index];
    }

    //! @brief ユーザー計測用 CpuMeter を取得します。
    //! @param[in] index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
    //! @return  指定されたユーザー計測用 CpuMeter を返します。
    static CpuMeter* GetCpuUserMeter(int index) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index < LoadMeterCenter::GetInstance().m_Info.GetUserMeterCount() );
        return &LoadMeterCenter::GetInstance().m_CpuUserMeterArray[index];
    }

    //! @brief GpuMeter を取得します。
    //! @return GpuMeter を返します。
    static GpuMeter* GetGpuMeter() NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(&LoadMeterCenter::GetInstance().m_GpuMeter);
        return &LoadMeterCenter::GetInstance().m_GpuMeter;
    }

    //! @}
    //! @name 集計結果取得
    //! @{

    //! @brief 指定したセクションの計測結果(CallCount, ElapsedTime, maxElapsedTime, minElapsedTime) を返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  LoadMeterBase::ProfileSection 型の構造体を返します。
    static ProfileSection GetResultAll(UnitType unit, const char* sectionName, uint32_t tag) NN_NOEXCEPT;

    //! @brief 指定したセクションの計測結果(CallCount, ElapsedTime, maxElapsedTime, minElapsedTime) を返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
    //!            NULL を指定した場合はすべてのメーターの同名セクションを集計します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  LoadMeterBase::ProfileSection 型の構造体を返します。
    static ProfileSection GetResultAll(UnitType unit, const char* meterName, const char* sectionName, uint32_t tag) NN_NOEXCEPT;

    //! @brief 指定したメーターのセクションが呼ばれた回数を返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
    //!            NULL を指定した場合はすべてのメーターの同名セクションを集計します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  指定したセクションが呼ばれた回数を返します。
    static int GetCallCount(UnitType unit, const char* meterName, const char* sectionName, uint32_t tag) NN_NOEXCEPT
    {
        ProfileSection result = GetResultAll( unit, meterName, sectionName, tag );
        return result.callCount;
    }

    //! @brief 指定したセクション内の所要時間の合計を nn::TimeSpan で返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
    //!            NULL を指定した場合はすべてのメーターの同名セクションを集計します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  所要時間の合計を返します。
    static nn::TimeSpan GetElapsedTime(UnitType unit, const char* meterName, const char* sectionName, uint32_t tag) NN_NOEXCEPT
    {
        ProfileSection result = GetResultAll( unit, meterName, sectionName, tag );
        return result.elapsedTime;
    }

    //! @brief 指定したセクションの所要時間の最大値を nn::TimeSpan で返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
    //!            NULL を指定した場合はすべてのメーターの同名セクションを集計します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  所要時間の最大値を返します。
    static nn::TimeSpan GetMaxElapsedTime(UnitType unit, const char* meterName, const char* sectionName, uint32_t tag) NN_NOEXCEPT
    {
        ProfileSection result = GetResultAll( unit, meterName, sectionName, tag );
        return result.maxElapsedTime;
    }

    //! @brief 指定したセクションの所要時間の最小値を nn::TimeSpan で返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
    //!            NULL を指定した場合はすべてのメーターの同名セクションを集計します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  所要時間の最小値を返します。
    static nn::TimeSpan GetMinElapsedTime(UnitType unit, const char* meterName, const char* sectionName, uint32_t tag) NN_NOEXCEPT
    {
        ProfileSection result = GetResultAll( unit, meterName, sectionName, tag );
        return result.minElapsedTime;
    }

    //! @brief 指定したセクションの所要時間の平均値を nn::TimeSpan で返します。
    //! @param[in] unit CPU か GPU かを指定します。
    //! @param[in] meterName 指定する CpuMeter に対応するコア名もしくはメーター名を指定します。
    //!            NULL を指定した場合はすべてのメーターの同名セクションを集計します。
    //! @param[in] sectionName 指定するセクション名です。
    //!            NULL を指定した場合は名前は無視します。
    //! @param[in] tag 指定するタグです。
    //!            0 を指定した場合はタグは無視します。
    //! @return  所要時間の平均値を返します。
    static nn::TimeSpan GetAverageElapsedTime(UnitType unit, const char* meterName, const char* sectionName, uint32_t tag) NN_NOEXCEPT
    {
        ProfileSection result = GetResultAll( unit, meterName, sectionName, tag );
        if( result.callCount != 0 )
        {
            return nn::TimeSpan::FromNanoSeconds(static_cast<int64_t>( result.elapsedTime.GetNanoSeconds() / static_cast<float>(result.callCount)));
        }
        return nn::TimeSpan(0);
    }

    //! @}
    //! @name フレーム計測
    //! @{

    //! @brief フレーム計測用 CpuMeter を使用してフレームの開始時間を記録します。
    //! @details  フレーム開始時に呼んでください。
    static void BeginFrameMeasure() NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            LoadMeterCenter::GetInstance().m_FrameMeter.BeginMeasure();
        }
    }

    //! @brief  フレーム計測用 CpuMeter を使用してフレームの終了時間を記録します。
    //! @param[in] isClearProfile 集計結果を初期化するかどうかです。
    //! @details  フレーム終了時に呼んでください。
    //!           内部で以下の操作をします。
    //!           - フレーム全体の計測の終了
    //!           - 集計結果の初期化(isClearProfile == true の場合)。
    //!           - 各 CpuMeter と GpuMeter の計測結果の集計とバッファの切り替え。
    //!           - LoadMeterCenter による計測結果の集計。
    //! @n ※集計結果の初期化は任意のタイミングで Clear() を使って行うことも可能です。
    static void EndFrameMeasure(bool isClearProfile) NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            LoadMeterCenter::GetInstance().m_FrameMeter.EndMeasure();

            if(isClearProfile)
            {
                LoadMeterCenter::GetInstance().Clear();
            }
            LoadMeterCenter::GetInstance().m_FrameMeter.Next();
            LoadMeterCenter::GetInstance().Profile( UnitType_Cpu );
            LoadMeterCenter::GetInstance().Profile( UnitType_Gpu );
        }
    }

    //! @}
    //! @name CPU 計測
    //! @{

    //! @brief 実行中のコアに対応した CpuMeter で計測を開始します。
    //! @param[in] sectionName 計測区間の名前です。
    //! @param[in] tag 計測する区間に設定する任意のタグです。
    //! @details      計測区間を処理するコアが固定されている場合に限りこの関数を使用することができます。
    static void BeginMeasure( const char* sectionName, uint32_t tag ) NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            int index = LoadMeterCenter::GetInstance().m_GetCoreNumberFunction();
            NN_SDK_ASSERT(index < LoadMeterCenter::GetInstance().m_Info.GetCoreCount());
            LoadMeterCenter::GetInstance().m_CpuCoreMeterArray[index].BeginMeasure(sectionName, tag);
        }
    }

    //! @brief 実行中のコアに対応した CpuMeter で計測を終了します。
    //! @details      計測区間を処理するコアが固定されている場合に限りこの関数を使用することができます。
    static void EndMeasure() NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            int index = LoadMeterCenter::GetInstance().m_GetCoreNumberFunction();
            NN_SDK_ASSERT(index < LoadMeterCenter::GetInstance().m_Info.GetCoreCount());
            LoadMeterCenter::GetInstance().m_CpuCoreMeterArray[index].EndMeasure();
        }
    }

    //! @brief 指定した CpuMeter で計測を開始します。
    //! @param[in] index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
    //! @param[in] sectionName 計測区間の名前です。
    //! @param[in] tag 計測する区間に設定する任意のタグです。
    //! @details 動作するコアが固定されていない区間を計測する場合はこの関数を使用してください。
    //! @n       並列に処理が行われる複数区間の計測には別々の CpuMeter を指定する必要があります。
    static void BeginMeasureIndex( int index, const char* sectionName, uint32_t tag ) NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            NN_SDK_ASSERT(index < LoadMeterCenter::GetInstance().m_Info.GetUserMeterCount());
            LoadMeterCenter::GetInstance().m_CpuUserMeterArray[index].BeginMeasure(sectionName, tag);
        }
    }

    //! @brief 指定した CpuMeter で計測を終了します。
    //! @param[in] index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
    //! @details BeginMeasureIndex で計測開始した場合はこの関数を使用してください。
    static void EndMeasureIndex( int index ) NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            NN_SDK_ASSERT(index < LoadMeterCenter::GetInstance().m_Info.GetUserMeterCount());
            LoadMeterCenter::GetInstance().m_CpuUserMeterArray[index].EndMeasure();
        }
    }

    //! @}
    //! @name GPU 計測
    //! @{

    //! @brief GpuMeter で GPU 計測を開始します。
    //! @param[in] pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in] sectionName 計測区間の名前です。
    //! @param[in] tag 計測する区間に設定する任意のタグです。
    static void BeginMeasureGpu( nn::gfx::CommandBuffer* pCommandBuffer, const char* sectionName, uint32_t tag ) NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            LoadMeterCenter::GetInstance().m_GpuMeter.BeginMeasure(pCommandBuffer, sectionName, tag);
        }
    }

    //! @brief GpuMeter で GPU 計測を終了します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    static void EndMeasureGpu(nn::gfx::CommandBuffer* pCommandBuffer) NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            LoadMeterCenter::GetInstance().m_GpuMeter.EndMeasure(pCommandBuffer);
        }
    }

    //! @}
    //! @name 色設定
    //! @{

    //! @brief CPU コアに対応した CpuMeter の色を変更します。
    //! @param[in]    color 変更する色です。
    static void SetColor( const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        int index = LoadMeterCenter::GetInstance().m_GetCoreNumberFunction();
        NN_SDK_ASSERT( index < LoadMeterCenter::GetInstance().m_Info.GetCoreCount() );
        LoadMeterCenter::GetInstance().m_CpuCoreMeterArray[index].SetColor( color );
    }

    //! @brief  ユーザー計測用 CpuMeter の色を変更します。
    //! @param[in]    index 保持するユーザ計測用 CpuMeter の個数未満の値でインデクス指定します。
    //! @param[in]    color 変更する色です。
    static void SetColorIndex( int index, const nn::util::Color4u8Type& color) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index < LoadMeterCenter::GetInstance().m_Info.GetUserMeterCount() );
        LoadMeterCenter::GetInstance().m_CpuUserMeterArray[index].SetColor( color );
    }

    //! @brief GpuMeter の色を変更します。
    //! @param[in]    color 変更する色です。
    static void SetColorGpu( const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        LoadMeterCenter::GetInstance().m_GpuMeter.SetColor( color );
    }

    //! @}

    //! @brief 累計の計測結果をクリアします。
    static void Clear() NN_NOEXCEPT
    {
        if (LoadMeterCenter::GetInstance().m_IsEnabled)
        {
            LoadMeterCenter::GetInstance().Clear(UnitType_Cpu);
            LoadMeterCenter::GetInstance().Clear(UnitType_Gpu);
        }
    }

    //! @brief   最新の EndFrameMeasure() で集計された累計結果をコンソール出力します。
    //! @details リリースビルド時もログが出力されます。
    static void Dump() NN_NOEXCEPT;

private:
    void InitializeFrameMeter(void* memory) NN_NOEXCEPT;
    void InitializeCpuMeter(void* memory) NN_NOEXCEPT;
    void InitializeGpuMeter(void* memory, nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset ) NN_NOEXCEPT;

    static size_t GetFrameMeterBufferAlignment() NN_NOEXCEPT;
    static size_t GetCpuMeterBufferAlignment() NN_NOEXCEPT;
    static size_t GetGpuMeterBufferAlignment() NN_NOEXCEPT;

    static size_t CalculateFrameMeterBufferSize(LoadMeterCenterInfo info) NN_NOEXCEPT;
    static size_t CalculateCpuMeterBufferSize(LoadMeterCenterInfo info) NN_NOEXCEPT;
    static size_t CalculateGpuMeterBufferSize(LoadMeterCenterInfo info) NN_NOEXCEPT;

    void Profile(UnitType unit) NN_NOEXCEPT;

    void Clear(UnitType unit) NN_NOEXCEPT;

    void DumpDetails(ProfileSection* profileSection) NN_NOEXCEPT;

    ProfileSection* GetProfileSectionArray( UnitType unit ) NN_NOEXCEPT;

    int GetProfiledSectionCount( UnitType unit ) NN_NOEXCEPT;

    void SetProfiledSectionCount( UnitType unit, int value ) NN_NOEXCEPT;

    //! @brief インスタンスを取得します。
    //! @return シングルトンのインスタンスを返します。
    static LoadMeterCenter& GetInstance() NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_Instance);
        return *m_Instance;
    }

    LoadMeterCenter() NN_NOEXCEPT
    {}

private:
    typedef nn::util::IntrusiveList<LoadMeterBase, nn::util::IntrusiveListBaseNodeTraits<LoadMeterBase>> LoadMeterList;
    static LoadMeterCenter*                   m_Instance;

    nn::perf::GetCoreNumberFunction           m_GetCoreNumberFunction;   //!< 動作中のスレッドのコア番号を取得する関数です

    CpuMeter                                  m_FrameMeter;              //!< 1フレームの時間を計測する CpuMeter です。
    nn::util::PlacementArray<CpuMeter>        m_CpuCoreMeterArray;       //!< 任意の範囲を計測するコア計測用 CpuMeter の配列です。全ての CpuMeter は m_FrameMeter にアタッチされます。
    nn::util::PlacementArray<CpuMeter>        m_CpuUserMeterArray;       //!< 任意の範囲を計測するユーザー計測用 CpuMeter の配列です。全ての CpuMeter は m_FrameMeter にアタッチされます。
    GpuMeter                                  m_GpuMeter;                //!< 任意の範囲を計測する GpuMeter です。m_FrameMeter にアタッチされます。

    nn::util::PlacementArray<ProfileSection>  m_ProfileCpuSectionArray;  //!< CpuMeter のセクションの計測結果をメーターとタグ、名前別に集計して格納する配列です。
    nn::util::PlacementArray<ProfileSection>  m_ProfileGpuSectionArray;  //!< GpuMeter のセクションの計測結果をメーターとタグ、名前別に集計して格納する配列です。
    int                                       m_ProfiledCpuSectionCount; //!< タグと名前別に集計した CpuMeter のセクション分類数です。
    int                                       m_ProfiledGpuSectionCount; //!< タグと名前別に集計した GpuMeter のセクション分類数です。

    nn::perf::LoadMeterCenterInfo             m_Info;                    //!< 作成する CpuMeter の個数や、メモリ確保・解放用の関数ポインタ等の情報を保持します。
    float                                     m_FrameRate;               //!< 設定するフレームレートです。

    bool                                      m_IsEnabled;               //!< 計測処理をするかどうかのフラグです。
};

//! @brief        コンストラクタとデストラクタで自動的に計測の開始・終了を行うクラスです。
//! @pre          LoadMeterCenter が初期化されている。
//! @details      LoadMeterCenter が保持する、動作中の CPU コアに対応した CpuMeter で計測します。
//! @n            計測区間中で動作する CPU コアが移動しない場合に限り使用してください。
class AutoMeasure
{
private:
    NN_DISALLOW_COPY(AutoMeasure);

public:
    //! @brief コンストラクタです。呼ばれると動作中の CPU コアに対応した CpuMeter で計測を開始し、開始時間を記録します。
    //! @param[in] sectionName 計測区間の名前です。
    //! @param[in] tag 計測する区間に設定する任意のタグです。
    //! @pre       LoadMeterCenter が初期化されている必要があります。
    AutoMeasure(const char* sectionName, uint32_t tag ) NN_NOEXCEPT
    {
        LoadMeterCenter::BeginMeasure( sectionName, tag );
    }

    //! @brief デストラクタです。呼ばれると計測を終了します。
    ~AutoMeasure() NN_NOEXCEPT
    {
        LoadMeterCenter::EndMeasure();
    }
};

//! @brief        コンストラクタとデストラクタで自動的に計測の開始・終了を行うクラスです。
//! @pre          LoadMeterCenter が初期化されている。
//! @details      LoadMeterCenter が保持する、インデクス指定したユーザー計測用の CpuMeter で計測します。
class AutoMeasureIndex
{
private:
    NN_DISALLOW_COPY(AutoMeasureIndex);

public:
    //! @brief コンストラクタです。呼ばれるとインデクス指定した CpuMeter で計測を開始し、開始時間を記録します。
    //! @param[in] index LoadMeter を指定します。
    //! @param[in] sectionName 計測区間の名前です。
    //! @param[in] tag 計測する区間に設定する任意のタグです。
    //! @pre       LoadMeterCenter が初期化されている必要があります。
    AutoMeasureIndex( int index, const char* sectionName, uint32_t tag ) NN_NOEXCEPT
        :m_Index( index )
    {
        LoadMeterCenter::BeginMeasureIndex( m_Index, sectionName, tag );
    }

    //! @brief デストラクタです。呼ばれると計測を終了します。
    ~AutoMeasureIndex() NN_NOEXCEPT
    {
        LoadMeterCenter::EndMeasureIndex( m_Index );
    }

private:
    int m_Index;
};
} // namespace perf
} // namespace nn

