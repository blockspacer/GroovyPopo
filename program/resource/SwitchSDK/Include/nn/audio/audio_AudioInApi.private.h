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

namespace nn { namespace audio {

/**
 * @name オーディオ入力
 * @{
 */

/*
 * @brief       AudioIn の機能を有効にします。
 */
NN_DEPRECATED void SetAudioInEnabled(bool enable) NN_NOEXCEPT;

/*
 * @brief       AudioIn の機能が有効かどうかを取得します。
 */
NN_DEPRECATED bool IsAudioInEnabled() NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::audio
