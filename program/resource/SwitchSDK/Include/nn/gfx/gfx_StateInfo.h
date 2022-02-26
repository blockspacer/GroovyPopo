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
* @brief ステート初期化用の情報に関する API の宣言
*/

#pragma once

#include <limits>

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DataAccessorConverter.h>
#include <nn/gfx/gfx_StateInfoData.h>

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief マルチサンプルステート情報を表すクラスです。
*/
class MultisampleStateInfo
    : public detail::DataContainer< MultisampleStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    MultisampleStateInfo() NN_NOEXCEPT
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
    * - SetAlphaToCoverageEnabled( false );
    * - SetSampleCount( 1 );
    * - SetSampleMask( 0xFFFFFFFF );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief サンプル数を設定します。
    *
    * @param[in] value サンプル数
    *
    * @pre
    * - value >= 0 && value <= 0xFF
    *
    * @post
    * - サンプル数が設定されている
    *
    * @platformbegin{GL}
    * このパラメータはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetSampleCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= 0xFF );
        this->sampleCount = static_cast< uint8_t >( value );
    }

    /**
    * @brief サンプルマスクを設定します。
    *
    * @param[in] value サンプルマスク
    *
    * @post
    * - サンプルマスクが設定されている
    */
    void SetSampleMask( int value ) NN_NOEXCEPT
    {
        this->sampleMask = static_cast< uint32_t >( value );
    }

    /**
    * @brief マルチサンプリングの際にアルファ値をカバレッジ値とするかどうかを設定します。
    *
    * @param[in] value アルファ値をカバレッジ値とするかどうかを表す真偽値
    *
    * @post
    * - アルファ値をカバレッジ値とするかどうかが設定されている
    *
    * @details
    * BlendStateInfo の SetAlphaToCoverageEnabled と同じ値を設定してください。
    */
    void SetAlphaToCoverageEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_AlphaToCoverageEnable, value );
    }

    /**
    * @brief マルチサンプリングの際にアルファ値をカバレッジ値とするかどうかを取得します。
    *
    * @return アルファ値をカバレッジ値とするかどうかを表す真偽値を返します。
    */
    bool IsAlphaToCoverageEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_AlphaToCoverageEnable );
    }

    /**
    * @brief サンプル数を取得します。
    *
    * @return サンプル数を返します。
    */
    int GetSampleCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->sampleCount );
    }

    /**
    * @brief サンプルマスクを取得します。
    *
    * @return サンプルマスクを返します。
    */
    int GetSampleMask() const NN_NOEXCEPT
    {
        return static_cast< int >( this->sampleMask );
    }
};

/**
* @brief ラスタライザステートを初期化するための情報を表すクラスです。
*/
class RasterizerStateInfo
    : public detail::DataContainer< RasterizerStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    RasterizerStateInfo() NN_NOEXCEPT
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
    * - SetFillMode( nn::gfx::FillMode_Solid );
    * - SetCullMode( nn::gfx::CullMode_Back );
    * - SetFrontFace( nn::gfx::FrontFace_Ccw );
    * - SetPrimitiveTopologyType( nn::gfx::PrimitiveTopologyType_Triangle );
    * - SetRasterEnabled( true );
    * - SetMultisampleEnabled( false );
    * - SetDepthClipEnabled( true );
    * - SetScissorEnabled( false );
    * - SetDepthBias( 0 );
    * - SetDepthBiasClamp( 0.0f );
    * - SetSlopeScaledDepthBias( 0.0f );
    * - SetConservativeRasterizationMode( nn::gfx::ConservativeRasterizationMode_Disable );
    * - EditMultisampleStateInfo().SetDefault();
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 塗りつぶしモードを設定します。
    *
    * @param[in] value 塗りつぶしモード
    *
    * @post
    * - 塗りつぶしモードが設定されている
    */
    void SetFillMode( FillMode value ) NN_NOEXCEPT
    {
        this->fillMode = static_cast< Bit8 >( value );
    }

    /**
    * @brief 前面とする面を設定します。
    *
    * @param[in] value 前面とする面
    *
    * @post
    * - 前面とする面が設定されている
    */
    void SetFrontFace( FrontFace value ) NN_NOEXCEPT
    {
        this->frontFace = static_cast< Bit8 >( value );
    }

    /**
    * @brief カリングモードを設定します。
    *
    * @param[in] value カリングモード
    *
    * @post
    * - カリングモードが設定されている
    */
    void SetCullMode( CullMode value ) NN_NOEXCEPT
    {
        this->cullMode = static_cast< Bit8 >( value );
    }

    /**
    * @brief プリミティブトポロジータイプを設定します。
    *
    * @param[in] value プリミティブトポロジータイプ
    *
    * @post
    * - プリミティブトポロジータイプが設定されている
    */
    void SetPrimitiveTopologyType( PrimitiveTopologyType value ) NN_NOEXCEPT
    {
        this->primitiveTopologyType = static_cast< Bit8 >( value );
    }

    /**
    * @brief ラスタライズを有効または無効に設定します。
    *
    * @param[in] value ラスタライザーの有効・無効を表す真偽値
    *
    * @post
    * - ラスタライザーの有効・無効が設定されている
    */
    void SetRasterEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_RasterDisable, !value );
    }

    /**
    * @brief マルチサンプリングを有効または無効に設定します。
    *
    * @param[in] value マルチサンプリングの有効・無効を表す真偽値
    *
    * @post
    * - マルチサンプリングの有効・無効が設定されている
    */
    void SetMultisampleEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_MultisampleEnable, value );
    }

    /**
    *  @brief 深度によるクリッピングを有効または無効に設定します。
    *
    * @param[in] value 深度によるクリッピングの有効・無効を表す真偽値
    *
    * @post
    * - 深度によるクリッピングの有効・無効が設定されている
    */
    void SetDepthClipEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_DepthClipDisable, !value );
    }

    /**
    * @brief シザリングを有効または無効に設定します。
    *
    * @param[in] value シザリングの有効・無効を表す真偽値
    *
    * @post
    * - シザリングの有効・無効が設定されている
    *
    * @details
    * ビューポートステートのシザリングの有効・無効と一致している必要があります。
    */
    void SetScissorEnabled( bool value ) NN_NOEXCEPT
    {
        return this->flag.SetBit( Flag_ScissorEnable, value );
    }

    /**
    * @brief 最大深度スロープでスケールされる、ピクセルの深度値に加算する値を設定します。
    *
    * @param[in] value 最大深度スロープでスケールされる、ピクセルの深度値に加算する値
    *
    * @post
    * - 最大深度スロープでスケールされる、ピクセルの深度値に加算する値が設定されている
    */
    void SetSlopeScaledDepthBias( float value ) NN_NOEXCEPT
    {
        this->slopeScaledDepthBias = value;
    }

    /**
    * @brief 深度フォーマットの最小の表現可能な数でスケールされる、ピクセルの深度値に加算する値を設定します。
    *
    * @param[in] value 深度フォーマットの最小の表現可能な数でスケールされる、ピクセルの深度値に加算する値
    *
    * @post
    * - 深度フォーマットの最小の表現可能な数でスケールされる、ピクセルの深度値に加算する値が設定されている
    */
    void SetDepthBias( int value ) NN_NOEXCEPT
    {
        this->depthBias = static_cast< int32_t >( value );
    }

    /**
    * @brief ピクセルの深度値に加算される最大値を設定します。
    *
    * @param[in] value ピクセルの深度値に加算される最大値
    *
    * @post
    * - ピクセルの深度値に加算される最大値が設定されている
    *
    * @platformbegin{GL}
    * このパラメーターはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetDepthBiasClamp( float value ) NN_NOEXCEPT
    {
        this->depthBiasClamp = value;
    }

    /**
    * @brief 保守的なラスタライゼーションモードを設定します。
    *
    * @param[in] value 保守的なラスタライゼーションモード
    *
    * @post
    * - 保守的なラスタライゼーションモードが設定されている
    */
    void SetConservativeRasterizationMode( ConservativeRasterizationMode value ) NN_NOEXCEPT
    {
        this->conservativeRasterizationMode = static_cast< Bit8 >( value );
    }

    /**
    * @brief マルチサンプリングステート情報を編集します。
    *
    * @return マルチサンプリングステート情報への参照を返します。
    */
    MultisampleStateInfo& EditMultisampleStateInfo() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->multisample );
    }

    /**
    * @brief 塗りつぶしモードを取得します。
    *
    * @return 塗りつぶしモードを返します。
    */
    FillMode GetFillMode() const NN_NOEXCEPT
    {
        return static_cast< FillMode >( this->fillMode );
    }

    /**
    * @brief 前面とする面を取得します。
    *
    * @return 前面とする面を返します。
    */
    FrontFace GetFrontFace() const NN_NOEXCEPT
    {
        return static_cast< FrontFace >( this->frontFace );
    }

    /**
    * @brief カリングモードを取得します。
    *
    * @return カリングモードを返します。
    */
    CullMode GetCullMode() const NN_NOEXCEPT
    {
        return static_cast< CullMode >( this->cullMode );
    }

    /**
    * @brief プリミティブトポロジータイプを取得します。
    *
    * @return プリミティブトポロジータイプを返します。
    */
    PrimitiveTopologyType GetPrimitiveTopologyType() const NN_NOEXCEPT
    {
        return static_cast< PrimitiveTopologyType >( this->primitiveTopologyType );
    }

    /**
    * @brief ラスタライザーが有効かどうかを取得します。
    *
    * @return ラスタライザーが有効かどうかを表す真偽値を返します。
    */
    bool IsRasterEnabled() const NN_NOEXCEPT
    {
        return !this->flag.GetBit( Flag_RasterDisable );
    }

    /**
    * @brief マルチサンプリングが有効かどうかを取得します。
    *
    * @return マルチサンプリングが有効かどうかを表す真偽値を返します。
    */
    bool IsMultisampleEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_MultisampleEnable );
    }

    /**
    * @brief 深度によるクリッピングが有効かどうかを取得します。
    *
    * @return 深度によるクリッピングが有効かどうかを表す真偽値を返します。
    */
    bool IsDepthClipEnabled() const NN_NOEXCEPT
    {
        return !this->flag.GetBit( Flag_DepthClipDisable );
    }

    /**
    * @brief シザリングが有効かどうかを取得します。
    *
    * @return シザリングが有効かどうかを表す真偽値を返します。
    */
    bool IsScissorEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_ScissorEnable );
    }

    /**
    * @brief 最大深度スロープでスケールされる、ピクセルの深度値に加算する値を取得します。
    *
    * @return 最大深度スロープでスケールされる、ピクセルの深度値に加算する値を返します。
    */
    float GetSlopeScaledDepthBias() const NN_NOEXCEPT
    {
        return this->slopeScaledDepthBias;
    }

    /**
    * @brief 深度フォーマットの最小の表現可能な数でスケールされる、ピクセルの深度値に加算する値を取得します。
    *
    * @return 深度フォーマットの最小の表現可能な数でスケールされる、ピクセルの深度値に加算する値を返します。
    */
    int GetDepthBias() const NN_NOEXCEPT
    {
        return static_cast< int >( this->depthBias );
    }

    /**
    * @brief ピクセルの深度値に加算される最大値を取得します。
    *
    * @return ピクセルの深度値に加算される最大値を返します。
    */
    float GetDepthBiasClamp() const NN_NOEXCEPT
    {
        return this->depthBiasClamp;
    }

    /**
    * @brief 保守的なラスタライゼーションモードを取得します。
    *
    * @return 保守的なラスタライゼーションモードを返します。
    */
    ConservativeRasterizationMode GetConservativeRasterizationMode() const NN_NOEXCEPT
    {
        return static_cast< ConservativeRasterizationMode >( this->conservativeRasterizationMode );
    }

    /**
    * @brief マルチサンプリングステートを取得します。
    *
    * @return マルチサンプリングステートへの参照を返します。
    */
    const MultisampleStateInfo& GetMultisampleStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->multisample );
    }
};

