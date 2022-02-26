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

// package 'controllerSupport'
//! @defresulta{hid,ControllerSupport に関するエラーです,202,3100,3109,ResultControllerSupportError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultControllerSupportError, 202, 3100, 3110);
    //! @defresult{hid,コントローラーサポートがキャンセルされました,202,3101,3101,ResultControllerSupportCanceled\, ResultControllerSupportError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultControllerSupportCanceled, 202, 3101, 3102);
    //! @defresult{hid,コントローラーサポートが対応していない操作形態が設定されています,202,3102,3102,ResultControllerSupportNotSupportedNpadStyle\, ResultControllerSupportError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultControllerSupportNotSupportedNpadStyle, 202, 3102, 3103);
//! @defresulta{hid,ファームウェア更新に関するエラーです,202,3200,3209,ResultControllerFirmwareUpdateError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultControllerFirmwareUpdateError, 202, 3200, 3210);
    //! @defresult{hid,ファームウェア更新に失敗しました,202,3201,3201,ResultControllerFirmwareUpdateFailed\, ResultControllerFirmwareUpdateError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultControllerFirmwareUpdateFailed, 202, 3201, 3202);
