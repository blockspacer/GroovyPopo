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
* @brief シェーダーに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_ShaderInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Shader-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Shader-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Shader-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Shader-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief シェーダーを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TShader
    : public detail::ShaderImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TShader );

    typedef detail::ShaderImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief シェーダーを初期化するための情報クラスです。
    */
    typedef ShaderInfo InfoType;

    /**
    * @brief バイナリーコードに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @return バイナリーコードに要求されるアライメントを返します。
    */
    static size_t GetBinaryCodeAlignment( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::GetBinaryCodeAlignment( pDevice );
    }

    /**
    * @brief コンストラクターです。
    */
    TShader() NN_NOEXCEPT
    {
    }

    /**
    * @brief シェーダーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return シェーダーの初期化の結果を返します。
    * @retval ShaderInitializeResult_Success 成功しました。
    * @retval ShaderInitializeResult_InvalidFormat info に設定されているシェーダーフォーマットがこの環境では無効です。
    * @retval ShaderInitializeResult_SetupFailed コンパイル失敗などによりシェーダーのセットアップに失敗しました。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - シェーダーが初期化されていない
    *
    * @post
    * - シェーダーが初期化されている
    */
    ShaderInitializeResult Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief シェーダーを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - シェーダーが初期化されている
    *
    * @post
    * - シェーダーが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief シェーダーインターフェイスのスロット番号を取得します。
    *
    * @param[in] stage 対象のシェーダーステージ
    * @param[in] shaderInterfaceType 対象のインターフェイスタイプ
    * @param[in] pName 対象のインターフェイスの名前へのポインター
    *
    * @pre
    * - シェーダーが初期化されている
    * - pName != NULL
    *
    * @return
    * 指定されたインターフェイスのスロット番号を返します。指定されたインターフェイスが見つからなかった場合は負の値を返します。
    *
    * @details
    * シェーダーソースコード内で明示的に書かれている値とは異なる値が返る可能性があります。@n
    * リソースをセットする際などは、この関数で取得された値を指定してください。
    */
    int GetInterfaceSlot( ShaderStage stage, ShaderInterfaceType shaderInterfaceType,
        const char* pName ) const NN_NOEXCEPT
    {
        return Impl::GetInterfaceSlot( stage, shaderInterfaceType, pName );
    }

    /**
    * @brief 演算シェーダーのワークグループサイズを取得します。
    *
    * @param[out] pOutWorkGroupSizeX 取得される演算シェーダーのワークグループの X 次元のサイズを格納する領域へのポインター
    * @param[out] pOutWorkGroupSizeY 取得される演算シェーダーのワークグループの Y 次元のサイズを格納する領域へのポインター
    * @param[out] pOutWorkGroupSizeZ 取得される演算シェーダーのワークグループの Z 次元のサイズを格納する領域へのポインター
    *
    * @pre
    * - シェーダーが初期化されている
    * - pOutWorkGroupSizeX != NULL
    * - pOutWorkGroupSizeY != NULL
    * - pOutWorkGroupSizeZ != NULL
    */
    void GetWorkGroupSize( int* pOutWorkGroupSizeX, int* pOutWorkGroupSizeY,
        int* pOutWorkGroupSizeZ ) const NN_NOEXCEPT
    {
        return Impl::GetWorkGroupSize( pOutWorkGroupSizeX, pOutWorkGroupSizeY, pOutWorkGroupSizeZ );
    }

    /**
    * @brief 任意のユーザーポインターを設定します。
    *
    * @param[in] value 設定するユーザーポインター
    *
    * @post
    * - ユーザーポインターが設定されている
    */
    void SetUserPtr( void* value ) NN_NOEXCEPT
    {
        return SetUserPtrImpl( this, value );
    }

    /**
    * @brief 設定されている任意のユーザーポインターを取得します。
    *
    * @return 設定されている任意のユーザーポインターを返します。
    */
    void* GetUserPtr() NN_NOEXCEPT
    {
        return GetUserPtrImpl( this );
    }

    /**
    * @brief 設定されている任意のユーザーポインターを取得します。
    *
    * @return 設定されている任意のユーザーポインターを返します。
    */
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return GetUserPtrImpl( this );
    }
};

}
}
