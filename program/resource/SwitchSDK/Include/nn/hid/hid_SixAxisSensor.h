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
 * @brief       6 軸センサーに関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/nn_TimeSpan.h>
#include <nn/util/util_BitFlagSet.h>
#include <nn/util/util_MathTypes.h>

namespace nn { namespace hid {

const int SixAxisSensorStateCountMax = 16;      //!< 内部的に保持される SixAxisSensorState の最大数

const float AccelerometerMax = 7.0f;       //!< 加速度センサーの取る最大の絶対値です。単位は G です。
const float AngularVelocityMax = 5.0f;     //!< ジャイロセンサーの取る最大の絶対値です。単位は 360dps を 1.0 とする値です。

/**
 * @brief       6軸センサーのハンドルを表す構造体です。
 */
struct SixAxisSensorHandle
{
    int32_t _storage;
};

struct DirectionState
{
    ::nn::util::Float3  x;
    ::nn::util::Float3  y;
    ::nn::util::Float3  z;
};

/**
 * @brief       SixAxisSensor の入力状態の属性定義です。
 */
struct SixAxisSensorAttribute
{
    typedef ::nn::util::BitFlagSet<32, SixAxisSensorAttribute>::Flag<0>
            IsConnected;    //!< 対応するデバイスがシステムと接続状態にあるか否か
    typedef ::nn::util::BitFlagSet<32, SixAxisSensorAttribute>::Flag<1>
            IsInterpolated;    //!< 対応する入力状態が内挿補間により生成されたものか否か
};

/**
 * @brief       SixAxisSensor の入力状態の属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, SixAxisSensorAttribute> SixAxisSensorAttributeSet;

/**
 * @brief       6 軸センサーから取得される状態を表す構造体です。
 */
struct SixAxisSensorState
{
    ::nn::TimeSpanType        deltaTime;        //!< 前回サンプリングされてから今回のサンプリングまでの経過時間です。
    int64_t                   samplingNumber;   //!< 6 軸センサーの入力状態がサンプリングされる度に増加する値です。
    ::nn::util::Float3        acceleration;     //!< 加速度センサーの各方向ごとの加速度の値です。単位は G です。
    ::nn::util::Float3        angularVelocity;  //!< ジャイロセンサーの各方向ごとの角速度の値です。360dps を 1.0 とする値です。
    ::nn::util::Float3        angle;            //!< 各方向ごとの角速度の値を積算して得られる回転角の値です。360度 を 1.0 とする値です。
    DirectionState            direction;
    SixAxisSensorAttributeSet attributes;       //!< SixAxisSensor の入力状態の属性です。

