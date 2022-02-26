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


namespace nn
{
namespace nifm
{
    //! @name テスト用通信制御 API
    //! @{

    /**
     * @brief   無線通信の有効・無効を制御します。
     *
     * @param[in]   isEnabled    無線通信の有効・無効を指定します。
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 処理に成功しました。
     *   }
     * @endretresult
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @details
     *  この関数はテスト用であり、成功させるには事前に DevMenu の Network ページにある
     *  Testing mode enabled のチェックボックスにチェックを入れておく必要があります。
     *  この許可設定をおこなえない製品上では常に失敗します。@n
     *
     *  実際に無線通信が停止・再開するのは遅延する可能性があります。@n
     *
     *  この関数で制御される無線通信の有効・無効の設定は、ユーザー操作による無線通信の有効・無効と共通です。@n
     *
     *  設定値は永続化されず、システムの再起動でユーザー操作によって設定された値に戻ります。
     */
    nn::Result SetWirelessCommunicationEnabledForTest(bool isEnabled) NN_NOEXCEPT;

    /**
     * @brief   有線通信の有効・無効を制御します。
     *
     * @param[in]   isEnabled    有線通信の有効・無効を指定します。
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 処理に成功しました。
     *   }
     * @endretresult
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @details
     *  この関数はテスト用であり、成功させるには事前に DevMenu の Network ページにある
     *  Testing mode enabled のチェックボックスにチェックを入れておく必要があります。
     *  この許可設定をおこなえない製品上では常に失敗します。@n
     *
     *  実際に有線通信が停止・再開するのは遅延する可能性があります。@n
     *
     *  設定値は永続化されず、システムの再起動で有効に戻ります。
     */
    nn::Result SetEthernetCommunicationEnabledForTest(bool isEnabled) NN_NOEXCEPT;

    //! @}

}
}
