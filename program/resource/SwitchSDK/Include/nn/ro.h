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
 * @brief   動的モジュールに関する API の宣言
 */


#pragma once


/**
 * @namespace   nn::ro
 * @brief       RO ライブラリの名前空間です。
 *
 *              ### 機能概要
 *              RO ライブラリでは、動的モジュールを利用するための手段を提供します。
 *
 *              ### 使用方法
 *              1 つの動的モジュールを読み込んで使用し、すべてのメモリを解放するまでのフローは次のようになります。@n
 *              @n
 *              1.  nn::ro::Initialize() を呼び出す。@n
 *              2.  nrr ファイルを読み込むためのヒープ領域を確保する。@n
 *              3.  nrr ファイルを確保したヒープ領域に読み込む。@n
 *              4.  nn::ro::RegisterModuleInfo() を呼び出す。@n
 *              5.  nro ファイルを読み込むためのヒープ領域を確保する。@n
 *              6.  nro ファイルを確保したヒープ領域に読み込む。@n
 *              7.  nn::ro::GetBufferSize() を呼び出し、動的モジュールの BSS セクションに
 *                  必要なヒープ領域のサイズを調べる。@n
 *              8.  ヒープ領域を確保する。@n
 *              9.  nn::ro::LoadModule() を呼び出す。@n
 *              10. nn::ro::LookupSymbol() や nn::ro::LookupModuleSymbol() を利用し、
 *                  読み込んだ動的モジュールの機能を利用する。@n
 *              11. nn::ro::UnloadModule() を呼び出す。@n
 *              12. 動的モジュールの BSS セクションのために確保したヒープ領域を解放する。
 *              13. nro ファイルのために確保したメモリを解放する。@n
 *              14. nn::ro::UnregisterModuleInfo() を呼び出す。@n
 *              15. nrr ファイルのために確保したメモリを解放する。@n
 *              16. nn::ro::Finalize() を呼び出す。@n
 *
 *              ### 用語
 *              #### 実行コード
 *              CPU が解釈し実行する機械語列や、そこから直接参照される定数/変数類のこと。
 *              #### モジュール
 *              意味のある単位で区切られたひとまとまりの実行コード。
 *              #### 静的モジュール
 *              アプリケーションの起動時にロードされるメモリが確保されているモジュール。 アプリケーションの実行中に解放されることがない。
 *              #### 動的モジュール
 *              動的にメモリに読み込んで使用することが出来るモジュール。アプリケーションの実行中に解放することが出来る。
 *              #### シンボル
 *              モジュール内の意味を持つ位置のこと。 多くの場合は名前がつけられているが、名前がない場合もある。
 *              #### 参照
 *              他のモジュールのシンボルを使用すること。@n
 *              他のモジュールで定義されている関数を呼び出したり、 他のモジュールで定義されている変数を読み書きすることにあたる。
 *              #### 解決
 *              参照するシンボルを利用可能な状態にすること。@n
 *              シンボルのアドレスを決定し、関数呼び出しや、変数の読み書きができるようになる。
 *              #### nro ファイル
 *              動的モジュールのファイル。
 *              #### nrr ファイル
 *              nro ファイルの登録情報が格納されたファイル。@n
 *              1 つ以上の nro ファイルの情報が格納されている。
 *
 */

#include <nn/ro/ro_Result.h>
#include <nn/ro/ro_Api.h>
#include <nn/ro/ro_Types.h>

