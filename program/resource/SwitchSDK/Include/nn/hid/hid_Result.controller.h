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

// package 'controller'
    //! @defresult{hid,Joy-Con が2本接続された Npad が指定されたためマージできません,202,601,601,ResultNpadDualConnected\, ResultNpadError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNpadDualConnected, 202, 601, 602);
    //! @defresult{hid,同じ種類の Joy-Con が接続された2つのNpadが指定されたためマージできません,202,602,602,ResultNpadSameJoyTypeConnected\, ResultNpadError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNpadSameJoyTypeConnected, 202, 602, 603);
    //! @defresult{hid,Npad の色情報を取得できませんでした,202,603,603,ResultNpadColorNotAvailable\, ResultNpadError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNpadColorNotAvailable, 202, 603, 604);
