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
 * @brief nn::gfx オブジェクトにデバッグラベルを追加するユーティリティ関数を定義します。
 */

#pragma once

#include <nn/gfx.h>

namespace nn {
namespace gfx {
namespace util {

/**
* @brief nn::gfx::MemoryPool にデバッグラベルを追加します。
*
* @param[in] pMemoryPool デバッグラベルを追加するメモリプールのポインタ
* @param[in] label デバッグラベルの文字列
*
*/
template< typename TTarget >
void SetMemoryPoolDebugLabel( TMemoryPool< TTarget >* pMemoryPool, const char* label ) NN_NOEXCEPT;

/**
* @brief nn::gfx::Buffer にデバッグラベルを追加します。
*
* @param[in] pBuffer デバッグラベルを追加するバッファのポインタ
* @param[in] label デバッグラベルの文字列
*
*/
template< typename TTarget >
void SetBufferDebugLabel( TBuffer< TTarget >* pBuffer, const char* label ) NN_NOEXCEPT;

/**
* @brief nn::gfx::Texture にデバッグラベルを追加します。
*
* @param[in] pTexture デバッグラベルを追加するテクスチャのポインタ
* @param[in] label デバッグラベルの文字列
*
*/
template< typename TTarget >
void SetTextureDebugLabel( TTexture< TTarget >* pTexture, const char* label ) NN_NOEXCEPT;

/**
* @brief nn::gfx::Sampler にデバッグラベルを追加します。
*
* @param[in] pSampler デバッグラベルを追加するサンプラーのポインタ
* @param[in] label デバッグラベルの文字列
*
*/
template< typename TTarget >
void SetSamplerDebugLabel( TSampler< TTarget >* pSampler, const char* label) NN_NOEXCEPT;

} // namespace util
} // namespace gfx
} // namespace nn
