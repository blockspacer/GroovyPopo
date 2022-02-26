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
* @brief キュー初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_QueueInfoData.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

#if !defined( NN_GFX_CONFIG_DISABLE_TINFO )

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_QueueInfo-api.nvn.8.h>
#endif

#endif

namespace nn {
namespace gfx {

/**
* @brief キューを初期化するための情報を表すクラスです。
*/
class QueueInfo
    : public detail::DataContainer< QueueInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    QueueInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 以下を呼ぶことに相当します。
    * - SetCapability( nn::gfx::QueueCapability_Graphics |
    *     nn::gfx::QueueCapability_Compute | nn::gfx::QueueCapability_Copy );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief キューの機能性を設定します。
    *
    * @param[in] value nn::gfx::QueueCapability の値の組み合わせによるキューの機能性
    *
    * @post
    * キューの機能性が設定されている
    */
    void SetCapability( int value ) NN_NOEXCEPT
    {
        this->capability = static_cast< Bit16 >( value );
    }

    /**
    * @brief キューの機能性を取得します。
    *
    * @return nn::gfx::QueueCapability の値の組み合わせによるキューの機能性を返します。
    */
    int GetCapability() const NN_NOEXCEPT
    {
        return this->capability;
    }
};

/**
* @brief キューを初期化するための情報を表すグラフィックス抽象レイヤーです。
*/
template< typename TTarget >
class TQueueInfo
    : public detail::TargetInfoImpl< detail::QueueInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, QueueInfo >::Type
{
    typedef typename detail::TargetInfoImpl< detail::QueueInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, QueueInfo >::Type Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename detail::TargetVariation< TTarget >::Type Target;

    /**
    * @brief 共通版の情報から特殊版の情報に変換します。
    *
    * @param[in] pDstInfo 変換先の特殊版の情報へのポインタ
    * @param[in] info もとになる共通版の情報
    *
    * @pre
    * - pDstInfo != NULL
    */
    static void ConvertFromInfo( TQueueInfo< TTarget >* pDstInfo, const QueueInfo& info ) NN_NOEXCEPT
    {
        return detail::InfoHelper::ConvertFromInfo< Impl >( pDstInfo, info );
    }

    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TQueueInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 以下を呼ぶことに相当します。
    * - SetCapability( nn::gfx::QueueCapability_Graphics |
    *     nn::gfx::QueueCapability_Compute | nn::gfx::QueueCapability_Copy );
    */
    void SetDefault() NN_NOEXCEPT
    {
        return Impl::SetDefault();
    }

    /**
    * @brief キューの機能性を設定します。
    *
    * @param[in] value nn::gfx::QueueCapability の値の組み合わせによるキューの機能性
    *
    * @post
    * キューの機能性が設定されている
    */
    void SetCapability( int value ) NN_NOEXCEPT
    {
        return Impl::SetCapability( value );
    }
};

}
}
