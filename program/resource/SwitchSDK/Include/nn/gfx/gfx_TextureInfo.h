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
* @brief テクスチャー初期化用の情報に関する API の宣言
*/

#pragma once

#include <limits>

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DataAccessorConverter.h>
#include <nn/gfx/gfx_TextureInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

#if !defined( NN_GFX_CONFIG_DISABLE_TINFO )

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_TextureInfo-api.nvn.8.h>
#endif

#endif

namespace nn {
namespace gfx {

class TextureMipRange;
class TextureArrayRange;
class TextureSubresourceRange;

/**
* @brief テクスチャーを初期化するための情報を表すクラスです。
*/
class TextureInfo
    : public detail::DataContainer< TextureInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureInfo() NN_NOEXCEPT
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
    * - SetImageStorageDimension( nn::gfx::ImageStorageDimension_2d );
    * - SetImageFormat( nn::gfx::ImageFormat_Undefined );
    * - SetGpuAccessFlags( 0 );
    * - SetTileMode( nn::gfx::TileMode_Optimal );
    * - SetWidth( 1 );
    * - SetHeight( 1 );
    * - SetDepth( 1 );
    * - SetMipCount( 1 );
    * - SetArrayLength( 0 );
    * - SetSwizzle( 0 );
    * - SetMultiSampleCount( 1 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief イメージデータの次元を設定します。
    *
    * @param[in] value イメージデータの次元
    *
    * @post
    * - イメージデータの次元が設定されている
    */
    void SetImageStorageDimension( ImageStorageDimension value ) NN_NOEXCEPT
    {
        this->imageStorageDimension = static_cast< Bit8 >( value );
    }

    /**
    * @brief イメージフォーマットを設定します。
    *
    * @param[in] value イメージフォーマット
    *
    * @post
    * - イメージフォーマットが設定されている
    */
    void SetImageFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->imageFormat = static_cast< Bit32 >( value );
    }

    /**
    * @brief GPU がアクセスする方法を設定します。
    *
    * @param[in] value nn::gfx::GpuAccess の値の組み合わせによる GPU がアクセスする方法
    *
    * @post
    * - GPU がアクセスする方法が設定されている
    */
    void SetGpuAccessFlags( int value ) NN_NOEXCEPT
    {
        this->gpuAccessFlags = static_cast< Bit32 >( value );
    }

    /**
    * @brief タイリングモードを設定します。
    *
    * @param[in] value タイリングモード
    *
    * @post
    * - タイリングモードが設定されている
    */
    void SetTileMode( TileMode value ) NN_NOEXCEPT
    {
        this->tileMode = static_cast< Bit16 >( value );
    }

    /**
    * @brief ベースレベルのイメージの幅を設定します。
    *
    * @param[in] value ベースレベルのイメージの幅
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ベースレベルのイメージの幅が設定されている
    */
    void SetWidth( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->width = static_cast< uint32_t >( value );
    }

    /**
    * @brief ベースレベルのイメージの高さを設定します。
    *
    * @param[in] value ベースレベルのイメージの高さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ベースレベルのイメージの高さが設定されている
    */
    void SetHeight( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->height = static_cast< uint32_t >( value );
    }

    /**
    * @brief ベースレベルのイメージの深さを設定します。
    *
    * @param[in] value ベースレベルのイメージの深さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ベースレベルのイメージの深さが設定されている
    */
    void SetDepth( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->depth = static_cast< uint32_t >( value );
    }

    /**
    * @brief 配列の長さを設定します。
    *
    * @param[in] value 配列の長さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 配列の長さが設定されている
    */
    void SetArrayLength( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->arrayLength = static_cast< uint32_t >( value );
    }

    /**
    * @brief スウィズリングのベース値を設定します。
    *
    * @param[in] value スウィズリングのベース値
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - スウィズリングのベース値が設定されている
    */
    void SetSwizzle( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->swizzle = static_cast< uint16_t >( value );
    }

    /**
    * @brief マルチサンプリングの数を設定します。
    *
    * @param[in] value マルチサンプリングの数
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - マルチサンプリングの数が設定されている
    */
    void SetMultiSampleCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->multisampleCount = static_cast< uint16_t >( value );
    }

