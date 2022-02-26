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
* @brief マテリアルに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResMaterial.h>
#include <nn/g3d/detail/g3d_Flag.h>

namespace nn { namespace g3d {

//! @brief マテリアルインスタンスです。
class MaterialObj
{
public:
    static const uint32_t TextureCountMax = 16;

    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer       = 8, //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //! @briefprivate テクスチャー差し替え時に呼ばれるコールバックの型
    typedef void (*TextureChangeCallback)(MaterialObj* pMaterialObj, int idxTexture);

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実際の構築処理は Builder で行います。
    //!
    MaterialObj() NN_NOEXCEPT
        : m_pRes(NULL)
        , m_Flag(0)
        , m_BufferingCount(0)
        , m_DirtyFlagSet()
        , m_pMemoryPool(NULL)
        , m_MemoryPoolOffset(0)
        , m_pMaterialBlockArray(NULL)
        , m_pSrcParam(NULL)
        , m_ppTextureArray(NULL)
        , m_pTextureDescriptorSlotArray(NULL)
        , m_SizePerMaterialBlock(0)
        , m_pUserPtr(NULL)
        , m_pBufferPtr(NULL)
        , m_pTextureChangeCallback(NULL)
    {
    }

    //! @brief ユニフォームブロックのサイズを計算します。
    //!
    //! @return 計算したバッファーサイズを返します。
    //!
    size_t CalculateBlockBufferSize(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    //! @brief メモリプールを渡して、ユニフォームブロックを構築します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool メモリプールへのポインター。
    //! @param[in] offset メモリプールの使用する領域へのオフセット。
    //! @param[in] memoryPoolSize offset以降で使用可能なメモリプールのサイズ。
    //!
    //! @return 構築に成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool が初期化されている。
    //! - offset で指されたメモリプールの位置が GetBlockBufferAlignment() のアラインメント上にある。
    //! - memoryPoolSize は CalculateBlockBufferSize() 以上である。
    //! - ユニフォームブロックが未構築である。
    //!
    //! @post
    //! - ユニフォームブロックが構築されている
    //!
    //! @details
    //! 特殊ケースとしてリソースの nn::g3d::ResMaterial::SetRawParamSize() で設定された値が0の場合、ユニフォームブロックを
    //! 構築せず、trueを返します。
    //!
    bool SetupBlockBuffer(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    //! @brief ユニフォームブロックを破棄します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - ユニフォームブロックが構築済みである。
    //!
    //! @post
    //!- ユニフォームブロックが破棄されている。
    //!
    void CleanupBlockBuffer(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief マテリアルに関する描画リソースを計算します。
    //!
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つユニフォームブロックのバッファーリング数範囲内。
    //!
    //! @details
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! フレームバッファーのコピーアウト中などに呼ぶことを想定しています。
    //!
    void CalculateMaterial(int bufferIndex) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief ユニフォームブロックが要求するアライメントを取得します。
    //!
    //! @return ユニフォームブロックが要求するアライメントを返します。
    //!
    size_t GetBlockBufferAlignment(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    //! @brief リソースを取得します。
    //!
    //! @return ResMaterial へのポインターを返します。
    //!
    const ResMaterial* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief Builder で Build 時に渡されたバッファーを取得します。
    //!
    //! @return Builder で Build 時に渡されたバッファーへのポインターを返します。
    //!
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのポインターを取得します。
    //!
    //! @return メモリプールへのポインターを返します。
    //!
    nn::gfx::MemoryPool* GetMemoryPoolPtr() NN_NOEXCEPT
    {
        return m_pMemoryPool;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのオフセットを取得します。
    //!
    //! @return メモリプールのオフセットを返します。
    //!
    ptrdiff_t GetMemoryPoolOffset() NN_NOEXCEPT
    {
        return m_MemoryPoolOffset;
    }

    //! @brief ユニフォームブロックが構築済みであるかどうかを取得します。
    //!
    //! @return 構築済みの場合は true、未構築の場合は false を返します。
    //!
    bool IsBlockBufferValid() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_BlockBufferValid) != 0;
    }

    //! @brief マテリアルが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つバッファー数の範囲内。
    //!
    //! @details
    //! ユニフォームブロックが未構築の場合、NULL を返します。
    //!
    nn::gfx::Buffer* GetMaterialBlock(int bufferIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (!IsBlockBufferValid())
        {
            return NULL;
        }
        return m_pMaterialBlockArray ? &m_pMaterialBlockArray[bufferIndex] : NULL;
    }

    //! @brief マテリアルが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つバッファー数の範囲内。
    //!
    //! @details
    //! ユニフォームブロックが未構築の場合、NULL を返します。
    //!
    const nn::gfx::Buffer* GetMaterialBlock(int bufferIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (!IsBlockBufferValid())
        {
            return NULL;
        }
        return m_pMaterialBlockArray ? &m_pMaterialBlockArray[bufferIndex] : NULL;
    }

    //! @brief マテリアルが保持するユニフォームブロックのサイズを取得します。
    //!
    //! @return マテリアルが保持するユニフォームブロックのサイズを返します。
    //!
    size_t GetMaterialBlockSize() const NN_NOEXCEPT
    {
        return m_SizePerMaterialBlock;
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief ユニフォームブロックのエンディアンスワップを行うかどうかを取得します。
    //!
    //! @return ユニフォームブロックをスワップする場合は true、しない場合は false を返します。
    //!
    bool IsBlockSwapEnabled() const NN_NOEXCEPT
    {
#if defined(NN_BUILD_CONFIG_OS_COS)
        return true;
#else
        return false;
#endif
    }

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        m_pUserPtr = pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserPtr);
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(m_pUserPtr);
    }

    //! @brief マテリアル名を取得します。
    //!
    //! @return ResMaterial のマテリアル名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

    //----------------------------------------
    //! @name シェーダーパラメータ
    //@{

    //! @brief マテリアル配下のシェーダーパラメータの数を取得します。
    //!
    //! @return マテリアル配下のシェーダーパラメータの数を返します。
    //!
    int GetShaderParamCount() const NN_NOEXCEPT
    {
        return m_pRes->GetShaderParamCount();
    }

    //! @brief インデックスからシェーダーパラメータ名を取得します。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @return シェーダーパラメータ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメータ数の範囲内。
    //!
    const char* GetShaderParamName(int paramIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetShaderParamName(paramIndex);
    }

    //! @brief シェーダーパラメータ名からインデックスを取得します。
    //!
    //! @param[in] name シェーダーパラメータ名へのポインター。
    //!
    //! @return シェーダーパラメータのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーパラメータが存在しない場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShaderParamIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindShaderParamIndex(name);
    }

    //! @brief インデックス引きでシェーダーパラメータを取得します。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @return ResShaderParam へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメータ数の範囲内。
    //!
    const ResShaderParam* GetResShaderParam(int paramIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetShaderParam(paramIndex);
    }

    //! @brief 名前引きでシェーダーパラメータを取得します。
    //!
    //! @param[in] name シェーダーパラメータ名へのポインター。
    //!
    //! @return ResShaderParam へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーパラメータが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderParam* FindResShaderParam(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindShaderParam(name);
    }

