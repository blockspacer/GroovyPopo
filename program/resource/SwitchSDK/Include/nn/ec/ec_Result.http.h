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

// package 'http'
//! @defresulta{ec,Http Status Code,164,7000,7599,ResultHttpStatus}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultHttpStatus, 164, 7000, 7600);
    //! @defresult{ec,Unexpected HTTP Status Code,164,7000,7000,ResultHttpStatusUnexpected\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatusUnexpected, 164, 7000, 7001);
    //! @defresult{ec,HTTP 400 Bad Request,164,7400,7400,ResultHttpStatus400BadRequest\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus400BadRequest, 164, 7400, 7401);
    //! @defresult{ec,HTTP 401 Unauthorized,164,7401,7401,ResultHttpStatus401Unauthorized\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus401Unauthorized, 164, 7401, 7402);
    //! @defresult{ec,HTTP 403 Forbidden,164,7403,7403,ResultHttpStatus403Forbidden\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus403Forbidden, 164, 7403, 7404);
    //! @defresult{ec,HTTP 404 Not Found,164,7404,7404,ResultHttpStatus404NotFound\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus404NotFound, 164, 7404, 7405);
    //! @defresult{ec,HTTP 405 Method Not Allowed,164,7405,7405,ResultHttpStatus405MethodNotAllowed\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus405MethodNotAllowed, 164, 7405, 7406);
    //! @defresult{ec,HTTP 406 Not Acceptable,164,7406,7406,ResultHttpStatus406NotAcceptable\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus406NotAcceptable, 164, 7406, 7407);
    //! @defresult{ec,HTTP 407 Proxy Authentication Required,164,7407,7407,ResultHttpStatus407ProxyAuthenticationRequired\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus407ProxyAuthenticationRequired, 164, 7407, 7408);
    //! @defresult{ec,HTTP 408 Request Timeout,164,7408,7408,ResultHttpStatus408RequestTimeout\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus408RequestTimeout, 164, 7408, 7409);
    //! @defresult{ec,HTTP 409 Conflict,164,7409,7409,ResultHttpStatus409Conflict\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus409Conflict, 164, 7409, 7410);
    //! @defresult{ec,HTTP 410 Gone,164,7410,7410,ResultHttpStatus410Gone\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus410Gone, 164, 7410, 7411);
    //! @defresult{ec,HTTP 411 Length Required,164,7411,7411,ResultHttpStatus411LengthRequired\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus411LengthRequired, 164, 7411, 7412);
    //! @defresult{ec,HTTP 412 Precondition Failed,164,7412,7412,ResultHttpStatus412PreconditionFailed\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus412PreconditionFailed, 164, 7412, 7413);
    //! @defresult{ec,HTTP 413 Payload Too Large,164,7413,7413,ResultHttpStatus413PayloadTooLarge\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus413PayloadTooLarge, 164, 7413, 7414);
    //! @defresult{ec,HTTP 414 URI Too Long,164,7414,7414,ResultHttpStatus414UriTooLong\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus414UriTooLong, 164, 7414, 7415);
    //! @defresult{ec,HTTP 415 Unsupported Media Type,164,7415,7415,ResultHttpStatus415UnsupportedMediaType\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus415UnsupportedMediaType, 164, 7415, 7416);
    //! @defresult{ec,HTTP 416 Range Not Satisfiable,164,7416,7416,ResultHttpStatus416RangeNotSatisfiable\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus416RangeNotSatisfiable, 164, 7416, 7417);
    //! @defresult{ec,HTTP 417 Expectation Failed,164,7417,7417,ResultHttpStatus417ExpectationFailed\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus417ExpectationFailed, 164, 7417, 7418);
    //! @defresult{ec,HTTP 500 Internal Server Error,164,7500,7500,ResultHttpStatus500InternalServerError\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus500InternalServerError, 164, 7500, 7501);
    //! @defresult{ec,HTTP 501 Not Implemented,164,7501,7501,ResultHttpStatus501NotImplemented\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus501NotImplemented, 164, 7501, 7502);
    //! @defresult{ec,HTTP 502 Bad Gateway,164,7502,7502,ResultHttpStatus502BadGateway\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus502BadGateway, 164, 7502, 7503);
    //! @defresult{ec,HTTP 503 Service Unavailable,164,7503,7503,ResultHttpStatus503ServiceUnavailable\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus503ServiceUnavailable, 164, 7503, 7504);
    //! @defresult{ec,HTTP 504 Gateway Timeout,164,7504,7504,ResultHttpStatus504GatewayTimeout\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus504GatewayTimeout, 164, 7504, 7505);
    //! @defresult{ec,HTTP 505 HTTP Version Not Supported,164,7505,7505,ResultHttpStatus505HttpVersionNotSupported\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus505HttpVersionNotSupported, 164, 7505, 7506);
    //! @defresult{ec,HTTP 509 Bandwidth Limit Exceeded,164,7509,7509,ResultHttpStatus509BandwidthLimitExceeded\, ResultHttpStatus}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHttpStatus509BandwidthLimitExceeded, 164, 7509, 7510);