    /**
    * @brief ミップマップの数を設定します。
    *
    * @param[in] value ベースレベルを含むミップマップの数
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ミップマップの数が設定されている
    */
    void SetMipCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->mipCount = static_cast< uint16_t >( value );
    }

    /**
    * @brief イメージデータの次元を取得します。
    *
    * @return イメージデータの次元を返します。
    */
    ImageStorageDimension GetImageStorageDimension() const NN_NOEXCEPT
    {
        return static_cast< ImageStorageDimension >( this->imageStorageDimension );
    }

    /**
    * @brief イメージフォーマットを取得します。
    *
    * @return イメージフォーマットを返します。
    */
    ImageFormat GetImageFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->imageFormat );
    }

    /**
    * @brief GPU がアクセスする方法を取得します。
    *
    * @return GPU がアクセスする方法を返します。
    */
    int GetGpuAccessFlags() const NN_NOEXCEPT
    {
        return static_cast< int >( this->gpuAccessFlags );
    }

    /**
    * @brief タイリングモードを取得します。
    *
    * @return タイリングモードを返します。
    */
    TileMode GetTileMode() const NN_NOEXCEPT
    {
        return static_cast< TileMode >( this->tileMode );
    }

    /**
    * @brief ベースレベルのイメージの幅を取得します。
    *
    * @return ベースレベルのイメージの幅を返します。
    */
    int GetWidth() const NN_NOEXCEPT
    {
        return static_cast< int >( this->width );
    }

    /**
    * @brief ベースレベルのイメージの高さを取得します。
    *
    * @return ベースレベルのイメージの高さを返します。
    */
    int GetHeight() const NN_NOEXCEPT
    {
        return static_cast< int >( this->height );
    }

    /**
    * @brief ベースレベルのイメージの深さを取得します。
    *
    * @return ベースレベルのイメージの深さを返します。
    */
    int GetDepth() const NN_NOEXCEPT
    {
        return static_cast< int >( this->depth );
    }

    /**
    * @brief ミップマップの数を取得します。
    *
    * @return ミップマップの数を返します。
    */
    int GetMipCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->mipCount );
    }

    /**
    * @brief 配列の長さを取得します。
    *
    * @return 配列の長さを返します。
    */
    int GetArrayLength() const NN_NOEXCEPT
    {
        return static_cast< int >( this->arrayLength );
    }

    /**
    * @brief スウィズリングのベース値を取得します。
    *
    * @return スウィズリングのベース値を返します。
    */
    int GetSwizzle() const NN_NOEXCEPT
    {
        return static_cast< int >( this->swizzle );
    }

    /**
    * @brief マルチサンプリングの数を取得します。
    *
    * @return マルチサンプリングの数を返します。
    */
    int GetMultisampleCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->multisampleCount );
    }
};

