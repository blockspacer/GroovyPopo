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
 * @brief ショップサービスアクセスユーティリティ機能で利用する型定義
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_StaticAssert.h>

namespace nn { namespace ec {

/**
 * @brief 接続先ショップサービス要求定義。
 */
struct ShopService
{
    /**
     * @brief   接続先ショップサービスサーバー種別の定義。
     */
    enum Type
    {
        Type_OwnedConsumableServiceItem,    //!< 消費型サービスアイテムサービスサーバーへの接続要求。
        Type_Catalog,                       //!< ショップアイテムカタログサービスサーバーへの接続要求。
        Type_EndOfConstants,                // 末尾判断定数。利用可能な要求メソッドではありません。
    };

    /**
     * @brief   ショップサービスサーバーAPI 要求 HTTP メソッド種別の定義。
     */
    enum Method
    {
        Method_Get,             //!< GET    HTTPメソッド要求種別。
        Method_Post,            //!< POST   HTTPメソッド要求種別。
        Method_Put,             //!< PUT    HTTPメソッド要求種別。
        Method_Delete,          //!< DELETE HTTPメソッド要求種別。
        Method_EndOfConstants,  // 末尾判断定数。利用可能な要求メソッドではありません。
    };

    Type    type;   //!< 接続先ショップサービスサーバー種別。

    /**
     * @brief       コンストラクタ。
     *
     * @param[in]   type_   希望する接続先ショップサービスサーバー種別を指定します。
     */
    NN_IMPLICIT ShopService(const Type type_) NN_NOEXCEPT : type(type_) {}
};

}}

