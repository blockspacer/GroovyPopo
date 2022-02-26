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
 * @brief   コンパイラのスレッドローカル実装用メモリアロケータ API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Allocator.h>

namespace nn { namespace os {

//! @name コンパイラのスレッドローカル用メモリアロケータの初期化
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   コンパイラのスレッドローカル実装で使用されるメモリアロケータ／デアロケータを登録します。
 *
 * @param[in] allocator     メモリアロケータへの関数ポインタ
 * @param[in] deallocator   メモリデアロケータへの関数ポインタ
 *
 * @pre
 *  - プログラムの中で本 API の呼出しが初めてである
 *
 * @details
 *  コンパイラのスレッドローカル指定子（_Thread_local, thread_local, __thread）
 *  の実装で使用されるメモリアロケータおよびデアロケータ関数を登録します。
 *  （ただし、コンパイラがこれらの指定子をサポートしているかどうかは、
 *    別途コンパイラのドキュメントを参照して下さい。）
 *
 *  本関数は各プログラムの起動時、具体的には nninitStartup() の中で一度だけ
 *  呼ばれることを想定しています。デフォルトの nninitStartup() では
 *  メモリヒープの初期化を行なった後に本関数が呼ばれ、スレッドローカル用の
 *  デフォルトアロケータ関数およびデフォルトデアロケータ関数が登録されます。
 *
 *  上記デフォルトのアロケータ関数、デアロケータ関数は malloc() や free() と
 *  同じメモリ領域を使用します。これらの領域と異なるメモリ領域を使用させたい
 *  場合は、アプリケーションで独自に nninitStartup() を定義し、かつその中で、
 *  別途用意したアロケータ関数、デアロケータ関数を登録して下さい。
 *
 *  なお、アロケータおよびデアロケータの関数には nn::AlignedAllocateFunction 型
 *  および nn::DeallocateFunction 型の関数ポインタを渡して下さい。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は何も行なわずにリターンし、アロケータ関数および
 *    デアロケータ関数は未登録のままとなります。
 *  - Windows 環境におけるスレッドローカル指定されたオブジェクトの振舞いは、
 *    Visual C++ の仕様に従います。
 *
 * @platformend
 *
 */
void SetMemoryAllocatorForThreadLocal(nn::AlignedAllocateFunction allocator, nn::DeallocateFunction deallocator) NN_NOEXCEPT;

//! @}

}}  // namespace nn::os