    //! @brief シェーダーパラメータを編集するために取得します。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @return シェーダーパラメータへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメータ数の範囲内。
    //!
    //! @details
    //! この関数で取得したパラメータは DirtyFlag で設定され、
    //! CalculateMaterial() を呼び出すことでユニフォームブロックに反映されます。
    //!
    void* EditShaderParam(int paramIndex) NN_NOEXCEPT
    {
        const ResShaderParam* pParam = GetResShaderParam(paramIndex);
        if (pParam->GetOffset() >= 0)
        {
            SetDirtyFlag(paramIndex); // 書き込み先が存在する場合のみフラグを立てる。
        }
        int dependedIndex = pParam->GetDependedIndex();
        const ResShaderParam *pDependedParam = GetResShaderParam(dependedIndex);
        if (pDependedParam->GetOffset() >= 0)
        {
            SetDirtyFlag(dependedIndex); // 書き込み先が存在する場合のみフラグを立てる。
        }
        return AddOffset(m_pSrcParam, pParam->GetSrcOffset());
    }

    //! @brief シェーダーパラメータを取得します。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @return シェーダーパラメータへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメータ数の範囲内。
    //!
    //! @details
    //! 取得したメモリを書き換える場合は EditShaderParam() を呼び出してください。
    //!
    const void* GetShaderParam(int paramIndex) const NN_NOEXCEPT
    {
        return AddOffset(m_pSrcParam, GetResShaderParam(paramIndex)->GetSrcOffset());
    }