/**
* @brief テクスチャービューを初期化するための情報を表すクラスです。
*/
class TextureViewInfo
    : public detail::DataContainer< TextureViewInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureViewInfo() NN_NOEXCEPT
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
    * - SetImageDimension( nn::gfx::ImageDimension_2d );
    * - SetDepthStencilTextureMode( nn::gfx::DepthStencilFetchMode_DepthComponent );
    * - SetImageFormat( nn::gfx::ImageFormat_Undefined );
    * - EditSubresourceRange().SetDefault();
    * - SetChannelMapping( nn::gfx::ChannelMapping_Red, nn::gfx::ChannelMapping_Green,
    *     nn::gfx::ChannelMapping_Blue, nn::gfx::ChannelMapping_Alpha );
    * - SetTexturePtr( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief イメージの次元を設定します。
    *
    * @param[in] value イメージの次元
    *
    * @post
    * - イメージの次元が設定されている
    *
    * @details
    * 設定する値は SetTexturePtr で設定するテクスチャーの次元と互換性がある必要があります。
    */
    void SetImageDimension( ImageDimension value ) NN_NOEXCEPT
    {
        this->imageDimension = static_cast< Bit8 >( value );
    }

    /**
    * @brief 深度ステンシルテクスチャーのフェッチモードを設定します。
    *
    * @param[in] value 深度ステンシルテクスチャーのフェッチモード
    *
    * @post
    * - 深度ステンシルテクスチャーのフェッチモードが設定されている
    *
    * @details
    * 深度ステンシルテクスチャーの場合のみ設定してください。SetChannelMapping とは排他的な設定です。
    */
    void SetDepthStencilTextureMode( DepthStencilFetchMode value ) NN_NOEXCEPT
    {
        this->depthStencilTextureMode = static_cast< Bit8 >( value );
    }

    /**
    * @brief イメージフォーマットを設定します。
    *
    * @param[in] value イメージフォーマット
    *
    * @post
    * - イメージフォーマットが設定されている
    *
    * @details
    * 設定する値は SetTexturePtr で設定するテクスチャーのフォーマットと互換性がある必要があります。
    */
    void SetImageFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->imageFormat = static_cast< Bit32 >( value );
    }

    /**
    * @brief 参照するテクスチャーを設定します。
    *
    * @tparam TTarget テクスチャーの低レベルグラフィックス API
    *
    * @param[in] value 参照するテクスチャーへのポインター
    *
    * @post
    * - 参照するテクスチャーが設定されている
    *
    * @details
    * TTarget は初期化するつもりのテクスチャービューの TTarget と同じである必要があります。
    */
    template< typename TTarget >
    void SetTexturePtr( const TTexture< TTarget >* value ) NN_NOEXCEPT
    {
        this->pTexture = value;
    }

    /**
    * @brief 参照するテクスチャーを設定します。
    *
    * @param[in] value 参照するテクスチャーへのポインター
    *
    * @post
    * - 参照するテクスチャーが設定されている
    *
    * @details
    * NULL 以外の値を設定する場合はテンプレート引数付きのほうを呼んでください
    */
    void SetTexturePtr( const void* value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value == NULL );
        this->pTexture = value;
    }

    /**
    * @brief RGBA 要素のマッピングを設定します。
    *
    * @param[in] red 赤要素のマッピング
    * @param[in] green 緑要素のマッピング
    * @param[in] blue 青要素のマッピング
    * @param[in] alpha アルファ要素のマッピング
    *
    * @post
    * - RGBA 要素のマッピングが設定されている
    *
    * @details
    * カラーテクスチャーの場合のみ設定してください。SetDepthStencilTextureMode とは排他的な設定です。
    */
    void SetChannelMapping( ChannelMapping red, ChannelMapping green,
        ChannelMapping blue, ChannelMapping alpha ) NN_NOEXCEPT
    {
        this->channelMapping[ 0 ] = static_cast< Bit8 >( red );
        this->channelMapping[ 1 ] = static_cast< Bit8 >( green );
        this->channelMapping[ 2 ] = static_cast< Bit8 >( blue );
        this->channelMapping[ 3 ] = static_cast< Bit8 >( alpha );
    }

    /**
    * @brief 参照するイメージ範囲を編集します。
    *
    * @return 参照するイメージ範囲への参照を返します。
    *
    * @details
    * 編集する範囲は、SetTexturePtr で設定するテクスチャーのイメージ範囲内である必要があります。
    */
    TextureSubresourceRange& EditSubresourceRange() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->subresourceRange );
    }

    /**
    * @brief イメージの次元を取得します。
    *
    * @return イメージの次元を返します。
    */
    ImageDimension GetImageDimension() const NN_NOEXCEPT
    {
        return static_cast< ImageDimension >( this->imageDimension );
    }

    /**
    * @brief 深度ステンシルテクスチャーのフェッチモードを取得します。
    *
    * @return 深度ステンシルテクスチャーのフェッチモードを返します。
    */
    DepthStencilFetchMode GetDepthStencilTextureMode() const NN_NOEXCEPT
    {
        return static_cast< DepthStencilFetchMode >( this->depthStencilTextureMode );
    }

    /**
    * @brief イメージフォーマットを取得します。
    *
    * @return イメージフォーマットを返します。
    */
    ImageFormat GetImageFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->imageFormat );
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief 参照するテクスチャーを取得します。
    *
    * @return nn::gfx::Texture 型の参照するテクスチャーへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Texture 型のポインターに代入してください
    */
    const Texture* GetTexturePtr() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetTexturePtr() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pTexture.ptr );
    }
#endif

    /**
    * @brief 要素のマッピングを取得します。
    *
    * @param[in] channel 取得するマッピングの色要素
    *
    * @return 要素のマッピングを返します。
    */
    ChannelMapping GetChannelMapping( ColorChannel channel ) const NN_NOEXCEPT
    {
        return static_cast< ChannelMapping >( this->channelMapping[ channel ] );
    }

    /**
    * @brief 参照するイメージ範囲を取得します。
    *
    * @return 参照するイメージ範囲を返します。
    */
    const TextureSubresourceRange& GetSubresourceRange() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->subresourceRange );
    }
};

