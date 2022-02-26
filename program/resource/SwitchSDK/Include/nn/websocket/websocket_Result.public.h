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
//! @defresult{websocket,引数が不正です。,223,1,1,ResultInvalidArgument}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidArgument, 223, 1, 2);
//! @defresult{websocket,メモリの確保に失敗しました。,223,2,2,ResultOutOfMemory}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfMemory, 223, 2, 3);
//! @defresult{websocket,バッファが不足しています。,223,3,3,ResultInsufficientBuffer}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInsufficientBuffer, 223, 3, 4);
//! @defresult{websocket,これ以上データを送信できません。,223,4,4,ResultNoMoreMessage}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNoMoreMessage, 223, 4, 5);
//! @defresult{websocket,データが破損しています。,223,5,5,ResultDataCorrupted}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDataCorrupted, 223, 5, 6);
//! @defresult{websocket,この機能はサポートしていません。,223,98,98,ResultNotSupported}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotSupported, 223, 98, 99);
//! @defresult{websocket,内部エラーが発生しました。,223,99,99,ResultInternalError}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInternalError, 223, 99, 100);
//! @defresulta{websocket,通信エラーです。,223,1000,2999,ResultCommunicationError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultCommunicationError, 223, 1000, 3000);
    //! @defresult{websocket,インターネット利用要求が受理されていません。,223,1000,1000,ResultInternetRequestNotAccepted\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInternetRequestNotAccepted, 223, 1000, 1001);
    //! @defresult{websocket,処理がタイムアウトしました。,223,1001,1001,ResultOperationTimedOut\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultOperationTimedOut, 223, 1001, 1002);
    //! @defresult{websocket,処理がキャンセルされました。,223,1002,1002,ResultOperationCanceled\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultOperationCanceled, 223, 1002, 1003);
    //! @defresult{websocket,URI が不正です。,223,1100,1100,ResultInvalidUri\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidUri, 223, 1100, 1101);
    //! @defresult{websocket,サポートしていない URI です。,223,1101,1101,ResultUnsupportedUri\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUnsupportedUri, 223, 1101, 1102);
    //! @defresult{websocket,レスポンスが不正です。,223,1200,1200,ResultInvalidResponse\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidResponse, 223, 1200, 1201);
    //! @defresult{websocket,レスポンスヘッダーが長すぎます。,223,1201,1201,ResultResponseHeaderTooLong\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultResponseHeaderTooLong, 223, 1201, 1202);
    //! @defresult{websocket,圧縮拡張の合意が得られませんでした。,223,1202,1202,ResultCompressionExtensionNotAgreed\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCompressionExtensionNotAgreed, 223, 1202, 1203);
    //! @defresult{websocket,ソケットエラーが発生しました。,223,1300,1300,ResultSocketError\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSocketError, 223, 1300, 1301);
    //! @defresult{websocket,送信に失敗しました。,223,1301,1301,ResultSendError\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSendError, 223, 1301, 1302);
    //! @defresult{websocket,受信に失敗しました。,223,1302,1302,ResultReceiveError\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultReceiveError, 223, 1302, 1303);
    //! @defresult{websocket,接続が閉じられました。,223,1303,1303,ResultConnectionClosed\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionClosed, 223, 1303, 1304);
    //! @defresult{websocket,ホストの名前解決に失敗しました。,223,1400,1400,ResultCouldntResolveHost\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCouldntResolveHost, 223, 1400, 1401);
    //! @defresult{websocket,ホストに接続できませんでした。,223,1401,1401,ResultCouldntConnectHost\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCouldntConnectHost, 223, 1401, 1402);
    //! @defresult{websocket,プロキシの名前解決に失敗しました。,223,1500,1500,ResultCouldntResolveProxy\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCouldntResolveProxy, 223, 1500, 1501);
    //! @defresult{websocket,プロキシに接続できませんでした。,223,1501,1501,ResultCouldntConnectProxy\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCouldntConnectProxy, 223, 1501, 1502);
    //! @defresult{websocket,プロキシ認証に失敗しました。,223,1502,1502,ResultProxyAuthorizationFailed\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultProxyAuthorizationFailed, 223, 1502, 1503);
    //! @defresult{websocket,プロキシ認証が必要です。,223,1503,1503,ResultProxyAuthorizationRequired\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultProxyAuthorizationRequired, 223, 1503, 1504);
    //! @defresult{websocket,SSL コンテキストの作成に失敗しました。,223,1600,1600,ResultSslContextCreationFailed\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSslContextCreationFailed, 223, 1600, 1601);
    //! @defresult{websocket,SSL コネクションの作成に失敗しました。,223,1601,1601,ResultSslConnectionCreationFailed\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSslConnectionCreationFailed, 223, 1601, 1602);
    //! @defresult{websocket,SSL ハンドシェイクに失敗しました。,223,1610,1629,ResultSslHandshakeFailed\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSslHandshakeFailed, 223, 1610, 1630);
        //! @defresult{websocket,サーバー証明書のホスト名検証に失敗しました。,223,1611,1611,ResultHostNameVerificationFailed\, ResultSslHandshakeFailed\, ResultCommunicationError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultHostNameVerificationFailed, 223, 1611, 1612);
        //! @defresult{websocket,サーバー証明書の有効期限が切れています。,223,1612,1612,ResultServerCertificateRevoked\, ResultSslHandshakeFailed\, ResultCommunicationError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultServerCertificateRevoked, 223, 1612, 1613);
        //! @defresult{websocket,サーバー証明書が失効しています。,223,1613,1613,ResultServerCertificateExpired\, ResultSslHandshakeFailed\, ResultCommunicationError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultServerCertificateExpired, 223, 1613, 1614);
        //! @defresult{websocket,サーバー証明書の発行者が不明、または、自己署名されています。,223,1614,1614,ResultUnknownCa\, ResultSslHandshakeFailed\, ResultCommunicationError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultUnknownCa, 223, 1614, 1615);
        //! @defresult{websocket,サーバー証明書に問題があります。,223,1615,1615,ResultServerCertificateProblem\, ResultSslHandshakeFailed\, ResultCommunicationError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultServerCertificateProblem, 223, 1615, 1616);
        //! @defresult{websocket,クライアント証明書に問題があります。,223,1616,1616,ResultClientCertificateProblem\, ResultSslHandshakeFailed\, ResultCommunicationError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultClientCertificateProblem, 223, 1616, 1617);
    //! @defresult{websocket,HTTP ステータスコード 100 番台のハンドルしていないステータスです。,223,2199,2199,ResultHttpStatus1xx\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus1xx, 223, 2199, 2200);
    //! @defresult{websocket,HTTP ステータスコード 200 番台のハンドルしていないステータスです。,223,2299,2299,ResultHttpStatus2xx\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus2xx, 223, 2299, 2300);
    //! @defresult{websocket,HTTP ステータスコード 300 番台のハンドルしていないステータスです。,223,2399,2399,ResultHttpStatus3xx\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus3xx, 223, 2399, 2400);
    //! @defresult{websocket,HTTP ステータスコード 400 (Bad Request),223,2400,2400,ResultHttpStatus400\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus400, 223, 2400, 2401);
    //! @defresult{websocket,HTTP ステータスコード 401 (Unauthorized),223,2401,2401,ResultHttpStatus401\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus401, 223, 2401, 2402);
    //! @defresult{websocket,HTTP ステータスコード 403 (Forbidden),223,2403,2403,ResultHttpStatus403\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus403, 223, 2403, 2404);
    //! @defresult{websocket,HTTP ステータスコード 404 (Not Found),223,2404,2404,ResultHttpStatus404\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus404, 223, 2404, 2405);
    //! @defresult{websocket,HTTP ステータスコード 405 (Method Not Allowed),223,2405,2405,ResultHttpStatus405\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus405, 223, 2405, 2406);
    //! @defresult{websocket,HTTP ステータスコード 408 (Request Timeout),223,2408,2408,ResultHttpStatus408\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus408, 223, 2408, 2409);
    //! @defresult{websocket,HTTP ステータスコード 409 (Conflict),223,2409,2409,ResultHttpStatus409\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus409, 223, 2409, 2410);
    //! @defresult{websocket,HTTP ステータスコード 410 (Gone),223,2410,2410,ResultHttpStatus410\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus410, 223, 2410, 2411);
    //! @defresult{websocket,HTTP ステータスコード 412 (Precondition Failed),223,2412,2412,ResultHttpStatus412\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus412, 223, 2412, 2413);
    //! @defresult{websocket,HTTP ステータスコード 414 (URI Too Long),223,2414,2414,ResultHttpStatus414\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus414, 223, 2414, 2415);
    //! @defresult{websocket,HTTP ステータスコード 400 番台のハンドルしていないステータスです。,223,2499,2499,ResultHttpStatus4xx\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus4xx, 223, 2499, 2500);
    //! @defresult{websocket,HTTP ステータスコード 500 (Internal Server Error),223,2500,2500,ResultHttpStatus500\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus500, 223, 2500, 2501);
    //! @defresult{websocket,HTTP ステータスコード 502 (Bad Gateway),223,2502,2502,ResultHttpStatus502\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus502, 223, 2502, 2503);
    //! @defresult{websocket,HTTP ステータスコード 503 (Service Unavailable),223,2503,2503,ResultHttpStatus503\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus503, 223, 2503, 2504);
    //! @defresult{websocket,HTTP ステータスコード 504 (Gateway Timeout),223,2504,2504,ResultHttpStatus504\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus504, 223, 2504, 2505);
    //! @defresult{websocket,HTTP ステータスコード 509 (Bandwidth Limit Exceeded),223,2509,2509,ResultHttpStatus509\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus509, 223, 2509, 2510);
    //! @defresult{websocket,HTTP ステータスコード 500 番台のハンドルしていないステータスです。,223,2599,2599,ResultHttpStatus5xx\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus5xx, 223, 2599, 2600);
    //! @defresult{websocket,ハンドルしていないステータスです。,223,2999,2999,ResultHttpStatusXxx\, ResultCommunicationError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatusXxx, 223, 2999, 3000);
