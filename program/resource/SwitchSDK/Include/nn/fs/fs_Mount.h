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
*   @brief  マウント関連のファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace fs {

/**
* @brief マウント時に指定する名前の最大サイズ（単位は Byte 数）です。終端の NULL 文字を含みません。
*/
const int MountNameLengthMax = 15;

//! @name ファイルシステム関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      マウント済みのファイルシステムをアンマウントし、リソースを解放します。
*
*   @param[in]  name       マウント時に指定したマウント名
*
*   @return     なし
*/
void Unmount(const char* name) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs

