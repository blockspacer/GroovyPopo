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
*   @brief  デバッグ用途の SD カードファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

//! @name デバッグ用途 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      SD カードを扱うファイルシステムを指定したマウント名でマウントします。
*
*   @param[in]  name       マウント名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*/
Result MountSdCardForDebug(const char* name) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
