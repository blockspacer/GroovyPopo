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
    //! @defresult{account,呼び出し元の要求に従って処理を中断した,124,0,0,ResultCancelled\, ResultInterrupted}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCancelled, 124, 0, 1);
    //! @defresult{account,ユーザー操作に従って処理を中断した,124,1,1,ResultCancelledByUser\, ResultInterrupted}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultCancelledByUser, 124, 1, 2);
    //! @defresult{account,ユーザーアカウントが存在しない,124,100,100,ResultUserNotExist\, ResultUnexpectedUserState}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUserNotExist, 124, 100, 101);
//! @defresulta{account,ネットワークサービスアカウントを利用可能な状態ではない,124,200,269,ResultNetworkServiceAccountUnavailable}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultNetworkServiceAccountUnavailable, 124, 200, 270);
    //! @defresult{account,ニンテンドーアカウントの認可取得にはユーザーとの対話が必要,124,280,280,ResultNintendoAccountAuthorizationInteractionRequired\, ResultNintendoAccountAuthorizationFailure}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNintendoAccountAuthorizationInteractionRequired, 124, 280, 281);
    //! @defresulta{account,トークンキャッシュを利用できない,124,430,499,ResultTokenCacheUnavailable\, ResultDataCacheUnavailable}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultTokenCacheUnavailable, 124, 430, 500);
//! @defresulta{account,ネットワーク通信に関するエラー,124,3000,8191,ResultNetworkCommunicationError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultNetworkCommunicationError, 124, 3000, 8192);
