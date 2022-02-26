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
#include <nn/vfx/vfx_Data.h>
#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_Config.h>

namespace nn {
namespace vfx {
namespace detail {


//---------------------------------------------------------------------------
//! @brief      テクスチャサンプラスロット
//---------------------------------------------------------------------------
struct TextureSamplerSlot
{
    //---------------------------------------------------------------------------
    //! @brief      初期化
    //! @param[in]  pShader         シェーダへのポインタ
    //! @param[in]  samplerName     サンプラ名
    //! @return     成功した場合 true
    //---------------------------------------------------------------------------
    bool Initialize( nn::gfx::Shader* pShader, const char* samplerName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ピクセル サンプラロケーションを取得します。
    //! @return     ロケーション ID
    //---------------------------------------------------------------------------
    int GetPixelSamplerLocation() const NN_NOEXCEPT
    {
        return pixelShaderSamplerLocation;
    }

    //---------------------------------------------------------------------------
    //! @brief      頂点 サンプラロケーションを取得します。
    //! @return     ロケーション ID
    //---------------------------------------------------------------------------
    int GetVertexSamplerLocation() const NN_NOEXCEPT
    {
        return vertexShaderSamplerLocation;
    }

    //---------------------------------------------------------------------------
    //! @brief      ピクセルシェーダサンプラロケーションが有効かどうか。
    //! @return     有効な場合 true
    //---------------------------------------------------------------------------
    bool IsValid() const NN_NOEXCEPT
    {
        if( pixelShaderSamplerLocation == InvalidValueId_TextureSamplerSlotId )
        {
            return false;
        }
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        pixelShaderSamplerLocation = InvalidValueId_TextureSamplerSlotId;
    }

private:
    int pixelShaderSamplerLocation;        //!< ピクセルシェーダテクスチャサンプラロケーション
    int vertexShaderSamplerLocation;        //!< 頂点シェーダテクスチャサンプラロケーション
};


//---------------------------------------------------------------------------
//! @brief  テクスチャサンプラー
//---------------------------------------------------------------------------
class TextureSampler
{
    NN_DISALLOW_COPY( TextureSampler );
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    TextureSampler() NN_NOEXCEPT
    {
        m_IsInitialized = false;
    }

    //---------------------------------------------------------------------------
    //! @brief  必要となる全てのサンプラを生成する
    //---------------------------------------------------------------------------
    static void InitializeSamplerTable( nn::gfx::Device* pDevice, nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  全てのサンプラを破棄する
    //---------------------------------------------------------------------------
    static void FinalizeSamplerTable( nn::gfx::Device* pDevice, nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  生成済みサンプラを取得する
    //---------------------------------------------------------------------------
    static TextureSampler* GetSamplerFromTable( detail::ResTextureSampler* pTextureSampler ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  テクスチャーサンプラをディスクリプタプールに登録します。
    //---------------------------------------------------------------------------
    static void RegisterSamplerToDescriptorPool( void* pRegisterCallback, void* pUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  テクスチャーサンプラをディスクリプタプールから開放します。
    //---------------------------------------------------------------------------
    static void UnregisterSamplerFromDescriptorPool( void* pUnregisterCallback, void* pUserData ) NN_NOEXCEPT;


public:
    //---------------------------------------------------------------------------
    //! @brief  初期化
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] pTextureSampler  テクスチャサンプラリソース
    //---------------------------------------------------------------------------
    void Initialize( nn::gfx::Device* pDevice, detail::ResTextureSampler* pTextureSampler ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  終了処理
    //! @param[in] pDevice         gfxデバイス
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        m_IsInitialized = false;
        return;
    }

    //---------------------------------------------------------------------------
    //! @brief  有効かどうか
    //! @return trueで有効、falseで無効
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    //---------------------------------------------------------------------------
    //! @brief  テクスチャサンプラリソースを取得する
    //! @return テクスチャサンプラリソースへのポインタ
    //---------------------------------------------------------------------------
    const detail::ResTextureSampler GetTextureSamplerRes() const NN_NOEXCEPT
    {
        return m_TextureSamplerResource;
    }

    //---------------------------------------------------------------------------
    //! @brief  テクスチャサンプラを取得する
    //! @return テクスチャサンプラへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::Sampler& GetTextureSampler() const NN_NOEXCEPT
    {
        return m_Sampler;
    }

    //---------------------------------------------------------------------------
    //! @brief  テクスチャサンプラを取得する
    //! @return テクスチャサンプラへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::DescriptorSlot& GetDescriptorSlot() const NN_NOEXCEPT
    {
        return m_DescriptorSlot;
    }

private:
    bool                            m_IsInitialized;            //!< 初期化済みかどうか
    detail::ResTextureSampler       m_TextureSamplerResource;   //!< テクスチャサンプラリソース
    nn::gfx::DescriptorSlot         m_DescriptorSlot;           //!< デスクリプタスロット
    nn::gfx::Sampler                m_Sampler;                  //!< gfxサンプラ
    static  int                     g_SamplerPatternCount;      //!< サンプラの全パターン数
    static  TextureSampler*         g_SamplerBuffer;            //!< サンプラ配列
};

} // namespace detail
} // namespace nn
} // namespace vfx
