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
//! @defresult{fs,どのプロセスからでもハンドリングされるべきエラー,2,0,999,ResultHandledByAllProcess}
NN_DEFINE_ERROR_RANGE_RESULT(ResultHandledByAllProcess, 2, 0, 1000);
    //! @defresult{fs,指定されたパスに対象が見つからない,2,1,1,ResultPathNotFound\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultPathNotFound, 2, 1, 2);
    //! @defresult{fs,対象が既に存在している,2,2,2,ResultPathAlreadyExists\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultPathAlreadyExists, 2, 2, 3);
    //! @defresult{fs,対象がロックされている,2,7,7,ResultTargetLocked\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultTargetLocked, 2, 7, 8);
    //! @defresult{fs,対象のディレクトリ内にエントリがある,2,8,8,ResultDirectoryNotEmpty\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDirectoryNotEmpty, 2, 8, 9);
    //! @defresult{fs,ディレクトリ列挙中にディレクトリの状態が変わった,2,13,13,ResultDirectoryStatusChanged\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDirectoryStatusChanged, 2, 13, 14);
    //! @defresult{fs,空き容量が不足,2,30,45,ResultUsableSpaceNotEnough\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUsableSpaceNotEnough, 2, 30, 46);
        //! @defresult{fs,セーブデータを作成するための空き容量が不足,2,31,31,ResultUsableSpaceNotEnoughForSaveData\, ResultUsableSpaceNotEnough\, ResultHandledByAllProcess}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultUsableSpaceNotEnoughForSaveData, 2, 31, 32);
        //! @defresult{fs,キャッシュストレージを作成するための空き容量が不足,2,33,33,ResultUsableSpaceNotEnoughForCacheStorage\, ResultUsableSpaceNotEnough\, ResultHandledByAllProcess}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultUsableSpaceNotEnoughForCacheStorage, 2, 33, 34);
    //! @defresult{fs,サポートされていない SDK バージョン,2,50,50,ResultUnsupportedSdkVersion\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUnsupportedSdkVersion, 2, 50, 51);
    //! @defresult{fs,マウント名が既に存在している,2,60,60,ResultMountNameAlreadyExists\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultMountNameAlreadyExists, 2, 60, 61);
    //! @defresult{fs,個別ファイルデータキャッシュが有効化済み,2,70,70,ResultIndividualFileDataCacheAlreadyEnabled\, ResultHandledByAllProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultIndividualFileDataCacheAlreadyEnabled, 2, 70, 71);
