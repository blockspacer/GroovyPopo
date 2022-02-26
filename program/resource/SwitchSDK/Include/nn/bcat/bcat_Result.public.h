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
//! @defresult{bcat,引数の値が無効です。,122,1,1,ResultInvalidArgument}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidArgument, 122, 1, 2);
//! @defresult{bcat,対象が見つかりません。,122,2,2,ResultNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotFound, 122, 2, 3);
//! @defresult{bcat,対象はロックされています。,122,3,3,ResultLocked}
NN_DEFINE_ERROR_RANGE_RESULT(ResultLocked, 122, 3, 4);
//! @defresult{bcat,対象はすでにマウント済みです。,122,4,4,ResultAlreadyMounted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAlreadyMounted, 122, 4, 5);
//! @defresult{bcat,対象はマウントされていません。,122,5,5,ResultNotMounted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotMounted, 122, 5, 6);
//! @defresult{bcat,対象はすでに開かれています。,122,6,6,ResultAlreadyOpened}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAlreadyOpened, 122, 6, 7);
//! @defresult{bcat,対象は開かれていません。,122,7,7,ResultNotOpened}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotOpened, 122, 7, 8);
//! @defresult{bcat,インターネット利用要求が受理されていません。,122,8,8,ResultInternetRequestNotAccepted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInternetRequestNotAccepted, 122, 8, 9);
//! @defresult{bcat,ファイル、または、ディレクトリをこれ以上開くことはできません。,122,9,9,ResultTooManyHandles}
NN_DEFINE_ERROR_RANGE_RESULT(ResultTooManyHandles, 122, 9, 10);
//! @defresult{bcat,ストレージが見つかりません。,122,10,10,ResultStorageNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultStorageNotFound, 122, 10, 11);
//! @defresult{bcat,ストレージの空き容量が不足しています。,122,11,11,ResultUsableSpaceNotEnough}
NN_DEFINE_ERROR_RANGE_RESULT(ResultUsableSpaceNotEnough, 122, 11, 12);
//! @defresulta{bcat,処理がキャンセルされました。,122,20,29,ResultCanceled}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultCanceled, 122, 20, 30);
    //! @defresult{bcat,ユーザー操作によりキャンセルされました。,122,21,21,ResultCanceledByUser\, ResultCanceled}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCanceledByUser, 122, 21, 22);
//! @defresulta{bcat,サービスは利用できません。,122,30,39,ResultServiceUnavailable}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultServiceUnavailable, 122, 30, 40);
//! @defresulta{bcat,サービスは終了しました。,122,40,49,ResultServiceExpired}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultServiceExpired, 122, 40, 50);
//! @defresult{bcat,同期要求を出しすぎています。,122,50,50,ResultTooManySyncRequests}
NN_DEFINE_ERROR_RANGE_RESULT(ResultTooManySyncRequests, 122, 50, 51);
//! @defresult{bcat,パスフレーズが保存されていません。,122,80,80,ResultPassphraseNotSaved}
NN_DEFINE_ERROR_RANGE_RESULT(ResultPassphraseNotSaved, 122, 80, 81);
//! @defresult{bcat,データの検証に失敗しました。,122,81,81,ResultDataVerificationFailed}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDataVerificationFailed, 122, 81, 82);
//! @defresult{bcat,サポートしていないフォーマットのデータを検出しました。,122,82,82,ResultUnsupportedFormatDetected}
NN_DEFINE_ERROR_RANGE_RESULT(ResultUnsupportedFormatDetected, 122, 82, 83);
//! @defresult{bcat,API の呼出が許可されていません。,122,90,90,ResultNotPermitted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotPermitted, 122, 90, 91);
//! @defresult{bcat,無効な操作です。,122,98,98,ResultInvalidOperation}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidOperation, 122, 98, 99);
//! @defresult{bcat,未実装の機能です。,122,99,99,ResultNotImplemented}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 122, 99, 100);
//! @defresulta{bcat,HTTP エラーです。,122,1000,1999,ResultHttpError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultHttpError, 122, 1000, 2000);
//! @defresulta{bcat,サーバーエラーです。,122,2000,2999,ResultServerError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultServerError, 122, 2000, 3000);
