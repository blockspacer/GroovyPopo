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
#include <arm_neon.h>

//
// NEON を用いたベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::neon 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

namespace nn { namespace util { namespace neon {

// ベクトル
struct NN_ALIGNAS(8) Vector2fType
{
    float32x2_t _v;
};

struct NN_ALIGNAS(16) Vector3fType
{
    float32x4_t _v;
};

struct NN_ALIGNAS(16) Vector4fType
{
    float32x4_t _v;
};

// 行列・行優先
struct NN_ALIGNAS(8) MatrixRowMajor3x2fType
{
    float32x2x3_t _m;
};

struct NN_ALIGNAS(16) MatrixRowMajor4x3fType
{
    float32x4x4_t _m;
};

struct NN_ALIGNAS(16) MatrixRowMajor4x4fType
{
    float32x4x4_t _m;
};

// 行列・列優先
struct NN_ALIGNAS(16) MatrixColumnMajor3x2fType
{
    float32x4x2_t _m;
};

struct NN_ALIGNAS(16) MatrixColumnMajor4x3fType
{
    float32x4x3_t _m;
};

struct NN_ALIGNAS(16) MatrixColumnMajor4x4fType
{
    float32x4x4_t _m;
};

}}} // namespace nn::util::neon
