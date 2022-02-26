/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   開発用 ROM でのみ利用可能な機能の宣言
 *
 * @details
 *  本ヘッダファイルは nn/oe.h からはインクルードされていません。
 *  本ヘッダに記載の API を使用する際には、本ヘッダをインクルードして下さい。
 *
 *  また、このヘッダに記載の API を製品 ROM に含めることはできません。
 *
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/oe/oe_ApplicationControlTypes.h>

namespace nn { namespace oe {

//! @name アプリケーションの情報取得に関する API
//! @{

/**
 * @brief ストレージ種別を表す列挙型です。
 *
 * @details
 *  プログラムの起動ストレージやパッチ格納ストレージを表す列挙型です。
 *  詳細は各列挙子の説明を参照して下さい。
 *
 *  なお、 nn::oe::StorageTypeForDebug_Unknown は対象ストレージが未知もしくは
 *  不明であることを示します。より新しい本体やファームウェア環境でプログラムが
 *  起動した場合、この値が返されることがあります。
 *
 *  また、起動ストレージとして nn::oe::StorageTypeForDebug_None が
 *  返されることはありません。
 *  パッチ格納ストレージとして nn::oe::StorageTypeForDebug_None が
 *  返された場合は、パッチなしを意味します。
 *
 */
enum StorageTypeForDebug
{
    StorageTypeForDebug_Unknown        = -1, //!< 未知もしくは不明なストレージ
    StorageTypeForDebug_None           = 0,  //!< 対象ストレージなし
    StorageTypeForDebug_Host           = 1,  //!< ホスト PC
    StorageTypeForDebug_GameCard       = 2,  //!< ゲームカード
    StorageTypeForDebug_BuiltInStorage = 4,  //!< 内蔵ストレージ
    StorageTypeForDebug_SdCard         = 5,  //!< SD カード
};

/**
 * @brief プログラムの起動ストレージに関する情報を格納する構造体です。
 *
 */
struct LaunchStorageInfoForDebug
{
    StorageTypeForDebug launchStorage; //!< プログラムの起動ストレージ種別
    StorageTypeForDebug patchStorage;  //!< プログラムのパッチ格納ストレージ種別
};


/**
 * @brief   プログラムの起動ストレージ情報を取得します。
 *
 * @param[out] pOutInfo 起動ストレージ情報の格納先アドレス
 *
 * @details
 *  アプリケーションプログラムがどのストレージから起動したかの情報を返します。
 *  起動ストレージ情報は pOutInfo で示される nn::oe::LaunchStorageInfoForDebug
 *  構造体に格納されます。
 *
 *  nn::oe::LaunchStorageInfoForDebug の launchStorage にはプログラムの
 *  起動ストレージ種別が代入されます。ここに nn::oe::StorageTypeForDebug_None
 *  が格納されることはありません。
 *
 *  nn::oe::LaunchStorageInfoForDebug の patchStorage にはプログラムに
 *  適用されたパッチが格納されているストレージ種別が代入されます。
 *  ここが nn::oe::StorageTypeForDebug_None の場合は、パッチなしを意味します。
 *
 *  なお、本 API を製品 ROM に含めることはできません。開発時にのみ利用可能です。
 *
 */
void GetLaunchStorageInfoForDebug(LaunchStorageInfoForDebug* pOutInfo) NN_NOEXCEPT;

/**
 * @brief   クラッシュ時に全スレッドのログを出すかどうか
 *
 * @param[in] isEnabled 全ログを出す場合に true を指定します。
 *
 * @details
 *  アプリクラッシュ時に、全スレッドのログを出力するかどうかを切り替えます。
 *  同様の設定は DevMenu > Debug > CPU Crash Output Threads Log でも行うことが可能ですが、
 *  上記は永続的に設定が切り替わるのに対して、本関数での設定は呼び出したアプリにのみ適用されます。
 *  本関数を呼び出した場合、呼び出したアプリに限り、DevMenu での設定が無視されます。
 *  なお、本 API を製品 ROM に含めることはできません。開発時にのみ利用可能です。
 */
void EnableAllThreadDumpOnCrashForDebug(bool isEnabled) NN_NOEXCEPT;

//! @}

}} // namespace nn::oe