    //! @brief シェーダーパラメータを編集するために取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @return シェーダーパラメータへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメータ数の範囲内。
    //!
    //! @details
    //! この関数で取得したパラメータは DirtyFlag で設定され、
    //! CalculateMaterial() を呼び出すことでユニフォームブロックに反映されます。
    //! テンプレート引数を指定することで型チェックを行います。
    //! 真偽値は4バイトで格納されているため、int32_t、uint32_t などの
    //! 4バイトの型で取得する必要があります。
    //!
    template <typename T>
    T* EditShaderParam(int paramIndex) NN_NOEXCEPT
    {
        const ResShaderParam* pParam = GetResShaderParam(paramIndex);
        NN_SDK_ASSERT(sizeof(T) <= pParam->GetSrcSize(), "Oversized T. %u <= %u\n%s\n",
            sizeof(T), pParam->GetSrcSize(),
            NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (pParam->GetOffset() >= 0)
        {
            SetDirtyFlag(paramIndex); // 書き込み先が存在する場合のみフラグを立てる。
        }
        int dependedIndex = pParam->GetDependedIndex();
        const ResShaderParam* pDependedParam = GetResShaderParam(dependedIndex);
        if (pDependedParam->GetOffset() >= 0)
        {
            SetDirtyFlag(dependedIndex); // 書き込み先が存在する場合のみフラグを立てる。
        }
        return AddOffset<T>(m_pSrcParam, pParam->GetSrcOffset());
    }

    //! @brief シェーダーパラメータを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @return シェーダーパラメータへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメータ数の範囲内。
    //!
    //! @details
    //! 取得したメモリを書き換える場合は EditShaderParam() を呼び出してください。
    //! テンプレート引数を指定することで型チェックを行います。
    //! 真偽値は4バイトで格納されているため、int32_t、uint32_t などの
    //! 4バイトの型で取得する必要があります。
    //!
    template <typename T>
    const T* GetShaderParam(int paramIndex) const NN_NOEXCEPT
    {
        const ResShaderParam* pParam = GetResShaderParam(paramIndex);
        NN_SDK_ASSERT(sizeof(T) <= pParam->GetSrcSize(), "Oversized T. %u <= %u\n%s\n",
            sizeof(T), pParam->GetSrcSize(),
            NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return AddOffset<T>(m_pSrcParam, pParam->GetSrcOffset());
    }

    //! @brief シェーダーパラメータをリソースの状態に戻します。
    void ClearShaderParam() NN_NOEXCEPT
    {
        ResetDirtyFlags();
        memcpy(m_pSrcParam, m_pRes->ToData().pSrcParam.Get(), m_pRes->GetSrcParamSize());
        InitializeDependPointer();
    }

    //! @brief ユニフォームブロックに書き込まれる前の論理的なシェーダーパラメータのバッファーを取得します。
    //!
    //! @return ユニフォームブロックに書き込まれる前の論理的なシェーダーパラメータのバッファーへのポインターを返します。
    //!
    void* GetSrcParam() NN_NOEXCEPT
    {
        return m_pSrcParam;
    }

    //! @brief ユニフォームブロックに書き込まれる前の論理的なシェーダーパラメータのバッファーを取得します。
    //!
    //! @return ユニフォームブロックに書き込まれる前の論理的なシェーダーパラメータのバッファーへのポインターを返します。
    //!
    const void* GetSrcParam() const NN_NOEXCEPT
    {
        return m_pSrcParam;
    }

    //! @brief すべてのシェーダーパラメータの DirtyFlag を Dirty 状態に戻します。
    void ResetDirtyFlags() NN_NOEXCEPT;

    //! @brief CalculateMaterial() による更新の必要性有無を取得します。
    //!
    //! @return 更新が必要な場合は true、不要な場合は false を返します。
    //!
    bool IsMaterialBlockDirty() const NN_NOEXCEPT
    {
        return m_DirtyFlagSet.IsFlagDirty() || (m_pRes->ToData().shaderParamVolatileCount > 0);
    }

    //@}

    //----------------------------------------
    //! @name テクスチャー
    //@{

    //! @brief マテリアル配下のテクスチャーの数を取得します。
    //!
    //! @return マテリアル配下のテクスチャーの数を返します。
    //!
    int GetTextureCount() const NN_NOEXCEPT
    {
        return m_pRes->GetTextureCount();
    }

    //! @brief インデックスからテクスチャーの名前を取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャー名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つテクスチャー数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetTextureName(int texIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetTextureName(texIndex);
    }

    //! @brief テクスチャーを取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return nn::g3d::TextureRef を返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つテクスチャー数の範囲内。
    //!
    //! @details
    //! あるサンプラーが参照するテクスチャーは、そのサンプラーと同じインデックスのテクスチャーです。
    //!
    nn::g3d::TextureRef GetTexture(int texIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return nn::g3d::TextureRef(m_ppTextureArray[texIndex], m_pTextureDescriptorSlotArray[texIndex]);
    }

    //! @brief テクスチャーを設定します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //! @param[in] textureRef 差し替えるテクスチャーのビューおよびディスクリプタスロットを格納した nn::g3d::TextureRef への参照。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つテクスチャー数の範囲内。
    //!
    //! @details
    //! マテリアルが参照するテクスチャーを差し替えることができます。
    //! あるサンプラーが参照するテクスチャーは、そのサンプラーと同じインデックスのテクスチャーです。
    //!
    void SetTexture(int texIndex, const TextureRef& textureRef) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        const nn::gfx::TextureView* pOldTextureView = m_ppTextureArray[texIndex];
        const nn::gfx::DescriptorSlot* pOldDescriptorSlot = &m_pTextureDescriptorSlotArray[texIndex];
        m_ppTextureArray[texIndex] = textureRef.GetTextureView();
        m_pTextureDescriptorSlotArray[texIndex] = textureRef.GetDescriptorSlot();
        if (m_pTextureChangeCallback && (pOldTextureView != textureRef.GetTextureView() ||
            memcmp(pOldDescriptorSlot, &m_pTextureDescriptorSlotArray[texIndex], sizeof(nn::gfx::DescriptorSlot)) != 0))
        {
            m_pTextureChangeCallback(this, texIndex);
        }
    }