/**
* @brief ブレンドターゲットステート情報を表すクラスです。
*/
class BlendTargetStateInfo
    : public detail::DataContainer< BlendTargetStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    BlendTargetStateInfo() NN_NOEXCEPT
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
    * - SetBlendEnabled( false );
    * - SetSourceColorBlendFactor( nn::gfx::BlendFactor_One );
    * - SetDestinationColorBlendFactor( nn::gfx::BlendFactor_Zero );
    * - SetColorBlendFunction( nn::gfx::BlendFunction_Add );
    * - SetSourceAlphaBlendFactor( nn::gfx::BlendFactor_One );
    * - SetDestinationAlphaBlendFactor( nn::gfx::BlendFactor_Zero );
    * - SetAlphaBlendFunction( nn::gfx::BlendFunction_Add );
    * - SetChannelMask( nn::gfx::ChannelMask_All );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief ブレンディングを有効または無効に設定します。
    *
    * @param[in] value ブレンディングの有効・無効を表す真偽値
    *
    * @post
    * - ブレンディングの有効・無効が設定されている
    */
    void SetBlendEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_BlendEnable, value );
    }

    /**
    * @brief ブレンディングの際のソースのカラー要素のブレンドファクターを設定します。
    *
    * @param[in] value ソースのカラー要素のブレンドファクター
    *
    * @post
    * - ソースのカラー要素のブレンドファクターが設定されている
    */
    void SetSourceColorBlendFactor( BlendFactor value ) NN_NOEXCEPT
    {
        this->sourceColorBlendFactor = static_cast< Bit8 >( value );
    }

    /**
    * @brief ブレンディングの際のデスティネーションのカラー要素のブレンドファクターを設定します。
    *
    * @param[in] value デスティネーションのカラー要素のブレンドファクター
    *
    * @post
    * - デスティネーションのカラー要素のブレンドファクターが設定されている
    */
    void SetDestinationColorBlendFactor( BlendFactor value ) NN_NOEXCEPT
    {
        this->destinationColorBlendFactor = static_cast< Bit8 >( value );
    }

    /**
    * @brief ブレンディングの際のカラー要素のブレンド関数を設定します。
    *
    * @param[in] value カラー要素のブレンド関数
    *
    * @post
    * - カラー要素のブレンド関数が設定されている
    */
    void SetColorBlendFunction( BlendFunction value ) NN_NOEXCEPT
    {
        this->colorBlendFunction = static_cast< Bit8 >( value );
    }

    /**
    * @brief ブレンディングの際のソースのアルファ要素のブレンドファクターを設定します。
    *
    * @param[in] value ソースのアルファ要素のブレンドファクター
    *
    * @post
    * - ソースのアルファ要素のブレンドファクターが設定されている
    */
    void SetSourceAlphaBlendFactor( BlendFactor value ) NN_NOEXCEPT
    {
        this->sourceAlphaBlendFactor = static_cast< Bit8 >( value );
    }

    /**
    * @brief ブレンディングの際のデスティネーションのアルファ要素のブレンドファクターを設定します。
    *
    * @param[in] value デスティネーションのアルファ要素のブレンドファクター
    *
    * @post
    * - デスティネーションのアルファ要素のブレンドファクターが設定されている
    */
    void SetDestinationAlphaBlendFactor( BlendFactor value ) NN_NOEXCEPT
    {
        this->destinationAlphaBlendFactor = static_cast< Bit8 >( value );
    }

    /**
    * @brief ブレンディングの際のアルファ要素のブレンド関数を設定します。
    *
    * @param[in] value アルファ要素のブレンド関数
    *
    * @post
    * - アルファ要素のブレンド関数が設定されている
    */
    void SetAlphaBlendFunction( BlendFunction value ) NN_NOEXCEPT
    {
        this->alphaBlendFunction = static_cast< Bit8 >( value );
    }

    /**
    * @brief 要素ごとの書き込みマスクを設定します。
    *
    * @param[in] value nn::gfx::ChannelMask の組み合わせによる書き込みマスク
    *
    * @post
    * - 書き込みマスクが設定されている
    */
    void SetChannelMask( int value ) NN_NOEXCEPT
    {
        this->channelMask = static_cast< Bit8 >( value );
    }

    /**
    * @brief ブレンディングが有効かどうかを取得します。
    *
    * @return ブレンディングが有効かどうかを表す真偽値を返します。
    */
    bool IsBlendEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_BlendEnable );
    }

    /**
    * @brief ブレンディングの際のソースのカラー要素のブレンドファクターを取得します。
    *
    * @return ソースのカラー要素のブレンドファクターを返します。
    */
    BlendFactor GetSourceColorBlendFactor() const NN_NOEXCEPT
    {
        return static_cast< BlendFactor >( this->sourceColorBlendFactor );
    }

    /**
    * @brief ブレンディングの際のデスティネーションのカラー要素のブレンドファクターを取得します。
    *
    * @return デスティネーションのカラー要素のブレンドファクターを返します。
    */
    BlendFactor GetDestinationColorBlendFactor() const NN_NOEXCEPT
    {
        return static_cast< BlendFactor >( this->destinationColorBlendFactor );
    }

    /**
    * @brief ブレンディングの際のカラーのブレンド関数を取得します。
    *
    * @return カラー要素のブレンド関数を返します。
    */
    BlendFunction GetColorBlendFunction() const NN_NOEXCEPT
    {
        return static_cast< BlendFunction >( this->colorBlendFunction );
    }

    /**
    * @brief ブレンディングの際のソースのアルファ要素のブレンドファクターを取得します。
    *
    * @return ソースのアルファ要素のブレンドファクターを返します。
    */
    BlendFactor GetSourceAlphaBlendFactor() const NN_NOEXCEPT
    {
        return static_cast< BlendFactor >( this->sourceAlphaBlendFactor );
    }

    /**
    * @brief ブレンディングの際のデスティネーションのアルファ要素のブレンドファクターを取得します。
    *
    * @return デスティネーションのアルファ要素のブレンドファクターを返します。
    */
    BlendFactor GetDestinationAlphaBlendFactor() const NN_NOEXCEPT
    {
        return static_cast< BlendFactor >( this->destinationAlphaBlendFactor );
    }

    /**
    * @brief ブレンディングの際のアルファ要素のブレンド関数を取得します。
    *
    * @return アルファ要素のブレンド関数を返します。
    */
    BlendFunction GetAlphaBlendFunction() const NN_NOEXCEPT
    {
        return static_cast< BlendFunction >( this->alphaBlendFunction );
    }

    /**
    * @brief 要素ごとの書き込みマスクを取得します。
    *
    * @return nn::gfx::ChannelMask の組み合わせによる要素ごとの書き込みマスクを返します。
    */
    int GetChannelMask() const NN_NOEXCEPT
    {
        return static_cast< int >( this->channelMask );
    }
};

