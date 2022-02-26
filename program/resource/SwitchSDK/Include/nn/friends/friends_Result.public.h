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

// package 'public'
//! @defresult{friends,ライブラリが初期化されていません。,121,1,1,ResultNotInitialized}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotInitialized, 121, 1, 2);
//! @defresult{friends,引数の値が無効です。,121,2,2,ResultInvalidArgument}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidArgument, 121, 2, 3);
//! @defresult{friends,ユーザーアカウントが Open 状態ではありません。,121,3,3,ResultUserNotOpened}
NN_DEFINE_ERROR_RANGE_RESULT(ResultUserNotOpened, 121, 3, 4);
//! @defresult{friends,ネットワークサービスアカウントが紐付けられていません。,121,4,4,ResultNetworkServiceAccountNotLinked}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNetworkServiceAccountNotLinked, 121, 4, 5);
//! @defresult{friends,自分のネットワークサービスアカウントを指定しました。,121,5,5,ResultOwnNetworkServiceAccountSpecified}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOwnNetworkServiceAccountSpecified, 121, 5, 6);
//! @defresult{friends,インターネット利用要求が受理されていません。,121,6,6,ResultInternetRequestNotAccepted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInternetRequestNotAccepted, 121, 6, 7);
//! @defresult{friends,非同期処理が実行していません。,121,7,7,ResultNotCalled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotCalled, 121, 7, 8);
//! @defresult{friends,非同期処理実行中です。,121,8,8,ResultCallInProgress}
NN_DEFINE_ERROR_RANGE_RESULT(ResultCallInProgress, 121, 8, 9);
//! @defresult{friends,非同期処理をキャンセルしました。,121,9,9,ResultCanceled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultCanceled, 121, 9, 10);
//! @defresult{friends,プロフィール画像のキャッシュが見つかりません。,121,10,10,ResultProfileImageCacheNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultProfileImageCacheNotFound, 121, 10, 11);
//! @defresult{friends,メモリが確保できませんでした。,121,11,11,ResultOutOfMemory}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfMemory, 121, 11, 12);
//! @defresult{friends,バッファに空きがありません。,121,12,12,ResultOutOfResource}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfResource, 121, 12, 13);
//! @defresult{friends,指定したキーは予約されています。,121,13,13,ResultReservedKey}
NN_DEFINE_ERROR_RANGE_RESULT(ResultReservedKey, 121, 13, 14);
//! @defresult{friends,指定したキーはすでに登録されています。,121,14,14,ResultDuplicatedKey}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDuplicatedKey, 121, 14, 15);
//! @defresult{friends,通知はありません。,121,15,15,ResultNotificationNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotificationNotFound, 121, 15, 16);
//! @defresult{friends,いっしょに遊んだユーザーの登録キーが壊れています。,121,21,21,ResultPlayHistoryRegistrationKeyBroken}
NN_DEFINE_ERROR_RANGE_RESULT(ResultPlayHistoryRegistrationKeyBroken, 121, 21, 22);
//! @defresult{friends,自分のいっしょに遊んだユーザーの登録キーです。,121,22,22,ResultOwnPlayHistoryRegistrationKey}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOwnPlayHistoryRegistrationKey, 121, 22, 23);
//! @defresult{friends,呼び出したアプレットがキャンセルされました。,121,30,30,ResultAppletCanceled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAppletCanceled, 121, 30, 31);
//! @defresult{friends,アプリケーション情報が登録されていません。,121,40,40,ResultApplicationInfoNotRegistered}
NN_DEFINE_ERROR_RANGE_RESULT(ResultApplicationInfoNotRegistered, 121, 40, 41);
//! @defresult{friends,API の呼出が許可されていません。,121,90,90,ResultNotPermitted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotPermitted, 121, 90, 91);
//! @defresult{friends,未実装の機能です。,121,99,99,ResultNotImplemented}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 121, 99, 100);
//! @defresulta{friends,応答フォーマットエラーです。,121,100,199,ResultResponseFormatError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultResponseFormatError, 121, 100, 200);
//! @defresulta{friends,HTTP エラーです。,121,1000,1999,ResultHttpError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultHttpError, 121, 1000, 2000);
//! @defresulta{friends,サーバーエラーです。,121,2000,3999,ResultServerError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultServerError, 121, 2000, 4000);
