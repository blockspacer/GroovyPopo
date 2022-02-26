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
 * @brief   PCM ライブラリのチャンネルアクセス機能の API 宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/pcm/pcm_MeasuringPoint.h>

namespace nn {
namespace pcm {

//! @name ライブラリの初期化と終了
//! @{

/**
 * @brief PCM ライブラリを初期化します。
 *
 * @post
 *  - ライブラリが初期化済の状態である
 *
 * @details     PCM ライブラリを初期化します。@n
 *              本関数の呼び出し回数はライブラリ内部でカウントされます。
 */
void Initialize() NN_NOEXCEPT;

/**
 * @brief PCM ライブラリを終了します。
 *
 * @post
 *  - ライブラリが初期化前の状態である
 *
 * @details     PCM ライブラリを終了します。@n
 *              ライブラリ初期化を行った回数分、本関数が呼ばれた場合にのみ、
 *              ライブラリが初期化前の状態になります。
 */
void Finalize() NN_NOEXCEPT;

//! @}
// ~ライブラリの初期化と終了

//! @name 消費電力の取得
//! @{

/**
 * @brief 指定した電力計測ポイントが利用可能であるかを取得します。
 *
 * @param[in]  point      電力計測ポイント識別子
 *
 * @return 計測ポイントが利用可能であるか
 * @retval true   計測ポイントは利用できます。
 * @retval false  計測ポイントは利用できません。
 *
 * @pre
 *  - ライブラリが初期化済の状態である
 *
 * @details
 *  指定した電力計測ポイントがボード上に実装されており、利用可能であるかを取得します。
 */
bool IsSupported(MeasuringPoint point) NN_NOEXCEPT;

/**
 * @brief 指定したポイントでの直近の消費電力を計測デバイスから読み出し mW 単位で取得します。
 *
 * @param[in]  point      電力計測ポイント識別子
 *
 * @return  消費電力 [mW]
 *
 * @pre
 *  - ライブラリが初期化済の状態である
 *
 * @details
 *  指定したポイントでの直近の消費電力を mW 単位で取得します。@n
 *  指定した電力計測ポイントが利用可能でない場合、常に 0 を返します。@n
 *  電力計測ポイントが利用可能かどうかの判定には @ref nn::pcm::IsSupported() を使用してください。@n
 *  関数内で計測デバイスへのアクセスを行うため、本関数は最大数 msec ブロックする可能性があります。
 */
int ReadCurrentPower(MeasuringPoint point) NN_NOEXCEPT;

//! @}
// ~消費電力の取得

} // pcm
} // nn