/**
* @brief ブレンドステートを初期化するための情報を表すクラスです。
*/
class BlendStateInfo
    : public detail::DataContainer< BlendStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    BlendStateInfo() NN_NOEXCEPT
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
    * - SetAlphaToCoverageEnabled( false );
    * - SetDualSourceBlendEnabled( false );
    * - SetIndependentBlendEnabled( false );
    * - SetLogicOperationEnabled( false );
    * - SetLogicOperation( nn::gfx::LogicOperation_NoOp );
    * - SetBlendConstant( 0.0f, 0.0f, 0.0f, 1.0f );
    * - SetBlendTargetStateInfoArray( NULL, 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 論理オペレーションを設定します。
    *
    * @param[in] value 論理オペレーション
    *
    * @post
    * - 論理オペレーションが設定されている
    */
    void SetLogicOperation( LogicOperation value ) NN_NOEXCEPT
    {
        this->logicOperation = static_cast< Bit8 >( value );
    }

    /**
    * @brief マルチサンプリングの際にアルファ値をカバレッジ値とするかどうかを設定します。
    *
    * @param[in] value アルファ値をカバレッジ値とするかどうかを表す真偽値
    *
    * @post
    * - アルファ値をカバレッジ値とするかどうかが設定されている
    *
    * @details
    * MultisampleStateInfo の SetAlphaToCoverageEnabled と同じ値を設定してください。
    */
    void SetAlphaToCoverageEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_AlphaToCoverageEnable, value );
    }

    /**
    * @brief デュアルソースブレンディングを有効または無効に設定します。
    *
    * @param[in] value デュアルソースブレンディングの有効・無効を表す真偽値
    *
    * @post
    * - デュアルソースブレンディングの有効・無効が設定されている
    *
    * @platformbegin{GL}
    * このパラメータはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetDualSourceBlendEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_DualSourceBlendEnable, value );
    }

    /**
    * @brief 各カラーターゲットで独立したブレンディング処理を行うかどうかを設定します。
    *
    * @param[in] value 独立したブレンディング処理を行うかどうかを表す真偽値
    *
    * @post
    * - 独立したブレンディング処理を行うかどうかが設定されている
    *
    * @details
    * false を設定した場合は 0 番目の設定が全カラーターゲットについて使われます。
    */
    void SetIndependentBlendEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_IndependentBlendEnable, value );
    }

    /**
    * @brief 論理オペレーションを有効または無効に設定します。
    *
    * @param[in] value 論理オペレーションの有効・無効を表す真偽値
    *
    * @post
    * - 論理オペレーションの有効・無効が設定されている
    *
    * @details
    * 論理オペレーションを有効にした場合はブレンディングは無効になります。
    */
    void SetLogicOperationEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_LogicOperationEnable, value );
    }

    /**
    * @brief ブレンドファクターに使われるブレンド定数の値を設定します。
    *
    * @param[in] red ブレンド定数の赤要素
    * @param[in] green ブレンド定数の緑要素
    * @param[in] blue ブレンド定数の青要素
    * @param[in] alpha ブレンド定数のアルファ要素
    *
    * @post
    * - ブレンド定数の値が設定されている
    */
    void SetBlendConstant( float red, float green, float blue, float alpha ) NN_NOEXCEPT
    {
        this->blendConstant[ 0 ] = red;
        this->blendConstant[ 1 ] = green;
        this->blendConstant[ 2 ] = blue;
        this->blendConstant[ 3 ] = alpha;
    }

    /**
    * @brief ブレンドターゲットステート情報の配列を設定します。
    *
    * @param[in] pBlendTargetStateInfoArray ブレンドターゲットステート情報の配列へのポインター
    * @param[in] blendTargetStateCount ブレンドターゲットステートの数
    *
    * @pre
    * - blendTargetStateCount >= 0 && blendTargetStateCount <= 255
    *
    * @post
    * - ブレンドステート情報の配列とその数が設定されている
    *
    * @details
    * pBlendTargetStateInfoArray の各要素は順番に各レンダーターゲットに対応します。@n
    * ポインター先の内容はオブジェクトの初期化後に破棄できます。
    */
    void SetBlendTargetStateInfoArray( const BlendTargetStateInfo* pBlendTargetStateInfoArray,
        int blendTargetStateCount ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( blendTargetStateCount >= 0 &&
            ( std::numeric_limits< uint8_t >::max )() );
        this->blendTargetCount = static_cast< uint8_t >( blendTargetStateCount );
        this->pBlendTargetArray.ptr = nn::gfx::AccessorToData( pBlendTargetStateInfoArray );
    }

    /**
    * @brief ブレンドターゲットの数を取得します。
    *
    * @return ブレンドターゲットの数を返します。
    */
    int GetBlendTargetCount() const NN_NOEXCEPT
    {
        return this->blendTargetCount;
    }

    /**
    * @brief 論理オペレーションを取得します。
    *
    * @return 論理オペレーションを返します。
    */
    LogicOperation GetLogicOperation() const NN_NOEXCEPT
    {
        return static_cast< LogicOperation >( this->logicOperation );
    }

    /**
    * @brief マルチサンプリングの際にアルファ値をカバレッジ値とするかどうかを取得します。
    *
    * @return アルファ値をカバレッジ値とするかどうかを表す真偽値を返します。
    */
    bool IsAlphaToCoverageEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_AlphaToCoverageEnable );
    }

    /**
    * @brief デュアルソースブレンディングが有効かどうかを取得します。
    *
    * @return デュアルソースブレンディングが有効かどうかを表す真偽値を返します。
    */
    bool IsDualSourceBlendEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_DualSourceBlendEnable );
    }

    /**
    * @brief 各カラーターゲットで独立したブレンディング処理を行うかどうかを取得します。
    *
    * @return 独立したブレンディング処理を行うかどうかを表す真偽値を返します。
    */
    bool IsIndependentBlendEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_IndependentBlendEnable );
    }

    /**
    * @brief 論理オペレーションが有効かどうかを取得します。
    *
    * @return 論理オペレーションが有効かどうかを表す真偽値を返します。
    */
    bool IsLogicOperationEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_LogicOperationEnable );
    }

    /**
    * @brief ブレンドファクターに使われるブレンド定数を取得します。
    *
    * @param[in] channel 取得するブレンド定数の色要素
    *
    * @return ブレンドファクターに使われるブレンド定数を返します。
    */
    float GetBlendConstant( ColorChannel channel ) const NN_NOEXCEPT
    {
        return this->blendConstant[ channel ];
    }

    /**
    * @brief ブレンドターゲットステート情報の配列を取得します。
    *
    * @return ブレンドターゲットステート情報の配列へのポインターを返します。
    */
    const BlendTargetStateInfo* GetBlendTargetStateInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pBlendTargetArray.ptr );
    }
};

