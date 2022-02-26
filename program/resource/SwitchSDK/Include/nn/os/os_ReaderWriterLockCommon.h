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
 * @file
 * @brief   Readers-Writer ロックに関する共通定義
 */

#pragma once

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//----------------------------------------------------------------------------
/**
 * @brief   Readers-Writer ロックのロックカウントの最大値を表す定数です。
 */
const int   ReaderWriterLockCountMax = 65535;


}} // namespace nn::os

