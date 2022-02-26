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

#include <nn/vfx/vfx_Data.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @brief                          エミッタ時間 キーフレームアニメーションを計算します。
//! @param[out] pOutValue           キーフレームアニメーション結果
//! @param[out] pOutIsAnimeOver     最終キーを過ぎているかどうか
//! @param[in]  pAnimationParam     アニメーションパラメータへのポインタ
//! @param[in]  time                アニメーション時刻
//---------------------------------------------------------------------------
void CalculateEmitterKeyFrameAnimation( nn::util::Float3* pOutValue, bool* pOutIsAnimeOver, const ResAnimEmitterKeyParamSet* pAnimationParam, float time ) NN_NOEXCEPT;

} // namespace detail
} // namespace vfx
} // namespace nn
