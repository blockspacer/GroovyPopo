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
#include <nn/vfx/vfx_UniformBlock.h>
#include <nn/vfx/vfx_TextureSampler.h>


namespace nn {
namespace vfx {

class Emitter;

namespace detail {

//---------------------------------------------------------------------------
//! @brief  コンピュートシェーダ
//---------------------------------------------------------------------------
class ComputeShader
{
    NN_DISALLOW_COPY( ComputeShader );

public:

    //---------------------------------------------------------------------------
    //! @brief  頂点アトリビュート バッファ インデックス
    //---------------------------------------------------------------------------
    enum AttributeBufferIndex
    {
        ComputeShaderAttributeBufferIndex_Pos           =  0,    //!< 位置
        ComputeShaderAttributeBufferIndex_Vec           =  1,    //!< 速度
        ComputeShaderAttributeBufferIndex_Diff          =  2,    //!< 位置差分
        ComputeShaderAttributeBufferIndex_Scale         =  3,    //!< スケール
        ComputeShaderAttributeBufferIndex_Random        =  4,    //!< ランダム
        ComputeShaderAttributeBufferIndex_MatrixSrt0    =  5,    //!< 生成時SRTマトリクス0
        ComputeShaderAttributeBufferIndex_MatrixSrt1    =  6,    //!< 生成時SRTマトリクス1
        ComputeShaderAttributeBufferIndex_MatrixSrt2    =  7,    //!< 生成時SRTマトリクス2
        ComputeShaderAttributeBufferIndex_MaxIndexCount =  8,    //!< 最大数
    };

    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    ComputeShader() NN_NOEXCEPT
    {
        m_pComputeShader = NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief  コンピュートシェーダを初期化します。
    //! @param[in] computeShader  gfxシェーダ
    //! @return 成否
    //---------------------------------------------------------------------------
    bool Initialize( nn::gfx::Shader* computeShader ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  nn::gfx シェーダオブジェクトを取得します。
    //! @return nn::gfx シェーダオブジェクト
    //---------------------------------------------------------------------------
    nn::gfx::Shader* GetGfxShader() const NN_NOEXCEPT
    {
        return m_pComputeShader;
    }


    //---------------------------------------------------------------------------
    //! @brief  ビュー コンスタントバッファのスロットを取得します。
    //! @return スロット
    //---------------------------------------------------------------------------
    int GetViewBufferSlot() const NN_NOEXCEPT
    {
        return m_ViewBufferSlot;
    }


    //---------------------------------------------------------------------------
    //! @brief  エミッタ動的 コンスタントバッファのスロットを取得します。
    //! @return スロット
    //---------------------------------------------------------------------------
    int GetEmitterDynamicBufferSlot() const NN_NOEXCEPT
    {
        return m_EmitterDynamicBufferSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタ静的 コンスタントバッファのスロットを取得します。
    //! @return スロット
    //---------------------------------------------------------------------------
    int GetEmitterStaticBufferSlot() const NN_NOEXCEPT
    {
        return m_EmitterStaticBufferSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタフィールド コンスタントバッファのスロットを取得します。
    //! @return スロット
    //---------------------------------------------------------------------------
    int GetEmitterFieldBufferSlot() const NN_NOEXCEPT
    {
        return m_EmitterFieldBufferSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  カールノイズテクスチャサンプラのスロットを取得します。
    //! @return スロット
    //---------------------------------------------------------------------------
    int GetCurlNoiseTexSamplerSlot() const NN_NOEXCEPT
    {
        return m_CurlNoiseTexSamplerSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  パーティクルダイナミクスバッファのスロットを取得します。
    //! @return スロット
    //---------------------------------------------------------------------------
    int GetParticleAttributeBufferSlot( AttributeBufferIndex index) const NN_NOEXCEPT
    {
        return m_BufferSlot[index];
    }


private:
    nn::gfx::Shader* m_pComputeShader;

    int m_ViewBufferSlot;                                   //!< ビューコンスタントバッファスロット
    int m_EmitterDynamicBufferSlot;                         //!< エミッタコンスタントバッファスロット
    int m_EmitterStaticBufferSlot;                          //!< エミッタリソースコンスタントバッファスロット
    int m_EmitterFieldBufferSlot;                           //!< フィールドコンスタントバッファスロット
    int m_CurlNoiseTexSamplerSlot;                          //!< カールノイズテクスチャサンプラスロット
    int m_BufferSlot[ ComputeShaderAttributeBufferIndex_MaxIndexCount ]; //!< アトリビュートスロット
};


//---------------------------------------------------------------------------
//! @brief  シェーダ
//---------------------------------------------------------------------------
class Shader
{
    NN_DISALLOW_COPY( Shader );
public:

    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    Shader() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief 定数バッファタイプ
    //---------------------------------------------------------------------------
    enum ConstantBufferType
    {
        ConstantBufferType_View                   = 0,    //!< ビュー定数バッファ
        ConstantBufferType_EmitterStatic          = 1,    //!< TBD
        ConstantBufferType_EmitterDynamic         = 2,    //!< TBD
        ConstantBufferType_EmitterField           = 3,    //!< TBD
        ConstantBufferType_EmitterPlugin          = 4,    //!< TBD
        ConstantBufferType_ReservedParam          = 5,    //!< TBD
        ConstantBufferType_MaxType                = 6,    //!< TBD
    };

private:
    //---------------------------------------------------------------------------
    //! @brief 定数バッファが有効かどうかのビットフラグ
    //---------------------------------------------------------------------------
    enum ConstantBufferEnabledBitFlag
    {
        ConstantBufferEnabledBitFlag_VertexView                   = 0x01 << 0,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexEmitterStatic          = 0x01 << 1,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexEmitterDynamic         = 0x01 << 2,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexEmitterField           = 0x01 << 3,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexEmitterPlugin          = 0x01 << 4,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexReservedParam          = 0x01 << 5,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexCustomConstantBuffer0  = 0x01 << 6,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexCustomConstantBuffer1  = 0x01 << 7,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexCustomConstantBuffer2  = 0x01 << 8,       //!< TBD
        ConstantBufferEnabledBitFlag_VertexCustomConstantBuffer3  = 0x01 << 9,       //!< TBD

        ConstantBufferEnabledBitFlag_PixelView                   = 0x01 << 15,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelEmitterStatic          = 0x01 << 16,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelEmitterDynamic         = 0x01 << 17,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelField                  = 0x01 << 18,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelEmitterPlugin          = 0x01 << 19,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelReservedParam          = 0x01 << 20,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelCustomConstantBuffer0  = 0x01 << 21,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelCustomConstantBuffer1  = 0x01 << 22,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelCustomConstantBuffer2  = 0x01 << 23,       //!< TBD
        ConstantBufferEnabledBitFlag_PixelCustomConstantBuffer3  = 0x01 << 24,       //!< TBD
    };


public:
    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シェーダを初期済かどうかを取得します。
    //! @return trueで初期化済み
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    //---------------------------------------------------------------------------
    //! @brief  シェーダを初期化します。
    //! @param[in] shaderVariation  gfxシェーダバリエーション
    //! @param[in] codeType         コードタイプ
    //! @return TBD
    //---------------------------------------------------------------------------
    bool InitializeShader( nn::gfx::ResShaderVariation* shaderVariation, nn::gfx::ShaderCodeType codeType ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サンプラスロットを初期化します。
    //---------------------------------------------------------------------------
    void InitializeSamplerSlot() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  定数バッファスロットを初期化します。
    //---------------------------------------------------------------------------
    void InitializeConstantBufferSlot() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シェーダの終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      頂点シェーダを取得する
    //! @return     頂点シェーダへのポインタ
    //------------------------------------------------------------------------------
    const nn::gfx::Shader* GetShader() const NN_NOEXCEPT
    {
        return m_pShader;
    }

    //----------------------------------------
    //! @name 定数バッファ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  指定タイプ定数バッファの頂点シェーダでのスロットを取得します。
    //---------------------------------------------------------------------------
    int GetVertexConstantBufferSlot( ConstantBufferType Type ) const NN_NOEXCEPT
    {
        return m_ConstantBufferSlot[ Type ].GetVertexShaderSlot();
    }

    //---------------------------------------------------------------------------
    //! @brief  指定タイプ定数バッファのピクセルシェーダでのスロットを取得します。
    //---------------------------------------------------------------------------
    int GetPixelConstantBufferSlot( ConstantBufferType Type ) const NN_NOEXCEPT
    {
        return m_ConstantBufferSlot[ Type ].GetPixelShaderSlot();
    }

    //---------------------------------------------------------------------------
    //! @brief  指定タイプカスタムシェーダ定数バッファの頂点シェーダでのスロットを取得します。
    //---------------------------------------------------------------------------
    int GetCustomShaderVertexConstantBufferSlot( CustomShaderConstantBufferIndex index ) const NN_NOEXCEPT
    {
        return m_CustomShaderConstantBufferSlot[ index ].GetVertexShaderSlot();
    }

    //---------------------------------------------------------------------------
    //! @brief  指定タイプカスタムシェーダ定数バッファのピクセルシェーダでのスロットを取得します。
    //---------------------------------------------------------------------------
    int GetCustomShaderPixelConstantBufferSlot( CustomShaderConstantBufferIndex index ) const NN_NOEXCEPT
    {
        return m_CustomShaderConstantBufferSlot[ index ].GetPixelShaderSlot();
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムシェーダユニフォームブロックをバインドします。
    //! @param[in] pCommandBuffer   コマンドバッファ
    //! @param[in] index   カスタムシェーダコンスタントバッファインデックス
    //! @param[in] address ユニフォームブロックのGpuアドレス
    //! @param[in] uniformBlockSize ユニフォームブロックサイズ
    //---------------------------------------------------------------------------
    bool BindCustomShaderUniformBlock( nn::gfx::CommandBuffer* pCommandBuffer, CustomShaderConstantBufferIndex index, nn::gfx::GpuAddress* address, size_t uniformBlockSize ) NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name テクスチャロケーション
    //@{

    //---------------------------------------------------------------------------
    //! @brief  ピクセル テクスチャサンプラロケーションを取得します。
    //! @param[in] slot TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetPixelTextureSamplerLocation( TextureSlotId slot ) const NN_NOEXCEPT
    {
        return m_TexSmpLoc[ slot ].GetPixelSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点 テクスチャサンプラロケーションを取得します。
    //! @param[in] slot TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetVertexTextureSamplerLocation( TextureSlotId slot ) const NN_NOEXCEPT
    {
        return m_TexSmpLoc[ slot ].GetVertexSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  ピクセル フレームバッファテクスチャサンプラロケーションを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetFrameBufferTexturePixelSamplerLocation() const NN_NOEXCEPT
    {
        return m_FrameBuferTexSmpLoc.GetPixelSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点フレームバッファテクスチャサンプラロケーションを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetFrameBufferTextureVertexSamplerLocation() const NN_NOEXCEPT
    {
        return m_FrameBuferTexSmpLoc.GetVertexSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  ピクセル デプスバッファテクスチャサンプラロケーションを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetDepthBufferTexturePixelSamplerLocation() const NN_NOEXCEPT
    {
        return m_DepthBuferTexSmpLoc.GetPixelSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点 デプスバッファテクスチャサンプラロケーションを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetDepthBufferTextureVertexSamplerLocation() const NN_NOEXCEPT
    {
        return m_DepthBuferTexSmpLoc.GetVertexSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  ピクセル カールノイズテクスチャサンプラロケーションを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetCurlNoiseTexturePixelSamplerLocation() const NN_NOEXCEPT
    {
        return m_CurlNoiseTexSmpLoc.GetPixelSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点 カールノイズテクスチャサンプラロケーションを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetCurlNoiseTextureVertexSamplerLocation() const NN_NOEXCEPT
    {
        return m_CurlNoiseTexSmpLoc.GetVertexSamplerLocation();
    }

    //---------------------------------------------------------------------------
    //! @brief  テクスチャサンプラロケーションを取得します。
    //! @param[in] type カスタムシェーダテクスチャタイプ
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetCustomTexturePixelSamplerLocation( CustomShaderTextureType type ) const NN_NOEXCEPT
    {
        return m_CustomTextureLoc[ type ].GetPixelSamplerLocation();
    }
    //---------------------------------------------------------------------------
    //! @brief  テクスチャサンプラロケーションを取得します。
    //! @param[in] type カスタムシェーダテクスチャタイプ
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetCustomTextureVertexSamplerLocation( CustomShaderTextureType type ) const NN_NOEXCEPT
    {
        return m_CustomTextureLoc[ type ].GetVertexSamplerLocation();
    }

    //@}

private:
    bool                        m_IsInitialized;                                                //!< 初期化済かどうかリビュート
    nn::gfx::Shader*            m_pShader;                                                      //!< シェーダ
    uint32_t                    m_ConstantBufferEnableFlag;                                     //!< 定数バッファ有効フラグ
    detail::TextureSamplerSlot  m_CustomTextureLoc[ CustomShaderTextureType_MaxTextureType ];   //!< カスタムテクスチャ
    detail::TextureSamplerSlot  m_TexSmpLoc[ TextureSlotId_MaxStandardTextureSlotId ];          //!< テクスチャ0-2 サンプラロケーション
    detail::TextureSamplerSlot  m_FrameBuferTexSmpLoc;                                          //!< フレームバッファテクスチャ サンプラロケーション
    detail::TextureSamplerSlot  m_DepthBuferTexSmpLoc;                                          //!< デプスバッファテクスチャ   サンプラロケーション
    detail::TextureSamplerSlot  m_CurlNoiseTexSmpLoc;                                           //!< カールノイズテクスチャ   サンプラロケーション
    ConstantBufferSlot          m_ConstantBufferSlot[ ConstantBufferType_MaxType ];             //!< 定数バッファスロット
    ConstantBufferSlot          m_CustomShaderConstantBufferSlot[ CustomShaderConstantBufferIndex_MaxIndexCount ];      //!< カスタム定数バッファスロット
};

} // namespace detail
} // namespace vfx
} // namespace nn
