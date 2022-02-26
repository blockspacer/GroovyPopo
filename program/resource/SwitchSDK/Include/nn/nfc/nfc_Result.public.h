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
//! @defresult{nfc,正しいステートの NFC デバイスが見つかりません。ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。,127,64,79,ResultNfcDeviceNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNfcDeviceNotFound, 127, 64, 80);
//! @defresult{nfc,NFC 機能が無効になっています。NFC 機能が有効になった後、StartDetection() によるタグの検知からやり直す必要があります。,127,80,80,ResultNfcDisabled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNfcDisabled, 127, 80, 81);
//! @defresult{nfc,タグへのアクセスに失敗しました。アクセス対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。,127,96,103,ResultNeedRestart}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNeedRestart, 127, 96, 104);
//! @defresult{nfc,想定のタグではありません。使用しているタグを確認する必要があります。,127,176,183,ResultNotSupported}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotSupported, 127, 176, 184);
//! @defresult{nfc,NFC 機能を利用中の NFC デバイスが最大数に達しているため、指定された NFC デバイスで NFC 機能を利用できません。NFC 機能を利用中の NFC デバイスが減った後、再試行する必要があります。,127,192,192,ResultMaxNfcDeviceActivated}
NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxNfcDeviceActivated, 127, 192, 193);
//! @defresult{nfc,競合する他の機能が利用されているため、NFC 機能を利用できません。競合する他の機能の利用が終わった後、再試行する必要があります。,127,200,200,ResultConflictFunction}
NN_DEFINE_ERROR_RANGE_RESULT(ResultConflictFunction, 127, 200, 201);
//! @defresult{nfc,タグへのアクセスに失敗しました。タグへのアクセス方法が正しいか確認し、@ref StartDetection() によるタグの検知からやり直す必要があります。,127,288,295,ResultAccessError}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAccessError, 127, 288, 296);
