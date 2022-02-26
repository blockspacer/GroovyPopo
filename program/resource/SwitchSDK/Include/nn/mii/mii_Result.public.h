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
//! @defresult{mii,引数に間違いがあります,126,1,1,ResultInvalid}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalid, 126, 1, 2);
//! @defresult{mii,コンテナのサイズよりもデータが多いです,126,2,2,ResultOverflow}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOverflow, 126, 2, 3);
//! @defresult{mii,更新はありませんでした,126,3,3,ResultNotUpdated}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotUpdated, 126, 3, 4);
//! @defresult{mii,見つかりませんでした,126,4,4,ResultNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotFound, 126, 4, 5);
//! @defresult{mii,満杯です,126,5,5,ResultFull}
NN_DEFINE_ERROR_RANGE_RESULT(ResultFull, 126, 5, 6);
//! @defresult{mii,すでに存在します,126,6,6,ResultExist}
NN_DEFINE_ERROR_RANGE_RESULT(ResultExist, 126, 6, 7);
//! @defresult{mii,処理がキャンセルされました,126,7,7,ResultCanceled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultCanceled, 126, 7, 8);
//! @defresult{mii,データが破損しています,126,100,199,ResultBroken}
NN_DEFINE_ERROR_RANGE_RESULT(ResultBroken, 126, 100, 200);
//! @defresult{mii,開発用の機能です。事前に機能を有効化してください,126,204,204,ResultDevelopmentOnly}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDevelopmentOnly, 126, 204, 205);
