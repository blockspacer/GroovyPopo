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
//! @defresult{audio,指定したものが存在しません,153,1,1,ResultNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotFound, 153, 1, 2);
//! @defresult{audio,処理に失敗しました,153,2,2,ResultOperationFailed}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOperationFailed, 153, 2, 3);
//! @defresult{audio,無効なサンプルレートが指定されました,153,3,3,ResultInvalidSampleRate}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidSampleRate, 153, 3, 4);
//! @defresult{audio,バッファが不足しています,153,4,4,ResultInsufficientBuffer}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInsufficientBuffer, 153, 4, 5);
//! @defresult{audio,リソースが不足しています,153,5,5,ResultOutOfResource}
NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfResource, 153, 5, 6);
//! @defresult{audio,デフォルトのオーディオ入力が利用不可能です,153,6,6,ResultDefaultAudioInNotAvailable}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDefaultAudioInNotAvailable, 153, 6, 7);
//! @defresult{audio,デフォルトのオーディオ出力が利用不可能です,153,7,7,ResultDefaultAudioOutNotAvailable}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDefaultAudioOutNotAvailable, 153, 7, 8);
//! @defresult{audio,既にオープンされています,153,9,9,ResultAlreadyOpen}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAlreadyOpen, 153, 9, 10);
//! @defresult{audio,無効なチャンネル数が指定されました,153,10,10,ResultInvalidChannelCount}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidChannelCount, 153, 10, 11);
//! @defresulta{audio,AudioRenderer の動作中に不明なエラーが発生しました,153,32,127,ResultAudioRendererUnknownError}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultAudioRendererUnknownError, 153, 32, 128);
    //! @defresulta{audio,オーディオレンダラの更新中に不明なエラーが発生しました,153,40,79,ResultUpdateAudioRendererError\, ResultAudioRendererUnknownError}
    NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultUpdateAudioRendererError, 153, 40, 80);
        //! @defresult{audio,RequestUpdateAudioRenderer() に渡されたデータが破損しています。,153,41,41,ResultInvalidUpdateInfo\, ResultUpdateAudioRendererError\, ResultAudioRendererUnknownError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidUpdateInfo, 153, 41, 42);
        //! @defresult{audio,渡されたアドレスが含まれる nn::audio::MemoryPoolType が存在しません,153,42,42,ResultNoMemoryPoolEntry\, ResultUpdateAudioRendererError\, ResultAudioRendererUnknownError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultNoMemoryPoolEntry, 153, 42, 43);
        //! @defresult{audio,オーディオレンダラに渡された SubMix/FinalMix の接続に閉路が検出されました,153,43,43,ResultCycleDetected\, ResultUpdateAudioRendererError\, ResultAudioRendererUnknownError}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultCycleDetected, 153, 43, 44);
//! @defresult{audio,無効なコーデックボリュームが指定されました,153,256,256,ResultInvalidCodecVolume}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidCodecVolume, 153, 256, 257);
//! @defresult{audio,無効なコーデックボリュームターゲットが指定されました,153,257,257,ResultInvalidCodecVolumeTarget}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidCodecVolumeTarget, 153, 257, 258);
//! @defresult{audio,システム上にオーディオデバイスが存在しません,153,512,512,ResultNoAudioDeviceFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNoAudioDeviceFound, 153, 512, 513);
//! @defresult{audio,機能がサポートされていません,153,513,513,ResultNotSupported}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotSupported, 153, 513, 514);
//! @defresult{audio,機能が実装されていません,153,514,514,ResultNotImplemented}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 153, 514, 515);
//! @defresult{audio,指定された applet 識別子が見つかりません,153,1024,1024,ResultAppletResourceUserIdNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAppletResourceUserIdNotFound, 153, 1024, 1025);
//! @defresult{audio,applet 識別子の登録数が既に限界に達しています,153,1025,1025,ResultMaxAppletResourceUserId}
NN_DEFINE_ERROR_RANGE_RESULT(ResultMaxAppletResourceUserId, 153, 1025, 1026);