/**
* @brief カラーターゲットビューを初期化するための情報を表すクラスです。
*/
class ColorTargetViewInfo
    : public detail::DataContainer< ColorTargetViewInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ColorTargetViewInfo() NN_NOEXCEPT
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
    * - SetImageDimension( nn::gfx::ImageDimension_2D );
    * - SetImageFormat( nn::gfx::ImageFormat_Undefined );
    * - SetMipLevel( 0 );
    * - EditArrayRange().SetDefault();
    * - SetTexturePtr( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief イメージの次元を設定します。
    *
    * @param[in] value イメージの次元
    *
    * @post
    * - イメージの次元が設定されている
    *
    * @details
    * 設定する次元は、SetTexturePtr で設定するテクスチャーの次元と互換性がある必要があります。
    */
    void SetImageDimension( ImageDimension value ) NN_NOEXCEPT
    {
        this->imageDimension = static_cast< Bit8 >( value );
    }

    /**
    * @brief イメージフォーマットを設定します。
    *
    * @param[in] value イメージフォーマット
    *
    * @post
    * - イメージフォーマットが設定されている
    *
    * @details
    * 設定するフォーマットは、SetTexturePtr で設定するテクスチャーのフォーマットと互換性がある必要があります。
    */
    void SetImageFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->imageFormat = static_cast< Bit32 >( value );
    }

    /**
    * @brief 参照する単一のミップマップレベルを設定します。
    *
    * @param[in] value 参照するミップマップレベル
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - 参照する単一のミップマップレベルが設定されている
    *
    * @details
    * 設定する値は、SetTexturePtr で設定するテクスチャーのミップマップ範囲内である必要があります。
    */
    void SetMipLevel( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->mipLevel = static_cast< uint16_t >( value );
    }

    /**
    * @brief 参照するテクスチャーを設定します。
    *
    * @tparam TTarget テクスチャーの低レベルグラフィックス API
    *
    * @param[in] value 参照するテクスチャーへのポインター
    *
    * @post
    * - 参照するテクスチャーが設定されている
    *
    * @details
    * 参照するテクスチャーのフォーマットはカラーフォーマットである必要があります。
    * TTarget は初期化するつもりのテクスチャービューの TTarget と同じである必要があります。
    */
    template< typename TTarget >
    void SetTexturePtr( const TTexture< TTarget >* value ) NN_NOEXCEPT
    {
        this->pTexture = value;
    }

    /**
    * @brief 参照するテクスチャーを設定します。
    *
    * @param[in] value 参照するテクスチャーへのポインター
    *
    * @post
    * - 参照するテクスチャーが設定されている
    *
    * @details
    * NULL 以外の値を設定する場合はテンプレート引数付きのほうを呼んでください
    */
    void SetTexturePtr( const void* value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value == NULL );
        this->pTexture = value;
    }

    /**
    * @brief 参照する配列範囲を編集します。
    *
    * @return 参照する配列範囲への参照を返します。
    *
    * @details
    * 編集する内容は、SetTexturePtr で設定するテクスチャーの配列の範囲内である必要があります。
    */
    TextureArrayRange& EditArrayRange() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->arrayRange );
    }

    /**
    * @brief イメージの次元を取得します。
    *
    * @return イメージの次元を返します。
    */
    ImageDimension GetImageDimension() const NN_NOEXCEPT
    {
        return static_cast< ImageDimension >( this->imageDimension );
    }

    /**
    * @brief イメージフォーマットを取得します。
    *
    * @return イメージフォーマットを返します。
    */
    ImageFormat GetImageFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->imageFormat );
    }

    /**
    * @brief 参照するミップマップレベルを取得します。
    *
    * @return 参照するミップマップレベルを返します。
    */
    int GetMipLevel() const NN_NOEXCEPT
    {
        return static_cast< int >( this->mipLevel );
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief 参照するテクスチャーを取得します。
    *
    * @return nn::gfx::Texture 型の参照するテクスチャーへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Texture 型のポインターに代入してください
    */
    const Texture* GetTexturePtr() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetTexturePtr() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pTexture.ptr );
    }
#endif

    /**
    * @brief 参照する配列範囲を取得します。
    *
    * @return 参照するテクチャー範囲への参照を返します。
    */
    const TextureArrayRange& GetArrayRange() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->arrayRange );
    }
};

