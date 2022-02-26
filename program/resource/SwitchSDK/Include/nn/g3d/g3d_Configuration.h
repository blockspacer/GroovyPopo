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

#if !defined(NN_SDK_BUILD_DEBUG) && !defined(NN_SDK_BUILD_DEVELOP) && !defined(NN_SDK_BUILD_RELEASE)
#error "Build target is not defined."
#endif

#include <nn/nn_Common.h>
#include <nn/TargetConfigs/build_Base.h>

#define NN_G3D_CONFIG_PERF_ENABLE (0)
#define NN_G3D_CONFIG_PERF_LEVEL  (0) // 0 はレベル指定の測定を無効にします。
#define NN_G3D_CONFIG_MAX_SHADER_KEY (32) // シェーダーキーの最大長です

#if defined(NN_BUILD_CONFIG_OS_COS)
#include <cafe/os.h>
#include <ppc_ghs.h>
#include <ppc_ps.h>
#endif

#include <nn/g3d/g3d_Define.h>
#include <nn/g3d/g3d_Assert.h>

