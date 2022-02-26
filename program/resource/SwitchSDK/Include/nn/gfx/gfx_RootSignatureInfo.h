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
* @brief ルートシグネチャ初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DataAccessorConverter.h>
#include <nn/gfx/gfx_RootSignatureInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief デスクリプター範囲を表すクラスです。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
class NN_DEPRECATED DescriptorRangeInfo
    : public detail::DataContainer< DescriptorRangeInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DescriptorRangeInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 設定される値はゼロ初期化とは異なります。
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief デスクリプタースロットの種類を設定します。
    *
    * @param[in] value デスクリプタースロットの種類
    *
    * @post
    * - デスクリプタースロットの種類が設定されている
    */
    void SetDescriptorSlotType( DescriptorSlotType value ) NN_NOEXCEPT
    {
        this->descriptorSlotType = static_cast< Bit8 >( value );
    }

    /**
    * @brief デスクリプタースロットの数を設定します。
    *
    * @param[in] value デスクリプタースロットの数
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - デスクリプタースロットの数が設定されている
    */
    void SetDescriptorSlotCount( int value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value >= 0 );
        this->descriptorSlotCount = static_cast< uint32_t >( value );
    }

    /**
    * @brief シェーダースロットのベース値を設定します。
    *
    * @param[in] value シェーダースロットのベース値
    *
    * @post
    * - シェーダースロットのベース値が設定されている
    *
    * @details
    * N 個目のデスクリプタースロットに対して value + N のシェーダースロット番号が適用されます。
    */
    void SetBaseShaderSlot( int value ) NN_NOEXCEPT
    {
        this->baseShaderSlot = value;
    }

    /**
    * @brief バッファーのデスクリプターテーブルの先頭からのスロットのオフセットを設定します。
    *
    * @param[in] value バッファーのスロットのオフセット値
    *
    * @pre
    * - value >= 0
    *
    * @post
    * - バッファーのデスクリプターテーブルの先頭からのスロットのオフセットが設定されている。
    */
    void SetBufferDescriptorSlotOffset( int value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value >= 0 );
        this->bufferDescriptorSlotOffset = static_cast< uint32_t >( value );
    }

    /**
    * @brief テクスチャーとサンプラーのデスクリプターテーブルの先頭からのスロットのオフセットを設定します。
    *
    * @param[in] textureDescriptorSlotOffset テクスチャーのスロットのオフセット値
    * @param[in] samplerDescriptorSlotOffset サンプラーのスロットのオフセット値
    *
    * @pre
    * - textureDescriptorSlotOffset >= 0
    * - samplerDescriptorSlotOffset >= 0
    *
    * @post
    * - テクスチャーとサンプラーのデスクリプターテーブルの先頭からのスロットのオフセットが設定されている。
    */
    void SetTextureSamplerDescriptorSlotOffset( int textureDescriptorSlotOffset, int samplerDescriptorSlotOffset ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( textureDescriptorSlotOffset >= 0 );
        NN_SDK_ASSERT( samplerDescriptorSlotOffset >= 0 );
        this->textureSamplerDescriptorSlotOffset.textureDescriptorSlotOffset = static_cast< uint32_t >( textureDescriptorSlotOffset );
        this->textureSamplerDescriptorSlotOffset.samplerDescriptorSlotOffset = static_cast< uint32_t >( samplerDescriptorSlotOffset );
    }

    /**
    * @brief デスクリプタースロットの種類を取得します。
    *
    * @return デスクリプタースロットの種類を返します。
    */
    DescriptorSlotType GetDescriptorSlotType() const NN_NOEXCEPT
    {
        return static_cast< DescriptorSlotType >( this->descriptorSlotType );
    }

    /**
    * @brief デスクリプタースロットの数を取得します。
    *
    * @return デスクリプタースロットの数を返します。
    */
    int GetDescriptorSlotCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->descriptorSlotCount );
    }

    /**
    * @brief シェーダースロットのベース値を取得します。
    *
    * @return シェーダースロットのベース値を返します。
    */
    int GetBaseShaderSlot() const NN_NOEXCEPT
    {
        return this->baseShaderSlot;
    }

    /**
    * @brief バッファーのデスクリプターテーブルの先頭からのスロットのオフセットを取得します。
    *
    * @return バッファーのデスクリプターテーブルの先頭からのスロットのオフセットを返します。
    */
    int GetBufferDescriptorSlotOffset() const NN_NOEXCEPT
    {
        return static_cast< int >( this->bufferDescriptorSlotOffset );
    }

    /**
    * @brief テクスチャーのデスクリプターテーブルの先頭からのスロットのオフセットを取得します。
    *
    * @return テクスチャーのデスクリプターテーブルの先頭からのスロットのオフセットを返します。
    */
    int GetTextureDescriptorSlotOffset() const NN_NOEXCEPT
    {
        return static_cast< int >( this->textureSamplerDescriptorSlotOffset.textureDescriptorSlotOffset );
    }

    /**
    * @brief サンプラーのデスクリプターテーブルの先頭からのスロットのオフセットを取得します。
    *
    * @return サンプラーのデスクリプターテーブルの先頭からのスロットのオフセットを返します。
    */
    int GetSamplerDescriptorSlotOffset() const NN_NOEXCEPT
    {
        return static_cast< int >( this->textureSamplerDescriptorSlotOffset.samplerDescriptorSlotOffset );
    }
};

