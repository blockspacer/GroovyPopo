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

#include <nn/nifm/nifm_Common.h>

#include <nn/nifm/nifm_TypesProxySetting.h>

namespace nn
{
namespace nifm
{
    //! @name 接続情報取得 API
    //! @{

    /**
     * @brief   現在の接続に利用されているプロキシ設定を取得します。
     *
     * @param[out]  pOutProxySetting    取得されるプロキシ設定を格納するバッファへのポインタ
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 処理に成功しました。
     *   }
     * @endretresult
     *
     * @pre
     *  - ライブラリが初期化済み
     *  - pOutProxySetting != nullptr
     *
     * @details
     *  ネットワークに接続されていない状態ではこの関数は失敗を返し、その際に得られるプロキシ設定の内容は不定です。
     *  直前にネットワーク接続があることを確認しても外的要因により不意の切断が発生することがあるため、必ずこの関数の成否を確認してください。@n
     *
     *  NintendoSDK に収録された libcurl ライブラリを利用する場合、プロキシの設定は自動で反映されるため、この関数から取得した設定を明示的に登録する必要はありません。
     */
    nn::Result GetCurrentProxySetting(ProxySetting* pOutProxySetting) NN_NOEXCEPT;

    //! @}

}
}
