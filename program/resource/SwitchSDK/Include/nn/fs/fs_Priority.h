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
*   @brief  アクセス優先度 関連のファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

/**
* @brief アクセス優先度です。
*/
enum Priority
{
    Priority_Realtime,      //!< リアルタイム処理に適した高優先度でアクセスすることを指定します。
    Priority_Normal,        //!< 通常優先度でアクセスすることを指定します。
    Priority_Low            //!< 低優先度でアクセスすることを指定します。
};

//! @name アクセス優先度 関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      アクセス優先度を設定します。
*
*   @details    現在のスレッドにおけるアクセス優先度を設定します。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{277373520, アクセス優先度} を参照してください。@n
*
*   @param[in]  priority            アクセス優先度
*
*   @return     なし
*/
void SetPriorityOnCurrentThread(Priority priority) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      アクセス優先度を取得します。
*
*   @details    現在のスレッドにおけるアクセス優先度を取得します。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{277373520, アクセス優先度} を参照してください。@n
*
*   @return     アクセス優先度
*/
Priority GetPriorityOnCurrentThread() NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
