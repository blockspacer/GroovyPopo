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

namespace nn { namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       Opus でサポートしているビットレート最小値を取得します。
 * @param[in]   channelCount  音源のチャンネル数
 * @return      ビットレート最小値を返します。
 * @pre
 * - channelCount == 1 || channelCount == 2
 */
int GetOpusBitRateMin(int channelCount) NN_NOEXCEPT;

/**
 * @brief       Opus でサポートしているビットレート最大値を取得します。
 * @param[in]   channelCount  音源のチャンネル数
 * @return      ビットレート最大値を返します。
 * @pre
 * - channelCount == 1 || channelCount == 2
 */
int GetOpusBitRateMax(int channelCount) NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::codec