//! @defresult{fs,システムプロセスのみハンドリングするエラー,2,1000,2999,ResultHandledBySystemProcess}
NN_DEFINE_ERROR_RANGE_RESULT(ResultHandledBySystemProcess, 2, 1000, 3000);
    //! @defresult{fs,対象のパーティションが見つからない,2,1001,1001,ResultPartitionNotFound\, ResultHandledBySystemProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultPartitionNotFound, 2, 1001, 1002);
    //! @defresult{fs,対象が見つからない,2,1002,1002,ResultTargetNotFound\, ResultHandledBySystemProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultTargetNotFound, 2, 1002, 1003);
    //! @defresult{fs,Nca の外部鍵が利用できない,2,1004,1004,ResultNcaExternalKeyUnavailable\, ResultHandledBySystemProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNcaExternalKeyUnavailable, 2, 1004, 1005);
    //! @defresult{fs,SD カードへのアクセスに失敗,2,2000,2499,ResultSdCardAccessFailed\, ResultHandledBySystemProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultSdCardAccessFailed, 2, 2000, 2500);
    //! @defresult{fs,ゲームカードへのアクセスに失敗,2,2500,2999,ResultGameCardAccessFailed\, ResultHandledBySystemProcess}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultGameCardAccessFailed, 2, 2500, 3000);
        //! @defresult{fs,GameCard - ゲームカードへのアクセスエラー,2,2530,2559,ResultGameCardCardAccessFailure\, ResultGameCardAccessFailed\, ResultHandledBySystemProcess}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultGameCardCardAccessFailure, 2, 2530, 2560);
        //! @defresult{fs,GameCard - ASIC 初期化エラー,2,2629,2669,ResultGameCardInitializeAsicFailure\, ResultGameCardAccessFailed\, ResultHandledBySystemProcess}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultGameCardInitializeAsicFailure, 2, 2629, 2670);
    //! @defresult{fs,未実装の操作,2,3001,3001,ResultNotImplemented\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultNotImplemented, 2, 3001, 3002);
    //! @defresult{fs,バージョンが新し過ぎる,2,3002,3002,ResultUnsupportedVersion\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUnsupportedVersion, 2, 3002, 3003);
    //! @defresult{fs,同名のファイルまたはディレクトリが既に存在する,2,3003,3003,ResultAlreadyExists\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultAlreadyExists, 2, 3003, 3004);
    //! @defresult{fs,操作範囲が領域外,2,3005,3005,ResultOutOfRange\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfRange, 2, 3005, 3006);
    //! @defresult{fs,アクセスエラーが発生し遅延した処理の解決に失敗,2,3006,3006,ResultDeferredProcessRetryFailure\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDeferredProcessRetryFailure, 2, 3006, 3007);
    //! @defresult{fs,メモリ不足,2,3200,3499,ResultAllocationMemoryFailed\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultAllocationMemoryFailed, 2, 3200, 3500);
    //! @defresult{fs,本体保存メモリーへのアクセスに失敗,2,3500,3999,ResultMmcAccessFailed\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultMmcAccessFailed, 2, 3500, 4000);
    //! @defresult{fs,データ壊れや改竄の検知,2,4000,4999,ResultDataCorrupted\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDataCorrupted, 2, 4000, 5000);
            //! @defresult{fs,ホストファイルシステムのデータ壊れや改竄の検知,2,4241,4259,ResultRomHostFileSystemCorrupted\, ResultRomCorrupted\, ResultDataCorrupted\, ResultInternal}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultRomHostFileSystemCorrupted, 2, 4241, 4260);
                //! @defresult{fs,ホストのエントリが壊れている,2,4242,4242,ResultRomHostEntryCorrupted\, ResultRomHostFileSystemCorrupted\, ResultRomCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultRomHostEntryCorrupted, 2, 4242, 4243);
                //! @defresult{fs,ホストのデータが壊れている,2,4243,4243,ResultRomHostFileDataCorrupted\, ResultRomHostFileSystemCorrupted\, ResultRomCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultRomHostFileDataCorrupted, 2, 4243, 4244);
                //! @defresult{fs,ホストのファイルが壊れている,2,4244,4244,ResultRomHostFileCorrupted\, ResultRomHostFileSystemCorrupted\, ResultRomCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultRomHostFileCorrupted, 2, 4244, 4245);
                //! @defresult{fs,ホストのハンドルが不正,2,4245,4245,ResultInvalidRomHostHandle\, ResultRomHostFileSystemCorrupted\, ResultRomCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidRomHostHandle, 2, 4245, 4246);
            //! @defresult{fs,ホストファイルシステムのデータ壊れや改竄の検知,2,4441,4459,ResultSaveDataHostFileSystemCorrupted\, ResultSaveDataCorrupted\, ResultDataCorrupted\, ResultInternal}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultSaveDataHostFileSystemCorrupted, 2, 4441, 4460);
                //! @defresult{fs,ホストのエントリが壊れている,2,4442,4442,ResultSaveDataHostEntryCorrupted\, ResultSaveDataHostFileSystemCorrupted\, ResultSaveDataCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultSaveDataHostEntryCorrupted, 2, 4442, 4443);
                //! @defresult{fs,ホストのデータが壊れている,2,4443,4443,ResultSaveDataHostFileDataCorrupted\, ResultSaveDataHostFileSystemCorrupted\, ResultSaveDataCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultSaveDataHostFileDataCorrupted, 2, 4443, 4444);
                //! @defresult{fs,ホストのファイルが壊れている,2,4444,4444,ResultSaveDataHostFileCorrupted\, ResultSaveDataHostFileSystemCorrupted\, ResultSaveDataCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultSaveDataHostFileCorrupted, 2, 4444, 4445);
                //! @defresult{fs,ホストのハンドルが不正,2,4445,4445,ResultInvalidSaveDataHostHandle\, ResultSaveDataHostFileSystemCorrupted\, ResultSaveDataCorrupted\, ResultDataCorrupted\, ResultInternal}
                NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidSaveDataHostHandle, 2, 4445, 4446);
        //! @defresult{fs,ホストファイルシステムのデータ壊れや改竄の検知,2,4701,4719,ResultHostFileSystemCorrupted\, ResultDataCorrupted\, ResultInternal}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultHostFileSystemCorrupted, 2, 4701, 4720);
            //! @defresult{fs,ホストのエントリが壊れている,2,4702,4702,ResultHostEntryCorrupted\, ResultHostFileSystemCorrupted\, ResultDataCorrupted\, ResultInternal}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultHostEntryCorrupted, 2, 4702, 4703);
            //! @defresult{fs,ホストのデータが壊れている,2,4703,4703,ResultHostFileDataCorrupted\, ResultHostFileSystemCorrupted\, ResultDataCorrupted\, ResultInternal}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultHostFileDataCorrupted, 2, 4703, 4704);
            //! @defresult{fs,ホストのファイルが壊れている,2,4704,4704,ResultHostFileCorrupted\, ResultHostFileSystemCorrupted\, ResultDataCorrupted\, ResultInternal}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultHostFileCorrupted, 2, 4704, 4705);
            //! @defresult{fs,ホストのハンドルが不正,2,4705,4705,ResultInvalidHostHandle\, ResultHostFileSystemCorrupted\, ResultDataCorrupted\, ResultInternal}
            NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidHostHandle, 2, 4705, 4706);
        //! @defresult{fs,ゲームカード上のロゴデータのデータ壊れや改竄の検知,2,4781,4781,ResultGameCardLogoDataCorrupted\, ResultDataCorrupted\, ResultInternal}
        NN_DEFINE_ERROR_RANGE_RESULT(ResultGameCardLogoDataCorrupted, 2, 4781, 4782);
    //! @defresult{fs,不明な失敗,2,5000,5999,ResultUnexpected\, ResultInternal}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultUnexpected, 2, 5000, 6000);
