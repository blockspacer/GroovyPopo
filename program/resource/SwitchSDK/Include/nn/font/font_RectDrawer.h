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

#include <nn/font/font_Font.h>
#include <nn/font/font_GpuBuffer.h>
#include <nn/util/util_MathTypes.h>
#include <nn/font/font_DispStringBuffer.h>
#include <nn/gfx.h>
#include <nn/util/util_Vector.h>

namespace nn {
namespace font {


//---------------------------------------------------------------------------
//! @brief      描画クラスです。
//---------------------------------------------------------------------------
class RectDrawer
{
public:
    static const int    IndexCountByLetter = 6; //!< 文字単位のインデックス数です。
    static const int    VertexAttributeCount = 3; //!< 頂点属性の数です。

    //! @brief      RectDrawer が必要なワークバッファのサイズを取得します。
    //!
    //! @details    メモリープール用のサイズも合算しています。
    //!             Initialize() で別にメモリープールを指定する場合は、この関数の戻り値から、CalculateMemoryPoolSize() の値を減算して利用してください。
    //!
    //! @param[in]  pDevice         デバイス
    //! @param[in] charCount 1度に描画できる文字数です。
    //!
    //! @return     頂点バッファ・コマンドバッファのメモリサイズを返します。( メモリは、GetWorkBufferAlignment で アライメント調整する必要があります。)
    //!
    //! @sa         Initialize
    //! @sa         GetWorkBufferAlignment
    //! @sa         CalculateMemoryPoolSize
    static size_t           GetWorkBufferSize(nn::gfx::Device* pDevice, uint32_t  charCount);

    //! @brief      RectDrawer が必要なメモリープールのサイズを取得します。
    //!
    //! @param[in]  pDevice         デバイス
    //! @param[in] charCount 1度に描画できる文字数です。
    //!
    //! @return     メモリープールのサイズを返します。
    //!
    //! @sa         Initialize
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice, uint32_t  charCount);

    //! @brief RectDrawer用のメモリプールアライメントを計算します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @return RectDrawer用のメモリプールアライメントが返ります。
    //!
    //! @sa         Initialize
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice);

    //! @brief      RectDrawer のワークバッファのアライメントを取得します。
    //!
    //! @return     RectDrawer のワークバッファのアライメントを返します。
    //!
    static size_t GetWorkBufferAlignment();

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! コンストラクタです。
                        RectDrawer();

    //! @brief      デストラクタです。
    //!
    //! @details    事前に Finalize() を呼び出す必要があります。
    //!
    //! @sa         Finalize
    //!
    virtual             ~RectDrawer();

    //@}

    //----------------------------------------
    //! @name 初期化／後処理
    //@{

    //! @brief      初期化を行います。
    //!
    //! @param[in] pDevice デバイス
    //! @param[in] pWorkBuffer  頂点用バッファ( GetWorkBufferAlignment で アライメント調整する必要があります。)
    //! @param[in] charCount 文字数です。
    //!
    //! @sa         Finalize
    //! @sa         GetWorkBufferAlignment
    bool Initialize(nn::gfx::Device* pDevice, void* pWorkBuffer, uint32_t  charCount)
    {
        return Initialize(pDevice, pWorkBuffer, charCount, NULL, 0, 0);
    }

    //! @brief      初期化を行います。
    //!
    //! @details    メモリープールは、nn::gfx::MemoryPoolProperty_CpuUncached、nn::gfx::MemoryPoolProperty_GpuCached プロパティを持つプールを指定してください。
    //!             サイズは、CalculateMemoryPoolSize() で、オフセットのアラインメントは、CalculateMemoryPoolAlignment() で求めます 。
    //!
    //! @param[in] pDevice デバイス
    //! @param[in] pWorkBuffer  頂点用バッファ( GetWorkBufferAlignment() で アライメント調整する必要があります。)
    //! @param[in] charCount 文字数です。
    //! @param[in] pExternalMemoryPool メモリープールです。
    //! @param[in] externalMemoryPoolOffset 利用するメモリープール領域へのオフセットです。
    //! @param[in] externalMemoryPoolSize メモリープールのサイズです。
    //!
    //! @sa         Finalize
    //! @sa         GetWorkBufferAlignment
    //! @sa         CalculateMemoryPoolSize
    virtual bool Initialize(nn::gfx::Device* pDevice, void* pWorkBuffer, uint32_t  charCount,
        nn::gfx::MemoryPool* pExternalMemoryPool, ptrdiff_t externalMemoryPoolOffset, size_t externalMemoryPoolSize);

    //! @brief 初期化済みか調べます。
    //!
    //! @return 初期化済みであれば true を返します。
    //!
    //! @sa Initialize
    //!
    bool IsInitialized() const
    {
        return (m_CharCountMax != 0);
    }

    //! @brief      開放を行います。
    //!
    //! @param[in] pDevice デバイス
    //!
    //! @sa         Initialize
    virtual void        Finalize(nn::gfx::Device* pDevice);

    //! @brief サンプラーをディスクリプタプールに登録します。
    //!
    //! @param[in] pRegisterSamplerSlot     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void RegisterSamplerToDescriptorPool(RegisterSamplerSlot pRegisterSamplerSlot, void* pUserData);

