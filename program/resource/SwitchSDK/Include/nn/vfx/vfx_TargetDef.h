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

//--------------------------------------------------------------------------------
//  プラットフォーム共通
//--------------------------------------------------------------------------------
#include <nn/TargetConfigs/build_Base.h>
#include <nn/nn_SdkLog.h>
#include <nn/nn_SdkAssert.h>
#include <nn/util/util_Constant.h>
#include <nn/util/util_Arithmetic.h>
#include <nn/util/util_Vector.h>
#include <nn/util/util_Matrix.h>
#include <nn/util/util_Quaternion.h>

#define VFX_ERROR( ... )    NN_SDK_ASSERT( false, __VA_ARGS__ )

#if defined( NN_BUILD_CONFIG_OS_COS )
// GX2 を利用
#include <types.h>
#include <cafe/demo.h>
#include <cafe/gx2.h>
#endif

#include <nn/gfx.h>

//--------------------------------------------------------------------------------
//  エンディアン設定
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//! @brief  エンディアン指定
//--------------------------------------------------------------------------------
enum EndianMode
{
    EndianMode_Little = 0,                                              //!< リトルエンディアン
    EndianMode_Big = 1,                                                 //!< ビッグエンディアン
};

#if defined( NN_BUILD_CONFIG_ENDIAN_LITTLE )
    #define VFX_ENDIAN_LITTLE                   1                       // NOLINT(readability/define)
    static const EndianMode SystemEndianType    = EndianMode_Little;    //!< エンディアン型
#elif defined( NN_BUILD_CONFIG_ENDIAN_BIG )
    #define VFX_ENDIAN_BIG                      1                       // NOLINT(readability/define)
    static const EndianMode SystemEndianType    = EndianMode_Big;       //!< エンディアン型
#else
    #error "不明なエンディアン指定です。"
#endif

//--------------------------------------------------------------------------------
//  バイエンディアン指定
//--------------------------------------------------------------------------------
#if defined( NN_BUILD_CONFIG_SPEC_GENERIC )
    #define VFX_BI_ENDIAN                       1                       // NOLINT(readability/define)
#endif
