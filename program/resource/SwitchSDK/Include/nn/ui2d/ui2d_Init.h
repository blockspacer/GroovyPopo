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

#include <nn/nn_Allocator.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief レイアウトライブラリの初期化を行います。
//!
//! @details
//! レイアウトライブラリを使用する前に必ず呼び出してください。
//!
//! @param[in] pAllocateFunction   メモリ確保関数のポインタです。
//! @param[in] pFreeFunction       メモリ解放関数のポインタです。
//! @param[in] pUserData           メモリ確保・確保関数に引数として渡されるユーザーデータです。
//!
//! @sa Layout::SetAllocator
//!
//---------------------------------------------------------------------------
void Initialize(nn::AlignedAllocateFunctionWithUserData pAllocateFunction, nn::FreeFunctionWithUserData pFreeFunction, void* pUserData);

} // namespace nn::ui2d
} // namespace nn
