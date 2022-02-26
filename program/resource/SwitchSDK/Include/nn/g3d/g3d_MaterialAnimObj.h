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
* @brief マテリアルアニメーションに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_AnimObj.h>
#include <nn/g3d/g3d_Binding.h>
#include <nn/g3d/g3d_ResMaterialAnim.h>

namespace nn { namespace g3d {

class ModelObj;
class MaterialObj;

//! @brief マテリアルアニメーションインスタンスです。
class MaterialAnimObj : public ModelAnimObj
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = 8 //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    MaterialAnimObj() NN_NOEXCEPT
        : ModelAnimObj()
        , m_pRes(NULL)
        , m_pMatAnimArray(NULL)
        , m_MaxMaterialAnimCount(0)
        , m_MaxSubBindCount(0)
        , m_MaxTextureCount(0)
        , m_MaxCurveCount(0)
        , m_pSubBindIndexArray(NULL)
        , m_ppTextureArray(NULL)
        , m_pTextureDescriptorSlotArray(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 関連付け
    //@{

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel ResModel へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel ModelObj へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ModelObj* pModel) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel アニメーション対象の ResModel へのポインター。
    //!
    //! @pre
    //! - pModel と設定されている ResMaterialAnim のリソースが ResMaterialAnim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を ResMaterialAnim::PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    virtual void BindFast(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief 指定したインデックスのマテリアルにアニメーションフラグを設定します。
    //!
    //! @param[in] materialIndex マテリアルのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(int materialIndex, BindFlag flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        SetBindFlagImpl(materialIndex, flag);
    }

    //! @brief 指定したインデックスのマテリアルにアニメーションフラグを取得します。
    //!
    //! @param[in] materialIndex マテリアルのインデックス。
    //!
    //! @return フラグを返します。
    //!
    BindFlag GetBindFlag(int materialIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return GetBindFlagImpl(materialIndex);
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief 何も行いません。
    //!
    //! @details
    //! 0フレームで初期化する場合は、frame 0 で Calculate() し、ApplyTo() してください。
    //!
    virtual void ClearResult() NN_NOEXCEPT
    {
    }

    virtual void Calculate() NN_NOEXCEPT;
    virtual void ApplyTo(ModelObj* pModelObj) const NN_NOEXCEPT;

    //! @brief 自身のマテリアルアニメーションが書き換えるパラメーターをマテリアルリソースの値に戻します。
    //!
    //! @param[in] pModelObj パラメータを戻すモデルインスタンス。
    //!
    //! @pre
    //! - m_pRes != NULL
    //! - IsTargetBound()
    //! - pModelObj != NULL
    //!
    //! @details
    //! - モデルのマテリアルが持つパラメーターに対して複数のマテリアルアニメーションで変更を加えていた場合、
    //!   この関数を呼び出すとパラメーターがマテリアルリソースの値に上書きされてしまうので注意してください。
    //! - SetBindFlag() で Apply が無効化されているマテリアルは処理がスキップされます。
    //!
    void RevertTo(ModelObj* pModelObj) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return マテリアルアニメーションリソースへのポインターを返します。
    //!
    const ResMaterialAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes マテリアルアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するマテリアルパラメータアニメーションを差し替えます。
    //!
    void SetResource(const ResMaterialAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
        SetTargetUnbound();
    }

    //! @brief アニメーション対象に対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pModel 対象のモデルリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResModel* pModel) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(pModel != NULL, NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return pModel->GetMaterialCount() <= GetBindTable().GetSize();
    }

