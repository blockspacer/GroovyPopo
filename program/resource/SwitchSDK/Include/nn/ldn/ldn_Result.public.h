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
//! @defresult{ldn,通信デバイスが使用できない状態です。,203,16,31,ResultDeviceNotAvailable}
NN_DEFINE_ERROR_RANGE_RESULT(ResultDeviceNotAvailable, 203, 16, 32);
    //! @defresult{ldn,既に通信デバイスが占有されています。,203,17,21,ResultDeviceOccupied\, ResultDeviceNotAvailable}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDeviceOccupied, 203, 17, 22);
    //! @defresult{ldn,無線機能が無効化されており、通信デバイスを使用できません。,203,22,26,ResultDeviceDisabled\, ResultDeviceNotAvailable}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDeviceDisabled, 203, 22, 27);
        //! @defresult{ldn,機内モードの設定により、無線機能が無効化されています。,203,23,23,ResultWifiOff\, ResultDeviceDisabled\, ResultDeviceNotAvailable}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultWifiOff, 203, 23, 24);
        //! @defresult{ldn,スリープ状態のため、無線機能が無効化されています。,203,24,24,ResultSleep\, ResultDeviceDisabled\, ResultDeviceNotAvailable}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultSleep, 203, 24, 25);
//! @defresult{ldn,ライブラリの状態が正しくありません。,203,32,47,ResultInvalidState}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidState, 203, 32, 48);
//! @defresult{ldn,対象のノードが見つかりませんでした。,203,48,63,ResultNodeNotFound}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNodeNotFound, 203, 48, 64);
//! @defresult{ldn,対象のネットワークへの接続に失敗しました。,203,64,79,ResultConnectionFailed}
NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionFailed, 203, 64, 80);
    //! @defresult{ldn,対象のネットワークが見つかりませんでした。,203,65,65,ResultNetworkNotFound\, ResultConnectionFailed}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNetworkNotFound, 203, 65, 66);
    //! @defresult{ldn,接続要求がタイムアウトしました。,203,66,66,ResultConnectionTimeout\, ResultConnectionFailed}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionTimeout, 203, 66, 67);
    //! @defresult{ldn,接続要求が拒否されました。,203,67,67,ResultConnectionRejected\, ResultConnectionFailed}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConnectionRejected, 203, 67, 68);
    //! @defresult{ldn,LDN ネットワークではありません。,203,68,68,ResultInvalidNetwork\, ResultConnectionFailed}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidNetwork, 203, 68, 69);
//! @defresult{ldn,処理がキャンセルされました。,203,80,95,ResultCancelled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultCancelled, 203, 80, 96);
//! @defresulta{ldn,ローカル通信バージョンが一致しないため接続できません。,203,112,127,ResultIncompatibleVersion}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultIncompatibleVersion, 203, 112, 128);
    //! @defresult{ldn,ローカル通信バージョンが通信相手よりも低いため接続できません。おすそわけ通信によるアップデートを試行できます。,203,113,113,ResultLowerVersion\, ResultIncompatibleVersion}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultLowerVersion, 203, 113, 114);
    //! @defresult{ldn,ローカル通信バージョンが通信相手よりも高いため接続できません。おすそわけ通信によるアップデートを試行できます。,203,114,114,ResultHigherVersion\, ResultIncompatibleVersion}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHigherVersion, 203, 114, 115);
    //! @defresult{ldn,ローカル通信バージョンが通信相手よりも低いため接続できません。おすそわけ通信によるアップデートは実行できません。,203,115,115,ResultLowerVersionCannotUpdate\, ResultIncompatibleVersion}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultLowerVersionCannotUpdate, 203, 115, 116);
    //! @defresult{ldn,ローカル通信バージョンが通信相手よりも高いため接続できません。おすそわけ通信によるアップデートは実行できません。,203,116,116,ResultHigherVersionCannotUpdate\, ResultIncompatibleVersion}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultHigherVersionCannotUpdate, 203, 116, 117);
//! @defresult{ldn,ネットワークの最大ノード数が上限を超えています。,203,144,159,ResultNodeCountLimitation}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNodeCountLimitation, 203, 144, 160);
