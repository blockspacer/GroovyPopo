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
//! @defresult{time,ライブラリが初期化されていません,116,0,0,ResultNotInitialized}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotInitialized, 116, 0, 1);
//! @defresult{time,適切な権限がありません,116,1,1,ResultNoCapability}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNoCapability, 116, 1, 2);
//! @defresulta{time,時計が無効になっています,116,100,199,ResultClockInvalid}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultClockInvalid, 116, 100, 200);
//! @defresult{time,与えられた2つの値に連続性がなく、比較・減算できません,116,200,200,ResultNotComparable}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotComparable, 116, 200, 201);
//! @defresult{time,オーバーフローまたはアンダーフローが発生しました,116,201,201,ResultOverflowed}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOverflowed, 116, 201, 202);
//! @defresulta{time,内部リソースが不足しています,116,800,819,ResultOutOfResource}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultOutOfResource, 116, 800, 820);
    //! @defresult{time,内部メモリが不足しています,116,801,801,ResultOutOfMemory\, ResultOutOfResource}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfMemory, 116, 801, 802);