    //! @brief アニメーションリソースに対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pRes 対象のマテリアルアニメーションリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResMaterialAnim* pRes) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(pRes);
        return pRes->GetPerMaterialAnimCount() <= m_MaxMaterialAnimCount && pRes->GetParamAnimCount() <= m_MaxSubBindCount
            && pRes->GetTextureCount() <= m_MaxTextureCount && pRes->GetCurveCount() <= m_MaxCurveCount;
    }

    //! @brief アニメーションがもつテクスチャーの数を取得します。
    //!
    //! @return アニメーションがもつテクスチャーの数を返します。
    //!
    int GetTextureCount() const NN_NOEXCEPT
    {
        return m_MaxTextureCount;
    }

    //! @brief アニメーションが参照するテクスチャーを取得します。
    //!
    //! @param[in] idxTexture テクスチャーのインデックス。
    //!
    //! @return nn::g3d::TextureRef を返します。
    //!
    //! @pre
    //! -インデックスはアニメーションがもつテクスチャーの数の範囲内。
    //!
    TextureRef GetTexture(int idxTexture) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(idxTexture, 0, GetTextureCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return TextureRef(m_ppTextureArray[idxTexture], m_pTextureDescriptorSlotArray[idxTexture]);
    }

    //! @brief アニメーションが参照するテクスチャーを設定します。
    //!
    //! @param[in] idxTexture テクスチャーのインデックス。
    //! @param[in] textureRef 差し替えるテクスチャーのビューおよびディスクリプタスロットを格納した nn::g3d::TextureRef への参照。
    //!
    //! @pre
    //! -インデックスはアニメーションがもつテクスチャーの数の範囲内。
    //!
    //! @details
    //! アニメーションが参照するテクスチャーを差し替えることができます。
    //!
    void SetTexture(int idxTexture, TextureRef& textureRef) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(idxTexture, 0, GetTextureCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        m_ppTextureArray[idxTexture] = textureRef.GetTextureView();
        m_pTextureDescriptorSlotArray[idxTexture] = textureRef.GetDescriptorSlot();
    }

    //! @brief テクスチャーをリソースの状態に戻します。
    void ClearTexture() NN_NOEXCEPT
    {
        for (int idxTex = 0, texCount = m_pRes->GetTextureCount(); idxTex < texCount; ++idxTex)
        {
            m_ppTextureArray[idxTex] = m_pRes->GetTexture(idxTex);
            m_pTextureDescriptorSlotArray[idxTex] = m_pRes->GetTextureDescriptorSlot(idxTex);
        }
    }

    //! @brief インデックスからテクスチャーの名前を取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャー名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルアニメーションが持つテクスチャー数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetTextureName(int texIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return m_pRes->GetTextureName(texIndex);
    }

    //! @brief マテリアルアニメーションの数を取得します。
    //!
    //! @return マテリアルアニメーションの数を返します。
    //!
    int GetPerMaterialAnimCount() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return m_pRes->GetPerMaterialAnimCount();
    }

    //! @brief マテリアルアニメーション名を取得します。
    //!
    //! @return ResMaterialAnim のマテリアルアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return m_pRes->GetName();
    }

    //@}

