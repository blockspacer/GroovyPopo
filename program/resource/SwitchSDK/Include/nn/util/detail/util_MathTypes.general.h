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

//
// SIMD ユニットを用いないベクトル・行列計算の実装を提供します。
//
// 注意：
//    アプリケーションは、 nn::util::general 名前空間にある API を直接利用してはいけません。
//    アプリケーションは、nn::util 直下の API を利用してください。
//

namespace nn { namespace util { namespace general {

// ベクトル
struct NN_ALIGNAS(16) Vector2fType
{
    float _v[2];
};

struct NN_ALIGNAS(16) Vector3fType
{
    float _v[3];
};

struct NN_ALIGNAS(16) Vector4fType
{
    float _v[4];
};

// 行列・行優先
struct NN_ALIGNAS(16) MatrixRowMajor3x2fType
{
    float _m[3][2];
};

struct NN_ALIGNAS(16) MatrixRowMajor4x3fType
{
    float _m[4][3];
};

struct NN_ALIGNAS(16) MatrixRowMajor4x4fType
{
    float _m[4][4];
};

// 行列・列優先
struct NN_ALIGNAS(16) MatrixColumnMajor3x2fType
{
    float _m[2][3];
};

struct NN_ALIGNAS(16) MatrixColumnMajor4x3fType
{
    float _m[3][4];
};

struct NN_ALIGNAS(16) MatrixColumnMajor4x4fType
{
    float _m[4][4];
};

}}} // namespace nn::util::general
