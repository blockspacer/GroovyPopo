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
 * @brief   高速に確保・解放が可能な動的メモリ確保ライブラリを include するヘッダです。
 * @details 高速に確保・解放が可能な動的メモリ確保ライブラリを利用する場合、このヘッダを include してください。
 */


#pragma once


/**
 * @namespace nn::mem
 * @brief     高速に確保・解放が可能な動的メモリ確保ライブラリの名前空間です。
 * @details   高速に確保・解放が可能な動的メモリ確保ライブラリはこの名前空間に用意されます。@n
 *            Cafe SDK と CTR-SDK と同等機能を持つヒープは、nn::lmem 名前空間に配置されます。
 */

#include <nn/mem/mem_StandardAllocator.h>
