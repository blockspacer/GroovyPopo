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
//! @defresulta{album,不明なエラー,214,1,2047,ResultUnknown}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultUnknown, 214, 1, 2048);
    //! @defresult{album,サポートされていません,214,10,10,ResultNotSupported\, ResultUnknown}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNotSupported, 214, 10, 11);
    //! @defresult{album,アルバムに不明なエラーが発生しました,214,100,199,ResultAlbumUnknown\, ResultUnknown}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumUnknown, 214, 100, 200);
        //! @defresult{album,アルバムにエラーが発生しました,214,101,101,ResultAlbumError\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumError, 214, 101, 102);
        //! @defresult{album,アルバムの容量が足りません,214,102,102,ResultAlbumFull\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumFull, 214, 102, 103);
        //! @defresult{album,アルバムに保存できるファイル数の上限に達しています,214,103,103,ResultAlbumFileCountLimit\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumFileCountLimit, 214, 103, 104);
        //! @defresult{album,アルバムに保存できるファイルサイズの上限に達しました,214,105,105,ResultAlbumFileSizeLimit\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumFileSizeLimit, 214, 105, 106);
        //! @defresult{album,対象ファイルが見つかりません,214,107,107,ResultAlbumFileNotFound\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumFileNotFound, 214, 107, 108);
        //! @defresult{album,SD カードが利用できません,214,110,110,ResultSdcardNotAvailable\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSdcardNotAvailable, 214, 110, 111);
        //! @defresult{album,対象ファイルの読み込みに失敗しました,214,111,111,ResultAlbumFileDataError\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumFileDataError, 214, 111, 112);
        //! @defresult{album,未知のファイルバージョンです,214,112,112,ResultAlbumFileUnknownVersion\, ResultAlbumUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultAlbumFileUnknownVersion, 214, 112, 113);
    //! @defresult{album,投稿に不明なエラーが発生しました,214,500,599,ResultShareUnknown\, ResultUnknown}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultShareUnknown, 214, 500, 600);
        //! @defresult{album,投稿がキャンセルされました,214,501,501,ResultShareCanceled\, ResultShareUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultShareCanceled, 214, 501, 502);
        //! @defresult{album,投稿に失敗しました,214,502,502,ResultShareFailed\, ResultShareUnknown\, ResultUnknown}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultShareFailed, 214, 502, 503);
