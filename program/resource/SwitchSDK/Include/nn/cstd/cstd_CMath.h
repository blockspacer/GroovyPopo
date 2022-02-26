/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

/**
* @file
* @brief   \<cmath\> 関連のワークアラウンド
*/

#include <nn/nn_Common.h>

namespace nn { namespace cstd {

/**
* @brief   標準の std::isnan と同一です。
*
* @param[in]  value   long double 型の浮動小数点値です。
* @return             標準と同一です。
*
* @overloadlist{nn_cstd_IsNan}
*/
bool IsNan(long double value) NN_NOEXCEPT;

/**
* @brief   標準の std::isnan と同一です。
*
* @param[in]  value   double 型の浮動小数点値です。
* @return             標準と同一です。
*
* @overloadlist{nn_cstd_IsNan}
*/
bool IsNan(double value) NN_NOEXCEPT;

/**
* @brief   標準の std::isnan と同一です。
*
* @param[in]  value   float 型の浮動小数点値です。
* @return             標準と同一です。
*
* @overloadlist{nn_cstd_IsNan}
*/
bool IsNan(float value) NN_NOEXCEPT;

/**
* @brief   標準の std::isinf と同一です。
*
* @param[in]  value   long double 型の浮動小数点値です。
* @return             標準と同一です。
*
* @overloadlist{nn_cstd_IsInf}
*/
bool IsInf(long double value) NN_NOEXCEPT;

/**
* @brief   標準の std::isinf と同一です。
*
* @param[in]  value   double 型の浮動小数点値です。
* @return             標準と同一です。
*
* @overloadlist{nn_cstd_IsInf}
*/
bool IsInf(double value) NN_NOEXCEPT;

/**
* @brief   標準の std::isinf と同一です。
*
* @param[in]  value   float 型の浮動小数点値です。
* @return             標準と同一です。
*
* @overloadlist{nn_cstd_IsInf}
*/
bool IsInf(float value) NN_NOEXCEPT;

}}
