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
//! @defresult{atk,不明なエラー,209,1,1,ResultUnknown}
NN_DEFINE_ERROR_RANGE_RESULT(ResultUnknown, 209, 1, 2);
//! @defresulta{atk,サウンドアーカイブプレイヤーに関するエラーです,209,100,199,ResultSoundArchivePlayerError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultSoundArchivePlayerError, 209, 100, 200);
    //! @defresult{atk,指定した ID がアーカイブに存在していない、もしくは、異なるタイプの ID が渡されました,209,101,101,ResultSoundArchivePlayerInvalidId\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerInvalidId, 209, 101, 102);
    //! @defresult{atk,無効な名前を指定したか、文字列テーブルが読み込まれていません,209,102,102,ResultSoundArchivePlayerInvalidName\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerInvalidName, 209, 102, 103);
    //! @defresult{atk,対応していないフォーマットのバイナリが渡されました,209,103,103,ResultSoundArchivePlayerInvalidFormat\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerInvalidFormat, 209, 103, 104);
    //! @defresult{atk,指定したサウンドがロードされていません,209,104,104,ResultSoundArchivePlayerSoundNotLoaded\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerSoundNotLoaded, 209, 104, 105);
    //! @defresult{atk,対象のストリームサウンドは再生中です,209,105,105,ResultSoundArchivePlayerStreamSoundIsPlaying\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerStreamSoundIsPlaying, 209, 105, 106);
    //! @defresult{atk,対象のストリームサウンドのファイルパスの読み込みに失敗しました,209,106,106,ResultSoundArchivePlayerReadStreamSoundFilePathFailed\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerReadStreamSoundFilePathFailed, 209, 106, 107);
    //! @defresult{atk,ヘッダのロードに失敗しました,209,107,107,ResultSoundArchivePlayerLoadFileHeaderFailed\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerLoadFileHeaderFailed, 209, 107, 108);
    //! @defresult{atk,ファイルシステム処理に失敗しました,209,108,108,ResultSoundArchivePlayerFileSystemError\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerFileSystemError, 209, 108, 109);
    //! @defresult{atk,対象のストリームサウンドにリージョン情報がありません,209,109,109,ResultSoundArchivePlayerStreamSoundHasNoRegion\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerStreamSoundHasNoRegion, 209, 109, 110);
    //! @defresult{atk,対象のストリームサウンドから指定されたリージョンが見つかりませんでした,209,110,110,ResultSoundArchivePlayerStreamSoundRegionNotFound\, ResultSoundArchivePlayerError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSoundArchivePlayerStreamSoundRegionNotFound, 209, 110, 111);
