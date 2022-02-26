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

#include <nn/nn_ApplicationId.h>
#include <nn/nn_Common.h>
#include <nn/nn_TimeSpan.h>

namespace nn { namespace pdm {

/**
* @brief    アプリケーションのプレイ情報を表す構造体です。
*/
struct ApplicationPlayStatistics
{
    nn::ApplicationId applicationId;  //!< アプリケーションID
    nn::TimeSpan      totalPlayTime;  //!< 合計プレイ時間
    int32_t           totalPlayCount; //!< 合計プレイ回数
};

}}
