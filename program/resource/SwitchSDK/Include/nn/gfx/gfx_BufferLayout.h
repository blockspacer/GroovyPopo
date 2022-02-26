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
* @brief GPU により読み書きされるバッファーのレイアウトに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_TargetConfig.h>

#include <nn/gfx/detail/gfx_Declare.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_BufferLayout-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_BufferLayout-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_BufferLayout-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_BufferLayout-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_BufferLayout-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief タイムスタンプに使われるバッファーを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*
* @details
* このクラスは GPU が書き込む内容のレイアウトを表現します。
* 低レベルグラフィックス API のオブジェクトを表現しません。
*/
template< typename TTarget >
class TTimestampBuffer
    : public detail::TimestampBufferImpl< typename detail::TargetVariation< TTarget >::Type >
{
    typedef detail::TimestampBufferImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief タイムスタンプの値を取得します。
    *
    * @return タイムスタンプの値を返します。
    */
    int64_t GetValue() const NN_NOEXCEPT
    {
        return Impl::GetValue();
    }
};

/**
* @brief クエリーに使われるバッファーのレイアウトを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*
* @details
* このクラスは GPU が書き込む内容のレイアウトを表現します。
* 低レベルグラフィックス API のオブジェクトを表現しません。
*/
template< typename TTarget >
class TQueryBuffer
    : public detail::QueryBufferImpl< typename detail::TargetVariation< TTarget >::Type >
{
    typedef detail::QueryBufferImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief クエリー結果を取得します。
    *
    * @return クエリー結果を返します。
    */
    int64_t GetValue() const NN_NOEXCEPT
    {
        return Impl::GetValue();
    }
};

/**
* @brief 2 つのタイムスタンプ値の間の時間を取得します。
*
* @param[in] startTimestampValue 始点のタイムスタンプの値
* @param[in] endTimestampValue 終点のタイムスタンプの値
*
* @return 指定された始点と終点の間の時間を返します。
*
* @details
* タイムスタンプの値には nn::gfx::TimestampBuffer::GetValue で得られる値を使うことができます。
*/
inline nn::TimeSpan GetDuration( int64_t startTimestampValue, int64_t endTimestampValue ) NN_NOEXCEPT
{
    return detail::GetDuration< TargetConfig::Variation >( startTimestampValue, endTimestampValue );
}

}
}