/**
* @brief ステンシルステート情報を表すクラスです。
*/
class StencilStateInfo
    : public detail::DataContainer< StencilStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    StencilStateInfo() NN_NOEXCEPT
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
    * - SetStencilFailOperation( nn::gfx::StencilOperation_Keep );
    * - SetDepthFailOperation( nn::gfx::StencilOperation_Keep );
    * - SetDepthPassOperation( nn::gfx::StencilOperation_Keep );
    * - SetComparisonFunction( nn::gfx::ComparisonFunction_Always );
    * - SetStencilRef( 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief ステンシルテストに失敗したときのステンシルオペレーションを設定します。
    *
    * @param[in] value ステンシルテストに失敗したときのステンシルオペレーション
    *
    * @post
    * - ステンシルテストに失敗したときのステンシルオペレーションが設定されている
    */
    void SetStencilFailOperation( StencilOperation value ) NN_NOEXCEPT
    {
        this->stencilFailOperation = static_cast< Bit8 >( value );
    }

    /**
    * @brief 深度テストに失敗したときのステンシルオペレーションを設定します。
    *
    * @param[in] value 深度テストに失敗したときのステンシルオペレーション
    *
    * @post
    * - 深度テストに失敗したときのステンシルオペレーションが設定されている
    */
    void SetDepthFailOperation( StencilOperation value ) NN_NOEXCEPT
    {
        this->depthFailOperation = static_cast< Bit8 >( value );
    }

    /**
    * @brief 深度テストに成功したときのステンシルオペレーションを設定します。
    *
    * @param[in] value 深度テストに成功したときのステンシルオペレーション
    *
    * @post
    * - 深度テストに成功したときのステンシルオペレーションが設定されている
    */
    void SetDepthPassOperation( StencilOperation value ) NN_NOEXCEPT
    {
        this->depthPassOperation = static_cast< Bit8 >( value );
    }

    /**
    * @brief ステンシルテストの際の比較関数を設定します。
    *
    * @param[in] value ステンシルテストの比較関数
    *
    * @post
    * - ステンシルテストの比較関数が設定されている
    */
    void SetComparisonFunction( ComparisonFunction value ) NN_NOEXCEPT
    {
        this->comparisonFunction = static_cast< Bit8 >( value );
    }

    /**
    * @brief ステンシルテストに使われるステンシル参照値を設定します。
    *
    * @param[in] value ステンシル参照値
    *
    * @pre
    * - value >= 0 && value <= 255
    *
    * @post
    * - ステンシル参照値が設定されている
    */
    void SetStencilRef( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint8_t >::max )() );
        this->stencilRef = static_cast< uint8_t >( value );
    }

    /**
    * @brief ステンシルテストに失敗したときのステンシルオペレーションを取得します。
    *
    * @return ステンシルテストに失敗したときのステンシルオペレーションを返します。
    */
    StencilOperation GetStencilFailOperation() const NN_NOEXCEPT
    {
        return static_cast< StencilOperation >( this->stencilFailOperation );
    }

    /**
    * @brief 深度テストに失敗したときのステンシルオペレーションを取得します。
    *
    * @return 深度テストに失敗したときのステンシルオペレーションを返します。
    */
    StencilOperation GetDepthFailOperation() const NN_NOEXCEPT
    {
        return static_cast< StencilOperation >( this->depthFailOperation );
    }

    /**
    * @brief 深度テストに成功したときのステンシルオペレーションを取得します。
    *
    * @return 深度テストに成功したときのステンシルオペレーションを返します。
    */
    StencilOperation GetDepthPassOperation() const NN_NOEXCEPT
    {
        return static_cast< StencilOperation >( this->depthPassOperation );
    }

    /**
    * @brief 深度テストの比較関数を取得します。
    *
    * @return 深度テストの比較関数を返します。
    */
    ComparisonFunction GetComparisonFunction() const NN_NOEXCEPT
    {
        return static_cast< ComparisonFunction >( this->comparisonFunction );
    }

    /**
    * @brief ステンシルテストに使われるステンシル参照値を取得します。
    *
    * @return ステンシル参照値を返します。
    */
    int GetStencilRef() const NN_NOEXCEPT
    {
        return this->stencilRef;
    }
};