    //! @brief テクスチャーをリソースの状態に戻します。
    void ClearTexture() NN_NOEXCEPT
    {
        for (int idxTex = 0, texCount = GetTextureCount(); idxTex < texCount; ++idxTex)
        {
            TextureRef resTextureRef(m_pRes->GetTexture(idxTex), m_pRes->GetTextureDescriptorSlot(idxTex));
            SetTexture(idxTex, resTextureRef);
        }
    }

    //! @briefprivate テクスチャー差し替え時に呼ばれるコールバックを設定します。
    void SetTextureChangeCallback(TextureChangeCallback pCallback) NN_NOEXCEPT
    {
        m_pTextureChangeCallback = pCallback;
    }

    //! @briefprivate テクスチャー差し替え時に呼ばれるコールバックを取得します。
    TextureChangeCallback GetTextureChangeCallback() const NN_NOEXCEPT
    {
        return m_pTextureChangeCallback;
    }

    //@}

    //----------------------------------------
    //! @name サンプラー
    //@{

    //! @brief マテリアル配下のサンプラーの数を取得します。
    //!
    //! @return マテリアル配下のサンプラーの数を返します。
    //!
    int GetSamplerCount() const NN_NOEXCEPT
    {
        return m_pRes->GetSamplerCount();
    }

    //! @brief インデックスからサンプラー名を取得します。
    //!
    //! @param[in] samplerIndex サンプラーのインデックス。
    //!
    //! @return サンプラー名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つサンプラー数の範囲内。
    //!
    //! @details
    //! サンプラーのインデックスは、それが参照するテクスチャーのインデックスと一致します。
    //!
    const char* GetSamplerName(int samplerIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetSamplerName(samplerIndex);
    }