/**
* @brief 深度ステンシルビューを初期化するための情報を表すクラスです。
*/
class DepthStencilViewInfo
    : public detail::DataContainer< DepthStencilViewInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DepthStencilViewInfo() NN_NOEXCEPT
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
    * - SetImageDimension( nn::gfx::ImageDimension_2d );
    * - SetMipLevel( 0 );
    * - EditArrayRange().SetDefault();
    * - SetTexturePtr( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief イメージの次元を設定します。
    *
    * @param[in] value イメージの次元
    *
    * @post
    * - イメージの次元が設定されている
    *
    * @details
    * 設定するイメージの次元は、SetTexturePtr で設定するテクスチャーの次元と互換性がある必要があります。
    */
    void SetImageDimension( ImageDimension value ) NN_NOEXCEPT
    {
        this->imageDimension = static_cast< Bit8 >( value );
    }

    /**
    * @brief 参照する単一のミップマップレベルを設定します。
    *
    * @param[in] value 参照するミップマップレベル
    *
    * @post
    * - 参照する単一のミップマップレベルが設定されている
    */
    void SetMipLevel( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->mipLevel = static_cast< uint16_t >( value );
    }

    /**
    * @brief 参照するテクスチャーを設定します。
    *
    * @tparam TTarget テクスチャーの低レベルグラフィックス API
    *
    * @param[in] value 参照するテクスチャーへのポインター
    *
    * @post
    * - 参照するテクスチャーが設定されている
    *
    * @details
    * 参照するテクスチャーのフォーマットは深度ステンシルフォーマットである必要があります。
    * TTarget は初期化するつもりのテクスチャービューの TTarget と同じである必要があります。
    */
    template< typename TTarget >
    void SetTexturePtr( const TTexture< TTarget >* value ) NN_NOEXCEPT
    {
        this->pTexture = value;
    }

    /**
    * @brief 参照するテクスチャーを設定します。
    *
    * @param[in] value 参照するテクスチャーへのポインター
    *
    * @post
    * - 参照するテクスチャーが設定されている
    *
    * @details
    * NULL 以外の値を設定する場合はテンプレート引数付きのほうを呼んでください
    */
    void SetTexturePtr( const void* value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value == NULL );
        this->pTexture = value;
    }

    /**
    * @brief 参照する配列範囲を編集します。
    *
    * @return 参照する配列範囲への参照を返します。
    */
    TextureArrayRange& EditArrayRange() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->arrayRange );
    }

    /**
    * @brief イメージの次元を取得します。
    *
    * @return イメージの次元を返します。
    */
    ImageDimension GetImageDimension() const NN_NOEXCEPT
    {
        return static_cast< ImageDimension >( this->imageDimension );
    }

    /**
    * @brief 参照する単一のミップマップレベルを取得します。
    *
    * @return 参照する単一のミップマップレベルを返します。
    */
    int GetMipLevel() const NN_NOEXCEPT
    {
        return static_cast< int >( this->mipLevel );
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief 参照するテクスチャーを取得します。
    *
    * @return nn::gfx::Texture 型の参照するテクスチャーへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Texture 型のポインターに代入してください
    */
    const Texture* GetTexturePtr() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetTexturePtr() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pTexture.ptr );
    }
#endif

    /**
    * @brief 参照する配列範囲を取得します。
    *
    * @return 参照する配列範囲を返します。
    */
    const TextureArrayRange& GetArrayRange() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->arrayRange );
    }
};

/**
* @brief テクスチャーのミップマップ範囲を表すクラスです。
*/
class TextureMipRange
    : public detail::DataContainer< TextureMipRangeData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureMipRange() NN_NOEXCEPT
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
    * - SetMinMipLevel( 0 );
    * - SetMipCount( 1 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 最小のミップマップレベルを設定します。
    *
    * @param[in] value 最小のミップマップレベル
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - 最小のミップマップレベルが設定されている
    */
    void SetMinMipLevel( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->minMipLevel = static_cast< uint16_t >( value );
    }

    /**
    * @brief ミップマップの数を設定します。
    *
    * @param[in] value ミップマップの数
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - ミップマップの数が設定されている
    */
    void SetMipCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->mipCount = static_cast< uint16_t >( value );
    }

    /**
    * @brief 最小のミップマップレベルを取得します。
    *
    * @return 最小のミップマップを返します。
    */
    int GetMinMipLevel() const NN_NOEXCEPT
    {
        return static_cast< int >( this->minMipLevel );
    }

    /**
    * @brief ミップマップの数を取得します。
    *
    * @return ミップマップの数を返します。
    */
    int GetMipCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->mipCount );
    }
};

/**
* @brief テクスチャーの配列範囲を表すクラスです。
*/
class TextureArrayRange
    : public detail::DataContainer< TextureArrayRangeData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureArrayRange() NN_NOEXCEPT
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
    * - SetBaseArrayIndex( 0 );
    * - SetArrayLength( 1 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 配列インデックスのベース値を設定します。
    *
    * @param[in] value 配列インデックスのベース値
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 配列インデックスのベース値が設定されている
    */
    void SetBaseArrayIndex( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->baseArrayIndex = static_cast< uint32_t >( value );
    }

    /**
    * @brief 配列の長さを設定します。
    *
    * @param[in] value 配列の長さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 配列の長さが設定されている
    */
    void SetArrayLength( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->arrayLength = static_cast< uint32_t >( value );
    }

    /**
    * @brief 配列インデックスのベース値を取得します。
    *
    * @return 配列インデックスのベース値を返します。
    */
    int GetBaseArrayIndex() const NN_NOEXCEPT
    {
        return static_cast< int >( this->baseArrayIndex );
    }

    /**
    * @brief 配列の長さを取得します。
    *
    * @return 配列の長さを返します。
    */
    int GetArrayLength() const NN_NOEXCEPT
    {
        return static_cast< int >( this->arrayLength );
    }
};