/**
* @brief 深度ステンシルステートを初期化するための情報を表すクラスです。
*/
class DepthStencilStateInfo
    : public detail::DataContainer< DepthStencilStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DepthStencilStateInfo() NN_NOEXCEPT
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
    * - SetDepthComparisonFunction( nn::gfx::ComparisonFunction_Less );
    * - SetDepthTestEnabled( false );
    * - SetDepthWriteEnabled( false );
    * - SetStencilTestEnabled( false );
    * - SetDepthBoundsTestEnabled( false );
    * - SetStencilReadMask( static_cast< uint8_t >( ~0 ) );
    * - SetStencilWriteMask( static_cast< uint8_t >( ~0 ) );
    * - EditFrontStencilStateInfo().SetDefault();
    * - EditBackStencilStateInfo().SetDefault();
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 深度テストの比較関数を設定します。
    *
    * @param[in] value 深度テストの比較関数
    *
    * @post
    * - 深度テストの比較関数が設定されている
    */
    void SetDepthComparisonFunction( ComparisonFunction value ) NN_NOEXCEPT
    {
        this->depthComparisonFunction = static_cast< Bit8 >( value );
    }

    /**
    * @brief 深度テストを有効または無効に設定します。
    *
    * @param[in] value 深度テストの有効・無効を表す真偽値
    *
    * @post
    * - 深度テストの有効・無効が設定されている
    */
    void SetDepthTestEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_DepthTestEnable, value );
    }

    /**
    * @brief 深度値の書き込みを有効または無効に設定します。
    *
    * @param[in] value 深度値の書き込みの有効・無効を表す真偽値
    *
    * @post
    * - 深度値の書き込みの有効・無効が設定されている
    */
    void SetDepthWriteEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_DepthWriteEnable, value );
    }

    /**
    * @brief ステンシルテストを有効または無効に設定します。
    *
    * @param[in] value ステンシルテストの有効・無効を表す真偽値
    *
    * @post
    * - ステンシルテストの有効・無効が設定されている
    */
    void SetStencilTestEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_StencilTestEnable, value );
    }

    /**
    * @brief 深度境界テストを有効または無効に設定します。
    *
    * @param[in] value 深度境界テストの有効・無効を表す真偽値
    *
    * @post
    * - 深度境界テストの有効・無効が設定されている
    */
    void SetDepthBoundsTestEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_DepthBoundsTestEnable, value );
    }

    /**
    * @brief ステンシル読み込みに適用されるマスクを設定します。
    *
    * @param[in] value ステンシル読み込み時のマスク
    *
    * @pre
    * - value >= 0 && value <= 255
    *
    * @post
    * - ステンシル読み込み時のマスクが設定されている
    */
    void SetStencilReadMask( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint8_t >::max )() );
        this->stencilReadMask = static_cast< uint8_t >( value );
    }

    /**
    * @brief ステンシル書き込みに適用されるマスクを設定します。
    *
    * @param[in] value ステンシル書き込み時のマスク
    *
    * @pre
    * - value >= 0 && value <= 255
    *
    * @post
    * - ステンシル書き込み時のマスクが設定されている
    */
    void SetStencilWriteMask( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint8_t >::max )() );
        this->stencilWriteMask = static_cast< uint8_t >( value );
    }

    /**
    * @brief 前面ポリゴンについてのステンシルステート情報を編集します。
    *
    * @return 前面ポリゴンについてのステンシルステート情報への参照を返します。
    */
    StencilStateInfo& EditFrontStencilStateInfo() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->frontStencil );
    }

    /**
    * @brief 背面ポリゴンについてのステンシルステート情報を編集します。
    *
    * @return 背面ポリゴンについてのステンシルステート情報への参照を返します。
    */
    StencilStateInfo& EditBackStencilStateInfo() NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->backStencil );
    }

    /**
    * @brief 深度テストの比較関数を取得します。
    *
    * @return 深度テストの比較関数を返します。
    */
    ComparisonFunction GetDepthComparisonFunction() const NN_NOEXCEPT
    {
        return static_cast< ComparisonFunction >( this->depthComparisonFunction );
    }

    /**
    * @brief 深度テストが有効かどうかを取得します。
    *
    * @return 深度テストが有効かどうかを表す真偽値を返します。
    */
    bool IsDepthTestEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_DepthTestEnable );
    }

    /**
    * @brief 深度書き込みが有効かどうかを取得します。
    *
    * @return 深度書き込みが有効かどうかを表す真偽値を返します。
    */
    bool IsDepthWriteEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_DepthWriteEnable );
    }

    /**
    * @brief ステンシルテストが有効かどうかを取得します。
    *
    * @return ステンシルテストが有効かどうかを表す真偽値を返します。
    */
    bool IsStencilTestEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_StencilTestEnable );
    }

    /**
    * @brief 深度境界テストが有効かどうかを取得します。
    *
    * @return 深度境界テストが有効かどうかを表す真偽値を返します。
    */
    bool IsDepthBoundsTestEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_DepthBoundsTestEnable );
    }

    /**
    * @brief ステンシル読み込み時のマスクを取得します。
    *
    * @return ステンシル読み込み時のマスクを返します。
    */
    int GetStencilReadMask() const NN_NOEXCEPT
    {
        return this->stencilReadMask;
    }

    /**
    * @brief ステンシル書き込み時のマスクを取得します。
    *
    * @return ステンシル書き込み時のマスクを返します。
    */
    int GetStencilWriteMask() const NN_NOEXCEPT
    {
        return this->stencilWriteMask;
    }

    /**
    * @brief 前面ポリゴンについてのステンシル情報を取得します。
    *
    * @return 前面ポリゴンについてのステンシル情報を返します。
    */
    const StencilStateInfo& GetFrontStencilStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->frontStencil );
    }

    /**
    * @brief 背面ポリゴンについてのステンシル情報を取得します。
    *
    * @return 背面ポリゴンについてのステンシル情報を返します。
    */
    const StencilStateInfo& GetBackStencilStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->backStencil );
    }
};

/**
* @brief カラーターゲットステートの情報を表すクラスです。
*/
class ColorTargetStateInfo
    : public detail::DataContainer< ColorTargetStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ColorTargetStateInfo() NN_NOEXCEPT
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
    * - SetFormat( nn::gfx::ImageFormat_Undefined );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief カラーバッファーのイメージフォーマットを設定します。
    *
    * @param[in] value カラーバッファーのイメージフォーマット
    *
    * @post
    * - カラーバッファーのイメージフォーマットが設定されている
    *
    * @platformbegin{GL}
    * このパラメーターはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->format = static_cast< Bit32 >( value );
    }

    /**
    * @brief カラーバッファーのイメージフォーマットを取得します。
    *
    * @return カラーバッファーのイメージフォーマットを返します。
    */
    ImageFormat GetFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->format );
    }
};

