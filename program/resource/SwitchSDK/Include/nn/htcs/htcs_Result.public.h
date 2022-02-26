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
//! @defresult{htcs,操作にはブロックが必要,4,1,1,ResultWouldBlock}
NN_DEFINE_ERROR_RANGE_RESULT(ResultWouldBlock, 4, 1, 2);
//! @defresult{htcs,対象が見つからない,4,2,2,ResultNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotFound, 4, 2, 3);
//! @defresult{htcs,サービス名がすでに登録されている,4,3,3,ResultDuplicatedServiceName}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDuplicatedServiceName, 4, 3, 4);
//! @defresult{htcs,操作が拒否された,4,4,4,ResultOperationDenied}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOperationDenied, 4, 4, 5);
//! @defresult{htcs,接続が切断されている,4,5,5,ResultDisconnected}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDisconnected, 4, 5, 6);
//! @defresult{htcs,処理が中断された,4,6,6,ResultCanceled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultCanceled, 4, 6, 7);
//! @defresult{htcs,処理がタイムアウトした,4,7,7,ResultTimedOut}
NN_DEFINE_ERROR_RANGE_RESULT(ResultTimedOut, 4, 7, 8);
//! @defresult{htcs,非同期処理のキューがいっぱい,4,8,8,ResultQueueFull}
NN_DEFINE_ERROR_RANGE_RESULT(ResultQueueFull, 4, 8, 9);
//! @defresult{htcs,ハンドルが不正,4,9,9,ResultInvalidHandle}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidHandle, 4, 9, 10);
//! @defresult{htcs,未実装の操作,4,1021,1021,ResultNotImplemented}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 4, 1021, 1022);
//! @defresult{htcs,内部エラー,4,2000,2999,ResultInternalError}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInternalError, 4, 2000, 3000);
