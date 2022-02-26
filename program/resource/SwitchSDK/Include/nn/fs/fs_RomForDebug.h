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
*   @brief  デバッグ用途のリソースデータ（ROM）関連 API の宣言
*/

#pragma once

namespace nn { namespace fs {

//! @name デバッグ用途 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      nn::fs::MountRom() が実行可能かどうかを取得します。
*
*   @return     処理の結果が返ります。
*   @retval     true                            MountRom() が実行可能です。
*   @retval     false                           MountRom() が実行できません。
*
*   @pre
*       - マウントされている数が @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限未満である。
*
*   @details    この API はデバッグ用途に限り使用可能です。
*/
bool CanMountRomForDebug() NN_NOEXCEPT;

}} // namespace nn::fs
