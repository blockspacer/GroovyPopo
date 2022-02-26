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

namespace nn {
namespace vfx {

//----------------------------------------
//! @name ディスクリプタ スロット関数の定義
//@{

//----------------------------------------
//! @brief テクスチャディスクリプタスロット確保関数のインタフェースです。
//!
//! @param[out] dstSlot     確保するスロット
//! @param[in]  textureView テクスチャビューです。
//! @param[in]  pUserData   ユーザデータです。
//!
//! @return                 確保できなかった場合は、 false を返します。
//----------------------------------------
typedef bool( *RegisterTextureViewSlot )( nn::gfx::DescriptorSlot* dstSlot, const nn::gfx::TextureView& textureView, void* pUserData );

//----------------------------------------
//! @brief サンプラーディスクリプタスロット確保関数のインタフェースです。
//!
//! @param[out] dstSlot     確保するスロット
//! @param[in]  sampler     サンプラーです。
//! @param[in]  pUserData   ユーザデータです。
//!
//! @return                 確保できなかった場合は、 false を返します。
//----------------------------------------
typedef bool( *RegisterSamplerSlot )( nn::gfx::DescriptorSlot* dstSlot, const nn::gfx::Sampler& sampler, void* pUserData );

//----------------------------------------
//! @brief テクスチャディスクリプタスロット解放関数のインタフェースです。
//!
//! @param[out] dstSlot     解放するスロットです。
//! @param[in]  textureView テクスチャビューです。
//! @param[in]  pUserData   ユーザデータです。
//----------------------------------------
typedef void( *UnregisterTextureViewSlot )( nn::gfx::DescriptorSlot* dstSlot, const nn::gfx::TextureView& textureView, void* pUserData );

//----------------------------------------
//! @brief サンプラーディスクリプタスロット解放関数のインタフェースです。
//!
//! @param[out] dstSlot     解放するスロットです。
//! @param[in]  sampler     サンプラーです。
//! @param[in]  pUserData   ユーザデータです。
//----------------------------------------
typedef void( *UnregisterSamplerSlot )( nn::gfx::DescriptorSlot* dstSlot, const nn::gfx::Sampler& sampler, void* pUserData );

//@}

} // namespace vfx
} // namespace nn
