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
    //! @defresulta{irsensor,モーションIRカメラが利用できません,205,110,119,ResultIrsensorUnavailable\, ResultStatusError}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultIrsensorUnavailable, 205, 110, 120);
        //! @defresult{irsensor,デバイスが接続されていません,205,110,110,ResultIrsensorUnconnected\, ResultIrsensorUnavailable\, ResultStatusError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIrsensorUnconnected, 205, 110, 111);
        //! @defresult{irsensor,デバイスにモーションIRカメラが搭載されていません,205,111,111,ResultIrsensorUnsupported\, ResultIrsensorUnavailable\, ResultStatusError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIrsensorUnsupported, 205, 111, 112);
    //! @defresulta{irsensor,モーションIRカメラのデバイスが不正な状態のため利用できません,205,122,139,ResultIrsensorDeviceError\, ResultStatusError}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultIrsensorDeviceError, 205, 122, 140);
    //! @defresult{irsensor,コントローラの FW バージョンを確認中です,205,150,150,ResultIrsensorFirmwareCheckIncompleted\, ResultStatusError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultIrsensorFirmwareCheckIncompleted, 205, 150, 151);
    //! @defresulta{irsensor,モーションIRカメラが起動中のため処理結果がまだありません,205,160,169,ResultIrsensorNotReady\, ResultStatusError}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultIrsensorNotReady, 205, 160, 170);
        //! @defresult{irsensor,モーションIRカメラのデバイスが起動中のため処理結果がまだありません,205,160,160,ResultIrsensorDeviceNotReady\, ResultIrsensorNotReady\, ResultStatusError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIrsensorDeviceNotReady, 205, 160, 161);
        //! @defresult{irsensor,無線接続台数制限、もしくはデバイスの排他制御が適切に行われていないため利用できません,205,161,161,ResultIrsensorDeviceResourceNotAvailable\, ResultIrsensorNotReady\, ResultStatusError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultIrsensorDeviceResourceNotAvailable, 205, 161, 162);
//! @defresulta{irsensor,Hand Analysis Error,205,1100,1199,ResultHandAnalysisError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultHandAnalysisError, 205, 1100, 1200);
    //! @defresult{irsensor,The current hand analysis mode does not match the one required by the function,205,1101,1101,ResultHandAnalysisModeIncorrect\, ResultHandAnalysisError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHandAnalysisModeIncorrect, 205, 1101, 1102);
