/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/vfx/vfx_TargetDef.h>
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/vfx_Shader.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  シェーダマネージャ
//---------------------------------------------------------------------------
class ShaderManager
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    ShaderManager() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  初期化処理
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] pHeap            ヒープへのポインタ
    //! @param[in] shaderContainer  シェーダコンテナ
    //! @param[in] delaySetup       シェーダ遅延セットアップ
    //---------------------------------------------------------------------------
    void Initialize( nn::gfx::Device* pDevice, Heap* pHeap, nn::gfx::ResShaderContainer* shaderContainer, bool delaySetup ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  終了処理
    //! @param[in] pDevice      gfxデバイス
    //! @param[in] pHeap         ヒープへのポインタ
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice, Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シェーダを取得します。
    //! @param[in] index インデックス
    //! @return シェーダへのポインタ
    //---------------------------------------------------------------------------
    Shader* GetShader( int index ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  管理しているシェーダ数を取得します。
    //! @return 管理しているシェーダ数
    //---------------------------------------------------------------------------
    int GetShaderCount() const NN_NOEXCEPT
    {
        return m_ShaderCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  初期化済みフラグを取得します。
    //! @return 初期化済みかどうか
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

private:
    bool                            m_IsInitialized;            //!< 初期化済みフラグ
    nn::gfx::ResShaderContainer*    m_pResShaderContainer;      //!< シェーダコンテナ
    nn::gfx::ShaderCodeType         m_CodeType;                 //!< gfx シェーダコードタイプ
    nn::gfx::Device*                m_GfxDevice;                //!< gfx デバイス( 遅延セットアップ用 )
    Shader*                         m_ShaderArray;              //!< シェーダ配列
    void*                           m_ShaderArrayPtr;           //!< シェーダ配列用バッファ
    int                             m_ShaderCount;              //!< 管理しているシェーダ数
};



//---------------------------------------------------------------------------
//! @brief  コンピュートシェーダマネージャ
//---------------------------------------------------------------------------
class ComputeShaderManager
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    ComputeShaderManager() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  初期化処理
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] pHeap            ヒープへのポインタ
    //! @param[in] shaderContainer  シェーダコンテナ
    //---------------------------------------------------------------------------
    void Initialize( nn::gfx::Device* pDevice, Heap* pHeap, nn::gfx::ResShaderContainer* shaderContainer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  終了処理
    //! @param[in] pDevice      gfxデバイス
    //! @param[in] pHeap         ヒープへのポインタ
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice, Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シェーダを取得します。
    //! @param[in] index インデックス
    //! @return シェーダへのポインタ
    //---------------------------------------------------------------------------
    ComputeShader* GetShader( int index ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index < m_ShaderCount );
        NN_SDK_ASSERT_NOT_NULL( m_ShaderArray );
        return &m_ShaderArray[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  管理しているシェーダ数を取得します。
    //! @return 管理しているシェーダ数
    //---------------------------------------------------------------------------
    int GetShaderCount() const NN_NOEXCEPT
    {
        return m_ShaderCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  初期化済みフラグを取得します。
    //! @return 初期化済みかどうか
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

private:
    bool                            m_IsInitialized;            //!< 初期化済みフラグ
    nn::gfx::ResShaderContainer*    m_pResShaderContainer;      //!< シェーダコンテナ
    nn::gfx::ShaderCodeType         m_CodeType;                 //!< gfx シェーダコードタイプ
    ComputeShader*                  m_ShaderArray;              //!< シェーダ配列
    void*                           m_ShaderArrayPtr;           //!< シェーダ配列用バッファ
    int                             m_ShaderCount;              //!< 管理しているシェーダ数
};


} // namespace detail
} // namespace vfx
} // namespace nn
