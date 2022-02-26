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
//! @defresult{htc,通信に失敗しました,18,1,1,ResultConnectionFailure}
NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionFailure, 18, 1, 2);
//! @defresult{htc,対象が見つかりません,18,2,2,ResultNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotFound, 18, 2, 3);
//! @defresult{htc,バッファサイズが足りません,18,3,3,ResultNotEnoughBuffer}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotEnoughBuffer, 18, 3, 4);
//! @defresult{htc,未実装の操作,18,1022,1022,ResultNotImplemented}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 18, 1022, 1023);
//! @defresult{htc,不明な失敗,18,1023,1023,ResultUnknown}
NN_DEFINE_ERROR_RANGE_RESULT(ResultUnknown, 18, 1023, 1024);
