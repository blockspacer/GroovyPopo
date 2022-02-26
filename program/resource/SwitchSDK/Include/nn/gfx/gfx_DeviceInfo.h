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
* @brief デバイス初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DeviceInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief デバイスを初期化するための情報を表すクラスです。
*/
class DeviceInfo
    : public detail::DataContainer< DeviceInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DeviceInfo() NN_NOEXCEPT
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
    * - SetDebugMode( nn::gfx::DebugMode_Disable );
    * - SetTileCacheMode( nn::gfx::TileCacheMode_Disable );
    * - SetApiVersion( 0, 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief デバッグモードを設定します。
    *
    * @param[in] value デバッグモード
    *
    * @post
    * - デバッグモードが設定されている
    */
    void SetDebugMode( DebugMode value ) NN_NOEXCEPT
    {
        this->debugMode = static_cast< Bit8 >( value );
    }

    /**
    * @brief API バージョンを設定します。
    *
    * @param[in] majorVersion API のメジャーバージョン
    * @param[in] minorVersion API のマイナーバージョン
    *
    * @post
    * - API バージョンが設定されている
    */
    void SetApiVersion( int majorVersion, int minorVersion ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( majorVersion >= 0 );
        NN_SDK_REQUIRES( minorVersion >= 0 );
        this->apiMajorVersion = static_cast< uint16_t >( majorVersion );
        this->apiMinorVersion = static_cast< uint16_t >( minorVersion );
    }

    /**
    * @brief デバッグモードを取得します。
    *
    * @return デバッグモードを返します。
    */
    DebugMode GetDebugMode() const NN_NOEXCEPT
    {
        return static_cast< DebugMode >( this->debugMode );
    }

    /**
    * @brief API メジャーバージョンを取得します。
    *
    * @return API メジャーバージョンを返します。
    */
    int GetApiMajorVersion() const NN_NOEXCEPT
    {
        return static_cast< int >( this->apiMajorVersion );
    }

    /**
    * @brief API マイナーバージョンを取得します。
    *
    * @return API マイナーバージョンを返します。
    */
    int GetApiMinorVersion() const NN_NOEXCEPT
    {
        return static_cast< int >( this->apiMinorVersion );
    }
};

}
}