/**
* @brief レンダーターゲットステートの情報を表すクラスです。
*/
class RenderTargetStateInfo
    : public detail::DataContainer< RenderTargetStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    RenderTargetStateInfo() NN_NOEXCEPT
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
    * - SetDepthStencilFormat( nn::gfx::ImageFormat_Undefined );
    * - SetColorTargetStateInfoArray( NULL, 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 深度ステンシルフォーマットを設定します。
    *
    * @param[in] value 深度ステンシルフォーマット
    *
    * @post
    * - 深度ステンシルフォーマットが設定されている
    *
    * @platformbegin{GL}
    * このパラメーターはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetDepthStencilFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->depthStencilFormat = static_cast< Bit32 >( value );
    }

    /**
    * @brief カラーターゲットステート情報の配列を設定します。
    *
    * @param[in] pColorTargetStateInfoArray カラーターゲットステート情報の配列へのポインター
    * @param[in] colorTargetStateCount カラーターゲットステートの数
    *
    * @pre
    * - colorTargetStateCount >= 0 && colorTargetStateCount <= 255
    *
    * @post
    * - カラーターゲットステート情報の配列とその数が設定されている
    *
    * @details
    * ポインター先の内容はオブジェクトの初期化後に破棄できます。
    */
    void SetColorTargetStateInfoArray( const ColorTargetStateInfo* pColorTargetStateInfoArray,
        int colorTargetStateCount ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( colorTargetStateCount >= 0 &&
            ( std::numeric_limits< uint8_t >::max )() );
        this->pColorTargetStateArray.ptr = nn::gfx::AccessorToData( pColorTargetStateInfoArray );
        this->colorTargetCount = static_cast< uint8_t >( colorTargetStateCount );
    }

    /**
    * @brief 深度ステンシルフォーマットを取得します。
    *
    * @return 深度ステンシルフォーマットを返します。
    */
    ImageFormat GetDepthStencilFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->depthStencilFormat );
    }

    /**
    * @brief カラーターゲットの数を取得します。
    *
    * @return カラーターゲットの数を返します。
    */
    int GetColorTargetCount() const NN_NOEXCEPT
    {
        return this->colorTargetCount;
    }

    /**
    * @brief カラーターゲットステート情報の配列を取得します。
    *
    * @return カラーターゲットステート情報の配列へのポインターを返します。
    */
    const ColorTargetStateInfo* GetColorTargetStateInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pColorTargetStateArray.ptr );
    }
};

/**
* @brief 頂点属性ステートの情報を表すクラスです。
*/
class VertexAttributeStateInfo
    : public detail::DataContainer< VertexAttributeStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    VertexAttributeStateInfo() NN_NOEXCEPT
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
    * - SetSemanticIndex( 0 );
    * - SetShaderSlot( -1 );
    * - SetBufferIndex( 0 );
    * - SetOffset( 0 );
    * - SetFormat( nn::gfx::AttributeFormat_Undefined );
    * - SetNamePtr( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 要素を複数とる属性についてセマンティックインデックスを設定します。
    *
    * @param[in] value 要素のセマンティックインデックス
    *
    * @pre
    * - value >= 0 && value <= 255
    *
    * @post
    * - 要素のセマンティックインデックスが設定されている
    *
    * @platformbegin{GL}
    * このパラメーターはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetSemanticIndex( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint8_t >::max )() );
        this->semanticIndex = static_cast< uint8_t >( value );
    }

    /**
    * @brief シェーダーでのスロット番号を設定します。
    *
    * @param[in] value スロット番号
    *
    * @pre
    * - value >= -1 && value <= 32767
    *
    * @post
    * - シェーダーでのスロット番号が設定されている
    *
    * @details
    * スロット番号はシェーダーから問い合わせた値を設定してください。@n
    * SetNamePtr に NULL 以外が設定されている場合は SetNamePtr の値が優先されます。@n
    * 無効なスロット番号を設定するには -1 を設定します。
    */
    void SetShaderSlot( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= -1 && value <= ( std::numeric_limits< int16_t >::max )() );
        this->shaderSlot = static_cast< int16_t >( value );
    }

    /**
    * @brief フェッチする頂点バッファーのインデックスを設定します。
    *
    * @param[in] value 頂点バッファーのインデックス
    *
    * @pre
    * - value >= 0 && value <= 65535
    *
    * @post
    * - 頂点バッファーのインデックスが設定されている
    */
    void SetBufferIndex( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 && value <= ( std::numeric_limits< uint16_t >::max )() );
        this->bufferIndex = static_cast< uint16_t >( value );
    }

    /**
    * @brief 頂点バッファーにおける先頭からのオフセットを設定します。
    *
    * @param[in] value 頂点バッファーにおける先頭からのオフセット
    *
    * @pre
    * - value >= 0 && value <= 4294967295
    *
    * @post
    * - 頂点バッファーにおける先頭からのオフセットが設定されている
    */
    void SetOffset( ptrdiff_t value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( static_cast< int64_t >( value ) >= 0 &&  static_cast< int64_t >( value ) <= ( std::numeric_limits< uint32_t >::max )() );
        this->offset = static_cast< uint32_t >( value );
    }

    /**
    * @brief 頂点属性フォーマットを設定します。
    *
    * @param[in] value 頂点属性フォーマット
    *
    * @post
    * - 頂点属性フォーマットが設定されている
    */
    void SetFormat( AttributeFormat value ) NN_NOEXCEPT
    {
        this->format = static_cast< Bit32 >( value );
    }

    /**
    * @brief シェーダーにおける頂点属性のシンボル名を設定します。
    *
    * @param[in] value 頂点属性のシンボル名へのポインター
    *
    * @post
    * - 頂点属性のシンボル名が設定されている
    *
    * @details
    * NULL を設定した場合は SetShaderSlot の値が使われます。
    */
    void SetNamePtr( const char* value ) NN_NOEXCEPT
    {
        this->pName = value;
    }

    /**
    * @brief 要素を複数とる属性についてセマンティックインデックスを取得します。
    *
    * @return セマンティックインデックスを返します。
    */
    int GetSemanticIndex() const NN_NOEXCEPT
    {
        return static_cast< int >( this->semanticIndex );
    }

    /**
    * @brief シェーダーにおけるスロット番号を取得します。
    *
    * @return シェーダーにおけるスロット番号を返します。
    */
    int GetShaderSlot() const NN_NOEXCEPT
    {
        return static_cast< int >( this->shaderSlot );
    }

    /**
    * @brief フェッチする頂点バッファーのインデックスを取得します。
    *
    * @return フェッチする頂点バッファーのインデックスを返します。
    */
    int GetBufferIndex() const NN_NOEXCEPT
    {
        return static_cast< int >( this->bufferIndex );
    }

    /**
    * @brief 頂点バッファーの先頭からのオフセットを取得します。
    *
    * @return 頂点バッファーの先頭からのオフセットを返します。
    */
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return static_cast< ptrdiff_t >( this->offset );
    }

    /**
    * @brief 頂点属性フォーマットを取得します。
    *
    * @return 頂点属性フォーマットを返します。
    */
    AttributeFormat GetFormat() const NN_NOEXCEPT
    {
        return static_cast< AttributeFormat >( this->format );
    }

    /**
    * @brief シェーダーにおけるシンボル名を取得します。
    *
    * @return シェーダーにおけるシンボル名を返します。
    */
    const char* GetNamePtr() const NN_NOEXCEPT
    {
        return this->pName;
    }
};

