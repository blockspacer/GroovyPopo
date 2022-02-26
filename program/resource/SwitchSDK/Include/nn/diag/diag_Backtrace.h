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
*   @file
*   @brief  バックトレース取得関連の API 宣言
*/

#include <nn/nn_Common.h>

namespace nn { namespace diag {

/**
*   @brief  呼び出したスレッドのバックトレースを取得します。
*
*   @param[out]     pOutArray       アドレスを格納する配列
*   @param[in]      arrayCountMax   pOutArray に格納できる最大個数
*   @return                         pOutArray に格納したアドレスの個数
*/
int GetBacktrace(uintptr_t *pOutArray, int arrayCountMax) NN_NOEXCEPT;

#if defined(NN_BUILD_CONFIG_OS_HORIZON) || defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

/**
*   @brief  レジスタの情報からバックトレースを取得します。
*
*   @param[out]     pOutArray       アドレスを格納する配列
*   @param[in]      arrayCountMax   pOutArray に格納できる最大個数
*   @param[in]      fp              バックトレースを取得するスレッドのフレームポインタ
*   @param[in]      sp              バックトレースを取得するスレッドのスタックポインタ
*   @param[in]      pc              バックトレースを取得するスレッドのプログラムカウンタ
*   @return                         pOutArray に格納したアドレスの個数
*/
int GetBacktrace(uintptr_t *pOutArray, int arrayCountMax, uintptr_t fp, uintptr_t sp, uintptr_t pc) NN_NOEXCEPT;

#endif // #if defined(NN_BUILD_CONFIG_OS_HORIZON)

}} // nn::diag
