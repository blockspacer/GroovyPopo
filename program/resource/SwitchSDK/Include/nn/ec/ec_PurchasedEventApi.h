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
 * @brief サーバー側の商品購入イベントを扱う API の定義
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/ec/ec_ShopTypes.h>
#include <nn/os/os_SystemEvent.h>

namespace nn { namespace ec {

/**
 * @brief 購入された商品の情報を表す構造体です。
 *
 * @see nn::ec::PopPurchasedItemInfo()
 */
struct PurchasedItemInfo
{
    /**
     * @brief 商品の種類の定義です。
     */
    enum class Type : int32_t
    {
        Subscription    = 0,    //!< 期間型
        Consumable      = 1,    //!< 消費型
    };

    Type type; //!< 商品の種類
    nn::account::NetworkServiceAccountId nsaId; //!< 商品購入者のネットワークサービスアカウントの ID

    /**
     * @brief   期間型サービスのコースを識別する ID を取得します。
     *
     * @pre
     *  - type == nn::ec::PurchasedItemInfo::Type::Subscription
     *
     * @details
     *  コース ID を表す文字列は NULL 終端が保証されます。
     */
    const nn::ec::CourseId& GetCourseId() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_EQUAL(type, PurchasedItemInfo::Type::Subscription);
        return _courseId;
    }

    nn::ec::CourseId _courseId;
};

/**
 * @name ショップの商品購入を知るための関数
 *
 *  NX デバイス上のショップアプリでの商品購入や、任天堂ホームページでの商品購入の検知を行うための API 群です。@n
 *  「いつもあそぶ本体」に登録していない本体であっても商品の購入を検知可能です。
 *
 *  追加コンテンツの購入検知には対応していません。@n
 *  購入した追加コンテンツが利用可能になるタイミングを知るには nn::aoc::GetAddOnContentListChangedEvent() を利用してください。
 *
 * @{
 */

/**
 * @brief   ショップの商品購入を検知するための機能を初期化します。
 *
 * @details
 *  本関数の実行後に行われるNX デバイス上のショップアプリでの商品購入や、
 *  任天堂ホームページでの商品購入の検知が可能になります。
 *
 *  ネットワーク環境やサーバーの状況によっては、
 *  商品購入を検知できないことや、検知のタイミングがいくらか遅延することもあり得ます。
 *
 *  既に本関数が実行された状態で呼び出しても副作用はありません。
 *
 * @see nn::ec::GetPurchasedEvent(), nn::ec::PopPurchasedItemInfo()
 */
void InitializePurchasedEvent() NN_NOEXCEPT;

/**
 * @brief   ショップの商品購入のたびにシグナルするイベントを取得します。
 *
 * @pre
 *  - nn::ec::InitializePurchasedEvent() を実行済
 *
 * @details
 *  ショップの商品が購入されるとシグナルするイベントを取得します。
 *
 *  イベントのシグナル後、 nn::ec::PopPurchasedItemInfo() によって購入された商品の情報を把握できます。
 *
 *  イベントのクリアモードは nn::os::ClearMode_ManualClear です。@n
 *  イベントはアプリケーションのネットワーク利用宣言に関わらずシグナルされる可能性があります。
 *
 * @see nn::ec::PopPurchasedItemInfo()
 */
nn::os::SystemEvent& GetPurchasedEvent() NN_NOEXCEPT;

/**
 * @brief ショップで購入された商品の情報を取り出します。
 *
 * @pre
 *  - nn::ec::InitializePurchasedEvent() を実行済
 *  - pOut != nullptr
 *
 * @return  情報を取得できたかどうか。
 *
 * @details
 *  システムが保持している商品購入の情報を 1 件ずつ取り出します。
 *
 *  取りこぼし防止のため、本関数が false を返すまで繰り返し呼び出すことを推奨します。
 *
 * @see nn::ec::GetPurchasedEvent()
 */
bool PopPurchasedItemInfo(PurchasedItemInfo* pOut) NN_NOEXCEPT;

/**
 * @}
 */

}}
