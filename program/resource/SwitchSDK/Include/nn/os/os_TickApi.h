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
 * @brief   チックに関する API の宣言
 */

#pragma once

#include <stdint.h>
#include <nn/nn_Macro.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

class   Tick;

//! @name 時間管理関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   システムチック値を取得します。
 *
 * @return  システムチック値です。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 現在のシステムチック値を返す
 *
 * @details
 *  現在のシステムチック値を Tick 型で返します。
 *
 */
Tick  GetSystemTick()   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   システムチック値の周波数を取得します。
 *
 * @return  システムチックの 1 秒あたりのカウント数です。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 現在のシステムチックの周波数を返す
 *
 * @details
 *  システムチックの１秒間のカウント数を返します。@n
 *  これはつまり、システムチックの周波数（Hz）に相当します。
 *
 */
int64_t GetSystemTickFrequency()  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   システムチックを nn::TimeSpan 型の時間表現に変換して返します。
 *
 * @param[in]   tick    システムチック値
 *
 * @return      nn::TimeSpan 型の時間表現です。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 指定されたシステムチック値を nn::TimeSpan 型へ変換したものを返す
 *
 * @details
 *  システム依存の時間表現であるシステムチック値を、@n
 *  自然時間表現された nn::TimeSpan 型のシステム時間に変換します。
 *
 */
nn::TimeSpan  ConvertToTimeSpan(Tick tick)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   nn::TimeSpan 型の時間表現をシステムチックに変換して返します。
 *
 * @param[in]   timeSpan    nn::TimeSpan 型の時間表現です。
 *
 * @return      システムチック値です。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 指定された nn::TimeSpan 型の時間表現をシステムチックへ変換したものを返す
 *
 * @details
 *  自然時間表現された nn::TimeSpan 型の時間表現を、
 *  システム依存の時間表現であるシステムチック値に変換します。
 *
 */
Tick    ConvertToTick(nn::TimeSpan timeSpan)  NN_NOEXCEPT;


//! @}

}} // namespace nn::os

