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
* @brief バッファー初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_BufferInfoData.h>

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/detail/gfx_Declare.h>

#if !defined( NN_GFX_CONFIG_DISABLE_TINFO )

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_BufferInfo-api.nvn.8.h>
#endif

#endif

namespace nn {
namespace gfx {

/**
* @brief バッファーを初期化するための情報を表すクラスです。
*/
class BufferInfo
    : public detail::DataContainer< BufferInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    BufferInfo() NN_NOEXCEPT
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
    * - SetSize( 0 );
    * - SetGpuAccessFlags( 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief バッファーのサイズを設定します。
    *
    * @param[in] value バイトでのバッファーのサイズ
    *
    * @post
    * - バッファーのサイズが設定されている
    */
    void SetSize( size_t value ) NN_NOEXCEPT
    {
        this->size = static_cast< uint32_t >( value );
    }

    /**
    * @brief GPU がアクセスする方法を設定します。
    *
    * @param[in] value nn::gfx::GpuAccess の組み合わせによるアクセスフラグ
    *
    * @post
    * - GPU がアクセスする方法が設定されている
    */
    void SetGpuAccessFlags( int value ) NN_NOEXCEPT
    {
        this->gpuAccessFlag = static_cast< Bit32 >( value );
    }

    /**
    * @brief バッファーのサイズを取得します。
    *
    * @return バッファーのサイズを返します。
    */
    size_t GetSize() const NN_NOEXCEPT
    {
        return static_cast< size_t >( this->size );
    }

    /**
    * @brief GPU がアクセスする方法を取得します。
    *
    * @return GPU がアクセスする方法を返します。
    */
    int GetGpuAccessFlags() const NN_NOEXCEPT
    {
        return static_cast< int >( this->gpuAccessFlag );
    }
};

/**
* @brief バッファーテクスチャービューを初期化するための情報を表すクラスです。
*/
class BufferTextureViewInfo
    : public detail::DataContainer< BufferTextureViewInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    BufferTextureViewInfo()
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
    * - SetImageFormat( ImageFormat_Undefined );
    * - SetOffset( 0 );
    * - SetSize( 0 );
    * - SetBufferPtr( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief バッファーの内容を解釈するイメージフォーマットを設定します。
    *
    * @param[in] value イメージフォーマット
    *
    * @post
    * - バッファーの内容を解釈するイメージフォーマットが設定されている。
    */
    void SetImageFormat( ImageFormat value ) NN_NOEXCEPT
    {
        this->format = static_cast<Bit32>( value );
    }

    /**
    * @brief 参照するバッファーの先頭からのオフセットを設定します。
    *
    * @param[in] value バイトでのバッファーの先頭からのオフセット
    *
    * @post
    * - 参照するバッファーの先頭からのオフセットが設定されている。
    */
    void SetOffset( ptrdiff_t value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value >= 0 );
        this->offset = static_cast< uint32_t >( value );
    }

    /**
    * @brief 参照するバッファーのサイズを設定します。
    *
    * @param[in] value バイトでの参照するサイズ
    *
    * @post
    * - 参照するバッファーのサイズが設定されている。
    */
    void SetSize( size_t value ) NN_NOEXCEPT
    {
        this->size = static_cast< uint32_t >( value );
    }

    /**
    * @brief 参照するバッファーを設定します。
    *
    * @tparam TTarget バッファーの低レベルグラフィックス API
    *
    * @param[in] value 参照するバッファーへのポインター
    *
    * @post
    * - 参照するバッファーが設定されている
    *
    * @details
    * TTarget は初期化するつもりのバッファーテクスチャービューの TTarget と同じである必要があります。
    */
    template< typename TTarget >
    void SetBufferPtr( const TBuffer< TTarget >* value ) NN_NOEXCEPT
    {
        this->pBuffer = value;
    }

    /**
    * @brief 参照するバッファーを設定します。
    *
    * @param[in] value 参照するバッファーへのポインター
    *
    * @post
    * - 参照するバッファーが設定されている
    *
    * @details
    * NULL 以外の値を設定する場合はテンプレート引数付きのほうを呼んでください。
    */
    void SetBufferPtr( const void* value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value == NULL );
        this->pBuffer = value;
    }

    /**
    * @brief バッファーの内容を解釈するイメージフォーマットを取得します。
    *
    * @return バッファーの内容を解釈するイメージフォーマットを返します。
    */
    ImageFormat GetImageFormat() const NN_NOEXCEPT
    {
        return static_cast< ImageFormat >( this->format );
    }

    /**
    * @brief 参照するバッファーの先頭からのオフセットを取得します。
    *
    * @return 参照するバッファーの先頭からのバイトでのオフセットを返します。
    */
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return this->offset;
    }

    /**
    * @brief 参照するバッファーのサイズを取得します。
    *
    * @return 参照するバッファーのバイトでのサイズを返します。
    */
    size_t GetSize() const NN_NOEXCEPT
    {
        return this->size;
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief 参照するバッファーを取得します。
    *
    * @return nn::gfx::Buffer 型の参照するバッファーへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Buffer 型のポインターに代入してください
    */
    const Buffer* GetBufferPtr() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetBufferPtr() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pBuffer.ptr );
    }
#endif
};

/**
* @brief バッファーを初期化するための情報を表すグラフィックス抽象レイヤーです。
*/
template< typename TTarget >
class TBufferInfo
    : public detail::TargetInfoImpl< detail::BufferInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, BufferInfo >::Type
{
    typedef typename detail::TargetInfoImpl< detail::BufferInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, BufferInfo >::Type Impl;

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
    static void ConvertFromInfo( TBufferInfo< TTarget >* pDstInfo, const BufferInfo& info ) NN_NOEXCEPT
    {
        return detail::InfoHelper::ConvertFromInfo< Impl >( pDstInfo, info );
    }

    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TBufferInfo() NN_NOEXCEPT
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
    * - SetSize( 0 );
    * - SetGpuAccessFlags( 0 );
    */
    void SetDefault() NN_NOEXCEPT
    {
        return Impl::SetDefault();
    }

    /**
    * @brief バッファーのサイズを設定します。
    *
    * @param[in] value バイトでのバッファーのサイズ
    *
    * @post
    * - バッファーのサイズが設定されている
    */
    void SetSize( size_t value ) NN_NOEXCEPT
    {
        return Impl::SetSize( value );
    }

    /**
    * @brief GPU がアクセスする方法を設定します。
    *
    * @param[in] value nn::gfx::GpuAccess の組み合わせによるアクセスフラグ
    *
    * @post
    * - GPU がアクセスする方法が設定されている
    */
    void SetGpuAccessFlags( int value ) NN_NOEXCEPT
    {
        return Impl::SetGpuAccessFlags( value );
    }
};

}
}
