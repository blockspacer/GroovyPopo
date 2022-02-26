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

#include <nn/nn_Result.h>

/**
 *  @file
 *  @brief 似顔絵エディタを起動するためAPI群
 *  @details 似顔絵エディタを起動するためのAPIを定義します。
 */

namespace nn { namespace mii {

//! @name 似顔絵エディタ呼び出し関数
//! @{

/**
 * @brief 似顔絵エディタをMii追加モードで呼び出します。
 * @param[out] pOutAppenedIndex 追加したMiiのデータベース上のindexを返します(Source_Database)。
 * @return     処理結果を返します。
 * @retval     nn::ResultSuccess      Mii 1体を追加して閉じた
 * @retval     nn::mii::ResultCanceled 何らかの理由によりキャンセルされた
 * @pre pOutAppenedIndex != nullptr
 * @attention pOutAppenedIndexは、戻り値がnn::ResultSuccessの場合のみ有効です。
 * @details Miiを1体追加するモードで似顔絵エディタを起動します。
 *          pOutAppenedIndex は、Database::Get() で取得する の pElements の index 番号を表しています。
 *          
 */
nn::Result AppendMii(int* pOutAppenedIndex) NN_NOEXCEPT;

//! @}

}}
