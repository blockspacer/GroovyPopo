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

#include <nn/nifm/nifm_TypesIpSetting.h>
#include <nn/socket/socket_Types.h>

namespace nn
{
namespace nifm
{
    //! @name 接続情報取得 API
    //! @{

    /**
     * @brief   利用中の IP アドレスを取得します。
     *
     * @param[out] pOutInAddr   利用中の IP アドレスが格納されるバッファへのポインタ
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 処理に成功しました。
     *   }
     * @endretresult
     *
     * @pre
     *   - ライブラリが初期化済み
     *   - pOutInAddr != nullptr
     *
     * @details
     *  インターネット通信に利用しているネットワークインターフェースに割り当てられた IP アドレスを取得します。@n
     *
     *  インターネット通信中以外に呼んだ場合、関数は失敗を返します。
     */
    nn::Result GetCurrentPrimaryIpAddress(nn::socket::InAddr* pOutInAddr) NN_NOEXCEPT;

    /**
     * @copydoc GetCurrentPrimaryIpAddress(nn::socket::InAddr*)
     */
    nn::Result GetCurrentPrimaryIpAddress(in_addr* pOutInAddr) NN_NOEXCEPT;


    /**
     * @brief 　利用中の IP アドレスなどの情報を取得します。
     *
     * @param[out] pOutIpAddress       利用中の IP アドレスが格納されるバッファへのポインタ
     * @param[out] pOutSubnetMask      利用中のサブネットマスクが格納されるバッファへのポインタ
     * @param[out] pOutDefaultGateway  利用中のデフォルトゲートウェイアドレス格納されるバッファへのポインタ
     * @param[out] pOutPreferredDns    利用中の優先 DNS アドレスが格納されるバッファへのポインタ
     * @param[out] pOutAlternateDns    利用中の代替 DNS アドレスが格納されるバッファへのポインタ
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 処理に成功しました。
     *   }
     * @endretresult
     *
     * @pre
     *   - ライブラリが初期化済み
     *   - pOutIpAddress != nullptr
     *   - pOutSubnetMask != nullptr
     *   - pOutDefaultGateway != nullptr
     *   - pOutPreferredDns != nullptr
     *   - pOutAlternateDns != nullptr
     *
     * @details
     *  インターネット通信に利用しているネットワークインターフェースに割り当てられた IP アドレスなどの情報を取得します。@n
     *
     *  インターネット通信中以外に呼んだ場合、関数は失敗を返します。
     */
    nn::Result GetCurrentIpConfigInfo(nn::socket::InAddr* pOutIpAddress, nn::socket::InAddr* pOutSubnetMask, nn::socket::InAddr* pOutDefaultGateway, nn::socket::InAddr* pOutPreferredDns, nn::socket::InAddr* pOutAlternateDns) NN_NOEXCEPT;

    /**
     * @copydoc GetCurrentIpConfigInfo(nn::socket::InAddr*,nn::socket::InAddr*,nn::socket::InAddr*,nn::socket::InAddr*,nn::socket::InAddr*)
     */
    nn::Result GetCurrentIpConfigInfo(in_addr* pOutIpAddress, in_addr* pOutSubnetMask, in_addr* pOutDefaultGateway, in_addr* pOutPreferredDns, in_addr* pOutAlternateDns) NN_NOEXCEPT;

    //! @}
}
}
