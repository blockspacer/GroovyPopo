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
 * @brief   乱数に関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>


namespace nn { namespace os {

//! @name 乱数生成
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   ランダムなバイト列を生成します
 *
 * @param[in]    pOutBuffer  ランダムなバイト列を格納するバッファを指定します。
 * @param[in]    size        生成するバイト列のサイズを指定します。
 *
 * @details
 *  初期化済の乱数生成器を使用して size バイトのランダムなバイト列を生成し、
 *  pOutBuffer で指定されるバッファに格納します。
 *
 *  乱数生成器のシードを含む比較的少量の乱数生成に使用することを想定しています。
 *  大量の乱数を生成する場合や乱数の再現性が必要な場合には、
 *  別途 C++ 標準ライブラリや @ref nn::util::TinyMt を使用してください。
 *
 *  本関数は暗号用途では利用できません。
 *  暗号用途の乱数生成には @ref nn::crypto::GenerateCryptographicallyRandomBytes が用意されています。
 *
 *  本 API はスレッドセーフです。
 *
 */
void GenerateRandomBytes(void* pOutBuffer, size_t size) NN_NOEXCEPT;


//--------------------------------------------------------------------------

//! @}

}} // namespace nn::os

