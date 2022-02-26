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
 * @brief クライアントにおける消費型サービスアイテムの型を定義します。
 */

#pragma once
#include <nn/util/util_TypedStorage.h>
#include <nn/util/util_StringUtil.h>

namespace nn { namespace ec {

/**
    @brief ライブラリの内部処理に必要なワークバッファの最小メモリサイズ
*/
static const size_t RequiredWorkMemorySizeForConsumableServiceItem = 256 * 1024;

/**
    @brief シリアライズされた権利情報を保存するのに必要なセーブデータサイズ
*/
static const size_t RequiredUserSaveDataSizeForConsumableServiceItem = 32 * 1024;

/**
    @brief 購入した消費型サービスアイテムを識別する ID です。最大 16 文字を文字列として保持します。

    @details
        nn::ec::ConsumableServiceItemId は、消費型サービスアイテムへの各種リクエストを行うために必要なサーバーパラメータである、アイテム ID に相当します。@n
        アイテム ID はショップに登録する必要があります。@n
        登録方法や、ID の詳細は『Nintendo Switch サービスアイテム プログラミングマニュアル』の「開発を始めるための準備」を参照してください。
*/
struct ConsumableServiceItemId
{
    static const int Length = 16;   //!< ID を示す文字列の長さ
    char value[Length + 1];         //!< ID を示す文字列

    /**
        @brief      ID の有効性を評価します

        @return     ID が有効かどうか
    */
    bool IsValid() const NN_NOEXCEPT;
};

}}
