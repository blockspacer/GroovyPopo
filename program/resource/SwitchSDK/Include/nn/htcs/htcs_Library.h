/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

/**
* @file
* @brief   ホスト・ターゲット間ソケット通信の関数
*/
#include <nn/nn_Allocator.h>
#include <nn/htcs/htcs_Types.h>

namespace nn { namespace htcs {

/**
*   @brief      ホスト・ターゲット間ソケット通信を初期化します。@n
*               nn::htcs::GetWorkingMemorySize(nn::htcs::SocketCountMax) で返るサイズのメモリが、この関数呼び出し時に alloc を使用して確保されます。@n
*               これ以降はメモリのアロケートは行われません。
*
*   @param[in]  alloc       メモリ確保用の関数を指定します。
*   @param[in]  dealloc     メモリ解放用の関数を指定します。
*/
void Initialize(AllocateFunction alloc, DeallocateFunction dealloc) NN_NOEXCEPT;

/**
*   @brief      ホスト・ターゲット間ソケット通信を初期化します。@n
*               必要なバッファのサイズは nn::htcs::GetWorkingMemorySize で算出できます。
*
*   @param[in]  buffer      割り当てるバッファを指定します。
*   @param[in]  bufferSize  バッファのサイズを指定します。
*/
void Initialize(void* buffer, size_t bufferSize) NN_NOEXCEPT;

/**
*   @brief      ホスト・ターゲット間ソケット通信を終了します。
*/
void Finalize() NN_NOEXCEPT;

/**
*   @brief      ホスト・ターゲット間ソケット通信が初期化されているかどうかを返します。
*
*   @return     処理の結果が返ります。
*   @retval true  初期化済み状態です。
*   @retval false 未初期化状態です。
*/
bool IsInitialized() NN_NOEXCEPT;

/**
*   @brief      使用するソケット数に応じてライブラリ内で使用される最大バイト数を返します。
*
*   @param[in]  socketCountMax       使用するソケットの最大数を指定します。
*
*   @return     確保される最大のバイト数が返ります。
*   @pre
*               - socketCountMax <= nn::htcs::SocketCountMax
*/
size_t GetWorkingMemorySize(int socketCountMax) NN_NOEXCEPT;

}}

