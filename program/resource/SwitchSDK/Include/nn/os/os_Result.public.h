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
//! @defresult{os,許可されていない操作,3,1,1,ResultNoPermission}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNoPermission, 3, 1, 2);
//! @defresult{os,不正アクセスを検出,3,2,2,ResultAccessViolation}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAccessViolation, 3, 2, 3);
//! @defresult{os,不正なリクエストコード,3,3,3,ResultBadRequest}
NN_DEFINE_ERROR_RANGE_RESULT(ResultBadRequest, 3, 3, 4);
//! @defresult{os,リソースが使用中,3,4,4,ResultBusy}
NN_DEFINE_ERROR_RANGE_RESULT(ResultBusy, 3, 4, 5);
//! @defresult{os,デッドロック,3,5,5,ResultDeadlock}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDeadlock, 3, 5, 6);
//! @defresult{os,アドレスが不正,3,6,6,ResultInvalidAddress}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidAddress, 3, 6, 7);
//! @defresult{os,引数（パラメータ）が無効,3,7,7,ResultInvalidParameter}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidParameter, 3, 7, 8);
//! @defresult{os,十分な空きメモリ領域がない,3,8,8,ResultOutOfMemory}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfMemory, 3, 8, 9);
//! @defresult{os,十分な空きリソースがない,3,9,9,ResultOutOfResource}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfResource, 3, 9, 10);
//! @defresult{os,最大値を超えた,3,10,10,ResultOverflow}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOverflow, 3, 10, 11);
//! @defresult{os,操作がタイムアウトした,3,11,11,ResultTimedout}
NN_DEFINE_ERROR_RANGE_RESULT(ResultTimedout, 3, 11, 12);
//! @defresult{os,十分な仮想アドレス空間がない,3,12,12,ResultOutOfVirtualAddressSpace}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfVirtualAddressSpace, 3, 12, 13);
//! @defresult{os,使用リソースが上限に達した,3,13,13,ResultResourceLimit}
NN_DEFINE_ERROR_RANGE_RESULT(ResultResourceLimit, 3, 13, 14);
//! @defresult{os,関数が未実装,3,1000,1000,ResultNotImplemented}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 3, 1000, 1001);
//! @defresult{os,未実装,3,1001,1001,ResultNotSupported}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotSupported, 3, 1001, 1002);
//! @defresult{os,内部エラー,3,1023,1023,ResultInternalError}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInternalError, 3, 1023, 1024);
