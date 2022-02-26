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
    //! @defresulta{hid,振動ファイルが不正です,202,140,149,ResultVibrationFileInvalid\, ResultVibrationFileError}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultVibrationFileInvalid, 202, 140, 150);
//! @defresulta{hid,Npad に関するエラーです,202,600,799,ResultNpadError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultNpadError, 202, 600, 800);
    //! @defresult{hid,Npad に対してコントローラーが接続されていません,202,604,604,ResultNpadControllerNotConnected\, ResultNpadError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNpadControllerNotConnected, 202, 604, 605);
