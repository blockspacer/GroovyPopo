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
//! @defresult{nfp,正しいステートの NFC デバイスが見つかりません。ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。,115,64,79,ResultNfcDeviceNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNfcDeviceNotFound, 115, 64, 80);
//! @defresult{nfp,NFC 機能が無効になっています。NFC 機能が有効になった後、StartDetection() によるタグの検知からやり直す必要があります。,115,80,80,ResultNfcDisabled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNfcDisabled, 115, 80, 81);
//! @defresult{nfp,タグへのアクセスに失敗しました。再試行によって成功する可能性があります。再試行にて 3 回失敗した場合は、@ref StartDetection() によるタグの検知からやり直す必要があります。,115,88,95,ResultNeedRetry}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedRetry, 115, 88, 96);
//! @defresult{nfp,タグへのアクセスに失敗しました。アクセス対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。,115,96,103,ResultNeedRestart}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedRestart, 115, 96, 104);
//! @defresult{nfp,登録情報が設定されていません。amiibo 設定での設定が必要です。,115,120,120,ResultNeedRegister}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedRegister, 115, 120, 121);
//! @defresult{nfp,アプリケーション専用領域が作成されていません。@ref CreateApplicationArea() によるアプリケーション専用領域の作成を行う必要があります。,115,128,128,ResultNeedCreate}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedCreate, 115, 128, 129);
//! @defresult{nfp,タグのデータが壊れています。@ref Restore() によるタグの復旧を行う必要があります。,115,136,136,ResultNeedRestore}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedRestore, 115, 136, 137);
//! @defresult{nfp,タグのデータが壊れていますが、バックアップデータが存在しません。タグのフォーマットを行う必要があります。,115,144,144,ResultNeedFormat}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedFormat, 115, 144, 145);
//! @defresult{nfp,アクセス ID が一致しないため、アプリケーション専用領域へのアクセスができません。,115,152,152,ResultAccessIdMisMatch}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAccessIdMisMatch, 115, 152, 153);
//! @defresult{nfp,タグが壊れていないため、復旧は不要です。,115,160,160,ResultNotBroken}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotBroken, 115, 160, 161);
//! @defresult{nfp,既にアプリケーション専用領域が作成されています。,115,168,168,ResultAlreadyCreated}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAlreadyCreated, 115, 168, 169);
//! @defresult{nfp,NFP タグではありません。使用しているタグを確認する必要があります。,115,176,183,ResultNotSupported}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotSupported, 115, 176, 184);
//! @defresult{nfp,対応していないバージョンのタグです。使用しているタグを確認する必要があります。,115,184,184,ResultInvalidFormatVersion}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidFormatVersion, 115, 184, 185);
//! @defresult{nfp,NFC 機能を利用中の NFC デバイスが最大数に達しているため、指定された NFC デバイスで NFC 機能を利用できません。NFC 機能を利用中の NFC デバイスが減った後、再試行する必要があります。,115,192,192,ResultMaxNfcDeviceActivated}
NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxNfcDeviceActivated, 115, 192, 193);
//! @defresult{nfp,競合する他の機能が利用されているため、NFC 機能を利用できません。競合する他の機能の利用が終わった後、再試行する必要があります。,115,200,200,ResultConflictFunction}
NN_DEFINE_ERROR_RANGE_RESULT(ResultConflictFunction, 115, 200, 201);
//! @defresult{nfp,タグは更新されていません。,115,208,208,ResultNotUpdated}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotUpdated, 115, 208, 209);