protected:

    //! @briefprivate 単体のマテリアルアニメーション取得します。
    //!
    const ResPerMaterialAnim* GetPerMaterialAnim(int animIndex) const NN_NOEXCEPT
    {
        return &m_pMatAnimArray[animIndex];
    }

    //! @briefprivate マテリアルごとに関連付けを行う内部関数です。
    //!
    BindResult SubBind(const ResPerMaterialAnim* pMatAnim, const ResMaterial* pMaterial, int beginSubBindTableIndex) NN_NOEXCEPT;

    //! @briefprivate マテリアルごとに関連付けを行う内部関数です。
    //!
    BindResult SubBindFast(const ResPerMaterialAnim* pMatAnim, int beginSubBindTableIndex) NN_NOEXCEPT;

    //! @briefprivate マテリアルごとに適用を行う内部関数です。
    //!
    void ApplyTo(MaterialObj* pMaterialObj, const ResPerMaterialAnim* pMatAnim, int beginSubBindTableIndex) const NN_NOEXCEPT;

    //! @briefprivate マテリアルごとに書き換えたパラメータを元に戻す内部関数です。
    //!
    void RevertTo(MaterialObj* pMaterialObj, const ResPerMaterialAnim* pMatAnim, int beginSubBindTableIndex) const NN_NOEXCEPT;

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResMaterialAnim* m_pRes;
    const ResPerMaterialAnim* m_pMatAnimArray;
    int m_MaxMaterialAnimCount;
    int m_MaxSubBindCount;
    int m_MaxTextureCount;
    int m_MaxCurveCount;
    uint16_t* m_pSubBindIndexArray;
    const nn::gfx::TextureView** m_ppTextureArray;
    nn::gfx::DescriptorSlot* m_pTextureDescriptorSlotArray;

    NN_DISALLOW_COPY(MaterialAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate MaterialAnimObj::Initialize() に渡して初期化を行うパラメータです。
class MaterialAnimObj::InitializeArgument
{
public:
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_BindTable,
        MemoryBlockIndex_SubBindTable,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_TextureTable,
        MemoryBlockIndex_TextureDescriptorSlotTable,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! インスタンスで使用するすべてのマテリアルアニメーションリソースとすべてのモデルリソースに対して
    //! Reserve() するか、SetMaxMaterialCount()、SetMaxMaterialAnimCount()、SetMaxParamAnimCount()、
    //! SetMaxCurveCount() SetMaxTextureCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_MaterialCount = m_MaterialAnimCount = m_ParamAnimCount = m_CurveCount = m_TextureCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となる ResModel を使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResModel モデルリソースへのポインター。
    //!
    void Reserve(const ResModel* pResModel) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResModel);
        m_MaterialCount = std::max(m_MaterialCount, pResModel->GetMaterialCount());
    }

    //! @brief 対象となるマテリアルアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim マテリアルアニメーションリソースへのポインター。
    //!
    void Reserve(const ResMaterialAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
        m_MaterialAnimCount = std::max(m_MaterialAnimCount, pResAnim->GetPerMaterialAnimCount());
        m_ParamAnimCount = std::max(m_ParamAnimCount, pResAnim->GetParamAnimCount());
        m_CurveCount = std::max(m_CurveCount, pResAnim->GetCurveCount());
        m_TextureCount = std::max(m_TextureCount, pResAnim->GetTextureCount());
        m_ContextAvailable |= !pResAnim->IsCurveBaked();
    }

    //! @brief アニメーション高速化のためのコンテクストを有効にします。
    void SetContextEnabled() NN_NOEXCEPT
    {
        m_ContextEnabled = true;
    }

    //! @brief アニメーション高速化のためのコンテクストを無効にします。
    void SetContextDisabled() NN_NOEXCEPT
    {
        m_ContextEnabled = false;
    }

    //! @brief アニメーション高速化のためのコンテクストが有効かどうかを取得します。
    bool IsContextEnabled() const NN_NOEXCEPT
    {
        return m_ContextAvailable && m_ContextEnabled;
    }

    //! @brief 使用できる最大のマテリアル数を指定します。
    //!
    //! @param[in] matCount 使用できる最大のマテリアル数。
    //!
    void SetMaxMaterialCount(int matCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(matCount >= 0);
        m_MaterialCount = matCount;

    }

    //! @brief 使用できる最大のマテリアル数を取得します。
    //!
    //! @return 使用できる最大のマテリアル数を返します。
    //!
    int GetMaxMaterialCount() const NN_NOEXCEPT
    {
        return m_MaterialCount;
    }

    //! @brief 使用できる最大のマテリアルアニメーション数を指定します。
    //!
    //! @param[in] matAnimCount 使用できる最大のマテリアルアニメーション数。
    //!
    void SetMaxMaterialAnimCount(int matAnimCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(matAnimCount >= 0);
        m_MaterialAnimCount = matAnimCount;
    }

    //! @brief 使用できる最大のマテリアルアニメーション数を取得します。
    //!
    //! @return 使用できる最大のマテリアルアニメーション数を返します。
    //!
    int GetMaxMaterialAnimCount() const NN_NOEXCEPT
    {
        return m_MaterialAnimCount;
    }

    //! @brief 使用できる最大のパラメータアニメーション数を指定します。
    //!
    //! @param[in] paramAnimCount 使用できる最大のパラメータアニメーション数。
    //!
    void SetMaxParamAnimCount(int paramAnimCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(paramAnimCount >= 0);
        m_ParamAnimCount = paramAnimCount;
    }

    //! @brief 使用できる最大のパラメータアニメーション数を取得します。
    //!
    //! @return 使用できる最大のパラメータアニメーション数。
    //!
    int GetMaxParamAnimCount() const NN_NOEXCEPT
    {
        return m_ParamAnimCount;
    }

    //! @brief 使用できる最大のテクスチャー数を指定します。
    //!
    //! @param[in] textureCount 使用できる最大のテクスチャー数。
    //!
    void SetMaxTextureCount(int textureCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(textureCount >= 0);
        m_TextureCount = textureCount;
    }

    //! @brief 使用できる最大のテクスチャー数を取得します。
    //!
    //! @return 使用できる最大のテクスチャー数。
    //!
    int GetMaxTextureCount() const NN_NOEXCEPT
    {
        return m_TextureCount;
    }

    //! @brief 使用できる最大のカーブ数を指定します。
    //!
    //! @param[in] curveCount 使用できる最大のカーブ数。
    //!
    void SetMaxCurveCount(int curveCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(curveCount >= 0);
        m_CurveCount = curveCount;
        m_ContextAvailable = true;
    }

    //! @brief 使用できる最大のカーブ数を取得します。
    //!
    //! @return 使用できる最大のカーブ数を返します。
    //!
    int GetMaxCurveCount() const NN_NOEXCEPT
    {
        return m_CurveCount;
    }

    //! @brief 必要なパラメータが設定されているかどうかを取得します。
    //!
    //! @return 必要なパラメータが設定されている場合は true、設定されていない場合は false を返します。
    //!
    bool IsValid() const NN_NOEXCEPT
    {
        return m_MaterialCount >= 0 && m_MaterialAnimCount >= 0 && m_ParamAnimCount >= 0 && m_CurveCount >= 0 && m_TextureCount >= 0;
    }

    //! @brief MaterialAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  MaterialAnimObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief MaterialAnimObj 構築に必要なメモリサイズを計算します。
    void CalculateMemorySize() NN_NOEXCEPT;

    // インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief MaterialAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_MaterialCount;
    int m_MaterialAnimCount;
    int m_ParamAnimCount;
    int m_TextureCount;
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief MaterialAnimObj の構築を行うクラスです。
class MaterialAnimObj::Builder : public MaterialAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのマテリアルアニメーションリソースとすべてのモデルリソースに対して
    //! Reserve() するか、SetMaxMatCount()、SetMaxMatAnimCount()、SetMaxParamAnimCount()、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief MaterialAnimObj を構築します。
    //!
    //! @param[in] pMaterialAnimObj MaterialAnimObjへのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(MaterialAnimObj* pMaterialAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pMaterialAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}} // namespace nn::g3d

