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
//! @defresult{ssl,nn::ssl service error,123,0,4999,ResultSslService}
NN_DEFINE_ERROR_RANGE_RESULT(ResultSslService, 123, 0, 5000);
    //! @defresult{ssl,Unkown error,123,1,1,ResultUnknown\, ResultSslService}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUnknown, 123, 1, 2);
    //! @defresult{ssl,Will be handled as fatal error,123,9,99,ResultFatal\, ResultSslService}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultFatal, 123, 9, 100);
        //! @defresult{ssl,(ONLY FOR BACKWARD COMPATIBILITY) Will be handled as fatal error,123,10,98,ResultFatail\, ResultFatal\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultFatail, 123, 10, 99);
            //! @defresult{ssl,Operation failed in low level,123,11,11,ResultErrorLower\, ResultFatail\, ResultFatal\, ResultSslService}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultErrorLower, 123, 11, 12);
            //! @defresult{ssl,System encountered invalid reference,123,12,12,ResultInvalidReference\, ResultFatail\, ResultFatal\, ResultSslService}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidReference, 123, 12, 13);
            //! @defresult{ssl,Failed due to internal logic error,123,13,13,ResultInternalLogicError\, ResultFatail\, ResultFatal\, ResultSslService}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultInternalLogicError, 123, 13, 14);
    //! @defresult{ssl,Passed argument is invalid,123,100,199,ResultRetryRequired\, ResultSslService}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultRetryRequired, 123, 100, 200);
        //! @defresult{ssl,nn::ssl library is not initialized yet,123,101,101,ResultLibraryNotInitialized\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultLibraryNotInitialized, 123, 101, 102);
        //! @defresult{ssl,System failed to allocate memory,123,102,102,ResultInsufficientMemory\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInsufficientMemory, 123, 102, 103);
        //! @defresult{ssl,Socket descriptor is not registerted yet,123,103,103,ResultSocketNotRegistered\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSocketNotRegistered, 123, 103, 104);
        //! @defresult{ssl,Host name is not registered yet,123,104,104,ResultHostNameNotRegistered\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultHostNameNotRegistered, 123, 104, 105);
        //! @defresult{ssl,Host name is too long,123,105,105,ResultHostNameTooLong\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultHostNameTooLong, 123, 105, 106);
        //! @defresult{ssl,Server cert buffer is already registered,123,106,106,ResultBufferAlreadyRegistered\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultBufferAlreadyRegistered, 123, 106, 107);
        //! @defresult{ssl,Failed because there is still remaining connection,123,107,107,ResultConnectionRemaining\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionRemaining, 123, 107, 108);
        //! @defresult{ssl,Failed because there is still remaining context,123,108,108,ResultContextRemaining\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultContextRemaining, 123, 108, 109);
        //! @defresult{ssl,SSL Connection context is not properly created yet,123,109,109,ResultInvalidConnectionContext\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidConnectionContext, 123, 109, 110);
        //! @defresult{ssl,SSL context is not properly created yet,123,110,110,ResultInvalidContext\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidContext, 123, 110, 111);
        //! @defresult{ssl,I/O mode is invalid,123,111,111,ResultInvalidIoMode\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidIoMode, 123, 111, 112);
        //! @defresult{ssl,The length of provided buffer is too short,123,112,112,ResultBufferTooShort\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultBufferTooShort, 123, 112, 113);
        //! @defresult{ssl,Invalid pointer was passed,123,113,113,ResultInvalidPointer\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidPointer, 123, 113, 114);
        //! @defresult{ssl,The resource is already max,123,114,114,ResultResourceMax\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultResourceMax, 123, 114, 115);
        //! @defresult{ssl,System resource is busy,123,115,115,ResultResourceBusy\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultResourceBusy, 123, 115, 116);
        //! @defresult{ssl,Invalid socket descriptor was passed,123,116,116,ResultInvalidSocketDescriptor\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidSocketDescriptor, 123, 116, 117);
        //! @defresult{ssl,Invalid format type was passed,123,117,117,ResultInvalidCertFormat\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidCertFormat, 123, 117, 118);
        //! @defresult{ssl,Invalid value is passed for the length of password,123,118,118,ResultInvalidPasswordSize\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidPasswordSize, 123, 118, 119);
        //! @defresult{ssl,Passed socket doesn not have a TCP connection on it,123,119,119,ResultNoTcpConnection\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultNoTcpConnection, 123, 119, 120);
        //! @defresult{ssl,Invalid internal PKI type was passed,123,120,120,ResultInvalidInternalPkiType\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidInternalPkiType, 123, 120, 121);
        //! @defresult{ssl,Invalid session cache mode was passed,123,121,121,ResultInvalidSessionCacheMode\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidSessionCacheMode, 123, 121, 122);
        //! @defresult{ssl,Invalid renegotiation mode was passed,123,122,122,ResultInvalidRenegotiationMode\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidRenegotiationMode, 123, 122, 123);
        //! @defresult{ssl,Invalid concurrency limit was passed,123,123,123,ResultInvalidConcurrencyLimit\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidConcurrencyLimit, 123, 123, 124);
        //! @defresult{ssl,Policy OID string is too long,123,124,124,ResultPolicyOidStringTooLong\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultPolicyOidStringTooLong, 123, 124, 125);
        //! @defresult{ssl,The buffer length for Policy OID string is invalid,123,125,125,ResultInvalidPolicyOidStringBufferLength\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidPolicyOidStringBufferLength, 123, 125, 126);
        //! @defresult{ssl,Invalid option type was passed,123,126,126,ResultInvalidOptionType\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidOptionType, 123, 126, 127);
        //! @defresult{ssl,Invalid number of certificate IDs was provided,123,127,127,ResultInvalidIdCount\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidIdCount, 123, 127, 128);
        //! @defresult{ssl,Invalid index was passed,123,128,128,ResultInvalidIndex\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidIndex, 123, 128, 129);
        //! @defresult{ssl,Invalid CRL format was provided,123,129,129,ResultInvalidCrlFormat\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidCrlFormat, 123, 129, 130);
        //! @defresult{ssl,Unknown CRL issuer,123,130,130,ResultUnknownCrlIssuer\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultUnknownCrlIssuer, 123, 130, 131);
        //! @defresult{ssl,The CRL signature verification failed,123,131,131,ResultCrlSignatureVerifyFail\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultCrlSignatureVerifyFail, 123, 131, 132);
        //! @defresult{ssl,The provided argument is invalid for the operation,123,132,132,ResultInvalidArgument\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidArgument, 123, 132, 133);
        //! @defresult{ssl,The operation is not allowed,123,133,133,ResultOperationNotAllowed\, ResultRetryRequired\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultOperationNotAllowed, 123, 133, 134);
    //! @defresult{ssl,Reporting status,123,200,299,ResultReport\, ResultSslService}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultReport, 123, 200, 300);
        //! @defresult{ssl,nn::ssl library is already initialized,123,201,201,ResultLibraryAlreadyInitialized\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultLibraryAlreadyInitialized, 123, 201, 202);
        //! @defresult{ssl,Provided buffer was too short to store server certitiface,123,202,202,ResultInsufficientServerCertBuffer\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInsufficientServerCertBuffer, 123, 202, 203);
        //! @defresult{ssl,Socket descriptor is already registered,123,203,203,ResultSocketAlreadyRegistered\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSocketAlreadyRegistered, 123, 203, 204);
        //! @defresult{ssl,There was no I/O event to process,123,204,204,ResultIoWouldBlock\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIoWouldBlock, 123, 204, 205);
        //! @defresult{ssl,I/O API timeout,123,205,205,ResultIoTimeout\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIoTimeout, 123, 205, 206);
        //! @defresult{ssl,I/O operation was aborted,123,206,206,ResultIoAborted\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIoAborted, 123, 206, 207);
        //! @defresult{ssl,Failed to verify certificate,123,207,207,ResultVerifyCertFailed\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultVerifyCertFailed, 123, 207, 208);
        //! @defresult{ssl,No underlying connection is established to perform SSL,123,208,208,ResultNoConnection\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultNoConnection, 123, 208, 209);
        //! @defresult{ssl,Underlying connection reset by the peer,123,209,209,ResultConnectionReset\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionReset, 123, 209, 210);
        //! @defresult{ssl,Underlying connection was aborted,123,210,210,ResultConnectionAborted\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionAborted, 123, 210, 211);
        //! @defresult{ssl,Socket shutdown,123,211,211,ResultSocketShutdown\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSocketShutdown, 123, 211, 212);
        //! @defresult{ssl,Network interface is down,123,212,212,ResultNetworkDown\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultNetworkDown, 123, 212, 213);
        //! @defresult{ssl,Certificate format is invalid,123,213,213,ResultInvalidCertificate\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidCertificate, 123, 213, 214);
        //! @defresult{ssl,Invalid CertStoreId is used,123,214,214,ResultInvalidCertStoreId\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidCertStoreId, 123, 214, 215);
        //! @defresult{ssl,Client certificate and private key is already registered,123,215,215,ResultClientPkiAlreadyRegistered\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultClientPkiAlreadyRegistered, 123, 215, 216);
        //! @defresult{ssl,No SSL connection is established,123,216,216,ResultNoSslConnection\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultNoSslConnection, 123, 216, 217);
        //! @defresult{ssl,Underlying connection was closed by the peer,123,217,217,ResultConnectionClosed\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionClosed, 123, 217, 218);
        //! @defresult{ssl,The maximum number of server PKI entity is already registered,123,218,218,ResultMaxServerPkiRegistered\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxServerPkiRegistered, 123, 218, 219);
        //! @defresult{ssl,The maximum number of policy OIds are already registered,123,219,219,ResultMaxPolicyOidRegistered\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxPolicyOidRegistered, 123, 219, 220);
        //! @defresult{ssl,The SSL core is initializing,123,220,220,ResultSslCoreInitializing\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslCoreInitializing, 123, 220, 221);
        //! @defresult{ssl,No server certificate chain is present in the buffer,123,221,221,ResultNoServerChain\, ResultReport\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultNoServerChain, 123, 221, 222);
    //! @defresult{ssl,SSL errors,123,300,399,ResultSslError\, ResultSslService}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSslError, 123, 300, 400);
        //! @defresult{ssl,Host name mismatch,123,301,301,ResultSslErrorInvalidCertDomain\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorInvalidCertDomain, 123, 301, 302);
        //! @defresult{ssl,Certificate was not sent from the peer,123,302,302,ResultSslErrorNoCertificate\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorNoCertificate, 123, 302, 303);
        //! @defresult{ssl,Certificate valid date is out of date,123,303,303,ResultSslErrorExpiredCertificate\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorExpiredCertificate, 123, 303, 304);
        //! @defresult{ssl,Certificate is marked as revoked,123,304,304,ResultSslErrorRevokedCertificate\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorRevokedCertificate, 123, 304, 305);
        //! @defresult{ssl,Received certificate in unsupported format,123,305,305,ResultSslErrorUnsupportedCertificate\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUnsupportedCertificate, 123, 305, 306);
        //! @defresult{ssl,Received unknown certificate,123,306,306,ResultSslErrorUnknownCertificate\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUnknownCertificate, 123, 306, 307);
        //! @defresult{ssl,Received certificate which contains invalid data,123,307,307,ResultSslErrorBadCertificate\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorBadCertificate, 123, 307, 308);
        //! @defresult{ssl,Certificate issuer is unkown/untrusted,123,308,308,ResultSslErrorUnkownCa\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUnkownCa, 123, 308, 309);
        //! @defresult{ssl,Access denied because certificate is untrusted,123,309,309,ResultSslErrorAccessDenied\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorAccessDenied, 123, 309, 310);
        //! @defresult{ssl,Received unknown data which cannot be processed in SSL protocol level,123,310,310,ResultSslErrorReceivedUnkownData\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorReceivedUnkownData, 123, 310, 311);
        //! @defresult{ssl,Received malformed data,123,311,311,ResultSslErrorReceivedMalformedData\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorReceivedMalformedData, 123, 311, 312);
        //! @defresult{ssl,Received unexpected data for SSL handshake when it is not expected,123,312,312,ResultSslErrorReceivedUnexpectedHandshakeData\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorReceivedUnexpectedHandshakeData, 123, 312, 313);
        //! @defresult{ssl,Received unexpected SSL record when it is not expected,123,313,313,ResultSslErrorReceivedUnexpectedRecordData\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorReceivedUnexpectedRecordData, 123, 313, 314);
        //! @defresult{ssl,Received too long record to process\, returned when record_overflow alert will be sent,123,314,314,ResultSslErrorReceivedTooLongRecord\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorReceivedTooLongRecord, 123, 314, 315);
        //! @defresult{ssl,Received unexpected hello verify request,123,315,315,ResultSslErrorUnexpectedHelloVerifyRequest\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUnexpectedHelloVerifyRequest, 123, 315, 316);
        //! @defresult{ssl,Received unexpected certificate status response,123,316,316,ResultSslErrorUnexpectedCertificateStatusResponse\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUnexpectedCertificateStatusResponse, 123, 316, 317);
        //! @defresult{ssl,Offered inconsistent algorithm for the public key in the certificate,123,317,317,ResultSslErrorInconsistentSignAlgorithm\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorInconsistentSignAlgorithm, 123, 317, 318);
        //! @defresult{ssl,Offered weak key,123,318,318,ResultSslErrorInsufficientSecurity\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorInsufficientSecurity, 123, 318, 319);
        //! @defresult{ssl,Decryption failed due to invalid MAC\, bad_record_mac alert will be sent,123,319,319,ResultSslErrorDecryptionWithInvalidMac\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorDecryptionWithInvalidMac, 123, 319, 320);
        //! @defresult{ssl,Failed to decompress record,123,330,330,ResultSslErrorDecompressionFailed\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorDecompressionFailed, 123, 330, 331);
        //! @defresult{ssl,Failed to calculate digest,123,321,321,ResultSslErrorDigestCalculationFailed\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorDigestCalculationFailed, 123, 321, 322);
        //! @defresult{ssl,Failed to calculate MAC,123,322,322,ResultSslErrorMacCalculationFailed\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorMacCalculationFailed, 123, 322, 323);
        //! @defresult{ssl,Server requested client certificate though no client certificate was found,123,323,323,ResultSslErrorClientCertificateNotFound\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorClientCertificateNotFound, 123, 323, 324);
        //! @defresult{ssl,No certificate policies extension field was found,123,324,324,ResultSslErrorNoExtendedField\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorNoExtendedField, 123, 324, 325);
        //! @defresult{ssl,An object identifier in the certificate extension field is untrusted,123,325,325,ResultSslErrorUntrustedOid\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUntrustedOid, 123, 325, 326);
        //! @defresult{ssl,Invalid certificate policy was found in the certificate chain,123,326,326,ResultSslErrorInvalidPolicy\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorInvalidPolicy, 123, 326, 327);
        //! @defresult{ssl,Server does not support cipher suites offered by the client,123,327,327,ResultSslErrorNoCipher\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorNoCipher, 123, 327, 328);
        //! @defresult{ssl,Root CA fingerprint does not match any known EV qualified CA certificates.,123,328,328,ResultSslErrorUnknownEvFingerprint\, ResultSslError\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslErrorUnknownEvFingerprint, 123, 328, 329);
    //! @defresult{ssl,SSL Alert description types,123,1500,1999,ResultSslAlert\, ResultSslService}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlert, 123, 1500, 2000);
        //! @defresult{ssl,Code 0\, close notify,123,1501,1501,ResultSslAlertCloseNotify\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertCloseNotify, 123, 1501, 1502);
        //! @defresult{ssl,Code 10\, unexpected message,123,1502,1502,ResultSslAlertUnexpectedMessage\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUnexpectedMessage, 123, 1502, 1503);
        //! @defresult{ssl,Code 20\, bad record mac,123,1503,1503,ResultSslAlertBadRecordMac\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertBadRecordMac, 123, 1503, 1504);
        //! @defresult{ssl,Code 21\, decryption failed,123,1504,1504,ResultSslAlertDecriptionFailed\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertDecriptionFailed, 123, 1504, 1505);
        //! @defresult{ssl,Code 22\, record overflow,123,1505,1505,ResultSslAlertRecordOverfrlow\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertRecordOverfrlow, 123, 1505, 1506);
        //! @defresult{ssl,Code 30\, decompression failure,123,1506,1506,ResultSslAlertDecompressionFailure\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertDecompressionFailure, 123, 1506, 1507);
        //! @defresult{ssl,Code 40\, handshake failure,123,1507,1507,ResultSslAlertHandshakeFailure\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertHandshakeFailure, 123, 1507, 1508);
        //! @defresult{ssl,Code 41\, no certificate,123,1508,1508,ResultSslAlertNoCertificate\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertNoCertificate, 123, 1508, 1509);
        //! @defresult{ssl,Code 42\, bad certificate,123,1509,1509,ResultSslAlertBadCertificate\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertBadCertificate, 123, 1509, 1510);
        //! @defresult{ssl,Code 43\, unsupported certificate,123,1510,1510,ResultSslAlertUnsupportedCertificate\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUnsupportedCertificate, 123, 1510, 1511);
        //! @defresult{ssl,Code 44\, certificate revoked,123,1511,1511,ResultSslAlertCertificateRevoked\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertCertificateRevoked, 123, 1511, 1512);
        //! @defresult{ssl,Code 45\, certificate expired,123,1512,1512,ResultSslAlertCertificateExpired\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertCertificateExpired, 123, 1512, 1513);
        //! @defresult{ssl,Code 46\, certificate unkown,123,1513,1513,ResultSslAlertCertificateUnkown\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertCertificateUnkown, 123, 1513, 1514);
        //! @defresult{ssl,Code 47\, illegal parameter,123,1514,1514,ResultSslAlertIllegalParameter\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertIllegalParameter, 123, 1514, 1515);
        //! @defresult{ssl,Code 48\, unknown CA,123,1515,1515,ResultSslAlertUnknownCa\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUnknownCa, 123, 1515, 1516);
        //! @defresult{ssl,Code 49\, access denied,123,1516,1516,ResultSslAlertAccessDenied\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertAccessDenied, 123, 1516, 1517);
        //! @defresult{ssl,Code 50\, decode error,123,1517,1517,ResultSslAlertDecodeError\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertDecodeError, 123, 1517, 1518);
        //! @defresult{ssl,Code 51\, decrypt error,123,1518,1518,ResultSslAlertDecryptError\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertDecryptError, 123, 1518, 1519);
        //! @defresult{ssl,Code 60\, export restriction,123,1519,1519,ResultSslAlertExportRestriction\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertExportRestriction, 123, 1519, 1520);
        //! @defresult{ssl,Code 70\, protocol version,123,1520,1520,ResultSslAlertProtocolVersion\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertProtocolVersion, 123, 1520, 1521);
        //! @defresult{ssl,Code 71\, insufficient securith,123,1521,1521,ResultSslAlertInsufficientSecurity\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertInsufficientSecurity, 123, 1521, 1522);
        //! @defresult{ssl,Code 80\, internal error,123,1522,1522,ResultSslAlertInternalError\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertInternalError, 123, 1522, 1523);
        //! @defresult{ssl,Code 86\, for SSL POODLE vulnerability,123,1523,1523,ResultSslAlertInappropriateFallback\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertInappropriateFallback, 123, 1523, 1524);
        //! @defresult{ssl,Code 90\, user canceled,123,1524,1524,ResultSslAlertUserCancelled\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUserCancelled, 123, 1524, 1525);
        //! @defresult{ssl,Code 100\, no renegotiation,123,1525,1525,ResultSslAlertNoRenegotiation\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertNoRenegotiation, 123, 1525, 1526);
        //! @defresult{ssl,Code 110\, unsupported extension,123,1526,1526,ResultSslAlertUnsupportedExtention\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUnsupportedExtention, 123, 1526, 1527);
        //! @defresult{ssl,Code 111\, certificate unobtainable,123,1527,1527,ResultSslAlertCertificateUnobtainable\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertCertificateUnobtainable, 123, 1527, 1528);
        //! @defresult{ssl,Code 112\, unrecognized name,123,1528,1528,ResultSslAlertUnrecognizedName\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUnrecognizedName, 123, 1528, 1529);
        //! @defresult{ssl,Code 113\, bad certificate status response,123,1529,1529,ResultSslAlertBadCertificateStatusResponse\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertBadCertificateStatusResponse, 123, 1529, 1530);
        //! @defresult{ssl,Code 114\, bad certificate hash value,123,1530,1530,ResultSslAlertBadCertificateHashValue\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertBadCertificateHashValue, 123, 1530, 1531);
        //! @defresult{ssl,Code 115\, unknown PSK identity,123,1531,1531,ResultSslAlertUnkownPskIdentity\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertUnkownPskIdentity, 123, 1531, 1532);
        //! @defresult{ssl,Code 120\, no application protocol,123,1532,1532,ResultSslAlertNoApplicationProtocol\, ResultSslAlert\, ResultSslService}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSslAlertNoApplicationProtocol, 123, 1532, 1533);
