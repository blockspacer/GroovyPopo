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
 * @brief   疑似本体 ID の取得に関する API 宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_UuidTypes.h>

namespace nn { namespace oe {

//! @name 疑似本体 ID の取得に関する API
//! @{

/**
 * @brief   疑似本体 ID の取得
 *
 * @param[out] pOutId 疑似本体 ID の格納先アドレス
 *
 * @details
 *  疑似本体 ID を取得します。
 *  疑似本体 ID は pOutId で示される nn::util::Uuid 構造体に格納されます。
 *
 *  本 API で取得できる ID には以下のような特徴があります。
 *
 *  - アプリケーションと本体の組み合わせに対して固有な値である。よって、同一本体でもアプリケーションが異なれば値が異なる
 *  - 過去のプラットフォームで取得できた本体固有な ID と異なり、本体初期化を行っても値は変わらない
 *
 *  ユーザアカウントごとに固有の値を取得したい場合は ユーザ ID (nn::account::Uid) を使用し、
 *  本体固有の値を取得したい場合に限り、疑似本体 ID を使用して下さい。
 *  将来的にアプリケーションとセーブデータの部分引っ越し機能が提供された場合、
 *  その機能では疑似本体 ID は移行されず、引越し元と引越し先の本体で取得できる値は異なります。
**/
void GetPseudoDeviceId(nn::util::Uuid* pOutId) NN_NOEXCEPT;

//! @}

}} // namespace nn::oe