/**
* @brief テクスチャーにおけるイメージ範囲を表すためのクラスです。
*/
class TextureSubresourceRange
    : public detail::DataContainer< TextureSubresourceRangeData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureSubresourceRange() NN_NOEXCEPT
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
    * - EditMipRange().SetDefault();
    * - EditArrayRange().SetDefault();
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief ミップマップ範囲を編集します。
    *
    * @return ミップマップ範囲への参照を返します。
    */
    TextureMipRange& EditMipRange() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->mipRange );
    }

    /**
    * @brief 配列範囲を編集します。
    *
    * @return 配列範囲への参照を返します。
    */
    TextureArrayRange& EditArrayRange() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->arrayRange );
    }

    /**
    * @brief ミップマップ範囲を取得します。
    *
    * @return ミップマップ範囲への参照を返します。
    */
    const TextureMipRange& GetMipRange() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->mipRange );
    }

    /**
    * @brief 配列範囲を取得します。
    *
    * @return 配列範囲への参照を返します。
    */
    const TextureArrayRange& GetArrayRange() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->arrayRange );
    }
};

/**
* @brief テクスチャーにおける特定のイメージを表すクラスです。
*/
class TextureSubresource
    : public detail::DataContainer< TextureSubresourceData >
{
public:
    /**
    * コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureSubresource() NN_NOEXCEPT
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
    * - SetMipLevel( 0 );
    * - SetArrayIndex( 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 単一のミップマップレベルを設定します。
    *
    * @param[in] value ミップマップレベル
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - ミップマップレベルが設定されている
    */
    void SetMipLevel( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->mipLevel = static_cast< uint16_t >( value );
    }

    /**
    * @brief 単一の配列インデックスを設定します。
    *
    * @param[in] value 配列インデックス
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 配列インデックスが設定されている
    */
    void SetArrayIndex( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->arrayIndex = static_cast< uint32_t >( value );
    }

    /**
    * @brief ミップマップレベルを取得します。
    *
    * @return ミップマップレベルを返します。
    */
    int GetMipLevel() const NN_NOEXCEPT
    {
        return static_cast< int >( this->mipLevel );
    }

    /**
    * @brief 配列インデックスを取得します。
    *
    * @return 配列インデックスを返します。
    */
    int GetArrayIndex() const NN_NOEXCEPT
    {
        return static_cast< int >( this->arrayIndex );
    }
};

/**
* @brief テクスチャーコピーオペレーションの範囲を表すクラスです。
*/
class TextureCopyRegion
    : public detail::DataContainer< TextureCopyRegionData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TextureCopyRegion() NN_NOEXCEPT
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
    * - SetOffsetU( 0 );
    * - SetOffsetV( 0 );
    * - SetOffsetW( 0 );
    * - SetWidth( 1 );
    * - SetHeight( 1 );
    * - SetDepth( 1 );
    * - EditSubresource().SetDefault();
    * - SetArrayLength( 1 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief U 座標のオフセットを設定します。
    *
    * @param[in] value U 座標のオフセット
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - U 座標のオフセットが設定されている。
    */
    void SetOffsetU( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->offsetU = static_cast< uint32_t >( value );
    }

    /**
    * @brief V 座標のオフセットを設定します。
    *
    * @param[in] value V 座標のオフセット
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - V 座標のオフセットが設定されている。
    */
    void SetOffsetV( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->offsetV = static_cast< uint32_t >( value );
    }

    /**
    * @brief W 座標のオフセットを設定します。
    *
    * @param[in] value W 座標のオフセット
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - W 座標のオフセットが設定されている。
    */
    void SetOffsetW( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->offsetW = static_cast< uint32_t >( value );
    }

    /**
    * @brief 幅を設定します。
    *
    * @param[in] value 幅
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 幅が設定されている
    */
    void SetWidth( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->width = static_cast< uint32_t >( value );
    }

    /**
    * @brief 高さを設定します。
    *
    * @param[in] value 高さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 高さが設定されている
    */
    void SetHeight( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->height = static_cast< uint32_t >( value );
    }

    /**
    * @brief 深さを設定します。
    *
    * @param[in] value 深さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 深さが設定されている
    */
    void SetDepth( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->depth = static_cast< uint32_t >( value );
    }

    /**
    * @brief イメージを特定するパラメーターを編集します。
    *
    * @return イメージを特定するパラメーターへの参照を返します。
    */
    TextureSubresource& EditSubresource() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->subresource );
    }

    /**
    * @brief 配列の長さを設定します。
    *
    * @param[in] value 配列の長さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 配列の長さが設定されている
    */
    void SetArrayLength( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->arrayLength = static_cast< uint32_t >( value );
    }

    /**
    * @brief U 座標のオフセットを取得します。
    *
    * @return U 座標のオフセットを返します。
    */
    int GetOffsetU() const NN_NOEXCEPT
    {
        return static_cast< int >( this->offsetU );
    }

    /**
    * @brief V 座標のオフセットを取得します。
    *
    * @return V 座標のオフセットを返します。
    */
    int GetOffsetV() const NN_NOEXCEPT
    {
        return static_cast< int >( this->offsetV );
    }

    /**
    * @brief W 座標のオフセットを取得します。
    *
    * @return W 座標のオフセットを返します。
    */
    int GetOffsetW() const NN_NOEXCEPT
    {
        return static_cast< int >( this->offsetW );
    }

    /**
    * @brief 幅を取得します。
    *
    * @return 幅を返します。
    */
    int GetWidth() const NN_NOEXCEPT
    {
        return static_cast< int >( this->width );
    }

    /**
    * @brief 高さを取得します。
    *
    * @return 高さを返します。
    */
    int GetHeight() const NN_NOEXCEPT
    {
        return static_cast< int >( this->height );
    }

    /**
    * @brief 深さを取得します。
    *
    * @return 深さを返します。
    */
    int GetDepth() const NN_NOEXCEPT
    {
        return static_cast< int >( this->depth );
    }

    /**
    * @brief イメージを特定するパラメーターを取得します。
    *
    * @return イメージを特定するパラメーターへの参照を返します。
    */
    const TextureSubresource& GetSubresource() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->subresource );
    }

    /**
    * @brief 配列の長さを取得します。
    *
    * @return 配列の長さを返します。
    */
    int GetArrayLength() const NN_NOEXCEPT
    {
        return static_cast< int >( this->arrayLength );
    }
};