    //! @brief サンプラー用ディスクリプタスロットを取得します。
    //!
    //! @param[in] pAcquireSamplerSlot    ディスクリプタスロット確保関数です。
    //! @param[in] pUserData              関数に渡されるユーザーデーターです。
    //!
    //! @details
    //! RegisterSamplerToDescriptorPool と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
    //!
    //! @sa ReleaseCommonSamplerSlot
    //! @sa RegisterSamplerToDescriptorPool
    //!
    void AcquireCommonSamplerSlot(AcquireSamplerSlot pAcquireSamplerSlot, void* pUserData);

    //! @brief サンプラーをディスクリプタプールから解放します。
    //!
    //! @param[in] pUnregisterSamplerSlot   ディスクリプタスロット解放関数です。
    //! @param[in] pUserData                関数に渡されるユーザーデーターです。
    //!
    void UnregisterSamplerFromDescriptorPool(UnregisterSamplerSlot pUnregisterSamplerSlot, void* pUserData);

    //! @brief サンプラー用ディスクリプタスロットを解放します。
    //!
    //! @param[in] pReleaseSamplerSlot     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData               関数に渡されるユーザーデーターです。
    //!
    //! @details
    //! UnregisterSamplerFromDescriptorPool と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
    //!
    //! @sa AcquireCommonSamplerSlot
    //! @sa UnregisterSamplerFromDescriptorPool
    //!
    void ReleaseCommonSamplerSlot(ReleaseSamplerSlot pReleaseSamplerSlot, void* pUserData);

    //@}

    //----------------------------------------
    //! @name 描画
    //@{

    //! @brief        描画を行います。
    //!
    //! @param[in]  commandBuffer コマンドバッファ
    //! @param[in]  dispStringBuffer 描画文字列バッファ
    //!
    //! @sa BuildConstantBuffer
    //!
    virtual void Draw(
        nn::gfx::CommandBuffer& commandBuffer,
        const DispStringBuffer& dispStringBuffer) const;

    //@}

    //----------------------------------------
    //! @name 取得／設定
    //@{

    //! @brief      初期化時に与えたバッファを取得します。
    //!
    //! @return     バッファです。
    //!
    void*               GetBuffer() const { return static_cast<void*>(m_WorkMemory); }

    //@}

protected:

    //! @brief      グリフデータの持つテクスチャをロードします。
    //!
    //! @param[in]  pTexObj ロードすべきテクスチャ情報を持つグリフデータ。
    //!
    void                LoadTexture(const TextureObject* pTexObj);

    //! @brief      インデックス生成を行います。
    //!
    //! @param[in]  indices インデックスデータを格納するバッファです。
    //! @param[in]  charCount 文字数です。
    //!
    static void        CreateIndices(uint16_t * indices, uint32_t  charCount);

private:

    //----------------------------------------------------------

    enum ShaderVariation
    {
        // 通常描画シェーダー
        ShaderVariation_NormalShader,
        ShaderVariation_BorderShader,
        ShaderVariation_InvisibleBorderShader,
        // 文字単位の変換行列付きシェーダー
        ShaderVariation_NormalShaderWithPerCharacterTransform,
        ShaderVariation_BorderShaderWithPerCharacterTransform,
        ShaderVariation_InvisibleBorderShaderWithPerCharacterTransform,

        ShaderVariation_ShaderQuantity,
        ShaderVariation_Invalid
    };

    struct ShaderParamBlackWhiteInterpolation
    {
        enum Flags
        {
            Flags_BlackWhiteInterpolation = 1 << 0,
            Flags_None = 0
        };

        int32_t flags;
    };

private:
    // 描画処理の本体です。
    void AddDrawCommand(
        nn::gfx::CommandBuffer& commandBuffer,
        const DispStringBuffer::VertexBufferData& vertexBufferData,
        const uint32_t contentDrawFlags,
        const nn::gfx::GpuAddress& constantBufferGpuAddr,
        const nn::gfx::GpuAddress& perCharacterParamsGpuAddr) const;

    nn::gfx::Shader* GetVertexShader(int variation) const;
    nn::gfx::Shader* GetPixelShader(int variation) const;

    nn::gfx::ResShaderFile* m_pResShaderFile;
    nn::gfx::ShaderCodeType m_CodeType;
    int m_VertexShaderSlots[ShaderVariation_ShaderQuantity];
    int m_VertexShaderPerCharacterParamsSlots[ShaderVariation_ShaderQuantity];
    int m_PixelShaderSlots[ShaderVariation_ShaderQuantity];
    int m_BlackWhiteInterpolationSlots[ShaderVariation_ShaderQuantity];
    int m_TextureSlots[ShaderVariation_ShaderQuantity];

    int32_t m_CharCountMax;

    nn::gfx::VertexState    m_VertexStates[ShaderVariation_ShaderQuantity];

    nn::gfx::MemoryPool     m_MemoryPoolForBuffers;

    nn::gfx::Buffer         m_VertexBuffer;
    nn::gfx::Buffer         m_IndexBuffer;
    nn::gfx::Buffer         m_ShaderParamBlackWhiteInterpolationEnabledBuffer;
    nn::gfx::Buffer         m_ShaderParamBlackWhiteInterpolationDisabledBuffer;

    nn::gfx::Sampler        m_Sampler;
    nn::gfx::DescriptorSlot m_DescriptorSlotForSampler;

    void* m_WorkMemory; //!< ワークメモリ
};

}   // namespace font
}   // namespace nn