/**
* @brief デスクリプターテーブルを表すクラスです。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
class NN_DEPRECATED DescriptorTableInfo
    : public detail::DataContainer< DescriptorTableInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DescriptorTableInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 設定される値はゼロ初期化とは異なります。
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief シェーダーステージを設定します。
    *
    * @param[in] value シェーダーステージ
    *
    * @post
    * - シェーダーステージが設定されている
    */
    void SetShaderStage( ShaderStage value ) NN_NOEXCEPT
    {
        this->shaderStage = static_cast< Bit8 >( value );
    }

    /**
    * @brief デスクリプター範囲の配列を設定します。
    *
    * @param[in] pDescriptorRangeInfoArray デスクリプター範囲の配列へのポインター
    * @param[in] descriptorRangeInfoCount デスクリプター範囲の数
    *
    * @pre
    * - descriptorRangeInfoCount >= 0
    *
    * @post
    * - デスクリプター範囲の配列とその数が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetDescriptorRangeInfoArray( const DescriptorRangeInfo* pDescriptorRangeInfoArray,
        int descriptorRangeInfoCount ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( descriptorRangeInfoCount >= 0 );
        this->pDescriptorRangeArray.ptr = nn::gfx::AccessorToData( pDescriptorRangeInfoArray );
        this->descriptorRangeCount = descriptorRangeInfoCount;
    }

    /**
    * @brief シェーダーステージを取得します。
    *
    * @return シェーダーステージを返します。
    */
    ShaderStage GetShaderStage() const NN_NOEXCEPT
    {
        return static_cast< ShaderStage >( this->shaderStage );
    }

    /**
    * @brief デスクリプター範囲の配列へのポインターを取得します。
    *
    * @return デスクリプター範囲の配列へのポインターを返します。
    */
    const DescriptorRangeInfo* GetDescriptorRangeInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pDescriptorRangeArray.ptr );
    }

    /**
    * @brief デスクリプター範囲の数を取得します。
    *
    * @return デスクリプター範囲の数を返します。
    */
    int GetDescriptorRangeCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->descriptorRangeCount );
    }
};

