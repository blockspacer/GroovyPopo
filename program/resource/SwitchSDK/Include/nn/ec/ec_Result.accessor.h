/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

// このファイルは自動生成ファイルです。
// 手動で変更しないでください。

// NOLINT(build/header_guard)

// package 'accessor'
//! @defresulta{ec,ShopServiceAccessor のエラー,164,200,299,ResultShopServiceAccessError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultShopServiceAccessError, 164, 200, 300);
    //! @defresult{ec,呼び出し元の要求に従って処理を中断した,164,200,200,ResultShopServiceAccessCanceled\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessCanceled, 164, 200, 201);
    //! @defresult{ec,出力先バッファ容量が不足している,164,201,201,ResultShopServiceAccessInsufficientBuffer\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessInsufficientBuffer, 164, 201, 202);
    //! @defresult{ec,非同期要求用ワークメモリ( ワークバッファ )が不足している,164,202,202,ResultShopServiceAccessInsufficientWorkMemory\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessInsufficientWorkMemory, 164, 202, 203);
    //! @defresult{ec,要求データ中にパラメータ使用不可の無効文字を検出した,164,203,203,ResultShopServiceAccessInvalidCharacter\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessInvalidCharacter, 164, 203, 204);
    //! @defresult{ec,同時発行可能な要求数の上限を超過した,164,204,204,ResultShopServiceAccessOverRequest\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessOverRequest, 164, 204, 205);
    //! @defresult{ec,HTTP 接続がタイムアウトした,164,205,205,ResultShopServiceAccessRequestTimeout\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessRequestTimeout, 164, 205, 206);
    //! @defresult{ec,年齢制限,164,206,206,ResultAgeRestriction\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultAgeRestriction, 164, 206, 207);
    //! @defresult{ec,ネットワーク時計が取れない,164,207,207,ResultNetworkTimeUnavailable\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNetworkTimeUnavailable, 164, 207, 208);
    //! @defresult{ec,サービスリソースの消費上限を超過した,164,219,219,ResultShopServiceAccessOutOfResource\, ResultShopServiceAccessError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShopServiceAccessOutOfResource, 164, 219, 220);
    //! @defresulta{ec,ショップサービスレスポンスエラー,164,220,299,ResultShopServiceResponseError\, ResultShopServiceAccessError}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultShopServiceResponseError, 164, 220, 300);
        //! @defresult{ec,エラービューア表示が必要なレスポンスエラーコードの受信,164,220,220,ResultShowErrorCodeRequired\, ResultShopServiceResponseError\, ResultShopServiceAccessError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultShowErrorCodeRequired, 164, 220, 221);
        //! @defresulta{ec,OwnedConsumableServiceItem サービスサーバーからのエラーレスポンス,164,230,239,ResultOwnedConsumableServiceItemResponseError\, ResultShopServiceResponseError\, ResultShopServiceAccessError}
        NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultOwnedConsumableServiceItemResponseError, 164, 230, 240);
            //! @defresult{ec,指定 right_id に紐付く権利が消費済,164,230,230,ResultOwnedConsumableServiceItemConsumedRights\, ResultOwnedConsumableServiceItemResponseError\, ResultShopServiceResponseError\, ResultShopServiceAccessError}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultOwnedConsumableServiceItemConsumedRights, 164, 230, 231);