/**
* @brief バッファー・テクスチャー間でのコピーオペレーションの範囲を表すクラスです。
*/
class BufferTextureCopyRegion
    : public detail::DataContainer< BufferTextureCopyRegionData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    BufferTextureCopyRegion() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを規定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 以下を呼ぶことに相当します。
    * - SetBufferOffset( 0 );
    * - SetBufferImageWidth( 0 );
    * - SetBufferImageHeight( 0 );
    * - EditTextureCopyRegion().SetDefault();
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief バッファーにおけるオフセットを指定します。
    *
    * @param[in] value バイトでのバッファーにおけるオフセット
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - バッファーにおけるオフセットが設定されている
    */
    void SetBufferOffset( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->bufferOffset = value;
    }

    /**
    * @brief バッファーにおけるイメージの幅を指定します。
    *
    * @param[in] value ピクセルでのバッファーにおけるイメージの幅
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - バッファーにおける行の長さが指定されている
    */
    void SetBufferImageWidth( int value ) NN_NOEXCEPT
    {
        this->bufferImageWidth = value;
    }

    /**
    * @brief バッファーにおけるイメージの高さを指定します。
    *
    * @param[in] value バッファーにおけるイメージの高さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - バッファーにおけるイメージの高さが設定されている
    *
    * @details
    * 0 を指定した場合は GetTextureCopyRegion().GetHeight() と等しいとみなされます。
    */
    void SetBufferImageHeight( int value ) NN_NOEXCEPT
    {
        this->bufferImageHeight = value;
    }

    /**
    * @brief テクスチャーのコピー領域についての情報を編集します。
    *
    * @return テクスチャーのコピー領域についての情報への参照を返します。
    */
    TextureCopyRegion& EditTextureCopyRegion() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->textureCopyRegion );
    }

    /**
    * @brief バッファーにおけるオフセットを取得します。
    *
    * @return バイトでのバッファーにおけるオフセットを返します。
    */
    int GetBufferOffset() const NN_NOEXCEPT
    {
        return this->bufferOffset;
    }

    /**
    * @brief バッファーにおけるイメージの幅を取得します。
    *
    * @return ピクセルでのバッファーにおけるイメージの幅を返します。
    */
    int GetBufferImageWidth() const NN_NOEXCEPT
    {
        return this->bufferImageWidth;
    }

    /**
    * @brief バッファーにおけるイメージの高さを取得します。
    *
    * @return バッファーにおけるイメージの高さを返します。
    */
    int GetBufferImageHeight() const NN_NOEXCEPT
    {
        return this->bufferImageHeight;
    }

    /**
    * @brief テクスチャーのコピー領域についての情報を取得します。
    *
    * @return テクスチャーのコピー領域についての情報への参照を返します。
    */
    const TextureCopyRegion& GetTextureCopyRegion() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->textureCopyRegion );
    }
};