    /**
     * @brief       コントローラーの 3 次元姿勢を表すクォータニオンを取得します。
     *
     * @param[out]  pOutQuaternion      コントローラーの 3 次元姿勢を表すクォータニオンです。
     */
    void GetQuaternion(nn::util::Vector4fType* pOutQuaternion) const NN_NOEXCEPT;
};

/**
 * @brief       ジャイロセンサーのゼロ点補正モードの定義です。
 *
 * @details     コントローラーが静止している状態での角速度の出力値(以下、ゼロ点) は、
 *              コントローラー内部の温度変化などの影響により、値が変動する場合があります。(以下、ゼロ点ドリフト）
 *              ゼロ点ドリフトに依る影響を相殺するため、ライブラリ内部ではコントローラーの静止判定を常時行い、
 *              静止していると見做したときに内部計算に用いるゼロ点を補正しています。
 *              静止判定は、コントローラーの角速度入力が一定の期間変化しないかどうかを確認しており、
 *              判定の閾値は GyroscopeZeroDriftMode ごとに異なる設定となります。
 *
 *              GyroscopeZeroDriftMode_Loose は、コントローラーの静止判定が最も緩いモードで、
 *              ゼロ点の変動に最も強い設定となります。
 *              一方で、ゆっくりと動かした場合や、等角速度運動時にもゼロ点が補正されるため、意図しない入力を取得してしまう恐れがあります。
 *              この場合は、より条件が厳しいモードに設定することで改善される場合があります。
 *              デフォルト設定は GyroscopeZeroDriftMode_Standard です。
 *              非常にゆっくりとした動きや、細かい動作を取得したい場合は、 GyroscopeZeroDriftMode_Tight をご利用ください。
 *
 */
enum GyroscopeZeroDriftMode
{
    GyroscopeZeroDriftMode_Loose,        //!< Loose モードです。
    GyroscopeZeroDriftMode_Standard,     //!< Standard モードです。
    GyroscopeZeroDriftMode_Tight,        //!< Tight モードです。
};

//! @name 6 軸センサー関連 API
//! @{

/**
 * @brief       SixAxisSensor のサンプリングを開始します。
 *
 * @details     指定の handle と対応する 6 軸センサーのサンプリングを開始します。
 *              本関数が呼び出された際にコントローラの基準姿勢が設定されます。
 *              基準姿勢の Z 軸は重力方向、X/Y軸は初期化時点でのコントローラの姿勢と
 *              基準姿勢との差異が少なくなるような軸が指定されます。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 */
void StartSixAxisSensor(const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor のサンプリングを停止します。
 *
 * @details     指定の handle と対応する 6 軸センサーのサンプリングを停止します。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 */
void StopSixAxisSensor(const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor が静止状態にあるかどうかを取得します。
 *
 * @details     指定の handle と対応するコントローラーの 6 軸センサーが静止状態かどうかを取得します。
 *              以下の条件を満たした状態が一定時間継続したときに静止状態と判定します。
 *              - 各軸の加速度の変位が、ある閾値以内であること
 *              - 各軸の角速度の変位が、ある閾値以内であること
 *
 *              静止判定に利用する角速度は、6軸センサーから取得された加工処理前の生値を用いるため、
 *              遊び設定等のパラメータ設定値は静止判定に影響しません。
 *
 *              センサー自身のハードウェア特性に基づくノイズが内部的に発生することが避けられないため、
 *              本関数では 6軸センサーの種類に応じた最適な閾値を内部的に用いることで静止判定を行っています。
 *              コントローラーが静止状態にあるかどうかを判定する際は、本関数の利用を推奨します。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 *
 * @return      SixAxisSensor が静止状態かどうかを返します。
 */
bool IsSixAxisSensorAtRest(const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor の最新の入力状態を取得します。
 *
 * @details
 *              指定の handle と対応する 6 軸センサーから GetSixAxisSensorStates() で 1 つの入力状態を読み出した時と同じ値が返ります。
 *
 *              SixAxisSensorState::attributes メンバの SixAxisSensorAttribute::IsConnected フラグが false の場合、
 *              入力状態は無入力となります。
 *
 * @param[out]  outValue                    入力状態を読み出すバッファ
 * @param[in]   handle                      6軸センサーのハンドル
 *
 * @pre
 *              - outValue != nullptr
 */
void GetSixAxisSensorState(SixAxisSensorState* outValue,
                           const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に、指定の handle と対応する 6 軸センサーから入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *              読み出し可能な入力状態の最大数は SixAxisSensorStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は SixAxisSensorState::samplingNumber を参照してください。
 *              再解決の発生によって handle と対応するコントローラーの ID が解決状態でなくなった場合であっても、 SixAxisSensor の入力状態は無入力となります。
 *              無入力時は、accelerometer の入力状態は 1.0 G が基準方向に射影された値、
 *              angularVelocity は全成分が 0、 angle と direction は過去の入力状態が引き継がれたままとなりますが、値の読み出し自体は継続して実施可能です。
 *              取得される direction はコントローラーの 3次元姿勢を表す正規直交系に近づけたベクトルの組として得られます。
 *              なお、本 API により取得される 3次元姿勢は、SixAxisSensorState の仕様変更に合わせて、
 *              クオータニオンによる 3 次元姿勢の表現に置き換えられる予定です。
 *
 *              SixAxisSensorState::attributes メンバの SixAxisSensorAttribute::IsConnected フラグが false の場合、
 *              入力状態は無入力となります。
 *
 * @param[out]  outStates                   入力状態を読み出すバッファ（配列）
 * @param[in]   count                       読み出す入力状態の数
 * @param[in]   handle                      6軸センサーのハンドル
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - outStates != nullptr
 *              - count >= 0
 * @post
 *              - 戻り値 n について、n >= 0
 *              - accelerometer の絶対値は AccelerometerMax 以下
 *              - angularVelocity の絶対値は AngularVelocityMax 以下
 */
int GetSixAxisSensorStates(SixAxisSensorState* outStates,
                           int count,
                           const SixAxisSensorHandle& handle) NN_NOEXCEPT;

//! @}

//! @name 6 軸センサーの設定関連 API
//! @{

/**
 * @brief       SixAxisSensor の加速度による姿勢補正状態を取得します。
 *
 * @details     指定の handle と対応する 6 軸センサーの姿勢補正状態を取得します。
 *              デフォルトでは加速度による姿勢補正は ON であり、加速度補正処理の重みを 0.030f、
 *              加速度補正の有効半径を 0.400f とした補正が掛かっています。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 *
 * @return      加速度による姿勢補正の状態を返します。
 */
bool IsSixAxisSensorFusionEnabled(const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor の加速度による姿勢補正状態を制御します。
 *
 * @details     指定の handle と対応する 6 軸センサーの姿勢補正状態を制御します。
 *              加速度による姿勢補正はデフォルトで ON とし、設定パラメータは以下の通りです。
 *              - 加速度補正処理の重み = 0.030f
 *              - 加速度補正の有効半径 = 0.400f
 *
 * @param[in]   handle                      6軸センサーのハンドル
 * @param[in]   enable                      設定する 6軸センサーの姿勢補正の状態
 */
void EnableSixAxisSensorFusion(const SixAxisSensorHandle& handle,
                               bool enable) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor の加速度による姿勢補正のパラメータを設定します。
 *
 * @details     指定の handle と対応する 6 軸センサーの姿勢補正の内部計算に用いるパラメータを設定します。
 *              設定したパラメータは全ての軸方向に対して適用されます。
 *              加速度の大きさが指定された重力加速度 (=1.0f) を中心に指定された範囲内にあるときに、入力加速度の向きを元に姿勢を補正します。
 *
 * @param[in]   revisePower                 加速度補正の重みです。値が大きいほど急激に補正がかかります。
 * @param[in]   reviseRange                 加速度補正の有効範囲です。重力加速度(=1.0f) を中心にこの範囲内の加速度センサの値を補正計算に使用します。
 *                                          例えば 0.4f を指定した場合は 0.6f から 1.4f の範囲に収まる加速度センサの値を補正計算に使用します。
 * @param[in]   handle                      6軸センサーのハンドル
 *
 * @pre
 *              - revisePower >= 0.0f && revisePower <= 1.0f
 */
void SetSixAxisSensorFusionParameters(const SixAxisSensorHandle& handle,
                                      float revisePower,
                                      float reviseRange) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor の加速度による姿勢補正のパラメータを取得します。
 *
 * @details     指定の handle と対応する 6 軸センサーの姿勢補正の内部計算に用いるパラメータを取得します。
 *
 * @param[out]  pOutRevisePower             加速度補正の重みです。値が大きいほど急激に補正がかかります。
 * @param[out]  pOutReviseRange             加速度補正の有効範囲です。重力加速度(1.0f) を中心にこの範囲内の加速度センサの値を補正計算に使用します。
 *                                          例えば 0.4f を指定した場合は 0.6f から 1.4f の範囲に収まる加速度センサの値を補正計算に使用します。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 *
 */
void GetSixAxisSensorFusionParameters(float* pOutRevisePower,
                                      float* pOutReviseRange,
                                      const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       SixAxisSensor の加速度による姿勢補正のパラメータを初期化します。
 *
 * @details     指定の handle と対応する 6 軸センサーの姿勢補正の内部計算に用いるパラメータを初期化します。
 *              加速度補正処理の重みを 0.030f、加速度補正の有効半径を 0.400f として、
 *              SetSixAxisSensorFusionParameters() を呼び出すことと等価です。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 *
 */
void ResetSixAxisSensorFusionParameters(const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       ジャイロセンサーのゼロ点補正モードを設定します。
 *
 * @details     指定の handle と対応する 6 軸センサーのゼロ点ドリフト補正モードを設定します。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 * @param[in]   mode                        設定するゼロ点ドリフト補正モード
 *
 */
void SetGyroscopeZeroDriftMode(const SixAxisSensorHandle& handle,
                               const GyroscopeZeroDriftMode& mode) NN_NOEXCEPT;

/**
 * @brief       ジャイロセンサーのゼロ点補正モードを取得します。
 *
 * @details     指定の handle と対応する 6 軸センサーのゼロ点ドリフト補正モードを取得します。
 *
 * @param[in]   handle                      6軸センサーのハンドル
 *
 * @return      現在の 6 軸センサーのゼロ点ドリフト補正モードを返します。
 */
GyroscopeZeroDriftMode GetGyroscopeZeroDriftMode(const SixAxisSensorHandle& handle) NN_NOEXCEPT;

/**
 * @brief       6 軸センサーの性能改善のためのファームウェア更新が可能かどうかを確認します。
 *
 * @param[in]   handle                  6 軸センサーのハンドル
 *
 * @return      ファームウェア更新が可能かどうかを返します。
 *
 * @details     指定のハンドルに対応した 6 軸センサーが搭載されたコントローラにおいて、
 *              6 軸センサーの性能改善のためのファームウェア更新が利用可能かどうかを確認します。
 *
 *              6 軸センサーの性能改善のためにファームウェアバージョンを更新できる場合は、true が返ります。
 *              このとき、nn::hid::ShowControllerFirmwareUpdate を呼び出し、コントローラのアップデートを行うことを推奨します。
 */
bool IsFirmwareUpdateAvailableForSixAxisSensor(
    const SixAxisSensorHandle& handle) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