/**
* @brief 動的なデスクリプターを表すクラスです。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
class NN_DEPRECATED DynamicDescriptorInfo
    : public detail::DataContainer< DynamicDescriptorInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    DynamicDescriptorInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 設定される値はゼロ初期化とは異なります。
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief シェーダーステージを設定します。
    *
    * @param[in] value シェーダーステージ
    *
    * @post
    * - シェーダーステージが設定されている
    */
    void SetShaderStage( ShaderStage value ) NN_NOEXCEPT
    {
        this->shaderStage = static_cast< Bit8 >( value );
    }

    /**
    * @brief シェーダーのスロット番号を設定します。
    *
    * @param[in] value シェーダーのスロット番号
    *
    * @post
    * - シェーダーのスロット番号が設定されている
    */
    void SetShaderSlot( int value ) NN_NOEXCEPT
    {
        this->shaderSlot = value;
    }

    /**
    * @brief シェーダーステージを取得します。
    *
    * @return シェーダーステージを返します。
    */
    ShaderStage GetShaderStage() const NN_NOEXCEPT
    {
        return static_cast< ShaderStage>( this->shaderStage );
    }

    /**
    * @brief シェーダーのスロット番号を取得します。
    *
    * @return シェーダーのスロット番号を返します。
    */
    int GetShaderSlot() const NN_NOEXCEPT
    {
        return this->shaderSlot;
    }
};

/**
* @brief ルートシグネチャーを初期化するための情報を表すクラスです。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*/
class NN_DEPRECATED RootSignatureInfo
    : public detail::DataContainer< RootSignatureInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    RootSignatureInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 設定される値はゼロ初期化とは異なります。
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief デスクリプターテーブルの配列を設定します。
    *
    * @param[in] pDescriptorTableInfoArray デスクリプターテーブルの配列へのポインター
    * @param[in] descriptorTableInfoCount デスクリプターテーブルの数
    *
    * @pre
    * - descriptorTableInfoCount >= 0
    *
    * @post
    * - デスクリプターテーブルの配列とその数が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetDescriptorTableInfoArray( const DescriptorTableInfo* pDescriptorTableInfoArray,
        int descriptorTableInfoCount ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( descriptorTableInfoCount >= 0 );
        this->pDescriptorTableArray.ptr = nn::gfx::AccessorToData( pDescriptorTableInfoArray );
        this->descriptorTableCount = static_cast< uint32_t >( descriptorTableInfoCount );
    }

    /**
    * @brief 動的なデスクリプターの配列を設定します。
    *
    * @param[in] pDynamicDescriptorInfoArray 動的なデスクリプターの配列へのポインター
    * @param[in] dynamicDescriptorInfoCount 動的なデスクリプターの数
    *
    * @pre
    * - dynamicDescriptorInfoCount >= 0
    *
    * @post
    * - 動的なデスクリプターの配列とその数が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetDynamicDescriptorInfoArray( const DynamicDescriptorInfo* pDynamicDescriptorInfoArray,
        int dynamicDescriptorInfoCount ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( dynamicDescriptorInfoCount >= 0 );
        this->pDynamicDescriptorArray.ptr = nn::gfx::AccessorToData( pDynamicDescriptorInfoArray );
        this->dynamicDescriptorCount = static_cast< uint32_t >( dynamicDescriptorInfoCount );
    }

    /**
    * @brief デスクリプターテーブルの配列を取得します。
    *
    * @return デスクリプターテーブルの配列へのポインターを返します。
    */
    const DescriptorTableInfo* GetDescriptorTableInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pDescriptorTableArray.ptr );
    }

    /**
    * @brief デスクリプターテーブルの数を取得します。
    *
    * @return デスクリプターテーブルの数を返します。
    */
    int GetDescriptorTableCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->descriptorTableCount );
    }

    /**
    * @brief 動的なデスクリプターの配列を取得します。
    *
    * @return 動的なデスクリプターの配列へのポインターを返します。
    */
    const DynamicDescriptorInfo* GetDynamicDescriptorInfoArray() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pDynamicDescriptorArray.ptr );
    }

    /**
    * @brief 動的なデスクリプターの数を取得します。
    *
    * @return 動的なデスクリプターの数を返します。
    */
    int GetDynamicDescriptorCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->dynamicDescriptorCount );
    }
};

}
}

NN_PRAGMA_POP_WARNINGS
