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

#include <nn/nn_Common.h>
#include <nn/TargetConfigs/build_Base.h>

//--------------------------------------------------------------------------------------------------
// バージョン

// g3dのバージョン
#define NN_G3D_VERSION_MAJOR ( 1 ) // NOLINT(readability/define)
#define NN_G3D_VERSION_MINOR ( 0 ) // NOLINT(readability/define)
#define NN_G3D_VERSION_MICRO ( 0 ) // NOLINT(readability/define)

// 対応しているモデルバイナリーのバージョン
#define NN_G3D_MODEL_BINARY_VERSION_MAJOR ( 9 ) // NOLINT(readability/define)
#define NN_G3D_MODEL_BINARY_VERSION_MINOR ( 0 ) // NOLINT(readability/define)
#define NN_G3D_MODEL_BINARY_VERSION_MICRO ( 0 ) // NOLINT(readability/define)

// 対応しているシェーダーバイナリーのバージョン
#define NN_G3D_SHADER_BINARY_VERSION_MAJOR ( 8 ) // NOLINT(readability/define)
#define NN_G3D_SHADER_BINARY_VERSION_MINOR ( 0 ) // NOLINT(readability/define)
#define NN_G3D_SHADER_BINARY_VERSION_MICRO ( 0 ) // NOLINT(readability/define)

//--------------------------------------------------------------------------------------------------
// プラットフォーム

#if !defined(NN_BUILD_CONFIG_OS_WIN32) && !defined(NN_BUILD_CONFIG_OS_COS) && !defined(NN_BUILD_CONFIG_OS_HORIZON)
#error "platform is not defined."
#endif

//--------------------------------------------------------------------------------------------------
// 名前空間

namespace nn { namespace g3d {

namespace detail {}
using namespace nn::g3d::detail;

//--------------------------------------------------------------------------------------------------
// 基本型

#if defined(NN_BUILD_CONFIG_OS_HORIZON)
typedef uint16_t char16;
#else
typedef wchar_t char16;
#endif

//--------------------------------------------------------------------------------------------------
// 共通定数

const int MatrixVectorAlignment = 16;

}} // namespace nn::g3d


