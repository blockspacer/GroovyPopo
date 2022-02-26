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
*   @brief  追加コンテンツの API 宣言
*/

#pragma once

#include <nn/aoc/aoc_Types.h>
#include <nn/os/os_SystemEvent.h>

namespace nn { namespace aoc {

//! @name 追加コンテンツの列挙
//! @{

    /**
     * @brief   インストール済みで権利を保有する追加コンテンツの数を返します。
     *
     * @return  インストール済みで権利を保有する追加コンテンツの数が返ります。
     *
     * @details
     *  インストール済みで権利を保有する追加コンテンツの数を取得します。
     *
     */
    int CountAddOnContent() NN_NOEXCEPT;


    /**
     * @brief   インストール済みで権利を保有する追加コンテンツの追加コンテンツインデックスを列挙します。
     *
     * @param[out]  outIndices      列挙されたインデックスの格納先バッファ。
     * @param[in]   offset          列挙する追加コンテンツインデックスのオフセットを指定します。
     * @param[in]   count           列挙する最大数を指定します。
     *
     * @return  列挙した追加コンテンツインデックスの数が返ります。
     *
     * @pre
     *  - outIndices の最大要素数 >= count
     *  - offset >= 0
     *
     * @details
     *  インストール済みで権利を保有する追加コンテンツの追加コンテンツインデックスを列挙します。
     *  列挙された追加コンテンツインデックスの offset 番目から count で指定された数の
     *  追加コンテンツインデックスを outIndices に格納します。
     *  outIndices で指定したバッファの要素数を超える count を指定した場合の動作は未定義です。
     *  列挙される追加コンテンツインデックスは昇順にソートされています。
     */
    int ListAddOnContent(   AddOnContentIndex*  outIndices,
                            int                 offset,
                            int                 count ) NN_NOEXCEPT;

    /**
     * @brief   利用できる追加コンテンツが変化したことを通知するイベントを取得します。
     *
     * @param[out]  outEvent      イベントオブジェクト
     *
     * @details
     *  イベント取得後に更新された場合のみ通知されます。
     *  確実に利用可能なコンテンツを検出するためには、
     *  本 API でイベントを取得したのちに、利用可能な追加コンテンツの列挙を行ってください。
     *
     *  本イベントは AutoClear イベントです。
     *  一度通知を受けた後、次の利用できる追加コンテンツの変化の検知するためには、
     *  このイベントを再利用してください。
     *
     *  実行時に追加コンテンツのインストールを行われるようにするには、
     *  別途 nmeta に指定が必要です。
     */
     void GetAddOnContentListChangedEvent(os::SystemEvent* outEvent) NN_NOEXCEPT;

//! @}
}} // namespace nn::aoc
