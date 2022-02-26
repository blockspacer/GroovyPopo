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
* @brief サンプラー初期化用の情報に関する API の宣言
*/

#pragma once

#include <limits>

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_SamplerInfoData.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

#if !defined( NN_GFX_CONFIG_DISABLE_TINFO )

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_SamplerInfo-api.nvn.8.h>
#endif

#endif

namespace nn {
namespace gfx {

/**
* @brief サンプラーを初期化するための情報を表すクラスです。
*/
class SamplerInfo
    : public detail::DataContainer< SamplerInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    SamplerInfo() NN_NOEXCEPT
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
    * - SetFilterMode( nn::gfx::FilterMode_MinLinear_MagLinear_MipLinear );
    * - SetAddressU( nn::gfx::TextureAddressMode_ClampToEdge );
    * - SetAddressV( nn::gfx::TextureAddressMode_ClampToEdge );
    * - SetAddressW( nn::gfx::TextureAddressMode_ClampToEdge );
    * - SetComparisonFunction( nn::gfx::ComparisonFunction_Never );
    * - SetBorderColorType( nn::gfx::TextureBorderColorType_White );
    * - SetMaxAnisotropy( 1 );
    * - SetMinLod( -1000.0f );
    * - SetMaxLod( 1000.0f );
    * - SetLodBias( 0.0f );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief サンプリング時に適用されるフィルターモードを設定します。
    *
    * @param[in] value フィルターモード
    *
    * @post
    * - フィルターモードが設定されている
    */
    void SetFilterMode( FilterMode value ) NN_NOEXCEPT
    {
        this->filterMode = static_cast< Bit16 >( value );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の U 座標のアドレスモードを設定します。
    *
    * @param[in] value アドレスモード
    *
    * @post
    * - U 座標のアドレスモードが設定されている
    */
    void SetAddressU( TextureAddressMode value ) NN_NOEXCEPT
    {
        this->addressU = static_cast< Bit8 >( value );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の V 座標のアドレスモードを設定します。
    *
    * @param[in] value アドレスモード
    *
    * @post
    * - V 座標のアドレスモードが設定されている
    */
    void SetAddressV( TextureAddressMode value ) NN_NOEXCEPT
    {
        this->addressV = static_cast< Bit8 >( value );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の W 座標のアドレスモードを設定します。
    *
    * @param[in] value アドレスモード
    *
    * @post
    * - W 座標のアドレスモードが設定されている
    */
    void SetAddressW( TextureAddressMode value ) NN_NOEXCEPT
    {
        this->addressW = static_cast< Bit8 >( value );
    }

    /**
    * @brief フィルターモードが Comparison を含む場合に適用される比較関数を設定します。
    *
    * @param[in] value 比較関数
    *
    * @post
    * - 比較関数が設定されている
    */
    void SetComparisonFunction( ComparisonFunction value ) NN_NOEXCEPT
    {
        this->comparisonFunction = static_cast< Bit8 >( value );
    }

    /**
    * @brief アドレスモードが TextureAddressMode_ClampToBorder の際に適用されるボーダーカラータイプを設定します。
    *
    * @param[in] value ボーダーカラータイプ
    *
    * @post
    * - ボーダーカラータイプが設定されている
    */
    void SetBorderColorType( TextureBorderColorType value ) NN_NOEXCEPT
    {
        this->borderColorType = static_cast< Bit8 >( value );
    }

    /**
    * @brief フィルターモードが Anisotropy を含む際に適用される、異方性フィルタリングの最大サンプリング数を設定します。
    *
    * @param[in] value 異方性フィルタリングの最大サンプリング数
    *
    * @pre
    * - value >= 0 && value <= 255
    *
    * @post
    * - 異方性フィルタリングの最大サンプリング数が設定されている
    */
    void SetMaxAnisotropy( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint8_t >::max )() );
        this->maxAnisotropy = static_cast< uint8_t >( value );
    }

    /**
    * @brief 使用するミップマップ範囲の下限値を設定します。
    *
    * @param[in] value ミップマップ範囲の下限値
    *
    * @post
    * - ミップマップ範囲の下限値が設定されている
    */
    void SetMinLod( float value ) NN_NOEXCEPT
    {
        this->minLod = value;
    }

    /**
    * @brief 使用するミップマップ範囲の上限値を設定します。
    *
    * @param[in] value ミップマップ範囲の上限値
    *
    * @post
    * - ミップマップ範囲の上限値が設定されている
    */
    void SetMaxLod( float value ) NN_NOEXCEPT
    {
        this->maxLod = value;
    }

    /**
    * @brief 算出されるミップマップレベルに適用するオフセット値を設定します。
    *
    * @param[in] value ミップマップレベルへのオフセット値
    *
    * @post
    * - ミップマップレベルのオフセット値が設定されている
    */
    void SetLodBias( float value ) NN_NOEXCEPT
    {
        this->lodBias = value;
    }

    /**
    * @brief サンプリング時に適用されるフィルターモードを取得します。
    *
    * @return フィルターモードを返します。
    */
    FilterMode GetFilterMode() const NN_NOEXCEPT
    {
        return static_cast< FilterMode >( this->filterMode );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の U 座標のアドレスモードを取得します。
    *
    * @return U 座標のアドレスモードを返します。
    */
    TextureAddressMode GetAddressU() const NN_NOEXCEPT
    {
        return static_cast< TextureAddressMode >( this->addressU );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際のの V 座標アドレスモードを取得します。
    *
    * @return V 座標のアドレスモードを返します。
    */
    TextureAddressMode GetAddressV() const NN_NOEXCEPT
    {
        return static_cast< TextureAddressMode >( this->addressV );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際のの W 座標アドレスモードを取得します。
    *
    * @return W 座標のアドレスモードを返します。
    */
    TextureAddressMode GetAddressW() const NN_NOEXCEPT
    {
        return static_cast< TextureAddressMode >( this->addressW );
    }

    /**
    * @brief フィルターモードが Comparison を含む場合に適用される比較関数を取得します。
    *
    * @return 比較関数を返します。
    */
    ComparisonFunction GetComparisonFunction() const NN_NOEXCEPT
    {
        return static_cast< ComparisonFunction >( this->comparisonFunction );
    }

    /**
    * @brief アドレスモードが TextureAddressMode_ClampToBorder の際に適用されるボーダーカラータイプを取得します。
    *
    * @return ボーダーカラータイプを返します。
    */
    TextureBorderColorType GetBorderColorType() const NN_NOEXCEPT
    {
        return static_cast< TextureBorderColorType >( this->borderColorType );
    }

    /**
    * @brief フィルターモードが Anistoropy を含む際に適用される、異方性フィルタリングの最大サンプリング数を取得します。
    *
    * @return 異方性フィルタリングの最大サンプリング数を返します。
    */
    int GetMaxAnisotropy() const NN_NOEXCEPT
    {
        return this->maxAnisotropy;
    }

    /**
    * @brief 使用するミップマップ範囲の下限値を取得します。
    *
    * @return ミップマップ範囲の下限値を返します。
    */
    float GetMinLod() const NN_NOEXCEPT
    {
        return this->minLod;
    }

    /**
    * @brief 使用するミップマップ範囲の上限値を取得します。
    *
    * @return ミップマップ範囲の上限値を返します。
    */
    float GetMaxLod() const NN_NOEXCEPT
    {
        return this->maxLod;
    }

    /**
    * @brief 算出されるミップマップレベルに適用するオフセット値を取得します。
    *
    * @return ミップマップレベルへのオフセット値を返します。
    */
    float GetLodBias() const NN_NOEXCEPT
    {
        return this->lodBias;
    }
};

/**
* @brief サンプラーを初期化するための情報を表すグラフィックス抽象レイヤーです。
*/
template< typename TTarget >
class TSamplerInfo
    : public detail::TargetInfoImpl< detail::SamplerInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, SamplerInfo >::Type
{
    typedef typename detail::TargetInfoImpl< detail::SamplerInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, SamplerInfo >::Type Impl;

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
    static void ConvertFromInfo( TSamplerInfo< TTarget >* pDstInfo, const SamplerInfo& info ) NN_NOEXCEPT
    {
        return detail::InfoHelper::ConvertFromInfo< Impl >( pDstInfo, info );
    }

    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TSamplerInfo() NN_NOEXCEPT
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
    * - SetFilterMode( nn::gfx::FilterMode_MinLinear_MagLinear_MipLinear );
    * - SetAddressU( nn::gfx::TextureAddressMode_ClampToEdge );
    * - SetAddressV( nn::gfx::TextureAddressMode_ClampToEdge );
    * - SetAddressW( nn::gfx::TextureAddressMode_ClampToEdge );
    * - SetComparisonFunction( nn::gfx::ComparisonFunction_Never );
    * - SetBorderColorType( nn::gfx::TextureBorderColorType_White );
    * - SetMaxAnisotropy( 1 );
    * - SetMinLod( -1000.0f );
    * - SetMaxLod( 1000.0f );
    * - SetLodBias( 0.0f );
    */
    void SetDefault() NN_NOEXCEPT
    {
        return Impl::SetDefault();
    }

    /**
    * @brief サンプリング時に適用されるフィルターモードを設定します。
    *
    * @param[in] value フィルターモード
    *
    * @post
    * - フィルターモードが設定されている
    */
    void SetFilterMode( FilterMode value ) NN_NOEXCEPT
    {
        return Impl::SetFilterMode( value );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の U 座標のアドレスモードを設定します。
    *
    * @param[in] value アドレスモード
    *
    * @post
    * - U 座標のアドレスモードが設定されている
    */
    void SetAddressU( TextureAddressMode value ) NN_NOEXCEPT
    {
        return Impl::SetAddressU( value );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の V 座標のアドレスモードを設定します。
    *
    * @param[in] value アドレスモード
    *
    * @post
    * - V 座標のアドレスモードが設定されている
    */
    void SetAddressV( TextureAddressMode value ) NN_NOEXCEPT
    {
        return Impl::SetAddressV( value );
    }

    /**
    * @brief 0.0～1.0 の範囲外でサンプリングする際の W 座標のアドレスモードを設定します。
    *
    * @param[in] value アドレスモード
    *
    * @post
    * - W 座標のアドレスモードが設定されている
    */
    void SetAddressW( TextureAddressMode value ) NN_NOEXCEPT
    {
        return Impl::SetAddressW( value );
    }

    /**
    * @brief フィルターモードが Comparison を含む場合に適用される比較関数を設定します。
    *
    * @param[in] value 比較関数
    *
    * @post
    * - 比較関数が設定されている
    */
    void SetComparisonFunction( ComparisonFunction value ) NN_NOEXCEPT
    {
        return Impl::SetComparisonFunction( value );
    }

    /**
    * @brief アドレスモードが TextureAddressMode_ClampToBorder の際に適用されるボーダーカラータイプを設定します。
    *
    * @param[in] value ボーダーカラータイプ
    *
    * @post
    * - ボーダーカラータイプが設定されている
    */
    void SetBorderColorType( TextureBorderColorType value ) NN_NOEXCEPT
    {
        return Impl::SetBorderColorType( value );
    }

    /**
    * @brief フィルターモードが Anisotropy を含む際に適用される、異方性フィルタリングの最大サンプリング数を設定します。
    *
    * @param[in] value 異方性フィルタリングの最大サンプリング数
    *
    * @pre
    * - value >= 0 && value <= 255
    *
    * @post
    * - 異方性フィルタリングの最大サンプリング数が設定されている
    */
    void SetMaxAnisotropy( int value ) NN_NOEXCEPT
    {
        return Impl::SetMaxAnisotropy( value );
    }

    /**
    * @brief 使用するミップマップ範囲の下限値を設定します。
    *
    * @param[in] value ミップマップ範囲の下限値
    *
    * @post
    * - ミップマップ範囲の下限値が設定されている
    */
    void SetMinLod( float value ) NN_NOEXCEPT
    {
        return Impl::SetMinLod( value );
    }

    /**
    * @brief 使用するミップマップ範囲の上限値を設定します。
    *
    * @param[in] value ミップマップ範囲の上限値
    *
    * @post
    * - ミップマップ範囲の上限値が設定されている
    */
    void SetMaxLod( float value ) NN_NOEXCEPT
    {
        return Impl::SetMaxLod( value );
    }

    /**
    * @brief 算出されるミップマップレベルに適用するオフセット値を設定します。
    *
    * @param[in] value ミップマップレベルへのオフセット値
    *
    * @post
    * - ミップマップレベルのオフセット値が設定されている
    */
    void SetLodBias( float value ) NN_NOEXCEPT
    {
        return Impl::SetLodBias( value );
    }
};

}
}
