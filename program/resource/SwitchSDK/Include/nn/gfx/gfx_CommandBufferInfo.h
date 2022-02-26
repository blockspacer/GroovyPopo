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
* @brief コマンドバッファー初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_CommandBufferInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief コマンドバッファーを初期化するための情報を表すクラスです。
*/
class CommandBufferInfo
    : public detail::DataContainer< CommandBufferInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    CommandBufferInfo() NN_NOEXCEPT
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
    * - SetCommandBufferType( nn::gfx::CommandBufferType_Direct );
    * - SetQueueCapability( nn::gfx::QueueCapability_Graphics |
    *     nn::gfx::QueueCapability_Compute | nn::gfx::QueueCapability_Copy );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief コマンドバッファーの種類を設定します。
    *
    * @param[in] value コマンドバッファーの種類
    *
    * @post
    * - コマンドバッファーの種類が設定されている
    */
    void SetCommandBufferType( CommandBufferType value ) NN_NOEXCEPT
    {
        this->commandBufferType = static_cast< Bit8 >( value );
    }

    /**
    * @brief 提出先のキューの機能性を設定します。
    *
    * @param[in] value nn::gfx::QueueCapability の組み合わせによる提出先のキューの機能性
    *
    * @post
    * - 提出先のキューの機能性が設定されている
    */
    void SetQueueCapability( int value ) NN_NOEXCEPT
    {
        this->queueCapability = static_cast< Bit16 >( value );
    }

    /**
    * @brief コマンドバッファーの種類を取得します。
    *
    * @return コマンドバッファーの種類を返します。
    */
    CommandBufferType GetCommandBufferType() const NN_NOEXCEPT
    {
        return static_cast< CommandBufferType >( this->commandBufferType );
    }

    /**
    * @brief 提出先のキューの機能性を取得します。
    *
    * @return 提出先のキューの機能性を返します。
    */
    int GetQueueCapability() const NN_NOEXCEPT
    {
        return this->queueCapability;
    }
};

}
}