    //! @brief サンプラー名からインデックスを取得します。
    //!
    //! @param[in] name サンプラー名へのポインター。
    //!
    //! @return サンプラーのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが存在しない場合は  nn::util::ResDic::Npos を返します。
    //! サンプラーのインデックスは、それが参照するテクスチャーのインデックスと一致します。
    //!
    int FindSamplerIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindSamplerIndex(name);
    }

    //! @brief インデックス引きでサンプラーを取得します。
    //!
    //! @param[in] samplerIndex サンプラーのインデックス。
    //!
    //! @return nn::g3d::SamplerRef を返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つサンプラー数の範囲内。
    //!
    //! @details
    //! サンプラーのインデックスは、それが参照するテクスチャーのインデックスと一致します。
    //!
    SamplerRef GetSampler(int samplerIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(samplerIndex, 0, GetSamplerCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return SamplerRef(m_pRes->GetSampler(samplerIndex), m_pRes->GetSamplerDescriptorSlot(samplerIndex));
    }

    //! @brief 名前引きでサンプラーを取得します。
    //!
    //! @param[in] name サンプラー名へのポインター。
    //!
    //! @return nn::g3d::SamplerRef を返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが存在しない場合は 無効な SamplerRef を返します。
    //! nn::g3d::SamplerRef::IsValid() で確認出来ます。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    SamplerRef FindSampler(const char* name) const NN_NOEXCEPT
    {
        int index = m_pRes->FindSamplerIndex(name);
        if (index != nn::util::ResDic::Npos)
        {
            return SamplerRef(m_pRes->GetSampler(index), m_pRes->GetSamplerDescriptorSlot(index));
        }
        return SamplerRef();
    }

    //@}

    //! @briefprivate ユニフォームブロックのバッファー配列を設定します。
    //!
    //! @param[in] pBlockArray バッファー配列へのポインター。
    //!
    void SetMaterialBlockArray(nn::gfx::Buffer* pBlockArray) NN_NOEXCEPT
    {
        m_pMaterialBlockArray = pBlockArray;
    }

    //! @briefprivate ユニフォームブロックのバッファー配列を取得します。
    //!
    //! @return バッファー配列へのポインターを返します。
    //!
    nn::gfx::Buffer* GetMaterialBlockArray() NN_NOEXCEPT
    {
        return m_pMaterialBlockArray;
    }

protected:
    //! @brief マテリアルの状態を表すフラグです。
    enum Flag
    {
        Flag_BlockBufferValid = 0x1 << 0, //!< @briefprivate ユニフォームブロックが構築済みであるかどうかを表すフラグです。
    };

    //! @brief シェーダーパラメータに DirtyFlag を設定します。
    //!
    //! @param[in] paramIndex シェーダーパラメータのインデックス。
    //!
    //! @details
    //! DirtyFlag が設定されたシェーダーパラメータは、
    //! CalculateMaterial() を呼び出すことでユニフォームブロックに反映されます。
    //!
    void SetDirtyFlag(int paramIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(paramIndex, 0, GetShaderParamCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        m_DirtyFlagSet.Set(paramIndex);
    }

    //! @brief SrcParam を変換してユニフォームブロックに反映します。
    //!
    //! @param[in] pBuffer 変換した結果を格納するバッファーへのポインター。
    //!
    //! @details
    //! DirtyFlag を考慮してユニフォームブロックに反映する場合は ConvertDirtyParams() を呼び出してください。
    //!
    template <bool swap>
    void ConvertParams(void* pBuffer) NN_NOEXCEPT;

    //! @brief SrcParam を変換してユニフォームブロックに反映します。
    //!
    //! @param[in] pBuffer 変換した結果を格納するバッファーへのポインター。
    //! @param[in] pDirtyFlagArray DirtyFlag へのポインター。
    //!
    //! DirtyFlag を考慮して変換処理を行います。
    //! CalculateMaterial() が内部で呼び出す関数です。
    //!
    template <bool swap>
    void ConvertDirtyParams(void* pBuffer, Bit32* pDirtyFlagArray) NN_NOEXCEPT;

    //! @brief SrcParam を変換してユニフォームブロックに反映します。
    //!
    //! @param[in] pBuffer 変換した結果を格納するバッファーへのポインター。
    //!
    //! @details
    //! CalculateMaterial() が内部で呼び出す関数です。
    //!
    template <bool swap>
    void ConvertVolatileParams(void* pBuffer) NN_NOEXCEPT;

    //! @brief 依存パラメータのポインターを設定します。
    //!
    //! Build() と ClearShaderParam() が内部で呼び出す関数です。
    //!
    void InitializeDependPointer() NN_NOEXCEPT;

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;
    void SetupBlockBufferImpl(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    const ResMaterial*           m_pRes;

    Bit16                        m_Flag;
    uint8_t                      m_BufferingCount;
    NN_PADDING1;
    FlagSet                      m_DirtyFlagSet;
    nn::gfx::MemoryPool*         m_pMemoryPool;
    ptrdiff_t                    m_MemoryPoolOffset;
    nn::gfx::Buffer*             m_pMaterialBlockArray;
    void*                        m_pSrcParam; // m_MatBlock の元データ
    const nn::gfx::TextureView** m_ppTextureArray;
    nn::gfx::DescriptorSlot*     m_pTextureDescriptorSlotArray;
    size_t                       m_SizePerMaterialBlock;

    void*                        m_pUserPtr;
    void*                        m_pBufferPtr;

    TextureChangeCallback        m_pTextureChangeCallback;
    NN_DISALLOW_COPY(MaterialObj);
};

// 宣言のみで定義は行わない。
// bool は4バイトで格納されているため、int32_t uint32_t 等で取得しなければならない。
template <>
bool* MaterialObj::EditShaderParam<bool>(int) NN_NOEXCEPT;
template <>
const bool* MaterialObj::GetShaderParam<bool>(int) const NN_NOEXCEPT;

//--------------------------------------------------------------------------------------------------

//! @briefprivate MaterialObj::Initialize() に渡して初期化を行うパラメータです。
class MaterialObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_DirtyFlagSet,
        MemoryBlockIndex_SrcParamArray,
        MemoryBlockIndex_TextureArray,
        MemoryBlockIndex_MatBlockArray,
        MemoryBlockIndex_TextureDescriptorSlotArray,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResMaterial へのポインター。
    //!
    explicit InitializeArgument(const ResMaterial* resource) NN_NOEXCEPT
        : m_pRes(resource)
        , m_BufferingCount(1)
        , m_MemoryBlock()
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
        m_WorkMemory.Invalidate();
    }

    //! @brief リソースを取得します。
    //!
    //! @return ResMaterial へのポインターを返します。
    //!
    const ResMaterial* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief ユニフォームブロックのバッファーリング数を設定します。
    //!
    //! @param[in] count ユニフォームブロックのバッファーリング数。
    //!
    void BufferingCount(int count) NN_NOEXCEPT
    {
        m_BufferingCount = count;
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief 構築に必要なメモリサイズを取得します。
    //!
    //! @return MaterialObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief MaterialObj 構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたユニフォームブロックのバッファーリング数が１以上。
    //!
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーのサイズを取得します。
    size_t GetBufferSize( int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].GetSize();
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief MaterialObj 構築に必要なメモリサイズが計算済みかを取得します。
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    const ResMaterial*                    m_pRes;
    int                                   m_BufferingCount;
    nn::util::MemorySplitter              m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief MaterialObj の構築を行うクラスです。
class MaterialObj::Builder : public MaterialObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResMaterial へのポインター
    //!
    explicit Builder(const ResMaterial* resource) NN_NOEXCEPT
        : InitializeArgument(resource)
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
    }

    //! @brief MaterialObj を構築します。
    //!
    //! @param[in] pMaterialObj MaterialObj へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemSize() を呼び、メモリサイズが計算済みである。
    //! - pBufferはAlignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(MaterialObj* pMaterialObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pMaterialObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}} // namespace nn::g3d