/**
* @brief テクスチャーを初期化するための情報を表すグラフィックス抽象レイヤーです。
*/
template< typename TTarget >
class TTextureInfo
    : public detail::TargetInfoImpl< detail::TextureInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, TextureInfo >::Type
{
    typedef typename detail::TargetInfoImpl< detail::TextureInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, TextureInfo >::Type Impl;

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
    static void ConvertFromInfo( TTextureInfo< TTarget >* pDstInfo, const TextureInfo& info ) NN_NOEXCEPT
    {
        return detail::InfoHelper::ConvertFromInfo< Impl >( pDstInfo, info );
    }

    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TTextureInfo() NN_NOEXCEPT
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
    * - SetImageStorageDimension( nn::gfx::ImageStorageDimension_2d );
    * - SetImageFormat( nn::gfx::ImageFormat_Undefined );
    * - SetGpuAccessFlags( 0 );
    * - SetTileMode( nn::gfx::TileMode_Optimal );
    * - SetWidth( 1 );
    * - SetHeight( 1 );
    * - SetDepth( 1 );
    * - SetMipCount( 1 );
    * - SetArrayLength( 0 );
    * - SetSwizzle( 0 );
    * - SetMultiSampleCount( 1 );
    */
    void SetDefault() NN_NOEXCEPT
    {
        return Impl::SetDefault();
    }

    /**
    * @brief イメージデータの次元を設定します。
    *
    * @param[in] value イメージデータの次元
    *
    * @post
    * - イメージデータの次元が設定されている
    */
    void SetImageStorageDimension( ImageStorageDimension value ) NN_NOEXCEPT
    {
        return Impl::SetImageStorageDimension( value );
    }

    /**
    * @brief イメージフォーマットを設定します。
    *
    * @param[in] value イメージフォーマット
    *
    * @post
    * - イメージフォーマットが設定されている
    */
    void SetImageFormat( ImageFormat value ) NN_NOEXCEPT
    {
        return Impl::SetImageFormat( value );
    }

    /**
    * @brief GPU がアクセスする方法を設定します。
    *
    * @param[in] value nn::gfx::GpuAccess の値の組み合わせによる GPU がアクセスする方法
    *
    * @post
    * - GPU がアクセスする方法が設定されている
    */
    void SetGpuAccessFlags( int value ) NN_NOEXCEPT
    {
        return Impl::SetGpuAccessFlags( value );
    }

    /**
    * @brief タイリングモードを設定します。
    *
    * @param[in] value タイリングモード
    *
    * @post
    * - タイリングモードが設定されている
    */
    void SetTileMode( TileMode value ) NN_NOEXCEPT
    {
        return Impl::SetTileMode( value );
    }

    /**
    * @brief ベースレベルのイメージの幅を設定します。
    *
    * @param[in] value ベースレベルのイメージの幅
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ベースレベルのイメージの幅が設定されている
    */
    void SetWidth( int value ) NN_NOEXCEPT
    {
        return Impl::SetWidth( value );
    }

    /**
    * @brief ベースレベルのイメージの高さを設定します。
    *
    * @param[in] value ベースレベルのイメージの高さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ベースレベルのイメージの高さが設定されている
    */
    void SetHeight( int value ) NN_NOEXCEPT
    {
        return Impl::SetHeight( value );
    }

    /**
    * @brief ベースレベルのイメージの深さを設定します。
    *
    * @param[in] value ベースレベルのイメージの深さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ベースレベルのイメージの深さが設定されている
    */
    void SetDepth( int value ) NN_NOEXCEPT
    {
        return Impl::SetDepth( value );
    }

    /**
    * @brief 配列の長さを設定します。
    *
    * @param[in] value 配列の長さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - 配列の長さが設定されている
    */
    void SetArrayLength( int value ) NN_NOEXCEPT
    {
        return Impl::SetArrayLength( value );
    }

    /**
    * @brief スウィズリングのベース値を設定します。
    *
    * @param[in] value スウィズリングのベース値
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - スウィズリングのベース値が設定されている
    */
    void SetSwizzle( int value ) NN_NOEXCEPT
    {
        return Impl::SetSwizzle( value );
    }

    /**
    * @brief マルチサンプリングの数を設定します。
    *
    * @param[in] value マルチサンプリングの数
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - マルチサンプリングの数が設定されている
    */
    void SetMultiSampleCount( int value ) NN_NOEXCEPT
    {
        return Impl::SetMultiSampleCount( value );
    }

    /**
    * @brief ミップマップの数を設定します。
    *
    * @param[in] value ベースレベルを含むミップマップの数
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - ミップマップの数が設定されている
    */
    void SetMipCount( int value ) NN_NOEXCEPT
    {
        return Impl::SetMipCount( value );
    }
};

}
}
