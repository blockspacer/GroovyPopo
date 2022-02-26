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
    //! @name ライブラリの初期化・終了 API
    //! @{

    /**
     * @brief   ネットワーク接続管理ライブラリの初期化をおこない、機能を使用可能な状態にします。
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 処理に成功しました。
     *   }
     * @endretresult
     *
     * @post
     *   - ライブラリが初期化済み
     *
     * @details
     *  ネットワーク接続管理ライブラリが提供する API は、個別に明示されていないかぎり、利用前にこの関数でライブラリを初期化しておく必要があります。
     *  既に初期化済みの場合も成功を返します。
     */
    nn::Result Initialize() NN_NOEXCEPT;

    //! @}
}
}
