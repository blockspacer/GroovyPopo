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
 * @brief   メモリアロケータの初期化を行なう API の宣言
 */

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/nn_Common.h>
#include <nn/mem.h>

namespace nn { namespace init {

//! @name メモリアロケータの初期化
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   メモリアロケータを初期化します。
 *
 * @param[in] address   メモリアロケータが使用するメモリ領域の先頭アドレス
 * @param[in] size      メモリアロケータが使用するメモリ領域のサイズ
 *
 * @pre
 *  - プログラムの中で本 API の呼出しが初めてである
 *  - size > 0
 *
 * @details
 *  nn::init::InitializeAllocator(address, size, false) と等価です。
 *
 */
void InitializeAllocator(void* address, size_t size) NN_NOEXCEPT;

/**
 * @brief   メモリアロケータを初期化します。
 *
 * @param[in] address       メモリアロケータが使用するメモリ領域の先頭アドレス
 * @param[in] size          メモリアロケータが使用するメモリ領域のサイズ
 * @param[in] isCacheEnable スレッド毎のメモリキャッシュ機能を有効にするか
 *
 * @pre
 *  - プログラムの中で本 API の呼出しが初めてである
 *  - size > 0
 *
 * @details
 *  メモリアロケータを初期化します。
 *
 *  SDK のデフォルト状態では new や malloc は、この関数で初期化されるアロケータ
 *  を使って実際のメモリ獲得を行ないます。このため、この関数に指定する size
 *  に応じて、new や malloc で獲得できるメモリサイズが決定します。
 *
 *  メモリアロケータでは、その内部実装に nn::mem::StandardAllocator を使用
 *  しています。そのため、本 API の引数で指定する address, size および isCacheEnable は、
 *  以下の API を参考に指定してください。
 *
 *  - void nn::mem::StandardAllocator::Initialize(void* addr, size_t size, bool isCacheEnable);
 *
 *  なお、デフォルトの nninitStartup() が適用された場合、
 *  確保可能なメモリをすべてメモリヒープとして取得したうえで、本 API を使ってメモリアロケータを初期化します。@n
 *  デフォルトの nninitStartup() ではスレッド毎のキャッシュ機能は有効 (isCacheEnable には true が指定) となります。@n
 *  メモリアロケータで使用するバッファのサイズを変更したい場合は、
 *  @ref PageSampleInitStartup のサンプルを参照して下さい。
 *
 *  一方、独自のメモリ管理を行なう場合には以下のようにしてください。
 *
 *  - 独自の nninitStartup() を定義してください。
 *  - 独自の malloc(), free(), calloc(), realloc(), aligned_alloc() を定義してください。
 *  - InitializeAllocator() を呼び出さないでください。
 *
 *  上記のいずれかを満たさない場合、ビルド時にリンクエラーとなります。
 *  詳細は @ref PageSampleInitStartupWithMalloc を参照して下さい。
 *
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は何も行なわずにリターンします。
 *  - Windows 環境では malloc(), free(), calloc(), realloc() は Windows 標準の
 *    API がそのまま呼ばれます。
 *  - aligned_alloc() は VisualStudio のバージョンによっては使用できない
 *    場合があります。
 *
 * @platformend
 *
 */
void InitializeAllocator(void* address, size_t size, bool isCacheEnable) NN_NOEXCEPT;

/**
 * @brief   メモリアロケータの内部実装クラスオブジェクトを取得します。
 *
 * @pre
 *  - InitializeAllocator() によりメモリアロケータが初期化されている
 *
 * @return
 *  メモリアロケータの内部実装クラスオブジェクトが返ります。
 *
 * @details
 *  メモリアロケータでは、その内部実装に nn::mem::StandardAllocator を使用
 *  しています。本 API ではメモリアロケータが利用している nn::mem::StandardAllocator オブジェクトを返します。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は 常に NULL を返します。
 *
 * @platformend
 *
 */
nn::mem::StandardAllocator* GetAllocator() NN_NOEXCEPT;

//! @}

}}  // namespace nn::init

