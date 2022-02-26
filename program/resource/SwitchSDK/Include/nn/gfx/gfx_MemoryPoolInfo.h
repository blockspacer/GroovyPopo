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
* @brief メモリープール初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_MemoryPoolInfoData.h>

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

#if !defined( NN_GFX_CONFIG_DISABLE_TINFO )

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_MemoryPoolInfo-api.nvn.8.h>
#endif

#endif

namespace nn {
namespace gfx {

/**
* @brief メモリープールを初期化するための情報を表すクラスです
*/
class MemoryPoolInfo
    : public detail::DataContainer< MemoryPoolInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    MemoryPoolInfo() NN_NOEXCEPT
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
    * - SetMemoryPoolProperty( nn::gfx::MemoryPoolProperty_CpuUncached | nn::gfx::MemoryPoolProperty_GpuCached );
    * - SetPoolMemory( NULL, 0 );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief メモリープール特性を設定します。
    *
    * @param[in] value nn::gfx::MemoryPoolProperty の組み合わせによるメモリープール特性
    *
    * @post
    * - メモリープール特性が設定されている
    *
    * @details
    * 有効な値を指定するには、nn::gfx::MemoryPoolProperty_CpuInvisible、nn::gfx::MemoryPoolProperty_CpuUncached、nn::gfx::MemoryPoolProperty_CpuCached の中からひとつと、nn::gfx::MemoryPoolProperty_GpuInvisible、nn::gfx::MemoryPoolProperty_GpuUncached、nn::gfx::MemoryPoolProperty_GpuCached の中からひとつを指定する必要があります。
    */
    void SetMemoryPoolProperty( int value ) NN_NOEXCEPT
    {
        this->memoryPoolProperty = static_cast< Bit32 >( value );
    }

    /**
    * @brief プール用のメモリーを設定します
    *
    * @param[in] value メモリーへのポインター
    * @param[in] size バイトでのメモリーサイズ
    *
    * @post
    * - プール用のメモリーが設定されている
    */
    void SetPoolMemory( void* value, size_t size ) NN_NOEXCEPT
    {
        this->pMemory = value;
        this->memorySize = static_cast< uint32_t >( size );
    }

    /**
    * @brief メモリープール特性を取得します。
    *
    * @return nn::gfx::MemoryPoolProperty の組み合わせによるメモリープール特性を返します。
    */
    int GetMemoryPoolProperty() const NN_NOEXCEPT
    {
        return static_cast< int >( this->memoryPoolProperty );
    }

    /**
    * @brief プール用のメモリーを取得します
    *
    * @return プール用のメモリーへのポインターを返します
    */
    void* GetPoolMemory() const NN_NOEXCEPT
    {
        return this->pMemory;
    }

    /**
    * @brief プール用のメモリーのサイズを取得します
    *
    * @return プール用のメモリーのサイズを返します
    */
    size_t GetPoolMemorySize() const NN_NOEXCEPT
    {
        return this->memorySize;
    }
};

/**
* @brief メモリープールを初期化するための情報を表すグラフィックス抽象レイヤーです。
*/
template< typename TTarget >
class TMemoryPoolInfo
    : public detail::TargetInfoImpl< detail::MemoryPoolInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, MemoryPoolInfo >::Type
{
    typedef typename detail::TargetInfoImpl< detail::MemoryPoolInfoImpl<
        typename detail::TargetVariation< TTarget >::Type >, MemoryPoolInfo >::Type Impl;

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
    static void ConvertFromInfo( TMemoryPoolInfo< TTarget >* pDstInfo, const MemoryPoolInfo& info ) NN_NOEXCEPT
    {
        return detail::InfoHelper::ConvertFromInfo< Impl >( pDstInfo, info );
    }

    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    TMemoryPoolInfo() NN_NOEXCEPT
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
    * - SetMemoryPoolProperty( nn::gfx::MemoryPoolProperty_CpuUncached | nn::gfx::MemoryPoolProperty_GpuCached );
    * - SetPoolMemory( NULL, 0 );
    */
    void SetDefault() NN_NOEXCEPT
    {
        return Impl::SetDefault();
    }

    /**
    * @brief メモリープール特性を設定します。
    *
    * @param[in] value nn::gfx::MemoryPoolProperty の組み合わせによるメモリープール特性
    *
    * @post
    * - メモリープール特性が設定されている
    *
    * @details
    * 有効な値を指定するには、nn::gfx::MemoryPoolProperty_CpuInvisible、nn::gfx::MemoryPoolProperty_CpuUncached、nn::gfx::MemoryPoolProperty_CpuCached の中からひとつと、nn::gfx::MemoryPoolProperty_GpuInvisible、nn::gfx::MemoryPoolProperty_GpuUncached、nn::gfx::MemoryPoolProperty_GpuCached の中からひとつを指定する必要があります。
    */
    void SetMemoryPoolProperty( int value ) NN_NOEXCEPT
    {
        return Impl::SetMemoryPoolProperty( value );
    }

    /**
    * @brief プール用のメモリーを設定します
    *
    * @param[in] value メモリーへのポインター
    * @param[in] size バイトでのメモリーサイズ
    *
    * @post
    * - プール用のメモリーが設定されている
    */
    void SetPoolMemory( void* value, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetPoolMemory( value, size );
    }
};

}
}
