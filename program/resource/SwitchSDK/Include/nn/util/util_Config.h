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
*   @file
*   @brief  共通 API ライブラリ共通の config 入力
*/

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/TargetConfigs/build_Fpu.h>

// ベクトル・行列の実装分岐用マクロ
#define NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_GENERAL 0
#define NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_NEON    1

#if !defined(NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET)
    #if defined(NN_BUILD_CONFIG_FPU_NEON)
        #define NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_NEON
    #else
        #define NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_GENERAL
    #endif
#endif

#define NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_GENERAL (NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET==NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_GENERAL)
#define NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_IS_NEON (NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET==NN_UTIL_CONFIG_VECTOR_MATRIX_TARGET_NEON)