/**
* @brief 頂点バッファーステートの情報を表すクラスです。
*/
class VertexBufferStateInfo
    : public detail::DataContainer< VertexBufferStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    VertexBufferStateInfo() NN_NOEXCEPT
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
    * - SetStride( 0 );
    * - SetDivisor( 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief バッファーのストライドを設定します。
    *
    * @param[in] value バイトでのストライド
    *
    * @post
    * - バッファーのストライドが設定されている
    *
    * @platformbegin{GL}
    * このパラメーターはオブジェクトの初期化に影響しません。
    * @platformend
    */
    void SetStride( ptrdiff_t value ) NN_NOEXCEPT
    {
        this->stride = static_cast< int32_t >( value );
    }

    /**
    * @brief いくつの要素ごとにインスタンスを進めるかを表す、インスタンスの除数を設定します
    *
    * @param[in] value インスタンスの除数
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - インスタンスの除数が設定されている
    *
    * @details
    * 0 の値は、インスタンス単位ではなく頂点単位のデータであることを意味します。
    */
    void SetDivisor( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->divisor = static_cast< uint32_t >( value );
    }

    /**
    * @brief バッファーのストライドを取得します。
    *
    * @return バイトでのストライドを返します。
    */
    ptrdiff_t GetStride() const NN_NOEXCEPT
    {
        return static_cast< ptrdiff_t >( this->stride );
    }

    /**
    * @brief いくつの要素ごとにインスタンスを進めるかを表す、インスタンスの除数を取得します。
    *
    * @return インスタンスの除数を返します。
    *
    * @details
    * 0 の返り値は、インスタンス単位ではなく頂点単位のデータであることを意味します。
    */
    int GetDivisor() const NN_NOEXCEPT
    {
        return static_cast< int >( this->divisor );
    }
};

/**
* @brief 頂点ステートを初期化するための情報を表すクラスです。
*/
class VertexStateInfo
    : public detail::DataContainer< VertexStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    VertexStateInfo() NN_NOEXCEPT
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
    * - SetVertexAttributeStateInfoArray( NULL, 0 );
    * - SetVertexBufferStateInfoArray( NULL, 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief 頂点属性ステート情報の配列を設定します。
    *
    * @param[in] pVertexAttributeStateInfoArray 頂点属性ステート情報の配列へのポインター
    * @param[in] vertexAttributeStateCount 頂点属性ステートの数
    *
    * @pre
    * - vertexAttributeStateCount >= 0 && vertexAttributeStateCount <= 65535
    *
    * @post
    * - 頂点属性ステート情報の配列とその数が設定されている
    *
    * @details
    * ポインター先の内容はオブジェクトの初期化後に破棄できます。
    */
    void SetVertexAttributeStateInfoArray( const VertexAttributeStateInfo* pVertexAttributeStateInfoArray,
        int vertexAttributeStateCount ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( vertexAttributeStateCount >= 0 &&
            ( std::numeric_limits< uint16_t >::max )() );
        this->pAttributeArray.ptr = nn::gfx::AccessorToData( pVertexAttributeStateInfoArray );
        this->attributeCount = static_cast< uint16_t >( vertexAttributeStateCount );
    }

    /**
    * @brief 頂点バッファーステート情報の配列を設定します。
    *
    * @param[in] pVertexBufferStateInfoArray 頂点バッファーステート情報の配列へのポインター
    * @param[in] vertexBufferStateCount 頂点バッファーステートの数
    *
    * @pre
    * - vertexBufferStateCount >= 0 && vertexBufferStateCount <= 65535
    *
    * @post
    * - 頂点バッファーステート情報の配列とその数が設定されている
    *
    * @details
    * ポインター先の内容はオブジェクトの初期化後に破棄できます。
    */
    void SetVertexBufferStateInfoArray( const VertexBufferStateInfo* pVertexBufferStateInfoArray,
        int vertexBufferStateCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( vertexBufferStateCount >= 0 &&
            ( std::numeric_limits< uint16_t >::max )() );
        this->pBufferArray.ptr = nn::gfx::AccessorToData( pVertexBufferStateInfoArray );
        this->bufferCount = static_cast< uint16_t >( vertexBufferStateCount );
    }

    /**
    * @brief 頂点属性の数を取得します。
    *
    * @return 頂点属性の数を返します。
    */
    int GetVertexAttributeCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->attributeCount );
    }

    /**
    * @brief 頂点バッファーの数を取得します。
    *
    * @return 頂点バッファーの数を返します。
    */
    int GetVertexBufferCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->bufferCount );
    }

    /**
    * @brief 頂点属性ステート情報の配列を取得します。
    *
    * @return 頂点属性ステート情報の配列へのポインターを返します。
    */
    const VertexAttributeStateInfo* GetVertexAttributeStateInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pAttributeArray.ptr );
    }

    /**
    * @brief 頂点バッファーステート情報の配列を取得します。
    *
    * @return 頂点バッファーステート情報の配列へのポインターを返します。
    */
    const VertexBufferStateInfo* GetVertexBufferStateInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pBufferArray.ptr );
    }
};

/**
* @brief テッセレーションステートを表すクラスです。
*/
class TessellationStateInfo
    : public detail::DataContainer< TessellationStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TessellationStateInfo() NN_NOEXCEPT
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
    * - SetPatchControlPointCount( 1 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief パッチの制御点の数を設定します。
    *
    * @param[in] value パッチの制御点の数
    *
    * @pre
    * - value >= 0 && value < 65535
    *
    * @post
    * - パッチの制御点の数が設定されている
    */
    void SetPatchControlPointCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value >= 0 && value < ( std::numeric_limits< uint16_t >::max )() );
        this->patchControlPointCount = static_cast< uint16_t >( value );
    }

    /**
    * @brief パッチの制御点の数を取得します。
    *
    * @return パッチの制御点の数を返します。
    */
    int GetPatchControlPointCount() const NN_NOEXCEPT
    {
        return this->patchControlPointCount;
    }
};

/**
* @brief ビューポート情報を表すクラスです。
*/
class ViewportStateInfo
    : public detail::DataContainer< ViewportStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ViewportStateInfo() NN_NOEXCEPT
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
    * - SetOriginX( 0.0f );
    * - SetOriginY( 0.0f );
    * - SetWidth( 0.0f );
    * - SetHeight( 0.0f );
    * - SetMinDepth( 0.0f );
    * - SetMaxDepth( 1.0f );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief ビューポートの X 座標の原点を設定します。
    *
    * @param[in] value ビューポートの X 座標の原点
    *
    * @post
    * - ビューポートの X 座標の原点が設定されている
    */
    void SetOriginX( float value ) NN_NOEXCEPT
    {
        this->originX = value;
    }

    /**
    * @brief ビューポートの Y 座標の原点を設定します。
    *
    * @param[in] value ビューポートの Y 座標の原点
    *
    * @post
    * - ビューポートの Y 座標の原点が設定されている
    */
    void SetOriginY( float value ) NN_NOEXCEPT
    {
        this->originY = value;
    }

    /**
    * @brief ビューポートの幅を設定します。
    *
    * @param[in] value ビューポートの幅
    *
    * @post
    * - ビューポートの幅が設定されている
    */
    void SetWidth( float value ) NN_NOEXCEPT
    {
        this->width = value;
    }

    /**
    * @brief ビューポートの高さを設定します。
    *
    * @param[in] value ビューポートの高さ
    *
    * @post
    * - ビューポートの高さが設定されている
    */
    void SetHeight( float value ) NN_NOEXCEPT
    {
        this->height = value;
    }

    /**
    * @brief ビューポートの最小深度を設定します。
    *
    * @param[in] value ビューポートの最小深度
    *
    * @post
    * - ビューポートの最小深度が設定されている
    */
    void SetMinDepth( float value ) NN_NOEXCEPT
    {
        this->depthRange.minDepth = value;
    }

    /**
    * @brief ビューポートの最大深度を設定します。
    *
    * @param[in] value ビューポートの最大深度
    *
    * @post
    * - ビューポートの最大深度が設定されている
    */
    void SetMaxDepth( float value ) NN_NOEXCEPT
    {
        this->depthRange.maxDepth = value;
    }

    /**
    * @brief ビューポートの X 座標の原点を取得します。
    *
    * @return ビューポートの X 座標の原点を返します。
    */
    float GetOriginX() const NN_NOEXCEPT
    {
        return this->originX;
    }

    /**
    * @brief ビューポートの Y 座標の原点を取得します。
    *
    * @return ビューポートの Y 座標の原点を返します。
    */
    float GetOriginY() const NN_NOEXCEPT
    {
        return this->originY;
    }

    /**
    * @brief ビューポートの幅を取得します。
    *
    * @return ビューポートの幅を返します。
    */
    float GetWidth() const NN_NOEXCEPT
    {
        return this->width;
    }

    /**
    * @brief ビューポートの高さを取得します。
    *
    * @return ビューポートの高さを返します。
    */
    float GetHeight() const NN_NOEXCEPT
    {
        return this->height;
    }

    /**
    * @brief ビューポートの最小深度を取得します。
    *
    * @return ビューポートの最小深度を返します。
    */
    float GetMinDepth() const NN_NOEXCEPT
    {
        return this->depthRange.minDepth;
    }

    /**
    * @brief ビューポートの最大深度を取得します。
    *
    * @return ビューポートの最大深度を返します。
    */
    float GetMaxDepth() const NN_NOEXCEPT
    {
        return this->depthRange.maxDepth;
    }
};

