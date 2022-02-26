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
//! @defresult{ro,ro モジュールが生成するエラー,22,1,1023,ResultRoError}
NN_DEFINE_ERROR_RANGE_RESULT(ResultRoError, 22, 1, 1024);
    //! @defresult{ro,nro ファイルを読み込むために必要な空きメモリ領域がない,22,2,2,ResultOutOfAddressSpace\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfAddressSpace, 22, 2, 3);
    //! @defresult{ro,対象の nro ファイルは既に nn::ro::LoadModule() で読み込まれている,22,3,3,ResultNroAlreadyLoaded\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNroAlreadyLoaded, 22, 3, 4);
    //! @defresult{ro,指定されたメモリ領域に nro ファイルではないデータが読み込まれている,22,4,4,ResultInvalidNroImage\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidNroImage, 22, 4, 5);
    //! @defresult{ro,シンボルが見つからなかった,22,5,5,ResultNotFound\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNotFound, 22, 5, 6);
    //! @defresult{ro,指定されたメモリ領域に nrr ファイルではないデータが読み込まれている,22,6,6,ResultInvalidNrrImage\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidNrrImage, 22, 6, 7);
    //! @defresult{ro,読み込むことが出来るモジュール数の上限に達している,22,7,7,ResultMaxModule\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxModule, 22, 7, 8);
    //! @defresult{ro,登録できるnrr ファイル数の上限に達している,22,8,8,ResultMaxRegistration\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxRegistration, 22, 8, 9);
    //! @defresult{ro,読み込み時に認証に失敗した,22,9,9,ResultNotAuthorized\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNotAuthorized, 22, 9, 10);
    //! @defresult{ro,未実装,22,1022,1022,ResultNotImplemented\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 22, 1022, 1023);
    //! @defresult{ro,内部エラー,22,1023,1023,ResultInternalError\, ResultRoError}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInternalError, 22, 1023, 1024);
