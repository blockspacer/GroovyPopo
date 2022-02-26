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
* @brief スワップチェーン初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_SwapChainInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief スワップチェーンを初期化するための情報を表すクラスです。
*/
class SwapChainInfo
    : public detail::DataContainer< SwapChainInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    SwapChainInfo() NN_NOEXCEPT
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
    * - SetBufferCount( 2 );
    * - SetFormat( nn::gfx::ImageFormat_R8_G8_B8_A8_Unorm );
    * - SetWidth( 1280 );
    * - SetHeight( 720 );
    * - SetLayer( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief レイヤーを設定します。
    *
    * @param[in] value レイヤーへのポインター
    *
    * @post
    * - レイヤーが設定されている
    *
    * @see
    * nn::vi::Layer
    */
    void SetLayer( nn::vi::Layer* value ) NN_NOEXCEPT
    {
        this->pLayer = value;
    }

    /**
    * @brief 描画フォーマットを設定します。
    *
    * @param[in] value 描画フォーマット
    *
    * @post
    * - 描画フォーマットが設定されている
    */
    void SetFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->format = static_cast< Bit32 >( value );
    }

    /**
    * @brief スキャンバッファーの数を設定します。
    *
    * @param[in] value スキャンバッファーの数
    *
    * @post
    * - スキャンバッファーの数が設定されている
    */
    void SetBufferCount( int value ) NN_NOEXCEPT
    {
        this->bufferCount = static_cast< uint8_t >( value );
    }

    /**
    * @brief スキャンバッファーの幅を設定します。
    *
    * @param[in] value スキャンバッファーの幅
    *
    * @post
    * - スキャンバッファーの幅が設定されている
    */
    void SetWidth( int value ) NN_NOEXCEPT
    {
        this->width = static_cast< uint32_t >( value );
    }

    /**
    * @brief スキャンバッファーの高さを設定します。
    *
    * @param[in] value スキャンバッファーの高さ
    *
    * @post
    * - スキャンバッファーの高さが設定されている
    */
    void SetHeight( int value ) NN_NOEXCEPT
    {
        this->height = static_cast< uint32_t >( value );
    }

    /**
    * @brief レイヤーを取得します。
    *
    * @return レイヤーへのポインターを返します。
    *
    * @see
    * nn::vi::Layer
    */
    nn::vi::Layer* GetLayer() const NN_NOEXCEPT
    {
        return this->pLayer;
    }

    /**
    * @brief 描画フォーマットを取得します。
    *
    * @return 描画フォーマットを返します。
    */
    ImageFormat GetFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->format );
    }

    /**
    * @brief スキャンバッファーの数を取得します。
    *
    * @return スキャンバッファーの数を返します。
    */
    int GetBufferCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->bufferCount );
    }

    /**
    * @brief スキャンバッファーの幅を取得します。
    *
    * @return スキャンバッファーの幅を返します。
    */
    int GetWidth() const NN_NOEXCEPT
    {
        return static_cast< int >( this->width );
    }

    /**
    * @brief スキャンバッファーの高さを取得します。
    *
    * @return スキャンバッファーの高さを返します。
    */
    int GetHeight() const NN_NOEXCEPT
    {
        return static_cast< int >( this->height );
    }
};

}
}
