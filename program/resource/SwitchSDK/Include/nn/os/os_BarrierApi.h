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
 * @brief   バリアに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  BarrierType;

//! @name バリア関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   BarrierType オブジェクトを初期化します。
 *
 * @param[in] barrier       BarrierType オブジェクトへのポインタ
 * @param[in] numThreads    待ち合わせたいスレッド数
 *
 * @pre
 *  - numThreads > 0
 *  - barrier が未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - barrier が非シグナル状態である
 *
 */
void    InitializeBarrier(BarrierType* barrier, int numThreads) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   BarrierType オブジェクトを Finalize します。
 *
 * @param[in] barrier   BarrierType オブジェクトへのポインタ
 *
 * @pre
 *  - barrier が非シグナル状態である
 *
 * @post
 *  - barrier が未初期化状態である
 *
 */
void    FinalizeBarrier(BarrierType* barrier)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   BarrierType オブジェクトの待ち合わせを行ないます。
 *
 * @param[in] barrier   BarrierType オブジェクトへのポインタ
 *
 * @pre
 *  - barrier が非シグナル状態である
 *  - 待ち合わせスレッド数が、0 ～ (numThreads - 1) のいずれかである
 *
 * @post
 *  - barrier は非シグナル状態である
 *  - 待ち合わせスレッド数が 0 である
 *
 */
void    AwaitBarrier(BarrierType* barrier)  NN_NOEXCEPT;


//! @}

}} // namespace nn::os



