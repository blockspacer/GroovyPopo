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
*   @file
*   @brief  メモリアロケータ関連のファイルシステム API の宣言
*/

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace fs {

/**
*   @brief      メモリ確保用の関数ポインタです。
*   @details    SetAllocator() に指定します。
*/
    typedef void* (*AllocateFunction)(size_t);

/**
*   @brief      メモリ解放用の関数ポインタです。
*   @details    SetAllocator() に指定します。
*/
    typedef void(*DeallocateFunction)(void*, size_t);

//! @name ファイルシステム関連 API
//! @{

    //--------------------------------------------------------------------------
    /**
    *   @brief      fs ライブラリが使用するメモリアロケータを設定します。
    *
    *   @param[in]  alloc           メモリを確保する関数
    *   @param[in]  dealloc         メモリを開放する関数
    *
    */
    void SetAllocator(AllocateFunction alloc, DeallocateFunction dealloc) NN_NOEXCEPT;

//! @}

}}