/**
* @brief シザリングステート情報を表すクラスです。
*/
class ScissorStateInfo
    : public detail::DataContainer< ScissorStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ScissorStateInfo() NN_NOEXCEPT
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
    * - SetOriginX( 0 );
    * - SetOriginY( 0 );
    * - SetWidth( 0 );
    * - SetHeight( 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief シザリングの X 座標の原点を設定します。
    *
    * @param[in] value シザリングの X 座標の原点
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - シザリングの X 座標の原点が設定されている
    */
    void SetOriginX( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->originX = static_cast< uint32_t >( value );
    }

    /**
    * @brief シザリングの Y 座標の原点を設定します。
    *
    * @param[in] value シザリングの Y 座標の原点
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - シザリングの Y 座標の原点が設定されている
    */
    void SetOriginY( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->originY = static_cast< uint32_t >( value );
    }

    /**
    * @brief シザリングの幅を設定します。
    *
    * @param[in] value シザリングの幅
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - シザリングの幅が設定されている
    */
    void SetWidth( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->width = static_cast< uint32_t >( value );
    }

    /**
    * @brief シザリングの高さを設定します。
    *
    * @param[in] value シザリングの高さ
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - シザリングの高さが設定されている
    */
    void SetHeight( int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value >= 0 );
        this->height = static_cast< uint32_t >( value );
    }

    /**
    * @brief シザリングの X 座標の原点を取得します。
    *
    * @return シザリングの X 座標の原点を返します。
    */
    int GetOriginX() const NN_NOEXCEPT
    {
        return static_cast< int >( this->originX );
    }

    /**
    * @brief シザリングの Y 座標の原点を取得します。
    *
    * @return シザリングの Y 座標の原点を返します。
    */
    int GetOriginY() const NN_NOEXCEPT
    {
        return static_cast< int >( this->originY );
    }

    /**
    * @brief シザリングの幅を取得します。
    *
    * @return シザリングの幅を返します。
    */
    int GetWidth() const NN_NOEXCEPT
    {
        return static_cast< int >( this->width );
    }

    /**
    * @brief シザリングの高さを取得します。
    *
    * @return シザリングの高さを返します。
    */
    int GetHeight() const NN_NOEXCEPT
    {
        return static_cast< int >( this->height );
    }
};

/**
* @brief ビューポートシザリングステートを初期化するための情報を表すクラスです。
*/
class ViewportScissorStateInfo
    : public detail::DataContainer< ViewportScissorStateInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ViewportScissorStateInfo() NN_NOEXCEPT
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
    * - SetScissorEnabled( false );
    * - SetViewportStateInfoArray( NULL, 0 );
    * - SetScissorStateInfoArray( NULL, 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief シザリングを有効または無効に設定します。
    *
    * @param[in] value シザリングの有効・無効を表す真偽値
    *
    * @post
    * - シザリングの有効・無効が設定されている
    *
    * @details
    * ラスタライザステートのシザリング有効・無効と一致している必要があります。
    */
    void SetScissorEnabled( bool value ) NN_NOEXCEPT
    {
        this->flag.SetBit( Flag_ScissorEnable, value );
    }

    /**
    * @brief ビューポートステート情報の配列を設定します。
    *
    * @param[in] pViewportStateInfoArray ビューポートステート情報の配列へのポインター
    * @param[in] viewportStateCount ビューポートステートの数
    *
    * @pre
    * - viewportStateCount >= 0 && viewportStateCount <= 65535
    *
    * @post
    * - ビューポートステート情報の配列とその数が設定されている
    *
    * @details
    * ポインター先の内容はオブジェクトの初期化後に破棄できます。
    * シザリングが有効の場合、ビューポートステートの数はシザーステートの数と等しい必要があります。
    */
    void SetViewportStateInfoArray( const ViewportStateInfo* pViewportStateInfoArray,
        int viewportStateCount ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( viewportStateCount >= 0 &&
            viewportStateCount <= ( std::numeric_limits< uint16_t >::max )() );
        this->pViewportArray.ptr = nn::gfx::AccessorToData( pViewportStateInfoArray );
        this->viewportCount = static_cast< uint16_t >( viewportStateCount );
    }

    /**
    * @brief シザーステート情報の配列を設定します。
    *
    * @param[in] pScissorStateInfoArray シザーステート情報の配列へのポインター
    * @param[in] scissorStateCount シザーステートの数
    *
    * @pre
    * - scissorStateCount >= 0 && scissorStateCount <= 65535
    *
    * @post
    * - シザーステート情報の配列とその数が設定されている
    *
    * @details
    * ポインター先の内容はオブジェクトの初期化後に破棄できます。
    * シザリングが有効の場合、シザーステートの数はビューポートステートの数と等しい必要があります。
    */
    void SetScissorStateInfoArray( const ScissorStateInfo* pScissorStateInfoArray,
        int scissorStateCount ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( scissorStateCount >= 0 &&
            scissorStateCount <= ( std::numeric_limits< uint16_t >::max )() );
        this->pScissorArray.ptr = nn::gfx::AccessorToData( pScissorStateInfoArray );
        this->scissorCount = static_cast< uint16_t >( scissorStateCount );
    }

    /**
    * @brief シザリングが有効かどうかを取得します。
    *
    * @return シザリングが有効かどうかを表す真偽値を返します。
    */
    bool IsScissorEnabled() const NN_NOEXCEPT
    {
        return this->flag.GetBit( Flag_ScissorEnable );
    }

    /**
    * @brief ビューポートの数を取得します。
    *
    * @return ビューポートの数を返します。
    */
    int GetViewportCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->viewportCount );
    }

    /**
    * @brief シザーの数を取得します。
    *
    * @return シザーの数を返します。
    */
    int GetScissorCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->scissorCount );
    }

    /**
    * @brief ビューポートステート情報の配列を取得します。
    *
    * @return ビューポートステート情報の配列へのポインターを返します。
    */
    const ViewportStateInfo* GetViewportStateInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pViewportArray.ptr );
    }

    /**
    * @brief シザリングステート情報の配列を取得します。
    *
    * @return シザリングステート情報の配列へのポインターを返します。
    */
    const ScissorStateInfo* GetScissorStateInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pScissorArray.ptr );
    }
};

}
}
