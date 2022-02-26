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
 * @brief   スタートアップ初期化ルーチンの宣言
 */

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>

//--------------------------------------------------------------------------
//  C リンケージでの宣言
//--------------------------------------------------------------------------

//! @name スタートアップ初期化ルーチン
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   スタートアップ初期化用ルーチンです。
 *
 * @details
 *  アプリケーションがメモリ管理初期化用に使用するための関数です。
 *
 *  この関数はアプリケーションが呼び出す関数ではなく、アプリケーションで
 *  実装すべき関数です。本関数は C リンケージで定義して下さい。
 *  アプリケーションで本関数を独自に定義しない場合は、
 *  libnn_init ライブラリで定義されたデフォルトの実装が適用されます。
 *
 *  C++ では使い方によって nnMain() より前、例えば静的オブジェクトの
 *  コンストラクタなどでメモリ確保が必要になることがあります。
 *  そのためには、事前にアプリケーション独自のメモリ管理機構の初期化を
 *  行っておく必要があります。本関数は静的オブジェクトのコンストラクタよりも
 *  前に呼び出されるため、そこでメモリ管理機構の初期化を行なうことが出来ます。
 *
 *  この関数はアプリケーションの実行に必要な初期化が完全には実行されていない
 *  段階で呼び出されるため、SDK の多くの API が使用できない等の制約があります。
 *
 *  この関数の中から使用することのできる SDK の API については、
 *  @ref PageNotificationInitAvailableApiListInStartup を参照してください。
 *
 *  また、実際に nninitStartup() を定義する使い方については以下のサンプルも
 *  合わせて参照して下さい。
 *
 *  - @ref PageSampleInitStartup
 *  - @ref PageSampleInitStartupWithMalloc
 *
 */
extern "C" void nninitStartup(void);

//! @}


//--------------------------------------------------------------------------
//  廃止された nninitStartUp() 関数（'U' が大文字）を定義しようとした場合に
//  ビルドエラーとするための措置（可能なら対処法をメッセージで出力）。
//  また、ライブラリ内に同名関数を定義しているため、本ヘッダが include され
//  なかった場合でも多重定義によりリンクエラーとなる。
//
extern "C" {
#if defined(NN_BUILD_CONFIG_COMPILER_VC)
    // VC++ ではこの pragma で Warning を出す。
    #pragma deprecated(nninitStartUp)
#else
    // nninitStartUp() 関数を定義しようとした場合に文法エラーで落とす。
    #define nninitStartUp \
            "WARNING: You need to use nninitStartup() instead."
#endif
}

