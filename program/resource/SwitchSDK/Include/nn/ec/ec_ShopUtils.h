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
 * @brief ショップ用のユーティティ機能を提供するAPIの定義
 */

#pragma once

#include <nn/ec/ec_ShopTypes.h>

namespace nn { namespace ec {

/**
 * @name ショップ用の機能で使われる値の変換関数
 * @{
 */

/**
 * @brief 文字列で表された値を NsUid として返します。
 * @param[in] string NsUid の文字列表現
 * @return NsUid 型の値。
 *   変換できなかった場合は NsUid::GetInvalidId() の値が返ります。
 */
NsUid MakeNsUid(const char* string) NN_NOEXCEPT;

/**
 * @}
 */

}}
