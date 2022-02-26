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
* @brief デスクリプタープール初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DescriptorPoolInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief デスクリプタープールを初期化するための情報を表すクラスです。
*/
class DescriptorPoolInfo
    : public detail::DataContainer< DescriptorPoolInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DescriptorPoolInfo() NN_NOEXCEPT
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
    * - SetDescriptorPoolType( nn::gfx::DescriptorPoolType_BufferView );
    * - SetSlotCount( 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief デスクリプタープールの種類を設定します。
    *
    * @param[in] value デスクリプタープールの種類
    *
    * @post
    * - デスクリプタープールの種類が設定されている
    */
    void SetDescriptorPoolType( DescriptorPoolType value ) NN_NOEXCEPT
    {
        this->descriptorPoolType = static_cast< Bit8 >( value );
    }

    /**
    * @brief デスクリプタープールのスロット数を設定します。
    *
    * @param[in] value デスクリプタープールのスロット数
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - デスクリプタープールのスロット数が設定されている
    */
    void SetSlotCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value >= 0 );
        this->slotCount = static_cast< uint32_t >( value );
    }

    /**
    * @brief デスクリプタープールの種類を取得します。
    *
    * @return デスクリプタープールの種類を返します。
    */
    DescriptorPoolType GetDescriptorPoolType() const NN_NOEXCEPT
    {
        return static_cast< DescriptorPoolType >( this->descriptorPoolType );
    }

    /**
    * @brief デスクリプタープールのスロット数を取得します。
    *
    * @return デスクリプタープールのスロット数を返します。
    */
    int GetSlotCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->slotCount );
    }
};

}
}
