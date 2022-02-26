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
* @brief シェーダー初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_ShaderInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief シェーダーを初期化するための情報を表すクラスです。
*/
class ShaderInfo
    : public detail::DataContainer< ShaderInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ShaderInfo() NN_NOEXCEPT
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
    * - SetSeparationEnabled( false );
    * - SetCodeType( nn::gfx::ShaderCodeType_Source );
    * - SetSourceFormat( nn::gfx::ShaderSourceFormat_Glsl );
    * - SetBinaryFormat( 0 );
    * - SetShaderCodePtr( nn::gfx::ShaderStage_Vertex, NULL );
    * - SetShaderCodePtr( nn::gfx::ShaderStage_Hull, NULL );
    * - SetShaderCodePtr( nn::gfx::ShaderStage_Domain, NULL );
    * - SetShaderCodePtr( nn::gfx::ShaderStage_Geometry, NULL );
    * - SetShaderCodePtr( nn::gfx::ShaderStage_Pixel, NULL );
    * - SetShaderCodePtr( nn::gfx::ShaderStage_Compute, NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief シェーダーのセパレートを有効または無効に設定します。
    *
    * @param[in] value シェーダーのセパレートの有効・無効を表す真偽値
    *
    * @post
    * - シェーダーのセパレートの有効・無効が設定されている
    */
    void SetSeparationEnabled( bool value ) NN_NOEXCEPT
    {
        this->flags.SetBit( Flag_SeparationEnable, value );
    }

    /**
    * @brief SetCodePtr で設定するコードのタイプを設定します。
    *
    * @param[in] value コードタイプ
    *
    * @post
    * - コードタイプが設定されている
    *
    * @details
    * nn::gfx::ShaderCodeType_Source や nn::gfx::ShaderCodeType_SourceArray は、速度やメモリー効率の面において推奨されません。
    */
    void SetCodeType( ShaderCodeType value ) NN_NOEXCEPT
    {
        this->codeType = static_cast< Bit8 >( value );
    }

    /**
    * @brief コードタイプがソースである場合のソースフォーマットを設定します。
    *
    * @param[in] value ソースフォーマット
    *
    * @post
    * - ソースフォーマットが設定されている。
    */
    void SetSourceFormat( ShaderSourceFormat value ) NN_NOEXCEPT
    {
        this->sourceFormat = static_cast< uint8_t >( value );
    }

    /**
    * @brief コードタイプがバイナリーである場合のバイナリフォーマットを設定します。
    *
    * @param[in] value バイナリーフォーマット
    *
    * @post
    * - バイナリーフォーマットが設定されている
    *
    * @details
    * GL 版ではプログラムバイナリー取得時に得られる binaryFormat を設定します。
    *
    * @platformbegin{GX}
    * このパラメータはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetBinaryFormat( int value ) NN_NOEXCEPT
    {
        this->binaryFormat = static_cast< Bit32 >( value );
    }

    /**
    * @brief シェーダーコードを設定します。
    *
    * @param[in] shaderStage 対象のシェーダーステージ
    * @param[in] pCode シェーダーコードへのポインター
    *
    * @post
    * - シェーダーコードが設定されている
    *
    * @details
    * pCode の指すべき内容は nn::gfx::ShaderCode または nn::gfx::SourceArrayCode または低レベルグラフィックス API 特有の構造体です。
    */
    void SetShaderCodePtr( ShaderStage shaderStage, const void* pCode ) NN_NOEXCEPT;

    /**
    * @brief シェーダーのセパレートが有効かどうかを取得します。
    *
    * @return シェーダーのセパレートが有効かどうかを表す真偽値を返します。
    */
    bool IsSeparationEnabled() const NN_NOEXCEPT
    {
        return this->flags.GetBit( Flag_SeparationEnable );
    }

    /**
    * @brief コードタイプを取得します。
    *
    * @return
    * - コードタイプを返します。
    */
    ShaderCodeType GetCodeType() const NN_NOEXCEPT
    {
        return static_cast< ShaderCodeType >( this->codeType );
    }

    /**
    * @brief バイナリーフォーマットを取得します。
    *
    * @return バイナリーフォーマットを返します。
    */
    int GetBinaryFormat() const NN_NOEXCEPT
    {
        return static_cast< int >( this->binaryFormat );
    }

    /**
    * @brief ソースフォーマットを取得します。
    *
    * @return ソースフォーマットを返します。
    */
    ShaderSourceFormat GetSourceFormat() const NN_NOEXCEPT
    {
        return static_cast< ShaderSourceFormat >( this->sourceFormat );
    }

    /**
    * @brief シェーダーコードへのポインターを取得します。
    *
    * @return シェーダーコードへのポインターを返します。
    */
    const void* GetShaderCodePtr( ShaderStage shaderStage ) const NN_NOEXCEPT;
};

}
}
